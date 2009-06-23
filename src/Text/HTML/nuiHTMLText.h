/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTMLItem.h"

class nuiHTMLText : public nuiHTMLItem
{
public:
  nuiHTMLText(nuiHTMLNode* pNode, const nglString& rText);
  virtual ~nuiHTMLText();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
  virtual float GetAscender() const;
  virtual float GetDescender() const;
  
private:
  nglString mText;
  nuiFontLayout* mpLayout;
  nuiFont* mpFont;
  
  nuiColor mTextFgColor;
  nuiColor mTextBgColor;
};

