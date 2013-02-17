/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiDialogSelectDirectory::nuiDialogSelectDirectory(nuiMainWindow* pParent, const nglString& rTitle, const nglPath& rPath, const nglPath& rRootPath, nuiSize Left, nuiSize Top, nuiSize Width, nuiSize Height)
: nuiDialog(pParent), mpParent(pParent), mPath(rPath), mEventSink(this)
{
  mpContainer = new nuiSimpleContainer();

  nuiSize userWidth = (Width == 0.f) ? mpParent->GetWidth() * .8 : Width;
  nuiSize userHeight = (Height == 0.f) ? mpParent->GetHeight() * .8 : Height;
  
  mpContainer->SetUserSize(userWidth, userHeight);
  
  mpSelector = new nuiFileSelector(mPath, rRootPath);
  mpContainer->AddChild(mpSelector);
  
  mEventSink.Connect(mpSelector->OK, &nuiDialogSelectDirectory::OnSelectorOK);
  
  nuiLabel* pTitle = new nuiLabel(rTitle);
  pTitle->SetObjectName(_T("nuiDialog::Title"));
  InitDialog(pTitle, NULL, nuiDialog::eDialogButtonOk + nuiDialog::eDialogButtonCancel);
  
  nuiButton* pButton = new nuiButton(_T("New Folder"));
  pButton->SetObjectName(_T("nuiDialog::Button"));
  AddButton(pButton, nuiDialog::eDialogButtonCustom);
  mEventSink.Connect(pButton->Activated, &nuiDialogSelectDirectory::OnCreateNewFolder);
  
  SetContents(mpContainer);
  
  if ((Top == 0.f) && (Left == 0.f) && (Width == 0.f) && (Height == 0.f))
    SetDefaultPos();
  else
    SetUserPos(Left, Top);

  mEventSink.Connect(DialogDone, &nuiDialogSelectDirectory::OnDialogDone);
  
  mpSelector->UpdateLayout();
}

nuiDialogSelectDirectory::~nuiDialogSelectDirectory()
{
  
}


void nuiDialogSelectDirectory::OnSelectorOK(const nuiEvent& rEvent)
{
  mPath = mpSelector->GetPath();
  
  // send event and close the dialog if the user answered true
  if (!DirectorySelected())
    Trash();
}



void nuiDialogSelectDirectory::OnDialogDone(const nuiEvent& rEvent)
{
  nuiDialog::DialogResult result = GetResult();
  
  if (result == nuiDialog::eDialogAccepted)
  {
    OnSelectorOK(rEvent);
    rEvent.Cancel();
  }
}


const nglPath& nuiDialogSelectDirectory::GetSelectedDirectory()
{
  return mPath;
}

const nglPath& nuiDialogSelectDirectory::GetRootPath()
{
  return mRootPath;
}


void nuiDialogSelectDirectory::OnCreateNewFolder(const nuiEvent& rEvent)
{
  mpCreateDialog = new nuiDialog(mpParent);
  nuiSimpleContainer* pContainer = new nuiSimpleContainer();
  pContainer->SetUserSize(400, 80);
  
  mpCreateEditLine = new nuiEditLine();
  mpCreateEditLine->SetPosition(nuiFillHorizontal);
  pContainer->AddChild(mpCreateEditLine);
  
  mpCreateDialog->InitDialog(_T("CREATE A NEW FOLDER"), NULL, nuiDialog::eDialogButtonOk + nuiDialog::eDialogButtonCancel);
  
  mpCreateDialog->SetContents(pContainer);
  mpCreateDialog->SetDefaultPos();
  mEventSink.Connect(mpCreateDialog->DialogDone, &nuiDialogSelectDirectory::OnCreateNewFolderDone);
  
  mpCreateEditLine->Focus();
  rEvent.Cancel();
}


void nuiDialogSelectDirectory::OnCreateNewFolderDone(const nuiEvent& rEvent)
{

  nuiDialog::DialogResult result = mpCreateDialog->GetResult();
  
  if (result == nuiDialog::eDialogAccepted)
  {
    nglString text = mpCreateEditLine->GetText();
    text.Trim();
    if (text.IsNull())
    {
      rEvent.Cancel();
      return;
    }
    
    mPath = mpSelector->GetFolderPath();
    mPath += nglPath(mpCreateEditLine->GetText());
    mPath.Create();
    
    mpSelector->SetRootPath(mPath.GetParent());
    mpSelector->SetPath(mPath);
  }  
}

