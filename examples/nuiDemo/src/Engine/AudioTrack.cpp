/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Engine/AudioTrack.h"
#include "nuiMessageBox.h"

AudioTrack::AudioTrack(nuiAudioFifo* pAudioFifo)
: nuiAudioTrack()
{
  mpAudioFifo = pAudioFifo;
  
 }

AudioTrack::~AudioTrack()
{

}

void AudioTrack::Start()
{
//  uint32 nbChannels = 2;
//  float volume = 1.0f;
//  bool enableBuffering = true;
//  
//  if (!mpAudioFifo->RegisterTrack(this, mpAudioFifo->GetSampleRate(), nbChannels, volume, enableBuffering))
//  {
//    nuiMessageBox* pBox = new nuiMessageBox(GetMainWindow(), _T("nuiDemo"), _T("error trying to play sound"), eMB_OK);
//    pBox->QueryUser(); // make the message box modal. Once exiting the modal state, the box is auto-trashed, you don't have to do it yourself.
//  }
}