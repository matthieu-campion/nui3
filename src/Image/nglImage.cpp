/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#include "nglKernel.h"
#include "nglFile.h"
#include "nglIFile.h"
#include "nglOFile.h"
#include "nglImage.h"
#include "nglImageCodec.h"
#include "nglMath.h"

#ifndef _UIKIT_
#include "nglImageTGACodec.h"
#include "nglImagePPMCodec.h"
#endif

#ifdef HAVE_COREGRAPHICS
#include "nglImageCGCodec.h"
#endif
#ifdef HAVE_LIBPNG
#include "nglImagePNGCodec.h"
#endif
#ifdef HAVE_LIBJPEG
#include "nglImageJPEGCodec.h"
#endif


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

nglImageInfo::nglImageInfo(bool Managed)
{
  mBufferFormat = eImageFormatNone;
  mPixelFormat  = eImagePixelNone;
  mWidth = 0;
  mHeight = 0;
  mBitDepth = 0;
  mBytesPerPixel = 0;
  mBytesPerLine = 0;
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
    mpBuffer = (char*) calloc (mHeight, mBytesPerLine);
/*
    if (!mpBuffer)
      SetError ("image", NGL_IMAGE_EOOM);
*/
  }
  mOwnBuffer = true; // Now it's our own (managed) buffer (even if the alloc failed)
  return (mpBuffer != NULL);
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
  Init();
  mpCodec = pCodec;
  mOwnCodec = (pCodec == NULL);

  if (!mpCodec)
  {
    uint count;
    count = mpCodecInfos->size();
    for (uint i=0; i<count && !mpCodec; i++)
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

}

