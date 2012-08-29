/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglIMemory.h
\brief Memory buffer as input stream
*/

#ifndef __nglIMemory_h__
#define __nglIMemory_h__

//#include "nui.h"
#include "nglIStream.h"


//! Memory buffer as input stream
/*!
This simple class let you use any memory portion as an input stream.
See also nglOMemory.

*/
class NGL_API nglIMemory : public nglIStream
{
public:
  /** @name Life cycle */
  //@{
  nglIMemory(const void* pBuffer, int64 ByteCnt, bool OwnBuffer = false);
  /*!<
    \param pBuffer user buffer
    \param ByteCnt byte count of user buffer (if any)

    Build a nglIMemory from an existing buffer. The buffer is not managed by
    nglIMemory, ie. it won't be freed when the stream is deleted.
  */
  //@}
  
  virtual ~nglIMemory();

  /** @name State/error methods */
  //@{
  nglStreamState GetState() const;
  //@}

  /** @name Stream navigation */
  //@{
  nglFileOffset GetPos() const;
  nglFileOffset SetPos (nglFileOffset Where, nglStreamWhence Whence = eStreamFromStart);
  nglFileSize Available (uint WordSize = 1);
  //@}

  /** @name Input methods */
  //@{
  virtual int64 Read (void* pData, int64 WordCount, uint WordSize = 1);
  //@}

protected:
  char*   mpBuffer;
  bool mOwnBuffer;
  nglSize mSize;
  nglSize mOffset;
};

#endif // __nglIMemory_h__
