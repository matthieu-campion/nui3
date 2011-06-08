/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiAudioDecoder.h"
#include "nuiAudioConvert.h"



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
  };

nuiAudioDecoderPrivate::nuiAudioDecoderPrivate(nglIStream& rStream)
: mrStream(rStream)
{
}

nuiAudioDecoderPrivate::~nuiAudioDecoderPrivate()
{
	Clear();
}


bool nuiAudioDecoderPrivate::Init()
{
	return false;
}

void nuiAudioDecoderPrivate::Clear()
{
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
  
    return false;
}

bool nuiAudioDecoder::Seek(uint64 SampleFrame)
{
  return false;
}

uint32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format)
{
  if (!mInitialized)
    return 0;
  
  SetPosition((uint32)mPosition);
  
  uint32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  sampleframes = MIN(sampleframes, length - mPosition);
  
  uint32 channels = mInfo.GetChannels();
  if (buffers.size() != channels)
    return 0;
  
  mPosition += 0;
  return 0;
}