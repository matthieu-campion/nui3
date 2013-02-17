/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#include "nglImageTGACodec.h"
#include "nglImagePPMCodec.h"

#ifdef HAVE_COREGRAPHICS
#include "nglImageCGCodec.h"
#endif
#include "nglImagePNGCodec.h"
#ifdef HAVE_LIBJPEG
#include "nglImageJPEGCodec.h"
#endif
#include "nglImageGIFCodec.h"


using namespace std;

static const nglChar* gpImageErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Out of memory"),
         NULL
};

#define NGL_IMAGE_ENONE 0
#define NGL_IMAGE_EOOM  1
#define NGL_IMAGE_ELAST 1


/*
 * nglImageInfo
 */

nglImageInfo::nglImageInfo(uint32 width, uint32 height, uint32 bitdepth)
{
  switch (bitdepth)
  {
  case 32:
    mPixelFormat = eImagePixelRGBA;
    break;
  case 24:
    mPixelFormat = eImagePixelRGB;
    break;
  case 16:
    mPixelFormat = eImagePixelRGBA;
    break;
  case 15:
    mPixelFormat = eImagePixelRGB;
    break;
  case 8:
    mPixelFormat = eImagePixelLum;
    break;
  }
  
  mBufferFormat = eImageFormatRaw;
  mWidth = width;
  mHeight = height;
  mBitDepth = bitdepth;
  mBytesPerPixel = (bitdepth + 1) / 8;
  mBytesPerLine = mWidth * mBytesPerPixel;
  mPreMultAlpha = false;
  mpBuffer = NULL;
  mOwnBuffer = false;

  AllocateBuffer();
}


nglImageInfo::nglImageInfo(bool Managed)
{
  mBufferFormat = eImageFormatNone;
  mPixelFormat  = eImagePixelNone;
  mWidth = 0;
  mHeight = 0;
  mBitDepth = 0;
  mBytesPerPixel = 0;
  mBytesPerLine = 0;
  mPreMultAlpha = false;
  mpBuffer = NULL;
  mOwnBuffer = Managed;
}

nglImageInfo::nglImageInfo (const nglImageInfo& rInfo, bool Clone)
{
  Copy (rInfo, Clone);
}

nglImageInfo::~nglImageInfo()
{
  ReleaseBuffer();
}

bool nglImageInfo::AllocateBuffer()
{
  ReleaseBuffer();
  if (mHeight && mBytesPerLine)
  {
    mpBuffer = (char*) malloc (mHeight * mBytesPerLine);
/*
    if (!mpBuffer)
      SetError ("image", NGL_IMAGE_EOOM);
*/
  }
  mOwnBuffer = true; // Now it's our own (managed) buffer (even if the alloc failed)
  return (mpBuffer != NULL);
}


void nglImageInfo::Transfert(nglImageInfo& rInfo)
{
  Copy(rInfo, false);
  mOwnBuffer = true; 
  rInfo.mOwnBuffer = false;
}


void nglImageInfo::Copy (const nglImageInfo& rInfo, bool Clone)
{
  mBufferFormat   = rInfo.mBufferFormat;
  mPixelFormat    = rInfo.mPixelFormat;
  mWidth          = rInfo.mWidth;
  mHeight         = rInfo.mHeight;
  mBitDepth       = rInfo.mBitDepth;
  mBytesPerPixel  = rInfo.mBytesPerPixel;
  mBytesPerLine   = rInfo.mBytesPerLine;
  mPreMultAlpha   = rInfo.mPreMultAlpha;
  mpBuffer        = rInfo.mpBuffer;
  mOwnBuffer      = false; // We copy a reference, we don't own the buffer right now

  if (Clone)
  {
    AllocateBuffer(); // Now we hold our own (managed) copy
    if (rInfo.mpBuffer)
      memcpy(mpBuffer,rInfo.mpBuffer,mBytesPerLine * mHeight);
  }
}

void nglImageInfo::ReleaseBuffer()
{
  if (!mOwnBuffer) return;

  if (mpBuffer)
  {
    free (mpBuffer);
    mpBuffer = NULL;
  }
}



/*
 * nglImage
 */

vector<nglImageCodecInfo*> *nglImage::mpCodecInfos = NULL;


/*
 * Life cycle
 */
