/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiAudioFifo.h"
#include "nuiWaveReader.h"

class AudioTrack : public nuiAudioTrack
{
public:
  AudioTrack(nuiAudioFifo* pAudioFifo);
  ~AudioTrack();
  
  void Start();
  void Stop();
  
  const std::vector<std::vector<float> >& GetSamplesBuffer() const;
  
protected:
  
	// virtual method from nuiAudioTrack. Have a look to nuiAudioFifo.h
  virtual uint32 ReadSamples(uint32 sampleFrames, std::vector<float*>& rBuffer);
  
private:
  
  nuiAudioFifo* mpAudioFifo;
  std::vector<std::vector<float> > mWavContents;
  std::vector<std::vector<float> > mSamplesBuffer;
  
  nuiSampleInfo mInfos;
  uint32 mStartSample;
};