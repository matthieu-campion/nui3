/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiMemoryPool::nuiMemoryPool(size_t size)
{
  mSize = size;
  mpMemory = (uint8*)malloc(size);
  mpCurrent = mpMemory;
}

nuiMemoryPool::~nuiMemoryPool()
{
  Clear();
  free(mpMemory);
}


size_t nuiMemoryPool::GetAvailable()
{
  return mSize - (mpCurrent - mpMemory);
}


void* nuiMemoryPool::Allocate(size_t size)
{
  // check if there is enough free space
  if ((mpCurrent + size) > (mpMemory + mSize))
  {
    NGL_OUT(_T("nuiMemoryPool 0x%x error : could not allocate %d bytes! (%d bytes available)\n"), this, size, GetAvailable());
    NGL_ASSERT(0);
    return NULL;
  }
  else
  {
    uint8* pRet = mpCurrent;
    mpCurrent += size;
    mpCurrent += 16 - ((size_t)mpCurrent%16);
    
    return pRet;
  }
}

void nuiMemoryPool::Clear(uint8* pLimit)
{
  if (pLimit < mpCurrent)
  {
    if (pLimit > mpMemory)
    {
      mpCurrent = pLimit;
    }
    else
    {
      mpCurrent = mpMemory;
    }
  }
}

nuiMemoryPoolGuard::nuiMemoryPoolGuard(nuiMemoryPool& rPool)
: mrPool(rPool), mpMemory(rPool.mpCurrent)
{
}

nuiMemoryPoolGuard::~nuiMemoryPoolGuard()
{
  mrPool.Clear(mpMemory);
}