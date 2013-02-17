/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglImageTGACodec.h"


nglImageTGACodec::nglImageTGACodec(void)
{
  mState = tgaHeader;
  mpImage = NULL;
  mpImageID = NULL;
  mLine = 0;
  mLineSize = 0;
  mStop = true;
}

nglImageTGACodec::~nglImageTGACodec(void)
{
  if (mpImageID)
    delete mpImageID;
  mpImageID = NULL;
  mpImage = NULL;
  mState = tgaHeader;
}

bool nglImageTGACodec::Init(nglImage* pImage)
{
  mpImage = pImage;
  return true;
}

bool nglImageTGACodec::Probe(nglIStream* pIStream)
{
  return ReadHeader(pIStream,true);
}

bool nglImageTGACodec::ReadHeader(nglIStream* pIStream,bool Peek)
{
  nglImageInfo info;
  char buffer[18];
  nglIMemory IMem(buffer,18);

  // Try to read the header from the stream:
  if (Peek)
  {
    if (pIStream->Peek(buffer,1,18) != 1)
      return false;
  }
  else
  {
    if (!pIStream->Available(18)) // Only read the complete header or nothing.
      return false;
    if (pIStream->Read(buffer,1,18) != 1)
      return false;
  }
 
  IMem.ReadUInt8(&mHeader.IDFieldSize);
  IMem.ReadUInt8(&mHeader.ColorMapType);
  IMem.ReadUInt8(&mHeader.ImageTypeCode);
  // Color map specs:
  IMem.ReadUInt16(&mHeader.ColorMapOrigin);
  IMem.ReadUInt16(&mHeader.ColorMapLength);
  IMem.ReadUInt8(&mHeader.ColorMapEntrySize);
  // Image Specification:
  IMem.ReadUInt16(&mHeader.PosX);
  IMem.ReadUInt16(&mHeader.PosY);
  IMem.ReadUInt16(&mHeader.Width);
  IMem.ReadUInt16(&mHeader.Height);
  IMem.ReadUInt8(&mHeader.ImagePixelSize);
  IMem.ReadUInt8(&mHeader.ImageDescriptorByte);

  if (mHeader.ColorMapType > 1) // Only possible values are 0 (no color map) and 1 (colormap present)
    return false;

  if (mHeader.ImageTypeCode != 2) // only type 2 is supported at the time
    return false;

  if (mHeader.ImageDescriptorByte & 0x10)  // Reserved should be 0
    return false;

  uint8 attrbits = mHeader.ImageDescriptorByte & 0x0F;
//  bool  origin   = (mHeader.ImageDescriptorByte & 0x20)?true:false; // false = lower left corner, true = upper left corner
  uint8 interleave = (mHeader.ImageDescriptorByte >> 6) & 0x02; 
    // 00 = non-interleaved.
    // 01 = two-way (even/odd) interleaving.
    // 10 = four way interleaving.
    // 11 = reserved.

  if (interleave != 0) // For the moment we can only read uninterleaved TGA files. 
    return false; 

  if (mHeader.ImagePixelSize == 16)
  {
    if (attrbits > 1)  // We only can read 15 and 16 bits data.
      return 0; 
    info.mPixelFormat = attrbits?eImagePixelRGBA:eImagePixelRGB;
  }
  else if (mHeader.ImagePixelSize == 24)
  {                    
    if (attrbits != 0) // Only RGB data in this mode.
      return false;
    info.mPixelFormat = eImagePixelRGB;
  }
  else if (mHeader.ImagePixelSize == 32)
  {
    if (attrbits !=8) // 8 bits of alpha or nothing.
      return false;
    info.mPixelFormat = eImagePixelRGBA;
  }
  else // We only support 16, 24 and 32 bits of data...
    return false;

  // Ok , concistency check passed succesfully for this stream.

  if (!Peek)
  {
    mLine=0;
    info.mBitDepth = mHeader.ImagePixelSize;
    info.mBufferFormat = eImageFormatRaw;
    info.mBytesPerPixel = (mHeader.ImagePixelSize/8);
    info.mBytesPerLine = info.mBytesPerPixel*mHeader.Width;
    info.mHeight = mHeader.Height;
    info.mWidth = mHeader.Width;

    mLineSize=info.mBytesPerLine;

    SendInfo(info);
  }
//  NGL_OUT(_T("ReadHeader: %d x %d\n"), mHeader.Width, mHeader.Height);
  return true;
}

void ReverseRGB24(char*& pBuffer,uint count)
{
  for (uint i=0; i<count; i++)
  {
    uint8 t;
    t=pBuffer[0];
    pBuffer[0]=pBuffer[2];
    pBuffer[2]=t;
    pBuffer+=3;
  }
}

