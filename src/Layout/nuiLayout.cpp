/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiLayout.h"

const char* nuiGetLayoutAttributeName(nuiLayoutAttribute attrib)
{
  switch (attrib)
  {
    case eLayoutAttribute_Left:     return "Left";
    case eLayoutAttribute_Right:    return "Right";
    case eLayoutAttribute_Top:      return "Top";
    case eLayoutAttribute_Bottom:   return "Bottom";
    case eLayoutAttribute_Width:    return "Width";
    case eLayoutAttribute_Height:   return "Height";
    case eLayoutAttribute_CenterX:  return "CenterX";
    case eLayoutAttribute_CenterY:  return "CenterY";
  }
  
  NGL_ASSERTR(0, "error");
}

const char* nuiGetLayoutRelationName(nuiLayoutRelation relation)
{
  switch (relation)
  {
    case eLayoutRelation_Equals:          return "Equals";
    case eLayoutRelation_LessThanOrEqual: return "LessThanOrEqual";
    case eLayoutRelation_MoreThanOrEqual: return "MoreThanOrEqual";
    case eLayoutRelation_Dependency:      return "Dependency";
  }
  
  NGL_ASSERTR(0, "error");
}

const char* nuiGetLayoutPriorityName(nuiLayoutPriority priority)
{
  switch (priority)
  {
    case eLayoutPriority_FromWidget:  return "FromWidget";
    case eLayoutPriority_Low:         return "Low";
    case eLayoutPriority_Normal:      return "Normal";
    case eLayoutPriority_High:        return "High";
    case eLayoutPriority_Highest:        return "Highest";
  }
  
  NGL_ASSERTR(0, "error");
}


////////////////
// Contraint:
nuiLayout::Constraint::Constraint(nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, nuiLayoutRelation Relation, double Multiplier, double Constant, nuiLayoutPriority Priority)
:
  mpRefWidget(pRefWidget),
  mRefAttrib(RefAttrib),
  mRelation(Relation),
  mMultiplier(Multiplier),
  mConstant(Constant),
  mPriority(Priority)
{
}

nuiLayout::Constraint::~Constraint()
{
}


//////////////
// nuiLayout:
nuiLayout::nuiLayout()
{
  if (SetObjectClass("nuiLayout"))
  {
    //
  }
}

nuiLayout::~nuiLayout()
{
}

bool nuiLayout::AddConstraint(nuiWidget* pWidget, nuiLayoutAttribute Attrib, nuiLayoutRelation Relation, nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, double Multiplier, double Constant, nuiLayoutPriority Priority)
{
  Widget* pW = &mWidgets[pWidget];
  //pW->mRefs++;
  
  Constraint constraint(pRefWidget, RefAttrib, Relation, Multiplier, Constant, Priority);
  
  LayoutValue* pValue = NULL;
  DependencySet* pDeps = NULL;

  int32 deps = 1;

  switch (Attrib)
  {
    case eLayoutAttribute_Left:
      pValue = & pW->mLeft;
      deps = 0;
      break;
    case eLayoutAttribute_Right:
      pValue = & pW->mRight;
      deps = 0;
      break;
    case eLayoutAttribute_Width:
      pValue = & pW->mWidth;
      deps = 0;
      break;
    case eLayoutAttribute_CenterX:
      pValue = & pW->mCenterX;
      deps = 0;
      break;
     
      
    case eLayoutAttribute_Top:
      pValue = & pW->mTop;
      break;
    case eLayoutAttribute_Bottom:
      pValue = & pW->mBottom;
      break;
    case eLayoutAttribute_Height:
      pValue = & pW->mHeight;
      break;
    case eLayoutAttribute_CenterY:
      pValue = & pW->mCenterY;
      break;
  }
  
  pDeps = & pW->mDeps[deps];
  pValue->mConstraints.push_back(constraint);
  
  if (pRefWidget)
  {
    pDeps->insert(pRefWidget);
    Widget* pR = &mWidgets[pRefWidget];
    pR->mRefs++;
    pR->mIncommingDeps[deps]++;
  }
  
  return true;
}

