/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

const nuiMessageBox::Button nuiMessageBox::ButtonYes    = 1;
const nuiMessageBox::Button nuiMessageBox::ButtonNo     = 0;
const nuiMessageBox::Button nuiMessageBox::ButtonOK     = 1;
const nuiMessageBox::Button nuiMessageBox::ButtonCancel = 0;
const nuiMessageBox::Button nuiMessageBox::ButtonRetry  = 2;

nuiMessageBox::nuiMessageBox(nuiContainer * pParent, const nglString& rTitle, const nglString& rMessage, nuiMessageBoxType type, bool showIcon)
: nuiModalContainer(pParent), mpBtnBox(NULL), mSink(this)
{
  nuiLabel* pMessage = new nuiLabel(rMessage);
  pMessage->SetObjectName(_T("nuiMessageBox::Message"));
  Init(rTitle, pMessage, type, showIcon);
}


nuiMessageBox::nuiMessageBox(nuiContainer * pParent, const nglString& rTitle, nuiWidget* pContents, nuiMessageBoxType type, bool showIcon)
: nuiModalContainer(pParent), mpBtnBox(NULL), mSink(this)
{
  Init(rTitle, pContents, type, showIcon);
}

void nuiMessageBox::Init(const nglString& rTitle, nuiWidget* pContents, nuiMessageBoxType type, bool showIcon)
{
  SetObjectClass(_T("nuiMessageBox"));
  mType = type;
  
  mKeyDown = false;

  mClickedButton = ButtonCancel;
  

  nuiVBox* pVBox = new nuiVBox();
  pVBox->SetExpand(nuiExpandShrinkAndGrow);
  pVBox->SetObjectName(_T("nuiMessageBox::Client"));
  AddChild(pVBox);
  
  nuiLabel* pTitle = new nuiLabel(rTitle);
  pTitle->SetObjectName(_T("nuiMessageBox::Title"));
  pTitle->SetObjectClass(_T("nuiMessageBox::Title"));
  pVBox->AddCell(pTitle);
  
  nuiHBox* pHBox = new nuiHBox(2);
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetObjectName(_T("nuiMessageBox::Contents"));
  pVBox->AddCell(pHBox);
  pVBox->SetCellExpand(pVBox->GetNbCells()-1, nuiExpandShrinkAndGrow);

  if (showIcon)
  {
    nuiSimpleContainer* pIcon = new nuiSimpleContainer();
    nglString objectName;
    objectName.Format(_T("nuiMessageBox::Icon"), rTitle.GetChars());
    pIcon->SetObjectName(objectName);
    pHBox->SetCell(0, pIcon, nuiCenter);
  }
  
  pHBox->SetCell(1, pContents);
  pHBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  
  mpBtnBox = new nuiHBox();
  mpBtnBox->SetObjectName(_T("nuiMessageBox::ButtonBox"));
  pVBox->AddCell(mpBtnBox);

  mpBtnBox->AddCell(NULL);
  mpBtnBox->SetCellExpand(mpBtnBox->GetNbCells()-1, nuiExpandShrinkAndGrow);

  
  switch (mType)
  {
    case eMB_OK:
      mpBtnBox->AddCell(CreateOK());
      mpBtnBox->SetPosition(nuiCenter);
      break;
      
    case eMB_Cancel:
      mpBtnBox->AddCell(CreateCancel());
      mpBtnBox->SetPosition(nuiCenter);
      break;
      
    case eMB_OKCancel:
      mpBtnBox->AddCell(CreateOK());
      mpBtnBox->AddCell(CreateCancel());
      break;
      
    case eMB_RetryOKCancel:
      mpBtnBox->AddCell(CreateRetry());
      mpBtnBox->AddCell(CreateOK());
      mpBtnBox->AddCell(CreateCancel());
      break;
      
    case eMB_YesNo:
      mpBtnBox->AddCell(CreateYes());
      mpBtnBox->AddCell(CreateNo());
      break;
      
    case eMB_Custom:
      break;
  }
  
  
  SetPosition(nuiCenter);
  GetTopLevel()->SetFocus(this);
  
  // default decoration
  nuiDefaultDecoration::MessageBox(this);
}



nuiMessageBox::~nuiMessageBox()
{
}

nuiMessageBox::Button nuiMessageBox::Do(nuiContainer * pParent, const nglString& rTitle, const nglString& rMessage, nuiMessageBoxType type)
{
  nuiMessageBox* pBox = new nuiMessageBox(pParent, rTitle, rMessage, type);
  Button res = pBox->QueryUser();
  return res;
}

