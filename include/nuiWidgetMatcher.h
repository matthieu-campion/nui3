/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

#define NUI_WIDGET_MATCHER_PARENT 0
#define NUI_WIDGET_MATCHER_PARENTCONDITION 0
#define NUI_WIDGET_MATCHER_JOKER 0
#define NUI_WIDGET_MATCHER_STATIC 0
#define NUI_WIDGET_MATCHER_CLASS 2
#define NUI_WIDGET_MATCHER_NAME 3
#define NUI_WIDGET_MATCHER_STATE 4
#define NUI_WIDGET_MATCHER_ATTRIBUTE 5

#define NUI_WIDGET_MATCHTAG_NONE 0
#define NUI_WIDGET_MATCHTAG_BLOCK 0
#define NUI_WIDGET_MATCHTAG_ALL ((uint32)-1)
#define NUI_WIDGET_MATCHTAG_PARENT              (1 < NUI_WIDGET_MATCHER_PARENT)
#define NUI_WIDGET_MATCHTAG_PARENTCONDITION     (1 < NUI_WIDGET_MATCHER_PARENTCONDITION)
#define NUI_WIDGET_MATCHTAG_JOKER               (1 < NUI_WIDGET_MATCHER_JOKER)
#define NUI_WIDGET_MATCHTAG_STATE               (1 < NUI_WIDGET_MATCHER_STATE)
#define NUI_WIDGET_MATCHTAG_CLASS               (1 < NUI_WIDGET_MATCHER_CLASS)
#define NUI_WIDGET_MATCHTAG_NAME                (1 < NUI_WIDGET_MATCHER_NAME)
#define NUI_WIDGET_MATCHTAG_ATTRIBUTE           (1 < NUI_WIDGET_MATCHER_ATTRIBUTE)

/**
 This is a very simple parent class for all widget matchers: there is only one virtual method (Match) that returns true if
 the argument matches the given widget
 **/
class nuiWidgetMatcher
{
public:
  nuiWidgetMatcher(bool StateMatcher)
  : mStateMatcher(StateMatcher)
  {
    mPriority = 0; // By default we don't permit to skip and optimize any matcher, just to be safe
  }
  
  virtual ~nuiWidgetMatcher()
  {
  }
  
  virtual bool Match(nuiWidget*& pWidget) = 0;
  
  bool IsStateMatcher() const
  {
    return mStateMatcher;
  }
  
  int32 GetPriority() const
  {
    return mPriority;
  }

  uint32 GetTag() const
  {
    return 1 << mPriority;
  }
protected:
  bool mStateMatcher;
  int32 mPriority;
};

class nuiWidgetJokerMatcher : public nuiWidgetMatcher // Match all the widgets
{
public:
  nuiWidgetJokerMatcher()
  : nuiWidgetMatcher(false)
  {
    mPriority = NUI_WIDGET_MATCHER_JOKER;
  }
  
  bool Match(nuiWidget*& pWidget)
  {
    return true;
  }
};

class nuiWidgetParentMatcher : public nuiWidgetMatcher // Change the reference widget to its parent
{
public:
  nuiWidgetParentMatcher()
  : nuiWidgetMatcher(false)
  {
    mPriority = NUI_WIDGET_MATCHER_PARENT;
  }
  
  bool Match(nuiWidget*& pWidget)
  {
    pWidget = pWidget->GetParent();
    return pWidget != NULL;
  }
};

class nuiWidgetParentConditionMatcher : public nuiWidgetMatcher // Change the reference widget to its parent
{
public:
  nuiWidgetParentConditionMatcher(nuiWidgetMatcher* pCondition)
  : nuiWidgetMatcher(false)
  {
    mPriority = NUI_WIDGET_MATCHER_PARENTCONDITION;
    mpCondition.push_back(pCondition);
  }
  
  nuiWidgetParentConditionMatcher(const std::vector<nuiWidgetMatcher*> rMatchers)
  : nuiWidgetMatcher(false),
    mpCondition(rMatchers)
  {
    mPriority = NUI_WIDGET_MATCHER_PARENTCONDITION;
  }

