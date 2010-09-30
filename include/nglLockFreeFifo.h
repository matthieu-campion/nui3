/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiNonCopyable.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		LockFreeFifo
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class nglLockFreeFifo : nuiNonCopyable
  {
  public:
    nglLockFreeFifo (uint32 bufsz) : mReadIdx(0), mWriteIdx(0), mBuffer(bufsz)
    {
    }
    
    T Get(void)
    {
      NGL_ASSERT(mReadIdx != mWriteIdx);
      //	throw runtime_error ("lock free fifo underrun");
      
      T result = mBuffer[mReadIdx];
      
      if ((mReadIdx + 1) >= mBuffer.size())
        mReadIdx = 0;
      else
        mReadIdx = mReadIdx + 1;
      
      return result;
    }
    
    void Put(T element)
    {
      uint32 newIdx;
      
      if ((mWriteIdx + 1) >= mBuffer.size())
        newIdx = 0;
      else
        newIdx = mWriteIdx + 1;
      
      NGL_ASSERT(newIdx != mReadIdx);
      //throw runtime_error ("lock free fifo overrun");
      
      mBuffer[mWriteIdx] = element;
      
      mWriteIdx = newIdx;
    }
    
    bool CanRead() const 
    {
      return mReadIdx != mWriteIdx;
    }
    
    bool CanWrite() const 
    {
      uint32 newIdx;
      
      if ((mWriteIdx + 1) >= mBuffer.size())
        newIdx = 0;
      else
        newIdx = mWriteIdx + 1;
      
      return newIdx != mReadIdx;
    }
    
  private:
    
    nglAtomic mReadIdx;
    nglAtomic mWriteIdx;
    std::vector<T> mBuffer;
  };

