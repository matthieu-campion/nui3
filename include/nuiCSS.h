/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiWidgetMatcher.h"

class nuiCSSAction
{
public:
  nuiCSSAction();
  virtual ~nuiCSSAction();
  
  virtual void ApplyAction(nuiObject* pObject) = 0;
};

class nuiCSSAction_SetAttribute : public nuiCSSAction
{
public:
  nuiCSSAction_SetAttribute(const nglString& rAttribute, const nglString& rValue, int32 i0, int32 i1);
  virtual ~nuiCSSAction_SetAttribute();
  
  void ApplyAction(nuiObject* pObject);
  
private:
  nglString mAttribute;
  nglString mValue;
  bool mValueIsGlobal;
  int32 mIndex0;
  int32 mIndex1;
};

class nuiCSSRule : public nuiCSSAction
{
public:
  nuiCSSRule();
  virtual ~nuiCSSRule();
  
  void AddMatcher(nuiWidgetMatcher* pMatcher);
  void AddAction(nuiCSSAction* pAction);
  bool Match(nuiWidget* pWidget, uint32 MatchersMask); /// Returns true if the widget is matched by this rule
  
  virtual void ApplyRule(nuiWidget* pWidget, uint32 MatchersTag);
  
  virtual void ApplyAction(nuiObject* pObject);
  
  uint32 GetMatchersTag() const;
private:
  std::vector<nuiWidgetMatcher*> mMatchers;
  std::vector<nuiCSSAction*> mActions;
  
  uint32 mMatchersTag;
};

/**! Class nuiCSS
  Here are some examples of valid CSS declarations:

  // Include another CSS source file:
  #include "definitions.css"

  // create two color:
  @color my_color_name = rgb(255, 0, 0, 255); // opaque red
  @nuiColor my_other_color_name = rgb(0, 0, 255, 255); // opaque blue

  // create a font:
  @Font MsgBoxTitleFont
  {
    GenericName : sans-serif;
    Size : 16;
    Bold : true;
    Italic : false;
  }

  // create a gradiant decoration:
  @nuiGradientDecoration dTrackHeaderOff
  {
    Color1: Clr1;
    Color2: Clr2;
    Color3: Clr3;
    Color4: Clr4;
    Offset1: 0;
    Offset2: 0.15;
    Offset3: 0.75;
    Offset4: 1;

    StrokeColor: ClrStroke;
    StrokeSize: 1;

    ShapeMode: StrokeAndFill;
  }

  // create and init some global variables:
  @var variable_name = bidule;
  @var test = false;

  //////////////// Match widgets:
  * {} // Match any widget
  nuiLabel {} // all the nuiLabel and widgets the inherit from nuiLabel
  "bidule" {} // match the widgets name "bidule"
  (nuiPane) {} // match the widgets that have a nuiPane in their widget tree branch.
  nuiPane.* {} // match the widgets that are directly inside an nuiPane
  nuiLabel[type=normal] {}  // match the nuiLabels that have a property named "type" with value "normal"
  nuiLabel[Hover:true] {}  // match the nuiLabels that are in Hover state ( = is for properties, : is for attributes)
  nuiLabel $GlobalVar = "wow" {}  // match all nuiLabel if the global variable GlobalVar equals "wow"
  nuiLabel[$GlobalVar = "wow"} {} // match all nuiLabel if the global variable GlobalVar equals "wow" (same as precedent example)
  nuiPane! {} // match all nuiPanes only if it is the first time we are trying to match them. All subsequent times will fail the matching operation.

  ////////// Actions:
  SomeMatcher
  {
    MyProperty  = my_value;
    MyProperty2 = "my value";
    MyAttribute  : 12;
    MyAttribute2 : "some sentence";
    MyAttribute3 : true;
  }
 
*/

class nuiCSS
{
public:
  nuiCSS(); 
  virtual ~nuiCSS();
  
  bool Load(nglIStream& rStream, const nglPath& rSourcePath = nglPath(nglString::Null));
  bool Serialize(nglOStream& rStream);
  
  void AddRule(nuiCSSRule* pRule);
  void ApplyRules(nuiWidget* pWidget, uint32 MatchersMask);
  bool GetMatchingRules(nuiWidget* pWidget, std::vector<nuiCSSRule*>& rMatchingRules, uint32 MatchersMask); /// Returns true if the widget is matched by this rule

  virtual nuiObject* CreateObject(const nglString& rType, const nglString& rName);
  static nuiObject* CreateObject(const nglString& rDesc);
  
  const nglString& GetErrorString() const;

  uint32 GetRulesCount() const;
  const std::vector<nuiCSSRule*> GetRules() const;
private:

  std::vector<nuiCSSRule*> mRules;
  nglString mErrorString;

};
