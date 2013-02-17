/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#include <AvailabilityMacros.h>

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
#include <AudioToolbox/AudioToolbox.h>
#else
#include <AudioToolbox.h>
#endif


OSStatus MyAudioFile_ReadProc(void* pInClientData, SInt64 inPosition, UInt32 requestCount, void* pBuffer, UInt32* pActualCount)
{	
	nglIStream* pStream = (nglIStream*)pInClientData;
	pStream->SetPos(inPosition);
  
	uint8* pOut = (uint8*)pBuffer;
	*pActualCount = pStream->ReadUInt8(pOut, requestCount);
	
	return noErr;
}

SInt64 MyAudioFile_GetSizeProc(void* pInClientData)
{
	nglIStream* pStream = (nglIStream*)pInClientData;
	
	pStream->SetPos(0);
	nglFileSize size = pStream->Available();
	return size;
}


//
//
// nuiAudioDecoderPrivate
//
//

class nuiAudioDecoderPrivate
  {
  public:
    nuiAudioDecoderPrivate(nglIStream& rStream);
    virtual ~nuiAudioDecoderPrivate();
		
		bool Init();
		void Clear();
    
    nglIStream& mrStream;
		
		AudioFileID mAudioFileID;
		ExtAudioFileRef mExtAudioFileRef;
  };

nuiAudioDecoderPrivate::nuiAudioDecoderPrivate(nglIStream& rStream)
: mrStream(rStream),
	mAudioFileID(NULL),
	mExtAudioFileRef(NULL)
{
}

nuiAudioDecoderPrivate::~nuiAudioDecoderPrivate()
{
	Clear();
}


bool nuiAudioDecoderPrivate::Init()
{
	AudioFileTypeID typeID = 0;

  // we only want to read (not write) so give NULL for write callbacks (seems to work...)
  OSStatus err =  AudioFileOpenWithCallbacks(&mrStream, &MyAudioFile_ReadProc, NULL, &MyAudioFile_GetSizeProc, NULL, typeID, &mAudioFileID);
	
	if (err != noErr)
		return false;
	
	err = ExtAudioFileWrapAudioFileID(mAudioFileID, false /*not for writing*/, &mExtAudioFileRef);
	
	return (err == noErr);
}

void nuiAudioDecoderPrivate::Clear()
{
	if (mAudioFileID)
		AudioFileClose(mAudioFileID);
}




//
//
// nuiAudioDecoder
//
//

void nuiAudioDecoder::Clear()
{
  if (mpPrivate)
    delete mpPrivate;
}

bool nuiAudioDecoder::CreateAudioDecoderPrivate()
{
  mpPrivate = new nuiAudioDecoderPrivate(mrStream);
  bool res = mpPrivate->Init();
  
  if (!res)
  {
    delete mpPrivate;
    mpPrivate = NULL;
  }
  return res;
}

bool nuiAudioDecoder::ReadInfo()
{
  if (!mpPrivate)
    return false;
  
  AudioStreamBasicDescription FileDesc;
  AudioStreamBasicDescription ClientDesc;
  UInt32 PropDataSize;
  OSStatus res;
  
  
  // #FIXME: the value given by this function is sometimes wrong:
  // with some m4a files and with Mac OS 10.5, SampleFrames will be 16x too big
  
  // get length
  SInt64 SampleFrames = 0;  
  PropDataSize = sizeof(SampleFrames);
  res = ExtAudioFileGetProperty(mpPrivate->mExtAudioFileRef, kExtAudioFileProperty_FileLengthFrames, &PropDataSize, &SampleFrames); 
  if (res != noErr)
    return false;
  
  PropDataSize = sizeof(FileDesc);
  res = ExtAudioFileGetProperty(mpPrivate->mExtAudioFileRef, kExtAudioFileProperty_FileDataFormat, &PropDataSize, &FileDesc);
  if (res != noErr)
    return false;
  
  double SampleRate = FileDesc.mSampleRate;
  int32 channels = FileDesc.mChannelsPerFrame;
  int32 BitsPerSample = 32;
  ClientDesc.mSampleRate = SampleRate;
  ClientDesc.mChannelsPerFrame = channels;
  ClientDesc.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved;
  ClientDesc.mFormatID = kAudioFormatLinearPCM;
  ClientDesc.mFramesPerPacket = 1;
  ClientDesc.mBitsPerChannel = BitsPerSample;
  ClientDesc.mBytesPerFrame = BitsPerSample / 8;
  ClientDesc.mBytesPerPacket = BitsPerSample / 8;

  PropDataSize = sizeof(ClientDesc);  
  res = ExtAudioFileSetProperty(mpPrivate->mExtAudioFileRef, kExtAudioFileProperty_ClientDataFormat, PropDataSize, &ClientDesc);
  if (res != noErr)
    return false;
  
  
  
  mInfo.SetSampleFrames(SampleFrames);
  mInfo.SetSampleRate(SampleRate);
  mInfo.SetChannels(channels);
  mInfo.SetBitsPerSample(BitsPerSample);
  mInfo.SetFileFormat(eAudioCompressed);
  mInfo.SetStartFrame(0);
  mInfo.SetStopFrame(mInfo.GetSampleFrames());
  
  return true;
}

