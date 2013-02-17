/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiMemoryVoice::nuiMemoryVoice(nuiMemorySound* pSound)
: nuiVoice(pSound),
  mpMemorySound(pSound)
{
  
}

nuiMemoryVoice::~nuiMemoryVoice()
{
  
}

nuiMemoryVoice::nuiMemoryVoice(const nuiMemoryVoice& rVoice)
: mpMemorySound(NULL)
{
  *this = rVoice;
}

nuiMemoryVoice& nuiMemoryVoice::operator=(const nuiMemoryVoice& rVoice)
{
  this->nuiVoice::operator=(rVoice);
  
  mpMemorySound = rVoice.mpMemorySound;
  return *this;
}

bool nuiMemoryVoice::IsValid() const
{
  return (mpMemorySound != NULL);
}

int32 nuiMemoryVoice::GetChannels() const
{
  if (!IsValid())
    return 0;
  
  return mpMemorySound->GetChannels();
}

int32 nuiMemoryVoice::GetSampleFrames() const
{
  if (!IsValid())
    return 0;
  
  return mpMemorySound->GetSampleFrames();
}

int32 nuiMemoryVoice::ReadSamples(const std::vector<float*>& rOutput, int64 position, int32 SampleFrames)
{
  if (!IsValid())
    return 0;
  
  if (position > mpMemorySound->GetSampleFrames())
    return 0;
  
  int32 todo = MIN(SampleFrames, mpMemorySound->GetSampleFrames() - position);
  int32 read = mpMemorySound->ReadSamples(rOutput, position, todo);
  return read;
}

bool nuiMemoryVoice::Init()
{
  return true;
}