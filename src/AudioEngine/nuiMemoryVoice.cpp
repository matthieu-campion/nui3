/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiMemoryVoice.h"

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
}

bool nuiMemoryVoice::IsValid() const
{
  return (mpMemorySound != NULL);
}

uint64 nuiMemoryVoice::GetSampleFrames() const
{
  if (!IsValid())
    return 0;
  
  return mpMemorySound->GetSampleFrames();
}

uint32 nuiMemoryVoice::GetChannels() const
{
  if (!IsValid())
    return 0;
  
  return mpMemorySound->GetChannels();
}

uint32 nuiMemoryVoice::ReadSamples(const std::vector<float*>& rOutput, int64 position, uint32 SampleFrames)
{
  if (!IsValid())
    return 0;
  
  uint32 read = mpMemorySound->ReadSamples(rOutput, position, SampleFrames);
  return read;
}

bool nuiMemoryVoice::Init()
{
  return true;
}