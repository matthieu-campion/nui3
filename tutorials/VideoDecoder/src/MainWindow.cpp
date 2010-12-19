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
#include "nuiHBox.h"

#include "nuiVideoDecoder.h"

#define NOTIF_FILEBROWSE_DONE _T("FILEBROWSE_DONE")


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)),
  mEventSink(this),
  mpVideoDecoder(NULL),
  mpNglImage(NULL),
  mpImage(NULL),
  mpTexture(NULL),
  mTimer(1.f / 25.f)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  RegisterObserver(NOTIF_FILEBROWSE_DONE, this);
  
  LoadCSS(_T("rsrc:/css/main.css"));
  
  mEventSink.Connect(mTimer.Tick, &MainWindow::OnTimerTick);
}

MainWindow::~MainWindow()
{
  if (mpVideoDecoder)
    delete mpVideoDecoder;
  
  mpTexture->Release();
}

void MainWindow::OnCreation()
{
  // a vertical box for page layout
  mpMainVBox = new nuiVBox(0);
  mpMainVBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(mpMainVBox);

  {
    nuiHBox* pOpenBox = new nuiHBox();
    mpMainVBox->AddCell(pOpenBox);
    
    nuiButton* pOpenBtn = new nuiButton(_T("open"));
    mEventSink.Connect(pOpenBtn->Activated, &MainWindow::OnBrowse);
    pOpenBox->AddCell(pOpenBtn);
    
    mpVideoNameLabel = new nuiLabel(_T("no file loaded"));
    pOpenBox->AddCell(mpVideoNameLabel);
  }
  
  {
    nuiGrid* pInfoGrid= new nuiGrid(2, 4);
    mpMainVBox->AddCell(pInfoGrid);
    
    mpDurationLabel     = new nuiLabel(_T("unknown"));
    mpWidthLabel        = new nuiLabel(_T("unknown"));
    mpHeightLabel       = new nuiLabel(_T("unknown"));
    mpBitsPerPixelLabel = new nuiLabel(_T("unknown"));
    
    pInfoGrid->SetCell(0, 0, new nuiLabel(_T("duration:")));
    pInfoGrid->SetCell(1, 0, mpDurationLabel);
    
    pInfoGrid->SetCell(0, 1, new nuiLabel(_T("width:")));
    pInfoGrid->SetCell(1, 1, mpWidthLabel);
    
    pInfoGrid->SetCell(0, 2, new nuiLabel(_T("height:")));
    pInfoGrid->SetCell(1, 2, mpHeightLabel);
    
    pInfoGrid->SetCell(0, 3, new nuiLabel(_T("bits per pixel:")));
    pInfoGrid->SetCell(1, 3, mpBitsPerPixelLabel);
  }
  
  mpImage = new nuiImage();
  mpMainVBox->AddCell(mpImage, nuiCenter);
  
  {
    nuiHBox* pBox = new nuiHBox();
    mpMainVBox->AddCell(pBox);
    
    nuiButton* pBackBtn = new nuiButton(_T("<"));
    mEventSink.Connect(pBackBtn->Activated, &MainWindow::OnBackBtnClicked);
    
    nuiButton* pFastBackBtn = new nuiButton(_T("<<"));
    mEventSink.Connect(pFastBackBtn->Activated, &MainWindow::OnFastBackBtnClicked);
    
    nuiButton* pForwardBtn = new nuiButton(_T(">"));
    mEventSink.Connect(pForwardBtn->Activated, &MainWindow::OnForwardBtnClicked);
    
    nuiButton* pFastForwardBtn = new nuiButton(_T(">>"));
    mEventSink.Connect(pFastForwardBtn->Activated, &MainWindow::OnFastForwardBtnClicked);
    
    pBox->AddCell(pFastBackBtn);
    pBox->AddCell(pBackBtn);
    pBox->AddCell(pForwardBtn);
    pBox->AddCell(pFastForwardBtn);
  }
  
  {
    nuiButton* pBtn = new nuiButton(_T("play"));
    mpMainVBox->AddCell(pBtn);
    mEventSink.Connect(pBtn->Activated, &MainWindow::OnPlayBtnClicked);
  }

}



void MainWindow::OnBrowse(const nuiEvent& rEvent)
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
}

