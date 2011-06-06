/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#include "nuiAudioDevice_DirectSound.h"
#include "nuiAudioConvert.h"
#include "nglThread.h"
#include "nglRingBuffer.h"

#pragma comment( lib , "dsound.lib" )

#define API_NAME _T("DirectSound")

inline float	fmin(float a, float b) { if (a < b) return a; else return b; }
inline float	fmax(float a, float b) { if (a < b) return b; else return a; }
inline float	fclamp(float f, float xmin, float xmax) {
	assert( xmin <= xmax );
	return fmax(xmin, fmin(f, xmax));
}

#undef NGL_OUT
void NGL_OUT(const nglChar* pFormat, ...)
{
}

void NGL_OUT2(nglChar* msg)
{
  OutputDebugString(msg);
}


#define NUIAUDIODEVICE_DS_MSG_PROCESS _T("NUIAUDIODEVICE_DS_MSG_PROCESS")
#define NUIAUDIODEVICE_DS_MSG_OUTPUT _T("NUIAUDIODEVICE_DS_MSG_OUTPUT")


#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
  EXTERN_C const GUID DECLSPEC_SELECTANY name \
  = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

// DirectSound default playback device GUID {DEF00000-9C6D-47ED-AAF1-4DDA8F2B5C03}
DEFINE_GUID(DSDEVID_DefaultPlayback, 0xdef00000, 0x9c6d, 0x47ed, 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03);

// DirectSound default capture device GUID {DEF00001-9C6D-47ED-AAF1-4DDA8F2B5C03}
DEFINE_GUID(DSDEVID_DefaultCapture, 0x0def00001, 0x9c6d, 0x47ed, 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03);

// IDirectSoundNotify
DEFINE_GUID(IID_IDirectSoundNotify, 0x0b0210783, 0x89cd, 0x11d0, 0xaf, 0x08, 0x00, 0xa0, 0xc9, 0x25, 0xcd, 0x16);


class nuiAudioDevice_DirectSound;



//***************************************************
//
// Processing Thread class
//
class nuiAudioDevice_DS_ProcessingTh : public nglThread
{
public:

  nuiAudioDevice_DS_ProcessingTh(nuiAudioDevice_DirectSound* pDev, HANDLE hEvent0, HANDLE hEvent1, nuiAudioProcessFn pProcessFunction);
  virtual ~nuiAudioDevice_DS_ProcessingTh();

  void Stop();

private:
  virtual void OnStart(); ///< Main thread method

  void Process(uint pos);
  uint32 WriteToRingBuf(const std::vector<float*>& inbuf, uint32 nbSampleFrames, uint32 nbChannels);

  bool mContinue;

  uint32 mBufferSize;
  uint32 mInputNbChannels;
  uint32 mOutputNbChannels;
  LPDIRECTSOUNDCAPTUREBUFFER mpDSInputBuffer;
  nglRingBuffer* mpRingBuffer;

  HANDLE mEvents[2];
  int16* mpLocalBuf;

  std::vector<const float*> mFloatInputBuf;
  std::vector<float*> mFloatOutputBuf;

  nuiAudioProcessFn mpProcessFunction;


};




//***************************************************
//
// Output Thread class
//

class nuiAudioDevice_DS_OutputTh : public nglThread
{
public:

  nuiAudioDevice_DS_OutputTh(nuiAudioDevice_DirectSound* pDev, HANDLE inputEvent0, HANDLE inputEvent1, HANDLE outputEvent0, HANDLE outputEvent1);
  virtual ~nuiAudioDevice_DS_OutputTh();

  void Stop();

private:
  virtual void OnStart(); ///< Main thread method

  void Process(uint pos);
  uint32 ReadFromRingBuf(uint32 nbSampleFrames, std::vector<float*>& outbuf, uint32 nbChannels);

  bool mContinue;

  uint32 mBufferSize;
  uint32 mNbChannels;
  LPDIRECTSOUNDBUFFER mpDSOutputBuffer;
  nglRingBuffer* mpRingBuffer;

  HANDLE mInputEvents[2];
  HANDLE mOutputEvents[2];
  std::vector<float*> mFloatOutputBuf;
  int16* mpLocalBuf;
};




//***************************************************
//
// DirectSound Audio Device Class
//
class nuiAudioDevice_DirectSound: public nuiAudioDevice
{
public:
  nuiAudioDevice_DirectSound(GUID IGuid, GUID OGuid, const nglString& rInName, const nglString& rOutName, const nglString& rInModule, const nglString& rOutModule);
  nuiAudioDevice_DirectSound();
  virtual ~nuiAudioDevice_DirectSound();

  virtual bool Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction);
  virtual bool Close();

  bool HasInput();
  bool HasOutput();

  LPDIRECTSOUNDCAPTUREBUFFER GetInputBuffer();
  LPDIRECTSOUNDBUFFER GetOutputBuffer();

  DSBPOSITIONNOTIFY GetInputNotify(uint pos);
  DSBPOSITIONNOTIFY GetOutputNotify(uint pos);

  uint32 GetBufferSize();

  uint32 GetNbInputChannels();
  uint32 GetNbOutputChannels();


  nglRingBuffer* GetRingBuffer();

  HANDLE GetInputEvent(uint pos);


