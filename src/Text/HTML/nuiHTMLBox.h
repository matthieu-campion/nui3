/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTMLItem.h"

class nuiHTMLBox : public nuiHTMLItem
{
public:
  nuiHTMLBox(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, bool Inline);
  virtual ~nuiHTMLBox();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
  void AddItem(nuiHTMLItem* pItem);
  void AddItemEnd(nuiHTMLItem* pItem);
  
  void PushContext(const nuiHTMLContext& rContext);
  void PopContext(nuiHTMLContext& rContext);
  
  void GetItemsAt(std::vector<nuiHTMLItem*>& rHitItems, float X, float Y) const;

  void UpdateVisibility(const nuiRect& rVisibleRect);

  void SetMargins(float val);
  void SetMarginLeft(float val);
  void SetMarginTop(float val);
  void SetMarginRight(float val);
  void SetMarginBottom(float val);
  float GetMarginLeft() const;
  float GetMarginTop() const;
  float GetMarginRight() const;
  float GetMarginBottom() const;

  virtual int32 GetChildrenCount() const;
  virtual nuiHTMLItem* GetChild(int32 index) const;
  
  
protected:
  float LayoutLine(uint32& start, uint32& count, float& y, float& h, nuiHTMLContext& rContext);
  
  std::vector<nuiHTMLItem*> mItems;
  std::stack<nuiHTMLContext> mContextStack;

  float mMarginLeft;
  float mMarginTop;
  float mMarginRight;
  float mMarginBottom;
};

