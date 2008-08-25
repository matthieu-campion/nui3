/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglOMemory.h
\brief Memory buffer as input stream
*/

#ifndef __nglOMemory_h__
#define __nglOMemory_h__

//#include "nui.h"
#include "nglOStream.h"


//! Memory buffer as input stream class
/*!
This simple class let you use memory as an output stream. Memory is
automatically allocated and grown along stream usage.
See also nglIMemory.
*/
class NGL_API nglOMemory : public nglOStream
{
public:
  /** @name Life cycle */
  //@{
  nglOMemory();
  virtual ~nglOMemory();
  //@}

  /** @name State/error methods */
  //@{
  nglStreamState GetState() const;
  uint           GetError() const;
  const nglChar* GetErrorStr(uint Error) const;
  //@}

  /** @name Stream navigation */
  //@{
  nglFileOffset GetPos() const;
  nglFileOffset SetPos (nglFileOffset Where, nglStreamWhence Whence = eStreamFromStart);
  //@}

  /** @name Buffer access */
  //@{
  const char* GetBufferData() const;  ///< Return buffer data address
  nglFileOffset     GetBufferSize() const;  ///< Return buffer size in bytes. This is always greater or at least equal to GetSize().
  nglFileOffset     GetSize() const;        ///< Return the number of bytes written into the buffer
  //@}

  /** @name Output methods */
  //@{
  virtual int64 Write (const void* pData, int64 WordCount, uint WordSize);
  //@}

private:
  char*   mpBuffer;
  nglFileOffset mBufferSize;
  nglFileOffset mSize;
  nglFileOffset mOffset;
  nglFileOffset mChunkSize;
};

#endif // __nglOMemory_h__