protected:
  void Init();


  bool mHasInput;
  bool mHasOutput;

  GUID mIDeviceID;
  GUID mODeviceID;

  LPDIRECTSOUND mpDirectSound;
  LPDIRECTSOUNDCAPTURE mpDirectSoundCapture;

  LPDIRECTSOUNDCAPTUREBUFFER mpInputBuffer;
  LPDIRECTSOUNDBUFFER mpOutputBuffer;
  nglRingBuffer* mpRingBuffer;

  DSBPOSITIONNOTIFY mInputPosNotify[2];
  DSBPOSITIONNOTIFY mOutputPosNotify[2];
  HANDLE mNotifInputEvent[2];
  HANDLE mNotifOutputEvent[2];

  nuiAudioProcessFn mAudioProcessFn;
  std::vector<uint32> mActiveInputChannels;
  std::vector<uint32> mActiveOutputChannels;


  double mSampleRate;
  uint32 mBufferSize;


  nglString mInName;
  nglString mOutName;

  // threads
  nuiAudioDevice_DS_ProcessingTh* mpProcessingTh;
  nuiAudioDevice_DS_OutputTh* mpOutputTh;
};


nuiAudioDeviceAPI_DirectSound::nuiAudioDeviceAPI_DirectSound()
{
  mName = API_NAME;
}

nuiAudioDeviceAPI_DirectSound::~nuiAudioDeviceAPI_DirectSound()
{
}


nuiAudioDevice_DirectSound::nuiAudioDevice_DirectSound(GUID IGuid, GUID OGuid, const nglString& rInName, const nglString& rOutName, const nglString& rInModule, const nglString& rOutModule)
: nuiAudioDevice()
{
  mInName = rInName;
  mOutName = rOutName;
  if (rOutName.IsEmpty())
    mName = rInName;
  else
    mName = rOutName;

  mManufacturer = rOutModule;
  mIsPresent = false;
  mHasInput = false;
  mHasOutput = false;

  mIDeviceID = IGuid;
  mODeviceID = OGuid;

  mpDirectSound = NULL;
  mpDirectSoundCapture = NULL;

  mAPIName = API_NAME;

  mpRingBuffer = NULL;
  mpInputBuffer = NULL;
  mpOutputBuffer = NULL;

  mpProcessingTh = NULL;
  mpOutputTh = NULL;

  mNotifInputEvent[0] = NULL;
  mNotifInputEvent[1] = NULL;
  mNotifOutputEvent[0] = NULL;
  mNotifOutputEvent[1] = NULL;

  HRESULT hr;
  // Get Input device caps:
  hr = DirectSoundCaptureCreate(&mIDeviceID, &mpDirectSoundCapture, NULL);
  if (hr != S_OK || !mpDirectSoundCapture)
  {
    NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_INFO, _T("constructor ERROR : could not create DirectSoundCapture object!\n"));
  }

  // Get Output device caps:
  hr = DirectSoundCreate(&mODeviceID, &mpDirectSound, NULL);
  if (hr != S_OK || !mpDirectSound)
  {
    NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("constructor ERROR : could not create DirectSound object!\n")); // if there is no output, consider the device as not valid
    return;
  }


  Init();
}

nuiAudioDevice_DirectSound::nuiAudioDevice_DirectSound()
: nuiAudioDevice()
{
  mInName = _T("Default Device");
  mOutName = _T("Default Device");

  mIsPresent = false;
  mHasInput = false;
  mHasOutput = false;

  mpDirectSound = NULL;
  mpDirectSoundCapture = NULL;

  mAPIName = API_NAME;

  mpRingBuffer = NULL;
  mpInputBuffer = NULL;
  mpOutputBuffer = NULL;

  mpProcessingTh = NULL;
  mpOutputTh = NULL;

  mNotifInputEvent[0] = NULL;
  mNotifInputEvent[1] = NULL;
  mNotifOutputEvent[0] = NULL;
  mNotifOutputEvent[1] = NULL;

  HRESULT hr;
  // Get Input device caps:
  hr = DirectSoundCaptureCreate(NULL, &mpDirectSoundCapture, NULL);
  if (hr != S_OK || !mpDirectSoundCapture)
  {
    NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_INFO, _T("constructor ERROR : could not create DirectSoundCapture object!\n"));
  }

  // Get Output device caps:
  hr = DirectSoundCreate(NULL, &mpDirectSound, NULL);
  if (hr != S_OK || !mpDirectSound)
  {
    NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("constructor ERROR : could not create DirectSound object!\n"));// if there is no output, consider the device as not valid
    return;
  }

  Init();
}

