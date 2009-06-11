/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiRSSView.h"
#include "nuiVBox.h"

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
  printf("nuiRSSVew::Update\n");
  mpBox->Clear();
  for (uint32 i = 0; i < mpRSS->GetItemCount(); i++)
  {
    const nuiRSSItem& rItem(mpRSS->GetItem(i));
    printf("%ls / %ls\n", rItem.GetLink().GetChars(), rItem.GetTitle().GetChars());
    nuiHyperLink* pLink = new nuiHyperLink(rItem.GetLink(), rItem.GetTitle());
    pLink->SetToolTip(rItem.GetDescription());
    mpBox->AddCell(pLink);
  }
  return false;
}

void nuiRSSView::ForceUpdate()
{
  mpRSS->ForceUpdate();
}