nuiWidget* nuiMessageBox::CreateOK()
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));
  
  nuiLabel* pButtonLabel = new nuiLabel(_T("OK <ENTER>"));
  pButton->AddChild(pButtonLabel);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnOK);
  
  return pButton;
}

nuiWidget* nuiMessageBox::CreateCancel()
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));  
  
  nuiLabel* pButtonLabel = new nuiLabel(_T("Cancel <ESC>"));
  pButton->AddChild(pButtonLabel);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnCancel);
  
  return pButton;
}

nuiWidget* nuiMessageBox::CreateYes()
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));
  
  nuiLabel* pButtonLabel = new nuiLabel(_T("Yes <Y>"));
  pButton->AddChild(pButtonLabel);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnOK);
  
  return pButton;
}

nuiWidget* nuiMessageBox::CreateNo()
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));
  
  nuiLabel* pButtonLabel = new nuiLabel(_T("No <N>"));
  pButton->AddChild(pButtonLabel);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnCancel);
  
  return pButton;
}

nuiWidget* nuiMessageBox::CreateRetry()
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));
  
  nuiLabel* pButtonLabel = new nuiLabel(_T("Retry"));
  pButton->AddChild(pButtonLabel);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnRetry);
  
  return pButton;
}

nuiWidget* nuiMessageBox::CreateCustom(const nglString& rName, Button btn)
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));
  
  nuiLabel* pButtonLabel = new nuiLabel(rName);
  pButton->AddChild(pButtonLabel);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnCustom, (void*)btn);
  
  return pButton;
}

nuiWidget* nuiMessageBox::CreateCustom(nuiWidget* pWidget, Button btn)
{
  nuiButton* pButton = new nuiButton();
  pButton->SetObjectName(_T("nuiMessageBox::Button"));
  pButton->AddChild(pWidget);
  
  mSink.Connect(pButton->Activated, &nuiMessageBox::OnCustom, (void*)btn);
  
  return pButton;
}

nuiMessageBox::Button nuiMessageBox::QueryUser()
{
  Button btn = ButtonCancel;
  DoModal();
  btn = mClickedButton;
  Trash();
  return btn;
}

void nuiMessageBox::OnOK(const nuiEvent& rEvent)
{
  mClickedButton = ButtonOK;
  ExitModal();
}

void nuiMessageBox::OnCancel(const nuiEvent& rEvent)
{
  mClickedButton = ButtonCancel;
  ExitModal();
}

void nuiMessageBox::OnRetry(const nuiEvent& rEvent)
{
  mClickedButton = ButtonRetry;
  ExitModal();
}

void nuiMessageBox::OnCustom(const nuiEvent& rEvent)
{
  Button btn = (Button)(unsigned long int)rEvent.mpUser; 
  mClickedButton = btn;
  ExitModal();
}


// TODO: Add more shortcuts, default buttons, and shortcuts informations on the buttons
bool nuiMessageBox::KeyUp(const nglKeyEvent& rEvent)
{  
  if (!mKeyDown)
    return false;
  
  mKeyDown = false;
  
  if (rEvent.mKey == NK_ESC)
  {
    if ((mType == eMB_OKCancel) || (mType == eMB_Cancel) || (mType == eMB_YesNo))
    {
      mClickedButton = ButtonCancel;
      ExitModal();
      return true;
    }
  }
  else if (rEvent.mKey == NK_ENTER)
  {
    if ((mType == eMB_OKCancel) || (mType == eMB_OK) || (mType == eMB_YesNo))
    {
      mClickedButton = ButtonOK;
      ExitModal();
      return true;
    }
  }
  else if (rEvent.mKey == NK_Y)
  {
    if (mType == eMB_YesNo)
    {
      mClickedButton = ButtonYes;
      ExitModal();
      return true;
    }
  }
  else if (rEvent.mKey == NK_N)
  {
    if (mType == eMB_YesNo)
    {
      mClickedButton = ButtonNo;
      ExitModal();
      return true;
    }
  }
  
  nuiModalContainer::KeyUp(rEvent);
  return false;
}


bool nuiMessageBox::KeyDown(const nglKeyEvent& rEvent)
{
  mKeyDown = true;
  return nuiModalContainer::KeyDown(rEvent);
}

void nuiMessageBox::AddButton(const nglString& rName, Button btn)
{
  mpBtnBox->AddCell(CreateCustom(rName, btn));
}

void nuiMessageBox::AddButton(nuiWidget* pWidget, Button btn)
{
  mpBtnBox->AddCell(CreateCustom(pWidget, btn));
}
