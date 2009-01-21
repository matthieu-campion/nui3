/*
 *  nuiAudioDecoder.cpp
 *  nui3
 *
 *  Created by mat on 12/18/08.
 *  Copyright 2008 Mxp4. All rights reserved.
 *
 */
#include "nui.h"
#include "nuiAudioDecoder.h"

nuiAudioDecoder::nuiAudioDecoder(nglIStream& rStream) :
  nuiSampleReader(rStream),
  mpPrivate(NULL)
{
}

nuiAudioDecoder::nuiAudioDecoder(const nuiAudioDecoder& rDecoder, nglIStream& rStream) :
nuiSampleReader(rDecoder, rStream),
mpPrivate(NULL)
{
}

nuiAudioDecoder::~nuiAudioDecoder()
{
  Clear();
}

nuiSampleReader* nuiAudioDecoder::Clone(nglIStream& rStream) const
{
  nuiAudioDecoder* pDecoder = new nuiAudioDecoder(*this, rStream);
  pDecoder->SetPosition(0);
  return pDecoder;
}


void nuiAudioDecoder::SetPosition(uint32 pos)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return;
 
  if (Seek(pos))
  {
    mPosition = pos;
  }
}



