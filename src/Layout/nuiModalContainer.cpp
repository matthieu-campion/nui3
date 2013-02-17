/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiModalContainer::nuiModalContainer(nuiContainerPtr pParent)
: nuiSimpleContainer(),
  mIsModal(true),
  mInModalState(false),
  mModalEventSink(this)
{
  SetObjectClass(_T("nuiModalContainer"));
  InitParent(pParent);
}

nuiModalContainer::~nuiModalContainer()
{
}

void nuiModalContainer::InitParent(nuiContainerPtr pParent)
{
  if (!pParent)
    return;

  nuiTopLevel* pTop = NULL;
  pTop = pParent->GetTopLevel();
  NGL_ASSERT(pTop);
  mpPreviousFocus = pTop->GetFocus();
  if (mpPreviousFocus)
    mModalEventSink.Connect(mpPreviousFocus->Trashed, &nuiModalContainer::OnPreviousFocusTrashed);
  
  SetWantKeyboardFocus(true);
  pTop->AddChild(this);
  pTop->Invalidate();
  GetTopLevel()->CancelGrab();
  Grab();
  SetFocusVisible(false);
  Focus();
}

void nuiModalContainer::SetModal(bool enable_modal)
{
  mIsModal = enable_modal;
}

bool nuiModalContainer::DispatchMouseClick(const nglMouseInfo& rInfo)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;
  
  if (HasGrab())
    Ungrab();
  if (IsDisabled())
    return false;

  {
    {
      IteratorPtr pIt;
      for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem)
        {
          if (IsEnabled())
          {
            if (pItem->DispatchMouseClick(rInfo))
            {
              delete pIt;
              return true;
            }
          }
        }
      }
      delete pIt;
    }

    nuiSize X = rInfo.X;
    nuiSize Y = rInfo.Y;
    GlobalToLocal(X,Y);
    nglMouseInfo info(rInfo);
    info.X = X;
    info.Y = Y;
    if (PreClicked(rInfo))
      return true;
    bool ret = MouseClicked(rInfo);
    ret |= Clicked(rInfo);
    
    return mIsModal || ret;
  }
  return false;
}

bool nuiModalContainer::DispatchMouseUnclick(const nglMouseInfo& rInfo)
{  
  if (IsTrashed())
    return false;
  
  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
    {
      if (IsEnabled())
        if ((pItem)->DispatchMouseUnclick(rInfo))
        {
          delete pIt;
          return true;
        }
    }
  }
  delete pIt;

  nuiSize X = rInfo.X;
  nuiSize Y = rInfo.Y;
  GlobalToLocal(X, Y);
  nglMouseInfo info(rInfo);
  info.X = X;
  info.Y = Y;
  bool res = PreUnclicked(info);
  if (!res)
  {
    res = MouseUnclicked(info);
    res |= Unclicked(info);
  }
  
  if (mWantKeyboardFocus && (rInfo.Buttons == nglMouseInfo::ButtonLeft || rInfo.Buttons == nglMouseInfo::ButtonRight))
    Focus();
  
  return mIsModal || res;
}

nuiWidgetPtr nuiModalContainer::DispatchMouseMove(const nglMouseInfo& rInfo)
{
  if (!mMouseEventEnabled || mTrashed)
    return NULL;
  
  if (HasGrab())
    Ungrab();
  if (IsDisabled())
    return NULL;

  {
    SetHover(true);
    {
      IteratorPtr pIt;
      for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem)
          if (pItem->IsVisible())
            pItem->DispatchMouseMove(rInfo);
      }
      delete pIt;
    }
  }

  nuiSize X = rInfo.X;
  nuiSize Y = rInfo.Y;
  GlobalToLocal(X, Y);
  nglMouseInfo info(rInfo);
  info.X = X;
  info.Y = Y;
  if (PreMouseMoved(info))
    return this;
  //  if (MouseMoved(info))
  //    return this;
  return mIsModal ? this : NULL;
}

bool nuiModalContainer::DispatchTextInput(const nglString& rUnicodeText)
{
  if (TextInput(rUnicodeText))
  {
    return true;
  }
  
  if (mpParent && !mIsModal)
  {
    return mpParent->DispatchTextInput(rUnicodeText);
  }
  
  return false;
}



