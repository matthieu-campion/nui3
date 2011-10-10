/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

enum nuiLayoutAttribute
{
  eLayoutAttribute_Left = 1 << 0,
  eLayoutAttribute_Right = 1 << 1,
  eLayoutAttribute_Top = 1 << 2,
  eLayoutAttribute_Bottom = 1 << 3,
  eLayoutAttribute_Width = 1 << 4,
  eLayoutAttribute_Height = 1 << 5,
  eLayoutAttribute_CenterX = 1 << 6,
  eLayoutAttribute_CenterY = 1 << 7
  //baseline

};

enum nuiLayoutRelation
{
  eLayoutRelation_Equals,
  eLayoutRelation_LessThanOrEqual,
  eLayoutRelation_MoreThanOrEqual
};


class nuiLayout : public nuiSimpleContainer
{
public:
  nuiLayout();
  virtual ~nuiLayout();
  
  bool AddConstraint(nuiWidget* pWidget, nuiLayoutAttribute Attrib, nuiLayoutRelation Relation, nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, double Multiplier, double Constant);
  
private:  
  class Node : public nuiRefCount
  {
  public:
    Node(nuiWidget* pWidget, nuiLayoutAttribute Attrib)
    {
      pWidget->Acquire();
    }
    
    ~Node()
    {
      if (mpWidget)
        mpWidget->Release();
    }
    
    nuiWidget* mpWidget;
    nuiLayoutAttribute mAttrib;
  };
  
  class Constraint
  {
  public:
    Constraint(Node* pTargetNode, nuiLayoutRelation Relation, Node* pRefNode, double Multiplier, double Constant);
    ~Constraint();

    Node* mpTargetNode;
    Node* mpRefNode;
    nuiLayoutRelation mRelation;
    double mMuliplier;
    double mConstant;
  };

  struct less_node
  {
    bool operator()(Node* pLeftNode, Node* pRightNode) const
    {
      return pLeftNode->mpWidget < pRightNode->mpWidget
          && pLeftNode->mAttrib < pRightNode->mAttrib;
    }
  };

  typedef std::set<Node*, less_node> NodeSet;
  NodeSet mNodes;
  std::vector<Constraint*> mConstraints;
  
  Node* GetNode(nuiWidget* pWidget, nuiLayoutAttribute Attrib);
};

