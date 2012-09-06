/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

/*
* Life cycle
*/

nglIMemory::nglIMemory(const void* pBuffer, int64 ByteCnt, bool OwnBuffer)
{
  mpBuffer = (char*)pBuffer;
  mOwnBuffer = OwnBuffer;
  mSize = ByteCnt;
  mOffset = 0;
}

nglIMemory::~nglIMemory()
{
  if (mOwnBuffer)
    delete[] mpBuffer;
}


/*
* State/error methods
*/

nglStreamState nglIMemory::GetState() const
{
  if (mpBuffer == NULL) return eStreamNone; else
    if (mOffset >= mSize) return eStreamEnd; else
      return eStreamReady;
}

/*
* Stream navigation
*/

nglFileOffset nglIMemory::GetPos() const
{
  return mOffset;
}

nglFileOffset nglIMemory::SetPos(nglFileOffset Where, nglStreamWhence Whence)
{
  if (mpBuffer == NULL) return 0;

  // convert request to absolute offset (in Where)
  switch (Whence)
  {
  case eStreamFromStart: mOffset = (int64)Where; break;
  case eStreamForward  : mOffset += (int64)Where; break;
  case eStreamRewind   : mOffset -= (int64)Where; break;
  case eStreamFromEnd  : mOffset = (int64)(mSize - Where); break;
  }
  if (mOffset < 0) mOffset = 0;
  if (mOffset >= mSize) mOffset = mSize;
  return mOffset;
}

nglFileSize nglIMemory::Available (uint WordSize)
{
  return (WordSize) ? ((mSize - mOffset) / WordSize) : 0;
}


/*
* Input method
*/

int64 nglIMemory::Read (void* pData, int64 WordCount, uint WordSize)
{
  int64 left, byte_cnt;
  if ((pData == NULL) || (WordCount == 0) || (mpBuffer == NULL)) return 0;

  left = (mSize - mOffset) / WordSize;
  if (WordCount > left) WordCount = left;
  byte_cnt = WordCount * WordSize;
  memcpy (pData, mpBuffer + mOffset, (size_t)byte_cnt);

  mOffset += byte_cnt;

  if (mEndian != eEndianNative)
  {
    switch (WordSize)
    {
    case 2: bswap_16_s ((uint16*)pData, (size_t)WordCount); break;
    case 4: bswap_32_s ((uint32*)pData, (size_t)WordCount); break;
    case 8: bswap_64_s ((uint64*)pData, (size_t)WordCount); break;
    }
  }
  return WordCount;
}