void nuiAudioDevice_DirectSound::Init()
{
  DSCCAPS icaps;
  icaps.dwSize = sizeof(DSCCAPS);
  if (mpDirectSoundCapture)
    mpDirectSoundCapture->GetCaps(&icaps);
  else
  {
    icaps.dwChannels = 0;
    icaps.dwFormats = 0;
    icaps.dwFlags = 0;
  }

  DSCAPS ocaps;
  ocaps.dwSize = sizeof(DSCAPS);
  mpDirectSound->GetCaps(&ocaps);

  // Input channels:
  if (icaps.dwChannels == 1)
  {
    nglString str(mInName);
    str.Add(_T(" (Input)"));
    mInputChannels.push_back(str);
  }
  else if (icaps.dwChannels > 1)
  {
    {
      nglString str(mInName);
      str.Add(_T(" (Left input)"));
      mInputChannels.push_back(str);
    }

    {
      nglString str(mInName);
      str.Add(_T(" (Right input)"));
      mInputChannels.push_back(str);
    }
  }



  // Output channels:
  {
    nglString str(mOutName);
    str.Add(_T(" (Left output)"));
    mOutputChannels.push_back(str);
  }
  {
    nglString str(mOutName);
    str.Add(_T(" (Right output)"));
    mOutputChannels.push_back(str);
  }

  mSampleRates.clear();
  mBufferSizes.clear();

  if ( ( ( icaps.dwFormats & WAVE_FORMAT_1M16 ) || ( icaps.dwFormats & WAVE_FORMAT_1S16 ) )
    && ocaps.dwMinSecondarySampleRate <= 11025 && ocaps.dwMaxSecondarySampleRate >= 11025 )
    mSampleRates.push_back(11025);
  if ( ( ( icaps.dwFormats & WAVE_FORMAT_2M16 ) || ( icaps.dwFormats & WAVE_FORMAT_2S16 ) )
    && ocaps.dwMinSecondarySampleRate <= 22050 && ocaps.dwMaxSecondarySampleRate >= 22050 )
    mSampleRates.push_back(22050);
  if ( ( ( icaps.dwFormats & WAVE_FORMAT_4M16 ) || ( icaps.dwFormats & WAVE_FORMAT_4S16 ) )
    && ocaps.dwMinSecondarySampleRate <= 44100 && ocaps.dwMaxSecondarySampleRate >= 44100 )
    mSampleRates.push_back(44100);
  if ( ( ( icaps.dwFormats & WAVE_FORMAT_48M16 ) || ( icaps.dwFormats & WAVE_FORMAT_48S16 ) )
    && ocaps.dwMinSecondarySampleRate <= 48000 && ocaps.dwMaxSecondarySampleRate >= 48000 )
    mSampleRates.push_back(48000);
  if ( ( ( icaps.dwFormats & WAVE_FORMAT_96M16 ) || ( icaps.dwFormats & WAVE_FORMAT_96S16 ) )
    && ocaps.dwMinSecondarySampleRate <= 96000 && ocaps.dwMaxSecondarySampleRate >= 96000 )
    mSampleRates.push_back(96000);

  if (mSampleRates.empty())
  {
    mInputChannels.clear();
    if (ocaps.dwMinSecondarySampleRate <= 11025 && ocaps.dwMaxSecondarySampleRate >= 11025 )
      mSampleRates.push_back(11025);
    if (ocaps.dwMinSecondarySampleRate <= 22050 && ocaps.dwMaxSecondarySampleRate >= 22050 )
      mSampleRates.push_back(22050);
    if (ocaps.dwMinSecondarySampleRate <= 44100 && ocaps.dwMaxSecondarySampleRate >= 44100 )
      mSampleRates.push_back(44100);
    if (ocaps.dwMinSecondarySampleRate <= 48000 && ocaps.dwMaxSecondarySampleRate >= 48000 )
      mSampleRates.push_back(48000);
    if (ocaps.dwMinSecondarySampleRate <= 96000 && ocaps.dwMaxSecondarySampleRate >= 96000 )
      mSampleRates.push_back(96000);
  }

  mIsPresent = true;

  uint i = 0;
  int sizes[] = { 64, 128, 256, 384, 512, 768, 1024, 2048, 4096, 8192, 16384, 32768 ,0 };
  while (sizes[i])
    mBufferSizes.push_back(sizes[i++]);
}



nuiAudioDevice_DirectSound::~nuiAudioDevice_DirectSound()
{
  Close();
}