void nuiLayout::AddDependency(nuiWidget* pFromWidget, nuiWidget* pToWidget)
{
}


bool nuiLayout::DoLayout(const nuiRect& rRect)
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
  
  std::set<const Widget*> startnodes[2];
  std::list<const Widget*> nodes[2];
  {
    // Fill startnodes:
    Widgets::iterator it = mWidgets.begin();
    Widgets::iterator end = mWidgets.end();
    
    while (it != end)
    {
      Widget* pWidget = &it->second;

      // fill the list of start nodes:
      int32 inx = pWidget->mIncommingDeps[0];
      int32 iny = pWidget->mIncommingDeps[1];
      if (inx == 0)
        startnodes[0].insert(pWidget);

      if (iny == 0)
        startnodes[1].insert(pWidget);

      // reset layout:
      nuiRect r;
      nuiWidget* pW = it->first;
      if (pW != this)
        r = pW->GetIdealRect();
      else
        r = pW->GetRect();
      
      pW->LocalToLocal(this, r);
      pWidget->Reset(r); // Use the ideal rect as the reference for all widgets except this one
      
      ++it;
    }
  }
  
  for (int32 c = 0; c < 2; c++)
  {
    // Now start the actual topological sort:
    while (!startnodes[c].empty())
    {
      // remove a node pNode from the list of start nodes:
      std::set<const Widget*>::iterator it = startnodes[c].begin();
      const Widget* pNode = *it;
      startnodes[c].erase(it);
      
      // Insert this node into the node list:
      nodes[c].push_back(pNode);
      
      // for each node referenced by pNode:
      DependencySet::iterator deps_it = pNode->mDeps[c].begin();
      DependencySet::iterator deps_end = pNode->mDeps[c].end();
      while (deps_it != deps_end)
      {
        nuiWidget* pRef = *deps_it;
        Widget* pRefWidget = &mWidgets[pRef];
        
        pRefWidget->mVisits[c]++;
        if (pRefWidget->mVisits[c] == pRefWidget->mIncommingDeps[c])
          startnodes[c].insert(pRefWidget);
        
        ++deps_it;
      }
    }


    // Now we can calculate the layout, node by node, going from the one everyone depends on to the star nodes:
    nodes[c].reverse();
    
    std::list<const Widget*>::iterator it = nodes[c].begin();
    std::list<const Widget*>::iterator end = nodes[c].end();
    
    while (it != end)
    {
      const Widget* pNode(*it);
      NGL_OUT("Laying out '%s' [%p]\n", pNode->mpWidget->GetObjectName().GetChars(), pNode->mpWidget);
      
      for (DependencySet::const_iterator deps_it = pNode->mDeps[c].begin(); deps_it != pNode->mDeps[c].end(); ++deps_it)
      {
        // Evaluate constraints:
        
        if (c == 0)
        {
          // Horizontal
          EvaluateConstraints(pNode->mLeft);
          EvaluateConstraints(pNode->mRight);
          EvaluateConstraints(pNode->mWidth);
          EvaluateConstraints(pNode->mCenterX);
        }
        else
        {
          // Vertical
          EvaluateConstraints(pNode->mTop);
          EvaluateConstraints(pNode->mBottom);
          EvaluateConstraints(pNode->mHeight);
          EvaluateConstraints(pNode->mCenterY);
        }
      }
      
      ++it;
    }
    
  }

  bool res = true;