  bool Match(nuiWidget*& pWidget)
  {
    int32 i = (int32)mpCondition.size() - 1;
    while (i >= 0)
    {
      if (!MatchOne(mpCondition[i], pWidget))
        return false;
      i--;
    }
    
    return true;
  }
  
protected:
  bool MatchOne(nuiWidgetMatcher* pCondition, nuiWidget*& pWidget)
  {    
    while (pWidget)
    {
      if (pCondition->Match(pWidget))
        return true;
      
      pWidget = pWidget->GetParent();
    }
    
    pWidget = NULL;
    return false;
  }
  
  std::vector<nuiWidgetMatcher*> mpCondition;
};

class nuiWidgetClassMatcher : public nuiWidgetMatcher
{
public:
  nuiWidgetClassMatcher(const nglString& rClass)
  : nuiWidgetMatcher(false), mClass(rClass)
  {
    mPriority = NUI_WIDGET_MATCHER_CLASS;
    mClassIndex = nuiObject::GetClassNameIndex(rClass);
  }
  
  virtual bool Match(nuiWidget*& pWidget)
  {
    return pWidget->IsOfClass(mClassIndex);
  }
  
protected:
  nglString mClass;
  uint32 mClassIndex;
};

class nuiWidgetNameMatcher : public nuiWidgetMatcher
{
public:
  nuiWidgetNameMatcher(const nglString& rName)
  : nuiWidgetMatcher(false), mName(rName)
  {
    mPriority = NUI_WIDGET_MATCHER_NAME;
  }
  
  virtual bool Match(nuiWidget*& pWidget)
  {
    return pWidget->GetObjectName() == mName;
  }
  
protected:
  nglString mName;
};

class nuiWidgetAttributeMatcher : public nuiWidgetMatcher
  {
  public:
    nuiWidgetAttributeMatcher(const nglString& rAttribute, const nglString& rValue, bool CaseSensitive, bool PartialMatch)
    : nuiWidgetMatcher(true), mAttribute(rAttribute), mValue(rValue), mCaseSensitive(CaseSensitive), mPartialMatch(PartialMatch)
    {
      mPriority = NUI_WIDGET_MATCHER_ATTRIBUTE;
    }
    
    virtual bool Match(nuiWidget*& pWidget)
    {
      const nuiAttribBase Attribute = pWidget->GetAttribute(mAttribute);
      if (!Attribute.IsValid())
        return false;
      
      nglString value;
      Attribute.ToString(value);
      if (value.IsNull())
        return false;
      
      if (mPartialMatch)
      {
        return value.Contains(mValue, mCaseSensitive) != 0;
      }
      
      return value.Compare(mValue, mCaseSensitive) == 0;
    }
    
  protected:
    nglString mAttribute;
    nglString mValue;
    bool mCaseSensitive;
    bool mPartialMatch;
  };


class nuiWidgetStateMatcher : public nuiWidgetMatcher
{
public:
  enum StateKinds
  {
    eEnabled,
    eVisible,
    eSelected,
    eHovered,
    eFocussed,
    eGrabbed,
    eDebugged
  };
  
  nuiWidgetStateMatcher(StateKinds states, bool State)
  : nuiWidgetMatcher(true), mState(states), mStateValue(State)
  {
    mPriority = NUI_WIDGET_MATCHER_STATE;
  }
  
  virtual bool Match(nuiWidget*& pWidget)
  {
    return false;
  }
  
protected:
  StateKinds mState;
  bool mStateValue;
  bool mCaseSensitive;
  bool mPartialMatch;
};


class nuiWidgetStaticMatcher : public nuiWidgetMatcher // Match all the widgets that have never been thru the CSS engine
{
public:
  nuiWidgetStaticMatcher()
  : nuiWidgetMatcher(false)
  {
    mPriority = NUI_WIDGET_MATCHER_STATIC;
  }
  
  bool Match(nuiWidget*& pWidget)
  {
    return pWidget->GetCSSPass() == 0;
  }
};

class nuiGlobalVariableMatcher : public nuiWidgetMatcher // Match if the global variable has the given value
{
public:
  nuiGlobalVariableMatcher(const nglString& rVariable, const nglString& rValue)
  : nuiWidgetMatcher(false),
    mVariable(rVariable),
    mValue(rValue)
  {
    mPriority = NUI_WIDGET_MATCHER_STATE;
  }
  
  bool Match(nuiWidget*& pWidget)
  {
    return nuiObject::GetGlobalProperty(mVariable) == mValue;
  }
  
private:
  nglString mVariable;
  nglString mValue;
};





