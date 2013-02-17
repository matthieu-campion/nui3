/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"


nglIZip::~nglIZip()
{
  mpZip->Close(mpUnzip);
}

nglStreamState nglIZip::GetState() const
{
  return mpZip->GetState(mpUnzip);
}

nglFileOffset nglIZip::GetPos() const
{
  return mpZip->GetPos(mpUnzip);
}

nglFileOffset nglIZip::SetPos (nglFileOffset Where, nglStreamWhence Whence)
{
  nglFileOffset OriginalPos = GetPos();
  nglFileOffset Pos = 0;
  switch (Whence)
  {
  case eStreamFromStart:
    {
      Pos = Where;
    }
    break;
  case eStreamForward:
    {
      Pos = OriginalPos + Where;
    }
    break;
  case eStreamRewind:
    {
      Pos = OriginalPos - Where;
    }
    break;
  case eStreamFromEnd:
    {
      Pos = mSize - Where;
    }
    break;
  default:
    return 0;
  }

  nglFileOffset NewPos = mpZip->SetPos(mpUnzip, Pos, this);

  if (NewPos == Pos)
    return Where;

  if (Whence == eStreamFromStart)
    return NewPos;
  if (Whence == eStreamForward)
    return NewPos - OriginalPos;
  if (Whence == eStreamRewind)
    return NewPos - OriginalPos;
  if (Whence == eStreamFromEnd)
    return NewPos - mSize;
  return 0;
}

nglFileSize nglIZip::Available (uint WordSize)
{
  return (mSize - GetPos()) / WordSize;
}

int64 nglIZip::Read (void* pData, int64 WordCount, uint WordSize)
{
  return mpZip->Read(mpUnzip, pData, WordCount, WordSize, mEndian);
}

nglIZip::nglIZip(nglZipFS* pZip, void* pUnzip, nglSize size, int NumOfFile,int PosInZipDirectory)
{
  //printf("new zip %p\n", pUnzip);
  mpZip = pZip;
  mpUnzip = pUnzip;
  mSize = size;
  mNumOfFile = NumOfFile;
  mPosInZipDirectory = PosInZipDirectory;
}

void nglIZip::SetRef(void* pUnzip)
{
  //printf("set zip %p (old: %p)\n", pUnzip, mpUnzip);
  mpUnzip = pUnzip;
}
