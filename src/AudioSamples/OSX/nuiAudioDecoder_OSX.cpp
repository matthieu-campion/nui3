/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiAudioDecoder.h"
#include "nuiAudioConvert.h"

#include <AudioToolBox/AudioToolBox.h>
#include "AudioToolBox/ExtendedAudioFile.h"

OSStatus MyAudioFile_ReadProc(void* pInClientData, SInt64 inPosition, UInt32 requestCount, void* pBuffer, UInt32* pActualCount)
{	
	nglIStream* pStream = (nglIStream*)pInClientData;
  
  uint32 avail = pStream->Available();
	pStream->SetPos(inPosition);
	
  avail = pStream->Available();
  
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
  uint32 channels = FileDesc.mChannelsPerFrame;
  uint32 BitsPerSample = 32;
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

bool nuiAudioDecoder::Seek(uint64 SampleFrame)
{
  OSStatus err = ExtAudioFileSeek(mpPrivate->mExtAudioFileRef, SampleFrame);
  return (err == noErr);
}

uint32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format)
{
  if (!mInitialized)
    return 0;
  
  uint32 channels = mInfo.GetChannels();
  if (buffers.size() != channels)
    return 0;
  
  std::vector<float*> temp(channels);
  for (uint32 c = 0; c < channels; c++)
  {
    if (format == eSampleFloat32)
      temp[c] = (float*)(buffers[c]);
    else
      temp[c] = new float[sampleframes];
  }
  
  //
  uint64 BytesToRead = SampleFramesToBytes(sampleframes);
  uint32 listSize = sizeof(AudioBufferList) + sizeof(AudioBuffer)* (channels-1);
  AudioBufferList* pBufList = reinterpret_cast<AudioBufferList*> (new Byte[listSize]);
  pBufList->mNumberBuffers = channels; // we query non-interleaved samples, so we need as many buffers as channels
  for (uint32 c = 0; c < pBufList->mNumberBuffers; c++)
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
    for (uint32 c = 0; c < channels; c++)
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