/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglIFile.h
\brief File as input file stream
*/

#ifndef __nglIFile_h__
#define __nglIFile_h__

//#include "nui.h"
#include "nglIStream.h"
#include "nglFile.h"


//! Input file stream
/*!
This input stream is based on the nglFile API, and can be used by all objects
based on generic input streams.
*/
class NGL_API nglIFile : public nglIStream
{
public:
  /** @name Life cycle */
  //@{
  nglIFile(nglFile* pFile, bool OwnFile = false);
  /*!<
    \param pFile input file
    \param OwnFile if true, \p pFile is automatically destroyed with this nglIFile instance

    Build a nglIFile from an existing nglFile object. The \a pFile object is
    not managed by nglIFile, ie. it won't be destroyed when the stream is
    deleted.
  */
  nglIFile(const nglPath& rPath, bool OpenNow = true);
  /*!<
    \param rPath file's path
    \param OpenNow if true, try to open file immediatly

    See nglFile::nglFile().
  */
  virtual ~nglIFile();
  //@}
	
	/** Retrieve the file handle of the stream */
	nglFile* GetFile() const;

  /** @name State/error methods */
  //@{
  bool Open();
  /*!< Open the file
    \return true if the file was succesfully opened

    Note that a file can be opened immediatly from the constructor if the \p OpenNow
    parameter is set to true.
  */
  
	bool IsOpen() const;
	/*!< Check if the file is open
   \return true if the file is open
   
   Some methods can set an an error for a File object, however
   the file is still open (and thus usable). This method let you tell if an error
   was fatal or not.
   
   Note : errors set by the File constructors are \e all fatal (ie. IsOpen()
   will return \e false if an error was set).
   */
  
  
  void Close();
  /*!< Close the file immediatly
    Note that a file is implicitly closed when its object is deleted.
  */
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

  virtual void SetEndian(nglEndian Endian);  
  
private:
  nglFile* mpFile;
  bool     mOwnFile;
};

#endif // __nglIFile_h__
