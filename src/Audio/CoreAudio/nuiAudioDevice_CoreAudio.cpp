/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiAudioDevice_CoreAudio.h"

#define API_NAME _T("CoreAudio")

#ifdef _DEBUG
  #define nui_verify_noerr(errorCode)                                            \
  do                                                                      \
  {                                                                       \
    if ( 0 != (errorCode) )                                             \
    {                                                                   \
    }                                                                   \
  } while ( 0 )
#else
  #define nui_verify_noerr(errorCode)                                            \
  do                                                                      \
  {                                                                       \
    int evalOnceErrorCode = (errorCode);                                \
    if ( 0 != evalOnceErrorCode )                                       \
    {                                                                   \
      DEBUG_ASSERT_MESSAGE(                                           \
        DEBUG_ASSERT_COMPONENT_NAME_STRING,                         \
        #errorCode " == 0 ",                                        \
        0,                                                          \
        0,                                                          \
        __FILE__,                                                   \
        __LINE__,                                                   \
        evalOnceErrorCode);                                         \
    }                                                                   \
  } while ( 0 )
#endif


class AudioDeviceState
  {
  public:
    AudioDeviceState()
    {
      mSampleRate = 0;
      mBufferSize = 0;
      mInputLatency = 0;
      mOutputLatency = 0;
    }
    
    bool Update(AudioDeviceID ID)
    {
      if (ID == 0)
        return false;
      
      Float64 sr;
      UInt32 size = sizeof(Float64);
      if (!AudioDeviceGetProperty(ID, 0, false, kAudioDevicePropertyNominalSampleRate, &size, &sr))
        mSampleRate = sr;
      
      UInt32 FPB;
      size = sizeof (FPB);
      if (!AudioDeviceGetProperty(ID, 0, false, kAudioDevicePropertyBufferFrameSize, &size, &FPB))
        mBufferSize = FPB;
      
      mInputLatency = 0;
      mOutputLatency = 0;
      UInt32 Latency;
      size = sizeof (UInt32);
      if (!AudioDeviceGetProperty(ID, 0, true, kAudioDevicePropertyLatency, &size, &Latency))
        mInputLatency = Latency;
      
      size = sizeof(UInt32);
      if (!AudioDeviceGetProperty(ID, 0, false, kAudioDevicePropertyLatency, &size, &Latency))
        mOutputLatency = Latency;
      
      return true;
    }
    
    double mSampleRate;
    uint32 mBufferSize;
    uint32 mInputLatency;
    uint32 mOutputLatency;
  };

nuiAudioDevice_CoreAudio::nuiAudioDevice_CoreAudio(AudioDeviceID id)
{
  mDeviceID = id;
  
  mAPIName = _T("CoreAudio");
  
  char buf[1024];
  UInt32 maxlen = 1024;
  if (AudioDeviceGetProperty(mDeviceID, 0, false, kAudioDevicePropertyDeviceName, &maxlen, buf) != 0) // try as an output
  {
    nui_verify_noerr(AudioDeviceGetProperty(mDeviceID, 0, true, kAudioDevicePropertyDeviceName, &maxlen, buf)); // then as an input
  }
  mName.Import(buf);
  
  if (AudioDeviceGetProperty(mDeviceID, 0, false, kAudioDevicePropertyDeviceManufacturer, &maxlen, buf) != 0) // try as an output
  {
    nui_verify_noerr(AudioDeviceGetProperty(mDeviceID, 0, true, kAudioDevicePropertyDeviceManufacturer, &maxlen, buf)); // then as an input
  }
  mManufacturer.Import(buf);
  
  EnumSampleRates(); /// Fill mSampleRates;
  EnumBufferSizes(); /// Fill mBufferSizes;
  EnumChannels();
  
  mIsPresent = true; ///#FIXME there is a way to ask CoreAudio for this
  
}

nuiAudioDevice_CoreAudio::~nuiAudioDevice_CoreAudio()
{
  Close();
}