bool nuiAudioDevice_DirectSound::Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction)
{
  if (!mpDirectSound)
    return false;
  
  HRESULT hr = S_OK;
  mAudioProcessFn = pProcessFunction;

  mBufferSize = BufferSize;

  hr = mpDirectSound->SetCooperativeLevel(GetDesktopWindow(), DSSCL_EXCLUSIVE);

  mHasInput = (rInputChannels.size() > 0) && (mInputChannels.size() > 0);
  mHasOutput = (rOutputChannels.size() > 0) && (mOutputChannels.size() > 0);


  mpInputBuffer = NULL;
  mpOutputBuffer = NULL;

  if (!mHasInput && !mHasOutput)
    return false;

  // init ringbuffer
  mpRingBuffer = new nglRingBuffer(BufferSize*4, sizeof(float), rOutputChannels.size());
  mpRingBuffer->AdvanceWriteIndex(BufferSize);

  // init input buffers
  if (mHasInput)
  {
    {
      mActiveInputChannels = rInputChannels;
    }

    WAVEFORMATEX IFormat;
    IFormat.wFormatTag = WAVE_FORMAT_PCM;
    IFormat.nChannels = (WORD)mInputChannels.size();
    IFormat.nSamplesPerSec = ToNearest(SampleRate);
    IFormat.wBitsPerSample = 16;
    IFormat.nAvgBytesPerSec = IFormat.nChannels * IFormat.nSamplesPerSec * (IFormat.wBitsPerSample / 8);
    IFormat.nBlockAlign = IFormat.nChannels * (IFormat.wBitsPerSample / 8);
    IFormat.cbSize = 0;

    DSCBUFFERDESC IBufferDesc;
    memset(&IBufferDesc, 0, sizeof(IBufferDesc));
    IBufferDesc.dwSize = sizeof(DSCBUFFERDESC);
    IBufferDesc.dwFlags = DSCBCAPS_WAVEMAPPED;
    IBufferDesc.dwBufferBytes = (IFormat.wBitsPerSample / 8) * IFormat.nChannels * BufferSize * 2;
    IBufferDesc.dwReserved = 0;
    IBufferDesc.lpwfxFormat = &IFormat;
    IBufferDesc.dwFXCount = 0;
    IBufferDesc.lpDSCFXDesc = NULL;

    NGL_ASSERT(mpDirectSoundCapture);
    hr = mpDirectSoundCapture->CreateCaptureBuffer(&IBufferDesc, &mpInputBuffer, NULL);
  }


  // init output buffers
  if (mHasOutput)
  {
    {
      mActiveOutputChannels = rOutputChannels;
    }

    WAVEFORMATEX OFormat;
    OFormat.wFormatTag = WAVE_FORMAT_PCM;
    OFormat.nChannels = (WORD)mOutputChannels.size();
    OFormat.nSamplesPerSec = ToNearest(SampleRate);
    OFormat.wBitsPerSample = 16;
    OFormat.nAvgBytesPerSec = OFormat.nChannels * OFormat.nSamplesPerSec * (OFormat.wBitsPerSample / 8);
    OFormat.nBlockAlign = OFormat.nChannels * OFormat.wBitsPerSample / 8;
    OFormat.cbSize = 0;

    DSBUFFERDESC  OBufferDesc;
    memset(&OBufferDesc, 0, sizeof(OBufferDesc));
    OBufferDesc.dwSize = sizeof(OBufferDesc);
    OBufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY;
    OBufferDesc.dwBufferBytes = (OFormat.wBitsPerSample / 8) * OFormat.nChannels * BufferSize * 2;
    OBufferDesc.dwReserved = 0;
    OBufferDesc.lpwfxFormat = &OFormat;

    hr = mpDirectSound->CreateSoundBuffer(&OBufferDesc, &mpOutputBuffer, NULL);
  }


  // create event for notifications
  mNotifInputEvent[0] = CreateEvent(NULL, FALSE, FALSE, _T("NUI_DSoundInputEvent0"));
  mNotifInputEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("NUI_DSoundInputEvent1"));
  mNotifOutputEvent[0] = CreateEvent(NULL, FALSE, FALSE, _T("NUI_DSoundOutputEvent0"));
  mNotifOutputEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("NUI_DSoundOutputEvent1"));




  // set the notification for the input buffer
  if (mHasInput)
  {
    // Setup the notification positions
    ZeroMemory( &mInputPosNotify, sizeof(DSBPOSITIONNOTIFY) * 2);
    mInputPosNotify[0].dwOffset = BufferSize * sizeof(int16) * mInputChannels.size() - 1;
    mInputPosNotify[0].hEventNotify = mNotifInputEvent[0];             
    mInputPosNotify[1].dwOffset = BufferSize * sizeof(int16) * mInputChannels.size() * 2 - 1;
    mInputPosNotify[1].hEventNotify = mNotifInputEvent[1];           

    LPDIRECTSOUNDNOTIFY pInputNotify = NULL;
    if( FAILED( hr = mpInputBuffer->QueryInterface( IID_IDirectSoundNotify, (VOID**)&pInputNotify ) ) )
    {
      NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("Open ERROR : failed in querying interface for input notifications.\n"));
      return false;
    }


    // Tell DirectSound when to notify us. the notification will come in the from 
    // of signaled events that are handled in WinMain()
    if( FAILED( hr = pInputNotify->SetNotificationPositions( 2, mInputPosNotify ) ) )
    {
      NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("Open ERROR : failed in setting notifications for input\n"));
      return false;
    }

    pInputNotify->Release();
  }



  // set the notification events for the output buffer
  if (mHasOutput)
  {
    // Setup the notification positions
    ZeroMemory( &mOutputPosNotify, sizeof(DSBPOSITIONNOTIFY) * 2);
    mOutputPosNotify[0].dwOffset = BufferSize * sizeof(int16) * mOutputChannels.size() - 1;
    mOutputPosNotify[0].hEventNotify = mNotifOutputEvent[0];             
    mOutputPosNotify[1].dwOffset = BufferSize * sizeof(int16) * mOutputChannels.size() * 2 - 1;
    mOutputPosNotify[1].hEventNotify = mNotifOutputEvent[1];    

    LPDIRECTSOUNDNOTIFY pOutputNotify = NULL;
    if( FAILED( hr = mpOutputBuffer->QueryInterface( IID_IDirectSoundNotify, (VOID**)&pOutputNotify ) ) )
    {
      NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("Open ERROR : failed in querying interface for output notifications.\n"));
      return false;
    }

    // Tell DirectSound when to notify us. the notification will come in the from 
    // of signaled events that are handled in WinMain()
    if( FAILED( hr = pOutputNotify->SetNotificationPositions( 2, mOutputPosNotify ) ) )
    {
      NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("Open ERROR : failed in setting notifications for output\n"));
      return false;
    }

    pOutputNotify->Release();
  }


  // start input processing thread
  mpProcessingTh = new nuiAudioDevice_DS_ProcessingTh(this, mNotifInputEvent[0], mNotifInputEvent[1], mAudioProcessFn);
  mpProcessingTh->Start();



  // start output thread
  if (mHasOutput)
  {
    mpOutputTh = new nuiAudioDevice_DS_OutputTh(this, mNotifInputEvent[0], mNotifInputEvent[1], mNotifOutputEvent[0], mNotifOutputEvent[1]);
    mpOutputTh->Start();
    hr = mpOutputBuffer->Play(0,0,DSCBSTART_LOOPING);
    if (FAILED(hr))
      NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("OutputBuffer->Play ERROR!\n"));
  }

  // start input capture
  if (mHasInput)
  {
    hr = mpInputBuffer->Start(DSCBSTART_LOOPING);
    if (FAILED(hr))
      NGL_LOG(_T("nuiAudioDevice_DirectSound"), NGL_LOG_ERROR, _T("InputBuffer->Start ERROR!\n"));    
  }





  return true;
}