nglImage::nglImage (nglIStream* pInput, nglImageCodec* pCodec)
{
  StaticInit();
  mpCodec = pCodec;
  mOwnCodec = (pCodec == NULL);

  if (!mpCodec)
  {
    uint32 count;
    count = mpCodecInfos->size();
    for (uint32 i=0; i<count && !mpCodec; i++)
    {
      if ((*mpCodecInfos)[i])
      {
        mpCodec = (*mpCodecInfos)[i]->CreateInstance(); // try to create a codec
        if (mpCodec) // success?
        {
          if (!mpCodec->Probe(pInput)) // try to make the codec recognize the data.
          { // :-(
            delete mpCodec;
            mpCodec = NULL;
          }
        }
      }
    }
  }

  if (mpCodec)
  {
    mpCodec->Init(this);
    mpCodec->Feed(pInput);
    if (mOwnCodec)
    {
      delete mpCodec;
      mpCodec = NULL;
      mOwnCodec = false;
    }
  }

  if (IsValid() && !mInfo.mPreMultAlpha)
    PreMultiply();
}

nglImage::nglImage (const nglPath& rPath, nglImageCodec* pCodec )
{
  StaticInit();
  mpCodec = pCodec;
  mOwnCodec = (pCodec == NULL);

  nglIStream* pIFile = rPath.OpenRead();
  if (!pIFile)
  {
    return;
  }
  
  if (pIFile->GetState() != eStreamReady)
  {
    delete pIFile;
    return;
  }

  if (!mpCodec)
  {
    uint32 count;
    count = mpCodecInfos->size();
    for (uint32 i=0; i<count && !mpCodec; i++)
    {
      if ((*mpCodecInfos)[i])
      {
        mpCodec = (*mpCodecInfos)[i]->CreateInstance(); // try to create a codec
        if (mpCodec) // success?
        {
          if (!mpCodec->Probe(pIFile)) // try to make the codec recognize the data.
          { // :-(
            delete mpCodec;
            mpCodec = NULL;
          }
        }
      }
    }
  }

  if (!mpCodec) // If not codec was able to detect the image format we try to match the file with its extension.
  {
    uint32 count = mpCodecInfos->size();
    nglString filename = rPath.GetPathName();
    for (uint32 i=0; i<count && !mpCodec; i++)
    {
      if ((*mpCodecInfos)[i])
      {
        if (!((*mpCodecInfos)[i]->ExtensionMatch (filename))) // success?
        {
          // :-(
          delete mpCodec;
          mpCodec = NULL;
        }
      }
    }
  }

  if (mpCodec)
  {
    mpCodec->Init(this);
    mpCodec->Feed(pIFile);
    if (mOwnCodec)
    {
      delete mpCodec;
      mpCodec = NULL;
      mOwnCodec = false;
    }
  }
  
  delete pIFile;

  if (IsValid() && !mInfo.mPreMultAlpha)
    PreMultiply();
}

nglImage::nglImage(nglImageInfo& rInfo, nuiCopyPolicy policy)
{
  StaticInit();
  if (policy == eTransfert)
    mInfo.Transfert(rInfo);
  else
  {
    bool clone = (policy == eClone);
    mInfo.Copy(rInfo, clone);
  }
  mpCodec = NULL;
  mOwnCodec = true;
  mCompletion = (mInfo.mpBuffer) ? 1.0f : 0.0f;

  if (IsValid() && !rInfo.mPreMultAlpha)
    PreMultiply();
}

nglImage::nglImage (const nglImage& rImage)
{
  StaticInit();
  mInfo.Copy(rImage.mInfo,true); // Clone image buffer
  mpCodec = NULL;                // Don't share the codec, and don't bother making a copy
  mOwnCodec = true;
  mCompletion = rImage.mCompletion;
}


bool nglImage::IsValid() const
{
  return GetBuffer() != NULL;
}






template <int32 depth>
uint32 average_pixels(uint32 a, uint32 b)
{
  if (depth == 32 || depth == 24)
  {
    const uint32 a0 = (a & 0x00ff00ff);
    const uint32 b0 = (b & 0x00ff00ff);
    
    const uint32 a1 = (a & 0xff00ff00) >> 1;
    const uint32 b1 = (b & 0xff00ff00) >> 1;
    
    return (((a0 + b0) >> 1) & 0x00ff00ff) + ((a1 + b1) & 0xff00ff00);
  }
  else if (depth == 16)
  {
    return a;    
  }
  
  return (a + b) / 2;
}

template <int32 depth>
void AdvancePtr(const uint8*& pSource, int32 set)
{
  pSource += set * (depth / 8);
}

template <int32 depth>
void AdvancePtr(uint8*& pSource, int32 set)
{
  pSource += set * (depth / 8);
}

template <int32 depth>
void CopyPixel(uint8* pTarget, uint32 pixel)
{
  if (depth == 32)
  {
    *(uint32*)pTarget = pixel;
  }
  else
  {
    for (uint32 i = 0; i < depth / 8; i++)
    {
      *pTarget++ = pixel & 0xff;
      pixel >>= 8;
    }
  }
}

