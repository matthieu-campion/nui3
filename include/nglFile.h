/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/


#ifndef __nglFile_h__
#define __nglFile_h__

//! File opening mode
/*!
The file opening mode is set from the File constructor invocation.
*/
enum nglFileMode
{
	eFileRead,   /*!< The file can be read but not written to.
	 The file must exist. */
	 eFileWrite,  /*!< The file can be written to (and read).
	  If the file already exists, it is erased (length truncated to 0), otherwise it is created. */
	  eFileModify, /*!< The file can be written to (and read).
   If the file already exists, its content is preserved, otherwise it is created.
   The read/write position is initially set to the beginning of the file. */
   eFileAppend  /*!< The file can be written to (and read).
	If the file already exists, its content is preserved, otherwise it is created.
	The read/write position is initially set to the end of the file. */
};

//! Seek hint
/*!
Seeking positions and directions for File::SetPos().
*/
enum nglFileWhence
{
	eFileFromStart,  ///< Move forward, from the beginning of the file (offset 0).
	eFileForward,    ///< Move forward from the current read/write position.
	eFileRewind,     ///< Move backwards from the current read/write position.
	eFileFromEnd     ///< Move backwards from the end of the file.
};


//! Portable file class
/*!
To open a file, you only need to instantiate a File object. You will need a
valid nglPath object first. You can either use the nglPath::Open method (this
can be desirable if you need file polymorphism) or the File constructor.

You can set explicitly the file endianness from the constructor. If the application
is built on a platform with a different endianness from the one you specify, proper
byte swapping will occur automatically on word access (read/write).

To close a file (needed to properly finalize write access), you must delete the
file object.

File are buffered by the system as a default : the Write* methods return immediatly
but the data is actually written later on the medium. You can force all data writes
pending on a currently opened file with the Flush() method (this will have no effect
on read-only files). You can also unbuffer the file with the AutoFlush() method :
setting auto-flush to true will make all write access immediate (ie. Write* methods
will only return when the data has been written on the medium).
*/
class NGL_API nglFile
{
public:
	/** @name Life cycle */
	//@{
	nglFile (const nglPath& rPath, nglFileMode Mode = eFileRead, bool OpenNow = true);
	/*!<
	\param rPath file's path
	\param Mode read/write mode
	\param OpenNow if true, try to open file immediately

	Open the file designated by \a rPath, using read/write \a Mode.

	You must check any errors with GetError() after calling the constructor. If
	\p OpenNow is false, you can defer file opening by calling Open() later.

	If there is an error, the object is unusable and should be destroyed.
	*/
	virtual ~nglFile();
	/*!< Close the file (and flush pending writes if any). */
	//@}

	/** @name Properties */
	//@{
	nglPath GetPath() const;
	/*!< Retrieve file's path
	\return a copy of the path passed to the constructor
	*/
	nglFileMode GetMode() const;
	/*!< Get file's read/write mode
	\return the file mode passed to the constructor
	*/
	nglEndian GetEndian() const;
	/*!< Get file's endianness convention
	\return current endianness convention
	*/
	void SetEndian(nglEndian nglEndian);
	/*!<
	\param nglEndian new endianness convention

	Set file's endianness on the fly. All further read and writes will use the
	new endianness (buffered pending writes are not affected).
	*/
	nglFileSize GetSize() const;
	/*!< Get current file size
	\return current (real time, including last writes) file size in bytes
	*/
	//@}

	/** @name Data flow */
	//@{
	bool Open();
	/*!< Open the file
	\return true if the file was succesfully opened

	Note that a file can be opened immediatly from the constructor if the \p OpenNow
	parameter is set to true.
	*/
	void Close();
	/*!< Close the file immediatly
	Note that a file is implicitly closed when its object is deleted.
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
	bool IsEOF() const;
	/*!< Check End Of File status
	\return \e true if the read/write position reached the end of the file (no more bytes to read).
	*/
	nglFileOffset GetPos() const;
	/*!< Return current read/write position
	\return current read/write position in byte units
	*/
	nglFileOffset SetPos(nglFileOffset Offset, nglFileWhence Whence = eFileFromStart);
	/*!<
	\param Offset relative offset in byte units
	\param Whence displacement origin and direction

	Set the current read/write position, in byte units. \a Offset must be positive,
	the direction can be chosen using the proper nglFileWhence item.
	*/
	void Flush();
	/*!<
	Immediately put pending writes on the storage medium (ignored for a read-only file)
	*/
	void AutoFlush(bool Auto = true);
	/*!<
	\param Auto turn on/off auto flushing

	If auto-flush is set, all write operations will return only once the data is
	effectively written on the storage medium
	*/
	//@}

	/** @name Data reading */
	//@{
	int64 Read(char*  pData, int64 Count = 1);  ///< Read \a Count 8-bit integer values, returns number of bytes effectively read
	int64 Read(uint8*  pData, int64 Count = 1);  ///< Read \a Count 8-bit integer values, returns number of bytes effectively read
	int64 Read(uint16* pData, int64 Count = 1);  ///< Read \a Count 16-bit integer values, returns number of 16-bit words effectively read
	int64 Read(uint32* pData, int64 Count = 1);  ///< Read \a Count 32-bit integer values, returns number of 32-bit words effectively read
	int64 Read(uint64* pData, int64 Count = 1);  ///< Read \a Count 64-bit integer values, returns number of 64-bit words effectively read
	int64 Read(int8*  pData, int64 Count = 1);  ///< Read \a Count 8-bit integer values, returns number of bytes effectively read
	int64 Read(int16* pData, int64 Count = 1);  ///< Read \a Count 16-bit integer values, returns number of 16-bit words effectively read
	int64 Read(int32* pData, int64 Count = 1);  ///< Read \a Count 32-bit integer values, returns number of 32-bit words effectively read
	int64 Read(int64* pData, int64 Count = 1);  ///< Read \a Count 64-bit integer values, returns number of 64-bit words effectively read
	int64 Read(float*  pData, int64 Count = 1);  ///< Read \a Count float values (IEEE 754 simple precision, 32 bits), returns number of 32-bit words effectively read
	int64 Read(double* pData, int64 Count = 1);  ///< Read \a Count double values (IEEE 754 double precision, 64 bits), returns number of 64-bit words effectively read