bool nuiAudioDevice_DirectSound::Close()
{
  if (mpOutputBuffer)
  {
    mpOutputBuffer->Stop();
  }

  if (mpInputBuffer)
  {
    mpInputBuffer->Stop();
  }

  if (mpProcessingTh)
  {
    mpProcessingTh->Stop();
    mpProcessingTh->Join();
    delete mpProcessingTh;
    mpProcessingTh = NULL;
  }
  if (mpOutputTh)
  {
    mpOutputTh->Stop();
    mpOutputTh->Join();
    delete mpOutputTh;
    mpOutputTh = NULL;
  }



  if (mpOutputBuffer)
  {
    mpOutputBuffer->Release();
    mpOutputBuffer=NULL;
  }

  if (mpInputBuffer)
  {
    mpInputBuffer->Release();
    mpInputBuffer=NULL;
  }

  delete mpRingBuffer;
  mpRingBuffer = NULL;


  if (mNotifInputEvent[0])
  {
    CloseHandle(mNotifInputEvent[0]);
    mNotifInputEvent[0] = NULL;
  }
  if (mNotifInputEvent[1])
  {
    CloseHandle(mNotifInputEvent[1]);
    mNotifInputEvent[1] = NULL;
  }

  if (mNotifOutputEvent[0])
  {
    CloseHandle(mNotifOutputEvent[0]);
    mNotifOutputEvent[0] = NULL;
  }
  if (mNotifOutputEvent[1])
  {
    CloseHandle(mNotifOutputEvent[1]);
    mNotifOutputEvent[1] = NULL;
  }

  return false;
}


bool nuiAudioDevice_DirectSound::HasInput()
{
  return mHasInput;
}

bool nuiAudioDevice_DirectSound::HasOutput()
{
  return mHasOutput;
}



LPDIRECTSOUNDCAPTUREBUFFER nuiAudioDevice_DirectSound::GetInputBuffer()
{
  return mpInputBuffer;
}


LPDIRECTSOUNDBUFFER nuiAudioDevice_DirectSound::GetOutputBuffer()
{
  return mpOutputBuffer;
}



DSBPOSITIONNOTIFY nuiAudioDevice_DirectSound::GetInputNotify(uint pos)
{
  return mInputPosNotify[pos];
}


DSBPOSITIONNOTIFY nuiAudioDevice_DirectSound::GetOutputNotify(uint pos)
{
  return mOutputPosNotify[pos];
}


uint32 nuiAudioDevice_DirectSound::GetBufferSize()
{
  return mBufferSize;
}



uint32 nuiAudioDevice_DirectSound::GetNbInputChannels()
{
  return (uint32)mActiveInputChannels.size();
}

uint32 nuiAudioDevice_DirectSound::GetNbOutputChannels()
{
  return (uint32)mActiveOutputChannels.size();
}





nglRingBuffer* nuiAudioDevice_DirectSound::GetRingBuffer()
{
  return mpRingBuffer;
}


HANDLE nuiAudioDevice_DirectSound::GetInputEvent(uint pos)
{
  return mNotifInputEvent[pos];
}


//********************************************************************************************************************************
//********************************************************************************************************************************
//
// PROCESSING THREAD
//
//********************************************************************************************************************************
//********************************************************************************************************************************


nuiAudioDevice_DS_ProcessingTh::nuiAudioDevice_DS_ProcessingTh(nuiAudioDevice_DirectSound* pDev, HANDLE hEvent0, HANDLE hEvent1, nuiAudioProcessFn pProcessFunction)
: nglThread(High)
{
  uint i;

  mpProcessFunction = pProcessFunction;

  mContinue = false;

  mBufferSize = pDev->GetBufferSize();
  mInputNbChannels = pDev->GetNbInputChannels();
  mOutputNbChannels = pDev->GetNbOutputChannels();

  mpDSInputBuffer = NULL;
  if (pDev->HasInput())
    mpDSInputBuffer = pDev->GetInputBuffer();
  mpRingBuffer = pDev->GetRingBuffer();

  mEvents[0] = hEvent0;
  mEvents[1] = hEvent1;
  mpLocalBuf = NULL;

  // init output float buffer
  mFloatOutputBuf.resize(pDev->GetNbOutputChannels());
  for (i=0; i < pDev->GetNbOutputChannels(); i++)
    mFloatOutputBuf[i] = new float[pDev->GetBufferSize()];


  // init input float buffer and local input int16 buffer
  if (pDev->HasInput())
  {
    mFloatInputBuf.resize(pDev->GetNbInputChannels());
    for (i=0; i < pDev->GetNbInputChannels(); i++)
      mFloatInputBuf[i] = new float[pDev->GetBufferSize()];

    mpLocalBuf = new int16[pDev->GetBufferSize() * pDev->GetNbInputChannels()];
  }

}


nuiAudioDevice_DS_ProcessingTh::~nuiAudioDevice_DS_ProcessingTh()
{
  uint i;

  delete mpLocalBuf;

  for (i=0; i < mFloatOutputBuf.size(); i++)
    delete mFloatOutputBuf[i];

  for (i=0; i < mFloatInputBuf.size(); i++)
    delete mFloatInputBuf[i];

}