template <int32 depth>
void CopyPixel(uint8*& pTarget, uint32 pixel, bool adv)
{
  CopyPixel<depth>(pTarget, pixel);  
  if (adv)
    AdvancePtr<depth>(pTarget, 1);
}

template <int32 depth>
uint32 GetPixel(const uint8* pSrc)
{
  if (depth == 32)
  {
    return *(uint32*)pSrc;
  }
  else if (depth == 24)
  {
    return (*(uint32*)pSrc) & 0xffffff;
  }
  else if (depth == 16)
  {
    return (*(uint16*)pSrc);
  }
  
  return *pSrc;
}


template <int32 depth>
void ScaleLineAvg(uint8* pTarget, int32 TgtWidth, const uint8* pSource, int32 SrcWidth)
{
  int32 NumPixels = TgtWidth;
  int32 IntPart   = SrcWidth / TgtWidth;
  int32 FractPart = SrcWidth % TgtWidth;
  int32 Mid       = TgtWidth / 2;
  int32 E         = 0;
  int32 skip;
  uint32 p;
  
  skip = (TgtWidth < SrcWidth) ? 0 : TgtWidth / (2 * SrcWidth) + 1;
  NumPixels -= skip;
  
  while (NumPixels-- > 0) 
  {
    p = GetPixel<depth>(pSource);
    
    if (E >= Mid)
      p = average_pixels<depth>(p, GetPixel<depth>(pSource + (depth/8)));
    
    CopyPixel<depth>(pTarget, p, true);
    AdvancePtr<depth>(pSource, IntPart);
    
    E += FractPart;
    
    if (E >= TgtWidth) 
    {
      E -= TgtWidth;
      AdvancePtr<depth>(pSource, 1);
    } 
  } 
  
  skip *= (depth / 8);
  while (skip-- > 0)
  {
    *pTarget++ = *pSource;
  }
}



template <int32 depth>
void ScaleRectAvg(uint8* pTarget, int32 TgtWidth, int32 TgtHeight,
                  const uint8* pSource, int32 SrcWidth, int32 SrcHeight)
{
  int32 NumPixels = TgtHeight;
  int32 IntPart   = (SrcHeight / TgtHeight) * SrcWidth;
  int32 FractPart = SrcHeight % TgtHeight;
  int32 Mid       = TgtHeight / 2;
  int32 E         = 0;
  const uint8 *pPrevSource = NULL;
  const uint8 *pPrevSourceAhead = NULL;
  const uint32 bpp = depth / 8;
  
  int32 skip = (TgtHeight < SrcHeight) ? 0 : TgtHeight / (2 * SrcHeight) + 1;
  NumPixels -= skip;
  
  uint8* pScanLine      = new uint8[TgtWidth * bpp];
  uint8* pScanLineAhead = new uint8[TgtWidth * bpp];
  
  while (NumPixels-- > 0) 
  {
    if (pSource != pPrevSource) 
    {
      if (pSource == pPrevSourceAhead) 
      {
        /* the next scan line has already been scaled and stored in
         * ScanLineAhead; swap the buffers that ScanLine and ScanLineAhead
         * point to
         */
        uint8* tmp = pScanLine;
        pScanLine      = pScanLineAhead;
        pScanLineAhead = tmp;
      }
      else 
      {
        ScaleLineAvg<depth>(pScanLine, TgtWidth, pSource, SrcWidth);
      }
      
      pPrevSource = pSource;
    }
    
    if (E >= Mid && pPrevSourceAhead != pSource + (SrcWidth * bpp)) 
    {
      int32 x;
      ScaleLineAvg<depth>(pScanLineAhead, TgtWidth, pSource + (SrcWidth * bpp), SrcWidth);
      
      for (x = 0; x < TgtWidth; x++)
      {
        const uint32 pix = average_pixels<depth>(GetPixel<depth>(pScanLine + x * bpp), GetPixel<depth>(pScanLineAhead + x * bpp));
        CopyPixel<depth>(pScanLine + x * bpp, pix);
      }
      
      pPrevSourceAhead = pSource + SrcWidth * bpp;
    } 
    
    memcpy(pTarget, pScanLine, TgtWidth * bpp);
    pTarget += TgtWidth * bpp;
    pSource += IntPart * bpp;
    E       += FractPart;
    
    if (E >= TgtHeight) 
    {
      E -= TgtHeight;
      pSource += SrcWidth * bpp;
    } 
  } 
  
  if (skip > 0 && pSource != pPrevSource)
    ScaleLineAvg<depth>(pScanLine, TgtWidth, pSource, SrcWidth);
  
  while (skip-- > 0) 
  {
    memcpy(pTarget, pScanLine, TgtWidth * bpp);
    pTarget += TgtWidth * bpp;
  }
  
  delete pScanLine;
  delete pScanLineAhead;
}