bool nuiAudioDevice_CoreAudio::Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction)
{
  mAudioProcessFn = pProcessFunction;
  
  {
    mActiveInputChannels = rInputChannels;
    mInputSamples.resize(mActiveInputChannels.size());
    for (uint32 ch = 0; ch < mInputSamples.size(); ch++)
      mInputSamples[ch].resize(BufferSize);
  }
  
  {
    mActiveOutputChannels = rOutputChannels;
    mOutputSamples.resize(mActiveOutputChannels.size());
    for (uint32 ch = 0; ch < mOutputSamples.size(); ch++)
      mOutputSamples[ch].resize(BufferSize);
  }
  
  OSStatus error = 0;
  
  // set sample rate only if it needs to change (changing causes disruption in audio of other applications)
  Float64 sr = SampleRate;
  Float64 currentSr;
  UInt32 size = sizeof (sr);
  error = AudioDeviceGetProperty(mDeviceID, 0, false, kAudioDevicePropertyNominalSampleRate, &size, &currentSr);
  if (memcmp(&currentSr, &sr, size))
  {
    error = AudioDeviceSetProperty (mDeviceID, 0, 0, false, kAudioDevicePropertyNominalSampleRate, size, &sr);
  }
  
  error = AudioDeviceGetProperty(mDeviceID, 0, true, kAudioDevicePropertyNominalSampleRate, &size, &currentSr);
  if (memcmp(&currentSr, &sr, size))
  {
    error = AudioDeviceSetProperty (mDeviceID, 0, 0, true, kAudioDevicePropertyNominalSampleRate, size, &sr);
  }
  
  // change buffer size
  UInt32 FPB = BufferSize;
  {
    uint32 minbufsize = mBufferSizes[0];
    uint32 maxbufsize = mBufferSizes[0];
    for (uint32 i = 1; i < mBufferSizes.size(); i++)
    {
      uint32 s = mBufferSizes[i];
      minbufsize = MIN(minbufsize, s); 
      maxbufsize = MAX(maxbufsize, s); 
    }
    
    if (FPB > maxbufsize)
      FPB = maxbufsize;
    if (FPB < minbufsize)
      FPB = minbufsize;
  }
  
  
  size = sizeof (FPB);
  
  error = AudioDeviceSetProperty (mDeviceID, 0, 0, false, kAudioDevicePropertyBufferFrameSize, size, &FPB);
  error = AudioDeviceSetProperty (mDeviceID, 0, 0, true,  kAudioDevicePropertyBufferFrameSize, size, &FPB);
  
  // wait for the changes to happen (on some devices)
  AudioDeviceState state;
  int i = 30;
  while (--i >= 0)
  {
    state.Update(mDeviceID);
    
    if (SampleRate == state.mSampleRate && FPB == state.mBufferSize)
      break;
    
    nglThread::MsSleep(100);
  }
  
  mSampleRate = state.mSampleRate;
  mBufferSize = state.mBufferSize;
  
  if (i < 0)
    return false;
  
  mInMap.clear();
  mOutMap.clear();
  
  error = AudioDeviceAddIOProc(mDeviceID, IOProc, this);
  if (!error)
    error = AudioDeviceStart(mDeviceID, IOProc);
  return !error;
}

bool nuiAudioDevice_CoreAudio::Close()
{
  OSStatus error = AudioDeviceStop(mDeviceID, IOProc);
  if (!error)
    error = AudioDeviceRemoveIOProc (mDeviceID, IOProc);
  return !error;
}

OSStatus nuiAudioDevice_CoreAudio::IOProc(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                                          AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime, void* inClientData)
{
  // Temporary thread priority boost...
  int policy;
  struct sched_param sched_original;
  struct sched_param sched;
  pthread_getschedparam(pthread_self(), &policy, &sched);
  pthread_getschedparam(pthread_self(), &policy, &sched_original);
  sched.sched_priority = sched.sched_priority + 50 > 120 ? 120 : sched.sched_priority +50;
  pthread_setschedparam(pthread_self(), policy, &sched);
  
  nuiAudioDevice_CoreAudio* pThis = (nuiAudioDevice_CoreAudio*)inClientData;
  NGL_ASSERT(pThis);
  OSStatus status = pThis->Process(inDevice, inNow, inInputData, inInputTime, outOutputData, inOutputTime);
  
  // Temporary thread priority boost end...
  pthread_setschedparam(pthread_self(), policy, &sched_original);
  
  return status;
}

