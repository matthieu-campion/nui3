/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*!
\file  nglStringConv.h
\brief Text encoding conversion management
*/

#ifndef __nglStringConv_h__
#define __nglStringConv_h__

//#include "nui.h"
#include "nglError.h"


/* Conversion error codes
 */
#define NGL_STRINGCONV_ENONE   0  ///< Conversion successfully completed
#define NGL_STRINGCONV_ERANGE  1  ///< Parameter out of range
#define NGL_STRINGCONV_ECONV   2  ///< Unsupported conversion
#define NGL_STRINGCONV_ELEFT   3  ///< Conversion incomplete (input bytes left represent an incomplete sequence)
#define NGL_STRINGCONV_EMORE   4  ///< Conversion incomplete (insufficient output buffer)


//! Text encoding (aka 'charset')
/*!
A text encoding is a mapping convention between symbols (number, latin characters, etc)
and numerical values. Since most encodings use a restricted 8 bit address space (256 symbols),
they are specific to a language or a set of languages. Alphabets with more symbols (such
as Japanese Kanjis or Katakana) need more values, and use a so called 'multi-byte character
set' where a symbol can be represented by one or more bytes.

Notes :
- ISO-8859-1 and ISO-8859-15 are ASCII supersets, prefer them other plain ASCII
- UTF-7 and UTF-8 are popular multi-byte encodings for the Unicode universal character
set, commonly used for Unicode storage or interchange (see http://www.unicode.org)
*/
enum nglTextEncoding
{
  /** @name 8 bit encodings
  */
  //@{
  // ISO standards
  eISO8859_1,    ///< Latin1 (West European)
  eISO8859_2,    ///< Latin2 (East European)
  eISO8859_3,    ///< Latin3 (South European)
  eISO8859_4,    ///< Latin4 (North European)
  eISO8859_5,    ///< Cyrillic (see also KOI8)
  eISO8859_6,    ///< Arabic
  eISO8859_7,    ///< Greek
  eISO8859_8,    ///< Hebrew
  eISO8859_9,    ///< Latin5 (Turkish)
  eISO8859_10,   ///< Latin6 (Nordic)
  eISO8859_11,   ///< Thai
  eISO8859_12,   ///< reserved
  eISO8859_13,   ///< Latin7 (Baltic Rim)
  eISO8859_14,   ///< Latin8 (Celtic)
  eISO8859_15,   ///< Latin15 or Latin0 (Latin1 replacement, with 'euro')

  // Mac
  eAppleRoman,   ///< Apple Roman

  // Windows code pages
  eCP1250,       ///< Code page 1250 (Central Europe)
  eCP1251,       ///< Code page 1251 (Cyrillic)
  eCP1252,       ///< Code page 1252 (Latin I)
  eCP1253,       ///< Code page 1253 (Greek)
  eCP1254,       ///< Code page 1254 (Turkish)
  eCP1255,       ///< Code page 1255 (Hebrew)
  eCP1256,       ///< Code page 1256 (Arabic)
  eCP1257,       ///< Code page 1257 (Baltic)
  eCP1258,       ///< Code page 1258 (Viet Nam)
  eCP874,        ///< Code page 874 (Thai)
  //@}

  /** @name Multi-byte encodings
  */
  //@{
  eKOI8R,        ///< KOI8-R (Cyrillic)
  eSJIS,         ///< Shift-JIS (Japanese)
  eEUCJP,        ///< EUC-JP (Japanese)
  eGB2312,       ///< GB-2312 (Chinese)
  eBig5,         ///< Big5 (Traditional chinese)

  // Windows code pages
  eCP932,        ///< Code page 932 (Japanese Shift-JIS)
  eCP936,        ///< Code page 936 (Simplified Chinese GBK, \e not GB2312)
  eCP949,        ///< Code page 949 (Korean)
  eCP950,        ///< Code page 950 (big5)
  eCP1200,       ///< Code page 1200 (Unicode BMP, UCS-2, native endian)

  // Unicode variants
  eUTF7,         ///< UTF-7 (Unicode)
  eUTF8,         ///< UTF-8 (Unicode)
  eUCS2,         ///< UCS-2 (Unicode)
  eUCS4,         ///< UCS-4 (Unicode)
  //@}

  /** @name Others
  */
  //@{
  eEncodingNative,     ///< Current locale encoding (host, user and application dependent)
  eEncodingInternal,   ///< nglString internal encoding
  eEncodingUnknown     ///< Unsupported encoding
  //@}
};


//! String conversion processor
/*!
To convert from a given encoding to another one, you have to setup a nglStringConv object.
You can then process any chunk of text data with multiple calls to Process(). Note that
converting is not a trivial job and many errors can arise. See Process() documentation for
more info.

Most of the time, you will just want to important a text using a foreign locale into a
nglString object, or the opposite, export into a foreign locale from a nglString
object. The nglString::Import() and nglString::Export() methods will provide you an easy
and convenient way to do this.
*/
class NGL_API nglStringConv : public nglError
{
public:
  static nglChar mUnknown;  ///< Default char used to mark in place a conversion error. Unused if set to '\\0'

  nglStringConv(nglTextEncoding From, nglTextEncoding To, nglChar Default = nglStringConv::mUnknown);
  /*!< Create a conversion context
    \param From source encoding
    \param To target encoding
    \param Default char to display in place of invalid or unconvertible input sequences.
           If set to '\\0', do not display a default char.
  */
  virtual ~nglStringConv();

  int Process(const char*& pSource, int& rToRead, char*& pTarget, int& rToWrite);
  /*!< Process with conversion
    \param pSource source buffer
    \param rToRead number of bytes to read from source buffer
    \param pTarget target buffer
    \param rToWrite maximum number of bytes to write to target buffer
    \return number of input errors encountered

    Proceed conversion from \p pSource to \p pTarget. Both \p rToRead and \p rToWrite
    values are updated to reflect respectively the number of bytes left to read and
    the number of bytes that can be still written. Any conversion error (either an
    illegal input sequence or an inconvertible sequence) is replaced with the \e default
    char passed to the constructor. You must check GetError() after this call to resolve
    the different issues that may arise.
  */

private:
  nglChar         mDefault;
  nglTextEncoding mFrom;
  nglTextEncoding mTo;

  class StringCodec* mpCodec;

  virtual const nglChar* OnError (uint& rError) const;
};

#endif // __nglStringConv_h__