nglImage::nglImage (const nglPath& rPath, nglImageCodec* pCodec )
{
  Init();
  mpCodec = pCodec;
  mOwnCodec = (pCodec == NULL);

  nglIStream* pIFile = rPath.OpenRead();
  if (!pIFile)
    return;
  
  if (pIFile->GetState() != eStreamReady)
  {
    delete pIFile;
    return;
  }

  if (!mpCodec)
  {
    uint count;
    count = mpCodecInfos->size();
    for (uint i=0; i<count && !mpCodec; i++)
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
    uint count = mpCodecInfos->size();
    nglString filename = rPath.GetPathName();
    for (uint i=0; i<count && !mpCodec; i++)
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
}

nglImage::nglImage (nglImageInfo& rInfo, bool Clone)
{
  Init();
  mInfo.Copy(rInfo,Clone);
  mpCodec = NULL;
  mOwnCodec = true;
  mCompletion = (mInfo.mpBuffer) ? 1.0f : 0.0f;
}

nglImage::nglImage (const nglImage& rImage)
{
  Init();
  mInfo.Copy(rImage.mInfo,true); // Clone image buffer
  mpCodec = NULL;                // Don't share the codec, and don't bother making a copy
  mOwnCodec = true;
  mCompletion = rImage.mCompletion;
}






//
//#define BRESHENAM2D(copyfunc)                              \
//  if (sh<dh)																							 \
//    for (j=0; j<=dh; j++) 																 \
//    { 																										 \
//                                                           \
//      w_err=0;																						 \
//      sw_pos=sh_pos;																			 \
//      dw_pos=dh_pos;																			 \
//      if (sw<dw)																					 \
//        for (i=0; i<=dw; i++) 														 \
//        { 																								 \
//          copyfunc                            						 \
//          dw_pos+=dw_inc; 																 \
//          w_err+=sw;																			 \
//          if (w_err>=dw) { sw_pos+=sw_inc; w_err-=dw; } 	 \
//        } 																								 \
//      else																								 \
//        for (i=0; i<=sw; i++) 														 \
//        { 																								 \
//          copyfunc                                    		 \
//          sw_pos+=sw_inc; 																 \
//          w_err+=dw;																			 \
//          if (w_err>=sw) { dw_pos+=dw_inc; w_err-=sw; } 	 \
//        } 																								 \
//                                                           \
//      dh_pos+=dh_inc; 																		 \
//      h_err+=sh;																					 \
//      if (h_err>=dh) { sh_pos+=sh_inc; h_err-=dh; } 			 \
//    } 																										 \
//  else																										 \
//    for (j=0; j<=sh; j++) 																 \
//    { 																										 \
//                                                           \
//      w_err=0;																						 \
//      sw_pos=sh_pos;																			 \
//      dw_pos=dh_pos;																			 \
//      if (sw<dw)																					 \
//        for (i=0; i<=dw; i++) 														 \
//        { 																								 \
//          copyfunc                                      	 \
//          dw_pos+=dw_inc; 																 \
//          w_err+=sw;																			 \
//          if (w_err>=dw) { sw_pos+=sw_inc; w_err-=dw; } 	 \
//        } 																								 \
//      else																								 \
//        for (i=0; i<=sw; i++) 														 \
//        { 																								 \
//          copyfunc                            						 \
//          sw_pos+=sw_inc; 																 \
//          w_err+=dw;																			 \
//          if (w_err>=sw) { dw_pos+=dw_inc; w_err-=sw; } 	 \
//        } 																								 \
//                                                           \
//      sh_pos+=sh_inc; 																		 \
//      h_err+=dh;																					 \
//      if (h_err>=sh) { dh_pos+=dh_inc; h_err-=sh; } 			 \
//    }
//
//
//
//void nglImage::Breshenam24(char* dh_pos, uint32 dw, uint32 dh, uint32 dw_inc, uint32 dh_inc, char* sh_pos, uint32 sw, uint32 sh, uint32 sw_inc, uint32 sh_inc)
//{
//uint32 w_err, h_err, i, j;
//char* sw_pos;
//char* dw_pos;
//  
//  if (sh < dh)																							 
//    for (j=0; j <= dh; j++) 																 
//    { 																										 
//      w_err = 0;																						 
//      sw_pos = sh_pos;																			 
//      dw_pos = dh_pos;																			 
//      if (sw < dw)																					 
//        for (i=0; i <= dw; i++) 														 
//        { 																								 
//          *(char*)dw_pos=*(char*)sw_pos;
//          *(((char*)dw_pos)+1)=*(((char*)sw_pos)+1);
//          *(((char*)dw_pos)+2)=*(((char*)sw_pos)+2);
//
//          dw_pos+=dw_inc; 																 
//          w_err+=sw;																			 
//          if (w_err >= dw) { sw_pos+=sw_inc; w_err-=dw; } 	 
//        } 																								 
//      else																								 
//        for (i=0; i <= sw; i++) 														 
//        { 																								 
//          *(char*)dw_pos=*(char*)sw_pos;
//          *(((char*)dw_pos)+1)=*(((char*)sw_pos)+1);
//          *(((char*)dw_pos)+2)=*(((char*)sw_pos)+2);
//
//          sw_pos+=sw_inc; 																 
//          w_err+=dw;																			 
//          if (w_err >= sw) { dw_pos+=dw_inc; w_err-=sw; } 	 
//        } 																								 
//      
//      dh_pos += dh_inc; 																		 
//      h_err += sh;																					 
//      if (h_err >= dh) { sh_pos+=sh_inc; h_err-=dh; } 			 
//    } 																										 
//  else																										 
//    for (j=0; j <= sh; j++) 																 
//    { 																										 
//      
//      w_err = 0;																						 
//      sw_pos = sh_pos;																			 
//      dw_pos = dh_pos;																			 
//      if (sw < dw)																					 
//        for (i=0; i <= dw; i++) 														 
//        { 																								 
//          *(char*)dw_pos=*(char*)sw_pos;
//          *(((char*)dw_pos)+1)=*(((char*)sw_pos)+1);
//          *(((char*)dw_pos)+2)=*(((char*)sw_pos)+2);
//
//          dw_pos+=dw_inc; 																 
//          w_err+=sw;																			 
//          if (w_err >= dw) { sw_pos+=sw_inc; w_err-=dw; } 	 
//        } 																								 
//      else																								 
//        for (i=0; i <= sw; i++) 														 
//        { 																								 
//          *(char*)dw_pos=*(char*)sw_pos;
//          *(((char*)dw_pos)+1)=*(((char*)sw_pos)+1);
//          *(((char*)dw_pos)+2)=*(((char*)sw_pos)+2);
//
//          sw_pos+=sw_inc; 																 
//          w_err+=dw;																			 
//          if (w_err >= sw) 
//          { 
//            dw_pos += dw_inc; 
//            w_err -= sw; 
//          } 	 
//        } 																								 
//      
//      sh_pos+=sh_inc; 																		 
//      h_err+=dh;																					 
//      if (h_err >= sh) { dh_pos+=dh_inc; h_err-=sh; } 			 
//    }
//}
//  
//
//void nglImage::ScaleLine(char* Target, int TgtWidth, char* Source, int SrcWidth)
//{
//  int NumPixels = TgtWidth;
//  int IntPart = SrcWidth / TgtWidth;
//  int FractPart = SrcWidth % TgtWidth;
//  int E = 0;
//  while (NumPixels-- > 0) {
//    *Target++ = *Source;
//    Source += IntPart;
//    
//    E += FractPart;
//    if (E >= TgtWidth) {
//      E -= TgtWidth;
//      Source++;
//    } /* if */
//  } /* while */
//}
//
//
//void nglImage::ScaleLineAvg(char* Target, int TgtWidth, char* Source, int SrcWidth)
//{
//  int NumPixels = TgtWidth;
//  int Mid = TgtWidth / 2;
//  int E = 0;
//  char p;
//  if (TgtWidth > SrcWidth)
//    NumPixels--;
//  while (NumPixels-- > 0) 
//  {
//    p = *Source;
//    if (E >= Mid)
//      p = average(p, *(Source+1));
//    *Target++ = p;
//    E += SrcWidth;
//    if (E >= TgtWidth) 
//    {
//      E -= TgtWidth;
//      Source++;
//    }
//  }
//  
//  if (TgtWidth > SrcWidth)
//    *Target = *Source;
//}
//
//void nglImage::ScaleRect(char* Target, int TgtWidth, int TgtHeight, char* Source, int SrcWidth, int SrcHeight)
//{
//  int NumPixels = TgtHeight;
//  int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
//  int FractPart = SrcHeight % TgtHeight;
//  int E = 0;
//  char* PrevSource = NULL;
//  while (NumPixels-- > 0) 
//  {
//    if (Source == PrevSource) 
//      memcpy(Target, Target-TgtWidth, TgtWidth*sizeof(*Target));
//    else 
//    {
//      ScaleLineAvg(Target, TgtWidth, Source, SrcWidth);
//      PrevSource = Source;
//    } 
//    
//    Target += TgtWidth;
//    Source += IntPart;
//    E += FractPart;
//    if (E >= TgtHeight) 
//    {
//      E -= TgtHeight;
//      Source += SrcWidth;
//    } 
//  }
//}  
//  

#define average(a, b)   (char)(( (int)(a) + (int)(b) ) >> 1)


void ScaleLineAvg(uint32* pTarget, int32 TgtWidth, const uint32* pSource, int32 SrcWidth)
{
  int32 NumPixels = TgtWidth;
  int32 IntPart   = SrcWidth / TgtWidth;
  int32 FractPart = SrcWidth % TgtWidth;
  int32 Mid       = TgtWidth / 2;
  int32 E         = 0;
  int32 skip;
  uint32 p;
  
  skip = (TgtWidth < SrcWidth) ? 0 : TgtWidth / (2*SrcWidth) + 1;
  NumPixels -= skip;
  
  while (NumPixels-- > 0) 
  {
    p = *pSource;
    
    if (E >= Mid)
      p = average(p, *(pSource + 1));
    
    *pTarget++ = p;
    pSource += IntPart;
    E += FractPart;
    
    if (E >= TgtWidth) 
    {
      E -= TgtWidth;
      pSource++;
    } 
  } 
  
  while (skip-- > 0)
  {
    *pTarget++ = *pSource;
  }
}



void ScaleRectAvg(uint32* pTarget, int32 TgtWidth, int32 TgtHeight,
  const uint32* pSource, int32 SrcWidth, int32 SrcHeight)
{
  const uint32* pOriginalSource = pSource;
  uint32* pOriginalTarget = pTarget;
  
  int32 NumPixels = TgtHeight;
  int32 IntPart   = (SrcHeight * SrcWidth) / TgtHeight;
  int32 FractPart = SrcHeight % TgtHeight;
  int32 Mid       = TgtHeight / 2;
  int32 E         = 0;
  int32 skip;
  uint32 *pScanLine;
  uint32 *pScanLineAhead;
  const uint32 *pPrevSource = NULL;
  const uint32 *pPrevSourceAhead = NULL;
  
  skip = (TgtHeight < SrcHeight) ? 0 : TgtHeight / (2*SrcHeight) + 1;
  NumPixels -= skip;
  
  pScanLine      = new uint32[TgtWidth];
  pScanLineAhead = new uint32[TgtWidth];
  
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
        uint32* tmp = pScanLine;
        pScanLine      = pScanLineAhead;
        pScanLineAhead = tmp;
      }
      else 
      {
        ScaleLineAvg(pScanLine, TgtWidth, pSource, SrcWidth);
      }
      
      pPrevSource = pSource;
    }
    
    if (E >= Mid && pPrevSourceAhead != pSource + SrcWidth) 
    {
      int32 x;
      ScaleLineAvg(pScanLineAhead, TgtWidth, pSource + SrcWidth, SrcWidth);
      
      for (x = 0; x < TgtWidth; x++)
        pScanLine[x] = average(pScanLine[x], pScanLineAhead[x]);
      
      pPrevSourceAhead = pSource + SrcWidth;
    } 
    
    memcpy(pTarget, pScanLine, TgtWidth * sizeof(uint32));
    pTarget += TgtWidth;
    pSource += IntPart;
    E       += FractPart;
    
    if (E >= TgtHeight) 
    {
      E -= TgtHeight;
      pSource += SrcWidth;
    } 
  } 
  
  if (skip > 0 && pSource != pPrevSource)
    ScaleLineAvg(pScanLine, TgtWidth, pSource, SrcWidth);
  
  while (skip-- > 0) 
  {
    memcpy(pTarget, pScanLine, TgtWidth * sizeof(uint32));
    pTarget += TgtWidth;
  }
  
  delete pScanLine;
  delete pScanLineAhead;
}



