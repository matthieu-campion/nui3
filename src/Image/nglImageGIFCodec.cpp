/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglImageGIFCodec.h"

#include "gif_lib.h"

nglImageGIFCodec::nglImageGIFCodec(void)
{
  mpImage = NULL;
}

nglImageGIFCodec::~nglImageGIFCodec(void)
{
  mpImage = NULL;
}

bool nglImageGIFCodec::Init(nglImage* pImage)
{
  mpImage = pImage;
  return true;
}

bool nglImageGIFCodec::Probe(nglIStream* pIStream)
{
  if (pIStream->Available(4)) // A not so robust png detection routine...
  {
    char buffer[6];
    if (!pIStream->Peek(buffer, 6, 1))
      return false;

    if (buffer[0]=='G' && 
        buffer[1]=='I' && 
        buffer[2]=='F' &&
        buffer[3]=='8' &&
        (buffer[4]=='7' || buffer[4]=='9') &&
        buffer[5]=='a'
      )
    {
      return true;
    }
  }
  return false;
}

static int getTransparentColor(GifFileType * gft, int32 framenum)
{
  int i;
  ExtensionBlock *ext = gft->SavedImages[framenum].ExtensionBlocks;
  
  // Get transparency color from graphic extension block
  for (i = 0; i < gft->SavedImages[framenum].ExtensionBlockCount; i++, ext++)
  {
    if ((ext->Function == GRAPHICS_EXT_FUNC_CODE) && (ext->Bytes[0] & 1)) 
    {
      // there is a transparent color
      return ext->Bytes[3] == 0 ? 0 :     // exception
                                  (uint8)ext->Bytes[3];             // transparency color
    }
  }
  
  return -1;
}

static int GIFInputFunc(GifFileType* pGIF, GifByteType* pBuffer, int count)
{
  nglIStream* pStream = (nglIStream*)(pGIF->UserData);
  NGL_ASSERT(pStream);
  return pStream->Read(pBuffer, count, 1);
}

bool nglImageGIFCodec::Feed(nglIStream* pIStream) 
{
  GifFileType* pGIF = DGifOpen(pIStream, &GIFInputFunc);
  if (!pGIF)
    return false;
  
  if (DGifSlurp(pGIF) != GIF_OK)
    return false;
  
  nglImageInfo info(pGIF->SWidth, pGIF->SHeight, 32);
  if (!SendInfo(info))
  {
    DGifCloseFile(pGIF);
    return false;
  }

  
  GifImageDesc* pImg = &pGIF->SavedImages[0].ImageDesc;
  
  // Local colormap has precedence over Global colormap
  ColorMapObject* pColormap = pImg->ColorMap ? pImg->ColorMap : pGIF->SColorMap;
  uint32 numcolors = pColormap->ColorCount;
  uint32 alphapalette = getTransparentColor(pGIF, 0);  
  uint32 bgcolor = -1;
  
  // bgcolor is the background color to fill the bitmap
  if (pGIF->SColorMap)         // There is a GlobalColorMap
    bgcolor = (uint8)pGIF->SBackGroundColor;   // The SBGColor is meaningful
  else if (alphapalette >= 0) // There is a transparency color
    bgcolor = alphapalette; // set the bgcolor to tranparent
  else
    bgcolor = 0;

  // Don't know what to do here.
  // If this doesn't work, we could
  // create a new color and set the
  // alpha-channel to transparent
  // (unless we are using all the 256
  // colors, in which case either we
  // give up, or move to 16-bits palette
  
  uint32 palette[256];
  
  for (uint32 i = 0; i < numcolors; i++)
  {
    uint8 alpha = 255;
    if (i == bgcolor || i == alphapalette)
      alpha = 0;
    nuiColor c(pColormap->Colors[i].Red, pColormap->Colors[i].Green, pColormap->Colors[i].Blue, alpha);
    palette[i] = c.GetRGBA();
  }
  
  uint32* pOutBuf = (uint32*)mpImage->GetBuffer();
  uint8* pInBuf = (uint8*)(pGIF->SavedImages[0].RasterBits);
  for (uint32 i = 0; i < pGIF->SWidth * pGIF->SHeight; i++)
    *pOutBuf++ = palette[*pInBuf++];

  SendData(1.0f);
  DGifCloseFile(pGIF);
  return true;
}

bool nglImageGIFCodec::Save(nglOStream* pOStream) 
{
  return false;
}

float nglImageGIFCodec::GetCompletion()
{
  return 1.0;
}


