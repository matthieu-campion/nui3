/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDialogSelectDirectory.h"


nuiDialogSelectDirectory::nuiDialogSelectDirectory(nuiMainWindow* pParent, const nglString& rTitle, const nglPath& rPath, const nglPath& rRootPath)
: nuiDialog(pParent), mpParent(pParent), mPath(rPath), mEventSink(this)
{
  mpContainer = new nuiSimpleContainer();
  mpContainer->SetUserSize(mpParent->GetWidth() * .8, mpParent->GetHeight() * .8);
  
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
  SetDefaultPos();
  mEventSink.Connect(DialogDone, &nuiDialogSelectDirectory::OnDialogDone);
  
  mpSelector->UpdateLayout();
}

nuiDialogSelectDirectory::~nuiDialogSelectDirectory()
{
  
}



bool nuiDialogSelectDirectory::OnSelectorOK(const nuiEvent& rEvent)
{
  mPath = mpSelector->GetPath();
  
  // send event and close the dialog if the user answered true
  if (!DirectorySelected())
    Trash();
  
  return false;
}



bool nuiDialogSelectDirectory::OnDialogDone(const nuiEvent& rEvent)
{
  nuiDialog::DialogResult result = GetResult();
  
  if (result == nuiDialog::eDialogAccepted)
  {
    bool res = !OnSelectorOK(rEvent);
    return res;
  }
  
  return false;
}


const nglPath& nuiDialogSelectDirectory::GetSelectedDirectory()
{
  return mPath;
}

const nglPath& nuiDialogSelectDirectory::GetRootPath()
{
  return mRootPath;
}


bool nuiDialogSelectDirectory::OnCreateNewFolder(const nuiEvent& rEvent)
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
  return true;
}


bool nuiDialogSelectDirectory::OnCreateNewFolderDone(const nuiEvent& rEvent)
{

  nuiDialog::DialogResult result = mpCreateDialog->GetResult();
  
  if (result == nuiDialog::eDialogAccepted)
  {
    nglString text = mpCreateEditLine->GetText();
    text.Trim();
    if (text == nglString::Null)
      return true;
    
    mPath = mpSelector->GetFolderPath();
    mPath += nglPath(mpCreateEditLine->GetText());
    mPath.Create();
    
    //mRootPath = mpSelector->GetRootPath();
    mpSelector->SetRootPath(mPath.GetParent());
  }  
  return false;
}

