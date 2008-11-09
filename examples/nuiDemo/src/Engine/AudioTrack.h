/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiAudioFifo.h"
#include "nuiWaveReader.h"
#include "nuiSample.h"

class AudioTrack : public nuiAudioTrack
{
public:
  AudioTrack(nuiAudioFifo* pAudioFifo);
  ~AudioTrack();
  
  void Start();
  void Stop();
  
protected:
  
	// virtual method from nuiAudioTrack. Have a look to nuiAudioFifo.h
  virtual uint32 ReadSamples(uint32 sampleFrames, std::vector<float*>& rBuffer);
  
  
	// virtual method from nuiAudioTrack. Have a look to nuiAudioFifo.h
	virtual	void	ProcessedSamples(uint32 sampleFrames, uint32 bufSize, uint32 bufPos);
  
private:
  
  nuiAudioFifo* mpAudioFifo;
  nuiWaveReader* mpWaveReader;
//  nuiSample* mpSample;
  float* mpSamples;
};