/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiGLPainter.h"
#include "nuiSoftwarePainter.h"

#include "nuiAudioDecoder.h"
#include "nuiAudioConvert.h"

extern void* gmpNUI_AndroidBridge;

class nuiAndroidBridge : public nglContext, public nuiTopLevel
{
public:
  nuiAndroidBridge()
  : nuiTopLevel(_T(""))
  {
    *((nuiAndroidBridge**)&gmpNUI_AndroidBridge) = this;

    mWidth = 100;
    mHeight = 200;
    mClearBackground = false;
    
    mpAudioStream = NULL;
    mpReader = NULL;

    EnableRenderCache(true);
    EnablePartialRedraw(false);    
    SetRect(nuiRect(0.0f, 0.0f, (nuiSize)mWidth, (nuiSize)mHeight));
    
    
    mpAudioDevice = nuiAudioDeviceManager::Get().GetDefaultOutputDevice();
    NGL_OUT(_T("Default output: %s\n"), mpAudioDevice->GetName().GetChars());
    
    std::vector<uint32> InputChannels;
    std::vector<uint32> OutputChannels;
    OutputChannels.push_back(0);
    OutputChannels.push_back(1);
    
    nglPath audioPath(_T("/sdcard/mat/test.mp3"));
    mpAudioStream = audioPath.OpenRead();
    if (mpAudioStream)
    {
      LOGI("mpAudioStream OK");
      mpReader = new nuiAudioDecoder(*mpAudioStream);
      nuiSampleInfo info;
      if (!mpReader->GetInfo(info))
      {
        LOGI("mpReader ERROR");
        delete mpReader;
        mpReader = NULL;
        delete mpAudioStream;
        mpAudioStream = NULL;
      }
      else
      {
        LOGI("mpReader OK");
      }
    }
    else
      LOGI("can't open %ls", audioPath.GetPathName().GetChars());
    
    bool res = mpAudioDevice->Open(InputChannels, OutputChannels, 44100, 1024, nuiMakeDelegate(this, &nuiAndroidBridge::ProcessAudio));
  }
  
  virtual ~nuiAndroidBridge()
  {
    if (mpAudioDevice)
      delete mpAudioDevice;
    if(mpReader)
      delete mpReader;
    if (mpAudioStream)
      delete mpAudioStream;
    
    
    *((nuiAndroidBridge**)&gmpNUI_AndroidBridge) = this;
  }
  
  void Display()
  {
    Paint();
  }
  
  ///< Android call backs:
  //------------------------------------------------------------------------
  // Window resize function
  //------------------------------------------------------------------------
  static void androidResize(int width, int height)
  { 
    ((nuiAndroidBridge*)gmpNUI_AndroidBridge)->OnResize(width, height);
  }
  
  //------------------------------------------------------------------------
  // Function that handles keyboard inputs
  //------------------------------------------------------------------------
  static void androidKeyboard(unsigned char key, int x, int y)
  {
  }
  
  //------------------------------------------------------------------------
  // Moves the screen based on mouse pressed button
  //------------------------------------------------------------------------
  static void androidMotion(int x, int y)
  {
    nglMouseInfo Info;
    Info.Buttons = 0;
    Info.X = x;
    Info.Y = y;
    ((nuiAndroidBridge*)gmpNUI_AndroidBridge)->OnMouseMove(Info);
  }
  
  //------------------------------------------------------------------------
  // Function that handles mouse input
  //------------------------------------------------------------------------
  static void androidMouse(int button, int state, int x, int y)
  {
    nglMouseInfo Info;
    Info.Buttons = 0;
    switch (button)
    {
      case 0:
        Info.Buttons = nglMouseInfo::ButtonLeft;
        break;
      case 1:
        Info.Buttons = nglMouseInfo::ButtonRight;
        break;
      case 2:
        Info.Buttons = nglMouseInfo::ButtonMiddle;
        break;
    }
    Info.X = x;
    Info.Y = y;
    if (state)
      ((nuiAndroidBridge*)gmpNUI_AndroidBridge)->OnMouseUnclick(Info);
    else
      ((nuiAndroidBridge*)gmpNUI_AndroidBridge)->OnMouseClick(Info);
    
  }
  
  
protected:
  
