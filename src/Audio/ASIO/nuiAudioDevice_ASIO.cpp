/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAudioDevice_ASIO.h"
#include "nuiAudioConvert.h"
#include "nglThread.h"
#include "nglRingBuffer.h"

#define API_NAME _T("ASIO")

inline float	fmin(float a, float b) { if (a < b) return a; else return b; }
inline float	fmax(float a, float b) { if (a < b) return b; else return a; }
inline float	fclamp(float f, float xmin, float xmax)
{
	assert( xmin <= xmax );
	return fmax(xmin, fmin(f, xmax));
}


template <typename T>
inline bool Contains(const std::vector<T>& rVec, const T& rItem)
{
  std::vector<T>::const_iterator end = rVec.end();
  for (std::vector<T>::const_iterator it = rVec.begin(); it != end; ++it)
  {
    if ((*it) == rItem)
      return true;
  }
  return false;
}

nuiAudioDevice_ASIO::nuiAudioDevice_ASIO(IASIO* pDriver)
: mpDriver(pDriver), mBufferSize(0)
{
  ASIOBool res = pDriver->init((void*)App->GetHWnd());
  NGL_ASSERT(res);

  mAPIName = API_NAME;

  char name[255];
  pDriver->getDriverName(name);
  mName = nglString(name, eUTF8);

  mManufacturer = _T("");

  ASIOError err;
  const double sampleRates[] = { 44100.0, 48000.0, 88200.0, 96000.0, 176400.0, 192000.0 };
  const uint numSampleRates = 6;
  for (uint i = 0; i < numSampleRates; i++)
  {
    err = pDriver->canSampleRate(sampleRates[i]);
    if (err == ASE_OK)
    {
      mSampleRates.push_back(sampleRates[i]);
    }
  }

  if (mSampleRates.size() == 0)
  {
    double sampleRate;
    err = pDriver->getSampleRate(&sampleRate);
    if (err == ASE_OK)
      mSampleRates.push_back(sampleRate);
  }

  long minBufSize, maxBufSize, prefBufSize, granularity;

  pDriver->getBufferSize(&minBufSize, &maxBufSize, &prefBufSize, &granularity);
  if (granularity == -1)
  {
    uint32 size = minBufSize;
    while (size <= maxBufSize)
    {
      mBufferSizes.push_back(size);
      size <<= 1;
    }
  }
  else
  {
    uint32 size = minBufSize;
    while (size <= maxBufSize)
    {
      mBufferSizes.push_back(size);
      size += granularity;
    }
  }

  long numInChannels, numOutChannels;
  pDriver->getChannels(&numInChannels, &numOutChannels);
  for (uint i = 0; i < numInChannels; i++)
  {
    ASIOChannelInfo info;
    info.channel = i;
    info.isInput = ASIOTrue;
    pDriver->getChannelInfo(&info);
    mInputChannels.push_back(nglString(info.name, eUTF8));
    mInputSampleTypes.push_back(info.type);
  }

  for (uint i = 0; i < numOutChannels; i++)
  {
    ASIOChannelInfo info;
    info.channel = i;
    info.isInput = ASIOFalse;
    pDriver->getChannelInfo(&info);
    mOutputChannels.push_back(nglString(info.name, eUTF8));
    mOutputSampleTypes.push_back(info.type);
  }
}

nuiAudioDevice_ASIO::~nuiAudioDevice_ASIO()  
{
  Close();
  nuiAudioDevice_ASIO::gpCurrentDevice = NULL;
}