bool nuiAudioDecoder::Seek(int64 SampleFrame)
{
  OSStatus err = ExtAudioFileSeek(mpPrivate->mExtAudioFileRef, SampleFrame);
  return (err == noErr);
}

int32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, int32 sampleframes, nuiSampleBitFormat format)
{
  if (!mInitialized)
    return 0;
  
  SetPosition(mPosition);
  
  int32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  sampleframes = MIN(sampleframes, length - mPosition);
  
  int32 channels = mInfo.GetChannels();
  if (buffers.size() != channels)
    return 0;
  
  std::vector<float*> temp(channels);
  for (int32 c = 0; c < channels; c++)
  {
    if (format == eSampleFloat32)
      temp[c] = (float*)(buffers[c]);
    else
      temp[c] = new float[sampleframes];
  }
  
  //
  int64 BytesToRead = SampleFramesToBytes(sampleframes);
  int32 listSize = sizeof(AudioBufferList) + sizeof(AudioBuffer)* (channels-1);
  AudioBufferList* pBufList = reinterpret_cast<AudioBufferList*> (new Byte[listSize]);
  pBufList->mNumberBuffers = channels; // we query non-interleaved samples, so we need as many buffers as channels
  for (int32 c = 0; c < pBufList->mNumberBuffers; c++)
  {
    // each AudioBuffer represents one channel (non-interleaved samples)
    pBufList->mBuffers[c].mNumberChannels   = 1;
    pBufList->mBuffers[c].mDataByteSize     = BytesToRead / channels;
    pBufList->mBuffers[c].mData             = temp[c];
  }
  
  //
  UInt32 frames = sampleframes;
  OSStatus err = ExtAudioFileRead(mpPrivate->mExtAudioFileRef, &frames, pBufList);
  if (err != noErr)
    frames = 0;
  
  if (format == eSampleInt16)
  {
    for (int32 c = 0; c < channels; c++)
    {
      if (err != noErr)
      {
        nuiAudioConvert_FloatBufferTo16bits(temp[c], (int16*)(buffers[c]), frames);
      }
      delete[] temp[c];
    }
  }
  
  delete pBufList;
  
  mPosition += frames;
  return frames;
}


int32 nuiAudioDecoder::ReadIN(void* pBuffer, int32 sampleframes, nuiSampleBitFormat format)
{
  //don't increment mPosition: it's already done in ReadDE
  int32 channels = mInfo.GetChannels();
  
  int32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  sampleframes = MIN(sampleframes, length - mPosition);
  
  std::vector<float*> temp(channels);
  std::vector<void*> tempVoid(channels);
  for (int32 c= 0; c < channels; c++)
  {
    temp[c] = new float[sampleframes];
    tempVoid[c] = (void*)(temp[c]);
  }
  
  int32 sampleFramesRead = ReadDE(tempVoid, sampleframes, eSampleFloat32);
  if (format == eSampleFloat32)
  {
    float* pFloatBuffer = (float*)pBuffer;
    //just interleave samples
    for (int32 c = 0; c < channels; c++)
    {
      for (int32 s = 0; s < sampleFramesRead; s++)
      {
        pFloatBuffer[s * channels + c] = temp[c][s];
      }
    }
  }
  else
  {
    //16 bits int are required, so interleave samples and convert them into float
    int16* pInt16Buffer = (int16*)pBuffer;
    for (int32 c = 0; c < channels; c++)
    {
      nuiAudioConvert_DEfloatToINint16(temp[c], pInt16Buffer, c, channels, sampleFramesRead);
    }
    
  }
  
  for (int32 c= 0; c < channels; c++)
  {
    delete[] temp[c];
  }
  
  return sampleFramesRead;
}