OSStatus nuiAudioDevice_CoreAudio::Process(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                                           AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime)
{
  uint32 ch = 0;
  
  if (mInMap.empty() && !mActiveInputChannels.empty())
  {
    // Create input and output maping tables:
    mInMap.resize(mInputChannels.size());
    uint32 buf = 0;
    uint32 bufch = 0;
    for (ch = 0; ch < mInputChannels.size(); ch++)
    {
      mInMap[ch] = std::pair<uint32, uint32>(buf, bufch);
      
      bufch++;
      if (bufch >= inInputData->mBuffers[buf].mNumberChannels)
      {
        buf++;
        bufch = 0;
      }
    }
  }
  
  if (mOutMap.empty() && !mActiveOutputChannels.empty())
  {
    mOutMap.resize(mOutputChannels.size());
    uint32 buf = 0;
    uint32 bufch = 0;
    for (ch = 0; ch < mOutputChannels.size(); ch++)
    {
      mOutMap[ch] = std::pair<uint32, uint32>(buf, bufch);
      
      bufch++;
      if (bufch >= outOutputData->mBuffers[buf].mNumberChannels)
      {
        buf++;
        bufch = 0;
      }
    }
  }
  
  // Fetch the audio input and put it in our buffers if needed:
  mInputBuffers.resize(mActiveInputChannels.size());
  for (ch = 0; ch < mActiveInputChannels.size(); ch++)
  {
    uint32 Channel = mActiveInputChannels[ch];
    NGL_ASSERT(Channel < mInputChannels.size());
    
    // Find the buffer and the channel inside the buffer:
    uint32 buffer  = mInMap[Channel].first;
    uint32 channel = mInMap[Channel].second;
    
    // Copy the data if needed (only when it's interleaved)
    uint32 stride = inInputData->mBuffers[buffer].mNumberChannels;
    if (stride == 1)
    {
      mInputBuffers[ch] = (float*)inInputData->mBuffers[buffer].mData;
    }
    else
    {
      // We need to deinterleave:
      float* pData = (float*)inInputData->mBuffers[buffer].mData;
      pData += channel;
      for (uint32 s = 0; s < mBufferSize; s++)
      {
        mInputSamples[Channel][s] = *pData;
        pData += stride;
      }
      mInputBuffers[ch] = &mInputSamples[Channel][0];
    }
  }
  
  // Prepare the output buffers list:
  mOutputBuffers.resize(mActiveOutputChannels.size());
  for (ch = 0; ch < mActiveOutputChannels.size(); ch++)
  {
    uint32 Channel = mActiveOutputChannels[ch];
    NGL_ASSERT(Channel < mOutputChannels.size());
    
    // Find the buffer and the channel inside the buffer:
    uint32 buffer  = mOutMap[Channel].first;
    uint32 channel = mOutMap[Channel].second;
    
    // Copy the data if needed (only when it's interleaved)
    uint32 stride = outOutputData->mBuffers[buffer].mNumberChannels;
    if (stride == 1)
    {
      mOutputBuffers[Channel] = (float*)outOutputData->mBuffers[buffer].mData;
    }
    else
    {
      // We need to deinterleave so we use a temp buffer:
      mOutputBuffers[ch] = &mOutputSamples[ch][0];
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // We are now ready to call our processing delegate:
  mAudioProcessFn(mInputBuffers, mOutputBuffers, mBufferSize);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // Put back the output data in the actual buffers:
  mOutputBuffers.resize(mActiveOutputChannels.size());
  for (ch = 0; ch < mActiveOutputChannels.size(); ch++)
  {
    uint32 Channel = mActiveOutputChannels[ch];
    NGL_ASSERT(Channel < mOutputChannels.size());
    
    // Find the buffer and the channel inside the buffer:
    uint32 buffer  = mOutMap[Channel].first;
    uint32 channel = mOutMap[Channel].second;
    
    // Copy the data if needed (only when it's interleaved)
    uint32 stride = outOutputData->mBuffers[buffer].mNumberChannels;
    if (stride == 1)
    {
      // Do nothing, the buffer was modified inplace
    }
    else
    {
      // We need to interleave:
      float* pData = (float*)outOutputData->mBuffers[buffer].mData;
      float* pOutputSamples = &(mOutputSamples[Channel][0]);
      pData += channel;
      for (uint32 s = 0; s < mBufferSize; s++)
      {
        *pData = *(pOutputSamples++);
        pData += stride;
      }
    }
  }
  
  return 0;
}


void nuiAudioDevice_CoreAudio::EnumSampleRates()
{
  mSampleRates.clear();
  
  UInt32 RangeCount = GetPropertyDataSize(0, false, kAudioDevicePropertyAvailableNominalSampleRates)  / sizeof(AudioValueRange);
  
  std::vector<AudioValueRange> ranges;
  ranges.resize(RangeCount);
  
  UInt32 Size = RangeCount * sizeof(AudioValueRange);
  GetPropertyData(0, false, kAudioDevicePropertyAvailableNominalSampleRates, Size, &ranges[0]);
  
  uint32 ioNumberRanges = Size / sizeof(AudioValueRange);
  
  std::set<double> rates;
  double defaultrates[] = { 8000, 16000, 22050, 32000, 44100, 48000, 88200, 96000, 128000, 176400, 192000, 0 };
  
  for (uint32 i = 0; i < ioNumberRanges; i++)
  {
    double sr = ranges[i].mMinimum;
    uint32 j = 0;
    while (defaultrates[j] && sr > defaultrates[j])
      j++;
    
    for (; sr <= ranges[i].mMaximum && defaultrates[j];)
    {
      rates.insert(sr);
      sr = defaultrates[j++];
    }
  }
  
  std::set<double>::iterator it = rates.begin();
  std::set<double>::iterator end = rates.end();
  
  while (it != end)
  {
    mSampleRates.push_back(*it);
    it++;
  }
}

void nuiAudioDevice_CoreAudio::EnumBufferSizes()
{
  mBufferSizes.clear();
  
  UInt32 RangeCount = GetPropertyDataSize(0, false, kAudioDevicePropertyBufferFrameSizeRange)  / sizeof(AudioValueRange);
  
  std::vector<AudioValueRange> ranges;
  ranges.resize(RangeCount);
  
  UInt32 Size = RangeCount * sizeof(AudioValueRange);
  GetPropertyData(0, false, kAudioDevicePropertyBufferFrameSizeRange, Size, &ranges[0]);
  
  uint32 ioNumberRanges = Size / sizeof(AudioValueRange);
  
  std::set<double> sizes;
  double defaultsizes[] = { 8, 16, 32, 64, 96, 128, 192, 256, 384, 512, 768, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 0 };
  
  for (uint32 i = 0; i < ioNumberRanges; i++)
  {
    double sr = ranges[i].mMinimum;
    uint32 j = 0;
    while (defaultsizes[j] && sr > defaultsizes[j])
      j++;
    
    for (; sr <= ranges[i].mMaximum && defaultsizes[j];)
    {
      sizes.insert(sr);
      sr = defaultsizes[j++];
    }
  }
  
  std::set<double>::iterator it = sizes.begin();
  std::set<double>::iterator end = sizes.end();
  
  while (it != end)
  {
    mBufferSizes.push_back(*it);
    it++;
  }    
}

nglString nuiAudioDevice_CoreAudio::GetChannelName(bool IsInput, uint32 index) const
{
  UInt32 size = 0;
  OSStatus error = AudioDeviceGetPropertyInfo(mDeviceID, index + 1, IsInput, kAudioDevicePropertyChannelName, &size, NULL); 
  if (!error)    
  {      
    char* pName = new char[size];
    pName = new char[size+1]; 
    
    error = AudioDeviceGetProperty(mDeviceID, index + 1, IsInput, kAudioDevicePropertyChannelName, &size, pName);
    
    if ( !error )
    {
      nglString tmp(pName);
      delete pName;
      return tmp;
    }
    delete pName;
  }
  
  nglString str;
  if (IsInput)
    str.CFormat(_T("%ls:in[%d]"), mName.GetChars(), index + 1);
  else
    str.CFormat(_T("%ls:out[%d]"), mName.GetChars(), index + 1);
  return str; 
}  

void nuiAudioDevice_CoreAudio::EnumChannels()
{
  // get number of input channels
  OSErr err = 0;
  UInt32 outSize = 0;
  uint32 theNumberInputChannels = 0;
  err = AudioDeviceGetPropertyInfo( mDeviceID, 0, 1, kAudioDevicePropertyStreamConfiguration, &outSize, NULL);
  if((err == noErr) && (outSize != 0))
  {
    AudioBufferList* theBufferList = (AudioBufferList*)malloc(outSize);
    if (theBufferList != NULL)
    {
      // get the input stream configuration
      err = AudioDeviceGetProperty( mDeviceID, 0, 1, kAudioDevicePropertyStreamConfiguration, &outSize, theBufferList);
      if(err == noErr)
      {
        // count the total number of input channels in the stream
        for (uint32 theIndex = 0; theIndex < theBufferList->mNumberBuffers; ++theIndex)
          theNumberInputChannels += theBufferList->mBuffers[theIndex].mNumberChannels;
      }
      free(theBufferList);
    }
  }
  
  // get number of output channels
  outSize = 0;
  uint32 theNumberOutputChannels = 0;
  err = AudioDeviceGetPropertyInfo( mDeviceID, 0, 0, kAudioDevicePropertyStreamConfiguration, &outSize, NULL);
  if ((err == noErr) && (outSize != 0))
  {
    AudioBufferList* theBufferList = (AudioBufferList*)malloc(outSize);
    if (theBufferList != NULL)
    {
      // get the input stream configuration
      err = AudioDeviceGetProperty( mDeviceID, 0, 0, kAudioDevicePropertyStreamConfiguration, &outSize, theBufferList);
      if(err == noErr)
      {
        // count the total number of output channels in the stream
        for(uint32 theIndex = 0; theIndex < theBufferList->mNumberBuffers; ++theIndex)
          theNumberOutputChannels += theBufferList->mBuffers[theIndex].mNumberChannels;
      }
      free(theBufferList);
    }
  }
  
  /// Get The Channel names:
  OSStatus error; 
  
  for (uint32 i = 0; i < theNumberOutputChannels; i++)
  {
    mOutputChannels.push_back(GetChannelName(false, i));
  }
  
  for (uint32 i = 0; i < theNumberInputChannels; i++)
  {
    mInputChannels.push_back(GetChannelName(true, i));
  }
  
}

UInt32  nuiAudioDevice_CoreAudio::GetPropertyDataSize(UInt32 inChannel, bool inSection, AudioHardwarePropertyID inPropertyID) const
{
  UInt32 Size = 0;
  OSStatus Error;
  Error = AudioDeviceGetPropertyInfo(mDeviceID, inChannel, inSection, inPropertyID, &Size, NULL);
  NGL_ASSERT(!Error);
  return Size;
}

void nuiAudioDevice_CoreAudio::GetPropertyData(UInt32 inChannel, bool inSection, AudioHardwarePropertyID inPropertyID, UInt32& ioDataSize, void* outData) const
{
  OSStatus Error;
  Error = AudioDeviceGetProperty(mDeviceID, inChannel, inSection, inPropertyID, &ioDataSize, outData);
  NGL_ASSERT(!Error);
}

void nuiAudioDevice_CoreAudio::SetPropertyData(UInt32 inChannel, bool inSection, AudioHardwarePropertyID inPropertyID, UInt32 inDataSize, const void* inData, const AudioTimeStamp* inWhen)
{
  OSStatus Error;
  Error = AudioDeviceSetProperty(mDeviceID, inWhen, inChannel, inSection, inPropertyID, inDataSize, inData);
  NGL_ASSERT(!Error);
}


nuiAudioDeviceAPI_CoreAudio::nuiAudioDeviceAPI_CoreAudio()
{
  mName = API_NAME;
  RegisterWithManager();
}

nuiAudioDeviceAPI_CoreAudio::~nuiAudioDeviceAPI_CoreAudio()
{
}

uint32 nuiAudioDeviceAPI_CoreAudio::GetDeviceCount() const
{
  UInt32 propsize;
  mDeviceIDs.clear();
  
  nui_verify_noerr(AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &propsize, NULL));
  int nDevices = propsize / sizeof(AudioDeviceID);	
  mDeviceIDs.resize(nDevices);
  nui_verify_noerr(AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &propsize, &mDeviceIDs[0]));
  
  return mDeviceIDs.size();
}

