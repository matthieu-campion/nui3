/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


/*!
\file  nglIStream.h
\brief Input data stream base class
*/

#ifndef __nglIStream_h__
#define __nglIStream_h__

//#include "nui.h"
#include "nglStream.h"

class nglString;
class nglOStream;

#define NGL_STREAM_PIPE_BUF_SIZE (1024*64)


//! Base class for input streams
/*!
This is a pure virtual class, all object capable of providing data should
inherit from nglIStream (see nglIFile for instance).
*/
class NGL_API nglIStream : public virtual nglStream
{
public:
  /** @name Stream navigation */
  //@{
  virtual nglFileSize Available (uint WordSize = 1) = 0;
  /*!< Return the number of words immediately available with any input method (or Peek)
    \param WordSize word size in bytes
    \return number of available words of \a WordSize bytes
  */
  //@}

  /** @name Input methods */
  //@{
  virtual int64 ReadUInt8  (uint8*  pData, int64 Count = 1);  ///< Read \a Count 8-bit integer values, returns number of bytes effectively read
  virtual int64 ReadUInt16 (uint16* pData, int64 Count = 1);  ///< Read \a Count 16-bit integer values, returns number of 16-bit words effectively read
  virtual int64 ReadUInt32 (uint32* pData, int64 Count = 1);  ///< Read \a Count 32-bit integer values, returns number of 32-bit words effectively read
  virtual int64 ReadUInt64 (uint64* pData, int64 Count = 1);  ///< Read \a Count 64-bit integer values, returns number of 64-bit words effectively read
  virtual int64 ReadInt8   (int8*  pData, int64 Count = 1);  ///< Read \a Count 8-bit integer values, returns number of bytes effectively read
  virtual int64 ReadInt16  (int16* pData, int64 Count = 1);  ///< Read \a Count 16-bit integer values, returns number of 16-bit words effectively read
  virtual int64 ReadInt32  (int32* pData, int64 Count = 1);  ///< Read \a Count 32-bit integer values, returns number of 32-bit words effectively read
  virtual int64 ReadInt64  (int64* pData, int64 Count = 1);  ///< Read \a Count 64-bit integer values, returns number of 64-bit words effectively read
  virtual int64 ReadFloat  (float*  pData, int64 Count = 1);  ///< Read \a Count float values (IEEE 754 simple precision, 32 bits), returns number of 32-bit words effectively read
  virtual int64 ReadDouble (double* pData, int64 Count = 1);  ///< Read \a Count double values (IEEE 754 double precision, 64 bits), returns number of 64-bit words effectively read
  virtual int64 ReadLine   (nglString& rLine, nglTextFormat* pFormat = NULL);
  /*!< Read a text of line
    \param rLine string object where the text line will be stored
    \param pFormat used to return the encountered line ending convention (if non-NULL)
    \return number of \e bytes read from stream

    This method attempts to read a line of text from the stream.
    The ending convention (see nglTextFormat) is automatically detected and returned
    via *\a pFormat (if possible). If the end of stream was reached before the invocation
    or an error occurred, the method reports zero bytes read and *\a pFormat is left
    untouched. \b Note : SetTextFormat() and GetTextFormat() only set the line-ending
    convention for output, they have no relation with this method.

    Text encoding conversion is performed if necessary with nglStringImport defaults.
    See also SetTextEncoding() and GetTextEncoding().

    \a rLine is always emptied (even if an error occurred), and then filled with the
    encountered text line. This string does NOT contain the \\n line ending character,
    even if it was the last line of the stream or it didn't have a line ending (in
    this case \p *pFormat will also be set to \e eTextNone).

    \b Example : parse a stream line by line, display each of them with a number

\code
void DumpLines(nglIStream& rInput)
{
  nglString line;
  uint count = 0;

  while (rInput.ReadLine(line))
    NGL_OUT (_T("%d: %s\n"), ++count, line.GetChars());

  NGL_OUT(_T("Total: %d line%s\n"), count, PLURAL(count));
}
\endcode
  */
  virtual int64 ReadText  (nglString& rText, nglTextFormat* pFormat = NULL);
  /*!< Read a whole text file at once
    \param rText string object where the text will be stored
    \param pFormat used to return the encountered line ending convention (if non-NULL).
                   If several line endings are encountered during parsing, the best
                   occurrence is reported.
    \return number of \e bytes read from stream

    This method is equivalent to the following program (but more optimized, and with
    line ending weighting) :
\code
nglSize ReadText (nglString& rText, nglTextFormat* pFormat);
{
  nglString line;
  nglSize done = 0;

  while ((done =+ ReadLine(line)))
    rText += line + _T("\n");

  return done;
}
\endcode
  */
  virtual int64 Read (void* pData, int64 WordCount, uint WordSize) = 0;
  /*!< Read \a WordCount number of \a WordSize bytes long into \a pBuffer
    \param pData a buffer which may hold \a WordCount words of \a WordSize bytes
    \param WordCount number of words to read
    \param WordSize word size in bytes
    \return number of words effectively read

    Attempts to read data by \e word units of \a WordSize bytes. This method will not
    retrieve incomplete words (when there are not enough available bytes to make a word).
  */
  virtual int64 Peek (void* pData, int64 WordCount, uint WordSize);
  /*!< Peek \a WordCount number of \a WordSize bytes long into \a pBuffer
    \param pData a buffer which may hold \a WordCount words of \a WordSize bytes
    \param WordCount number of words to read
    \param WordSize word size in bytes
    \return number of words effectively read

    This method works as Read() but does not change the stream current position.
  */
  //@}