	bool Read(char& rData);  ///< Read 1 8-bit integer values, returns true if ok
	bool Read(uint8& rData);  ///< Read 1 8-bit integer values, returns true if ok
	bool Read(uint16& rData);  ///< Read 1 16-bit integer values, returns true if ok
	bool Read(uint32& rData);  ///< Read 1 32-bit integer values, returns true if ok
	bool Read(uint64& rData);  ///< Read 1 64-bit integer values, returns true if ok
	bool Read(int8& rData);  ///< Read 1 8-bit integer values, returns true if ok
	bool Read(int16& rData);  ///< Read 1 16-bit integer values, returns true if ok
	bool Read(int32& rData);  ///< Read 1 32-bit integer values, returns true if ok
	bool Read(int64& rData);  ///< Read 1 64-bit integer values, returns true if ok
	bool Read(float& rData);  ///< Read 1 float values (IEEE 754 simple precision, 32 bits), returns true if ok
	bool Read(double& rData);  ///< Read 1 double values (IEEE 754 double precision, 64 bits), returns true if ok

	virtual int64 Read (void* pData, int64 WordCount, uint WordSize);  ///< Read \a WordCount number of \a WordSize bytes long into \a pBuffer, returns the number of words effectively read
	//@}

	/** @name Data writing */
	//@{
	int64 Write(const char*  str);///< Write \a Count 8-bit integer values, returns number of bytes effectively written
	int64 Write(const char*  pData,  int64 Count);///< Write \a Count 8-bit integer values, returns number of bytes effectively written
	int64 Write(const uint8*  pData, int64 Count = 1);  ///< Write \a Count 8-bit integer values, returns number of bytes effectively written
	int64 Write(const uint16* pData, int64 Count = 1);  ///< Write \a Count 16-bit integer values, returns number of 16-bit words effectively written
	int64 Write(const uint32* pData, int64 Count = 1);  ///< Write \a Count 32-bit integer values, returns number of 32-bit words effectively written
	int64 Write(const uint64* pData, int64 Count = 1);  ///< Write \a Count 64-bit integer values, returns number of 64-bit words effectively written
	int64 Write(const int8*  pData, int64 Count = 1);  ///< Write \a Count 8-bit integer values, returns number of bytes effectively written
	int64 Write(const int16* pData, int64 Count = 1);  ///< Write \a Count 16-bit integer values, returns number of 16-bit words effectively written
	int64 Write(const int32* pData, int64 Count = 1);  ///< Write \a Count 32-bit integer values, returns number of 32-bit words effectively written
	int64 Write(const int64* pData, int64 Count = 1);  ///< Write \a Count 64-bit integer values, returns number of 64-bit words effectively written
	int64 Write(const float*  pData, int64 Count = 1);  ///< Write \a Count float values (IEEE 754 simple precision, 32 bits), returns number of 32-bit words effectively written
	int64 Write(const double* pData, int64 Count = 1);  ///< Write \a Count double values (IEEE 754 double precision, 64 bits), returns number of 64-bit words effectively written
	int64 Write(const nglString& str, const nglTextEncoding Encoding = eEncodingNative);  ///< Write \a nglString, returns number of 64-bit words effectively written
	int64 Write(const std::string& str);  ///< Write \a std::string, returns number of 64-bit words effectively written

	virtual int64 Write (const void* pData, int64 WordCount, uint WordSize);  ///< Write \a WordCount number of \a WordSize bytes long from \a pBuffer, returns the number of words effectively written

	bool Write(char rData);  ///< Write 1 8-bit char values, returns true if ok
	bool Write(uint8 rData);  ///< Write 1 8-bit integer values, returns true if ok
	bool Write(uint16 rData);  ///< Write 1 16-bit integer values, returns true if ok
	bool Write(uint32 rData);  ///< Write 1 32-bit integer values, returns true if ok
	bool Write(uint64 rData);  ///< Write 1 64-bit integer values, returns true if ok
	bool Write(int8 rData);  ///< Write 1 8-bit integer values, returns true if ok
	bool Write(int16 rData);  ///< Write 1 16-bit integer values, returns true if ok
	bool Write(int32 rData);  ///< Write 1 32-bit integer values, returns true if ok
	bool Write(int64 rData);  ///< Write 1 64-bit integer values, returns true if ok
	bool Write(float rData);  ///< Write 1 float values (IEEE 754 simple precision, 32 bits), returns true if ok
	bool Write(double rData);  ///< Write 1 double values (IEEE 754 double precision, 64 bits), returns true if ok
	//@}

private:
	nglPath         mPath;
	nglFileMode     mMode;
	nglEndian       mEndian;
	bool         mAutoFlush;
#if (defined _LINUX_) || (defined __APPLE__) || (defined _ANDROID_) || (defined _MINUI3_)
	int          mFD;
#endif
#ifdef WINCE
	HANDLE		mFD;
#elif defined _WIN32_
	FILE*       mFD;
#endif

	nglFile(const nglFile&) {} // Undefined copy constructor
};






#endif // __nglFile_h__