void nuiAudioDevice_DS_ProcessingTh::Process(uint pos)
{
  NGL_LOG(_T("nuiAudioDevice_DS_ProcessingTh"), NGL_LOG_DEBUG, _T("Process Thread received Event %d\n"), pos);

  int16* pBuf1=NULL;
  int16* pBuf2=NULL;
  DWORD size1=0;
  DWORD size2=0;
  DWORD bufferBytes = mBufferSize * mInputNbChannels * sizeof(int16);

  if (!mpRingBuffer->GetWritable())
    return;

  //
  // lock the input buffer if any,
  // and read data from it to the local buffer
  //
  if (  mpDSInputBuffer 
    && mpDSInputBuffer->Lock(pos * bufferBytes /* offset */, bufferBytes /*size*/, (LPVOID*)&pBuf1, &size1, (LPVOID*)&pBuf2, &size2, 0)
    )
  {
    if (!pBuf1 || !size1)
    {
      //NGL_LOG(_T("nuiAudioDevice_DS_ProcessingTh"), NGL_LOG_ERROR, _T("Process error : could not lock any part of the input buffer\n"));
      NGL_ASSERT(0);
      return;
    }

    // check that we got the right size
    NGL_ASSERT((size1+size2) == bufferBytes);


    // copy input data into local buffer
    memcpy(mpLocalBuf, pBuf1, size1);
    if (pBuf2)
      memcpy(mpLocalBuf+size1, pBuf2, size2);

    // convert int16 buffer to float buffer
    for (uint32 ch=0; ch < mInputNbChannels; ch++)
    {
      nuiAudioConvert_INint16ToDEfloat(mpLocalBuf, const_cast<float*>(mFloatInputBuf[ch]), ch, mInputNbChannels, mBufferSize); 
    }
  }


  // call user audio process function
  mpProcessFunction(mFloatInputBuf, mFloatOutputBuf, mBufferSize);

  // copy output buffer contents to ringbuffer
  uint32 nbWrite = WriteToRingBuf(mFloatOutputBuf, mBufferSize, mOutputNbChannels);

  // release DS input buffer
  if (mpDSInputBuffer)
    mpDSInputBuffer->Unlock(pBuf1, size1, pBuf2, size2);
}




uint32 nuiAudioDevice_DS_ProcessingTh::WriteToRingBuf(const std::vector<float*>& inbuf, uint32 nbSampleFrames, uint32 nbChannels)
{
  uint32 nbWrite, nbWrite2, c, i;
  std::vector<float*>  rbuf(nbChannels);
  bool need2ndPass=true;

  // prepare 1st pass writing
  nbWrite  = mpRingBuffer->GetWritableToEnd();
  if (nbWrite>=nbSampleFrames)
  {
    nbWrite = nbSampleFrames;
    need2ndPass=false;
  }

  // 1st pass writing
  for (c=0; c < nbChannels; c++)
    rbuf[c] = (float*)mpRingBuffer->GetWritePointer(c);

  for (i=0; i < nbWrite; i++)
  {
    for (c=0; c < nbChannels; c++)
      rbuf[c][i] = inbuf[c][i];
  } 

  // update ringbuf pointer
  mpRingBuffer->AdvanceWriteIndex(nbWrite);


  if (!need2ndPass)
    return nbWrite;

  // prepare 2nd pass writing
  nbWrite2  = mpRingBuffer->GetWritableToEnd();

  if ((nbWrite+nbWrite2) > nbSampleFrames)
  {
    nbWrite2 = nbSampleFrames - nbWrite;
  }

  // 2nd pass writing
  for (c=0; c<nbChannels; c++)
    rbuf[c] = (float*)mpRingBuffer->GetWritePointer(c);        

  for (i=nbWrite; i < nbWrite+nbWrite2; i++)
  {
    for (c=0; c < nbChannels; c++)
      rbuf[c][i] = inbuf[c][i];
  }

  // update ringbuf pointer
  mpRingBuffer->AdvanceWriteIndex(nbWrite2);

  return nbWrite+nbWrite2;
}




void nuiAudioDevice_DS_ProcessingTh::OnStart()
{
  mContinue = true;
  uint i=0;

  while (mContinue)
  {

    DWORD dwEvent  = WaitForMultipleObjects(2, mEvents, false, 500);

    switch (dwEvent) 
    { 
    case WAIT_OBJECT_0 + 0:
      Process(0);
      break;
    case WAIT_OBJECT_0 + 1:
      Process(1);
      break; 

    case WAIT_TIMEOUT:
      NGL_OUT2(_T("Process Thread : Wait timed out.\n"));
      break;

    default: 
      // NGL_LOG(_T("nuiAudioDevice_DS_ProcessingTh"), NGL_LOG_ERROR, _T("Process Thread : Wait error: %d\n"), GetLastError()); 
      mContinue=false;
      break;
    }


  }

}

void nuiAudioDevice_DS_ProcessingTh::Stop()
{
  mContinue=false;
}






//********************************************************************************************************************************
//********************************************************************************************************************************
//
// OUTPUT THREAD
//
//********************************************************************************************************************************
//********************************************************************************************************************************


nuiAudioDevice_DS_OutputTh::nuiAudioDevice_DS_OutputTh(nuiAudioDevice_DirectSound* pDev, HANDLE inputEvent0, HANDLE inputEvent1, HANDLE outputEvent0, HANDLE outputEvent1)
: nglThread(High)
{
  mContinue = false;

  mBufferSize = pDev->GetBufferSize();
  mNbChannels = pDev->GetNbOutputChannels();
  mpDSOutputBuffer = NULL;

  if (pDev->HasOutput())
  {
    mpDSOutputBuffer = pDev->GetOutputBuffer();
  }
  mpRingBuffer = pDev->GetRingBuffer();

  mInputEvents[0] = inputEvent0;
  mInputEvents[1] = inputEvent1;
  mOutputEvents[0] = outputEvent0;
  mOutputEvents[1] = outputEvent1;

  // init de-interlaced float buffer
  mFloatOutputBuf.resize(mNbChannels);
  for (uint32 i=0; i < mNbChannels; i++)
    mFloatOutputBuf[i] = new float[mBufferSize];

  // init interlaced int16 buffer
  mpLocalBuf = new int16[mBufferSize * mNbChannels];

}




