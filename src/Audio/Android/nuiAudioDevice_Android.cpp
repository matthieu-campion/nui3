/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiAudioDevice_Android.h"
#include "nuiAudioConvert.h"

#define API_NAME _T("AndroidAudio")

void nuiAudioDevice_Android_IOProc(SLAndroidSimpleBufferQueueItf bufferqueue, void* context)
{
  nuiAudioDevice_Android* pDevice = (nuiAudioDevice_Android*)context;
  NGL_ASSERT(pDevice);
  pDevice->Process();
}


nuiAudioDevice_Android::nuiAudioDevice_Android()
{
  mAPIName = API_NAME;


  mpEngine = NULL;

  mName = _T("AndroidDevice");
  mManufacturer = _T("AndroidManufacturer");

  EnumSampleRates(); /// Fill mSampleRates;
  EnumBufferSizes(); /// Fill mBufferSizes;

  mOutputChannels.push_back(_T("Output"));

  mIsPresent = true;
}

nuiAudioDevice_Android::~nuiAudioDevice_Android()
{
  Close();
}

bool nuiAudioDevice_Android::Open(std::vector<int32>& rInputChannels, std::vector<int32>& rOutputChannels, double SampleRate, int32 BufferSize, nuiAudioProcessFn pProcessFunction)
{
  mAudioProcessFn = pProcessFunction;
  mSampleRate = SampleRate;
  mBufferSize = BufferSize;

  {
    mActiveInputChannels = rInputChannels;
    mInputSamples.resize(mActiveInputChannels.size());
    for (int32 ch = 0; ch < mInputSamples.size(); ch++)
      mInputSamples[ch].resize(BufferSize);
  }

  {
    mActiveOutputChannels = rOutputChannels;
    mOutputSamples.resize(mActiveOutputChannels.size());
    for (int32 ch = 0; ch < mOutputSamples.size(); ch++)
      mOutputSamples[ch].resize(BufferSize);
  }

  mOutputInterleaved.resize(mOutputSamples.size() * mBufferSize);


  SLresult err;

  // create engine
  SLint32      numInterfaces = 1;
  SLInterfaceID interfaces[] = {SL_IID_ENGINE};
  SLboolean     requested[] = {true};
  err = slCreateEngine(&mpEngine, 0, NULL, numInterfaces, interfaces, requested);
  if (err != SL_RESULT_SUCCESS)
  {
    LOGI("nuiAudioDevice_Android::nuiAudioDevice_Android create engine ERROR %d", err);
    return false;
  }

  err = (*mpEngine)->Realize(mpEngine, SL_BOOLEAN_FALSE);
  if (err != SL_RESULT_SUCCESS)
  {
    LOGI("nuiAudioDevice_Android::nuiAudioDevice_Android realize audio engine ERROR %d", err);
    return false;
  }



  err = (*mpEngine)->GetInterface(mpEngine, SL_IID_ENGINE, &mpEngineItf);
  if (err == SL_RESULT_SUCCESS)
    LOGI("get engine interface OK");
  else
  {
    LOGI("get engine interface ERROR %d", err);
    return false;
  }


  err = (*mpEngineItf)->CreateOutputMix(mpEngineItf, &mpOutputMix, 0, NULL, NULL);
  if (err == SL_RESULT_SUCCESS)
    LOGI("create output mix OK");
  else
  {
    LOGI("create output mix ERROR %d", err);
    return false;
  }

  err = (*mpOutputMix)->Realize(mpOutputMix, SL_BOOLEAN_FALSE);
  if (err == SL_RESULT_SUCCESS)
    LOGI("realize output mix OK");
  else
  {
    LOGI("realize output mix ERROR %d", err);
    return false;
  }

  int channels = 2;
  int channelMask = (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT);
  SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
  SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, channels, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16, channelMask, SL_BYTEORDER_LITTLEENDIAN};
  SLDataSource audioSrc = {&loc_bufq, &format_pcm};

  // configure audio sink
  SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, mpOutputMix};
  SLDataSink audioSnk = {&loc_outmix, NULL};

  // create audio player
  const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
  const SLboolean req[1] = {SL_BOOLEAN_TRUE};
  err = (*mpEngineItf)->CreateAudioPlayer(mpEngineItf, &mpPlayer, &audioSrc, &audioSnk, 1, ids, req);
  if (err == SL_RESULT_SUCCESS)
    LOGI("create player OK");
  else
  {
    LOGI("create player ERROR %d", err);
    return false;
  }

  err = (*mpPlayer)->Realize(mpPlayer, SL_BOOLEAN_FALSE);
  if (err == SL_RESULT_SUCCESS)
    LOGI("realize player OK");
  else
  {
    LOGI("realize player ERROR %d", err);
    return false;
  }

  // get the play interface
  err = (*mpPlayer)->GetInterface(mpPlayer, SL_IID_PLAY, &mpPlayItf);
  if (err == SL_RESULT_SUCCESS)
    LOGI("get play interface OK");
  else
  {
    LOGI("get play interface ERROR %d", err);
    return false;
  }

  // get the buffer queue interface
  err = (*mpPlayer)->GetInterface(mpPlayer, SL_IID_BUFFERQUEUE, &mpBufferQueue);
  if (err == SL_RESULT_SUCCESS)
    LOGI("get buffer queue interface OK");
  else
  {
    LOGI("get buffer queue interface ERROR %d", err);
    return false;
  }

  // register callback on the buffer queue
  err = (*mpBufferQueue)->RegisterCallback(mpBufferQueue, nuiAudioDevice_Android_IOProc, this);
  if (err == SL_RESULT_SUCCESS)
    LOGI("register callback OK");
  else
  {
    LOGI("register callback ERROR %d", err);
    return false;
  }

  // set the player's state to playing
  err = (*mpPlayItf)->SetPlayState(mpPlayItf, SL_PLAYSTATE_PLAYING);
  if (err == SL_RESULT_SUCCESS)
    LOGI("play OK");
  else
  {
    LOGI("play ERROR %d", err);
    return false;
  }


  LOGI("first fill");
  nuiAudioDevice_Android_IOProc(mpBufferQueue, this);
  LOGI("first fill done");

  return true;
}