  void Paint()
  {
    do 
    {
      FillTrash();
      
      GetIdealRect();
      SetLayout(nuiRect(0, 0, mWidth, mHeight));
      
      EmptyTrash();
    } while (IsTrashFull());
    
    
    FillTrash();
    
    nuiDrawContext* pContext = GetDrawContext();
    nuiPainter* pOldPainter = pContext->GetPainter();
    
    pContext->GetPainter()->ResetStats();
    //nuiSoftwarePainter* pCTX = dynamic_cast<nuiSoftwarePainter*>(pContext->GetPainter());
    
    BeginSession();
    
    pContext->StartRendering();
    pContext->GetPainter()->SetState(nuiRenderState(), true);
    
    //static int counter = 0;
    //NGL_OUT(_T("%d OnPaint %d - %d\n"), counter++, DrawFullFrame, RestorePartial);
    
    pContext->LoadIdentity();
    
    pContext->MultMatrix(GetMatrix());
    DrawWidget(pContext);
    
    pContext->StopRendering();
    EmptyTrash();
    
    //   pContext->EndSession();
    pContext->SetPainter(pOldPainter);
    
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_TEXTURE_2D);
  }
  
  void OnResize(uint Width, uint Height)
  {
    mWidth = Width;
    mHeight = Height;
    FillTrash();
    nuiRect Rect;
    Rect.mRight=(nuiSize)Width;
    Rect.mBottom=(nuiSize)Height;
    //SetLayout(Rect);
    
    nuiDrawContext* pCtx = GetDrawContext();
    pCtx->SetSize(Width,Height);
    
    //NGL_OUT(_T("(OnResize)nglWindow::Invalidate()\n"));;
    InvalidateLayout();
    EmptyTrash();
  }
  
  void OnCreation()
  {
    EmptyTrash();
  }
  
  void OnDestruction()
  {
    //NGL_OUT(_T("OnDestruction\n"));
    EmptyTrash();
  }
  
  void OnActivation()
  {
    //OUT("OnActivation\n");
    EmptyTrash();
    CancelGrab();
    mMouseInfo.Buttons = 0;
  }
  
  void OnDesactivation()
  {
    //OUT("OnDesactivation\n");
    EmptyTrash();
    CancelGrab();
    mMouseInfo.Buttons = 0;
  }
  
  void OnState (nglWindow::StateInfo State)
  {
    //OUT("OnState\n");
    EmptyTrash();
  }
  
  
  void BroadcastInvalidate(nuiWidgetPtr pSender)
  {
    nuiTopLevel::BroadcastInvalidate(pSender);
    
//    glutPostRedisplay();
  }
  
  void BroadcastInvalidateRect(nuiWidgetPtr pSender, const nuiRect& rRect)
  {
    nuiTopLevel::BroadcastInvalidateRect(pSender, rRect);
    
//    glutPostRedisplay();
  }
  
  void BroadcastInvalidateLayout(nuiWidgetPtr pSender, bool BroadCastOnly)
  {
    nuiTopLevel::BroadcastInvalidateLayout(pSender, BroadCastOnly);
//    glutPostRedisplay();
  }
  
  bool IsKeyDown (nglKeyCode Key) const
  {
    //return mpNGLWindow->IsKeyDown(Key);
    return false;
  }
  
  bool SetMouseCursor(nuiMouseCursor Cursor)
  {
    //return mpNGLWindow->SetCursor(Cursor);
    return true;
  }
  
  bool OnMouseMove(nglMouseInfo& rInfo)
  {
    return CallMouseMove(rInfo);
  }
  
  bool OnMouseClick(nglMouseInfo& rInfo)
  {
    return CallMouseClick(rInfo);
  }
  
  bool OnMouseUnclick(nglMouseInfo& rInfo)
  {
    return CallMouseUnclick(rInfo);
  }
  
  bool OnKeyUp(const nglKeyEvent& rEvent)
  {
    return CallKeyUp(rEvent);
  }
  
  bool OnKeyDown(const nglKeyEvent& rEvent)
  {
    return CallKeyDown(rEvent);
  }
  
  virtual nglContext* GetNGLContext()
  {
    return this;
  }
  
  virtual void BeginSession()
  {
    
  }
  
  virtual bool MakeCurrent() const
  {
    return true;
  }
  
  virtual void EndSession()
  {
    
  }
  
  nglContext* GetNGLContext() const
  {
    return const_cast<nglContext*>((nglContext*)this);
  }
  
  
  void ProcessAudio(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames)
  {
    if (mpReader)
    {
      uint32 channels = rOutput.size();      
      std::vector<void*> temp;
      for (uint32 c = 0; c < channels; c++)
        temp.push_back(rOutput[c]);
      mpReader->ReadDE(temp, SampleFrames, eSampleFloat32);
    }
  }
  
  
protected:
  uint32 mWidth, mHeight;
  
  nuiAudioDevice* mpAudioDevice;
  nglIStream* mpAudioStream;
  nuiSampleReader* mpReader;
};