//  if (!edges.empty())
//  {
//    // There is a cycle in the graph!
//    NGL_OUT("\n\nERROR: There is a cycle in the graph!\n\n");
//    res = false;
//  }
  

  
  //// We're almost there!
  // Now we have the constraints all set, let's put them back into the widgets:
  {
    Widgets::iterator it  = mWidgets.begin();
    Widgets::iterator end = mWidgets.end();
    
    while (it != end)
    {
      nuiWidget* pWidget = it->first;
      if (pWidget != this)
      {
        Widget& rWidget = it->second;
        
        nuiRect r(pWidget->GetIdealRect());
        
        // Width / Height:
        if (rWidget.mWidth.mSet)
          r.SetWidth(rWidget.mWidth.mValue);
        
        if (rWidget.mHeight.mSet)
          r.SetHeight(rWidget.mHeight.mValue);

        // Left / Right:
        if (rWidget.mLeft.mSet)
        {
          if (rWidget.mRight.mSet)
            r.Left() = rWidget.mLeft.mValue;
          else
            r.MoveTo(rWidget.mLeft.mValue, r.Top());
        }
        
        if (rWidget.mRight.mSet)
        {
          if (rWidget.mLeft.mSet)
            r.Right() = rWidget.mRight.mValue;
          else
            r.Move(rWidget.mRight.mValue - r.GetWidth(), r.Top());
        }
        
        // Top / Bottom:
        if (rWidget.mTop.mSet)
        {
          if (rWidget.mBottom.mSet)
            r.Top() = rWidget.mTop.mValue;
          else
            r.MoveTo(r.Left(), rWidget.mTop.mValue);
        }
        
        if (rWidget.mBottom.mSet)
        {
          if (rWidget.mTop.mSet)
            r.Bottom() = rWidget.mBottom.mValue;
          else
            r.Move(r.Left(), rWidget.mBottom.mValue - r.GetHeight());
        }
        
        // Center X & Y:
        if (rWidget.mCenterX.mSet)
          r.MoveTo(rWidget.mCenterX.mValue - r.GetWidth() * .5, r.Top());

        if (rWidget.mCenterY.mSet)
          r.MoveTo(r.Left(), rWidget.mCenterY.mValue - r.GetHeight() * .5);
        
        pWidget->SetLayout(r);
      }
      
      ++it;
    }
    
  }
  
  
  return res;
}

void nuiLayout::EvaluateConstraints(LayoutValue& rValue)
{
  // Apply all constraints that go from this node:
  for (int32 i = 0; i < rValue.mConstraints.size(); i++)
  {
    Constraint& rConstraint(rValue.mConstraints[i]);
    
    double val = 0;
    if (rConstraint.mpRefWidget)
    {
      NGL_OUT("\twith RefWidget = %s (%s)\n", rConstraint.mpRefWidget->GetObjectName().GetChars(), nuiGetLayoutAttributeName(rConstraint.mRefAttrib));
      
      Widget& W(mWidgets[rConstraint.mpRefWidget]);
      LayoutValue& rRefValue(W.GetAttrib(rConstraint.mRefAttrib));
      
      val = rRefValue.mValue;
    }
    NGL_OUT("\tConstraint[%d] %s (%f * %f + %f)\n", i, nuiGetLayoutRelationName(rConstraint.mRelation), val, rConstraint.mMultiplier, rConstraint.mConstant);
    
    val = val * rConstraint.mMultiplier + rConstraint.mConstant;
    
    switch (rConstraint.mRelation)
    {
      case eLayoutRelation_Equals:
        if (rValue.mPriorityMin <= rConstraint.mPriority)
        {
          rValue.mMin = val;
          rValue.mPriorityMin = rConstraint.mPriority;
        }
        
        if (rValue.mPriorityMax <= rConstraint.mPriority)
        {
          rValue.mMax = val;
          rValue.mPriorityMax = rConstraint.mPriority;
        }
        break;
      case eLayoutRelation_LessThanOrEqual:
        if (rValue.mPriorityMax <= rConstraint.mPriority)
        {
          rValue.mMax = val;
          rValue.mPriorityMax = rConstraint.mPriority;
        }
        break;
      case eLayoutRelation_MoreThanOrEqual:
        if (rValue.mPriorityMin <= rConstraint.mPriority)
        {
          rValue.mMin = val;
          rValue.mPriorityMin = rConstraint.mPriority;
        }
        break;
    }
    
    rValue.mValue = nuiClamp(rValue.mValue, rValue.mMin, rValue.mMax);
    rValue.mSet = true;
  }
}

bool nuiLayout::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  NGL_OUT("nuiLayout::SetRect(%s)\n", rRect.GetValue().GetChars());
  
  nuiRect r(rRect.Size());
  DoLayout(r);
  
  return true;
}


