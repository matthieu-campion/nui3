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

#include "nui.h"
#include "nglKernel.h"
#include "nglFile.h"
#include "nglPath.h"

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <sys/stat.h>
#include <direct.h>

using namespace std;

/* Implemented in file/nglFile_shr.cpp */
extern const nglChar* nglFile_mode(nglFileMode mode);
extern const nglChar* nglFile_endian(nglEndian endian);


/*
 * Properties
 */

nglFileSize nglFile::GetSize() const
{
  LARGE_INTEGER pos;
  BOOL result = GetFileSizeEx(mFD,&pos);
  if (!result)
    return 0;
  return pos.QuadPart;
}


/*
 * Data flow
 */


bool nglFile::Open()
{
  if (IsOpen())
    return false;

  DWORD accessflag = 0;
  DWORD createflag = 0;

  nglPath resolvedPath = mPath;
  bool resolved = resolvedPath.ResolveLink();

  const nglChar* filename;
  if (resolved)
  {
    filename = resolvedPath.GetOSPathName();
  }
  else
  {
    filename = mPath.GetOSPathName();
  }

  NGL_DEBUG( NGL_LOG(_T("file"), NGL_LOG_INFO, _T("opening '%ls', %ls, %ls (%snative)\n"), mPath.GetPathName().GetChars(), nglFile_mode(mMode), nglFile_endian(mEndian), (mEndian == eEndianNative) ? _T("") : _T("non-")); )

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
  mFD = CreateFile(filename,
             accessflag,
             FILE_SHARE_READ + FILE_SHARE_WRITE,//#TEST : FILE_SHARE_WRITE is to be removed!
             NULL, // Default security attributes
             createflag,
             FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
             NULL
             );

  // FIXME : The error handling hasn't been tested and is quite tediously based on MSDN.
  UINT error = GetLastError();
  switch (error)
  {
    case ERROR_ACCESS_DENIED      : SetError ("file", NGL_FILE_EACCESS); break;
    case ERROR_BUFFER_OVERFLOW    : SetError ("file", NGL_FILE_ETOOLONG); break;
    case ERROR_FILE_NOT_FOUND     : SetError ("file", NGL_FILE_EBADPATH); break;
    case ERROR_WRITE_PROTECT      : SetError ("file", NGL_FILE_ERONLY); break;
    case ERROR_DISK_FULL          : SetError ("file", NGL_FILE_ENOSPACE); break;
    case ERROR_TOO_MANY_OPEN_FILES: SetError ("file", NGL_FILE_EPROCMAX); break;
    case ERROR_SHARING_VIOLATION: SetError ("file", NGL_FILE_EACCESS); break;
  }

  return IsOpen();
}

void nglFile::Close()
{
  if (!IsOpen())
    return;

  NGL_DEBUG( NGL_LOG(_T("file"), NGL_LOG_INFO, "closing '%ls'\n", mPath.GetChars()); )
  if (mAutoFlush) Flush();

  CloseHandle(mFD);
  mFD = INVALID_HANDLE_VALUE;
}

bool nglFile::IsOpen() const
{
  return mFD != INVALID_HANDLE_VALUE;
}

bool nglFile::IsEOF() const
{
  return GetPos() >= GetSize();
}

nglFileOffset nglFile::GetPos() const
{
  LARGE_INTEGER pos, pos2;
  pos.QuadPart= 0;
  BOOL result = SetFilePointerEx(mFD, pos, &pos2, FILE_CURRENT);
  if (!result)
    return 0;

  return pos2.QuadPart;
}

nglFileOffset nglFile::SetPos(nglFileOffset Offset, nglFileWhence Whence)
{
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
    whence = FILE_CURRENT;
    Offset = -Offset;
    break;
  case eFileFromEnd:
    whence = FILE_END;
    break;
  }

  LARGE_INTEGER pos, pos2;
  pos.QuadPart = Offset;
  BOOL result = SetFilePointerEx(mFD, pos, &pos2, whence);
  if (!result)
    return 0;

  return Offset;
}

void nglFile::Flush()
{
  FlushFileBuffers(mFD);
}


/*
 * Generic read/write methods
 */

nglSize nglFile::Read (void* pData, nglSize WordCount, uint WordSize)
{
/*
 * The following code deals with whole-word atomic read and byte swapping.
 * Most of it can be reused, until your read() system call can do such things :)
 * In case this code is reused, I'll migrate it in _shr and only mandates
 * platform implementor to write a SysRead() method
 */
  nglSize done, rest;

  if (!IsOpen() || (pData == NULL) || (WordCount == 0)) return 0;

  DWORD dwdone = 0;
  if (!ReadFile(mFD, pData, WordCount * WordSize, &dwdone, NULL))
    dwdone = 0;
  done = dwdone;

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

nglSize nglFile::Write (const void* pData, nglSize WordCount, uint WordSize)
{
/* See remarks in Read
 */
  nglSize done, rest;
  bool swapped;

  if (!IsOpen() || (mMode == eFileRead) || (pData == NULL) || (WordCount == 0)) return 0;

  // Here we suppose only LE & BE byte order (we forget PDP)
  swapped = ((WordSize > 1) && (mEndian != eEndianNative));
  if (swapped)
  {
    // allocate for a byte swapped buffer copy
    void* pTemp = malloc(WordCount * WordSize);
    if (!pData) return 0;
    memcpy(pTemp, pData, WordCount*WordSize);
    pData = pTemp;
    switch (WordSize)
    {
      case 2: bswap_16_s ((uint16*)pData, WordCount); break;
      case 4: bswap_32_s ((uint32*)pData, WordCount); break;
      case 8: bswap_64_s ((uint64*)pData, WordCount); break;
    }
  }

  DWORD dwdone = 0;
  if (!WriteFile(mFD, pData, WordCount * WordSize, &dwdone, NULL))
    dwdone = 0;
  done = dwdone;

  if (swapped) free ((void*)pData); // Force const-check (it's OK)
  if (mAutoFlush) Flush();

  if (WordSize == 1) return done;
  rest = done % WordSize;
  if (rest != 0) SetPos (rest, eFileRewind);
  done /= WordSize;
  return done;
}