void MainWindow::OnDialogDone(const nuiEvent& event)
{
  nuiDialog::DialogResult result = mpDialog->GetResult();
  
    // returning false in a dialog callback means: "close the dialog box".
    // returning true means "don't close the dialog box": it can be usefull if the user choice doesn't match with the application constraints for instance.
  
  if (result == nuiDialog::eDialogCanceled)
  {
    return;
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
}

void MainWindow::OnNotification(const nuiNotification& rNotif)
{
  const nglString& rName = rNotif.GetName();
  
    // notification receiver
  if (!rName.Compare(NOTIF_FILEBROWSE_DONE))
  {
    bool loaded = LoadVideo();
    if (loaded)
    {
      UpdateVideoName();
      UpdateVideoImage();
    }
    
    return;
  }
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

bool MainWindow::LoadVideo()
{
  nuiVideoDecoder* pVideoDecoder = new nuiVideoDecoder(mBrowsedFile);
  if (!pVideoDecoder->IsValid())
  {
    return false;
  }
  
  if (mpVideoDecoder)
    delete mpVideoDecoder;
  
  if (mpTexture)
  {
    mpTexture->Release();
    mpTexture = NULL;
  }
  
  if (mpNglImage)
    mpNglImage = NULL;
    
  mpVideoDecoder = pVideoDecoder;  
  return true;
}

void MainWindow::UpdateVideoName()
{
  if (!mpVideoDecoder)
  {
     mpVideoNameLabel->SetText(_T("no file loaded"));
    return;
  }
  
  mpVideoNameLabel->SetText(mBrowsedFile.GetNodeName());
}

void MainWindow::UpdateVideoInfos()
{
  if (!mpNglImage)
  {
    mpDurationLabel->SetText(_T("unknown"));
    mpWidthLabel->SetText(_T("unknown"));
    mpHeightLabel->SetText(_T("unknown"));
    mpBitsPerPixelLabel->SetText(_T("unknown"));
    return;
  }
  
  double duration = mpVideoDecoder->GetDuration();
  uint width = mpNglImage->GetWidth();
  uint height = mpNglImage->GetHeight();
  uint bitsDepth = mpNglImage->GetBitDepth();
  NGL_ASSERT(duration);
  NGL_ASSERT(width);
  NGL_ASSERT(height);
  NGL_ASSERT(bitsDepth);
  
  nglString durStr;
  durStr.Format(_T("%lf s"), duration);
  mpDurationLabel->SetText(durStr);
  
  nglString widthStr;
  widthStr.Format(_T("%u"), width);
  mpWidthLabel->SetText(widthStr);
  
  nglString heightStr;
  heightStr.Format(_T("%u"), height);
  mpHeightLabel->SetText(heightStr);
  
  nglString bitsStr;
  bitsStr.Format(_T("%u"), bitsDepth);
  mpBitsPerPixelLabel->SetText(bitsStr);
  
}

void MainWindow::UpdateVideoImage()
{
  if (!mpVideoDecoder)
    return;
  
//  mpNglImage = mpVideoDecoder->GetCurrentImage();
//  UpdateVideoInfos();
//  
//  if (!mpTexture)
//  {
//    mpTexture = nuiTexture::GetTexture(mpNglImage, false);
//    mpTexture->SetRetainBuffer(true);
//    mpImage->SetTexture(mpTexture);
//  }
//  
//  mpTexture->ForceReload(false);
//  mpImage->Invalidate();

  
  // with texture:
  
//  if (mpTexture)
//  {
//    mpTexture->Release();
//    mpTexture = NULL;
//  }
    
  

//  if (!mpTexture)
  {
    mpTexture = mpVideoDecoder->GetCurrentTexture();
    if (!mpTexture)
      return;
    mpImage->SetTexture(mpTexture);
  }
  
//  mpTexture->ForceReload(false);
  mpImage->Invalidate();
}

void MainWindow::OnBackBtnClicked(const nuiEvent& rEvent)
{
  if (!mpVideoDecoder)
    return;
  
  mpVideoDecoder->GoToPrevFrame();
  UpdateVideoImage();
}

void MainWindow::OnFastBackBtnClicked(const nuiEvent& rEvent)
{
  if (!mpVideoDecoder)
    return;
  
  for (uint32 i = 0; i < 25; i++)
    mpVideoDecoder->GoToPrevFrame();
  UpdateVideoImage();
}

void MainWindow::OnForwardBtnClicked(const nuiEvent& rEvent)
{
  if (!mpVideoDecoder)
    return;
  
  mpVideoDecoder->GoToNextFrame();

  UpdateVideoImage();
}

void MainWindow::OnFastForwardBtnClicked(const nuiEvent& rEvent)
{
  if (!mpVideoDecoder)
    return;
  
  for (uint32 i = 0; i < 25; i++)
    mpVideoDecoder->GoToNextFrame();
  UpdateVideoImage();
}

void MainWindow::OnPlayBtnClicked(const nuiEvent& rEvent)
{
  if (mTimer.IsRunning())
    mTimer.Stop();
  else
    mTimer.Start(true, true);
}

void MainWindow::OnTimerTick(const nuiEvent& rEvent)
{
  mpVideoDecoder->GoToNextFrame();
  UpdateVideoImage();
}