//ICI
nglImage::nglImage(const nglImage& rImage, uint NewWidth, uint NewHeight)
{
  Init();
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
  
  ScaleRectAvg((uint32*)GetBuffer(), NewWidth, NewHeight,
               (uint32*)rImage.GetBuffer(), sourceInfo.mWidth, sourceInfo.mHeight);  
  
//  // copy and scale image buffer
//  uint32 dx1 = 0;
//  uint32 dy1 = 0;
//  uint32 dx2 = scaledWidth - 1;
//  uint32 dy2 = scaledHeight - 1;
//  uint32 sx1 = 0;
//  uint32 sy1 = 0;
//  uint32 sx2 = sourceInfo.mWidth - 1;
//  uint32 sy2 = sourceInfo.mHeight - 1;
//    
//  uint32 sw = sx2-sx1;
//  uint32 sh = sy2-sy1;
//  uint32 dw = dx2-dx1;
//  uint32 dh = dy2-dy1;
//  
//  /* prepare bresenham vars */
//  uint32 h_err = 0;
//  char* sh_pos = rImage.GetBuffer();
//  sh_pos += (sy1 * sourceInfo.mBytesPerLine)  + (sx1 * sourceInfo.mBytesPerPixel);
//  char* dh_pos = GetBuffer();
//  dh_pos += (dy1 * mInfo.mBytesPerLine)       + (dx1 * mInfo.mBytesPerPixel);
//
//  char *sw_pos, *dw_pos;
//  uint32 sh_inc, dh_inc, sw_inc, dw_inc;
//  uint32 i, j, w_err;
//
//  
//  if (sh < 0) 
//  { 
//    sh_inc = - sourceInfo.mBytesPerLine; 
//    sh = -sh; 
//  } 
//  else 
//    sh_inc = sourceInfo.mBytesPerLine;
//  
//  if (dh < 0) 
//  { 
//    dh_inc = -mInfo.mBytesPerLine; 
//    dh = -dh; 
//  } 
//  else 
//    dh_inc = mInfo.mBytesPerLine;
//  
//  
//  if (sw < 0) 
//  { 
//    sw_inc = -sourceInfo.mBytesPerPixel; 
//    sw = -sw; 
//  } 
//  else 
//    sw_inc = sourceInfo.mBytesPerPixel;
//  
//  if (dw < 0) 
//  { 
//    dw_inc = -mInfo.mBytesPerPixel; 
//    dw = -dw; 
//  } 
//  else 
//    dw_inc = mInfo.mBytesPerPixel;
//  
//  
//  
//  switch (mInfo.mBytesPerPixel)
//  {
//    case 2: BRESHENAM2D(*(short*)dw_pos=*(short*)sw_pos; ) break;
//
//    case 3: 
//      //Breshenam24(dh_pos, dw, dh, dw_inc, dh_inc, sh_pos, sw, sh, sw_inc, sh_inc);
//      ScaleRect(dh_pos, mInfo.mWidth * 3, mInfo.mHeight, sh_pos, sourceInfo.mWidth *3, sourceInfo.mHeight);
//      break;
//
//    case 4: BRESHENAM2D( *(uint32*)dw_pos=*(uint32*)sw_pos; ) break;
//  }
}
  
  