nglImage::nglImage(const nglImage& rImage, uint32 NewWidth, uint32 NewHeight)
{
  StaticInit();
  mInfo.Copy(rImage.mInfo, false); // don't Clone image buffer
  mInfo.mWidth = NewWidth;
  mInfo.mHeight = NewHeight;
  mInfo.mBytesPerLine = NewWidth * mInfo.mBytesPerPixel;
  mInfo.AllocateBuffer();

  mpCodec = NULL;                // Don't share the codec, and don't bother making a copy
  mOwnCodec = true;
  mCompletion = rImage.mCompletion;
  
  nglImageInfo sourceInfo;
  rImage.GetInfo(sourceInfo);
  
//  (dh_pos, mInfo.mWidth * 3, mInfo.mHeight, sh_pos, sourceInfo.mWidth *3, sourceInfo.mHeight);
  
  switch (mInfo.mBitDepth)
  {
    case 8:
      ScaleRectAvg<8>((uint8*)GetBuffer(), NewWidth, NewHeight,
                      (uint8*)rImage.GetBuffer(), sourceInfo.mWidth, sourceInfo.mHeight);  
      break;
    case 16:
      ScaleRectAvg<16>((uint8*)GetBuffer(), NewWidth, NewHeight,
                       (uint8*)rImage.GetBuffer(), sourceInfo.mWidth, sourceInfo.mHeight);  
      break;
    case 24:
      ScaleRectAvg<24>((uint8*)GetBuffer(), NewWidth, NewHeight,
                       (uint8*)rImage.GetBuffer(), sourceInfo.mWidth, sourceInfo.mHeight);  
      break;
    case 32:
      ScaleRectAvg<32>((uint8*)GetBuffer(), NewWidth, NewHeight,
                       (uint8*)rImage.GetBuffer(), sourceInfo.mWidth, sourceInfo.mHeight);  
      break;
  }
  
}
  
  








nglImage::~nglImage()
{
  // Image buffer is released by mpInfo destruction
  if (mpCodec && mOwnCodec) 
    delete mpCodec;
}


/** This dummy class only exists to read image infos without the data... */
class nglImageDummy : public nglImage
{
public:
  nglImageDummy(nglIStream* pInput, nglImageCodec* pCodec)
  : nglImage(pInput, pCodec)
  {
    
  }
  
  bool OnInfo(nglImageInfo& rInfo)
  {
    nglImage::OnInfo(rInfo);
    return false;
  }
};

bool nglImage::GetImageInfo(nglImageInfo& rInfo, nglIStream* pIFile, nglImageCodec* pCodec)
{
  StaticInit();
  
  if (!pIFile)
    return false;
  
  if (pIFile->GetState() != eStreamReady)
  {
    delete pIFile;
    return false;
  }
  
  if (!pCodec)
  {
    uint32 count;
    count = mpCodecInfos->size();
    for (uint32 i=0; i<count && !pCodec; i++)
    {
      if ((*mpCodecInfos)[i])
      {
        pCodec = (*mpCodecInfos)[i]->CreateInstance(); // try to create a codec
        if (pCodec) // success?
        {
          if (!pCodec->Probe(pIFile)) // try to make the codec recognize the data.
          { // :-(
            delete pCodec;
            pCodec = NULL;
          }
        }
      }
    }
  }
    
  if (pCodec)
  {
    nglImageDummy img(pIFile, pCodec);
    
    pCodec->Init(&img);
    pCodec->Feed(pIFile); // Should return as soon as the header is read
    
    img.GetInfo(rInfo);
  }

  delete pCodec;
}

bool nglImage::GetImageInfo(nglImageInfo& rInfo, const nglPath& rPath, nglImageCodec* pCodec)
{
  nglIStream* pIFile = rPath.OpenRead();
  bool res = GetImageInfo(rInfo, pIFile, pCodec);
  delete pIFile;
  return res;
}


/*
 * Image description
 */

bool nglImage::GetInfo (nglImageInfo& rInfo) const
{
  rInfo.Copy (mInfo, false);
  return true;
}

nglImageBufferFormat nglImage::GetBufferFormat() const
{
  return mInfo.mBufferFormat;
}

nglImagePixelFormat nglImage::GetPixelFormat() const
{
  return mInfo.mPixelFormat;
}

uint32 nglImage::GetWidth() const
{
  return mInfo.mWidth;
}

uint32 nglImage::GetHeight() const
{
  return mInfo.mHeight;
}

uint32 nglImage::GetBitDepth() const
{
  return mInfo.mBitDepth;
}

uint32 nglImage::GetPixelSize() const
{
  return mInfo.mBytesPerPixel;
}

