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

protected:
  float LayoutLine(uint32& start, uint32& count, float& y, float& h, nuiHTMLContext& rContext);
  
  std::vector<nuiHTMLItem*> mItems;
  std::stack<nuiHTMLContext> mContextStack;
};

