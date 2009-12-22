/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglOFile.h
\brief File as output stream
*/

#ifndef __nglOFile_h__
#define __nglOFile_h__

//#include "nui.h"
#include "nglOStream.h"
#include "nglFile.h"


///< nglOFile open modes, see nglFile for more info
enum nglOFileMode
{
  eOFileCreate = eFileWrite, 
  eOFileModify = eFileModify,
  eOFileAppend = eFileAppend
};


//! Output file stream
/*!
This output stream is based on the nglFile API, and can be used by all objects
based on generic output streams.
*/
class NGL_API nglOFile : public nglOStream
{
public:
  /** @name Life cycle */
  //@{
  nglOFile(nglFile* pFile, bool OwnFile = false);
  /*!<
    \param pFile output file
    \param OwnFile if true, \p pFile is automatically destroyed with this nglIFile instance

    Build a nglOFile from an existing nglFile object. The \a pFile object is
    not managed by nglOFile, ie. it won't be destroyed when the stream is
    deleted.

    The creation will fail it \a pFile has been created in read-only mode.
  */
  nglOFile(const nglPath& rPath, nglOFileMode Mode, bool OpenNow = true);
  /*!<
    \param rPath file's path
    \param Mode write mode (they are mapped to the write modes of nglFileMode)
    \param OpenNow if true, try to open file immediatly

    See nglFile::nglFile() and nglFileMode for more info
  */
  virtual ~nglOFile();
  //@}

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
  //@}

  /** @name Output methods */
  //@{
  virtual int64 Write (const void* pData, int64 WordCount, uint WordSize);
  virtual void Flush();               ///< Immediatly put pending writes on the storage medium (ignored for a read-only file)
  virtual void AutoFlush(bool Auto);  ///< If auto-flush is set, all write operations will return only once the data is effectively written on the storage medium
  //@}

  virtual void SetEndian(nglEndian Endian);
  
private:
  nglFile* mpFile;
  bool     mOwnFile;
};

#endif // __nglOFile_h__
