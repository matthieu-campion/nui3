/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiLayout.h"


////////////////
// Contraint:
nuiLayout::Constraint::Constraint(nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, nuiLayoutRelation Relation, double Multiplier, double Constant)
:
  mpRefWidget(pRefWidget),
  mRefAttrib(RefAttrib),
  mRelation(Relation),
  mMuliplier(Multiplier),
  mConstant(Constant)
{
}

nuiLayout::Constraint::~Constraint()
{
}


//////////////
// nuiLayout:
nuiLayout::nuiLayout()
{
}

nuiLayout::~nuiLayout()
{
}

bool nuiLayout::AddConstraint(nuiWidget* pWidget, nuiLayoutAttribute Attrib, nuiLayoutRelation Relation, nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, double Multiplier, double Constant)
{
  WidgetLayout* pLayout = &mLayouts[pWidget];
  pLayout->mRefs++;
  
  Node Target(pWidget, Attrib, pLayout);
  
  Constraint constraint(pRefWidget, RefAttrib, Relation, Multiplier, Constant);
  
  mConstraints[Target].push_back(constraint);

  if (pRefWidget)
  {
    Node Ref(pRefWidget, RefAttrib, NULL);
    ConstraintMap::iterator it = mConstraints.find(Ref);
    
    if (it != mConstraints.end())
    {
      const Node& rNode(it->first);
      rNode.mIncommingConstraints++;
    }

  }
  return true;
}

bool nuiLayout::DoLayout()
{
  // Topological sort:
  /* Algorithm from wikipedia:
  L ← Empty list that will contain the sorted elements
  S ← Set of all nodes with no incoming edges
  while S is non-empty do
    remove a node n from S
    insert n into L
    for each node m with an edge e from n to m do
      remove edge e from the graph
      if m has no other incoming edges then
        insert m into S
  
  if graph has edges then
    output error message (graph has at least one cycle)
  else 
    output message (proposed topologically sorted order: L)
   */
  
  std::list<const Node*> nodes;
  std::set<const Node*> startnodes;
  std::set<const Constraint*> edges;
  
  {
    // Fill startnodes:
    ConstraintMap::iterator it = mConstraints.begin();
    ConstraintMap::iterator end = mConstraints.end();
    
    while (it != end)
    {
      const Node* pNode = &it->first;
      pNode->mVisits = 0; // Reset the visit count
      
      // fill the list of start nodes:
      if (pNode->mIncommingConstraints == 0)
      {
        startnodes.insert(pNode);
      }
      
      // Add all edges to the edge list
      ConstraintList& rList(it->second);
      for (ConstraintList::iterator i = rList.begin(); i != rList.end(); ++i)
        edges.insert(&*i);
      
      ++it;
    }
  }
  
  // Now start the actual topological sort:
  while (!startnodes.empty())
  {
    // remove a node pNode from the list of start nodes:
    std::set<const Node*>::iterator it = startnodes.begin();
    const Node* pNode = *it;
    startnodes.erase(it);
    
    // Insert this node into the node list:
    nodes.push_back(pNode);
    
    // for each node referenced by pNode:
    ConstraintMap::iterator constraint_it = mConstraints.find(*pNode);
    NGL_ASSERT(constraint_it != mConstraints.end());
    ConstraintList& constraints(constraint_it->second);
    for (ConstraintList::iterator cit = constraints.begin(); cit != constraints.end(); ++cit)
    {
      const Constraint& rConstraint(*cit);
      ConstraintMap::iterator refit = mConstraints.find(Node(rConstraint.mpRefWidget, rConstraint.mRefAttrib, NULL));
      NGL_ASSERT(refit != mConstraints.end());
      
      const Node* pRefNode = &refit->first;
      pRefNode->mVisits++;
      edges.erase(&rConstraint);
      
      // if pRefNode has no incoming edges, move it to the startnodes list
      if (pRefNode->mVisits == pRefNode->mIncommingConstraints)
        startnodes.insert(pRefNode);
    }
  }

  // Reset widget layout flags:
  {
    WidgetLayouts::iterator it = mLayouts.begin();
    while (it != mLayouts.end())
    {
      WidgetLayout& rLayout(it->second);
      rLayout.Reset(it->first);
      
      ++it;
    }
  }


  // Now we can calculate the layout, node by node, going from the one everyone depends on to the star nodes:
  nodes.reverse();
  
  std::list<const Node*>::iterator it = nodes.begin();
  std::list<const Node*>::iterator end = nodes.end();
  
  while (it != end)
  {
    const Node* pNode(*it);
    LayoutValue& rValue(pNode->mpLayout->GetAttrib(pNode->mAttrib));
    ConstraintMap::iterator cit = mConstraints.find(*pNode);
    NGL_ASSERT(cit != mConstraints.end());

    ConstraintList& rContraints(cit->second);

    // Apply all constraints that go from this node:
    for (int32 i = 0; i < rContraints.size(); i++)
    {
      Constraint& rConstraint(rContraints[i]);

      double val;
      if (rConstraint.mpRefWidget)
      {
        WidgetLayouts::iterator wit = mLayouts.find(rConstraint.mpRefWidget);
        NGL_ASSERT(wit != mLayouts.end());
        
        WidgetLayout& rRefLayout(wit->second);
        LayoutValue& rRefValue(rRefLayout.GetAttrib(rConstraint.mRefAttrib));

        val = rRefValue.mValue;
      }

      val = val * rConstraint.mMuliplier + rConstraint.mConstant;

      switch (rConstraint.mRelation)
      {
        case eLayoutRelation_Equals:
          rValue.mMin = rValue.mMax = val;
          break;
        case eLayoutRelation_LessThanOrEqual:
          rValue.mMax = val;
          break;
        case eLayoutRelation_MoreThanOrEqual:
          rValue.mMin = val;
          break;
      }
      
      rValue.mValue = nuiClamp(rValue.mValue, rValue.mMin, rValue.mMax);
    }

    ++it;
  }
  
  if (!edges.empty())
  {
    // There is a cycle in the graph!
    return false;
  }

  //// We're almost there!
  // Now we have the constraints all set, let's put them back into the widgets:
  
  return true;
}



