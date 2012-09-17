/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


/*
* Life cycle
*/

nglIFile::nglIFile (nglFile* pFile, bool OwnFile)
{
  mpFile = pFile;
  mOwnFile = OwnFile;
}

nglIFile::nglIFile (const nglPath& rPath, bool OpenNow)
{
  mpFile = new nglFile (rPath, eFileRead, OpenNow);
  if ((!mpFile) || (OpenNow && !mpFile->IsOpen()))
  {
    delete mpFile;
    mpFile = NULL;
  }
  mOwnFile = true;
}

nglIFile::~nglIFile()
{
  if (mOwnFile)
    delete mpFile;
}

/*
* Retrieve the file handle of the stream
*/
nglFile* nglIFile::GetFile() const
{
  return mpFile; 
}

/*
* Status/error
*/

bool nglIFile::Open()
{
  if (!mpFile) return eStreamNone;
  return mpFile->Open();
}


bool nglIFile::IsOpen() const
{
  if (!mpFile)
    return false;
  return mpFile->IsOpen();
}



void nglIFile::Close()
{
  if (mpFile) mpFile->Close();
}

nglStreamState nglIFile::GetState() const
{
  if (!mpFile) return eStreamNone;
  if (!mpFile->IsOpen()) return eStreamError;
  if (mpFile->IsEOF()) return eStreamEnd;
  return eStreamReady;
}

/*
* Stream navigation
*/

nglFileOffset nglIFile::GetPos() const
{
  return (mpFile) ? mpFile->GetPos() : 0;
}

nglFileOffset nglIFile::SetPos (nglFileOffset Where, nglStreamWhence Whence)
{
  if (mpFile == NULL) return false;
  switch (Whence)
  {
  case eStreamFromStart: return mpFile->SetPos (Where, eFileFromStart);
  case eStreamForward  : return mpFile->SetPos (Where, eFileForward);
  case eStreamRewind   : return mpFile->SetPos (Where, eFileRewind);
  case eStreamFromEnd  : return mpFile->SetPos (Where, eFileFromEnd);
  }
  return 0;
}

nglFileSize nglIFile::Available (uint WordSize)
{
  if (WordSize == 0) return 0;

  nglFileSize size,pos;
  pos = mpFile->GetPos();
  size = mpFile->GetSize();
  return (size-pos) / WordSize;
}


/*
* Input method
*/

int64 nglIFile::Read (void* pData, int64 WordCount, uint WordSize)
{
  if (!mpFile) return 0;

  mpFile->SetEndian(mEndian); // Truely inelegant hack, but somehow still efficient
  return mpFile->Read (pData, WordCount, WordSize);
}

void nglIFile::SetEndian(nglEndian Endian)
{
  mpFile->SetEndian(Endian);
  nglIStream::SetEndian(Endian);
}