uint32 nglImage::GetBytesPerLine() const
{
  return mInfo.mBytesPerLine;
}

char* nglImage::GetBuffer() const
{
  return mInfo.mpBuffer;
}

nglImageCodec* nglImage::GetCodec() const
{
  return mpCodec;
}


/*
 * Export (save)
 */

bool nglImage::Save (nglOStream* pOutput, nglImageCodec* pCodec)
{
  if (!pOutput)
    return false;
  if (!pCodec)
    return false;

  pCodec->Init(this);
  return pCodec->Save(pOutput);
}

bool nglImage::Save (const nglPath& filename, nglImageCodec* pCodec)
{
  nglOFile file(filename,eOFileCreate);
  return Save(&file,pCodec);
}







/*
 * image process
 */

nglImage* nglImage::Resize(uint32 width, uint32 height)
{
  // check
  if ((mInfo.mWidth <= 0)|| (mInfo.mHeight <= 0))
    return NULL;
  
  // init new image params.
  nglImageInfo newInfo;
  newInfo.Copy(mInfo, false/* don't clone*/);
  newInfo.mWidth = width;
  newInfo.mHeight = height;
  newInfo.mBytesPerLine = newInfo.mWidth * newInfo.mBytesPerPixel;
  newInfo.mOwnBuffer = false;
  newInfo.AllocateBuffer();

  // build new image
  nglImage* pNew = new nglImage(newInfo, eTransfert);
  
  switch (mInfo.mBitDepth)
  {
    case 8:
      ScaleRectAvg<8>((uint8*)pNew->GetBuffer(), newInfo.mWidth, newInfo.mHeight,
                      (uint8*)GetBuffer(), mInfo.mWidth, mInfo.mHeight);  
      break;
    case 16:
      ScaleRectAvg<16>((uint8*)pNew->GetBuffer(), newInfo.mWidth, newInfo.mHeight,
                       (uint8*)GetBuffer(), mInfo.mWidth, mInfo.mHeight);  
      break;
    case 24:
      ScaleRectAvg<24>((uint8*)pNew->GetBuffer(), newInfo.mWidth, newInfo.mHeight,
                       (uint8*)GetBuffer(), mInfo.mWidth, mInfo.mHeight);  
      break;
    case 32:
      ScaleRectAvg<32>((uint8*)pNew->GetBuffer(), newInfo.mWidth, newInfo.mHeight,
                       (uint8*)GetBuffer(), mInfo.mWidth, mInfo.mHeight);  
      break;
  }
  
  return pNew;
}


nglImage* nglImage::Crop(uint32 x, uint32 y, uint32 width, uint32 height)
{
  // check
  if (((x+width) > mInfo.mWidth)|| ((y+height) > mInfo.mHeight))
    return NULL;
  
  // init new image params.
  nglImageInfo newInfo;
  newInfo.Copy(mInfo, false/* don't clone*/);
  newInfo.mWidth = width;
  newInfo.mHeight = height;
  newInfo.mBytesPerLine = newInfo.mWidth * newInfo.mBytesPerPixel;
  newInfo.mOwnBuffer = false;
  newInfo.AllocateBuffer();
  
  // build new image
  nglImage* pNew = new nglImage(newInfo, eTransfert);
  
  char* pSrc = GetBuffer() + (y * mInfo.mBytesPerLine) + (x * mInfo.mBytesPerPixel);
  char* pDst = pNew->GetBuffer();
  
  for (uint32 i = 0; i < height; i++, pSrc += mInfo.mBytesPerLine, pDst += newInfo.mBytesPerLine)
  {
    memcpy(pDst, pSrc, width * newInfo.mBytesPerPixel);
  }
  
  return pNew;
}

nglImage* nglImage::Trim(int32& rXOffset, int32& rYOffset)
{
  int32 x = 0, y = 0, width = GetWidth(), height = GetHeight();

  // Eat transparent pixels from the top:
  {
    bool stop = false;
    for (int32 Y = 0; Y < GetHeight() && !stop; Y++)
    {
      for (int32 X = 0; X < GetWidth() && !stop; X++)
      {
        if (GetPixel(X, Y).mAlpha != 0)
        {
          stop = true;
          y = Y;
        }
      }
    }
  }
  
  // Eat transparent pixels from the bottom:
  {
    bool stop = false;
    for (int32 Y = GetHeight() - 1; Y > y && !stop; Y--)
    {
      for (int32 X = 0; X < GetWidth() && !stop; X++)
      {
        if (GetPixel(X, Y).mAlpha != 0)
        {
          stop = true;
          height = Y - y;
        }
      }
    }
  }
  
  // Eat transparent pixels from the left:
  {
    bool stop = false;
    for (int32 X = 0; X < GetWidth() && !stop; X++)
    {
      for (int32 Y = y; Y < y + height && !stop; Y++)
      {
        if (GetPixel(X, Y).mAlpha != 0)
        {
          stop = true;
          x = X;
        }
      }
    }
  }
  
  // Eat transparent pixels from the right:
  {
    bool stop = false;
    for (int32 X = GetWidth(); X > x && !stop; X--)
    {
      for (int32 Y = y; Y < y + height && !stop; Y++)
      {
        if (GetPixel(X, Y).mAlpha != 0)
        {
          stop = true;
          width = X - x;
        }
      }
    }
  }
  
  rXOffset = x;
  rYOffset = y;
  return Crop(x, y, width, height);
}



