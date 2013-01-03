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

#include <errno.h>

#if (!defined __CFM__ ) && (!defined __CFM_CLASSIC__ )
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

#include <fcntl.h>
#include <unistd.h>

/* Implemented in file/nglFile_shr.cpp */
const nglChar* nglFile_mode(nglFileMode mode)
{
  switch (mode)
  {
    case eFileRead: return "Read";
    case eFileWrite: return "Write";
    case eFileModify: return "Modify";
    case eFileAppend: return "Append";
  }
  return "???";
}

const nglChar* nglFile_endian(nglEndian endian)
{
  switch (endian)
  {
    case eEndianBig: return "Big";
    case eEndianLittle: return "Little";
    case eEndianPDP: return "PDP";
  }

  return "???";
}

/* Legacy code : enable build against Win32's POSIX emulation
#ifdef _WIN32_
  #include <io.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #define fsync _commit
#endif _WIN32
*/

/* Legacy code : enable build against Carbon's POSIX layer
#ifdef macintosh
  #include <types.h>
  #include <stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #define fsync commit
#endif // macintosh
*/


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
//NGL_OUT("nglFile::Open()\n");
  if (IsOpen())
  {
    //NGL_OUT("  -> file already open\n");
    return false;
  }

  int flags;
  std::string str(mPath.GetPathName().GetStdString());
	const char* filename = str.c_str();
	#ifdef _CARBON_
	nglPath resolvedPath = mPath;
	bool resolved = resolvedPath.ResolveLink();
  std::string resolved_path(resolvedPath.GetPathName().GetStdString());
	if (resolved)
	{
		filename = resolved_path.c_str();
	}
	#endif // _CARBON_


  NGL_DEBUG( NGL_LOG(_T("file"), NGL_LOG_INFO, _T("opening '%s', %s, %s (%snative)\n"), mPath.GetPathName().GetChars(), nglFile_mode(mMode), nglFile_endian(mEndian), (mEndian == eEndianNative) ? _T("") : _T("non-")); )

  switch (mMode)
  {
    case eFileRead  : flags = O_RDONLY; break;
    case eFileWrite : flags = O_RDWR + O_CREAT + O_TRUNC; break;
    case eFileModify: flags = O_RDWR + O_CREAT; break;
    case eFileAppend: flags = O_RDWR + O_CREAT + O_APPEND; break;
    default:
  //NGL_OUT("Invalid Mode... exiting\n");
        return false;
  }
#ifdef _WIN32_
  flags += _O_BINARY;
#endif
  mFD = open (filename, flags, 00644);
//NGL_OUT("open: %s -> %d\n", filename, mFD);
  if (mFD == -1)
  {
    switch (errno)
    {
/*
      case EISDIR: SetError ("file", NGL_FILE_EISDIR); break;
      case EACCES: SetError ("file", NGL_FILE_EACCESS); break;
      case ENAMETOOLONG: SetError ("file", NGL_FILE_ETOOLONG); break;
      case ENOENT: SetError ("file", NGL_FILE_EBADPATH); break;
      case EROFS : SetError ("file", NGL_FILE_ERONLY); break;
      case ENOSPC: SetError ("file", NGL_FILE_ENOSPACE); break;
      case EMFILE: SetError ("file", NGL_FILE_EPROCMAX); break;
      case ENFILE: SetError ("file", NGL_FILE_ESYSMAX); break;
*/
    }
    return false;
  }
  return true;
}

void nglFile::Close()
{
  if (!IsOpen())
    return;

  NGL_DEBUG( NGL_LOG(_T("file"), NGL_LOG_INFO, _T("closing '%s'\n"), mPath.GetChars()); )
  if (mAutoFlush) Flush();
  close(mFD);
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
  return lseek (mFD, pos, SEEK_SET);
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
  nglSize done, rest;

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
  nglSize done, rest;

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
