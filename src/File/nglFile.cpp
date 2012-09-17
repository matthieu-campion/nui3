/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

using namespace std;

// Defines & includes
#if (defined _LINUX_) || (defined __APPLE__) || (defined _MINUI3_)
	#define		ngl_fstat(fd,buffer)		_fstat64( (fd), (buffer))
#endif

#ifdef WINCE
	// none
#elif defined _WIN32_
	#define		ngl_fstat(fd,buffer)					_fstat64( (fd), (buffer))
	#define		ngl_ftell(fd)							_ftelli64( (fd) )
	#define		ngl_fseek(fd,off,flag)				_fseeki64( (fd), (off), (flag))
	#define		ngl_fclose(fd)						fclose( (fd) )
	#define		ngl_fflush(fd)						fflush( (fd) )
	#define		ngl_fread(buffer,size,count,fd)		fread( (buffer),(size),(count),(fd) )
	#define		ngl_fprintf(fd,buffer)				fprintf( (fd),(buffer) )
	#define		ngl_fwrite(buffer,size,count,fd)		fwrite( (buffer),(size),(count),(fd) )
#endif


/* These errors are shared by all platform-specific implementations
*/
//FIXME this is not used
#if 0
static const nglChar* gpFileErrorTable[] =
{
	/*  0 */ _T("No error"),
	/*  1 */ _T("nglPath is a folder"),
	/*  2 */ _T("Access permission denied"),
	/*  3 */ _T("nglPath name too long"),
	/*  4 */ _T("Invalid or non-existent path"),
	/*  5 */ _T("Write access denied (read-only file system)"),
	/*  6 */ _T("Not enough space on device to create a new file"),
	/*  7 */ _T("Process open file maximum reached"),
	/*  8 */ _T("System open file maximum reached"),
	NULL
};
#endif

#ifdef _DEBUG_
const nglChar* File_mode(nglFileMode mode)
{
	switch (mode)
	{
	case eFileRead  : return _T("R"); break;
	case eFileWrite : return _T("R/W (trunc.)"); break;
	case eFileModify: return _T("R/W (modify)"); break;
	case eFileAppend: return _T("R/W (append)"); break;
	}
	return _T("?");
}

const nglChar* File_endian(nglEndian endian)
{
	switch (endian)
	{
	case eEndianLittle: return _T("LE/Intel"); break;
	case eEndianBig   : return _T("BE/Motorola"); break;
	case eEndianPDP   : return _T("PDP"); break;
	}
	return _T("?");
}
#endif


/*
* Life cycle
*/

nglFile::nglFile (const nglPath& rPath, nglFileMode Mode, bool OpenNow)
{
	mPath      = rPath;
	mMode      = Mode;
	mEndian    = eEndianNative;
	mAutoFlush = false;
#if (defined _LINUX_) || (defined __APPLE__) || (defined _ANDROID_) || (defined _MINUI3_)
	mFD        = -1;
#endif
#ifdef WINCE
	mFD = INVALID_HANDLE_VALUE;
#elif defined _WIN32_
	mFD = NULL;
#endif

	if (OpenNow)
		Open();
}

nglFile::~nglFile()
{
  bool closeit = true;
#if (defined _LINUX_) || (defined __APPLE__) || (defined _ANDROID_) || (defined _MINUI3_)
	closeit = (mFD != -1);
#endif
#ifdef WINCE
  closeit = (mFD != INVALID_HANDLE_VALUE);
#elif defined _WIN32_
  closeit = (mFD != NULL);
#endif

  if (closeit)
    Close();
}


/*
* Properties
*/

nglPath nglFile::GetPath() const
{
	return mPath;
}

nglFileMode nglFile::GetMode() const
{
	return mMode;
}

nglEndian nglFile::GetEndian() const
{
	return mEndian;
}

void nglFile::SetEndian (nglEndian nglEndian)
{
	mEndian = nglEndian;
}


/*
* Data flow
*/

void nglFile::AutoFlush(bool Auto)
{
	mAutoFlush = Auto;
}


/*
* Data reading
*/

int64 nglFile::Read(char*  pData, int64 Count)
{
	return Read (pData, Count, sizeof(char));
}

int64 nglFile::Read(uint8*  pData, int64 Count)
{
	return Read (pData, Count, sizeof(uint8));
}

int64 nglFile::Read(uint16* pData, int64 Count)
{
	return Read (pData, Count, sizeof(uint16));
}

int64 nglFile::Read(uint32* pData, int64 Count)
{
	return Read (pData, Count, sizeof(uint32));
}