nglImage* nglImage::RotateLeft()
{
  int32 width = GetWidth();
  int32 height = GetHeight();
  int32 bpp = (GetBitDepth() + 1) / 8;
  
  nglImageInfo info(height, width, GetBitDepth());
  info.AllocateBuffer();
  
  int32 sbpl = GetBytesPerLine();
  int32 dbpl = info.mBytesPerLine;

  char* pDst = info.mpBuffer;
  char* pSrc = GetBuffer();
  
  if (mInfo.mBitDepth == 8)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = j * bpp + dbpl * (width - 1 - i);
        pDst[doff] = pSrc[soff];
      }
    }
  }
  else if (mInfo.mBitDepth == 16)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = j * bpp + dbpl * (width - 1 - i);
        pDst[doff++] = pSrc[soff++];
        pDst[doff] = pSrc[soff];
      }
    }
  }
  else if (mInfo.mBitDepth == 24)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = j * bpp + dbpl * (width - 1 - i);
        pDst[doff++] = pSrc[soff++];
        pDst[doff++] = pSrc[soff++];
        pDst[doff] = pSrc[soff];
      }
    }
  }
  else if (mInfo.mBitDepth == 32)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = j * bpp + dbpl * (width - 1 - i);
        pDst[doff++] = pSrc[soff++];
        pDst[doff++] = pSrc[soff++];
        pDst[doff++] = pSrc[soff++];
        pDst[doff] = pSrc[soff];
      }
    }
  }
  
  return new nglImage(info);
}

nglImage* nglImage::RotateRight()
{
  int32 width = GetWidth();
  int32 height = GetHeight();
  int32 bpp = (GetBitDepth() + 1) / 8;
  
  nglImageInfo info(height, width, GetBitDepth());
  info.AllocateBuffer();
  
  int32 sbpl = GetBytesPerLine();
  int32 dbpl = info.mBytesPerLine;
  
  char* pDst = info.mpBuffer;
  char* pSrc = GetBuffer();
  
  if (mInfo.mBitDepth == 8)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = (height - 1 - j) * bpp + dbpl * i;
        pDst[doff] = pSrc[soff];
      }
    }
  }
  else if (mInfo.mBitDepth == 16)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = (height - 1 - j) * bpp + dbpl * i;
        pDst[doff++] = pSrc[soff++];
        pDst[doff] = pSrc[soff];
      }
    }
  }
  else if (mInfo.mBitDepth == 24)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = (height - 1 - j) * bpp + dbpl * i;
        pDst[doff++] = pSrc[soff++];
        pDst[doff++] = pSrc[soff++];
        pDst[doff] = pSrc[soff];
      }
    }
  }
  else if (mInfo.mBitDepth == 32)
  {
    for (int32 i = 0; i < width; i++)
    {
      for (int32 j = 0; j < height; j++)
      {
        int32 soff = i * bpp + sbpl * j;
        int32 doff = (height - 1 - j) * bpp + dbpl * i;
        pDst[doff++] = pSrc[soff++];
        pDst[doff++] = pSrc[soff++];
        pDst[doff++] = pSrc[soff++];
        pDst[doff] = pSrc[soff];
      }
    }
  }
  
  return new nglImage(info);
}


void nglImage::PreMultiply()
{
  if (mInfo.mPreMultAlpha)
    return;
  
  if (mInfo.mBitDepth == 16 && mInfo.mPixelFormat == eImagePixelLumA)
  {
    nglPreMultLine16LumA(GetBuffer(), GetBuffer(), mInfo.mWidth * mInfo.mHeight);
  }
  else if (mInfo.mBitDepth == 32 && mInfo.mPixelFormat == eImagePixelRGBA)
  {
    nglPreMultLine32RGBA(GetBuffer(), GetBuffer(), mInfo.mWidth * mInfo.mHeight);
  }

  mInfo.mPreMultAlpha = true;
}

