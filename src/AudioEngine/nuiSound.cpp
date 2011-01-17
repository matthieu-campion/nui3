/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSound.h"
#include "nuiAudioDb.h"

nuiSound::nuiSound()
: mType(eUnknown),
  mGain(1),
  mPan(0),
  mLoop(false)
{
}
  
nuiSound::~nuiSound()
{
  nuiSoundManager::Instance.RemoveSound(this);
}

nuiVoice* nuiSound::GetVoice()
{
  nuiVoice* pVoice = GetVoiceInternal();
  pVoice->SetGain(mGain);
  pVoice->SetPan(mPan);
  pVoice->SetLoop(mLoop);
  
  return pVoice;
}


float nuiSound::GetGainDb() const
{
  float Db = GainToDB(GetGain());
  return Db;
}

void nuiSound::SetGainDb(float Db)
{
  SetGain(DBToGain(Db));
}

float nuiSound::GetGain() const
{
  return mGain;
}

void nuiSound::SetGain(float gain)
{
  mGain = gain;
}

float nuiSound::GetPan() const
{
  return mPan;
}

void nuiSound::SetPan(float pan)
{
  mPan = pan;
}

void nuiSound::SetLoop(bool loop)
{
  mLoop = loop;
}

bool nuiSound::IsLooping()
{
  return mLoop;
}