void ReverseRGB32(char*& pBuffer,uint count)
{
  for (uint i=0; i<count; i++)
  {
    uint8 t;
    t=pBuffer[1];
    pBuffer[1]=pBuffer[3];
    pBuffer[3]=t;
    pBuffer+=4;
  }
}

bool nglImageTGACodec::Feed(nglIStream* pIStream) 
{
  nglFileSize count;
  while (pIStream->Available(1) && pIStream->GetState()==eStreamReady && mStop)
  {
    switch (mState)
    {
    case tgaHeader:
      if (ReadHeader(pIStream,false))
        mState = tgaID;
      break;
    case tgaID:
      if (mHeader.IDFieldSize)
      {
        if (pIStream->Available(mHeader.IDFieldSize))
        {
          mpImageID = new char[mHeader.IDFieldSize];
          if (!mpImageID) // Memory alloc error is a show stopper
            return false;
          if (pIStream->Read(mpImageID,1,mHeader.IDFieldSize) != mHeader.IDFieldSize)
            return false;
          mState = tgaData;
        }
      }
      else
        mState = tgaData;
      break;
    case tgaData:
      if ((count=pIStream->Available(mLineSize)))
      {
        char* pBuffer = mpImage->GetBuffer();
        if (!(mHeader.ImageDescriptorByte & 0x20))
        {
          if (pIStream->Read(pBuffer+mLine*mLineSize,(nglSize)count,mLineSize) != (int)count)
            return false;
          pBuffer+=mLine*mLineSize;
          if (mHeader.ImagePixelSize == 24)
          {
            for (nglFileOffset j=mLine; j<count; j++)
              ReverseRGB24(pBuffer,mHeader.Width);
          }
          else if (mHeader.ImagePixelSize == 32)
          {
            for (nglFileOffset j=mLine; j<count; j++)
              ReverseRGB32(pBuffer,mHeader.Width);
          }
          mLine+=(nglSize)count;
        }
        else
        {
          for (nglFileOffset i=0; i<count; i++)
          {
            pBuffer = mpImage->GetBuffer()+(mHeader.Height-mLine-1)*mLineSize;

            // Read one line:
            if (pIStream->Read(pBuffer,1,mLineSize) != 1)
              return false;
            if (mHeader.ImagePixelSize == 24)
              ReverseRGB24(pBuffer,mHeader.Width);
            else if (mHeader.ImagePixelSize == 32)
              ReverseRGB32(pBuffer,mHeader.Width);
            mLine++;
          }
        }

        if (!SendData((float)mLine/(float)mHeader.Height))
          mStop = true;
      }
      return (pIStream->GetState()==eStreamWait) || (pIStream->GetState()==eStreamReady);
      break;
    case tgaError:
      return false;
    }
  }
  return (pIStream->GetState()==eStreamWait) || (pIStream->GetState()==eStreamReady);
}

bool nglImageTGACodec::Save(nglOStream* pOStream) 
{
  return false;
}

float nglImageTGACodec::GetCompletion()
{
  return (float)mLine/(float)mHeader.Height;
}


