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

#include "nuiAudioDevice.h"
#include "nuiAudioEngine.h"
#include "nuiWaveWriter.h"
#include "nglLockFreeFifo.h"

/*
 * MainWindow
 */

class Recorder : public nglThread
{
public:
  Recorder(nuiSampleWriter* pWriter, int32 BufferSize)
  : mpWriter(pWriter),
    mFifo(BufferSize)
  {
    mContinue = true;
    mChannelCount = 1;
  }
  
  void AddSamples(const std::vector<const float*>& Samples, int32 SampleFrames)
  {
    //printf("record %d\n", SampleFrames);
    mChannelCount = Samples.size();
    for (uint32 s = 0; s < SampleFrames; s++)
    {
      for (uint32 ch = 0; ch < Samples.size(); ch++)
      {
        NGL_ASSERT(mFifo.CanWrite());
        mFifo.Put(Samples[ch][s]);
      }
    }
    mEvent.Set();
  }
  
  void OnStart()
  {
    std::vector<float> samples;
    while (mContinue)
    {
      mEvent.Wait();
      mEvent.Reset();

      while (mFifo.CanRead())
      {
        const float sample = mFifo.Get();
        samples.push_back(sample);
      }
      
      mpWriter->Write(&samples[0], samples.size() / mChannelCount, eSampleFloat32);
      //printf("write %d\n", samples.size() / mChannelCount);
      samples.clear();
    }
    
    mpWriter->Finalize();

  }
  
  void Stop()
  {
    mContinue = false;
    mEvent.Set();
  }
  
private:
  nuiSampleWriter* mpWriter;
  nglLockFreeFifo<float> mFifo;
  nglSyncEvent mEvent;
  bool mContinue;
  uint32 mChannelCount;
};

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

  nglPath p = nglPath(ePathUserDocuments);
  p += nglPath(_T("Output.wav"));
  mpStream = p.OpenWrite();
  mpWriter = new nuiWaveWriter(*mpStream);
  nuiSampleInfo Infos;
  Infos.SetSampleRate(44100);
  Infos.SetChannels(1);
  Infos.SetBitsPerSample(16);
  mpWriter->WriteInfo(Infos);
  mpRecorder = new Recorder(mpWriter, 1024 * 64);
  mpRecorder->Start(); // Start the recording thread

  mpAudioEngine = new nuiAudioEngine(44100, 1024 * 8, nuiAudioEngine::eStereo);
  mpAudioEngine->SetInputProcessDelegate(nuiMakeDelegate(mpRecorder, &Recorder::AddSamples));
}

MainWindow::~MainWindow()
{
  StopRecording();
}

void MainWindow::OnCreation()
{
  // a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetObjectName(_T("MainBox"));
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  // image in the first box's cell
  nuiImage* pImg = new nuiImage();
  pImg->SetObjectName(_T("MyImage"));
  pImg->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pImg);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // button in the second cell : we use the default decoration for this button, but you could use the css to assign your own decoration
  nuiButton* pButton = new nuiButton();
  pButton->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pButton);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // click event on button
  mEventSink.Connect(pButton->Activated, &MainWindow::OnButtonClick);
  
  // label with border in the button (put the label string in the button's constructor if you don't need borders)
  nuiLabel* pButtonLabel = new nuiLabel(_T("click to stop recording!"));
  pButtonLabel->SetPosition(nuiCenter);
  pButtonLabel->SetBorder(8,8);
  pButton->AddChild(pButtonLabel);
  
  // label with decoration in the third cell
  mMyLabel = new nuiLabel(_T("my label"));
  mMyLabel->SetObjectName(_T("MyLabel"));
  mMyLabel->SetPosition(nuiCenter);
  pLayoutBox->AddCell(mMyLabel);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  
}

void MainWindow::StopRecording()
{
  if (!mpAudioEngine)
    return;
  delete mpAudioEngine;
  mpRecorder->Stop();
  mpRecorder->Join();
  delete mpRecorder;
  delete mpWriter;
  delete mpStream;
  
  mpAudioEngine = NULL;
  mpRecorder = NULL;
  mpWriter = NULL;
  mpStream = NULL;
}





void MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  StopRecording();
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
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