bool nuiAudioDevice_ASIO::Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction)
{
  if (Contains(mSampleRates, SampleRate) && Contains(mBufferSizes, BufferSize))
  {
    Process = pProcessFunction;

    mSampleRate = SampleRate;
    mBufferSize = BufferSize;
    mNumInputs = rInputChannels.size();
    mNumOutputs = rOutputChannels.size();
    mBufferInfos.resize(mNumInputs + mNumOutputs);

    std::vector<uint32>::const_iterator it;
    std::vector<uint32>::const_iterator end;
    int i = 0;

    end = rInputChannels.end();
    for (it = rInputChannels.begin(), i=0; it != end; ++it, i++)
    {
      mBufferInfos[i].channelNum = *it;
      mBufferInfos[i].isInput = ASIOTrue;
      mBufferInfos[i].buffers[0] = mBufferInfos[i].buffers[1] = NULL;
    }

    end = rOutputChannels.end();
    for (it = rOutputChannels.begin(), i=mNumInputs; it != end; ++it, i++)
    {
      mBufferInfos[i].channelNum = *it;
      mBufferInfos[i].isInput = ASIOFalse;
      mBufferInfos[i].buffers[0] = mBufferInfos[i].buffers[1] = NULL;
    }

    ASIOError res = mpDriver->createBuffers(&mBufferInfos[0], mNumInputs + mNumOutputs, BufferSize, &nuiAudioDevice_ASIO::gCallbacks);
    if (res != ASE_OK)
      return false;

    mInputBuffers.resize(mNumInputs);
    mInputConversionFuncs.clear();
    for (i = 0; i < mNumInputs; i++)
    {
      mInputBuffers[i] = new float[BufferSize];
      switch (mInputSampleTypes[mBufferInfos[i].channelNum])
      {
      case ASIOSTInt16MSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt16MSBToFloat);
        break;
      case ASIOSTInt24MSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt24MSBToFloat);
        break;
      case ASIOSTInt32MSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32MSBToFloat);
        break;
      case ASIOSTFloat32MSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloat32MSBToFloat);
        break;
      case ASIOSTFloat64MSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloat64MSBToFloat);
        break;
      case ASIOSTInt32MSB16:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32MSBToFloat);
        break;
      case ASIOSTInt32MSB18:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32MSBToFloat);
        break;
      case ASIOSTInt32MSB20:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32MSBToFloat);
        break;
      case ASIOSTInt32MSB24:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32MSBToFloat);
        break;
      case ASIOSTInt16LSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt16LSBToFloat);
        break;
      case ASIOSTInt24LSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt24LSBToFloat);
        break;
      case ASIOSTInt32LSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32LSBToFloat);
        break;
      case ASIOSTFloat32LSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloat32LSBToFloat);
        break;
      case ASIOSTFloat64LSB:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloat64LSBToFloat);
        break;
      case ASIOSTInt32LSB16:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32LSBToFloat);
        break;
      case ASIOSTInt32LSB18:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32LSBToFloat);
        break;
      case ASIOSTInt32LSB20:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32LSBToFloat);
        break;
      case ASIOSTInt32LSB24:
        mInputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertInt32LSBToFloat);
        break;   
      default:
        NGL_ASSERT(0);
      }
    }

    mOutputBuffers.resize(mNumOutputs);
    for (i = mNumInputs; i < (mNumInputs+mNumOutputs); i++)
    {
      mOutputBuffers[i] = new float[BufferSize];
      switch (mOutputSampleTypes[mBufferInfos[i].channelNum])
      {
      case ASIOSTInt16MSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt16MSB);
        break;
      case ASIOSTInt24MSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt24MSB);
        break;
      case ASIOSTInt32MSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32MSB);
        break;
      case ASIOSTFloat32MSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToFloat32MSB);
        break;
      case ASIOSTFloat64MSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToFloat64MSB);
        break;
      case ASIOSTInt32MSB16:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32MSB);
        break;
      case ASIOSTInt32MSB18:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32MSB);
        break;
      case ASIOSTInt32MSB20:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32MSB);
        break;
      case ASIOSTInt32MSB24:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32MSB);
        break;
      case ASIOSTInt16LSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt16LSB);
        break;
      case ASIOSTInt24LSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt24LSB);
        break;
      case ASIOSTInt32LSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32LSB);
        break;
      case ASIOSTFloat32LSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToFloat32LSB);
        break;
      case ASIOSTFloat64LSB:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToFloat64LSB);
        break;
      case ASIOSTInt32LSB16:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32LSB);
        break;
      case ASIOSTInt32LSB18:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32LSB);
        break;
      case ASIOSTInt32LSB20:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32LSB);
        break;
      case ASIOSTInt32LSB24:
        mOutputConversionFuncs.push_back(&nuiAudioDevice_ASIO::ConvertFloatToInt32LSB);
        break;   
      default:
        NGL_ASSERT(0);
      }
    }

    res = mpDriver->setSampleRate(SampleRate);
    if (res != ASE_OK)
    {
      mpDriver->disposeBuffers();
      return false;
    }

    res = mpDriver->start();
    if (res != ASE_OK)
    {
      mpDriver->disposeBuffers();
      return false;
    }

    return true;
  }
  return false;
}

