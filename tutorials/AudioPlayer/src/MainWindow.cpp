/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"

#include "AudioEngine.h"
#include "TimeBar.h"
#include "VolumeController.h"

#define NOTIF_FILEBROWSE_DONE _T("FILEBROWSE_DONE")

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  LoadCSS(_T("rsrc:/css/main.css"));
  
  RegisterWithManager(this, NOTIF_FILEBROWSE_DONE);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  AudioEngine* pAudioEngine = GetAudioEngine();
  
  // Time Bar
  nuiAttrib<uint64> posAttrib = pAudioEngine->GetAttribute(_T("position"));
  nuiAttrib<uint64> lengthAttrib = pAudioEngine->GetAttribute(_T("length"));
  mpTimeBar = new TimeBar(posAttrib, lengthAttrib, pAudioEngine->GetSampleRate());
  pLayoutBox->AddCell(mpTimeBar);
  
  // volume controller
  nuiAttrib<float> gainAttrib = pAudioEngine->GetAttribute(_T("gain"));
  nuiAttrib<bool> muteAttrib = pAudioEngine->GetAttribute(_T("mute"));
  VolumeController* pVolumeController = new VolumeController(gainAttrib, muteAttrib);
  pLayoutBox->AddCell(pVolumeController);
  
  // button in the second cell : we use the default decoration for this button, but you could use the css to assign your own decoration
  mpPlayButton = new nuiButton();
  mpPlayButton->SetPosition(nuiCenter);
  pLayoutBox->AddCell(mpPlayButton);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // click event on button
  mEventSink.Connect(mpPlayButton->Activated, &MainWindow::OnButtonClick);
  
  // label with border in the button (put the label string in the button's constructor if you don't need borders)
  nglString text;
  if (GetAudioEngine()->IsPlaying())
    text = _T("pause");
  else
    text = _T("play");
  mpPlayButtonLabel = new nuiLabel(text);
  mpPlayButtonLabel->SetPosition(nuiCenter);
  mpPlayButtonLabel->SetBorder(8,8);
  mpPlayButton->AddChild(mpPlayButtonLabel);
  mpPlayButton->SetEnabled(false);
  
  // load button
  nuiButton* pLoadBtn = new nuiButton(_T("Load"));
  pLayoutBox->AddCell(pLoadBtn);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  mEventSink.Connect(pLoadBtn->Clicked, &MainWindow::OnBrowse);
}



bool MainWindow::OnButtonClick(const nuiEvent& rEvent)
{  
  bool playing = GetAudioEngine()->IsPlaying();
  if (playing)
  {  
    GetAudioEngine()->Pause();
    mpPlayButtonLabel->SetText(_T("play"));
  }
  else
  {
    GetAudioEngine()->Play();
    mpPlayButtonLabel->SetText(_T("pause"));
  }
  
  
  return true; // means the event is caught and not broadcasted
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}

bool MainWindow::OnBrowse(const nuiEvent& rEvent)
{
  nglPath browsedPath             = nglPath(ePathUser);
  nglPath rootPath                = nglPath(_T("/"));
  nglString filter                = _T("*");
  bool showHiddenFiles            = false;
  nglString defaultEditLineValue  = nglString::Null;
  
  // nuiDialogSelectFile is a combo: it inherits from nuiDialog and contains a nuiFileSelector and a nuiEditLine.
  // It handles the user dialogs with the nuiDialog object (ok and cancel button), and with the file selector (double click on a file/folder).
  // It's been developped to easier the integration of a file selector in a application
  //
  // Of course, instead of using the nuiDialogSelectFile object, you are able to implement your own file selector,
  // using the nuiDialog class and nuiFileSelector class. Take a look to nuiDialogSelectFile to see how it's done.
  //
  mpDialog = new nuiDialogSelectFile(this, _T("SELECT A FILE TO LOAD"), browsedPath, rootPath, defaultEditLineValue, filter, showHiddenFiles);
  mEventSink.Connect(mpDialog->FileSelected, &MainWindow::OnDialogDone);    
  
  // we wish this dialog box to be modal
  mpDialog->DoModal();
  
  return true;
}



bool MainWindow::OnDialogDone(const nuiEvent& event)
{
  nuiDialog::DialogResult result = mpDialog->GetResult();
  
  // returning false in a dialog callback means: "close the dialog box".
  // returning true means "don't close the dialog box": it can be usefull if the user choice doesn't match with the application constraints for instance.
  
  if (result == nuiDialog::eDialogCanceled)
  {
    return false;
  }
  
  if (result == nuiDialog::eDialogAccepted)
  {
    // store the user choice
    mBrowsedFile = mpDialog->GetSelectedFile();
    
    // send an asynchrone message to the nuiNotificationManager (the MainWindow itself in that case), to tell the application to go on
    PostNotification(new nuiNotification(NOTIF_FILEBROWSE_DONE));
    
    // note: instead of using an asynchrone message, you can make a synchrone call to another method.
    // But in that case, returning false will close and delete the dialog, which will make your application crash since
    // you are still in the dialog process (OnDialogDone is an event callback from the dialog process).
    //
    // To avoid the crash, you could:
    // - hide the dialog: mpDialog->SetVisible(false)
    // - return true (<=> "don't close and delete the dialog automatically")
    // - delete manually the dialog when your main process exits
    // this solution is a bit ugly, and using an asynchrone message to get out of the dialog process is much better.
  }
  
  return false;
}



// virtual from nuiNotificationObserver
void MainWindow::OnNotification(const nuiNotification& rNotif)
{
  const nglString& rName = rNotif.GetName();
  
  // notification receiver
  if (!rName.Compare(NOTIF_FILEBROWSE_DONE))
  {
    bool loaded = GetAudioEngine()->LoadFile(mBrowsedFile);
    if (loaded)
    {
      mpPlayButton->SetEnabled(true);
    }
    
    return;
  }
}

