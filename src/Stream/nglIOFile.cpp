/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"


nglIOFile::nglIOFile(nglFile* pFile, bool OwnFile)
{
  mpFile = pFile;
  if ((mpFile) && (mpFile->GetMode() == eFileRead)) 
    mpFile = NULL;
  mOwnFile = OwnFile;
}

nglIOFile::nglIOFile(const nglPath& rPath, nglOFileMode Mode, bool OpenNow)
{
  mpFile = new nglFile (rPath, (nglFileMode)Mode, OpenNow);
  if ((!mpFile) || (OpenNow && !mpFile->IsOpen()))
  {
    delete mpFile;
    mpFile = NULL;
  }
  mOwnFile = true;
}

nglIOFile::~nglIOFile()
{
  if (mOwnFile && (mpFile))
    delete mpFile;
}

bool nglIOFile::Open()
{
  if (!mpFile) return eStreamNone;
  return mpFile->Open();
}

bool nglIOFile::IsOpen() const
{
  if (!mpFile)
    return false;
  return mpFile->IsOpen();
}


void nglIOFile::Close()
{
  if (mpFile) 
    mpFile->Close();
}

nglStreamState nglIOFile::GetState() const
{
  if (!mpFile) 
    return eStreamNone;
  if (!mpFile->IsOpen()) 
    return eStreamError;
  if (mpFile->IsEOF()) 
    return eStreamEnd;
  return eStreamReady;
}

nglFileOffset nglIOFile::GetPos() const
{
  // Needed on 32 bits platform
  return (mpFile) ? mpFile->GetPos() : 0;
}

nglFileOffset nglIOFile::SetPos(nglFileOffset Where, nglStreamWhence Whence)
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

nglFileSize nglIOFile::Available(uint WordSize)
{
  if (WordSize == 0) return 0;

  nglFileSize size,pos;
  pos = mpFile->GetPos();
  size = mpFile->GetSize();
  return (size-pos) / WordSize;
}

int64 nglIOFile::Read(void* pData, int64 WordCount, uint WordSize)
{
  if (!mpFile) 
    return 0;

  mpFile->SetEndian(mEndian); // Truly inelegant hack, but somehow still efficient
  return mpFile->Read(pData, WordCount, WordSize);
}

int64 nglIOFile::Write(const void* pData, int64 WordCount, uint WordSize)
{
  if (!mpFile) 
    return 0;

  mpFile->SetEndian(mEndian); // Truly inelegant hack, but somehow still efficient
  return mpFile->Write (pData, WordCount, WordSize);
}

void nglIOFile::Flush()
{
  if (mpFile) 
    mpFile->Flush();
}

void nglIOFile::AutoFlush(bool Auto)
{
  if (mpFile) 
    mpFile->AutoFlush (Auto);
}

void nglIOFile::SetEndian(nglEndian Endian)
{
  mpFile->SetEndian(Endian);
  nglIOStream::SetEndian(Endian);
}

