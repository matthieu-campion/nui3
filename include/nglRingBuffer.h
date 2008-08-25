/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nui.h"
#include "nglAtomic.h"



//////////////////////////////// nglRingBuffer.h 
#define NGL_MAKE_POWER_OF_TWO_RINGBUFFER(X) { uint32 Y = 1; while (X>Y) Y=Y<<1; X = Y; }


// nglRingBuffer schematics legend:
// [ and ] delimits the complete buffer. (= mSize)
// * is a selected sample.
// - is an unselected sample.
// R is the read index.
// W is the write index.
class NGL_API nglRingBuffer
{
public:
  
  nglRingBuffer(uint32 size, uint32 sizeOfElement, int8 nbChannels = 1)
  : mSize(size * sizeOfElement), 
    mSizeOfElement(sizeOfElement),
    mSizeOfFrame(mSizeOfElement),
    mReadIndex(0),
    mWriteIndex(0)
  {
    mEffectiveSize = mSize - (mSize % mSizeOfFrame);
    
    // we add enough space at the end of the ring buffer for the linear interpolation
    //
    mpBuffers.resize(nbChannels);
    for (int i = 0; i < nbChannels; i++)
      mpBuffers[i] = (uint8*)AlignedMalloc(mSize+1);
    
    NGL_MAKE_POWER_OF_TWO_RINGBUFFER(mSizeOfElement);
    assert(sizeOfElement == mSizeOfElement);
  }


  /////////////////////////////////////////

  virtual ~nglRingBuffer()
  {
    for (size_t i = 0; i < mpBuffers.size(); i++)
      AlignedFree(mpBuffers[i]);
  }

  inline void SetSizeOfElement(uint32 sizeOfElement)
  {
    mSizeOfElement = sizeOfElement;
    NGL_MAKE_POWER_OF_TWO_RINGBUFFER(mSizeOfElement);
    assert(sizeOfElement == mSizeOfElement);
  }

  inline uint32 GetSizeOfElement() const
  {
    return mSizeOfElement;
  }

  inline void SetSizeOfFrame(uint32 sizeOfFrame)
  {
    mSizeOfFrame = sizeOfFrame;
    mEffectiveSize = mSize - (mSize % mSizeOfFrame);
  }

  inline uint32 GetSizeOfFrame() const
  {
    return mSizeOfFrame;
  }

  inline void Reset()
  {
    ngl_atomic_set(mReadIndex, 0);
    ngl_atomic_set(mWriteIndex, 0);
  }

  // Getters and setters:
  inline uint32 GetReadIndex() const
  {
    return mReadIndex;
  }

  inline uint32 GetWriteIndex() const
  {
    return mWriteIndex;
  }

  inline void* GetReadPointer(uint32 Channel) const
  {
    return mpBuffers[Channel] + (mReadIndex   * mSizeOfElement);
  }

  inline void* GetWritePointer(uint32 Channel) const
  {
    return mpBuffers[Channel] + (mWriteIndex  * mSizeOfElement);
  }

  // Get Available sizes for read and write:
  inline uint32 GetReadable() const
  {
    const uint32 r = ngl_atomic_read(mReadIndex);
    const uint32 w = ngl_atomic_read(mWriteIndex);
    
    if (w >= r)
    {
      // [-----R*****W-----]
      return (w - r);
    }
    else
    {
      // [*****W-----R*****]
      return (mEffectiveSize - r + w);
    }
  }

  inline uint32 GetWritable() const
  {
    const uint32 r = ngl_atomic_read(mReadIndex);
    const uint32 w = ngl_atomic_read(mWriteIndex);
    
    if ( w > r )
    {
      // [*****R-----W*****]
      return ((mEffectiveSize - w + r) - 1);
    }
    else if ( w < r )
    {
      // [-----W*****R-----]
      return ((r - w) - 1);
    }
    else
    {
      // [-----RW-----]
      return (mEffectiveSize - 1);
    }
  }

  inline uint32 GetWritableToEnd() const
  {
    const uint32 r = ngl_atomic_read(mReadIndex);
    const uint32 w = ngl_atomic_read(mWriteIndex);
    
    if ( r > w )
    {
      // [-----W*****R-----]
      return (r - w - 1);
    }
    
    if ( r != 0 )
    {
      // [-----R-----W*****]
      return (mEffectiveSize - w);
    }
    
    // [R-----W*****]
    return (mEffectiveSize - w - 1);
  }

  inline uint32 GetReadableToEnd() const
  {
    const uint32 r = ngl_atomic_read(mReadIndex);
    const uint32 w = ngl_atomic_read(mWriteIndex);
    
    if ( w >= r )
    {
      // [-----R*****W-----]
      return (w - r);
    }
    
    // [-----W-----R*****]
    return (mEffectiveSize - r);
  }

  uint32 GetSize() const { return mSize; }

  bool AssertReadInRange(uint8* pPointer, int32 Channel) const
  {
    return ( (pPointer < mpBuffers[Channel] + (mEffectiveSize * mSizeOfElement)) && (pPointer >= mpBuffers[Channel]) );
  }

  inline void AdvanceReadIndex(uint32 Advance)
  {
    ngl_atomic_set(mReadIndex, (mReadIndex + Advance) % mEffectiveSize);
  }

  inline void SetReadIndex(uint32 readIndex)
  {
    ngl_atomic_set(mReadIndex, readIndex % mEffectiveSize);
  }

  inline void AdvanceWriteIndex(uint32 Advance)
  {
    if (mWriteIndex == 0)
    {
      // copy one frame from the beginning of the ringbuffer to the end in order to ensure
      // the correct linear interpolation
      for (int i=0; i < (int)mpBuffers.size(); i++)
        memcpy(mpBuffers[i] + (mEffectiveSize * mSizeOfElement/***/), mpBuffers[i], mSizeOfFrame);
    }
    ngl_atomic_set(mWriteIndex, (mWriteIndex + Advance) % mEffectiveSize);
  }

  uint32 GetNbChannels() const
  {
    return (uint32)mpBuffers.size(); 
  }

  private:
  ////////////////////////////////////////////
  //

  inline  uint8* AlignedMalloc (int32 size)
  {
    uint8* newptr;
    
    newptr = (uint8*) malloc (size * mSizeOfElement);
    
    return newptr;
  }

  inline void AlignedFree (uint8* ptr)
  {
    if (!ptr)
      return;
    free (ptr);        
  }

  std::vector<uint8*> mpBuffers;
  uint32 mSize;
  uint32 mEffectiveSize;
  uint32 mSizeOfElement;
  uint32 mSizeOfFrame;

  nglAtomic mReadIndex;
  nglAtomic mWriteIndex;
};

