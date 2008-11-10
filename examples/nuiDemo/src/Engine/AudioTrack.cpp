/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Engine/AudioTrack.h"
#include "Application.h"

#include "nuiMessageBox.h"


AudioTrack::AudioTrack(nuiAudioFifo* pAudioFifo)
: nuiAudioTrack()
{
  mpAudioFifo = pAudioFifo;
  
  // load sound sample
  nglIStream* istream = nglPath(_T("rsrc:/sample.wav")).OpenRead();
  if (!istream || !istream->Available())
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error trying to load sound"), eMB_OK);
    pBox->QueryUser();
  }
  
  nuiWaveReader* pWaveReader = new nuiWaveReader(*istream);
  if (!pWaveReader->ReadInfo(mInfos))
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error, source is not a wav file"), eMB_OK);
    pBox->QueryUser();
  }

  float* pSamples = new float[mInfos.GetSampleFrames() * mInfos.GetChannels()];  
  uint32 nbReadSamples = pWaveReader->Read((void*)pSamples, mInfos.GetSampleFrames(), eSampleFloat32);  
  delete pWaveReader;
  
  // init local deinterlaced buffer
  mWavContents.resize(mInfos.GetChannels());
  for (uint32 c = 0; c < mInfos.GetChannels(); c++)
    mWavContents[c].resize(mInfos.GetSampleFrames());
  
  // init
  mSamplesBuffer.resize(mInfos.GetChannels());
  for (uint32 c = 0; c < mInfos.GetChannels(); c++)
    mSamplesBuffer[c].resize(4096);
  
  // deinterlace
  float* pInput;
  float* pOutput;
  for (uint c = 0; c < mInfos.GetChannels(); c++)
  {
      pInput = pSamples + c;
      pOutput = &(mWavContents[c][0]);
      for (uint32 s = 0; s < nbReadSamples; s++)
      {
        *(pOutput++) = *pInput;
        pInput += mInfos.GetChannels();
      }
  }  
  
  
  mStartSample = 0;
}

AudioTrack::~AudioTrack()
{
}

const std::vector<std::vector<float> >& AudioTrack::GetSamplesBuffer() const
{
  return mSamplesBuffer;
}


void AudioTrack::Start()
{
  uint32 nbChannels = 2;
  float volume = 1.0f;
  bool enableBuffering = false;
  
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
  uint32 nbSamplesToCopy = MIN(sampleFrames, mInfos.GetSampleFrames() - mStartSample);

  // give what you have to the audio system
  for (uint32 c = 0; c < rBuffer.size(); c++)
  {
    uint32 startSample = mStartSample;
    
    for (uint32 s = 0; s < nbSamplesToCopy; s++, startSample++)
      rBuffer[c][s] = mWavContents[c][startSample];
  }
  
  mStartSample += nbSamplesToCopy;
  
  // (nbSamplesToCopy < sampleFrames) <=> you couldn't copy as much samples as requested by the audio system
  // it happens for instance when the end of the sound sample has been reached.
  //
  // you may decide what you're gonna use to fill the rest of the buffer (in order not to get a crappy sound due to unknown buffer contents)
  // option 1: fill the rest of the buffer with 0.0f: to provide a clean silence
  // option 2: you want the sound to loop: go back to the sample's beginning and continue to copy...
  // here, we want the sound to loop.
  if (nbSamplesToCopy < sampleFrames)
  {
    mStartSample = 0;
    
    nbSamplesToCopy = MIN(sampleFrames, mInfos.GetSampleFrames() - mStartSample);
    
    // give what you have to the audio system
    for (uint32 c = 0; c < rBuffer.size(); c++)
    {
      uint32 startSample = mStartSample;
      
      for (uint32 s = 0; s < nbSamplesToCopy; s++, startSample++)
        rBuffer[c][s] = mWavContents[c][startSample];
    }
    
    mStartSample += nbSamplesToCopy;
  }
  
  //
  for (uint32 c = 0; c < mSamplesBuffer.size(); c++)
  {
    for (uint32 s = 0; s < sampleFrames; s++)
      mSamplesBuffer[c][s] = rBuffer[c][s];
  }
  
  
  return sampleFrames;
}

