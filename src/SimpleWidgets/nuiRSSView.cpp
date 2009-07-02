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
nuiRSSView::nuiRSSView(const nglString& rURL, int32 SecondsBetweenUpdates, nglIStream* pOriginalStream)
: mViewSink(this)
{
  if (SetObjectClass(_T("nuiRSSView")))
  {
    // Init Attributes
  }
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
    nuiHTMLView* pLabel = new nuiHTMLView(480);
    pLabel->SetText(desc);
    pPane->AddChild(pLabel);
    mpBox->AddCell(pPane);
    
    pLabel->SetObjectClass(_T("nuiRSSContents"));
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

