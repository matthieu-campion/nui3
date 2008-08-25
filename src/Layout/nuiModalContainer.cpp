/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiModalContainer.h"
#include "nuiTopLevel.h"

nuiModalContainer::nuiModalContainer(nuiContainerPtr pParent)
: nuiSimpleContainer(),
  mIsModal(true),
  mInModalState(false)
{
  nuiTopLevel* pTop = NULL;
  if (pParent)
    pTop = pParent->GetTopLevel();
  if (pTop)
    pTop->AddChild(this);

  SetObjectClass(_T("nuiModalContainer"));
  GetTopLevel()->CancelGrab();
  Grab();
  Focus();
}

bool nuiModalContainer::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  mIsModal = true;
  GetTopLevel()->CancelGrab();
  Grab();
  Focus();
  
  return true;
}

nuiModalContainer::~nuiModalContainer()
{
}

void nuiModalContainer::SetModal(bool enable_modal)
{
  mIsModal = enable_modal;
}

bool nuiModalContainer::DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
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
            if (pItem->DispatchMouseClick(X,Y, Button))
            {
              delete pIt;
              return true;
            }
          }
        }
      }
      delete pIt;
    }

    GlobalToLocal(X,Y);
    if (PreClicked(X,Y, Button))
      return true;
    bool ret = MouseClicked(X,Y,Button);
    ret |= Clicked(X,Y,Button);
    
    return mIsModal;
  }
  return false;
}

bool nuiModalContainer::DispatchMouseUnclick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
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
            if ((pItem)->DispatchMouseUnclick(X,Y, Button))
            {
              delete pIt;
              return true;
            }
        }
      }
      delete pIt;
    }

    GlobalToLocal(X,Y);
    if (PreUnclicked(X,Y, Button))
      return true;
    bool ret = MouseUnclicked(X,Y,Button);
    ret |= Unclicked(X,Y,Button);
    
    return mIsModal;
  }
  return false;
}

nuiWidgetPtr nuiModalContainer::DispatchMouseMove (nuiSize X, nuiSize Y)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;
  
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
            pItem->DispatchMouseMove(X,Y);
      }
      delete pIt;
    }
  }

  GlobalToLocal(X,Y);
  if (PreMouseMoved(X,Y))
    return this;

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



bool nuiModalContainer::DispatchKeyDown(const nglKeyEvent& rEvent)
{
  if (TriggerHotKeys(rEvent, true, true))
  {
    return true;
  }
  
  if (KeyDown(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, true, false))
  {
    return true;
  }
  
  if (mpParent && !mIsModal)
  {
    return mpParent->DispatchKeyDown(rEvent);
  }
  
  return false;
}

bool nuiModalContainer::DispatchKeyUp(const nglKeyEvent& rEvent)
{
  if (TriggerHotKeys(rEvent, false, true))
  {
    return true;
  }
  
  if (KeyUp(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, false, false))
  {
    return true;
  }
  
  if (mpParent && !mIsModal)
  {
    return mpParent->DispatchKeyUp(rEvent);
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
  if (mInModalState)
  {
    nuiTopLevel* pTop = GetTopLevel();
    NGL_ASSERT(pTop);
    mInModalState = false;
    pTop->ExitModalState();
  }
  
  nuiSimpleContainer::OnTrash();
}

#include "nuiPositioner.h"
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

