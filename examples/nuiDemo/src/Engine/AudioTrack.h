/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiAudioFifo.h"

class AudioTrack : public nuiAudioTrack
{
public:
  AudioTrack(nuiAudioFifo* pAudioFifo);
  ~AudioTrack();
  
  void Start();
private:
  
  nuiAudioFifo* mpAudioFifo;
};