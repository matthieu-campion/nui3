/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglStream.h
\brief Common definitions and base class for NGL I/O streams
*/

#ifndef __nglStream_h__
#define __nglStream_h__

//#include "nui.h"
#include "nglEndian.h"
#include "nglString.h"


//! Stream states
/*!
Current stream state
*/
enum nglStreamState
{
  eStreamNone,   ///< The stream is uninitialized or destroyed
  eStreamReady,  ///< Data is immediatly available
  eStreamEnd,    ///< End of stream
  eStreamWait,   ///< No data currently available
  eStreamError   ///< An error occured (no more available data)
};

//! Seek hint
/*!
Seeking positions and directions for nglStream::SetPos().
*/
enum nglStreamWhence
{
  eStreamFromStart,  ///< Move forward, from the beginning of the stream (offset 0)
  eStreamForward,    ///< Move forward from the current read/write position
  eStreamRewind,     ///< Move backwards from the current read/write position
  eStreamFromEnd     ///< Move backwards from the end of the stream
};


//! Base class for I/O streams
/*!
NGL provides simple yet advanced stream objects. Input and output streams
are handled in the distinct nglIStream and nglOStream classes. All stream
share the same services :

- seeking in stream when possible

- all base types read and write, any endian convention (see nglEndian.h)

- all read and write methods have two useful calling conventions

- high level text read and write, any encoding (conversions done on the fly
  and transparently), efficient line by line extraction
*/
class NGL_API nglStream
{
public:
  /** @name Life cycle */
  //@{
  nglStream();
  virtual ~nglStream();
  //@}

  /** @name Stream navigation */
  //@{
  virtual nglStreamState GetState() const = 0;
  /*!< Return current stream state (see nglStreamState)
  */
  virtual nglFileOffset GetPos() const = 0;
  /*!< Return current stream position (read/write offset) in bytes
    \return stream position in bytes
  */
  virtual nglFileOffset SetPos (nglFileOffset Where, nglStreamWhence Whence = eStreamFromStart) = 0;
  /*!< Change current stream position
    \param Where motion length in bytes
    \param Whence origin and direction
    \return effective motion length in bytes (using \a Whence direction)

    In many case, backward - and even forward - motions will bot be available, hence
    this method will return zero.
  */
  //@}

  /** @name Endianness */
  //@{
  nglEndian GetEndian() const;
  /*!< Get stream's endianness convention
    \return current endianness convention
  */
  virtual void SetEndian(nglEndian nglEndian);
  /*!<
    \param nglEndian new endianness convention

    Set stream's endianness. All further read and writes will use the new
    endianness (buffered pending writes are not affected).
  */
  //@}

  /** @name Text encoding and line-ending format */
  //@{
  nglTextEncoding GetTextEncoding();
  /*!< Retrieve current text encoding
    \return current text encoding

    The ReadLine() and WriteText() methods assume text input and output with
    a given text encoding. As a default, it is set to \e eEncodingNative.
  */
  virtual bool SetTextEncoding (nglTextEncoding Encoding);
  /*!< Set current text encoding
    \param Encoding text encoding

    The ReadLine() and WriteText() methods assume text input and output with
    a given text encoding. As a default, it is set to \e eEncodingNative.
  */
  nglTextFormat GetTextFormat();
  /*!< Retrieve current text output format
    \return current text format

    The WriteText() method must convert end of line charaters appropriately,
    depending on the platform or a given convention.
    As a default, it is set to \e eTextNative.
  */
  virtual void SetTextFormat (nglTextFormat Format);
  /*!< Set current text output format
    \return current text format

    The WriteText() method must convert end of line charaters appropriately,
    depending on the platform or a given convention.
    As a default, it is set to \e eTextNative.
  */
  //@}

protected:
  nglEndian       mEndian;
  nglTextFormat   mTextFormat;
  nglTextEncoding mTextEncoding;
  nglStringConv*  mpConv;

private:
  nglStream(const nglStream&) {} // Undefined copy constructor
};

#endif // __nglStream_h__
