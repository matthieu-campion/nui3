/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiAudioDevice.h"
#include "nglThread.h"
#include "nuiAudioDevice_AudioUnit.h"
#include "nuiAudioConvert.h"

//class nuiAudioDevice_AudioUnit : public nuiAudioDevice
nuiAudioDevice_AudioUnit::nuiAudioDevice_AudioUnit()
{  
  EnumSampleRates();
  EnumBufferSizes();
}

nuiAudioDevice_AudioUnit::~nuiAudioDevice_AudioUnit()
{
	AudioOutputUnitStop(mAudioUnit);
	AudioComponentInstanceDispose(mAudioUnit);  
}


/*
 * The main audio i/o callback.  This number of frames is currently fixed at 1024, but Bill Stewart
 * tells me they're working on a way to let us specify smaller buffer sizes.
 *
 * I should point out that there's something wrong with my dsp somewhere.  This code works on
 * my iPhone, but there are discontinuities in the generated sine that I haven't figure out yet
 * Could be my fixed-point sine table, or maybe I'm doing something wrong with my 16.16
 * phase accumulator.
 */



OSStatus AudioUnitCallback(void* inRefCon, 
                    AudioUnitRenderActionFlags* ioActionFlags,
                    const AudioTimeStamp* inTimeStamp,
                    UInt32 inBusNumber,
                    UInt32 inNumberFrames_,
                    AudioBufferList* ioData)
{
  // Return if not pre-render
	if(!(*ioActionFlags & kAudioUnitRenderAction_PreRender))
  {
    return noErr;
  }
  
	// Get a pointer to the audio device
	nuiAudioDevice_AudioUnit* pAudioDevice = (nuiAudioDevice_AudioUnit*)inRefCon;
  
	// Process
	pAudioDevice->Process(inNumberFrames_, ioData);
	
	return noErr;
} 



void nuiAudioDevice_AudioUnit::Process(uint uNumFrames, AudioBufferList* ioData)
{
  //NGL_OUT(_T("nuiAudioDevice_AudioUnit::Process uNumFrames %d    %d %d\n"),uNumFrames, ioData->mBuffers[0].mNumberChannels, ioData->mBuffers[1].mNumberChannels );
  
  mAudioProcessFn(mInputBuffers, mOutputBuffers, uNumFrames);

  const float* ptr0 = mOutputBuffers[0];
  const float* ptr1 = mOutputBuffers[1];

  // copy buffers (int -> float)
  if (ioData->mNumberBuffers == 2)
  {
    int32* dst0 = (int32*)ioData->mBuffers[0].mData;
    int32* dst1 = (int32*)ioData->mBuffers[1].mData;
    
    const int32 mult = ((1 << 24) - 1);
    for (uint32 s = 0; s < uNumFrames; s++)
    {
      const float sl = *ptr0;
      const float sr = *ptr1;
      
      *dst0 = sl * mult;
      *dst1 = sr * mult;
      
      dst0++;
      dst1++;
      
      ptr0++;
      ptr1++;
    }
  }
  else
  {
    int16* dst0 = (int16*)ioData->mBuffers[0].mData;
    
    const int32 mult = ((1 << 16) - 1);
    for (uint32 s = 0; s < uNumFrames; s++)
    {
      const float sl = *ptr0;
      const float sr = *ptr1;
      
      *dst0 = sl * mult;
      dst0++;
      *dst0 = sr * mult;
      dst0++;
      
      
      ptr0++;
      ptr1++;
    }
  }
} 



void interruptionListener( void* inClientData, UInt32 inInterruptionState)
{
  
}		


bool nuiAudioDevice_AudioUnit::Open(std::vector<uint32>& rInputChannels, std::vector<uint32>& rOutputChannels, double SampleRate, uint32 BufferSize, nuiAudioProcessFn pProcessFunction)
{
  OSStatus err;
	
  mAudioProcessFn = pProcessFunction;
  mSampleRate = SampleRate;
  mBufferSize = BufferSize;

  
  mActiveInputChannels.resize(rInputChannels.size());
  mActiveOutputChannels.resize(rOutputChannels.size());
      
  mOutputBuffers.resize(mActiveOutputChannels.size(), NULL);
  // init buffers
  for (uint32 i = 0; i < mOutputBuffers.size(); i++)
    mOutputBuffers[i] = (float*)malloc(mBufferSize * sizeof(float));
  
  UInt32 size = sizeof (UInt32);
  UInt32 value = kAudioSessionOverrideAudioRoute_None;
  AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, size, &value);  
  
	// Initialize our session
	err = AudioSessionInitialize(NULL, NULL, interruptionListener, NULL);	
  
  
	// Set the category
	UInt32 uCategory = kAudioSessionCategory_LiveAudio;
	err = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(UInt32), &uCategory);
//	if (err != noErr)
//  {
//    NGL_ASSERT(0);
//    return false;
//  } 		
	
	// Set the buffer size
	Float32 fBufferSize = (float)mBufferSize / mSampleRate;	//256frames @ 44.1khz
	err = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(Float32), &fBufferSize);
//	if (err != noErr)
//  {
//    NGL_ASSERT(0);
//    return false;
//  }	
	
	// Set active
	err = AudioSessionSetActive(true);
