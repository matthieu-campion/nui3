/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Engine/AudioTrack.h"
#include "Application.h"

#include "nuiMessageBox.h"
#include "nuiWaveReader.h"


AudioTrack::AudioTrack(nuiAudioFifo* pAudioFifo)
: nuiAudioTrack()
{
  mpAudioFifo = pAudioFifo;
  
  // load sound sample
  nglIFile ifile(_T("./sample.wav"));
  if (!ifile.IsOpen())
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error trying to load sound"), eMB_OK);
    pBox->QueryUser();
  }
  
  mpWaveReader = new nuiWaveReader(ifile);
  nuiSampleInfo infos;
  bool res = mpWaveReader->ReadInfo(infos); 
  mpSamples = new float[infos.GetSampleFrames() * infos.GetChannels()];  
  uint32 nbReadSamples = mpWaveReader->Read((void*)mpSamples, infos.GetSampleFrames(), eSampleFloat32);  
//  mpSample = new nuiSample(*mpWaveReader);
}

AudioTrack::~AudioTrack()
{
  delete mpWaveReader;
}



void AudioTrack::Start()
{
  uint32 nbChannels = 2;
  float volume = 1.0f;
  bool enableBuffering = true;
  
  if (!mpAudioFifo->RegisterTrack(this, mpAudioFifo->GetSampleRate(), nbChannels, volume, enableBuffering))
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error trying to play sound"), eMB_OK);
    pBox->QueryUser(); // make the message box modal. Once exiting the modal state, the box is auto-trashed, you don't have to do it yourself.
  }
}



void AudioTrack::Stop()
{
  mpAudioFifo->UnregisterTrack(this);
}


// virtual method from nuiAudioTrack. Have a look to nuiAudioFifo.h
uint32 AudioTrack::ReadSamples(uint32 sampleFrames, std::vector<float*>& rBuffer)
{
  uint32 nbReadSamples = 0;

  // do it
  
  return nbReadSamples;
}


// virtual method from nuiAudioTrack. Have a look to nuiAudioFifo.h
void AudioTrack::ProcessedSamples(uint32 sampleFrames, uint32 bufSize, uint32 bufPos)
{
  // do nothing for now...
}
