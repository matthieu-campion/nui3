/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

#define CHUNK_INIT  1024
#define CHUNK_MAX   (64*1024)

/*
* Life cycle
*/

nglOMemory::nglOMemory()
{
  mpBuffer = NULL;
  mBufferSize = 0;
  mSize = 0;
  mOffset = 0;
  mChunkSize = CHUNK_INIT;
}

nglOMemory::~nglOMemory()
{
  if (mpBuffer) free (mpBuffer);
}


/*
* State/error methods
*/

nglStreamState nglOMemory::GetState() const
{
  return (mOffset >= mSize) ? eStreamEnd : eStreamReady;
}


/*
* Stream navigation
*/

nglFileOffset nglOMemory::GetPos() const
{
  return mOffset;
}

nglFileOffset nglOMemory::SetPos(nglFileOffset Where, nglStreamWhence Whence)
{
  if (mpBuffer == NULL) return 0;

  // convert request to absolute offset (in Where)
  switch (Whence)
  {
  case eStreamFromStart: mOffset = Where; break;
  case eStreamForward  : mOffset += Where; break;
  case eStreamRewind   : mOffset -= Where; break;
  case eStreamFromEnd  : mOffset = mSize - Where; break;
  }
  if (mOffset < 0) mOffset = 0;
  if (mOffset >= mSize) mOffset = mSize;

  return mOffset;
}


/*
* Buffer access
*/

const char* nglOMemory::GetBufferData() const
{
  return mpBuffer;
}

nglFileOffset nglOMemory::GetBufferSize() const
{
  return mBufferSize;
}

nglFileOffset nglOMemory::GetSize() const
{
  return mSize;
}


/*
* Output method
*/

int64 nglOMemory::Write(const void* pData, int64 WordCount, uint WordSize)
{
  nglFileOffset need, left, byte_cnt;
  if ((pData == NULL) || (WordCount == 0)) return 0;

  need = WordCount * WordSize;
  left = mBufferSize - mOffset;
  if (need > left)
  {
    char* ptr;
    nglFileOffset new_size = mOffset + need;

    // Round to chunk size
    new_size = ((new_size + mChunkSize - 1) / mChunkSize) * mChunkSize;
    if (mChunkSize < CHUNK_MAX)
      mChunkSize <<= 1;
    ptr = (char*) realloc (mpBuffer, (size_t)new_size);
    if (ptr)
    {
      mpBuffer = ptr;
      mBufferSize = new_size;
    }
  }
  byte_cnt = WordCount * WordSize;
  memcpy (mpBuffer + mOffset, pData, (size_t)byte_cnt);

  if ((WordSize > 1) && (mEndian != eEndianNative))
  {
    // allocate for a byte swapped buffer copy
    void* pTemp = mpBuffer + mOffset;
    if (!pData) return 0;
    switch (WordSize)
    {
    case 2: bswap_16_s ((uint16*)pTemp, (size_t)WordCount); break;
    case 4: bswap_32_s ((uint32*)pTemp, (size_t)WordCount); break;
    case 8: bswap_64_s ((uint64*)pTemp, (size_t)WordCount); break;
    }
  }

  mOffset += byte_cnt;
  if (mOffset > mSize) mSize = mOffset;
  return WordCount;
}
