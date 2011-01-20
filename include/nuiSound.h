/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSampleReader.h"

class nuiVoice;

class nuiSound : public nuiRefCount
{
public:
  friend class nuiSoundManager;
  
  enum Type 
  {
    eStream = 0,
    eMemory,
    eSynth,
    eUnknown
  };
  
  nuiVoice* GetVoice();
  
  float GetGainDb() const;
  void SetGainDb(float Db);
  float GetGain() const;
  void SetGain(float gain);
  
  float GetPan() const;
  void SetPan(float pan);
  
  void SetLoop(bool loop);
  bool IsLooping();
  
protected:
  nuiSound();
  virtual ~nuiSound();
  
  virtual nuiVoice* GetVoiceInternal() = 0;
  
  Type mType;  
  
  float mGain;
  float mPan;
  bool mLoop;
};