nglImage::~nglImage()
{
  // Image buffer is released by mpInfo destruction
  if (mpCodec && mOwnCodec) 
    delete mpCodec;
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

uint nglImage::GetWidth() const
{
  return mInfo.mWidth;
}

uint nglImage::GetHeight() const
{
  return mInfo.mHeight;
}

uint nglImage::GetBitDepth() const
{
  return mInfo.mBitDepth;
}

uint nglImage::GetPixelSize() const
{
  return mInfo.mBytesPerPixel;
}

uint nglImage::GetBytesPerLine() const
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
 * User callbacks
 */

void nglImage::OnInfo (nglImageInfo& rInfo)
{
//  rInfo.Copy (mInfo, false);
}

void nglImage::OnData(float Completion)
{
  mCompletion = Completion;
}

bool nglImage::OnError()
{
  return false; // Abort on error as a default (user overridable)
}


/*
 * Global codec registry
 */

void nglImage::Init()
{
  if (mpCodecInfos == NULL)
  {
    mpCodecInfos = new std::vector<nglImageCodecInfo*>();
    App->AddExit(Exit);
  #ifndef _UIKIT_
    mpCodecInfos->push_back(new nglImageTGACodecInfo());
    mpCodecInfos->push_back(new nglImagePPMCodecInfo());
  #endif

  #ifdef HAVE_COREGRAPHICS
    mpCodecInfos->push_back(new nglImageCGCodecInfo());
  #endif
  #ifdef HAVE_LIBPNG
    mpCodecInfos->push_back(new nglImagePNGCodecInfo());
  #endif
  #ifdef HAVE_LIBJPEG
    mpCodecInfos->push_back(new nglImageJPEGCodecInfo());
  #endif
  }
}

void nglImage::Exit()
{
  vector<nglImageCodecInfo*>::iterator i;

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
  Init();
  if (pCodecInfo)
  {
    mpCodecInfos->push_back (pCodecInfo);
    return true;
  }
  return false;
}

bool nglImage::DelCodec (nglImageCodecInfo* pCodecInfo)
{
  Init();
  vector<nglImageCodecInfo*>::iterator i;

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

nglImageCodec* nglImage::CreateCodec (int Index)
{
  Init();
  nglImageCodecInfo* info = (*mpCodecInfos)[Index];
  return (info) ? info->CreateInstance() : NULL;
}

nglImageCodec* nglImage::CreateCodec (const nglString& rName)
{
  Init();
  vector<nglImageCodecInfo*>::iterator i;

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

void nglImage::OnCodecInfo(nglImageInfo& rInfo)
{
  mInfo.Copy (rInfo, false);
  mInfo.AllocateBuffer();
  OnInfo (mInfo);
}

void nglImage::OnCodecData(float Completion)
{
  OnData (Completion);
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

