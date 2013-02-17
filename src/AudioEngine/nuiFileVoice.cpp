/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiFileVoice::nuiFileVoice(nuiFileSound* pSound)
: nuiVoice(pSound),
  mpFileSound(pSound),
  mpStream(NULL),
  mpReader(NULL)
{
  Init();
}

nuiFileVoice::~nuiFileVoice()
{
  
}

nuiFileVoice::nuiFileVoice(const nuiFileVoice& rVoice)
: nuiVoice(rVoice),
  mpFileSound(NULL),
  mpStream(NULL),
  mpReader(NULL)
{
  *this = rVoice;
}


nuiFileVoice& nuiFileVoice::operator=(const nuiFileVoice& rVoice)
{
  this->nuiVoice::operator=(rVoice);
  mpFileSound = rVoice.mpFileSound;
  
  Init();
  
  return *this;
}


bool nuiFileVoice::IsValid() const
{
  return mpSound && mpStream && mpReader;
}

bool nuiFileVoice::Init()
{
  if (mpStream)
    delete mpStream;
  if (mpReader)
    delete mpReader;
  mpStream = NULL;
  mpReader = NULL;
  
  if (!mpSound)
    return false;
  
  nglPath path = mpFileSound->GetPath();
  
  if (!path.Exists())
  {
    NGL_OUT(_T("Can't load this audio file: %s (file does not exist)\n"), path.GetNodeName().GetChars());
    return false;
  }
  
  nglIStream* pStream = path.OpenRead();
  if (!pStream)
  {
    NGL_OUT(_T("Can't load this audio file: %s (stream can't be open)\n"), path.GetNodeName().GetChars());
    return false;
  }
  
  nuiSampleReader* pReader = NULL;
  nuiSampleInfo info;
  
  pReader = new nuiWaveReader(*pStream);
  if (!pReader->GetInfo(info))
  {
    delete pReader;
    pReader = new nuiAiffReader(*pStream);
    if (!pReader->GetInfo(info))
    {
      delete pReader;
      pReader = new nuiAudioDecoder(*pStream);
      if (!pReader->GetInfo(info))
      {
        NGL_OUT(_T("Can't load this audio file: %s (reader can't be created)\n"), path.GetNodeName().GetChars());
        delete pReader;
        delete pStream;
        return false;
      }
    }
  }
  
  mpStream = pStream;
  mpReader = pReader;
  mInfo = info;
  NGL_OUT(_T("audio file loaded: %s\n"), path.GetNodeName().GetChars());
  return true;
}

int32 nuiFileVoice::GetChannels() const
{
  return mInfo.GetChannels();
}

int32 nuiFileVoice::GetSampleFrames() const
{
  return mInfo.GetSampleFrames();
}



int32 nuiFileVoice::ReadSamples(const std::vector<float*>& rOutput, int64 position, int32 SampleFrames)
{
  if (!IsValid())
    return 0;
  
  if (position >= mInfo.GetSampleFrames())
    return 0;
  int64 todo = MIN(SampleFrames, mInfo.GetSampleFrames() - position);
  
  std::vector<void*> temp;
  for (int32 i = 0; i < rOutput.size(); i++)
    temp.push_back((void*)rOutput[i]);
  
  mpReader->SetPosition(position);
  int32 read = mpReader->ReadDE(temp, todo, eSampleFloat32);
  return read;
}

