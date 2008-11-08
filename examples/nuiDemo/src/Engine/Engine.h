/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiAudioFifo.h"

#include "Engine/AudioTrack.h"


class Engine
{
public:
  
  Engine();
  ~Engine();
  
  AudioTrack* GetAudioTrack();
  
private:
  
  nuiAudioFifo* InitEngineAudio();
  
  nuiAudioFifo* mpAudioFifo;
  AudioTrack* mpAudioTrack;
};