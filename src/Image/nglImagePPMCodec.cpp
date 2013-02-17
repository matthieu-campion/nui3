/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglImagePPMCodec.h"

nglImagePPMCodec::nglImagePPMCodec(void)
{
  mpBuffer = NULL;
  mState = ppmReadHeader;
  mpImage = NULL;
  mpImageID = NULL;
  mLine = 0;
  mLineSize = 0;
  mStop = false;
}

nglImagePPMCodec::~nglImagePPMCodec(void)
{
  mpBuffer = NULL;
  if (mpImageID)
    delete mpImageID;
  mpImageID = NULL;
  mpImage = NULL;
  mState = ppmReadHeader;
}

bool nglImagePPMCodec::Init(nglImage* pImage)
{
  mpImage = pImage;
  return true;
}

bool nglImagePPMCodec::Probe(nglIStream* pIStream)
{
  bool success = false;
  char buffer[3];
  if(pIStream->Available(3))
  {                          
    if(pIStream->Peek(buffer, 3,1) >= 3)
    {                               
      if( buffer[0] == 'P' && (buffer[1] == '6' || buffer[1] == '3') )
      {
        success = true;
      }
    }
  }
  return success;
}

bool nglImagePPMCodec::ReadHeader(nglIStream* pIStream)
{
  bool success = false;
  nglImageInfo info;
  char buffer[3];
  if(pIStream->Read(buffer, 3,1) != 3)
  {                               
    return success;          
  }

  if( buffer[0] == 'P' && buffer[1] == '6' )
  {
    mHeader.MagicWord = nglString(buffer);
    mHeader.Comments = _T("");
    nglString line;
    do
    {
      mHeader.Comments += line;
      pIStream->ReadLine(line);
    } while(line[0] == '#');
    std::vector<nglString> dim;
    line.Tokenize(dim, _T(' '));
    mHeader.Width  = dim.size() > 0 ? dim[0].GetInt() : 0;
    mHeader.Height = dim.size() > 1 ? dim[1].GetInt() : 0;

    pIStream->ReadLine(line);
    mHeader.MaxColor = line.GetInt();
    

    mLine=0;
    info.mBitDepth = 24;
    info.mPixelFormat = eImagePixelRGB;
    info.mBufferFormat = eImageFormatRaw;
    info.mBytesPerPixel = 3;
    info.mBytesPerLine = info.mBytesPerPixel*mHeader.Width;
    info.mHeight = mHeader.Height;
    info.mWidth = mHeader.Width;
    mLineSize=info.mBytesPerLine;

    mStop = !SendInfo(info);

    success = true;
  }  
  return success;
}


bool nglImagePPMCodec::Feed(nglIStream* pIStream) 
{
  nglFileSize count;
  while (pIStream->Available() && pIStream->GetState() == eStreamReady && !mStop)
  {
    switch(mState)
    {
      case ppmReadHeader:
        if (ReadHeader(pIStream))
        {
          mpBuffer = mpImage->GetBuffer() + mLineSize*(mHeader.Height-1);
          mState = ppmReadData;
        }
        break;
      case(ppmReadData):
        if ((count = pIStream->Available(mLineSize)))
        {
          int i;
          for (i=0; i<count; i++)
          {
            if(pIStream->Read(mpBuffer, mLineSize, 1) != (int)mLineSize)
            {
              return false;
            }
            mpBuffer -= mLineSize;
          }
          mLine+=(uint)count;             
          mStop = !SendData((float)mLine/(float)mHeader.Height);
        }
        return (pIStream->GetState()==eStreamWait) || (pIStream->GetState()==eStreamReady);
        break;
      case(ppmError):
        return false;
    }
  }
  return (pIStream->GetState()==eStreamWait) || (pIStream->GetState()==eStreamReady);   
}

bool nglImagePPMCodec::Save(nglOStream* pOStream) 
{
  bool success = false;
  nglImageInfo Info;
  mpImage->GetInfo(Info);

  nglString header;
  pOStream->SetTextFormat(eTextUnix);
  header.Format("P6\n"
                "%d %d\n"
                "%d\n",
                Info.mWidth, Info.mHeight, 255);
  pOStream->WriteText(header);
  
  if(Info.mBufferFormat == eImageFormatRaw) 
  {
    char *pConvertedBuffer = new char[Info.mWidth*Info.mHeight*3];
    nglCopyImage(pConvertedBuffer, Info.mWidth, Info.mHeight, 24,
              mpImage->GetBuffer(), Info.mWidth, Info.mHeight, Info.mBitDepth,
              true, false);  
    

    pOStream->Write(pConvertedBuffer, Info.mWidth*Info.mHeight*3, 1);
    delete []pConvertedBuffer;
    success = true;
  }
  return success;
}

float nglImagePPMCodec::GetCompletion()
{
  return (float)mLine/(float)mHeader.Height;
}


