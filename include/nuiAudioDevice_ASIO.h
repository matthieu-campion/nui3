/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiAudioDevice.h"
#include "asiodrivers.h"
#include "asiosys.h"
#include "asio.h"
#include "iasiodrv.h"


typedef nuiFastDelegate3<const uint8*, float*, uint32> ConvertToFloatFunc;
typedef nuiFastDelegate3<const float*, uint8*, uint32> ConvertFromFloatFunc;

class nuiAudioDevice_ASIO : public nuiAudioDevice
{
  friend class AudioDeviceAPI_ASIO;
public:
  nuiAudioDevice_ASIO(IASIO* pDriver);
  virtual ~nuiAudioDevice_ASIO();

  virtual bool Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction);
  virtual bool Close();

private:
  IASIO* mpDriver;
  std::vector<ASIOSampleType> mInputSampleTypes;
  std::vector<ASIOSampleType> mOutputSampleTypes;

  std::vector<ASIOBufferInfo> mBufferInfos; // mBufferInfos[0]: first input buffer info | mBufferInfos[mNumInputs]: first output buffer info

  std::vector<float*> mOutputBuffers;
  std::vector<float*> mInputBuffers;

  uint32 mNumInputs;
  uint32 mNumOutputs;

  nuiAudioProcessFn Process;
  std::vector<ConvertToFloatFunc>   mInputConversionFuncs;
  std::vector<ConvertFromFloatFunc> mOutputConversionFuncs;

  static ASIOCallbacks gCallbacks;
  static nuiAudioDevice_ASIO* gpCurrentDevice;

  uint32 mBufferSize;
  double mSampleRate;

  void BufferSwitch(long doubleBufferIndex, ASIOBool directProcess);
  void Reset();

  // Callbacks
  static void BufferSwitchCallback(long doubleBufferIndex, ASIOBool directProcess);
  static void SampleRateDidChange(ASIOSampleRate sRate);
  static long AsioMessage (long selector, long value, void* message, double* opt);
  static ASIOTime* BufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);

  //conversion functions
  inline static void ConvertInt16MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertInt24MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertInt32MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertFloat32MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertFloat64MSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertInt16LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertInt24LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertInt32LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertFloat32LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);
  inline static void ConvertFloat64LSBToFloat(const uint8* pSrc, float* pDst, uint32 bufSize);

  inline static void ConvertFloatToInt16MSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt24MSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32MSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToFloat32MSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToFloat64MSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32MSB16(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32MSB18(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32MSB20(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32MSB24(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt16LSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt24LSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32LSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToFloat32LSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToFloat64LSB(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32LSB16(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32LSB18(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32LSB20(const float* pSrc, uint8* pDst, uint32 bufSize);
  inline static void ConvertFloatToInt32LSB24(const float* pSrc, uint8* pDst, uint32 bufSize);
};


class AudioDeviceAPI_ASIO : public nuiAudioDeviceAPI
{
public:
  AudioDeviceAPI_ASIO();

  virtual ~AudioDeviceAPI_ASIO();

  virtual uint32 GetDeviceCount() const;

  virtual nuiAudioDevice* GetDevice(uint32 index);
  virtual nuiAudioDevice* GetDevice(const nglString& rDeviceName);
  virtual nglString GetDeviceName(uint32 index) const;
  virtual nuiAudioDevice* GetDefaultInputDevice();
  virtual nuiAudioDevice* GetDefaultOutputDevice();

protected:
  AsioDriverList* mpAsioDrivers;
  int32 mCurrentDeviceIndex;
};

extern nuiAudioDeviceAPI_ASIO nuiAsioApi;
