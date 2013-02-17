/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiScrollBox::nuiScrollBox(nuiWidget* pBackward, nuiWidget* pForward, nuiOrientation Orientation, nuiWidget* pScrolledWidget)
: mSink(this)
{  
  SetObjectClass(_T("nuiScrollBox"));

  mOrientation = Orientation;
  mpBackwardButton = new nuiButton();
  mpBackwardButton->AddChild(pBackward);
  mpForwardButton = new nuiButton();
  mpForwardButton->AddChild(pBackward);
  mpScrolledWidget = pScrolledWidget;
  mShowDisabledButtons = false;
  
  if (mOrientation == nuiHorizontal)
  {
    mpMainBox = new nuiHBox(3);
    mpScrollView = new nuiScrollView(true, false);
  }
  else
  {
    mpMainBox = new nuiVBox(3);
    mpScrollView = new nuiScrollView(false, true);
  }  
  
  mpScrollView->AddChild(mpScrolledWidget);
  mpScrollView->SetBarSize(0);
  mpScrollView->SetForceNoSmartScroll(true);
  
  mpMainBox->SetCell(0, mpBackwardButton);
  mpMainBox->SetCell(1, mpScrollView);
  mpMainBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  mpMainBox->SetCell(2, mpForwardButton);
  
  mpMainBox->PlaceInvisibleWidgets(true);
  mpMainBox->SetExpand(nuiExpandShrinkAndGrow);
  mpMainBox->SetAllCellsSpacing(DEFAULT_NUISCROLLBOX_SPACING);
  
  AddChild(mpMainBox);
  
  
  mSink.Connect(mpBackwardButton->Activated, &nuiScrollBox::OnScrollBackward);
  mSink.Connect(mpForwardButton->Activated, &nuiScrollBox::OnScrollForward);
  mSink.Connect(mpScrollView->GetRange(mOrientation)->ValueChanged, &nuiScrollBox::OnScrollViewChanged);
  mSink.Connect(mpScrollView->GetRange(mOrientation)->Changed, &nuiScrollBox::OnScrollViewChanged); 

}


nuiScrollBox::nuiScrollBox(const nglPath& rBackwardIconPath, const nglPath& rForwardIconPath, nuiOrientation Orientation, nuiWidget* pScrolledWidget)
: mSink(this)
{
  SetObjectClass(_T("nuiScrollBox"));
  
  mOrientation = Orientation;
  mpBackwardButton = new nuiButton(rBackwardIconPath);
  mpForwardButton = new nuiButton(rForwardIconPath);
  mpScrolledWidget = pScrolledWidget;
  mShowDisabledButtons = false;
  
  if (mOrientation == nuiHorizontal)
  {
    mpMainBox = new nuiHBox(3);
    mpScrollView = new nuiScrollView(true, false);
  }
  else
  {
    mpMainBox = new nuiVBox(3);
    mpScrollView = new nuiScrollView(false, true);
  }  
  
  mpScrollView->AddChild(mpScrolledWidget);
  mpScrollView->SetBarSize(0);
  mpScrollView->SetForceNoSmartScroll(true);
  
  mpMainBox->SetCell(0, mpBackwardButton);
  mpMainBox->SetCell(1, mpScrollView);
  mpMainBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  mpMainBox->SetCell(2, mpForwardButton);
  
  mpMainBox->PlaceInvisibleWidgets(true);
  mpMainBox->SetExpand(nuiExpandShrinkAndGrow);
  mpMainBox->SetAllCellsSpacing(DEFAULT_NUISCROLLBOX_SPACING);
  
  AddChild(mpMainBox);
  
  
  mSink.Connect(mpBackwardButton->Activated, &nuiScrollBox::OnScrollBackward);
  mSink.Connect(mpForwardButton->Activated, &nuiScrollBox::OnScrollForward);
  mSink.Connect(mpScrollView->GetRange(mOrientation)->ValueChanged, &nuiScrollBox::OnScrollViewChanged);
  mSink.Connect(mpScrollView->GetRange(mOrientation)->Changed, &nuiScrollBox::OnScrollViewChanged); 
}




nuiScrollBox::~nuiScrollBox()
{
}


/////////////////////////////////
//
// SCROLLING MANAGEMENT
//
/////////////////////////////////


