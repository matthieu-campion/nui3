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

enum nuiLayoutPriority
{
  eLayoutPriority_FromWidget,
  eLayoutPriority_Low,
  eLayoutPriority_Normal,
  eLayoutPriority_High,
  eLayoutPriority_Highest
};


class nuiLayout : public nuiSimpleContainer
{
public:
  nuiLayout();
  virtual ~nuiLayout();
  
  bool AddConstraint(nuiWidget* pWidget, nuiLayoutAttribute Attrib, nuiLayoutRelation Relation, nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, double Multiplier, double Constant, nuiLayoutPriority Priority = eLayoutPriority_Highest);
  
  bool SetRect(const nuiRect& rRect);
  
private:  
  class Node;
  class Constraint
  {
  public:
    Constraint(nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, nuiLayoutRelation Relation, double Multiplier, double Constant, nuiLayoutPriority Priority);
    ~Constraint();

    nuiWidget* mpRefWidget;
    nuiLayoutAttribute mRefAttrib;
    nuiLayoutRelation mRelation;
    double mMuliplier;
    double mConstant;
    nuiLayoutPriority mPriority;
  };

  class LayoutValue
  {
  public:
    LayoutValue()
    : mValue(0),
      mMin(std::numeric_limits<double>::min()),
      mMax(std::numeric_limits<double>::max()),
      mPriorityMin(eLayoutPriority_FromWidget),
      mPriorityMax(eLayoutPriority_FromWidget),
      mSet(false)
    {
    }
    
    void Reset(double value)
    {
      mValue = value;
      mSet = false;
      mPriorityMin = eLayoutPriority_FromWidget;
      mPriorityMax = eLayoutPriority_FromWidget;
      mMin = std::numeric_limits<double>::min();
      mMax = std::numeric_limits<double>::max();
    }

    double mValue;
    double mMin;
    double mMax;
    nuiLayoutPriority mPriorityMin;
    nuiLayoutPriority mPriorityMax;
    bool mSet;
  };

  class WidgetLayout
  {
  public:
    void Reset(const nuiRect& r)
    {
      mWidth.Reset(r.GetWidth());
      mHeight.Reset(r.GetHeight());
      mTop.Reset(r.Top());
      mLeft.Reset(r.Left());
      mBottom.Reset(r.Bottom());
      mRight.Reset(r.Right());
      mCenterX.Reset(r.Left() + r.GetWidth() * .5);
      mCenterY.Reset(r.Top() + r.GetHeight() * .5);
    }
    
    LayoutValue& GetAttrib(nuiLayoutAttribute attrib)
    {
      switch (attrib)
      {
        case eLayoutAttribute_Left:     return mLeft;
        case eLayoutAttribute_Right:    return mRight;
        case eLayoutAttribute_Top:      return mTop;
        case eLayoutAttribute_Bottom:   return mBottom;
        case eLayoutAttribute_Width:    return mWidth;
        case eLayoutAttribute_Height:   return mHeight;
        case eLayoutAttribute_CenterX:  return mCenterX;
        case eLayoutAttribute_CenterY:  return mCenterY;
      }
      
      NGL_ASSERTR(0, *(LayoutValue*)NULL);
    }
    
    mutable LayoutValue mWidth;
    mutable LayoutValue mHeight;
    mutable LayoutValue mTop;
    mutable LayoutValue mLeft;
    mutable LayoutValue mBottom;
    mutable LayoutValue mRight;
    mutable LayoutValue mCenterX;
    mutable LayoutValue mCenterY;
    
    int32 mRefs;
  };
  
  class Node
  {
  public:
    Node(nuiWidget* pWidget, nuiLayoutAttribute Attrib, WidgetLayout* pLayout)
    {
      mpWidget = pWidget;
      mAttrib = Attrib;
      mpLayout = pLayout;
      mIncommingConstraints = 0;
      mVisits = 0;
    }
        
    nuiWidget* mpWidget;
    nuiLayoutAttribute mAttrib;
    
    mutable int32 mIncommingConstraints;
    mutable int32 mVisits;
    
    WidgetLayout* mpLayout;
  };
  
  struct less_node
  {
    bool operator()(const Node& rLeft, const Node& rRight) const
    {
      if (rLeft.mpWidget < rRight.mpWidget)
        return true;
      if (rLeft.mpWidget > rRight.mpWidget)
        return false;
      return (rLeft.mAttrib  < rRight.mAttrib);
    }
  };

  typedef std::vector<Constraint> ConstraintList;
  typedef std::map< Node, ConstraintList, less_node > ConstraintMap;
  ConstraintMap mConstraints;

  typedef std::map<nuiWidget*, WidgetLayout> WidgetLayouts;
  WidgetLayouts mLayouts;
  
  bool DoLayout();
};