//--------------------------------------------------------------------------------
//DATA TYPE 2:  Unmapped RGB images.                                             |
//_______________________________________________________________________________|
//| Offset | Length |                     Description                            |
//|--------|--------|------------------------------------------------------------|
//|--------|--------|------------------------------------------------------------|
//|    0   |     1  |  Number of Characters in Identification Field.             |
//|        |        |                                                            |
//|        |        |  This field is a one-byte unsigned integer, specifying     |
//|        |        |  the length of the Image Identification Field.  Its value  |
//|        |        |  is 0 to 255.  A value of 0 means that no Image            |
//|        |        |  Identification Field is included.                         |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//|    1   |     1  |  Color Map Type.                                           |
//|        |        |                                                            |
//|        |        |  This field contains either 0 or 1.  0 means no color map  |
//|        |        |  is included.  1 means a color map is included, but since  |
//|        |        |  this is an unmapped image it is usually ignored.  TIPS    |
//|        |        |  ( a Targa paint system ) will set the border color        |
//|        |        |  the first map color if it is present.                     |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//|    2   |     1  |  Image Type Code.                                          |
//|        |        |                                                            |
//|        |        |  This field will always contain a binary 2.                |
//|        |        |  ( That's what makes it Data Type 2 ).                     |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//|    3   |     5  |  Color Map Specification.                                  |
//|        |        |                                                            |
//|        |        |  Ignored if Color Map Type is 0; otherwise, interpreted    |
//|        |        |  as follows:                                               |
//|        |        |                                                            |
//|    3   |     2  |  Color Map Origin.                                         |
//|        |        |  Integer ( lo-hi ) index of first color map entry.         |
//|        |        |                                                            |
//|    5   |     2  |  Color Map Length.                                         |
//|        |        |  Integer ( lo-hi ) count of color map entries.             |
//|        |        |                                                            |
//|    7   |     1  |  Color Map Entry Size.                                     |
//|        |        |  Number of bits in color map entry.  16 for the Targa 16,  |
//|        |        |  24 for the Targa 24, 32 for the Targa 32.                 |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//|    8   |    10  |  Image Specification.                                      |
//|        |        |                                                            |
//|    8   |     2  |  X Origin of Image.                                        |
//|        |        |  Integer ( lo-hi ) X coordinate of the lower left corner   |
//|        |        |  of the image.                                             |
//|        |        |                                                            |
//|   10   |     2  |  Y Origin of Image.                                        |
//|        |        |  Integer ( lo-hi ) Y coordinate of the lower left corner   |
//|        |        |  of the image.                                             |
//|        |        |                                                            |
//|   12   |     2  |  Width of Image.                                           |
//|        |        |  Integer ( lo-hi ) width of the image in pixels.           |
//|        |        |                                                            |
//|   14   |     2  |  Height of Image.                                          |
//|        |        |  Integer ( lo-hi ) height of the image in pixels.          |
//|        |        |                                                            |
//|   16   |     1  |  Image Pixel Size.                                         |
//|        |        |  Number of bits in a pixel.  This is 16 for Targa 16,      |
//|        |        |  24 for Targa 24, and .... well, you get the idea.         |
//|        |        |                                                            |
//|   17   |     1  |  Image Descriptor Byte.                                    |
//|        |        |  Bits 3-0 - number of attribute bits associated with each  |
//|        |        |             pixel.  For the Targa 16, this would be 0 or   |
//|        |        |             1.  For the Targa 24, it should be 0.  For     |
//|        |        |             Targa 32, it should be 8.                      |
//|        |        |  Bit 4    - reserved.  Must be set to 0.                   |
//|        |        |  Bit 5    - screen origin bit.                             |
//|        |        |             0 = Origin in lower left-hand corner.          |
//|        |        |             1 = Origin in upper left-hand corner.          |
//|        |        |             Must be 0 for Truevision images.               |
//|        |        |  Bits 7-6 - Data storage interleaving flag.                |
//|        |        |             00 = non-interleaved.                          |
//|        |        |             01 = two-way (even/odd) interleaving.          |
//|        |        |             10 = four way interleaving.                    |
//|        |        |             11 = reserved.                                 |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//|   18   | varies |  Image Identification Field.                               |
//|        |        |  Contains a free-form identification field of the length   |
//|        |        |  specified in byte 1 of the image record.  It's usually    |
//|        |        |  omitted ( length in byte 1 = 0 ), but can be up to 255    |
//|        |        |  characters.  If more identification information is        |
//|        |        |  required, it can be stored after the image data.          |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//| varies | varies |  Color map data.                                           |
//|        |        |                                                            |
//|        |        |  If the Color Map Type is 0, this field doesn't exist.     |
//|        |        |  Otherwise, just read past it to get to the image.         |
//|        |        |  The Color Map Specification describes the size of each    |
//|        |        |  entry, and the number of entries you'll have to skip.     |
//|        |        |  Each color map entry is 2, 3, or 4 bytes.                 |
//|        |        |                                                            |
//|--------|--------|------------------------------------------------------------|
//| varies | varies |  Image Data Field.                                         |
//|        |        |                                                            |
//|        |        |  This field specifies (width) x (height) pixels.  Each     |
//|        |        |  pixel specifies an RGB color value, which is stored as    |
//|        |        |  an integral number of bytes.                              |
//|        |        |                                                            |
//|        |        |  The 2 byte entry is broken down as follows:               |
//|        |        |  ARRRRRGG GGGBBBBB, where each letter represents a bit.    |
//|        |        |  But, because of the lo-hi storage order, the first byte   |
//|        |        |  coming from the file will actually be GGGBBBBB, and the   |
//|        |        |  second will be ARRRRRGG. "A" represents an attribute bit. |
//|        |        |                                                            |
//|        |        |  The 3 byte entry contains 1 byte each of blue, green,     |
//|        |        |  and red.                                                  |
//|        |        |                                                            |
//|        |        |  The 4 byte entry contains 1 byte each of blue, green,     |
//|        |        |  red, and attribute.  For faster speed (because of the     |
//|        |        |  hardware of the Targa board itself), Targa 24 images are  |
//|        |        |  sometimes stored as Targa 32 images.                      |
//|        |        |                                                            |
//--------------------------------------------------------------------------------
