/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"


// list of attributes that can be used to set a nuiStateDecoration with bitmap files or decoration reference names
// OnUp
// OnDown
// OnUpHover
// OnDownHover
// OnSelected (if the widget is not a button. if it's a button, use OnDown instead)
// OnDisabled
// OnDisabledSelected
// OnUpDisabled
// OnDownDisabled
             

typedef uint32 nuiStateDescription;

const nuiStateDescription nuiStateUnselected = 0 << 27;
const nuiStateDescription nuiStateSelected   = 1 << 27;

const nuiStateDescription nuiStateReleased   = 0 << 28;
const nuiStateDescription nuiStatePressed    = 1 << 28;

const nuiStateDescription nuiStateEnabled    = 0 << 29;
const nuiStateDescription nuiStateDisabled   = 1 << 29;

const nuiStateDescription nuiStateHoverOff   = 0 << 30;
const nuiStateDescription nuiStateHoverOn    = 1 << 30;



const nuiStateDescription nuiStateMask       = (1 << 26) + (1 << 27) + (1 << 28) + (1 << 29) + (1 << 30) + (1 << 31);

class nuiStateDecoration : public nuiDecoration
{
public:
  nuiStateDecoration(const nglString& rName); /// undefined number of states
  nuiStateDecoration(const nglString& rName, const nglString& rUp, const nglString& rDown = nglString::Null, const nglString& rHoverOn = nglString::Null, const nglString& rDisabled = nglString::Null, const nglString& rDisabledSelected = nglString::Null); ///2 to 4 states
  nuiStateDecoration(const nglString& rName, const nglString& rUp, const nglString& rDown, const nglString& rUpHoverOn,  const nglString& rDownHoverOn, const nglString& rDisabled, const nglString& rDisabledSelected); /// 5 states

  virtual ~nuiStateDecoration();
  
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;
          nuiRect GetIdealSourceClientRect() const;
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
          nuiSize GetSourceBorder(nuiPosition position) const;

  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;


  void SetState(nuiStateDescription State, nuiDecoration* pDecoration);
  void SetState(nuiStateDescription State, const nglString& rDecorationName);
  
  // attributes setters/getters
  const nglString& GetDecoUp();
  void SetDecoUp(const nglString& rPath);
  const nglString& GetDecoDown();
  void SetDecoDown(const nglString& rPath);
  const nglString& GetDecoUpHover();
  void SetDecoUpHover(const nglString& rPath);
  const nglString& GetDecoDownHover();
  void SetDecoDownHover(const nglString& rPath);
  
  const nglString& GetDecoSelected();
  void SetDecoSelected(const nglString& rPath);

  const nglString& GetDecoDisabled();
  void SetDecoDisabled(const nglString& rPath);

  const nglString& GetDecoDisabledSelected();
  void SetDecoDisabledSelected(const nglString& rPath);  
  
  const nglString& GetDecoUpDisabled();
  void SetDecoUpDisabled(const nglString& rPath);
  const nglString& GetDecoDownDisabled();
  void SetDecoDownDisabled(const nglString& rPath);
  
  
private:
  std::map<uint32, nuiDecoration*> mStates;
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  nuiDecoration* GetState(nuiStateDescription state) const;
  nuiDecoration* GetDecoration(const nglString& rName) const;
  nuiDecoration* GetDecorationForWidgetState(const nuiWidget* pWidget) const;
  
  // attributes
  void InitAttributes();
  
  nglString mDecoUp;
  nglString mDecoDown;
  nglString mDecoUpHover;
  nglString mDecoDownHover;
  nglString mDecoUpDisabled;
  nglString mDecoDownDisabled;
  nglString mDecoSelected;
  nglString mDecoDisabledSelected;
  
  nuiRect mClientRect;
  bool mUseSourceClientRect;

  
  

};