int64 nglFile::Read(uint64* pData, int64 Count)
{
	return Read (pData, Count, sizeof(uint64));
}

int64 nglFile::Read(int8*  pData, int64 Count)
{
	return Read (pData, Count, sizeof(int8));
}

int64 nglFile::Read(int16* pData, int64 Count)
{
	return Read (pData, Count, sizeof(int16));
}

int64 nglFile::Read(int32* pData, int64 Count)
{
	return Read (pData, Count, sizeof(int32));
}

int64 nglFile::Read(int64* pData, int64 Count)
{
	return Read (pData, Count, sizeof(int64));
}

int64 nglFile::Read(float*  pData, int64 Count)
{
	return Read (pData, Count, sizeof(float));
}

int64 nglFile::Read(double* pData, int64 Count)
{
	return Read (pData, Count, sizeof(double));
}


/*
* Data reading
*/

///////////////////////////////////////////////////////
bool nglFile::Read(char& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(uint8& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(uint16& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(uint32& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(uint64& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(int8& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(int16& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(int32& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(int64& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(float& rData)
{
	return Read(&rData, 1) == 1;
}

bool nglFile::Read(double& rData)
{
	return Read(&rData, 1) == 1;
}

///////////////////////////////////////////////////////////////////////////////////////

/*
* Data writing
*/

int64 nglFile::Write(const char*  pData, int64 Count)
{
	return Write (pData, Count, sizeof(char));
}

int64 nglFile::Write(const uint8*  pData, int64 Count)
{
	return Write (pData, Count, sizeof(uint8));
}

int64 nglFile::Write(const uint16* pData, int64 Count)
{
	return Write (pData, Count, sizeof(uint16));
}

int64 nglFile::Write(const uint32* pData, int64 Count)
{
	return Write (pData, Count, sizeof(uint32));
}

int64 nglFile::Write(const uint64* pData, int64 Count)
{
	return Write (pData, Count, sizeof(uint64));
}

int64 nglFile::Write(const int8*  pData, int64 Count)
{
	return Write (pData, Count, sizeof(int8));
}

int64 nglFile::Write(const int16* pData, int64 Count)
{
	return Write (pData, Count, sizeof(int16));
}

int64 nglFile::Write(const int32* pData, int64 Count)
{
	return Write (pData, Count, sizeof(int32));
}

int64 nglFile::Write(const int64* pData, int64 Count)
{
	return Write (pData, Count, sizeof(int64));
}

int64 nglFile::Write(const float*  pData, int64 Count)
{
	return Write (pData, Count, sizeof(float));
}

int64 nglFile::Write(const double* pData, int64 Count)
{
	return Write (pData, Count, sizeof(double));
}

int64 nglFile::Write(const nglString& str, const nglTextEncoding Encoding)
{
	char* pTemp = str.Export(Encoding);
	std::string		tmp(pTemp);
	delete pTemp;
	return Write (tmp.c_str(), tmp.size(), sizeof(char));
}

int64 nglFile::Write(const std::string& str)
{
	return Write (str.c_str(), str.size(), sizeof(char));
}

int64 nglFile::Write(const char* str)
{
	return Write (str, strlen(str), sizeof(char));
}

///////////////////////////////////////////////////////////////////////////////////////////
bool nglFile::Write(char rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(uint8 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(uint16 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(uint32 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(uint64 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(int8 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(int16 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(int32 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(int64 rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(float rData)
{
	return Write(&rData, 1) == 1;
}

bool nglFile::Write(double rData)
{
	return Write(&rData, 1) == 1;
}

/////////////////////////


#ifdef _WIN32_

/*
* Properties
*/

nglFileSize nglFile::GetSize() const
{
#ifdef WINCE
	if(mFD == INVALID_HANDLE_VALUE)
		return 0;

	DWORD	high = 0;
	DWORD	low = GetFileSize(mFD, &high);
	if(low==0xFFFFFFFF)		return 0;
	int64	filesize = (((int64)high)<<32) + ((int64)low);
#else
	if (mFD == NULL)
		return 0;
	int64	previousPosition = ngl_ftell(mFD);
	if (ngl_fseek(mFD, 0, SEEK_END)!=0)
		return 0;

	int64	filesize = ngl_ftell(mFD);
	ngl_fseek(mFD, previousPosition, SEEK_SET);
#endif

	return filesize;
}


/*
* Data flow
*/


bool nglFile::Open()
{
#ifdef WINCE
	if (IsOpen())
		return false;

	DWORD accessflag = 0;
	DWORD createflag = 0;

	nglPath resolvedPath = mPath;
	bool resolved = resolvedPath.ResolveLink();

	nglString filename;
	if (resolved)
	{
		filename = resolvedPath.GetPathName();
	}
	else
	{
		filename = mPath.GetPathName();
	}

	switch (mMode)
	{
	case eFileRead:
		accessflag = GENERIC_READ;
		createflag = OPEN_EXISTING;
		break;

	case eFileWrite:
		accessflag = GENERIC_READ | GENERIC_WRITE;
		createflag = CREATE_ALWAYS;
		break;

	case eFileModify:
		accessflag = GENERIC_READ | GENERIC_WRITE;
		createflag = OPEN_ALWAYS;
		break;

	case eFileAppend:
		accessflag = GENERIC_READ | GENERIC_WRITE;
		createflag = OPEN_ALWAYS;
		break;
	}

/*	SECURITY_ATTRIBUTES SecurityAttributes;
	SecurityAttributes.nLength = sizeof( SecurityAttributes );
	SecurityAttributes.lpSecurityDescriptor = NULL;
	SecurityAttributes.bInheritHandle = TRUE;*/

	mFD = ::CreateFile(filename.GetChars(),
		accessflag,
		FILE_SHARE_READ + FILE_SHARE_WRITE,//#TEST : FILE_SHARE_WRITE is to be removed!
		NULL, // Default security attributes
		createflag,
		FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_RANDOM_ACCESS,
		NULL
		);

	// FIXME : The error handling hasn't been tested and is quite tediously based on MSDN.
	UINT error = GetLastError();
	switch (error)
	{ //#FIXME Handle errors...
	case ERROR_ACCESS_DENIED      :
		break;
	case ERROR_BUFFER_OVERFLOW    :
		break;
	case ERROR_FILE_NOT_FOUND     :
		break;
	case ERROR_WRITE_PROTECT      :
		break;
	case ERROR_DISK_FULL          :
		break;
	case ERROR_TOO_MANY_OPEN_FILES:
		break;
	case ERROR_SHARING_VIOLATION:
		break;
	}

	return IsOpen();

#else
	if (IsOpen())
		return false;

	nglPath resolvedPath = mPath;
	bool resolved = resolvedPath.ResolveLink();

	nglString filename;
	if (resolved)
	{
		filename = resolvedPath.GetPathName();
	}
	else
	{
		filename = mPath.GetPathName();
	}

	switch (mMode)
	{
		case eFileRead:
			mFD = _fsopen(filename.GetChars(), "rb", _SH_DENYNO);
			break;

		case eFileWrite:
			mFD = _fsopen(filename.GetChars(), "wb", _SH_DENYNO);
			break;

		case eFileModify:
			mFD = _fsopen(filename.GetChars(), "rb+", _SH_DENYNO);
      if (!mFD)
        mFD = _fsopen(filename.GetChars(), "wb", _SH_DENYNO);
      break;

		case eFileAppend:
			mFD = _fsopen(filename.GetChars(), "ab+", _SH_DENYNO);
			break;
	}

	return IsOpen();
#endif
}

void nglFile::Close()
{
	if (!IsOpen())
		return;

	if (mAutoFlush)
		Flush();

#ifdef WINCE
	CloseHandle(mFD);
	mFD = INVALID_HANDLE_VALUE;
#else
	ngl_fclose(mFD);
	mFD = NULL;
#endif
}

bool nglFile::IsOpen() const
{
#ifdef WINCE
	return mFD != INVALID_HANDLE_VALUE;
#else
	return mFD != NULL;
#endif
}

bool nglFile::IsEOF() const
{
	return GetPos() >= GetSize();
}

nglFileOffset nglFile::GetPos() const
{
#ifdef WINCE
	if (mFD == INVALID_HANDLE_VALUE)
		return 0;

	// PDA : to test !
	DWORD result = SetFilePointer(mFD, 0, NULL, FILE_CURRENT);
	if (result==0xFFFFFFFF)
		return 0;

	return result;
#else
	if (mFD == NULL)	return 0;
	return ngl_ftell(mFD);
#endif
}

nglFileOffset nglFile::SetPos(nglFileOffset Offset, nglFileWhence Whence)
{
#ifdef WINCE
	DWORD whence;
	switch (Whence)
	{
		case eFileFromStart:
			whence = FILE_BEGIN;
			break;
		case eFileForward:
			whence = FILE_CURRENT;
			break;
		case eFileRewind:
			Offset = -Offset;
			whence = FILE_CURRENT;
			break;
		case eFileFromEnd:
			Offset = -Offset;
			whence = FILE_END;
			break;
	}

	DWORD result = SetFilePointer(mFD, (LONG)Offset, NULL, whence);
	if (!result)
		return 0;

	return Offset;
#else
	switch (Whence)
	{
		case eFileFromStart:
			ngl_fseek(mFD, Offset, SEEK_SET);
			break;
		case eFileForward:
			ngl_fseek(mFD, Offset, SEEK_CUR);
			break;
		case eFileRewind:
			Offset = -Offset;
			ngl_fseek(mFD, Offset, SEEK_CUR);
			break;
		case eFileFromEnd:
			Offset = -Offset;
			ngl_fseek(mFD, Offset, SEEK_END);
			break;
	}

	return Offset;
#endif
}

void nglFile::Flush()
{
#ifdef WINCE
	FlushFileBuffers(mFD);
#else
	ngl_fflush(mFD);
#endif
}


/*
* Generic read/write methods
*/

int64 nglFile::Read (void* pData, int64 WordCount, uint WordSize)
{
	/*
	* The following code deals with whole-word atomic read and byte swapping.
	* Most of it can be reused, until your read() system call can do such things :)
	* In case this code is reused, I'll migrate it in _shr and only mandates
	* platform implementor to write a SysRead() method
	*/
	int64 done, rest;

	if (!IsOpen() || (pData == NULL) || (WordCount == 0))
		return 0;

#ifdef WINCE
	DWORD dwdone = 0;
	if (!ReadFile(mFD, pData, (DWORD)(WordCount * WordSize), &dwdone, NULL))
		dwdone = 0;
	done = dwdone;
#else
	done = ngl_fread(pData, 1, (size_t)(WordCount * WordSize), mFD);
#endif

	if (WordSize == 1)
		return done;
	rest = done % WordSize;
	if (rest != 0)
		SetPos (rest, eFileRewind);

	// Here we suppose only LE & BE byte order (we forget PDP)
	done /= WordSize;
	if ((done > 0) && (mEndian != eEndianNative))
	{
		switch (WordSize)
		{
		case 2: bswap_16_s ((uint16*)pData, (size_t)done); break;
		case 4: bswap_32_s ((uint32*)pData, (size_t)done); break;
		case 8: bswap_64_s ((uint64*)pData, (size_t)done); break;
		}
	}
	return done;
}

int64 nglFile::Write (const void* pData, int64 WordCount, uint WordSize)
{
	/* See remarks in Read
	*/
	int64 done, rest;
	bool swapped;

	if (!IsOpen() || (mMode == eFileRead) || (pData == NULL) || (WordCount == 0))
		return 0;

	// Here we suppose only LE & BE byte order (we forget PDP)
	swapped = ((WordSize > 1) && (mEndian != eEndianNative));
	if (swapped)
	{
		// allocate for a byte swapped buffer copy
		void* pTemp = malloc((size_t)(WordCount * WordSize));
		if (!pData) return 0;
		memcpy(pTemp, pData, (size_t)(WordCount*WordSize));
		pData = pTemp;
		switch (WordSize)
		{
		case 2: bswap_16_s ((uint16*)pData, (size_t)WordCount); break;
		case 4: bswap_32_s ((uint32*)pData, (size_t)WordCount); break;
		case 8: bswap_64_s ((uint64*)pData, (size_t)WordCount); break;
		}
	}

#ifdef WINCE
	DWORD dwdone = 0;
	if (!WriteFile(mFD, pData, (DWORD)(WordCount * WordSize), &dwdone, NULL))
		dwdone = 0;
	done = dwdone;
#else
	done = ngl_fwrite(pData, 1, (size_t)(WordCount * WordSize), mFD);
#endif

	if (swapped)
		free ((void*)pData); // Force const-check (it's OK)
	if (mAutoFlush)
		Flush();

	if (WordSize == 1)
		return done;
	rest = done % WordSize;
	if (rest != 0)
		SetPos (rest, eFileRewind);
	done /= WordSize;
	return done;
}


#endif // _WIN32_


#if ((defined __APPLE__)||(defined _LINUX_)) || (defined _ANDROID_) || (defined _MINUI3_)

/* Implemented in file/File_shr.cpp */
extern const nglChar* File_mode(nglFileMode mode);
extern const nglChar* File_endian(nglEndian endian);

/*
 * Properties
 */

nglFileSize nglFile::GetSize() const
{
  struct stat info;
  int res = fstat (mFD, &info);

  return (res == -1) ? 0 : info.st_size;
}


/*
 * Data flow
 */

bool nglFile::Open()
{
  if (IsOpen())
    return false;

  int flags;
	const char* filename = mPath.GetPathName().Export();
#if (defined _LINUX_) || (defined __APPLE__) || (defined _MINUI3_)
	nglPath resolvedPath = mPath;
	bool resolved = resolvedPath.ResolveLink();
	if (resolved)
	{
    delete filename;
		filename = resolvedPath.GetPathName().Export();
	}
#endif

    switch (mMode)
    {
      case eFileRead  :
        flags = O_RDONLY;
        break;
      case eFileWrite :
        flags = O_RDWR + O_CREAT + O_TRUNC;
        break;
      case eFileModify:
        flags = O_RDWR + O_CREAT;
        break;
      case eFileAppend:
        flags = O_RDWR + O_CREAT + O_APPEND;
        break;
      default:
        delete filename;
        return false;
    }
#ifdef _WIN32_
  flags += _O_BINARY;
#endif
  mFD = open(filename, flags, 00644);

  free(const_cast<char *>(filename));
  if (mFD == -1)
  {
    return false;
  }
  return true;
}

void nglFile::Close()
{
  if (!IsOpen())
    return;

  if (mAutoFlush)
    Flush();
  close(mFD);
  mFD = -1;
}

bool nglFile::IsOpen() const
{
  return (mFD != -1);
}

bool nglFile::IsEOF() const
{
  char c;

  if (!IsOpen()) return true;
  if (read(mFD, &c, 1) == 1)
  {
    lseek(mFD, -1, SEEK_CUR);
    return false;
  }
  else
    return true;
}

nglFileOffset nglFile::GetPos() const
{
  if (!IsOpen()) return 0;
  return lseek (mFD, 0, SEEK_CUR);
}

nglFileOffset nglFile::SetPos(nglFileOffset Offset, nglFileWhence Whence)
{
  nglFileOffset pos = 0;

  if (!IsOpen()) return 0;
  switch (Whence)
  {
    case eFileFromStart:
      pos = Offset;
      break;
    case eFileForward:
      pos = GetPos() + Offset;
      break;
    case eFileRewind:
      pos = GetPos() - Offset;
      break;
    case eFileFromEnd:
      pos = GetSize() - Offset;
      break;
  }
  if (pos < 0) pos = 0;
  nglFileOffset newPos = lseek (mFD, pos, SEEK_SET);
  return newPos;
}

void nglFile::Flush()
{
  if (!IsOpen()) return;
#if !macintosh  // there is no equivalent on mac OS 9 as ios are not buffered (yes this is incredible)
  fsync (mFD);
#endif
}


/*
 * Generic read/write methods
 */

int64 nglFile::Read (void* pData, int64 WordCount, uint WordSize)
{
  int64 done, rest;

  if (!IsOpen() || (pData == NULL) || (WordCount == 0)) return 0;
  done = read (mFD, pData, WordCount * WordSize);

  if (WordSize == 1) return done;
  rest = done % WordSize;
  if (rest != 0) SetPos (rest, eFileRewind);

  // Here we suppose only LE & BE byte order (we forget PDP)
  done /= WordSize;
  if ((done > 0) && (mEndian != eEndianNative))
    switch (WordSize)
    {
      case 2: bswap_16_s ((uint16*)pData, done); break;
      case 4: bswap_32_s ((uint32*)pData, done); break;
      case 8: bswap_64_s ((uint64*)pData, done); break;
    }
      return done;
}

int64 nglFile::Write (const void* pData, int64 WordCount, uint WordSize)
{
  int64 done, rest;

  if (!IsOpen() || (mMode == eFileRead) || (pData == NULL) || (WordCount == 0)) return 0;

  // Here we suppose only LE & BE byte order (we forget PDP)
  if ((WordSize > 1) && (mEndian != eEndianNative))
  {
    // allocate for a byte swapped buffer copy
    void* ptr = malloc (WordCount * WordSize);
    if (!ptr) return 0;
    memcpy(ptr, pData, WordCount * WordSize);
    switch (WordSize)
    {
      case 2: bswap_16_s ((uint16*)ptr, WordCount); break;
      case 4: bswap_32_s ((uint32*)ptr, WordCount); break;
      case 8: bswap_64_s ((uint64*)ptr, WordCount); break;
    }
    done = write (mFD, ptr, WordCount * WordSize);
    free(ptr);
  }
  else
  {
    done = write (mFD, pData, WordCount * WordSize);
  }
  if (mAutoFlush) Flush();

  if (WordSize == 1) return done;
  rest = done % WordSize;
  if (rest != 0) SetPos (rest, eFileRewind);
  done /= WordSize;
  return done;
}

#endif