bool nuiAudioDevice_Android::Close()
{
  SLresult err;
  err = (*mpPlayItf)->SetPlayState(mpPlayItf, SL_PLAYSTATE_STOPPED);
  if (err == SL_RESULT_SUCCESS)
    LOGI(" nuiAudioDevice_Android stop playing OK");
  else
    LOGI("nuiAudioDevice_Android stop playing ERROR %d", err);


  (*mpPlayer)->Destroy(mpPlayer);
  (*mpOutputMix)->Destroy(mpOutputMix);
  (*mpEngine)->Destroy(mpEngine);
}


void nuiAudioDevice_Android::Process()
{
  nglTime now;
  double elapsed = now.GetValue() - mLast.GetValue();
  mLast = now;

  std::vector<const float*> in;
  std::vector<float*> out;

  for (int32 c = 0; c < mInputSamples.size(); c++)
    in.push_back(&(mInputSamples[c][0]));

  for (int32 c = 0; c < mOutputSamples.size(); c++)
    out.push_back(&(mOutputSamples[c][0]));

  mAudioProcessFn(in, out, mBufferSize);

  int32 channels = mOutputSamples.size();
  for (int32 c = 0; c < channels; c++)
  {
    nuiAudioConvert_DEfloatToINint16(&(mOutputSamples[c][0]), &(mOutputInterleaved[0]), c, channels, mBufferSize);
  }

  // enqueue another buffer
  SLresult err = (*mpBufferQueue)->Enqueue(mpBufferQueue, &(mOutputInterleaved[0]), mBufferSize * channels * sizeof(int16));
  if (err != SL_RESULT_SUCCESS)
    LOGI("nuiAudioDevice_Android::Process Enqueue ERROR %d", err);

  nglTime end;
  double processTime = end.GetValue() - now.GetValue();
}

void nuiAudioDevice_Android::EnumSampleRates()
{
  mSampleRates.clear();
  mSampleRates.push_back(44100);
}

void nuiAudioDevice_Android::EnumBufferSizes()
{
  mBufferSizes.clear();
  mBufferSizes.push_back(256);
  mBufferSizes.push_back(512);
  mBufferSizes.push_back(1024);
  mBufferSizes.push_back(2048);
  mBufferSizes.push_back(4096);
  mBufferSizes.push_back(8192);
  mBufferSizes.push_back(16384);
  mBufferSizes.push_back(32768);
}


//
//
// API
//
//
nuiAudioDeviceAPI_Android::nuiAudioDeviceAPI_Android()
{
  mName = API_NAME;
}

nuiAudioDeviceAPI_Android::~nuiAudioDeviceAPI_Android()
{
}

int32 nuiAudioDeviceAPI_Android::GetDeviceCount() const
{
  return 1;
}

nuiAudioDevice* nuiAudioDeviceAPI_Android::GetDevice(int32 index)
{
  if (index == 0)
    return new nuiAudioDevice_Android();

  return NULL;
}

nuiAudioDevice* nuiAudioDeviceAPI_Android::GetDevice(const nglString& rDeviceName)
{
  if (GetDeviceName(0) == rDeviceName)
    return GetDevice(0);

  return NULL;
}

nglString nuiAudioDeviceAPI_Android::GetDeviceName(int32 index) const
{
  if (index == 0)
    return nglString(_T("Output"));

  return nglString::Null;
}

nuiAudioDevice* nuiAudioDeviceAPI_Android::GetDefaultOutputDevice()
{
  return GetDevice(0);
}

nuiAudioDevice* nuiAudioDeviceAPI_Android::GetDefaultInputDevice()
{
  return GetDevice(0);
}

nuiAudioDeviceAPI_Android AndroidAudioAPI;