bool nuiAudioDevice_ASIO::Close()
{
  mpDriver->stop();
  ASIOError err = mpDriver->disposeBuffers();
  if (err == ASE_OK)
  {
    std::vector<float*>::const_iterator inputEnd = mInputBuffers.end();
    for (std::vector<float*>::const_iterator it = mInputBuffers.begin(); it != inputEnd; ++it)
      delete[] *it;

    std::vector<float*>::const_iterator outputEnd = mOutputBuffers.end();
    for (std::vector<float*>::const_iterator it = mOutputBuffers.begin(); it != outputEnd; ++it)
      delete[] *it;
  }
  mInputBuffers.clear();
  mOutputBuffers.clear();
  return true;
}

void nuiAudioDevice_ASIO::Reset()
{
  std::vector<uint32> inputs, outputs;
  for (uint i = 0; i < mNumInputs; i++)
  {
    inputs.push_back(mBufferInfos[i].channelNum);
  }
  for (uint i = 0; i < mNumOutputs; i++)
  {
    inputs.push_back(mBufferInfos[i + mNumInputs].channelNum);
  }
  Close();
  Open(inputs, outputs, mSampleRate, mBufferSize, Process);
}

void nuiAudioDevice_ASIO::BufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
  for (uint i = 0; i < mNumInputs; i++)
  {
    mInputConversionFuncs[i]((const uint8*) mBufferInfos[i].buffers[doubleBufferIndex], mInputBuffers[i], mBufferSize);
  }

  //#HACK: ugly cast into vector<const float*>
  std::vector<const float*>& rInputBuffers = *(std::vector<const float*>*) &mInputBuffers;
  Process(rInputBuffers, mOutputBuffers, mBufferSize);

  for (uint i = mNumInputs; i < (mNumInputs + mNumOutputs); i++)
  {
    mOutputConversionFuncs[i](mOutputBuffers[i], (uint8*)mBufferInfos[i].buffers[doubleBufferIndex], mBufferSize);
  }
}

void nuiAudioDevice_ASIO::BufferSwitchCallback(long doubleBufferIndex, ASIOBool directProcess)
{
  nuiAudioDevice_ASIO::gpCurrentDevice->BufferSwitch(doubleBufferIndex, directProcess);
}

void nuiAudioDevice_ASIO::SampleRateDidChange(ASIOSampleRate sRate)
{
  nuiAudioDevice_ASIO::gpCurrentDevice->mSampleRate = sRate;
  nuiAudioDevice_ASIO::gpCurrentDevice->Reset();
}

long nuiAudioDevice_ASIO::AsioMessage (long selector, long value, void* message, double* opt)
{
  switch (selector)
  {
  case kAsioSelectorSupported:
    if (value == kAsioResetRequest
      || value == kAsioEngineVersion
      || value == kAsioResyncRequest
      || value == kAsioLatenciesChanged
      || value == kAsioSupportsInputMonitor)
      return 1;

  case kAsioResetRequest:
    nuiAudioDevice_ASIO::gpCurrentDevice->Reset();
    return 1;

  case kAsioEngineVersion:
    return 2;

  case kAsioResyncRequest:
    nuiAudioDevice_ASIO::gpCurrentDevice->Reset();
    return 1;

  case kAsioLatenciesChanged:
    return 1;

  default:
    return 0;
  }
}

ASIOTime* nuiAudioDevice_ASIO::BufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess)
{
  BufferSwitchCallback(doubleBufferIndex, directProcess);
  return NULL;
}

