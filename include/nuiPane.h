/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPane_h__
#define __nuiPane_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiEvent.h"
#include "nuiMouseEvent.h"
#include "nuiShape.h"
#include "nuiDrawContext.h"

/// This widget implements a pane area. It draws basic shape and can intercept mouse events
class NUI_API nuiPane : public nuiSimpleContainer
{
public:
  nuiPane(const nuiColor& rFillColor = nuiColor(.75f,.75f,.75f,.75f), const nuiColor& rStrokeColor = nuiColor(.5f,.5f,.5f,.75f), nuiShapeMode ShapeMode = eStrokeAndFillShape, nuiBlendFunc BlendFunc = nuiBlendTransp);
  virtual ~nuiPane();

  virtual bool Draw(nuiDrawContext* pContext);

  // Out going Events
  nuiMouseClicked   ClickedMouse;
  nuiMouseUnclicked UnclickedMouse;
  nuiMouseMoved     MovedMouse;

  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

  virtual void SetInterceptMouse(bool intercept=false); ///< If intercept==true then all the mouse event will be intercepted by the UserArea. By default no event is intercepted.
  virtual bool GetInterceptMouse(); ///< Return the mouse event interception state.

  void SetCurve(float Curve); ///< set rounded rectangle curve
  void SetLineWidth(float Width);
  
  void SetFillColor(const nuiColor& rFillColor);
  void SetStrokeColor(const nuiColor& rStrokeColor);
  void SetShapeMode(nuiShapeMode shapeMode);
  const nuiColor& GetFillColor() const;
  const nuiColor& GetStrokeColor() const;
  
  virtual bool SetRect(const nuiRect& rRect);
  virtual nuiRect CalcIdealSize();

protected:
  bool mInterceptMouse;
  nuiColor mFillColor;
  nuiColor mStrokeColor;
  nuiBlendFunc mBlendFunc;
  nuiShapeMode mShapeMode;
  nuiShape* mpShape;
  float mCurve;
  float mLineWidth;
};

#endif // __nuiPane_h__