void nglImage::UnPreMultiply()
{
  if (!mInfo.mPreMultAlpha)
    return;
  
  if (mInfo.mBitDepth == 16 && mInfo.mPixelFormat == eImagePixelLumA)
  {
    nglUnPreMultLine16LumA(GetBuffer(), GetBuffer(), mInfo.mWidth * mInfo.mHeight);
  }
  else if (mInfo.mBitDepth == 32 && mInfo.mPixelFormat == eImagePixelRGBA)
  {
    nglUnPreMultLine32RGBA(GetBuffer(), GetBuffer(), mInfo.mWidth * mInfo.mHeight);
  }
  
  mInfo.mPreMultAlpha = false;
}


/*
 * User callbacks
 */

bool nglImage::OnInfo (nglImageInfo& rInfo)
{
//  rInfo.Copy (mInfo, false);
  return true;
}

bool nglImage::OnData(float Completion)
{
  mCompletion = Completion;
  return true;
}

bool nglImage::OnError()
{
  return false; // Abort on error as a default (user overridable)
}


/*
 * Global codec registry
 */

void nglImage::StaticInit()
{
  if (mpCodecInfos == NULL)
  {
    mpCodecInfos = new std::vector<nglImageCodecInfo*>();
    App->AddExit(StaticExit);
    mpCodecInfos->push_back(new nglImageTGACodecInfo());
    mpCodecInfos->push_back(new nglImagePPMCodecInfo());

#ifdef HAVE_COREGRAPHICS
    mpCodecInfos->push_back(new nglImageCGCodecInfo());
#endif
    
    mpCodecInfos->push_back(new nglImagePNGCodecInfo());

  #ifdef HAVE_LIBJPEG
    mpCodecInfos->push_back(new nglImageJPEGCodecInfo());
  #endif
    mpCodecInfos->push_back(new nglImageGIFCodecInfo());

    
  }
}

void nglImage::StaticExit()
{
  if (!mpCodecInfos)
    return;
  std::vector<nglImageCodecInfo*>::iterator i;

  for (i = mpCodecInfos->begin(); i != mpCodecInfos->end(); ++i)
  {
    nglImageCodecInfo* info = *i;
    if (info)
    {
      delete info;
      info = NULL;
    }
  }
  delete mpCodecInfos;
  mpCodecInfos = NULL;
}

bool nglImage::AddCodec (nglImageCodecInfo* pCodecInfo)
{
  StaticInit();
  if (pCodecInfo)
  {
    mpCodecInfos->push_back (pCodecInfo);
    return true;
  }
  return false;
}

bool nglImage::DelCodec (nglImageCodecInfo* pCodecInfo)
{
  StaticInit();
  std::vector<nglImageCodecInfo*>::iterator i;

  for (i = mpCodecInfos->begin(); i != mpCodecInfos->end(); ++i)
  {
    if (*i == pCodecInfo)
    {
      mpCodecInfos->erase (i);
      return true;
    }
  }
  return false;
}

nglImageCodec* nglImage::CreateCodec (int32 Index)
{
  StaticInit();
  nglImageCodecInfo* info = (*mpCodecInfos)[Index];
  return (info) ? info->CreateInstance() : NULL;
}

nglImageCodec* nglImage::CreateCodec (const nglString& rName)
{
  StaticInit();
  std::vector<nglImageCodecInfo*>::iterator i;

  for (i = mpCodecInfos->begin(); i != mpCodecInfos->end(); ++i)
  {
    nglImageCodecInfo* info = *i;
    if (info && (info->mName == rName))
      return info->CreateInstance();
  }
  return NULL;
}


/*
 * Codec callbacks
 */

bool nglImage::OnCodecInfo(nglImageInfo& rInfo)
{
  mInfo.Copy(rInfo, false);
  mInfo.AllocateBuffer();
  return OnInfo (mInfo);
}

bool nglImage::OnCodecData(float Completion)
{
  return OnData(Completion);
}

bool nglImage::OnCodecError()
{
  return OnError();
}

/* nglError callback
 */
const nglChar* nglImage::OnError (uint& rError) const
{
  return FetchError(gpImageErrorTable, NULL, rError);
}

/* Image Buffer management
 */
void nglImage::ReleaseBuffer()
{
  mInfo.ReleaseBuffer();
}






/* Image Pixel Access:
 */

nglImage::Color nglImage::GetPixel(uint32 X, uint32 Y) const
{
  Color col;
  GetPixel(X,Y,col);
  return col;
}

