/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "Gui/Gui.h"
#include "Gui/Navigator.h"
#include "Gui/Inspector.h"
#include "Gui/Toolbar.h"
#include "Sql/Sql.h"
#include "Application.h"
#include "MainWindow.h"

#include "nuiDialogSelectFile.h"



Gui::Gui()
: nuiSimpleContainer(), mEventSink(this)
{
  nuiVBox* pMainBox = new nuiVBox(2);
  pMainBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pMainBox);
  
  // TOOLBAR **************************************************
  mpToolbar = new Toolbar();
  pMainBox->SetCell(0, mpToolbar);
  
  mpToolbar->AddButton(BUTTON_OPEN);
  mpToolbar->AddButton(BUTTON_REFRESH);
  
  mEventSink.Connect(mpToolbar->Activated, &Gui::OnToolbarActivated, (void*)mpToolbar);
  
  
  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  pMainBox->SetCell(1, pSplitter);
  pMainBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  
  pSplitter->SetMasterChild(false);
  
  mpNavigator = new Navigator();
  pSplitter->AddChild(mpNavigator);
  
  mpInspector = new Inspector();
  pSplitter->AddChild(mpInspector);
  
  pSplitter->SetHandlePos(33.f);
}

Gui::~Gui()
{

}


bool Gui::OnToolbarActivated(const nuiEvent& rEvent)
{ 
  Toolbar* pToolbar = (Toolbar*)rEvent.mpUser;
  const nglString& rButton = pToolbar->GetActivatedButton();
  
  if (rButton == BUTTON_OPEN)
  {
    OpenFileDialog();
    return true;
  }
  
  if (rButton == BUTTON_REFRESH)
  {
    mpNavigator->Update();    
    return true;
  }
  
  return true;
}


void Gui::OpenFileDialog()
{
  nglPath path;
  GetPreferences().GetPath(KEY_USER, _T("LastOpen"), path);
  nglPath rootpath = nglPath(_T("/"));
  
  nuiDialogSelectFile* pDialog = new nuiDialogSelectFile(GetMainWindow(), _T("Open a sqlite database"), path, rootpath, nglString::Null, _T("db"));
  mEventSink.Connect(pDialog->FileSelected, &Gui::OnFileSelected, (void*)pDialog);  
}

bool Gui::OnFileSelected(const nuiEvent& rEvent)
{
  nuiDialogSelectFile* pDialog = (nuiDialogSelectFile*)rEvent.mpUser;
  
  nglPath path = pDialog->GetSelectedFile();
  
  if (!path.IsLeaf() || !path.Exists())
    return true;
  
  
  nuiNotification* pNotif = new nuiNotification(NOTIF_DB_SELECTED);
  pNotif->SetToken(new nuiToken<nglPath>(path));
  GetMainWindow()->PostNotification(pNotif);
  
  return false;
}


void Gui::OpenDB(const nglPath& rPath)
{  
  GetPreferences().SetPath(KEY_USER, _T("LastOpen"), rPath);  
  Sql::Get()->SetDB(rPath);
  
  nglString title = rPath.GetPathName();
  if (title.GetLength() > 30)
  {
    title.DeleteLeft(title.GetLength() > 30);
    title.Prepend(_T("[...]"));
  }
  

  GetApp()->SetWindowTitle(title);

  
  mpNavigator->Update();
  mpInspector->Clear();
  
}

// virtual 
void Gui::ConnectTopLevel()
{
  RegisterWithManager(GetMainWindow(), NOTIF_DB_SELECTED);
  RegisterWithManager(GetMainWindow(), NOTIF_TABLE_SELECTED);
  
  // open automatically
  nglPath path;
  if (!GetPreferences().GetPath(KEY_USER, _T("LastOpen"), path))
    OpenFileDialog();
  else 
    OpenDB(path);
  
}


// virtual 
void Gui::OnNotification(const nuiNotification& rNotif)
{
  const nglString& name = rNotif.GetName();
  
  if (name == NOTIF_DB_SELECTED)
  {
    nglPath path;
    if (!nuiGetTokenValue<nglPath>(rNotif.GetToken(), path))
      return;
    
    OpenDB(path);
    
    return;
  }
  
  if (name == NOTIF_TABLE_SELECTED)
  {
    nglString table;
    if (!nuiGetTokenValue<nglString>(rNotif.GetToken(), table))
      return;
    
    mpInspector->Update(table);
    
    return;
  }
}