/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglIOFile.h
\brief File as input/output file stream
*/

#ifndef __nglIOFile_h__
#define __nglIOFile_h__

//#include "nui.h"
#include "nglIOStream.h"
#include "nglOFile.h"

class nglFile;
class nglPath;

//! Input file stream
/*!
This input stream is based on the nglFile API, and can be used by all objects
based on generic input streams.
*/
class NGL_API nglIOFile : public nglIOStream
{
public:
  /** @name Life cycle */
  //@{
  nglIOFile(nglFile* pFile, bool OwnFile = false);
  /*!<
  \param pFile input file
  \param OwnFile if true, \p pFile is automatically destroyed with this nglIOFile instance

  Build a nglIOFile from an existing nglFile object. The \a pFile object is
  not managed by nglIOFile, i.e. it won't be destroyed when the stream is
  deleted.
  */
  nglIOFile(const nglPath& rPath, nglOFileMode Mode, bool OpenNow = true);
  /*!<
  \param rPath file's path
  \param Mode write mode (they are mapped to the write modes of nglFileMode)
  \param OpenNow if true, try to open file immediately

  See nglFile::nglFile() and nglFileMode for more info
  */
  virtual ~nglIOFile();
  //@}

  /** @name State/error methods */
  //@{
  bool Open();
  /*!< Open the file
  \return true if the file was successfully opened

  Note that a file can be opened immediately from the constructor if the \p OpenNow
  parameter is set to true.
  */
  
  
	bool IsOpen() const;
	/*!< Check if the file is open
   \return true if the file is open
   
   Some methods can set an an error for a File object (see GetError()), however
   the file is still open (and thus usable). This method let you tell if an error
   was fatal or not.
   
   Note : errors set by the File constructors are \e all fatal (ie. IsOpen()
   will return \e false if an error was set).
   */  
  
  void Close();
  /*!< Close the file immediately
  Note that a file is implicitly closed when its object is deleted.
  */
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

  /** @name Output methods */
  //@{
  virtual int64 Write (const void* pData, int64 WordCount, uint WordSize);
  virtual void Flush();               ///< Immediately put pending writes on the storage medium (ignored for a read-only file)
  virtual void AutoFlush(bool Auto);  ///< If auto-flush is set, all write operations will return only once the data is effectively written on the storage medium
  //@}
private:
  nglFile* mpFile;
  bool     mOwnFile;
};

#endif // __nglIOFile_h__
