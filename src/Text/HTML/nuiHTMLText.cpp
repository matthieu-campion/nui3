/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLText.h"

///////////////////////////////////////// nuiHTMLText
nuiHTMLText::nuiHTMLText(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, const nglString& rText)
: nuiHTMLItem(pNode, pAnchor, true), mText(rText), mpLayout(NULL), mpFont(NULL), mpCompositeLayout(NULL), mpNextInRun(NULL), mFirstInRun(false), mUnderline(false), mStrikeThrough(false)
{

}

nuiHTMLText::~nuiHTMLText()
{
  if (mpFont)
    mpFont->Release();
  delete mpLayout;
  delete mpCompositeLayout;
  mpNextInRun = NULL;
}

void nuiHTMLText::Draw(nuiDrawContext* pContext)
{
  if (!mVisible)
    return;

  if (!mFirstInRun)
    return;
  
  //pContext->SetTextColor(mTextFgColor);
  pContext->SetTextColor(GetStyle().GetColor());
  //nuiColor mTextBgColor;
  pContext->SetFont(mpFont, false);
  
  if (!mpCompositeLayout)
  {
    nuiHTMLText* pIt = this;
    nglString str(nglString::Empty);
    do 
    {
      str.Add(pIt->mText);
      str.Add(_T(" "));
      pIt = pIt->mpNextInRun;
    } while (pIt && !pIt->mFirstInRun);
      
    mpCompositeLayout = new nuiTextLayout(mpFont, nuiHorizontal);
    mpCompositeLayout->SetUnderline(mUnderline);
    mpCompositeLayout->SetStrikeThrough(mStrikeThrough);
    mpCompositeLayout->Layout(str);
    
    //NGL_OUT(_T("Draw HTMLText: %s\n"), str.GetChars());
  }
  
  pContext->DrawText(0, mpCompositeLayout->GetAscender() , *mpCompositeLayout);
}

void nuiHTMLText::Layout(nuiHTMLContext& rContext)
{
  delete mpLayout;
  if (mpFont)
    mpFont->Release();
  mpFont = rContext.mpFont;
  mpFont->Acquire();
  
  mpLayout = new nuiTextLayout(mpFont, nuiHorizontal);
  mpLayout->SetUnderline(rContext.mUnderline);
  mpLayout->SetStrikeThrough(rContext.mStrikeThrough);
  mUnderline = rContext.mUnderline;
  mStrikeThrough = rContext.mStrikeThrough;
  
  mpLayout->Layout(mText);
  mIdealRect = mpLayout->GetRect();
  mIdealRect.SetWidth(mIdealRect.GetWidth() + rContext.mHSpace);
  mIdealRect.RoundToBiggest();

  mTextFgColor = rContext.mTextFgColor;
  mTextBgColor = rContext.mTextBgColor;
  //printf("text layout done (%s)\n", mIdealRect.GetValue().GetChars());
  
  delete mpCompositeLayout;
  mpCompositeLayout = NULL;
}

float nuiHTMLText::GetAscender() const
{
  return mpLayout->GetAscender();
}

float nuiHTMLText::GetDescender() const
{
  return mpLayout->GetDescender();
}

void nuiHTMLText::SetNextInRun(nuiHTMLText* pNext)
{
  mpNextInRun = pNext;
}

void nuiHTMLText::SetFirstInRun(bool set)
{
  mFirstInRun = set;
}