void nglImage::GetPixel(uint32 X, uint32 Y, nglImage::Color& rColor) const
{
  uint32 index = mInfo.mBytesPerPixel * X + Y * mInfo.mBytesPerLine;
  uint8* bytes = (uint8*)mInfo.mpBuffer;

  switch (mInfo.mBitDepth)
  {
  case 8:
    {
      // Lum or A
      switch (mInfo.mPixelFormat)
      {
      case eImagePixelNone:
      case eImagePixelIndex:
      case eImagePixelRGB:
      case eImagePixelRGBA:
      case eImagePixelLumA:
        NGL_ASSERT(0);
        break;
      case eImagePixelLum:
        rColor.mRed   = rColor.mGreen = rColor.mBlue = bytes[index] / 255.0f;
        rColor.mAlpha = 1.0f;
        break;
      case eImagePixelAlpha:
        rColor.mRed   = rColor.mGreen = rColor.mBlue = 1.0f;
        rColor.mAlpha = bytes[index] / 255.0f;
        break;
      }
    }
    break;
  case 16:
    {
      // RGB or RGBA or LumA
      switch (mInfo.mPixelFormat)
      {
      case eImagePixelNone:
      case eImagePixelIndex:
      case eImagePixelLum:
      case eImagePixelAlpha:
        NGL_ASSERT(0);
        break;
      case eImagePixelRGB:
        rColor.mAlpha = 1.0f; // #FIXME
        break;
      case eImagePixelRGBA:
        rColor.mAlpha = 1.0f; // #FIXME
        break;
      case eImagePixelLumA:
        rColor.mRed   = rColor.mGreen = rColor.mBlue = bytes[index];
        rColor.mAlpha = bytes[index + 1];
        break;
      }
    }
    break;
  case 24:
    {
      // RGB
      rColor.mRed   = bytes[index]      / 255.0f;
      rColor.mGreen = bytes[index + 1]  / 255.0f;
      rColor.mBlue  = bytes[index + 2]  / 255.0f;
      rColor.mAlpha = 1.0f;
    }
    break;
  case 32:
    {
      //RGBA
      rColor.mRed   = bytes[index]      / 255.0f;
      rColor.mGreen = bytes[index + 1]  / 255.0f;
      rColor.mBlue  = bytes[index + 2]  / 255.0f;
      rColor.mAlpha = bytes[index + 3]  / 255.0f;
    }
    break;
  }
}

void nglImage::SetPixel(uint32 X, uint32 Y, const nglImage::Color& rColor)
{
  uint32 index = mInfo.mBytesPerPixel * X + Y * mInfo.mBytesPerLine;
  uint8* bytes = (uint8*)mInfo.mpBuffer;

  switch (mInfo.mBitDepth)
  {
  case 8:
    {
      // Lum or A
      switch (mInfo.mPixelFormat)
      {
      case eImagePixelNone:
      case eImagePixelIndex:
      case eImagePixelRGB:
      case eImagePixelRGBA:
      case eImagePixelLumA:
        NGL_ASSERT(0);
        break;
      case eImagePixelLum:
        bytes[index] = ToBelow((rColor.mRed + rColor.mGreen + rColor.mBlue) * 255.0f);
        break;
      case eImagePixelAlpha:
        bytes[index] = ToBelow(rColor.mAlpha * 255.0f);
        break;
      }
    }
    break;
  case 16:
    {
      // RGB or RGBA or LumA
      switch (mInfo.mPixelFormat)
      {
      case eImagePixelNone:
      case eImagePixelIndex:
      case eImagePixelLum:
      case eImagePixelAlpha:
        NGL_ASSERT(0);
        break;
      case eImagePixelRGB:
        bytes[index]      = 0; // #FIXME
        bytes[index + 1]  = 0; // #FIXME
        NGL_ASSERT(0);
        break;
      case eImagePixelRGBA:
        bytes[index]      = 0; // #FIXME
        bytes[index + 1]  = 0; // #FIXME
        NGL_ASSERT(0);
        break;
      case eImagePixelLumA:
        bytes[index]      = ToBelow((rColor.mRed + rColor.mGreen + rColor.mBlue) * 255.0f);
        bytes[index + 1]  = ToBelow(rColor.mAlpha * 255.0f);
        break;
      }
    }
    break;
  case 24:
    {
      // RGB
      bytes[index]     = ToBelow(rColor.mRed   * 255.0f);
      bytes[index + 1] = ToBelow(rColor.mGreen * 255.0f);
      bytes[index + 2] = ToBelow(rColor.mBlue  * 255.0f);
    }
    break;
  case 32:
    {
      //RGBA
      bytes[index]     = ToBelow(rColor.mRed   * 255.0f);
      bytes[index + 1] = ToBelow(rColor.mGreen * 255.0f);
      bytes[index + 2] = ToBelow(rColor.mBlue  * 255.0f);
      bytes[index + 3] = ToBelow(rColor.mAlpha * 255.0f);
    }
    break;
  }
}