nuiAudioDevice_DS_OutputTh::~nuiAudioDevice_DS_OutputTh()
{
  delete mpLocalBuf;

  for (uint i=0; i < mNbChannels; i++)
    delete mFloatOutputBuf[i];
}


void nuiAudioDevice_DS_OutputTh::Process(uint pos)
{
  int16* pBuf1=NULL;
  int16* pBuf2=NULL;
  DWORD size1=0;
  DWORD size2=0;
  DWORD bufferBytes = mBufferSize * mNbChannels * sizeof(int16);
  bool isEmpty=false;

  if (!mpRingBuffer->GetReadable())
    isEmpty=true; 
  //
  // lock the output buffer if any,
  // and read data from ringbuffer and write to output buffer
  //

  if (mpDSOutputBuffer && FAILED(mpDSOutputBuffer->Lock(pos *bufferBytes /* offset */,bufferBytes /*size*/, (LPVOID*)&pBuf1, &size1, (LPVOID*)&pBuf2, &size2, 0)))
  {
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

    //NGL_LOG(_T("nuiAudioDevice_DS_ProcessingTh"), NGL_LOG_ERROR, _T("ERROR : GetOutputBuffer()->Lock failed with error %d: %s"), dw, lpMsgBuf); 

    LocalFree(lpMsgBuf);

  }
  else
    if (mpDSOutputBuffer)
    {
      if (!pBuf1 || !size1)
      {
        NGL_LOG(_T("nuiAudioDevice_DS_OutputTh"), NGL_LOG_ERROR, _T("Process error : could not lock any part of the input buffer\n"));
        NGL_ASSERT(0);
        return;
      }

      // check that we got the right size
      NGL_ASSERT((size1+size2) == bufferBytes);

      // no sound to play. silence please
      if (isEmpty)
      {
        memset(pBuf1, 0, size1);
        if (pBuf2)
          memset(pBuf2, 0, size2);
      }
      else
      {
        // copy ring buffer to local buffer 
        uint32 nbRead = ReadFromRingBuf(mBufferSize, mFloatOutputBuf, mNbChannels);

        // clamp values of output buffer between -1 and 1
        // convert and interlace local buffer from deinterlaced float to interlaced int16
        for (uint32 ch=0; ch < mNbChannels; ch++)
        {
          float* pFloat = mFloatOutputBuf[ch];
          for (uint32 s = 0; s < nbRead; s++)
          {
            (*pFloat) = fclamp(*pFloat, -1.0f, 1.0f);
            pFloat++;
          }
          nuiAudioConvert_DEfloatToINint16(mFloatOutputBuf[ch], mpLocalBuf, ch, mNbChannels, mBufferSize);
        }

        //#FIXME : here we can use pBuf1 and pBuf2 as the ouput buffer for DEfloatToINint16, instead of using a useless local buffer.
        // do that when you have the time to... :)

        // copy local buffer to output buffer
        memcpy(pBuf1, mpLocalBuf, size1);
        if (pBuf2)
          memcpy(pBuf2, mpLocalBuf+size1, size2);
      }

      // release DS input buffer
      mpDSOutputBuffer->Unlock(pBuf1, size1, pBuf2, size2);
    }


}






uint32 nuiAudioDevice_DS_OutputTh::ReadFromRingBuf(uint32 nbSampleFrames, std::vector<float*>& outbuf, uint32 nbChannels)
{
  bool need2ndPass = true;
  uint32 ch, i;
  float *pRing, *pOut;
  uint32 nbRead = 0, nbRead1 = 0, nbRead2 = 0;

  // prepare 1st pass reading
  nbRead1 = mpRingBuffer->GetReadableToEnd();

  // read what you need. not more! (maybe less, 'cause it's the 1st pass...)
  if (nbRead1 > nbSampleFrames)
    nbRead1 = nbSampleFrames;

  if (nbRead1 == nbSampleFrames)
  {
    need2ndPass = false;
  }

  // 1st pass reading
  for (ch = 0; ch < nbChannels; ch++)
  {
    pRing = (float*)mpRingBuffer->GetReadPointer(ch);
    pOut = outbuf[ch];


    for (i=0; i < nbRead1; i++)
    {
      *(pOut++) = *(pRing++);
    }
  }

  // update ringbuf pointer
  mpRingBuffer->AdvanceReadIndex (nbRead1);

  if (!need2ndPass)
    return nbRead1;

  // prepare 2nd pass reading
  nbRead2 = mpRingBuffer->GetReadableToEnd();
  nbRead = nbRead1 + nbRead2;
  if (nbRead > nbSampleFrames)
  {
    nbRead2 = nbSampleFrames - nbRead1;
    nbRead = nbRead1 + nbRead2;
  }

  //NGL_ASSERT((nbRead)==nbSampleFrames);

  for (ch = 0; ch < nbChannels; ch++)
  {
    pRing = (float*)mpRingBuffer->GetReadPointer(ch);
    pOut = &(outbuf[ch][nbRead1]);

    for (i=0; i < nbRead2; i++)
    {
      *(pOut++) = *(pRing++);
    }
  }

  // update ringbuf pointer
  mpRingBuffer->AdvanceReadIndex(nbRead2); 

  return nbRead;

}



