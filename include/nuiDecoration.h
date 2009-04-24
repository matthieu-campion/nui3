/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiRefCount.h"
#include "nuiDecorationDefines.h"


class nuiDecoration : public nuiObject, public nuiRefCount
{
public:
  nuiDecoration(const nglString& rName);
  void InitAttributes();

  virtual ~nuiDecoration();

  virtual bool Load(const nuiXMLNode* pNode) = 0;
  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode) = 0;
  
  void SetName(const nglString& rName);
  const nglString& GetName() const;
  
  void UseWidgetAlpha(bool use);
  bool IsWidgetAlphaUsed();
  
  void SetLayer(nuiDecorationLayer layer);
  nuiDecorationLayer GetLayer();
  
  virtual void DrawBack(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  virtual void DrawFront(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);
  
  
  virtual void GlobalToClientRect(nuiRect& rRect) const;
  virtual void ClientToGlobalRect(nuiRect& rRect) const;
  
  virtual nuiSize GetBorder(nuiPosition postion) const = 0;
  virtual nuiRect GetIdealClientRect() const = 0;
  virtual nuiRect GetMinimumClientRect() const;
  virtual nuiRect GetMaximumClientRect() const;

  nuiSignal0<> Changed;
  
  typedef std::map<nglString, nuiDecoration*> DecorationMap;
  static nuiDecoration* Get(const nglString& rName, bool silent=false);
  static const DecorationMap& Enum();
  static nuiSimpleEventSource<0> DecorationsChanged;
protected:

  bool mUseWidgetAlpha;
  nuiDecorationLayer mLayer;
    
private:
  
  static void AddDecoration(nuiDecoration* pDecoration);
  static void DelDecoration(nuiDecoration* pDecoration);

  static DecorationMap mDecorations;
  

  
};
