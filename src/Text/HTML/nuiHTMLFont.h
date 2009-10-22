/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTMLItem.h"

class nuiHTMLFont : public nuiHTMLItem
{
public:
  nuiHTMLFont(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor);
  virtual ~nuiHTMLFont();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
private:
  nglString mFamilyName;
  float mSize;
  nuiColor mTextFgColor;
  bool mUseColor;
  nuiFontRequest mBackup;
  nuiColor mBackupTextFgColor;
};