void nuiAudioDevice_DS_OutputTh::OnStart()
{
  mContinue = true;
  uint i=0;

  while (mContinue)
  {
    DWORD dwEvent  = WaitForMultipleObjects(2, mOutputEvents, false, 500);

    switch (dwEvent) 
    { 
    case WAIT_OBJECT_0 + 0:
      SetEvent(mInputEvents[0]);
      Process(0);
      break;
    case WAIT_OBJECT_0 + 1:
      SetEvent(mInputEvents[1]);
      Process(1);
      break; 

    case WAIT_TIMEOUT:
      NGL_OUT2(_T("."));
      break;

    default: 
      NGL_LOG(_T("nuiAudioDevice_DS_OutputTh"), NGL_LOG_ERROR, _T("Output process : Wait error: %d\n"), GetLastError()); 
      mContinue=false;
      break;
    }


  }

}


void nuiAudioDevice_DS_OutputTh::Stop()
{
  mContinue = false;
}





//********************************************************************************************************************************
//********************************************************************************************************************************
//********************************************************************************************************************************



uint32 nuiAudioDeviceAPI_DirectSound::GetDeviceCount() const
{
  mDeviceIDs.clear();
  mDeviceMap.clear();

  // Add the default device first:
  DeviceDesc desc;
  desc.mInName = _T("Default device");
  desc.mOutName = _T("Default device");
  mDeviceIDs.push_back(desc);
  mDeviceMap[_T("Default device")] = 0;

  HRESULT hr = DirectSoundEnumerate(&nuiAudioDeviceAPI_DirectSound::DSEnumOutputCallback, (LPVOID)this);
  hr = DirectSoundCaptureEnumerate(&nuiAudioDeviceAPI_DirectSound::DSEnumInputCallback, (LPVOID)this);

  return (uint32)mDeviceIDs.size();
}

nuiAudioDevice* nuiAudioDeviceAPI_DirectSound::GetDevice(uint32 index)
{
  return new nuiAudioDevice_DirectSound(mDeviceIDs[index].mInputGUID, mDeviceIDs[index].mOutputGUID, mDeviceIDs[index].mInName, mDeviceIDs[index].mOutName, mDeviceIDs[index].mInModule, mDeviceIDs[index].mOutModule);
}

nuiAudioDevice* nuiAudioDeviceAPI_DirectSound::GetDevice(const nglString& rDeviceName)
{
  std::vector<DeviceDesc>::const_iterator end = mDeviceIDs.end();
  for (std::vector<DeviceDesc>::const_iterator it = mDeviceIDs.begin(); it != end; ++it)
  {
    if (it->mOutName == rDeviceName)
    {
      return new nuiAudioDevice_DirectSound(it->mInputGUID, it->mOutputGUID, it->mInName, it->mOutName, it->mInModule, it->mOutModule);
    }
  }
  NGL_ASSERT(0);
}

nglString nuiAudioDeviceAPI_DirectSound::GetDeviceName(uint32 index) const
{
  return mDeviceIDs[index].mOutName;
}

nuiAudioDevice* nuiAudioDeviceAPI_DirectSound::GetDefaultOutputDevice()
{
  return new nuiAudioDevice_DirectSound();
}

nuiAudioDevice* nuiAudioDeviceAPI_DirectSound::GetDefaultInputDevice()
{
  return new nuiAudioDevice_DirectSound();
}


BOOL nuiAudioDeviceAPI_DirectSound::RealDSEnumInputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule)
{
  if (lpGuid)
  {
    std::map<nglString, int32>::const_iterator it = mDeviceMap.find(lpcstrDescription);
    if (it == mDeviceMap.end())
    {
      DeviceDesc desc;
      desc.mInputGUID = *lpGuid;
      desc.mInName = lpcstrDescription;
      desc.mInModule = lpcstrModule;
      mDeviceIDs.push_back(desc);
      mDeviceMap[lpcstrDescription] = (int)mDeviceIDs.size() - 1;
    }
    else
    {
      int index = it->second;
      mDeviceIDs[index].mInputGUID = *lpGuid;
    }
  }
  return TRUE;
}


//static
BOOL CALLBACK nuiAudioDeviceAPI_DirectSound::DSEnumInputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
  nuiAudioDeviceAPI_DirectSound* pAPI = (nuiAudioDeviceAPI_DirectSound*)lpContext;
  NGL_ASSERT(pAPI);
  return pAPI->RealDSEnumInputCallback(lpGuid, lpcstrDescription, lpcstrModule);
}



BOOL nuiAudioDeviceAPI_DirectSound::RealDSEnumOutputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule)
{
  if (lpGuid)
  {
    std::map<nglString, int32>::const_iterator it = mDeviceMap.find(lpcstrDescription);
    if (it == mDeviceMap.end())
    {
      DeviceDesc desc;
      desc.mOutputGUID = *lpGuid;
      desc.mOutName = lpcstrDescription;
      desc.mOutModule = lpcstrModule;
      mDeviceIDs.push_back(desc);
      mDeviceMap[lpcstrDescription] = (int)mDeviceIDs.size() - 1;
    }
    else
    {
      int index = it->second;
      mDeviceIDs[index].mOutputGUID = *lpGuid;
    }
  }
  return TRUE;
}


// static
BOOL CALLBACK nuiAudioDeviceAPI_DirectSound::DSEnumOutputCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
  nuiAudioDeviceAPI_DirectSound* pAPI = (nuiAudioDeviceAPI_DirectSound*)lpContext;
  NGL_ASSERT(pAPI);
  return pAPI->RealDSEnumOutputCallback(lpGuid, lpcstrDescription, lpcstrModule);
}

 nuiAudioDeviceAPI_DirectSound DirectSoundAPI;
