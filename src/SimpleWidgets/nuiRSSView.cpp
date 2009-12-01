/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiRSSView.h"
#include "nuiVBox.h"
#include "nuiFolderPane.h"
#include "nglIMemory.h"
#include "nuiHTML.h"
#include "nuiHTMLView.h"

//class nuiRSSView : public nuiSimpleContainer
nuiRSSView::nuiRSSView(const nglString& rURL, int32 SecondsBetweenUpdates, nglIStream* pOriginalStream, bool ForceNoHTML)
: mViewSink(this)
{
  if (SetObjectClass(_T("nuiRSSView")))
  {
    InitAttributes();
  }
  
  mTextColor = nuiColor(0,0,0);
  mFont = nglString::Null;
  
  mForceNoHTML = ForceNoHTML;
  mpRSS = new nuiRSS(rURL, SecondsBetweenUpdates, pOriginalStream);
  mpBox = new nuiVBox();
  AddChild(mpBox);
  mViewSink.Connect(mpRSS->UpdateDone, &nuiRSSView::Update);
  Update(nuiEvent());
}

nuiRSSView::~nuiRSSView()
{
  delete mpRSS;
}

nuiRect nuiRSSView::CalcIdealSize()
{
  return nuiSimpleContainer::CalcIdealSize();
}

bool nuiRSSView::SetRect(const nuiRect& rRect)
{
  nuiSimpleContainer::SetRect(rRect);
  return true;
}

void nuiRSSView::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Font")), nuiUnitName,
                nuiMakeDelegate(this, &nuiRSSView::_GetFont), 
                nuiMakeDelegate(this, &nuiRSSView::_SetFont)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("TextColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiRSSView::GetTextColor), 
                nuiMakeDelegate(this, &nuiRSSView::SetTextColor)));
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("URL")), nuiUnitName,
                nuiMakeDelegate(this, &nuiRSSView::GetURL), 
                nuiMakeDelegate(this, &nuiRSSView::SetURL)));
  
}



bool nuiRSSView::Update(const nuiEvent& rEvent)
{
  mpBox->Clear();
  for (uint32 i = 0; i < mpRSS->GetItemCount(); i++)
  {
    const nuiRSSItem& rItem(mpRSS->GetItem(i));
    //printf("%ls / %ls\n", rItem.GetLink().GetChars(), rItem.GetTitle().GetChars());
    nuiHyperLink* pLink = new nuiHyperLink(rItem.GetLink(), rItem.GetTitle());
    pLink->UseEllipsis(true);
    nuiFolderPane* pPane = new nuiFolderPane(pLink, true);
    
    nglString desc(rItem.GetDescription());
    std::string str(desc.GetStdString());
    nglIMemory mem(&str[0], str.size());
    nuiHTML html;
    bool res = html.Load(mem);
    
    nglString text;
    if (res)
    {
      // Contents is valid HTML
      html.GetSimpleText(text);
      //NGL_OUT(_T("%d - Could parse HTML tags:\n%ls\n"), i, text.GetChars());
    }
    else
    {
      text = rItem.GetDescription();
      //NGL_OUT(_T("%d - Couldn't parse HTML tags:\n%ls\n"), i, text.GetChars());
    }
    
//    nuiLabel* pLabel = new nuiLabel(text);
//    pLabel->SetObjectName(_T("nuiRSSView::Description"));
//    pLabel->SetWrapping(true);
    if (!mForceNoHTML)
    {
      nuiHTMLView* pLabel = new nuiHTMLView(480);
      pLabel->SetText(desc);
      pLabel->SetObjectClass(_T("nuiRSSContents"));
      if (mFont != nglString::Null)
        pLabel->_SetFont(mFont);
      pLabel->SetTextColor(mTextColor);
      pPane->AddChild(pLabel);
    }
    else
    {
      nuiLabel* pLabel = new nuiLabel(text);
      pLabel->SetObjectClass(_T("nuiRSSContents"));
      pLabel->SetWrapping(true);
      pPane->AddChild(pLabel);
      
    }
    mpBox->AddCell(pPane);
    
  }
  return false;
}

void nuiRSSView::ForceUpdate()
{
  mpRSS->ForceUpdate();
}

void nuiRSSView::SetURL(const nglString& rURL)
{
  mpRSS->SetURL(rURL);
}

const nglString& nuiRSSView::GetURL() const
{
  return mpRSS->GetURL();
}


void nuiRSSView::_SetFont(const nglString& rFontSymbol)
{
  mFont = rFontSymbol;
  ForceUpdate();  
}


const nglString& nuiRSSView::_GetFont() const
{
  return mFont;
}


const nuiColor& nuiRSSView::GetTextColor() const
{
  return mTextColor;
}


void nuiRSSView::SetTextColor(const nuiColor& Color)
{
  mTextColor = Color;
  ForceUpdate();  
}