AudioDeviceAPI_ASIO::AudioDeviceAPI_ASIO()
: mpAsioDrivers(new AsioDrivers()), mCurrentDeviceIndex(-1)
{
  mName = API_NAME;
  RegisterWithManager();
}

AudioDeviceAPI_ASIO::~AudioDeviceAPI_ASIO()
{
}

uint32 AudioDeviceAPI_ASIO::GetDeviceCount() const
{
  return mpAsioDrivers->asioGetNumDev();
}

nuiAudioDevice* AudioDeviceAPI_ASIO::GetDevice(uint32 index)
{
  if (mCurrentDeviceIndex > -1)
  {
    mpAsioDrivers->asioCloseDriver(mCurrentDeviceIndex);
    mCurrentDeviceIndex = -1;
  }
  nuiAudioDevice_ASIO::gpCurrentDevice = NULL;

  IASIO* pDriver;
  try
  {
    if(!mpAsioDrivers->asioOpenDriver(index, (void **) &pDriver))
    {
      mCurrentDeviceIndex = index;
      nuiAudioDevice_ASIO::gpCurrentDevice = new nuiAudioDevice_ASIO(pDriver);
      return nuiAudioDevice_ASIO::gpCurrentDevice;
    }
    else
    {
      if (mCurrentDeviceIndex > -1)
      {
        mpAsioDrivers->asioOpenDriver(mCurrentDeviceIndex, (void**) &pDriver);	// try restore
        nuiAudioDevice_ASIO::gpCurrentDevice = new nuiAudioDevice_ASIO(pDriver);
      }
      return NULL;
    }
  }
  catch (std::exception ex)
  {
    return NULL;
  }
  NGL_ASSERT(0);
}

nuiAudioDevice* AudioDeviceAPI_ASIO::GetDevice(const nglString& rDeviceName)
{
  char devname[64];

  for(long i = 0; i < mpAsioDrivers->asioGetNumDev(); i++)
  {
    mpAsioDrivers->asioGetDriverName(i, devname, 32);
    nglString devstr(devname, eUTF8);
    if(rDeviceName == devname)
    {
      return GetDevice(i);
    }
  }
  NGL_ASSERT(0);
}

nglString AudioDeviceAPI_ASIO::GetDeviceName(uint32 index) const
{
  char devname[64];
  mpAsioDrivers->asioGetDriverName(index, devname, 32);
  return nglString(devname, eUTF8);
}

nuiAudioDevice* AudioDeviceAPI_ASIO::GetDefaultInputDevice()
{
  return GetDevice(0);
}

nuiAudioDevice* AudioDeviceAPI_ASIO::GetDefaultOutputDevice()
{
  return GetDevice(0);
}

ASIOCallbacks nuiAudioDevice_ASIO::gCallbacks = {&nuiAudioDevice_ASIO::BufferSwitchCallback, &nuiAudioDevice_ASIO::SampleRateDidChange, &nuiAudioDevice_ASIO::AsioMessage, &nuiAudioDevice_ASIO::BufferSwitchTimeInfo};
nuiAudioDevice_ASIO* nuiAudioDevice_ASIO::gpCurrentDevice = NULL;

AudioDeviceAPI_ASIO AsioApi;

const static float Int16ToFloat = 1.0 / 32768.0f;
const static float Int32ToFloat = 1.0 / 2147483648.0f;

const static float B0Int24ToFloat = 1.0/128.0f;
const static float B1Int24ToFloat = 1.0/32768.0f;
const static float B2Int24ToFloat = 1.0 / 8388608.0f;

//conversion functions
void nuiAudioDevice_ASIO::ConvertInt16MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  int16* pTmpSrc = (int16*)alloca(bufSize*2);
  memcpy(pTmpSrc, pSrc, bufSize*2);
  bswap_16_s((uint16*)pTmpSrc, bufSize);
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = *(pTmpSrc++) * Int16ToFloat;
  }
}

