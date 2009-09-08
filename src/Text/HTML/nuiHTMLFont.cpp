/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLFont.h"


//class nuiHTMLFont : public nuiHTMLItem
nuiHTMLFont::nuiHTMLFont(nuiHTMLNode* pNode)
: nuiHTMLItem(pNode, true)
{
  //nuiFontRequest mFontRequest;
  
  mSize = -1;
  mUseColor = false;
  mTextFgColor = nuiColor(0, 0, 0, 255);
  
  nuiHTMLAttrib* pFace = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_FACE);
  nuiHTMLAttrib* pSize = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_SIZE);
  nuiHTMLAttrib* pColor = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_COLOR);
  
  if (pFace)
    mFamilyName = pFace->GetValue();
  if (pSize)
    mSize = pSize->GetValue().GetCDouble();
  if (pColor)
  {
    mTextFgColor.SetValue(pColor->GetValue());
    mUseColor = true;
  }
}

nuiHTMLFont::~nuiHTMLFont()
{
  
}

void nuiHTMLFont::Draw(nuiDrawContext* pContext)
{
  
}

void nuiHTMLFont::Layout(nuiHTMLContext& rContext)
{
  if (!IsEndTag())
  {
    mBackup = rContext.mFont;
    if (!mFamilyName.IsEmpty())
      rContext.mFont.SetName(mFamilyName, 1.0f);
    if (mSize > 0)
      rContext.mFont.MustHaveSize(mSize, 1.0f);
    if (mUseColor)
    {
      mBackupTextFgColor = rContext.mTextFgColor;
      rContext.mTextFgColor = mTextFgColor;
    }
  }
  else
  {
    rContext.mFont = mBackup;
    if (mUseColor)
      rContext.mTextFgColor = mBackupTextFgColor;
  }
  
}

