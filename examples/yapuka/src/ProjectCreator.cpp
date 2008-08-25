/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "ProjectCreator.h"
#include "nuiPane.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "Yapuka.h"
#include "MainWindow.h"

#include "nuiDialogSelectDirectory.h"
#include "nuiDialogSelectFile.h"
#include "nuiMessageBox.h"


ProjectCreator::ProjectCreator(nuiMainWindow* pWindow)
: nuiDialog(pWindow), mpWindow(pWindow), mEventSink(this)
{
  mpTimer = NULL;
  
  GetPreferences().GetString(PREFERENCES_PROJECTCREATOR, _T("nuiSourcePath"), mNuiSourcePath);
  if (!GetPreferences().GetString(PREFERENCES_PROJECTCREATOR, _T("nuiTargetPath"), mProjectTargetPath))
  {
    nglPath userPath(ePathUser);
    mProjectTargetPath = userPath.GetPathName();    
  }
  mProjectTargetPath.Append(_T("/newNuiProject"));
  
  
  
  nuiSimpleContainer* pPane = new nuiSimpleContainer();
  pPane->SetObjectName(_T("DialogPane"));
  pPane->SetUserSize(mpWindow->GetWidth()*.75f, 260);
  
  
  nuiVBox* pVBox = new nuiVBox(0);
  pVBox->SetExpand(nuiExpandShrinkAndGrow);
  pPane->AddChild(pVBox);
  
  
  //*******************************************************************
  // bloc for nui source directory
  //
  nuiVBox* pBloc = new nuiVBox(0);
  pBloc->SetObjectName(_T("DialogBloc"));
  pBloc->SetExpand(nuiExpandShrinkAndGrow);
  pVBox->AddCell(pBloc);
  
  pBloc->AddCell(new nuiLabel(_T("nui source directory:")));
  pBloc->SetCellExpand(pBloc->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // edit line
  nuiHBox* pBox = new nuiHBox(2);
  pBloc->AddCell(pBox);
  mpNuiSource = new nuiEditLine(mNuiSourcePath);
  mEventSink.Connect(mpNuiSource->TextChanged, &ProjectCreator::OnSourceTextChanged);
  
  pBox->SetCell(0, mpNuiSource);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  nuiButton* pBtn = new nuiButton(_T("browse"));
  pBox->SetCell(1, pBtn);
  mEventSink.Connect(pBtn->Activated, &ProjectCreator::OnBrowseSource);
  
  // separation line
  pBloc->AddCell(NULL);
  pBloc->SetCellPixels(pBloc->GetNbCells()-1, 10);

  
  // read-only information : nui3 selected directory checks
  nuiHBox* pHBox = new nuiHBox(0);
  pBloc->AddCell(pHBox);
  
  pHBox->AddCell(new nuiLabel(_T("nui3 project file: ")));
  mpNuiCheckProjectFile = new nuiLabel(_T(""));
  mNuiCheckProjectFile = false;
  pHBox->AddCell(mpNuiCheckProjectFile);
  pHBox->SetCellExpand(pHBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  pHBox->AddCell(NULL);
  pHBox->SetCellPixels(pHBox->GetNbCells()-1, 10);

  pHBox->AddCell(new nuiLabel(_T("nui3 tools: ")));
  mpNuiCheckTools = new nuiLabel(_T(""));
  mNuiCheckTools = false;
  pHBox->AddCell(mpNuiCheckTools);
  pHBox->SetCellExpand(pHBox->GetNbCells()-1, nuiExpandShrinkAndGrow);

  
  // read-only information : nui3 directory relative path from new project
  pBloc->AddCell(new nuiLabel(_T("nui3 relative path to project:")));
  mpNuiRelativeSource = new nuiLabel(mNuiRelativeSource.GetPathName());
  mpNuiRelativeSource->SetEnabled(false);
  pBloc->AddCell(mpNuiRelativeSource);
  
  
  
  
  
  //***********************************************************************
  // bloc for new project target
  //
  pBloc = new nuiVBox(0);
  pBloc->SetObjectName(_T("DialogBloc"));
  pBloc->SetExpand(nuiExpandShrinkAndGrow);
  pVBox->AddCell(pBloc);
  pVBox->SetCellExpand(pVBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  pBloc->AddCell(new nuiLabel(_T("new project target:")));
  
  pBox = new nuiHBox(2);
  pBloc->AddCell(pBox);
  mpProjectTarget = new nuiEditLine(mProjectTargetPath);
  mEventSink.Connect(mpProjectTarget->TextChanged, &ProjectCreator::OnTargetTextChanged);

  pBox->SetCell(0, mpProjectTarget);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  pBtn = new nuiButton(_T("browse"));
  pBox->SetCell(1, pBtn);
  mEventSink.Connect(pBtn->Activated, &ProjectCreator::OnBrowseTarget);  
  

  // separation line
  pBloc->AddCell(NULL);
  pBloc->SetCellPixels(pBloc->GetNbCells()-1, 10);

  // read-only bloc for project name, extracted from project target
  nglString projectFilename = mProjectTargetPath;
  projectFilename += _T("/newNuiProject.xcodeproj");
  pBloc->AddCell(new nuiLabel(_T("project file:")));
  mpProjectFilename = new nuiLabel(projectFilename);
  pBloc->AddCell(mpProjectFilename);
  mpProjectFilename->SetEnabled(false);
  
  
  InitDialog(_T("Create an empty nui project"), NULL, nuiDialog::eDialogButtonOk|nuiDialog::eDialogButtonCancel);
  SetContents(pPane);
  SetDefaultPos();
  
  mEventSink.Connect(DialogDone, &ProjectCreator::OnDialogdone);
 
  mpTimer = new nuiTimer(0.5 /*0.5s*/);
  mEventSink.Connect(mpTimer->Tick, &ProjectCreator::OnTimerTick);
  
  if (mNuiSourcePath != nglString::Null)
  {
    nuiEvent event;
    OnTimerTick(event);
  }
}


bool ProjectCreator::OnSourceTextChanged(const nuiEvent& rEvent)
{
  mpTimer->Stop();
  mpTimer->Start(false);
    
  return true;
}

bool ProjectCreator::OnTimerTick(const nuiEvent& rEvent)
{
  mpTimer->Stop();
  
  // check if nui3 project file has been found
  nglString text = mpNuiSource->GetText();
  text.Trim();
  
  nglPath path(text);
  
  nglPath proj = path;
  proj += nglPath(_T("nui3.xcodeproj"));
  
  if (proj.Exists())
  {
    mNuiCheckProjectFile = true;
    mpNuiCheckProjectFile->SetText(_T("found"));
    mpNuiCheckProjectFile->SetColor(eNormalTextFg, nuiColor(_T("green")));
  }
  else
  {
    mNuiCheckProjectFile = false;
    mpNuiCheckProjectFile->SetText(_T("not found!"));
    mpNuiCheckProjectFile->SetColor(eNormalTextFg, nuiColor(_T("red")));
  }
  
  // check if nui3 tool has been found (make_rc.py is taken as a reference)
  nglPath tool = path;
  tool += nglPath(_T("tools/make_rc.py"));
  
  if (tool.Exists())
  {
    mNuiCheckTools = true;
    mpNuiCheckTools->SetText(_T("found"));
    mpNuiCheckTools->SetColor(eNormalTextFg, nuiColor(_T("green")));
  }
  else
  {
    mNuiCheckTools = false;
    mpNuiCheckTools->SetText(_T("not found!"));
    mpNuiCheckTools->SetColor(eNormalTextFg, nuiColor(_T("red")));
  }
  
  // compute nui relative path to project
  mNuiRelativeSource = nglPath(mpNuiSource->GetText());
  mNuiRelativeSource.MakeRelativeTo(nglPath(mpProjectTarget->GetText()));
  mpNuiRelativeSource->SetText(mNuiRelativeSource.GetPathName());
  
  return true;
}


bool ProjectCreator::OnTargetTextChanged(const nuiEvent& rEvent)
{
  nglString text = mpProjectTarget->GetText();
  text.Trim();
  
  nglPath path(text);
  
  nglString newtext = text + _T("/") + path.GetNodeName() + _T(".xcodeproj");
  mpProjectFilename->SetText(newtext);
  
  mpTimer->Stop();
  mpTimer->Start(false);
  
  return true;
}



ProjectCreator::~ProjectCreator()
{
  mpTimer->Stop();
  delete mpTimer;
}


bool ProjectCreator::OnDialogdone(const nuiEvent& rEvent)
{
  DialogResult result = GetResult();
  
  // check everything is ok before generating the new project
  if (result == eDialogAccepted)
  {
    nglString source = mpNuiSource->GetText();
    nglString target = mpProjectTarget->GetText();
    source.Trim();
    target.Trim();
    
    if ((source == nglString::Null) || (target == nglString::Null))
    {
      nuiMessageBox* pMessageBox = new nuiMessageBox(GetMainWindow(), _T("Project Creator"), _T("source and target information can't be empty!"), eMB_OK);
      pMessageBox->QueryUser();   
      return true;  
    }

    nglPath sourcePath(source);
    if (!sourcePath.Exists())
    {
      nglString msg;
      msg.Format(_T("the nui source directory '%ls' does not exist!"), sourcePath.GetChars());
      nuiMessageBox* pMessageBox = new nuiMessageBox(GetMainWindow(), nglString(_T("Project Creator")), msg, eMB_OK);
      pMessageBox->QueryUser();     
      return true;  
    }
    
    if (!mNuiCheckProjectFile || !mNuiCheckTools)
    {
      nglString msg;
      msg.Format(_T("Parts of nui could not be found. Check the nui source directory or checkout the complete nui sources from libnui.net!"));
      nuiMessageBox* pMessageBox = new nuiMessageBox(GetMainWindow(), nglString(_T("Project Creator")), msg, eMB_OK);
      pMessageBox->QueryUser();     
      return true;        
    }

    mNuiSourcePath = source;
    mProjectTargetPath = target;
    nglPath path = nglPath(target);
    mProjectName = path.GetNodeName();

    nglPath targetPath(target);
    nglString xcodeproj = mProjectName + _T(".xcodeproj");
    targetPath += nglPath(xcodeproj);
    if (targetPath.Exists())
    {
      nglString msg;
      msg.Format(_T("the nui project '%ls' exists already!"), targetPath.GetChars());
      nuiMessageBox* pMessageBox = new nuiMessageBox(GetMainWindow(), nglString(_T("Project Creator")), msg, eMB_OK);
      pMessageBox->QueryUser();     
      return true;  
    }
    
    
      
 
    GetPreferences().Save();
    
    bool res = Make();
    
    return !res;
  }
  
  
  return false;
}



bool ProjectCreator::OnBrowseSource(const nuiEvent& rEvent)
{
  mNuiSourcePath.Trim();
  if (mNuiSourcePath == nglString::Null)
  {
    nglPath path(ePathUser);
    mNuiSourcePath = path.GetPathName();
  }

  nuiDialogSelectDirectory* pDialog = new nuiDialogSelectDirectory(mpWindow, _T("SELECT THE NUI SOURCE DIRECTORY"), mNuiSourcePath, nglPath(_T("/")));
  mEventSink.Connect(pDialog->DirectorySelected, &ProjectCreator::OnSourceSelected, (void*)pDialog);
  return true;
}


bool ProjectCreator::OnSourceSelected(const nuiEvent& rEvent)
{
  nuiDialogSelectDirectory* pDialog = (nuiDialogSelectDirectory*)rEvent.mpUser;
  mNuiSourcePath = pDialog->GetSelectedDirectory();
  mpNuiSource->SetText(mNuiSourcePath);
  
  OnTimerTick(rEvent);

 GetPreferences().SetString(PREFERENCES_PROJECTCREATOR, _T("nuiSourcePath"), mNuiSourcePath);

  return true;
}




bool ProjectCreator::OnBrowseTarget(const nuiEvent& rEvent)
{
  nglPath path = nglPath(mProjectTargetPath).GetParent();
  
  nuiDialogSelectDirectory* pDialog = new nuiDialogSelectDirectory(mpWindow, _T("ENTER THE NEW PROJECT TARGET"), path, nglPath(_T("/")));
  mEventSink.Connect(pDialog->DirectorySelected, &ProjectCreator::OnTargetSelected, (void*)pDialog);
  return true;
}


bool ProjectCreator::OnTargetSelected(const nuiEvent& rEvent)
{
  nuiDialogSelectDirectory* pDialog = (nuiDialogSelectDirectory*)rEvent.mpUser;
  mProjectTargetPath = pDialog->GetSelectedDirectory();
  mpProjectTarget->SetText(mProjectTargetPath);
  
  nglPath path(mProjectTargetPath);
  
  OnTargetTextChanged(rEvent);
  GetPreferences().SetString(PREFERENCES_PROJECTCREATOR, _T("nuiTargetPath"), path.GetParent().GetPathName());  
  
  return true;
}



bool ProjectCreator::Make()
{
  NGL_OUT(_T("Yapuka project gen\n"));
  

  // create target directory
  nglPath targetpath = nglPath(mProjectTargetPath);
  if (!targetpath.Create())
  {
    nglString msg;
    msg.Format(_T("creating target directory '%ls'"), targetpath.GetChars());
    return MsgError(msg);
  }

  NGL_OUT(_T("Yapuka project gen : target directory created '%ls'\n"), targetpath.GetChars());

    
  // create src folders
  nglPath targetsrcpath = targetpath;
  targetsrcpath += nglPath(_T("src"));
  if (!targetsrcpath.Create())
  {
    nglString msg;
    msg.Format(_T("creating src folder '%ls'"), targetsrcpath.GetChars());
    return MsgError(msg);
  }
    
  NGL_OUT(_T("Yapuka project gen : src folder created '%ls'\n"), targetsrcpath.GetChars());


  //copy the src folder contents
  nglPath srcpath = _T("rsrc:/project/src");
  std::list<nglPath> children;
  srcpath.GetChildren(&children);
  std::list<nglPath>::iterator it;
  for (it = children.begin(); it != children.end(); ++it)
  {
    const nglPath& srcpath = *it;
    nglPath dstpath = targetsrcpath;
    dstpath += srcpath.GetNodeName();
    
    nglString contents;
    
    nglIStream* piFile = srcpath.OpenRead();
    if (!piFile)
    {
      nglString msg;
      msg.Format(_T("opening for reading input file '%ls'"), srcpath.GetChars());
      return MsgError(msg);
    }

    nglOStream* poFile = dstpath.OpenWrite(false);
    if (!poFile)
    {
      nglString msg;
      msg.Format(_T("opening for writing output file '%ls'"), dstpath.GetChars());
      return MsgError(msg);
    }
    
    piFile->PipeTo(*poFile);
    delete poFile;
    delete piFile;
  
    NGL_OUT(_T("Yapuka project gen : created file '%ls'\n"), dstpath.GetChars());
  }
  
  // create xcodeproj folder
  nglPath projpath = targetpath;
  nglString projfolder = mProjectName + nglString(_T(".xcodeproj"));
  projpath += nglPath(projfolder);
  if (!projpath.Create())
  {
    nglString msg;
    msg.Format(_T("creatin xcodeproj folder '%ls'"), projpath.GetChars());
    return MsgError(msg);
  }
    
  NGL_OUT(_T("Yapuka project gen : project folder created '%ls'\n"), projpath.GetChars());
  
  
  // generate xcode project file
  nglPath projectfile = targetpath;
  projectfile += nglPath(projfolder);
  projectfile += nglPath(_T("project.pbxproj"));
  if (!GenerateFile(_T("rsrc:/project/project.pbxproj"), projectfile))
    return false;

  // generate visual studio project file
  nglString filename = mProjectName + nglString(_T(".vcproj"));
  projectfile = targetpath;
  projectfile += nglPath(filename);
  if (!GenerateFile(_T("rsrc:/project/project.vcproj"), projectfile))
    return false;

  // generate visual studio solution file
  filename = mProjectName + nglString(_T(".sln"));
  projectfile = targetpath;
  projectfile += nglPath(filename);
  if (!GenerateFile(_T("rsrc:/project/project.sln"), projectfile))
    return false;
    

  nglString msg;
  msg.Format(_T("nui project '%ls' successfully generated!"), mProjectName.GetChars());
  nuiMessageBox* pMessageBox = new nuiMessageBox(GetMainWindow(), nglString(_T("Project Creator")), msg, eMB_OK);
  pMessageBox->QueryUser();      
  
  return true;
  
  
}


bool ProjectCreator::GenerateFile(const nglPath& src, const nglPath& dst)
{
  uint32 srcsize = (uint32)src.GetSize();
  
  nglIStream* pFile = src.OpenRead();
  if (!pFile)
  {
    nglString msg;
    msg.Format(_T("reading input file '%ls'"), src.GetChars());
    return MsgError(msg);
  }

  char* str = new char[srcsize + 1];
  pFile->Read(str, srcsize, 1);
  str[srcsize] = 0;
  delete pFile;
  
  nglString contents(str);
  contents.Replace(_T("%YAPUKA_PROJECT%"), mProjectName);
  contents.Replace(_T("%YAPUKA_NUISOURCE%"), mNuiRelativeSource.GetPathName());

  
  nglOStream* poFile = dst.OpenWrite(false);
  if (!poFile)
  {
    nglString msg;
    msg.Format(_T("writing output file '%ls'"), dst.GetChars());
    return MsgError(msg);
  }

  char* ptr = contents.Export();
  
  poFile->Write(ptr, contents.GetLength(), 1);
  delete poFile;
  if (ptr)
    free(ptr);

  NGL_OUT(_T("Yapuka project gen : generated '%ls'\n"), dst.GetChars());
  return true;
}


bool ProjectCreator::MsgError(const nglString& error)
{
  nglString msg;
  msg.Format(_T("error %ls"), error.GetChars());
  NGL_OUT(msg);
  nuiMessageBox* pMessageBox = new nuiMessageBox(GetMainWindow(), _T("Project Creator"), msg, eMB_OK);
  pMessageBox->QueryUser();  
  return false;
}