bool nuiModalContainer::DispatchKeyDown(const nglKeyEvent& rEvent, nuiKeyModifier Mask)
{
  if (TriggerHotKeys(rEvent, true, true, Mask))
  {
    return true;
  }
  
  if (KeyDown(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, true, false, Mask))
  {
    return true;
  }
  
  if (mpParent && !mIsModal)
  {
    return mpParent->DispatchKeyDown(rEvent, Mask & mHotKeyMask);
  }
  
  return false;
}

bool nuiModalContainer::DispatchKeyUp(const nglKeyEvent& rEvent, nuiKeyModifier Mask)
{
  if (TriggerHotKeys(rEvent, false, true, Mask))
  {
    return true;
  }
  
  if (KeyUp(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, false, false, Mask))
  {
    return true;
  }
  
  if (mpParent && !mIsModal)
  {
    return mpParent->DispatchKeyUp(rEvent, Mask);
  }
  
  return false;
}

void nuiModalContainer::DoModal()
{
  nuiTopLevel* pTop = GetTopLevel();
  NGL_ASSERT(pTop);
  mInModalState = true;
  pTop->EnterModalState();
}

void nuiModalContainer::ExitModal()
{
  nuiTopLevel* pTop = GetTopLevel();
  NGL_ASSERT(pTop);
  mInModalState = false;
  pTop->ExitModalState();
}

void nuiModalContainer::OnTrash()
{
  nuiTopLevel* pTop = GetTopLevel();
  NGL_ASSERT(pTop);

  if (mInModalState)
  {
    mInModalState = false;
    pTop->ExitModalState();
  }
  
  /* LBDEBUG FIXME
  if (mpPreviousFocus)
    mpPreviousFocus->Focus();
  */
  nuiSimpleContainer::OnTrash();
}

#include "nuiPane.h"
#include "nuiLabel.h"
#include "nuiButton.h"
#include "nuiVBox.h"
void nuiMessageBox(nuiContainer* pParent, const nglString& rTitle, const nglString& rMessage)
{
  nuiModalContainer* pModal = new nuiModalContainer(pParent);
  
  nuiPane* pPane = new nuiPane();
  pPane->SetObjectName(_T("nuiMessageBox"));
  pModal->AddChild(pPane);
  
  nuiVBox* pVBox = new nuiVBox();
  pPane->AddChild(pVBox);

  nuiLabel* pTitle = new nuiLabel(rTitle);
  pTitle->SetObjectName(_T("nuiMessageBox_Title"));
  pVBox->AddCell(pTitle);
  
  nuiLabel* pMessage = new nuiLabel(rMessage);
  pMessage->SetObjectName(_T("nuiMessageBox_Message"));
  pVBox->AddCell(pMessage);
  
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox_Button"));
  pVBox->AddCell(pButton);
  
  nuiLabel* pButtonLabel = new nuiLabel(_T("OK"));
  pButtonLabel->SetObjectName(_T("nuiMessageBox_ButtonLabel"));
  pButton->AddChild(pButtonLabel);
  
  nuiEventSink<nuiWidget> sink(pModal);
  sink.Connect(pButton->Activated, &nuiWidget::AutoTrash);
  
  pModal->DoModal();
}

void nuiModalContainer::OnPreviousFocusTrashed(const nuiEvent& rEvent)
{
  mpPreviousFocus = NULL;
}

void nuiModalContainer::GetHoverList(nuiSize X, nuiSize Y, std::set<nuiWidget*>& rHoverSet, std::list<nuiWidget*>& rHoverList) const
{
  if (mIsModal)
  {
    // Only our children can get the hover...
    rHoverSet.clear();
    rHoverList.clear();
  }
  
  nuiContainer::ConstIteratorPtr pIt = NULL;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsInsideFromRoot(X, Y))
    {
      rHoverList.push_back(pItem);
      rHoverSet.insert(pItem);
      nuiContainer* pChild = dynamic_cast<nuiContainer*>(pItem);
      if (pChild)
        pChild->GetHoverList(X, Y, rHoverSet, rHoverList);
    }
  }
  delete pIt;
}

