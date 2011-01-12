/*
 *  nuiFileSound.cpp
 *  nui3
 *
 *  Created by  mat on 1/12/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#include "nuiFileSound.h"
#include "nuiFileVoice.h"

nuiFileSound::nuiFileSound(const nglPath& rPath)
: nuiSound(rPath)
{
  mType = eStream;
}
nuiFileSound::~nuiFileSound()
{
  
}

nuiVoice* nuiFileSound::GetVoice()
{
  nuiVoice* pVoice = new nuiFileVoice(this);
  return pVoice;
}