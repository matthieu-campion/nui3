/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Engine/Engine.h"

Engine::Engine()
{
  mpAudioFifo = InitEngineAudio();
  mpAudioTrack = new AudioTrack(mpAudioFifo);
}


Engine::~Engine()
{
  mpAudioTrack->Stop();
  mpAudioFifo->Close();
  delete mpAudioFifo;
}


nuiAudioFifo* Engine::InitEngineAudio()
{
  // parameters for audio system
  int64 inBufSize = 16 * 1024; // input buffer size <=> input sound buffering 
  int64 outBufSize = 4096;      // output buffer size <=> buffer for output playing to soundcard
  double sampleRate = 44100.0f; // smaple rate
  int64 nbChannels = 2;         // nb channels
  
  // we're going to use the system default audio devices (<=> nglString::Null as value for the following parameters)
  // take a look to nuiAudioDevice.h if you want to go forward into the audio devices handling
  nglString inDeviceName = nglString::Null; // input device reference name
  nglString outDeviceName = nglString::Null; // output device reference name
  nglString apiName = nglString::Null; // audio api reference name

  // The audio fifo is the main audio system of NUI.
  nuiAudioFifo* pAudioFifo = new nuiAudioFifo(inBufSize, outBufSize, sampleRate, nbChannels, inDeviceName, outDeviceName, apiName);
  pAudioFifo->Start();  
  return pAudioFifo;

  // Once you've initialized and started the audio fifo, 
  // you can register any nuiAudioTrack you want by the audio fifo. (in nuiDemo, it's done in Audio.cpp)
  // 
  // An nuiAudioTrack is a stream to the audio fifo, to play sound.
  //
  // Each registered nuiAudioTrack is instantly processed by the audio fifo.
  // Once unregistered, the nuiAudioTrack stops to be processed.
  //
  // All the nuiAudioTrack are mixed through the audio fifo.
  // If your application has several audio streams to play in same time,
  // you can use a nuiAudioTrack for each stream,
  // or mix the stream yourself if you like, and use a single nuiAudioTrack.
  //
}



AudioTrack* Engine::GetAudioTrack()
{
  return mpAudioTrack;
}
