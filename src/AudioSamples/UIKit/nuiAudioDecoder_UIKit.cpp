/*
 *  nuiAudioDevoder_UIKit.cpp
 *  nui3
 *
 *  Created by mat on 8/17/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "nuiAudioDecoder.h"

#include <AudioToolBox/AudioToolBox.h>
#include "AudioToolBox/ExtendedAudioFile.h"

OSStatus MyAudioFile_ReadProc(void* pInClientData, SInt64 inPosition, UInt32 requestCount, void* pBuffer, UInt32* pActualCount)
{	
	nglIStream* pStream = (nglIStream*)pInClientData;
	
	pStream->SetPos(inPosition);
	
	uint8* pOut = (uint8*)pBuffer;
	*pActualCount = pStream->ReadUInt8(pOut, requestCount);
	
	return noErr;
}

OSStatus MyAudioFile_WriteProc (void* pInClientData, SInt64 inPosition,	UInt32 requestCount, const void* pBuffer,	UInt32* pActualCount)
{
	NGL_ASSERT(0);
	*pActualCount = 0;
	return noErr;
}

SInt64 MyAudioFile_GetSizeProc(void* pInClientData)
{
	nglIStream* pStream = (nglIStream*)pInClientData;
	
	pStream->SetPos(0);
	nglFileSize size = pStream->Available();
	return size;
}

OSStatus MyAudioFile_SetSizeProc(void* pInClientData, SInt64 inSize)
{
	NGL_ASSERT(0);
	return noErr;
}


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
	OSStatus err =  AudioFileOpenWithCallbacks(&mrStream, &MyAudioFile_ReadProc, &MyAudioFile_WriteProc, &MyAudioFile_GetSizeProc, &MyAudioFile_SetSizeProc, typeID, &mAudioFileID);
	
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
  NGL_ASSERT(mpPrivate);
  
  AudioStreamBasicDescription desc;
  UInt32 PropDataSize;
  Boolean PropWritable;
  
  OSStatus res = ExtAudioFileGetPropertyInfo(mpPrivate->mExtAudioFileRef, kExtAudioFileProperty_FileDataFormat, &PropDataSize, &PropWritable);
  
  res = ExtAudioFileGetProperty(mpPrivate->mExtAudioFileRef, kExtAudioFileProperty_FileDataFormat, &PropDataSize, &desc);
  if (res != noErr)
    return false;
  
  // todo...
  
  return true;
}

bool nuiAudioDecoder::Seek(uint64 SampleFrame)
{
  return true;
}

uint32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format)
{
  return 0;
}