  /** @name Input methods (alternate) */
  //@{
  uint8   ReadUInt8  (uint8 Default = 0);     ///< Read an 8-bit integer value, returns \a Default if no data is available
  uint16  ReadUInt16 (uint16 Default = 0);    ///< Read a 16-bit integer value, returns \a Default if no data is available
  uint32  ReadUInt32 (uint32 Default = 0);    ///< Read a 32-bit integer value, returns \a Default if no data is available
  uint64  ReadUInt64 (uint64 Default = 0);    ///< Read a 64-bit integer value, returns \a Default if no data is available
  int8    ReadInt8   (int8 Default = 0);     ///< Read an 8-bit integer value, returns \a Default if no data is available
  int16   ReadInt16  (int16 Default = 0);    ///< Read a 16-bit integer value, returns \a Default if no data is available
  int32   ReadInt32  (int32 Default = 0);    ///< Read a 32-bit integer value, returns \a Default if no data is available
  int64   ReadInt64  (int64 Default = 0);    ///< Read a 64-bit integer value, returns \a Default if no data is available
  float   ReadFloat  (float Default = 0.0);   ///< Read a float value (IEEE 754 simple precision, 32 bits), returns \a Default if no data is available
  double  ReadDouble (double Default = 0.0);  ///< Read a double value (IEEE 754 double precision, 64 bits), returns \a Default if no data is available
  //@}
  
  /** @name Utilities */
  //@{
  int64 PipeTo(nglOStream& rTarget, int32 BufferSize = NGL_STREAM_PIPE_BUF_SIZE);
  /*!< Pipe to an output stream. Can be used to copy files.
    \param rTarget target stream
    \return total number of transfered bytes

    Transfer content from this input stream to the \p rTarget stream. Starts
    to pump bytes immediately until there is no more to read or the target
    stream is no more accepting data.
  */
  
  int64 PipeTo(nglOStream& rTarget, double MaxDuration, int32 BufferSize = NGL_STREAM_PIPE_BUF_SIZE);
  /*!< Pipe to an output stream during a period of time
    \param rTarget target stream
    \param MaxDuration max processing time (in seconds)
    \return number of transfered bytes
   
     Transfer content from this input stream to the \p rTarget stream. Starts
     to pump bytes immediately until the \p MaxDuration time has expired or there is no more to read or the target
     stream is no more accepting data.
   */
  
  //@}

};

#endif // __nglIStream_h__
