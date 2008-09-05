/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglOStream.h
\brief Output data stream base class
*/

#ifndef __nglOStream_h__
#define __nglOStream_h__

//#include "nui.h"
#include "nglStream.h"

class nglString;

//! Base class for output streams
/*!
This is a pure virtual base class for writable streams (see nglOFile for instance).
*/
class NGL_API nglOStream : public virtual nglStream
{
public:
  /** @name Output methods */
  //@{
  virtual int64 WriteUInt8  (const uint8*  pData, int64 Count = 1); ///< Write \a Count 8-bit integer values, returns number of bytes effectively written
  virtual int64 WriteUInt16 (const uint16* pData, int64 Count = 1); ///< Write \a Count 16-bit integer values, returns number of 16-bit words effectively written
  virtual int64 WriteUInt32 (const uint32* pData, int64 Count = 1); ///< Write \a Count 32-bit integer values, returns number of 32-bit words effectively written
  virtual int64 WriteUInt64 (const uint64* pData, int64 Count = 1); ///< Write \a Count 64-bit integer values, returns number of 64-bit words effectively written
  virtual int64 WriteInt8   (const int8*  pData, int64 Count = 1); ///< Write \a Count 8-bit integer values, returns number of bytes effectively written
  virtual int64 WriteInt16  (const int16* pData, int64 Count = 1); ///< Write \a Count 16-bit integer values, returns number of 16-bit words effectively written
  virtual int64 WriteInt32  (const int32* pData, int64 Count = 1); ///< Write \a Count 32-bit integer values, returns number of 32-bit words effectively written
  virtual int64 WriteInt64  (const int64* pData, int64 Count = 1); ///< Write \a Count 64-bit integer values, returns number of 64-bit words effectively written
  virtual int64 WriteFloat  (const float*  pData, int64 Count = 1); ///< Write \a Count float values (IEEE 754 simple precision, 32 bits), returns number of 32-bit words effectively written
  virtual int64 WriteDouble (const double* pData, int64 Count = 1); ///< Write \a Count double values (IEEE 754 double precision, 64 bits), returns number of 64-bit words effectively written
  virtual int64 WriteText   (const nglChar* pData, int64 Count = -1);
  /*!< Write some text
    \param pData text data
    \param Count maximum number of chars to write, none if -1 (\p pData must be null-terminated)
    \return number of \e bytes written

    This method uses the current format and encoding for its output (see SetTextFormat()
    and SetTextEncoding()). Please note that the number of \e bytes (not chars) is
    returned.
  */
  virtual int64 WriteText   (const nglString& rData);
  /*!< Write some text
    \param rData text data
    \return number of \e bytes written

    This method uses the current format and encoding for its output (see SetTextFormat()
    and SetTextEncoding()). Please note that the number of \e bytes (not chars) is
    returned.
  */
  virtual int64 Write (const void* pData, int64 WordCount, uint WordSize) = 0;
  /*!<
    Write \a WordCount number of \a WordSize bytes long from \a pBuffer,
    returns the number of words effectively written.
  */
  //@}

private:
  bool GetOConv();
};

#endif // __nglOStream_h__
