/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\Zip  nglIZip.h
\brief Zip as input Zip stream
*/

#ifndef __nglIZip_h__
#define __nglIZip_h__

//#include "nui.h"
#include "nglIStream.h"

class nglZipFS;


//! Input Zip stream
/*!
This input stream is based on the nglZip API, and can be used by all objects
based on generic input streams.
*/
class NGL_API nglIZip : public nglIStream
{
public:
  /** @name Destructor */
  //@{
  virtual ~nglIZip();
  //@}

  /** @name State/error methods */
  //@{
  nglStreamState GetState() const;
  uint           GetError() const;
  const nglChar* GetErrorStr() const;
  const nglChar* GetErrorStr(uint Error) const;
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

private:
  friend class nglZipFS;

  nglZipFS* mpZip;
  void*     mpUnzip;
  nglSize   mSize;
  int       mNumOfFile;
  int       mPosInZipDirectory;

  nglIZip(nglZipFS* pZip, void* pUnzip, nglSize size, int NumOfFile,int PosInZipDirectory);

  void SetRef(void* pUnzip);
};

#endif // __nglIZip_h__