//	if (err != noErr)
//  {
//    NGL_ASSERT(0);
//    return false;
//  }
	
	// Initialize the audio unit
	mAudioUnit = 0;
	
	AudioComponentDescription desc;	
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentType = kAudioUnitType_Output;
	
	//RemoteIO is equivalent to AUHAL on OS X 
	desc.componentSubType = kAudioUnitSubType_RemoteIO;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
	
	//@see 'Audio Component Services Reference'
	UInt32 count = AudioComponentCount(&desc);
	// Make sure we got RemoteIO
	if (1 != count)
  {
    NGL_ASSERT(0);
    return false;
  }
	
	AudioComponent comp = 0;
	comp = AudioComponentFindNext(comp, &desc);
	
	// Make sure we found RemoteIO
	if (!comp)
  {
    NGL_ASSERT(0);
    return false;
  }
	
	
	err = AudioComponentInstanceNew(comp, &mAudioUnit);
	// Make sure we instantiated
	if (noErr != err)
  {
    NGL_ASSERT(0);
    return false;
  }
	
	
	//find out about the device's format (but I can save you the suspense:
	//integer | non-interleaved | packed | little endian | 32 bits (8.24 fixed)
	AudioStreamBasicDescription out_fmt_desc;
	size = sizeof(out_fmt_desc);
	
	err = AudioUnitGetProperty(mAudioUnit, 
                             kAudioUnitProperty_StreamFormat, 
                             kAudioUnitScope_Output, 
                             0, &out_fmt_desc, &size);
	if( err != noErr )
  {
    NGL_ASSERT(0);
    return false;
  }
	

	// same for input device:
	AudioStreamBasicDescription in_fmt_desc;
	size = sizeof(in_fmt_desc);
	
	err = AudioUnitGetProperty(mAudioUnit, 
                             kAudioUnitProperty_StreamFormat, 
                             kAudioUnitScope_Input, 
                             0, &in_fmt_desc, &size);
	if( err != noErr )
  {
    NGL_ASSERT(0);
    return false;
  }
	
	
	
	//NSAssert (noErr == err, @"Retrieving ASBD failed");
	
	//
	// These new flags in CoreAudioTypes.h tell us how many fractional bits the fixed-point format
	// uses.  In the current iPhone OS devices, it's always 24
	
	//int number_fractional_bits = (fmt_desc.mFormatFlags & kLinearPCMFormatFlagsSampleFractionMask) >> kLinearPCMFormatFlagsSampleFractionShift;	
	err = AudioUnitInitialize(mAudioUnit);
	if (err != noErr)
  {
    NGL_ASSERT(0);
    return false;
  }	
	
	//NSAssert (noErr == err, @"AU Init Failed");
	err = AudioUnitAddRenderNotify(mAudioUnit, AudioUnitCallback, this);
	if (err != noErr)
  {
    NGL_ASSERT(0);
    return false;
  }	
	//NSAssert (noErr == err, @"Add Render Callback failed");
	
	
	err = AudioOutputUnitStart(mAudioUnit);
	if (err != noErr)
  {
    NGL_ASSERT(0);
    return false;
  }	
	//NSAssert (noErr == err, @"AU Start failed");
	
	return true;
}






bool nuiAudioDevice_AudioUnit::Close()
{
	
	AudioUnitUninitialize(mAudioUnit);
  return true;
}
    
    
void nuiAudioDevice_AudioUnit::EnumSampleRates()
{
}

void nuiAudioDevice_AudioUnit::EnumBufferSizes()
{
}

nglString nuiAudioDevice_AudioUnit::GetChannelName(bool IsInput, uint32 index) const
{
  if (IsInput || index > 0)
    return nglString::Null;
  
  return _T("Output");
}










//class nuiAudioDeviceAPI_AudioUnit : public nuiAudioDeviceAPI
nuiAudioDeviceAPI_AudioUnit::nuiAudioDeviceAPI_AudioUnit()
{
  mName = _T("AudioUnit");
}

nuiAudioDeviceAPI_AudioUnit::~nuiAudioDeviceAPI_AudioUnit()
{
}


uint32 nuiAudioDeviceAPI_AudioUnit::GetDeviceCount() const
{
  return 1;
}


nuiAudioDevice* nuiAudioDeviceAPI_AudioUnit::GetDevice(uint32 index)
{
  if (index == 0)
    return new nuiAudioDevice_AudioUnit();
  return NULL;
}

nuiAudioDevice* nuiAudioDeviceAPI_AudioUnit::GetDevice(const nglString& rDeviceName)
{
  if (index == 0)
    return new nuiAudioDevice_AudioUnit();
  return NULL;
}

nglString nuiAudioDeviceAPI_AudioUnit::GetDeviceName(uint32 index) const
{
  if (index == 0)
    return nglString(_T("Output"));
  return nglString::Null;
}

nuiAudioDevice* nuiAudioDeviceAPI_AudioUnit::GetDefaultOutputDevice()
{
  return GetDevice(0);
}

nuiAudioDevice* nuiAudioDeviceAPI_AudioUnit::GetDefaultInputDevice()
{
  return GetDevice(0);
}


nuiAudioDeviceAPI_AudioUnit AudioUnitAPI;
