/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Engine/AudioTrack.h"
#include "Application.h"

#include "nuiMessageBox.h"

//
// have a look to Engine.cpp, in Engine::InitEngineAudio(),
// to understand what we are doing with the AudioTrack object.
//

AudioTrack::AudioTrack(nuiAudioFifo* pAudioFifo)
: nuiAudioTrack(), mFilter(44100, 2)
{
  mpAudioFifo = pAudioFifo;
  
  // open sound file from the binary's resources volume. If the file is in a regular folder of the HD, use a nglIFile constructor instead of nglPath.OpenRead
  nglIStream* istream = nglPath(_T("rsrc:/sample.wav")).OpenRead();
  if (!istream || !istream->Available())
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error trying to load sound"), eMB_OK);
    pBox->QueryUser(); // <=> make the message box modal. Once exiting the modal state, the box is auto-trashed, you don't have to do it yourself.
  }
  
  // load the Wave infos
  nuiWaveReader* pWaveReader = new nuiWaveReader(*istream);
  if (!pWaveReader->ReadInfo(mInfos))
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error, source is not a wav file"), eMB_OK);
    pBox->QueryUser();
  }

  // alloc local buffer and read the wave contents
  float* pSamples = new float[mInfos.GetSampleFrames() * mInfos.GetChannels()];  
  uint32 nbReadSamples = pWaveReader->Read((void*)pSamples, mInfos.GetSampleFrames(), eSampleFloat32);  
  delete pWaveReader;
  delete istream;
  
  
  // init deinterlaced main buffer, that is going to the contain the whole wave contents
  mWavContents.resize(mInfos.GetChannels());
  for (uint32 c = 0; c < mInfos.GetChannels(); c++)
    mWavContents[c].resize(mInfos.GetSampleFrames());
  
  // init deinterlace buffer for GUI's oscillo. It'll be filled with samples the soundcard is gonna play
  mSamplesBuffer.resize(mInfos.GetChannels());
  for (uint32 c = 0; c < mInfos.GetChannels(); c++)
    mSamplesBuffer[c].resize(2048);
  
  // deinterlace wave contents
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
  
  // init first sample to play
  mStartSample = 0;
  
  
  // initialise filters
  mFilter.SetQ(0.0f);
  mFilter.SetFreq(4000);
  mFilter.SetType(BiQuad::LowPass);
}



AudioTrack::~AudioTrack()
{
}


const std::vector<std::vector<float> >& AudioTrack::GetSamplesBuffer() const
{
  return mSamplesBuffer;
}


BiQuad& AudioTrack::GetFilter()
{
  return mFilter;
}


//********************************************************************************
//
// start to play. all we have to do is to register the audioTrack to the audio fifo
//
void AudioTrack::Start()
{
  uint32 nbChannels = 2;  
  float volume = 1.0f;
  bool enableBuffering = false; // the sound process we do is very light. we don't need to buffer the sound playing.
  
  if (!mpAudioFifo->RegisterTrack(this, mpAudioFifo->GetSampleRate(), nbChannels, volume, enableBuffering))
  {
    nuiMessageBox* pBox = new nuiMessageBox((nuiMainWindow*)GetMainWindow(), _T("nuiDemo"), _T("error trying to play sound"), eMB_OK);
    pBox->QueryUser();
  }
}



//********************************************************************************
//
// stop playing.
//
void AudioTrack::Stop()
{
  mpAudioFifo->UnregisterTrack(this);
  
  // reset the buffer of the GUI's oscillo
  for (uint32 c = 0; c < mSamplesBuffer.size(); c++)
  {
    for (uint32 s = 0; s < mSamplesBuffer[c].size(); s++)
      mSamplesBuffer[c][s] = 0.0f;
  }  
}


//********************************************************************************
//
// virtual method from nuiAudioTrack. Have a look to nuiAudioFifo.h
//
// as soon as the audioTrack is registered to the nuiAudioFifo, this callback is running.
// This is where you give the soundcard the sound samples it has requested.
//
uint32 AudioTrack::ReadSamples(uint32 sampleFrames, std::vector<float*>& rBuffer)
{
  // check if we have enough to give
  uint32 nbSamplesToCopy = MIN(sampleFrames, mInfos.GetSampleFrames() - mStartSample);

  // fill the output buffer
  ReadWaveContents(nbSamplesToCopy, rBuffer);
  
  // update for next cycle
  mStartSample += nbSamplesToCopy;
  
  
  // (nbSamplesToCopy < sampleFrames) <=> you couldn't copy as much samples as requested by the audio system
  // it happens for instance when the end of the sound sample has been reached.
  //
  // you may decide what you're gonna use to fill the rest of the buffer (if you don't do anything, you may get a crappy sound due to unknown buffer contents)
  // option 1: fill the rest of the buffer with 0.0f: to provide a clean silence
  // option 2: you want the sound to loop: go back to the sample's beginning and continue to copy...
  //
  // here, we want the sound to loop. It's the same copying code.
  if (nbSamplesToCopy < sampleFrames)
  {
    mStartSample = 0;
    
    nbSamplesToCopy = MIN(sampleFrames, mInfos.GetSampleFrames() - mStartSample);
    ReadWaveContents(nbSamplesToCopy, rBuffer);
    mStartSample += nbSamplesToCopy;
  }


  // apply filter
  mFilter.Process(rBuffer, rBuffer, sampleFrames);
  
  
  // fill the buffer for the GUI's oscillo
  for (uint32 c = 0; c < mSamplesBuffer.size(); c++)
  {
    for (uint32 s = 0; s < sampleFrames; s++)
      mSamplesBuffer[c][s] = rBuffer[c][s];
  }
  
  
  return sampleFrames;
}





void AudioTrack::ReadWaveContents(uint32 sampleFrames, std::vector<float*>& rBuffer)
{
  for (uint32 c = 0; c < rBuffer.size(); c++)
  {
    uint32 startSample = mStartSample;
    
    for (uint32 s = 0; s < sampleFrames; s++, startSample++)
      rBuffer[c][s] = mWavContents[c][startSample];
  }  
}


