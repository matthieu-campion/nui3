/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

enum nuiLayoutAttribute
{
  eLayoutAttribute_Left,
  eLayoutAttribute_Right,
  eLayoutAttribute_Width,
  eLayoutAttribute_CenterX,

  eLayoutAttribute_Top,
  eLayoutAttribute_Bottom,
  eLayoutAttribute_Height,
  eLayoutAttribute_CenterY
  //baseline
};

enum nuiLayoutRelation
{
  eLayoutRelation_Equals,
  eLayoutRelation_LessThanOrEqual,
  eLayoutRelation_MoreThanOrEqual,
  eLayoutRelation_Dependency
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
  class Constraint
  {
  public:
    Constraint(nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, nuiLayoutRelation Relation, double Multiplier, double Constant, nuiLayoutPriority Priority);
    ~Constraint();

    nuiWidget* mpRefWidget;
    nuiLayoutAttribute mRefAttrib;
    nuiLayoutRelation mRelation;
    double mMultiplier;
    double mConstant;
    nuiLayoutPriority mPriority;
  };


  typedef std::vector<Constraint> ConstraintVector;
  typedef std::set<nuiWidget*> DependencySet;
  
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
      mPriorityMin = eLayoutPriority_FromWidget;
      mPriorityMax = eLayoutPriority_FromWidget;
      mMin = std::numeric_limits<double>::min();
      mMax = std::numeric_limits<double>::max();
      mSet = false;
    }

    double mValue;
    double mMin;
    double mMax;
    nuiLayoutPriority mPriorityMin;
    nuiLayoutPriority mPriorityMax;
    bool mSet;
    
    ConstraintVector mConstraints;
  };

  class Widget
  {
  public:
    Widget(nuiWidget* pWidget = NULL)
    : mRefs(0),
      mpWidget(pWidget)
    {
      mIncommingDeps[0] = 0;
      mIncommingDeps[1] = 0;
      mVisits[0] = 0;
      mVisits[1] = 0;
    }
    
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
      mVisits[0] = 0;
      mVisits[1] = 0;
      mRect = r;
    }
    
    LayoutValue& GetAttrib(nuiLayoutAttribute attrib)
    {
      switch (attrib)
      {
        case eLayoutAttribute_Left:     return mLeft;
        case eLayoutAttribute_Right:    return mRight;
        case eLayoutAttribute_Width:    return mWidth;
        case eLayoutAttribute_CenterX:  return mCenterX;

        case eLayoutAttribute_Top:      return mTop;
        case eLayoutAttribute_Bottom:   return mBottom;
        case eLayoutAttribute_Height:   return mHeight;
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
    
    DependencySet mDeps[2];
    
    int32 mIncommingDeps[2];
    int32 mVisits[2];
    
    nuiWidget* mpWidget;
    
    nuiRect mRect;
  };
  
  typedef std::map<nuiWidget*, Widget> Widgets;
  Widgets mWidgets;
  
  bool DoLayout(const nuiRect& rRect);

  void AddDependency(nuiWidget* pFromWidget, nuiWidget* pToWidget);
  
  void EvaluateConstraints(LayoutValue& rValue);
};

