/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

// nglChar
typedef char nglChar;
typedef wchar_t nglUChar;

class nglStringConv;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		nglTextFormat
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum nglTextFormat
{
	eTextNone,  	// no ending code
	eTextDOS,   	// \r\n (CR LF)
	eTextUnix,  	// \n   (LF)
	eTextMac,   	// \r   (CR)
	eTextZero,  	// \O

#if defined(_UNIX_) || defined(_BEOS_)
	eTextNative = eTextUnix
#endif
#ifdef WINCE
	eTextNative = eTextDOS
#elif (defined _WIN32_)
	eTextNative = eTextDOS
#elif (defined _CARBON_) || (defined _UIKIT_) || (defined _COCOA_)
	eTextNative = eTextUnix
#endif
};

/* Handy macros
*/
#ifndef YESNO
#define PLURAL(x)    (((x) > 1) ? _T("s") : _T(""))
#define YESNO(x)     ((x) ? _T("yes") : _T("no"))
#define TRUEFALSE(x) ((x) ? _T("true") : _T("false"))
#define ONOFF(x)     ((x) ? _T("on") : _T("off"))

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		nglFloatFormatFlag
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum nglFloatFormatFlag
{
	Simple		= 1,
	Scientific	= 2,
	Condensed	= 4,
	ShowSign	= 8,
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		nglTextFormat
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

typedef std::pair<nglTextEncoding, nglTextEncoding> nglEncodingPair;
typedef std::map<nglEncodingPair, nglStringConv*> nglStringConvMap;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Class nglString
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NGL_API nglString
{
public:
	static const nglChar Zero;     ///< The usual 'terminal' zero, cannot be embedded \e in a string
	static const nglChar Tab;      ///< Tabulation character (ie. _T('\\t'))
	static const nglChar NewLine;  ///< Newline character (ie. _T('\\n'))

	static nglString Null;
	static nglString Empty;
	static nglString WhiteSpace;
	/*!<
	Characters considered as white spaces (see Trim() and Tokenize())
	It is initialized with \e space, \e tab and \e newline as white spaces.
	*/

public:
	/** @name Life cycle */
	//@{
	nglString();                                 ///< Creates a null string (contains no data).
	explicit nglString(nglUChar Ch);                 ///< Creates a string with a single char. If \p nglChar is zero, the string is empty
  nglString(const nglChar* pSource);
	nglString(const nglString& rSource);            ///< Copy constructor
	nglString(const std::string& rSource, nglTextEncoding Encoding = eEncodingNative);       ///< Creates a string from a std::string

	nglString(const nglChar* pSource, nglTextEncoding Encoding);
	/*!<
	Creates a string, importing from the null-terminated \a pSource byte array,
	using the given \a Encoding. See Import() for more info.
	*/
	nglString(const nglChar* pSource, int32 Length, nglTextEncoding Encoding = eEncodingNative);
	/*!<
	Creates a string, importing no more than \p Length chars from the
	\p pSource string, using the given \p Encoding. See Import() for more info.
	*/

//	explicit nglString(int32 integer);
//	explicit nglString(uint32 integer);
//	explicit nglString(float fl, int32 precision = 8);
//	explicit nglString(double db, int32 precision = 8);

	~nglString();                            ///< Destructor. Release associated data
	//@}

	/** @name nglString size */
	//@{
	int32  GetLength() const; ///< Returns size in chars. If zero, the string is either \e null or \e empty
	int32  GetULength() const; ///< Returns size in unicode code points. Remember that GetULength() <= GetLength(). If zero, the string is either \e null or \e empty
  bool IsEmpty() const; ///< Returns whether the string contains characters or not. Null strings are considered empty.
  bool IsNull() const; ///< Returns whether the string is equal to nglString::Null.
	//@}

	/** @name Fetch string content */
	//@{
	nglChar GetChar(uint32 Index)	const;  ///< Returns the char at position \p Index. If \p Index is out of range, returns zero. See operator[]()
	nglChar GetChar(int32 Index)		const { return GetChar((uint32)Index); }  ///< Returns the char at position \p Index. If \p Index is out of range, returns zero. See operator[]()
	nglChar GetLastChar() const;        ///< Returns last char of string. Returns zero if the string is \e null or \e empty
	nglUChar GetUChar(uint32 Index)	const;  ///< Returns the char at position \p Index. If \p Index is out of range, returns zero. See operator[]()
	nglUChar GetUChar(int32 Index)		const { return GetChar((uint32)Index); }  ///< Returns the char at position \p Index. If \p Index is out of range, returns zero. See operator[]()
  nglUChar GetNextUChar(int32& Index) const;
	const nglChar* GetChars() const;           ///< Returns the string content as a char array. Returns null if the string is \e null
	std::string GetStdString(const nglTextEncoding Encoding=eUTF8) const;    ///< Returns the string content as an std::string. Return an empty string if the string is \e null.

	char* Export(const nglTextEncoding Encoding = eUTF8) const;
	/*!< Export to a given text encoding
	\param Encoding target encoding
	\return encoded string content, null if the string is \e null or an error occurred

	This simplified export interface returns a malloc'ed buffer which \e must be
	free'd by the user if non null. A valid output is always a null-terminated byte
	array.

	System functions expect char* strings to be encoded using the current locale's encoding
	("" empty string). Example :
	\code
	bool SetEnv(nglString& rName, nglString& rValue, bool Overwrite)
	{
	char* name = rName.Export();
	char* value = rValue.Export();
	int ret = -1;

	if (name && value)
	ret = setenv(name, value, Overwrite ? 1 : 0);

	if (value) free(value);
	if (name) free(name);

	return (ret == 0);
	}
	\endcode
	*/
  int32 Export(int32& rOffset, char* pBuffer, int32& rToWrite, const nglTextEncoding Encoding = eEncodingNative) const;
  /*!< Export to a given text encoding
  \param rOffset starting offset in string
  \param pBuffer target buffer
  \param rToWrite number of bytes which can be written at \p pBuffer address
  \param Encoding target encoding
  \return number of conversion errors encountered, or -1 if a fatal error occured

  This method let you provide your own target buffer. This method will write no more than \p rAvailable
  bytes into the \p pBuffer byte array, and will also update \p rOffset and \p rToWrite accordingly to
  reflect the number of bytes effectively read and written.
  This does not add a \0 pointer at the end of the array.
  */
  int32 Export(int32& rOffset, char* pBuffer, int32& rToWrite, nglStringConv& rConv) const;
  /*!< Export to a given text encoding
  \param rOffset starting offset in string
  \param pBuffer target buffer
  \param rToWrite number of bytes which can be written at \p pBuffer address
  \param rConv conversion context
  \return number of conversion errors encountered, or -1 if a fatal error occurred

  Advanced export : this method let you provide your own conversion context (\p rConv)
  and re-use it across multiple calls of Export(). This is useful to optimize
  chunk oriented conversions of text streams. It is used in the implementation of
  OStream.
  This does not add a \0 pointer at the end of the array.
  */

	nglString Extract(uint32 Index) const;
	nglString Extract(int32 Index) const
	{
    return Extract((uint32)Index);
  }
	
  /*!< Fetch a substring
	\param Index start of substring
	\return substring starting from \p Index till end of string.
	Returns a null string if \p Index is negative or below last char
	*/
	nglString Extract(int32 Index, int32 Length) const;
	/*!< Fetch a substring
	\param Index start of substring
	\param Length length in chars of substring
	\return \p Length chars from position \p Index.
	Returns a null string if \p Index is negative or below last char or
	if \p Length is negative. \p Length is automatically clamped.
	*/
	nglString      GetLeft(int32 Count) const;  ///< Returns \p Count leftmost chars
	nglString      GetRight(int32 Count) const;  ///< Returns \p Count rightmost chars

	int32   GetInt   (int Base = 10) const { return (int32) GetInt64 (Base); }  ///< Parse the string as an integer written in base \p Base
	uint32  GetUInt  (int Base = 10) const { return (uint32)GetUInt64(Base); }  ///< Parse the string as an unsigned integer written in base \p Base
	int64          GetInt64 (int Base = 10) const;  ///< Parse the string as an integer written in base \p Base
	uint64         GetUInt64(int Base = 10) const;  ///< Parse the string as an unsigned integer written in base \p Base
	float          GetFloat () const;               ///< Parse the string as a float decimal representation
	double         GetDouble() const;               ///< Parse the string as a double decimal representation

  bool IsInt() const;
  bool IsFloat() const;
  
  
	int32   GetCInt   (int Base = 10) const { return (int32) GetCInt64(Base); } /*!< Parse the string as an integer written in base \p Base, using the 'C' locale (american numeric conventions) */
	uint32  GetCUInt  (int Base = 10) const { return (uint32)GetCUInt64(Base); } /*!< Parse the string as an unsigned integer written in base \p Base, using the 'C' locale (american numeric conventions) */
	int64          GetCInt64 (int Base = 10) const; /*!< Parse the string as an integer written in base \p Base, using the 'C' locale (american numeric conventions) */
	uint64         GetCUInt64(int Base = 10) const; /*!< Parse the string as an unsigned integer written in base \p Base, using the 'C' locale (american numeric conventions) */
	float          GetCFloat () const;              /*!< Parse the string as a float decimal representation, using the 'C' locale (american numeric conventions) */
	double         GetCDouble() const;              /*!< Parse the string as a float decimal representation, using the 'C' locale (american numeric conventions) */
	//@}

	/** @name Add/modify string content */
	//@{
	bool SetChar (nglUChar Char, int32 Index);
	/*!< Set char value \p nglChar at position \p Index
	\return false if \p Index is out of range

	A zero char can be inserted, although it is a funny hack (use Delete() instead).
	*/

	bool Fill (nglUChar Pattern, int32 RepeatCount);            ///< Fills string with \p RepeatCount chars of value \p Pattern
	bool Fill (const nglChar* pPattern, int32 RepeatCount);    ///< Fills string with \p RepeatCount occurrences of the null-terminated \p pPattern string
	bool Fill (const nglString& rPattern, int32 RepeatCount);  ///< Fills string with \p RepeatCount occurrences of \p rString

  int32 Import (const char* pBuffer, int32 ByteCount, const nglTextEncoding Encoding);
  int32 Import (int32& rOffset, const char* pBuffer, int32& rToRead, const nglTextEncoding Encoding);
	int32 Import (const char* pBuffer, const nglTextEncoding Encoding = eEncodingNative);
	/*!< Import text from a given encoding
	\param pBuffer null-terminated string
	\param Encoding source string encoding
	\return number of conversion errors encountered, or -1 if a fatal error occurred

	System functions returning char* strings use the locale's current encoding.
	Example :
	\code
	nglString Home;
	Home.Import(getenv("HOME"));
	\endcode
	*/

  int32 Import(int32& rOffset, const char* pBuffer, int32& rToRead, nglStringConv& rConv);


	bool Copy   (nglChar Char);              ///< Set current string to one char which value is \p nglChar
	bool Copy   (const nglChar* pSource);
	bool Copy   (const nglChar* pSource, int32 len);
	/*!< Copy \a pSource content into string
	\param pSource null-terminated string
	*/
	bool Copy   (const nglString& rSource);  ///< Copy \p rSource content into string

	bool Insert(nglChar Char, int32 Index);
	/*!< Insert \p nglChar at position \p Index
	\return false if \p Index is out of range
	*/
	bool Insert(const nglChar* pSource, int32 Index);
	/*!< Insert the null-terminated \p pSource string at position \p Index
	\return false if \p pSource is null or \p Index is out of range
	*/
	bool Insert(const nglString& rSource, int32 Index);
	/*!< Insert \p rSource content at position \p Index
	\return false if \p rSource is a null string or \p Index is out of range
	*/

	nglString& Append(nglUChar Char);              ///< Append \p nglChar to string
	nglString& Append(const nglChar* pSource);    ///< Append the null-terminated \p pSource string
	nglString& Append(const nglString& rSource);  ///< Append \p rSource content

	nglString& Prepend(nglUChar nglChar);              ///< Prepend \p nglChar to string
	nglString& Prepend(const nglChar* pSource);    ///< Prepend the null-terminated \p pSource string
	nglString& Prepend(const nglString& rSource);  ///< Prepend \p rSource content

	void SetCInt   (int64 Value, int Base = 10);  ///< Set representation of \p Value in base \p Base
	void SetCUInt  (uint64 Value, int Base = 10); ///< Set representation of \p Value in base \p Base
	void SetCFloat (float Value, int32 precision = 8, nglFloatFormatFlag flags = Simple);  ///< Set representation of \p Value
	void SetCDouble(double Value, int32 precision = 8, nglFloatFormatFlag flags = Simple); ///< Set representation of \p Value
	//@}

	/** @name Remove string content */
	//@{
	void Nullify();  ///< Set to null string
	bool Wipe();     ///< Empty string (ie. set to "").

	bool Delete     (int32 Index = 0);          ///< Delete all chars from position \p Index
	bool Delete     (int32 Index, int32 Length);  ///< Delete \p Length chars from position \p Index
	bool DeleteLeft (int32 Count);              ///< Delete \p Count leftmost chars
	bool DeleteRight(int32 Count);              ///< Delete \p Count rightmost chars

	bool Trim();                           ///< Remove white spaces from both ends (see WhiteSpace)
	bool Trim(nglUChar Char);                 ///< Remove trailing chars \p nglChar from both ends
	bool Trim(const nglChar* pTrimSet);      ///< Remove chars from the null-termianted \p pTrimSet string from both ends
	bool Trim(const nglString& rTrimSet);    ///< Remove chars from the \p rTrimSet string from both ends

	bool TrimLeft();                             ///< Remove leftmost white spaces (see WhiteSpace)
	bool TrimLeft(nglUChar Char);                ///< Remove leftmost trailing chars \p nglChar
	bool TrimLeft(const nglChar* pTrimSet);     ///< Remove leftmost chars from the null-termianted \p pTrimSet string
	bool TrimLeft(const nglString& rTrimSet);   ///< Remove leftmost chars from the \p rTrimSet string

	bool TrimRight();                            ///< Remove rightmost white spaces (see WhiteSpace)
	bool TrimRight(nglUChar Char);               ///< Remove rightmost trailing chars \p nglChar
	bool TrimRight(const nglChar* pTrimSet);    ///< Remove rightmost chars from the null-termianted \p pTrimSet string
	bool TrimRight(const nglString& rTrimSet);  ///< Remove rightmost chars from the \p rTrimSet string
	//@}

	/** @name Edit string content */
	//@{
	bool Replace(int32 Index, int32 Length, nglChar Char);
	/*!< Replace string portion of length \p Length starting from position \p Index with char \p nglChar
   \return false if \p Index or \p Length are out of range
   */
	bool Replace(int32 Index, int32 Length, const nglChar* pSource);
	/*!< Replace string portion of length \p Length starting from position \p Index with the null-terminated \p pSource string
	\return false if \p pSource is null or \p Index or \p Length are out of range
	*/
	bool Replace(int32 Index, int32 Length, const nglString& rSource);
	/*!< Replace string portion of length \p Length starting from position \p Index with \p rSource
	\return false if \p rSource is a null string or \p Index or \p Length are out of range
	*/

	bool Replace(const nglChar Old, const nglChar New);
	/*!< Replace occurrences of char \p Old with \p New
   */
	bool Replace(const nglChar* pOld, const nglChar* pNew);
	/*!< Replace occurrences of the null-terminated \p pOld string with the null-terminated \p pNew string
	\return false if \p pOld or \p pNew are null
	*/
	bool Replace(const nglString& rOld, const nglString& rNew);
	/*!< Replace occurrences of \p rOld with \p rNew
	\return false if \p rOld or \p rNew are null strings
	*/

	nglString& ToUpper();                       ///< Convert all lower case chars to upper case
	nglString& ToUpper(int32 Index, int32 Length);  ///< Convert \p Length lower case chars from position \p Index to upper case
	nglString& ToLower();                       ///< Convert all upper case chars to lower case
	nglString& ToLower(int32 Index, int32 Length);  ///< Convert \p Length upper case chars from position \p Index to lower case
  
  nglChar* EncodeUrl(); ///< outputs a malloc'ed char* encoded as valid URL data in UTF8 as per RFC 2396
  void DecodeUrl();
  void EncodeToXML();
  void DecodeFromXML();
  void Unescape();
  
  void ToCanonicalComposition();
  void ToCanonicalDecomposition();
  
	nglString& Format(const nglString& rFormat, ...);
	nglString& Format(const nglChar* pFormat, ...);
	/*!< Format a string from arguments a la printf
	Please refer to your favorite printf's reference. Make sure you are using
	POSIX or ISOC99 format tags if you want to keep your program portable.

	<b>Important</b> : the %%s tag refers to a Unicode string on \e all platforms
	(ie. _T("hello") or mystring.GetChars()).
	*/
	nglString& Formatv(const nglString& rFormat, va_list Args);
	nglString& Formatv(const nglChar* pFormat, va_list Args);
	/*!< Format a string from arguments a la printf
	This is the vararg version of Format()
	*/

	nglString& CFormat(const nglString& rFormat, ...);
	nglString& CFormat(const nglChar* pFormat, ...);
	/*!< Format a string from arguments a la printf, using the 'C' locale (American numeric conventions) */
	nglString& CFormatv(const nglString& pFormat, va_list Args);
	nglString& CFormatv(const nglChar* pFormat, va_list Args);
	/*!< Format a string from arguments a la printf, using the 'C' locale (American numeric conventions) */

	nglString& Add(nglUChar s);
	nglString& Add(int8 s, int base = 10);
	nglString& Add(uint8 s, int base = 10);
	nglString& Add(int16 s, int base = 10);
	nglString& Add(uint16 s, int base = 10);
	nglString& Add(int32 s, int base = 10);
	nglString& Add(uint32 s, int base = 10);
	nglString& Add(int64 s, int base = 10);
	nglString& Add(uint64 s, int base = 10);
	nglString& Add(float s, int32 precision = 8, nglFloatFormatFlag flags = Simple);
	nglString& Add(double s, int32 precision = 8, nglFloatFormatFlag flags = Simple);
	nglString& Add(const nglString& rString, int32 count = 1);
  nglString& Add(const nglChar* pString, int32 count = 1);
	nglString& Add(void* pVoidPointer);
	nglString& AddNewLine();

  nglString& AddFormat(const nglChar* pString, ...);
  nglString& AddFormat(const nglChar* pString, va_list Args);

  int32 HexDump(const nglChar* pBuffer, int32 ByteCount, bool PrintChar = true, int32 Columns = 16);
	/*!< Output an hexadecimal byte dump
	\param pBuffer source buffer
	\param ByteCount size of buffer in bytes
	\param PrintChar if true, also display bytes as chars when printable
	\param Columns number of columns per line (one column is one displayed byte)
	\return number of lines

	This method format a byte oriented hexadecimal dump of a given buffer. An example of
	the first 50 bytes of a PNG file as formated by this method with default \p PrintChar
	and \p Columns :
	\code
	00000000  89 50 4e 47 0d 0a 1a 0a  00 00 00 0d 49 48 44 52  |.PNG........IHDR|
	00000010  00 00 00 5a 00 00 00 af  08 02 00 00 00 f2 95 5e  |...Z...Ø.....Ú.^|
	00000020  ff 00 00 20 00 49 44 41  54 78 da 64 bc 69 94 5d  |... .IDATx/dºi.]|
	00000030  d5 75                                             |u              |
	\endcode
	*/
	//@}

	/** @name nglString comparison */
	//@{
	int32 Compare(const nglChar* pSource, bool CaseSensitive = true) const;
	/*!< Compares string with \p pSource as right operand
	\param pSource null-terminated string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string is equal, 'less' or 'greater' than \p pSource
	*/
	int32 Compare(const nglString& rSource, bool CaseSensitive = true) const;
	/*!< Compares string with \p rSource as right operand
	\param rSource right operand string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string is equal, 'less' or 'greater' than \p pSource
	*/

	int32 Compare(const nglChar* pSource, int32 Index, int32 Length, bool CaseSensitive = true) const;
	/*!< Compares string portion with \p pSource as right operand
	\param pSource null-terminated string
	\param Index start position of left operand in string
	\param Length length of left operand in string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string portion is equal, 'less' or 'greater' than \p pSource
	*/
	int32 Compare(const nglString& rSource, int32 Index, int32 Length, bool CaseSensitive = true) const;
	/*!< Compares string portion with \p pSource as right operand
	\param rSource right operand string
	\param Index start position of left operand in string
	\param Length length of left operand in string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string portion is equal, 'less' or 'greater' than \p pSource
	*/
	int32 CompareNatural(const nglChar* pSource, bool CaseSensitive = true) const;
	/*!< Compares string with \p pSource as right operand using a human friendly algorithm for mixed number and characters
	\param pSource null-terminated string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string is equal, 'less' or 'greater' than \p pSource
	*/
	int32 CompareNatural(const nglString& rSource, bool CaseSensitive = true) const;
	/*!< Compares string with \p rSource as right operand using a human friendly algorithm for mixed number and characters
	\param rSource right operand string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string is equal, 'less' or 'greater' than \p pSource
	*/

	int32 CompareLeft(const nglChar* pSource, bool CaseSensitive = true) const;    ///< Compares the left portion of this with pSource. Is pSource is longer than this the comparison fails.
	/*!< Compares left string portion with \p pSource as right operand. Up to strlen(pSource) chars are compared. Comparisons starts from the left of the strings.
	\param pSource null-terminated string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string portion is equal, 'less' or 'greater' than \p pSource
	*/
	int32 CompareLeft(const nglString& rSource, bool CaseSensitive = true) const;  ///< Compares the left portion of this with rSource. Is rSource is longer than this the comparison fails.
	/*!< Compares left string portion with \p rSource as right operand. Up to rSource.GetLength() chars are compared. Comparisons starts from the left of the strings.
	\param pSource null-terminated string
	\param CaseSensitive if false perform a case insensitive comparison
	\return 0, -1 or 1 respectively if the string portion is equal, 'less' or 'greater' than \p pSource
	*/
	//@}
  
  uint32 GetLevenshteinDistance(const nglString& rSource, bool CaseSensitive);
  /*!< Returns the Levenshtein distance in between two strings.
      This metric is very useful to create smart searching UIs.
      Excerpt form the wikipedia entry about this algorithm:
      "The Levenshtein distance between two strings is given by the minimum number of operations needed to transform one string into the other,
      where an operation is an insertion, deletion, or substitution of a single character."
      For more information consult http://en.wikipedia.org/wiki/Levenshtein_distance
   */

  
  void EncodeBase64(const uint8* bytes_to_encode, unsigned int in_len);
  /*!< Encode the given binary buffer into a base64 string.
   */
  void EncodeBase64(const std::vector<uint8>& rBytes);
  /*!< Encode the given vector of bytes into a base64 string.
   */
  void DecodeBase64(std::vector<uint8>& rDecoded) const;
  /*!< Decode this base64 string into the given vector of bytes.
   */
              
  
	/** @name Substring search */
	//@{
	int32 Find(nglChar Char, int32 Index, int32 End,  bool CaseSensitive = true) const;
	/*!< Search the first occurrence of char \p nglChar from position \p Index to position \p End
	\param nglChar occurrence to find
	\param Index start search from this position
	\param End end search at this position
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/
	int32 Find(nglChar Char, int32 Index = 0, bool CaseSensitive = true) const;
	/*!< Search the first occurrence of char \p nglChar from position \p Index
	\param nglChar occurrence to find
	\param Index start search from this position
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/
	int32 Find(const nglChar* pSource, int32 Index = 0, bool CaseSensitive = true) const;
	/*!< Search the first occurrence of then null-terminated \p pSource string from position \p Index
	\param pSource occurrence to find (null-terminated string)
	\param Index start search from this position
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/
	int32 Find(const nglString& rSource, int32 Index = 0, bool CaseSensitive = true) const;
	/*!< Search the first occurrence of \p rSource from position \p Index
	\param rSource occurrence to find
	\param Index start search from this position
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/

	int32 FindLast(nglChar Char, int32 Index = -1, bool CaseSensitive = true) const;
	/*!< Search the last occurrence of char \p nglChar
	\param nglChar occurrence to find
	\param Index search start index. If negative, start search from GetLength() - Index.
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/
	int32 FindLast(const nglChar* pSource, int32 Index = -1, bool CaseSensitive = true) const;
	/*!< Search the last occurrence of then null-terminated \p pSource string
	\param pSource occurrence to find (null-terminated string)
	\param Index search start index. If negative, start search from GetLength() - Index.
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/
	int32 FindLast(const nglString& rSource, int32 Index = -1, bool CaseSensitive = true) const;
	/*!< Search the last occurrence of \p rSource
	\param rSource occurrence to find
	\param Index search start index. If negative, start search from GetLength() - Index.
	\param CaseSensitive if false perform a case insensitive comparison
	\return the position of the first occurrence, -1 if there is none
	*/

	int32 Contains(nglChar Char, int32 Index, int32 End, bool CaseSensitive = true) const; ///< Return the number of occurrences of \p nglChar
	int32 Contains(nglChar Char, bool CaseSensitive = true) const;              ///< Return the number of occurrences of \p nglChar
	int32 Contains(const nglChar* pSource, bool CaseSensitive = true) const;    ///< Return the number of occurrences of the null-terminated \p pSource string
	int32 Contains(const nglString& rSource, bool CaseSensitive = true) const;  ///< Return the number of occurrences of \p rSource
	//@}

	/** @name Simple tokenizer */
	//@{
	int32 Tokenize(std::vector<nglString>& rTokens, bool CreateEmptyTokens = false) const;
	/*!< Split a string into tokens, using WhiteSpace as separators
	\param rTokens tokens are appended to this list
	\return number of separators found
	*/
	int32 Tokenize(std::vector<nglString>& rTokens, nglChar Separator, bool CreateEmptyTokens = false) const;
	/*!< Split a string into tokens using \p Separator as separator
	\param rTokens tokens are appended to this list
	\param Separator separator char
	\return number of separators found
	*/
	int32 Tokenize(std::vector<nglString>& rTokens, const nglChar* pSeparators, bool CreateEmptyTokens = false) const;
	/*!< Split a string into tokens using chars from the null-terminated \p pSeparators string as separator
	\param rTokens tokens are appended to this list
	\param pSeparators separators set
	\return number of separators found
	*/
	int32 Tokenize(std::vector<nglString>& rTokens, const nglString& rSeparators, bool CreateEmptyTokens = false) const;
	/*!< Split a string into tokens using chars \p rSeparators as separator
	\param rTokens tokens are appended to this list
	\param rSeparators separators set
	\return number of separators found
	*/
	/*
	int32 Tokenize (std::vector <nglString>& rTokens, const nglChar** pSeparatorSet) const;
	int32 Tokenize (std::vector <nglString>& rTokens, const nglString** pSeparatorSet) const;
	*/
	//@}

	/** @name STL functors */
	//@{
  template <bool CaseSensitive>
  class LessFunctorBase : public std::binary_function <nglString, nglString, bool> 
  {
  public:
		/*! Comparison functor for STL containers
     \param rLeft left operand
     \param rRight right operand
     \return result of rLeft < rRight
     
     This functor can be used when nglString is involved in a STL container
     or algorithm. Example :
     
     \code
     std::map<nglString, int, nglString::LessFunctor> AgeOfPeople;
     nglString someone("John Doe");
     int age;
     
     AgeOfPeople[someone] = 33;
     age = AgeOfPeople[someone];
     \endcode
     */
    bool operator()(const nglString& rLeft, const nglString& rRight) const
    {
      return rLeft.Compare(rRight, CaseSensitive) < 0;
    }
  };  
  
	typedef LessFunctorBase<true> LessFunctor;
	typedef LessFunctorBase<false> CaseInsensitiveLessFunctor;

	class NaturalLessFunctor : public std::binary_function <nglString, nglString, bool> 
	{
	public:
		/*! Human friendly Comparison functor for STL containers
		\param rLeft left operand
		\param rRight right operand
		\return result of rLeft < rRight

		This functor can be used when nglString is involved in a STL container
		or algorithm. Example :

		\code
		std::map<nglString, int, nglString::NaturalLessFunctor> AgeOfPeople;
		nglString someone("John Doe");
		int age;

		AgeOfPeople[someone] = 33;
		age = AgeOfPeople[someone];
		\endcode
		*/
		bool operator()(const nglString& rLeft, const nglString& rRight) const
		{
			return rLeft.CompareNatural(rRight) < 0;
		}
	};
  //@}

	/** @name Operators */
	//@{
	// Assignment
	const nglString& operator=(const nglUChar Char);
	const nglString& operator=(const nglChar* pSource);
	const nglString& operator=(const nglString& rSource);

	// Appending
	const nglString& operator+=(const nglUChar Char);
	const nglString& operator+=(const nglChar* pSource);
	const nglString& operator+=(const nglString& rSource);

	// Concatenation
	friend nglString operator+(const nglString& rLeft, const nglUChar Right);
	friend nglString operator+(const nglString& rLeft, const nglChar* pRight);
	friend nglString operator+(const nglString& rLeft, const nglString& rRight);
	friend nglString operator+(const nglUChar Left, const nglString& rRight);
	friend nglString operator+(const nglChar* pLeft, const nglString& rRight);

	// Comparison (equal)
	friend bool operator==(const nglString& rLeft, const nglUChar Right);
	friend bool operator==(const nglString& rLeft, const nglChar* pRight);
	friend bool operator==(const nglString& rLeft, const nglString& rRight);
	friend bool operator==(const nglUChar Left, const nglString& rRight);
	friend bool operator==(const nglChar* pLeft, const nglString& rRight);

	// Comparison (different)
	friend bool operator!=(const nglString& rLeft, const nglUChar Right);
	friend bool operator!=(const nglString& rLeft, const nglChar* pRight);
	friend bool operator!=(const nglString& rLeft, const nglString& rRight);
	friend bool operator!=(const nglUChar Left, const nglString& rRight);
	friend bool operator!=(const nglChar* pLeft, const nglString& rRight);

	// Comparison (less than)
	friend bool operator<(const nglString& rLeft, const nglUChar Right);
	friend bool operator<(const nglString& rLeft, const nglChar* pRight);
	friend bool operator<(const nglString& rLeft, const nglString& rRight);
	friend bool operator<(const nglUChar Left, const nglString& rRight);
	friend bool operator<(const nglChar* pLeft, const nglString& rRight);

	// Comparison (less or equal)
	friend bool operator<=(const nglString& rLeft, const nglUChar Right);
	friend bool operator<=(const nglString& rLeft, const nglChar* pRight);
	friend bool operator<=(const nglString& rLeft, const nglString& rRight);
	friend bool operator<=(const nglUChar Left, const nglString& rRight);
	friend bool operator<=(const nglChar* pLeft, const nglString& rRight);

	// Comparison (more than)
	friend bool operator>(const nglString& rLeft, const nglUChar Right);
	friend bool operator>(const nglString& rLeft, const nglChar* pRight);
	friend bool operator>(const nglString& rLeft, const nglString& rRight);
	friend bool operator>(const nglUChar Left, const nglString& rRight);
	friend bool operator>(const nglChar* pLeft, const nglString& rRight);

	// Comparison (more or equal)
	friend bool operator>=(const nglString& rLeft, const nglUChar Right);
	friend bool operator>=(const nglString& rLeft, const nglChar* pRight);
	friend bool operator>=(const nglString& rLeft, const nglString& rRight);
	friend bool operator>=(const nglUChar Left, const nglString& rRight);
	friend bool operator>=(const nglChar* pLeft, const nglString& rRight);

	// Array access
	nglChar  operator[](uint32 Index) const;
	nglChar& operator[](uint32 Index);
	nglChar  operator[](int32 Index) const;
	nglChar& operator[](int32 Index);

	// Type cast
	//operator const nglChar*() const;
	//@}
  static nglStringConv* GetStringConv(const nglEncodingPair& rEncodings);
  
  #if (defined _CARBON_) || (defined _UIKIT_) || (defined _COCOA_)
  CFStringRef ToCFString() const; ///< return a Core Foundation string reference (don't forget to release the CFString using CFRelease)
  nglString(CFStringRef ref); ///< create an nglString from a Core Foundation string reference (don't forget to release the CFString using CFRelease)
  #endif

private:
	std::string	mString;
  bool mIsNull;
  
  static nglStringConvMap gStringConvCache;
  static void ReleaseStringConvs();
  friend class nglKernel;
  static std::set<char> gURLDontEncodeChars;
};


/*
* Inline code
*/

bool nglIsDigit(nglUChar a);
bool nglIsHexDigit(nglUChar a);
bool nglIsAlpha(nglUChar a);
bool nglIsAlphaNum(nglUChar a);

bool nglIsSpace(nglUChar a);

bool nglCompareStrings(const nglString& rLeft, const nglString& rRight);
bool nglCompareStringsNoCase(const nglString& rLeft, const nglString& rRight);
bool nglCompareNaturalStrings(const nglString& rLeft, const nglString& rRight);
bool nglCompareNaturalStringsNoCase(const nglString& rLeft, const nglString& rRight);


//! nglString conversion processor
/*!
To convert from a given encoding to another one, you have to setup a nglnglStringConv object.
You can then process any chunk of text data with multiple calls to Process(). Note that
converting is not a trivial job and many errors can arise. See Process() documentation for
more info.

Most of the time, you will just want to important a text using a foreign locale into a
nglString object, or the opposite, export into a foreign locale from a nglString
object. The nglString::Import() and nglString::Export() methods will provide you an easy
and convenient way to do this.
*/

enum nglStringConvState
{
  eStringConv_OK = 0,  ///< Conversion successfully completed
  eStringConv_RangeError,  ///< Parameter out of range
  eStringConv_InvalidConversion, ///< Unsupported conversion
  eStringConv_NeedInput,  ///< Conversion incomplete (input bytes left represent an incomplete sequence)
  eStringConv_NeedOutput ///< Conversion incomplete (insufficient output buffer)
};

class NGL_API nglStringConv
{
public:
  static nglChar mUnknown;  ///< Default char used to mark in place a conversion error. Unused if set to '\\0'

  nglStringConv(nglTextEncoding From, nglTextEncoding To, nglUChar Default = nglStringConv::mUnknown);
  /*!< Create a conversion context
  \param From source encoding
  \param To target encoding
  \param Default char to display in place of invalid or unconvertible input sequences.
  If set to '\\0', do not display a default char.
  */
  virtual ~nglStringConv();

  virtual int32 Process(const char*& pSource, int32& rToRead, char*& pTarget, int32& rToWrite);
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

  nglStringConvState GetState()
  {
    return mState;
  }

protected:
  nglStringConv(const nglTextEncoding From, const nglTextEncoding To, nglUChar Default, bool NoInit);
  
  nglUChar         mDefault;
  nglTextEncoding mFrom;
  nglTextEncoding mTo;

  class nglStringCodec* mpCodec;
  nglStringConvState mState;
};