void nuiAudioDevice_ASIO::ConvertInt24MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst) = *((int8*)(pSrc++)) * B0Int24ToFloat;
    *(pDst) += *(pSrc++) * B1Int24ToFloat;
    *(pDst++) += *(pSrc++) * B2Int24ToFloat;
  }
}

void nuiAudioDevice_ASIO::ConvertInt32MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  int32* pTmpSrc = (int32*)alloca(bufSize*4);
  memcpy(pTmpSrc, pSrc, bufSize*4);
  bswap_32_s((uint32*)pTmpSrc, bufSize);
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = *(pTmpSrc++) * Int32ToFloat;
  }
}

void nuiAudioDevice_ASIO::ConvertFloat32MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  memcpy(pDst, pSrc, bufSize * 4);
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloat64MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  double* pTmpSrc = (double*)alloca(bufSize * 8);
  memcpy(pTmpSrc, pSrc, bufSize * 8);
  bswap_64_s((uint64*)pTmpSrc, bufSize);
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = (float)*(pTmpSrc++);
  }
}

void nuiAudioDevice_ASIO::ConvertInt16LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  const int16* pTmpSrc = (const int16*)pSrc;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = *(pTmpSrc++) * Int16ToFloat;
  }
}

void nuiAudioDevice_ASIO::ConvertInt24LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst) = *(pSrc++) * B2Int24ToFloat;
    *(pDst) += *(pSrc++) * B1Int24ToFloat;
    *(pDst++) += *((int8*)(pSrc++)) * B0Int24ToFloat;
  }
}

void nuiAudioDevice_ASIO::ConvertInt32LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  const int32* pTmpSrc = (const int32*)pSrc;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = *(pTmpSrc++) * Int32ToFloat;
  }
}

void nuiAudioDevice_ASIO::ConvertFloat32LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  memcpy(pDst, pSrc, bufSize * 4);
}

void nuiAudioDevice_ASIO::ConvertFloat64LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize)
{
  const double* pTmpSrc = (const double*)pSrc;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = (float)*(pTmpSrc++);
  }
}

const static double FloatToInt16 = (double) 0x7fff;
const static double FloatToInt24 = (double) 0x7fffff;
const static double FloatToInt32 = (double) 0x7fffffff;

const static double FloatToInt18 = (double) 0x01ff;
const static double FloatToInt20 = (double) 0x07ff;

void nuiAudioDevice_ASIO::ConvertFloatToInt16MSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int16* pTmpDst = (int16*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt16;
  }
  bswap_16_s((uint16*)pTmpDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToInt24MSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst) = bswap_32(fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt24);
    pDst += 3;
    pTmpDst = (int32*)pDst;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32MSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt32;
  }
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToFloat32MSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  memcpy(pDst, pSrc, bufSize * 4);
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToFloat64MSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  double* pTmpDst = (double*) pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = (double)*(pSrc++);
  }
  bswap_64_s((uint64*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32MSB16(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt16;
  }
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32MSB18(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt18;
  }
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32MSB20(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt20;
  }
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32MSB24(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt24;
  }
  bswap_32_s((uint32*)pDst, bufSize);
}

void nuiAudioDevice_ASIO::ConvertFloatToInt16LSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int16* pTmpDst = (int16*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt16;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt24LSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt24;
    pDst += 3;
    pTmpDst = (int32*)pDst;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32LSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt32;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToFloat32LSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  memcpy(pDst, pSrc, bufSize * 4);
}

void nuiAudioDevice_ASIO::ConvertFloatToFloat64LSB(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  double* pTmpDst = (double*) pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = (double)*(pSrc++);
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32LSB16(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt16;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32LSB18(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt18;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32LSB20(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt20;
  }
}

void nuiAudioDevice_ASIO::ConvertFloatToInt32LSB24(const float* pSrc, uint8* pDst, uint32 bufSize)
{
  int32* pTmpDst = (int32*)pDst;
  for (uint32 i = 0; i<bufSize; i++)
  {
    *(pTmpDst++) = fclamp(*(pSrc++), -1.0f, 1.0f) * FloatToInt24;
  }
}