void nuiScrollBox::OnScrollBackward(const nuiEvent& rEvent)
{
  mpScrollView->GetRange(mOrientation)->Decrement();
  rEvent.Cancel();
}


void nuiScrollBox::OnScrollForward(const nuiEvent& rEvent)
{
  mpScrollView->GetRange(mOrientation)->Increment();
  rEvent.Cancel();
}


void nuiScrollBox::OnScrollViewChanged(const nuiEvent& rEvent)
{
  if (mpScrollView->GetRange(mOrientation)->GetValue() == mpScrollView->GetRange(mOrientation)->GetMinimum())
  {
    mpBackwardButton->SetEnabled(false);
    if(!mShowDisabledButtons)
    {
      mpBackwardButton->SetVisible(false);
    }
  }
  else
  {
    mpBackwardButton->SetEnabled(true); 
    if(!mShowDisabledButtons)
    {
      mpBackwardButton->SetVisible(true); 
    }
  }
  
  if (mpScrollView->GetRange(mOrientation)->GetValue() + mpScrollView->GetRange(mOrientation)->GetPageSize() == mpScrollView->GetRange(mOrientation)->GetMaximum())
  {
    mpForwardButton->SetEnabled(false);
    if(!mShowDisabledButtons)
    {
      mpForwardButton->SetVisible(false);
    }
  }
  else
  {
    mpForwardButton->SetEnabled(true);
    if(!mShowDisabledButtons)
    {
      mpForwardButton->SetVisible(true);
    }
  }
}


//////////////////////////////////
//
// GETTERS
//
//////////////////////////////////


nuiOrientation nuiScrollBox::GetOrientation() const
{
  return mOrientation;
}

nuiBox* nuiScrollBox::GetMainBox()
{
  return mpMainBox;
}

nuiButton* nuiScrollBox::GetBackwardButton()
{
  return mpBackwardButton;
}

nuiButton* nuiScrollBox::GetForwardButton()
{
  return mpForwardButton;
}

nuiScrollView* nuiScrollBox::GetScrollView()
{
  return mpScrollView;
}

nuiWidget* nuiScrollBox::GetScrolledWidget()
{
  return mpScrolledWidget;
}


////////////////////////////
//
// SIZING / SPACING
//
////////////////////////////



nuiSize nuiScrollBox::GetBackwardButtonSpacing() const
{
  return mpMainBox->GetCellSpacing(0);
}

nuiSize nuiScrollBox::GetScrolledWidgetSpacing() const
{
  return mpMainBox->GetCellSpacing(1);
}

nuiSize nuiScrollBox::GetForwardButtonSpacing() const
{
  return mpMainBox->GetCellSpacing(2);
}

//nuiSize nuiScrollBox::GetBackwardButtonSize() const
//{
//  if (mOrientation == nuiHorizontal)
//  {
//    return mpBackwardButton->GetRect().GetWidth();
//  }
//  else
//  {
//    return mpBackwardButton->GetRect().GetHeight();
//  }
//}
//
//nuiSize nuiScrollBox::GetForwardButtonSize() const
//{
//  if (mOrientation == nuiHorizontal)
//  {
//    return mpForwardButton->GetRect().GetWidth();
//  }
//  else
//  {
//    return mpForwardButton->GetRect().GetHeight();
//  }
//}

void nuiScrollBox::SetBackwardButtonSpacing(nuiSize spacing)
{
  mpMainBox->SetCellSpacing(0, spacing);
}

void nuiScrollBox::SetScrolledWidgetSpacing(nuiSize spacing)
{
  mpMainBox->SetCellSpacing(1, spacing);
}

void nuiScrollBox::SetForwardButtonSpacing(nuiSize spacing)
{
  mpMainBox->SetCellSpacing(2, spacing);
}

void nuiScrollBox::SetSpacing(nuiSize spacing)
{
  mpMainBox->SetAllCellsSpacing(spacing);
}

/////////////////////////////
//
// HIDE / SHOW DISABLED SCROLLBUTTONS
//
/////////////////////////////

void nuiScrollBox::SetShowDisabledButtons(bool set)
{
  mShowDisabledButtons = set;
}

bool nuiScrollBox::GetShowDisabledButtons() const
{
return mShowDisabledButtons;
}