nuiAudioDevice* nuiAudioDeviceAPI_CoreAudio::GetDevice(uint32 index)
{
  return new nuiAudioDevice_CoreAudio(mDeviceIDs[index]);
}

nuiAudioDevice* nuiAudioDeviceAPI_CoreAudio::GetDevice(const nglString& rDeviceName)
{
  for (uint32 i = 0; i < mDeviceIDs.size(); i++)
  {
    if (GetDeviceName(i) == rDeviceName)
      return GetDevice(i);
  }
  return NULL;
}

nglString nuiAudioDeviceAPI_CoreAudio::GetDeviceName(uint32 index) const
{
  UInt32 propsize;
  CFStringRef tempStr;
  nui_verify_noerr(AudioDeviceGetPropertyInfo(mDeviceIDs[index], 0, false, kAudioDevicePropertyDeviceNameCFString, &propsize, NULL));
  nui_verify_noerr(AudioDeviceGetProperty(mDeviceIDs[index], 0, false, kAudioDevicePropertyDeviceNameCFString, &propsize, &tempStr));
  nglString str(tempStr);
  CFRelease(tempStr);
  return str;
}

nuiAudioDevice* nuiAudioDeviceAPI_CoreAudio::GetDefaultOutputDevice()
{
  AudioDeviceID deviceID = 0;
  UInt32 size;
  nui_verify_noerr(AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDefaultOutputDevice, &size, NULL));
  nui_verify_noerr(AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice, &size, &deviceID));
  return new nuiAudioDevice_CoreAudio(deviceID);
}

nuiAudioDevice* nuiAudioDeviceAPI_CoreAudio::GetDefaultInputDevice()
{
  AudioDeviceID deviceID = 0;
  UInt32 size;
  nui_verify_noerr(AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDefaultInputDevice, &size, NULL));
  nui_verify_noerr(AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice, &size, &deviceID));
  return new nuiAudioDevice_CoreAudio(deviceID);
}

nuiAudioDeviceAPI_CoreAudio CoreAudioAPI;

