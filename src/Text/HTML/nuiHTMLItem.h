/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTML.h"
#include "nuiHTMLContext.h"

class nuiHTMLBox;

class nuiHTMLItem
{
public:
  nuiHTMLItem(nuiHTMLNode* pNode, bool Inline);
  ~nuiHTMLItem();
  
  void CallDraw(nuiDrawContext* pContext);
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
  const nuiRect& GetIdealRect() const;
  const nuiRect& GetRect() const;
  void SetRect(const nuiRect& rRect);
  void MoveTo(float x, float y);
  void SetWidth(float w);
  void SetHeight(float h);
  
  nuiHTMLNode* GetNode() const;
  void SetParent(nuiHTMLBox* pBox);
  nuiHTMLBox* GetParent() const;
  
  virtual float GetAscender() const;
  virtual float GetDescender() const;
  
  bool IsInline() const;
  void SetInline(bool set);
  bool IsEndTag() const;
  void SetEndTag(bool set);
  
  bool IsLineBreak() const;
  void ForceLineBreak(bool s);
  
  nglString GetAbsoluteURL(const nglString& rString) const;

  bool mSetRectCalled;
protected:
  uint32 nuiHTMLItem::GetDepth() const;

  nuiHTMLNode* mpNode;
  nuiRect mIdealRect;
  nuiRect mRect;
  nuiHTMLBox* mpParent;
  bool mInline;
  bool mEndTag;
  bool mLineBreak;
  
};

