/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiLayout.h"


////////////////
// Contraint:
nuiLayout::Constraint::Constraint(Node* pTargetNode, nuiLayoutRelation Relation, Node* pRefNode, double Multiplier, double Constant)
:
  mpTargetNode(pTargetNode),
  mpRefNode(pRefNode),
  mRelation(Relation),
  mMuliplier(Multiplier),
  mConstant(Constant)
{
  NGL_ASSERT(mpTargetNode != NULL);
  mpTargetNode->Acquire();
  if (mpRefNode)
    mpRefNode->Acquire();
}

nuiLayout::Constraint::~Constraint()
{
  mpTargetNode->Release();
  if (mpRefNode)
    mpRefNode->Release();
}


//////////////
// nuiLayout:
nuiLayout::nuiLayout()
{
}

nuiLayout::~nuiLayout()
{
  for (int32 i = 0; i < mConstraints.size(); i++)
  {
    delete mConstraints[i];
  }
}

bool nuiLayout::AddConstraint(nuiWidget* pWidget, nuiLayoutAttribute Attrib, nuiLayoutRelation Relation, nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, double Multiplier, double Constant)
{
  Node* pTarget = GetNode(pWidget, Attrib);
  Node* pRef = GetNode(pRefWidget, RefAttrib);
  Constraint* pConstraint = new Constraint(pTarget, Relation, pRef, Multiplier, Constant);
  
  mConstraints.push_back(pConstraint);
  
  return true;
}

nuiLayout::Node* nuiLayout::GetNode(nuiWidget* pWidget, nuiLayoutAttribute Attrib)
{
  Node node(pWidget, Attrib);
  NodeSet::iterator it = mNodes.find(&node);
  
  Node* pNode = NULL;
  if (it != mNodes.end())
    pNode = *it;
  else 
  {
    pNode = new Node(pWidget, Attrib);
    mNodes.insert(pNode);
  }
  return pNode;
}


