/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiDecorationDefines.h"
#include "nuiAttribute.h"
#include "nuiAttributeEditor.h"
#include "nuiObject.h"

class nuiDecoration : public nuiObject
{
public:
  nuiDecoration(const nglString& rName);
  void InitAttributes();

  virtual ~nuiDecoration();

  void SetObjectName(const nglString& rName);
  
  void UseWidgetAlpha(bool use);
  bool IsWidgetAlphaUsed() const;
  
  void SetLayer(nuiDecorationLayer layer);
  nuiDecorationLayer GetLayer() const;
  
  void EnableBorder(bool set);
  bool IsBorderEnabled() const;
  
  void RedrawOnHover(bool set);
  bool GetRedrawOnHover() const;
  
  
  virtual void DrawBack(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  virtual void DrawFront(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  
  
  virtual void GlobalToClientRect(nuiRect& rRect, const nuiWidget* pWidget) const;
  virtual void ClientToGlobalRect(nuiRect& rRect, const nuiWidget* pWidget) const;
  
  virtual nuiSize GetBorder(nuiPosition postion, const nuiWidget* pWidget) const = 0;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const = 0;
  virtual nuiRect GetMinimumClientRect(const nuiWidget* pWidget) const;
  virtual nuiRect GetMaximumClientRect(const nuiWidget* pWidget) const;

  nuiSignal0<> Changed;
  
  typedef std::map<nglString, nuiDecoration*> DecorationMap;
  static nuiDecoration* Get(const nglString& rName, bool silent=false);
  static const DecorationMap& Enum();
  static nuiSimpleEventSource<0> DecorationsChanged;
  
  nglString GetCSSDeclaration() const;
  
  static nuiAttributeEditor* GetAttributeEditor(void* pTarget, nuiAttribute<const nglString&>* pAttribute);
  
  static nuiDecoration* CreateDecoration(const nglString& rDecorationTypeName, const nglString& rDecorationName);
  typedef nuiDecoration* (*DecorationCreatorFunction)(const nglString&);
  static void AddDecorationType(const nglString& rDecorationTypeName, const DecorationCreatorFunction& rCreatorDelegate);
  static void InitDecorationEngine();
  static void ExitDecorationEngine();
protected:

  bool mUseWidgetAlpha : 1;
  bool mBorderEnabled : 1;
  bool mRedrawOnHover : 1;

  nuiDecorationLayer mLayer;
    
private:
  
  static void AddDecoration(nuiDecoration* pDecoration);
  static void DelDecoration(nuiDecoration* pDecoration);

  static DecorationMap mDecorations;
  static std::map<nglString, DecorationCreatorFunction> mDecorationCreators;
};

template <class T>
nuiDecoration* nuiCreateDecoration(const nglString& rName)
{
  return new T(rName);
}

