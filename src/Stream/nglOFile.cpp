/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

/*
* Life cycle
*/

nglOFile::nglOFile (nglFile* pFile, bool OwnFile)
{
  mpFile = pFile;
  if ((mpFile) && (mpFile->GetMode() == eFileRead)) mpFile = NULL;
  mOwnFile = OwnFile;
}

nglOFile::nglOFile (const nglPath& rPath, nglOFileMode Mode, bool OpenNow)
{
  mpFile = new nglFile (rPath, (nglFileMode)Mode, OpenNow);
  if ((!mpFile) || (OpenNow && !mpFile->IsOpen()))
  {
    delete mpFile;
    mpFile = NULL;
  }
  mOwnFile = true;
}

nglOFile::~nglOFile()
{
  if (mOwnFile && (mpFile)) delete mpFile;
}


/*
* Status/error
*/

bool nglOFile::Open()
{
  if (!mpFile) return eStreamNone;
  return mpFile->Open();
}


bool nglOFile::IsOpen() const
{
  if (!mpFile)
    return false;
  return mpFile->IsOpen();
}


void nglOFile::Close()
{
  if (mpFile) mpFile->Close();
}

nglStreamState nglOFile::GetState() const
{
  if (!mpFile) return eStreamNone;
  if (!mpFile->IsOpen()) return eStreamError;
  if (mpFile->IsEOF()) return eStreamEnd;
  return eStreamReady;
}

/*
* Stream navigation
*/

nglFileOffset nglOFile::GetPos() const
{
  // Needed on 32 bits platform
  return (mpFile) ? mpFile->GetPos() : 0;
}

nglFileOffset nglOFile::SetPos(nglFileOffset Where, nglStreamWhence Whence)
{
  if (mpFile == NULL) return false;
  switch (Whence)
  {
    // Needed on 32 bits platform
  case eStreamFromStart: return mpFile->SetPos (Where, eFileFromStart);
  case eStreamForward  : return mpFile->SetPos (Where, eFileForward);
  case eStreamRewind   : return mpFile->SetPos (Where, eFileRewind);
  case eStreamFromEnd  : return mpFile->SetPos (Where, eFileFromEnd);
  }
  return 0;
}


/*
* Output method
*/

int64 nglOFile::Write(const void* pData, int64 WordCount, uint WordSize)
{
  if (!mpFile) return 0;

  mpFile->SetEndian(mEndian); // Truly inelegant hack, but somehow still efficient
  return mpFile->Write (pData, WordCount, WordSize);
}

void nglOFile::Flush()
{
  if (mpFile) mpFile->Flush();
}

void nglOFile::AutoFlush(bool Auto)
{
  if (mpFile) mpFile->AutoFlush (Auto);
}

void nglOFile::SetEndian(nglEndian Endian)
{
  mpFile->SetEndian(Endian);
  nglOStream::SetEndian(Endian);
}

