/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nui.h"
#include "nglAtomic.h"
#include "nuiNonCopyable.h"



//////////////////////////////// nglRingBuffer.h 
#define NGL_MAKE_POWER_OF_TWO_RINGBUFFER(X) { uint32 Y = 1; while (X>Y) Y = Y << 1; X = Y; }


// nglRingBuffer schematics legend:
// [ and ] delimits the complete buffer. (= mSize)
// * is a selected sample.
// - is an unselected sample.
// R is the read index.
// W is the write index.
class NGL_API nglRingBuffer : nuiNonCopyable
{
public:
  
  nglRingBuffer(uint32 countOfElement, uint32 sizeOfElement, int8 nbChannels = 1)
  {
    NGL_MAKE_POWER_OF_TWO_RINGBUFFER(countOfElement);
    mSize = countOfElement * sizeOfElement; 
    mSizeOfElement = sizeOfElement;
    mReadIndex = 0;
    mWriteIndex = 0;
    mEffectiveSize = countOfElement;
    
    // we add enough space at the end of the ring buffer for the linear interpolation
    //
    mpBuffers.resize(nbChannels);
    for (int i = 0; i < nbChannels; i++)
    {
      mpBuffers[i] = (uint8*)AlignedMalloc(mEffectiveSize + sizeOfElement);
      memset(mpBuffers[i], 0, mEffectiveSize + sizeOfElement);
    }
  }

  /////////////////////////////////////////

  virtual ~nglRingBuffer()
  {
    for (size_t i = 0; i < mpBuffers.size(); i++)
      AlignedFree(mpBuffers[i]);
  }

  inline uint32 GetSizeOfElement() const ///< Return the number of bytes for one element in one channel.
  {
    return mSizeOfElement;
  }

  inline void Reset() ///< Reset the read and write indices and epty the buffer.
  {
    ngl_atomic_set(mReadIndex, 0);
    ngl_atomic_set(mWriteIndex, 0);
  }

  // Getters and setters:
  inline uint32 GetReadIndex() const ///< Return the current read index in number of elements.
  {
    return mReadIndex;
  }

  inline uint32 GetWriteIndex() const ///< Return the current write index in number of elements.
  {
    return mWriteIndex;
  }

  inline void* GetReadPointer(uint32 Channel) const ///< Return the current read pointer for the given channel.
  {
    return mpBuffers[Channel] + (mReadIndex   * mSizeOfElement);
  }

  inline void* GetWritePointer(uint32 Channel) const ///< Return the current write pointer for the given channel.
  {
    return mpBuffers[Channel] + (mWriteIndex  * mSizeOfElement);
  }

  // Get Available sizes for read and write:
  inline uint32 GetReadable() const ///< Return the total number of elements we can read.
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

  inline uint32 GetWritable() const ///< Return the total number of elements we can write.
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

  inline uint32 GetWritableToEnd() const ///< Return the number of elements writeable until the end of the buffer
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

  inline uint32 GetReadableToEnd() const ///< Return the number of elements readable until the end of the buffer
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

  uint32 GetSize() const ///< Get the size in number of elements.
  {
    return mEffectiveSize;
  }
  
  uint32 GetChannelSizeInBytes() const ///< Return the number of bytes per channel.
  {
    return mSize;
  }
  
  bool AssertReadInRange(uint8* pPointer, int32 Channel) const ///< Return true if the given pointer is correct for the given channel.
  {
    return ( (pPointer < mpBuffers[Channel] + mSize) && (pPointer >= mpBuffers[Channel]) );
  }

  inline void AdvanceReadIndex(uint32 Advance) ///< Advance the read index in number of elements.
  {
    ngl_atomic_set(mReadIndex, (mReadIndex + Advance) % mEffectiveSize);
  }

  inline void SetReadIndex(uint32 readIndex) ///< Set the read index in number of elements.
  {
    ngl_atomic_set(mReadIndex, readIndex % mEffectiveSize);
  }

  inline void AdvanceWriteIndex(uint32 Advance) ///< Advance the write index in number of elements.
  {
    if (mWriteIndex == 0)
    {
      // copy one frame from the beginning of the ringbuffer to the end in order to ensure
      // the correct linear interpolation
      for (int i=0; i < (int)mpBuffers.size(); i++)
        memcpy(mpBuffers[i] + mSize, mpBuffers[i], mSizeOfElement);
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
  uint32 mSize; // Size if bytes
  uint32 mEffectiveSize; // Size if number of elements
  uint32 mSizeOfElement; // Size in byte of ONE element

  nglAtomic32 mReadIndex; // Read index in number of elements
  nglAtomic32 mWriteIndex; // Write index in number of elements
};

