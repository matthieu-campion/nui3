/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglBitmapTools.h"


NGL_API void nglInvertLineSwap32 (char* pDst, char* pSrc, uint32 pixelcount)
{
  uint32 count = pixelcount * 4;
  pSrc += count - 1;
  while (--count)
    *pDst++ = *pSrc--;
}

NGL_API void nglInvertLineSwap24 (char* pDst, char* pSrc, uint32 pixelcount)
{
  uint32 count = pixelcount * 3;
  pSrc += count - 1;
  while (--count)
    *pDst++ = *pSrc--;
}

NGL_API void nglLineSwap24 (char* pDst, char* pSrc, uint32 pixelcount)
{
  char p1,p2,p3;
  uint32 count = pixelcount;
  while (--count)
  {
    p3 = *pSrc++;
    p2 = *pSrc++;
    p1 = *pSrc++;
    *pDst++ = p1;
    *pDst++ = p2;
    *pDst++ = p3;
  }
}

NGL_API void nglInvertLine32 (unsigned int* pDst, unsigned int* pSrc, uint32 pixelcount)
{
  uint32 count = pixelcount;
  pSrc += count - 1;
  while (--count)
  {
    *pDst++ = *pSrc--;
  }
}

NGL_API void nglInvertLine24 (char* pDst, char* pSrc, uint32 pixelcount)
{
  char p1,p2,p3;
  uint32 count = pixelcount;
  pSrc += count - 1;
  while (--count)
  {
    p3 = *pSrc--;
    p2 = *pSrc--;
    p1 = *pSrc--;
    *pDst++ = p1;
    *pDst++ = p2;
    *pDst++ = p3;
  }
}

NGL_API void nglInvertLine16 (short* pDst, short* pSrc, uint32 pixelcount)
{
  pSrc+=pixelcount - 1;
  while (--pixelcount)
    *pDst++ = *pSrc--;
}


/// From 32 BPP
NGL_API void nglCopyLine8To8 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst + (PixelCount - 1);
    for (i=0; i<PixelCount; i++)
      *pDest-- = *pSource++;
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*4);
  }
}                                          
                                           
NGL_API void nglCopyLine8To15 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource) & 0xF8) >> 3;
      temp |= ((*pSource) & 0xF8) << 2;
      temp |= ((*pSource) & 0xF8) << 7;
      pSource++;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource) & 0xF8) >> 3;
      temp |= ((*pSource) & 0xF8) << 2;
      temp |= ((*pSource) & 0xF8) << 7;
      pSource++;
      ((short*)pDst)[i] = temp;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine8To16 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource)&0xF8) >> 3;
      temp |= ((*pSource)&0xF8) << 3;
      temp |= ((*pSource)&0xF8) << 8;
      pSource++;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource) & 0xF8) >> 3;
      temp |= ((*pSource) & 0xF8) << 3;
      temp |= ((*pSource) & 0xF8) << 8;
      pSource++;
      ((short*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine8To24 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char r,g,b;
    char* pDest   = (char*) pDst + 3 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource;
      g = *pSource;
      b = *pSource;
      pSource++;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    char* pDest   = (char*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      *pDest++ = *pSource;
      *pDest++ = *pSource;
      *pDest++ = *pSource;
      pSource++;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine8To32 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    unsigned char r,g,b,a;
    unsigned char* pSource = (unsigned char*) pSrc;
    unsigned char* pDest   = (unsigned char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource;
      g = *pSource;
      b = *pSource;
      a = 255;
      *pDest-- = a;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*4);
  }
}    

NGL_API void nglCopyLineL8To32ARGB (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    uint8 src_val;
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      src_val = *pSource++;
      *pDest-- = 255;
      *pDest-- = src_val;
      *pDest-- = src_val;
      *pDest-- = src_val;
    }
  }
  else
  {
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst;
    for (int32 i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = pSource[i];
      pDest[i*4+1] = pSource[i];
      pDest[i*4+2] = pSource[i];
      pDest[i*4+3] = 255;
    }
  }
}  

NGL_API void nglCopyLineA8To32ARGB (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    uint8 src_val;
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      src_val = *pSource++;
      *pDest-- = src_val;
      *pDest-- = 0;
      *pDest-- = 0;
      *pDest-- = 0;
    }
  }
  else
  {
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst;
    for (int32 i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = 0;
      pDest[i*4+1] = 0;
      pDest[i*4+2] = 0;
      pDest[i*4+3] = pSource[i];
    }
  }
}  



/// From 15 BPP
NGL_API void nglCopyLine15To8 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  unsigned short temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (((temp >> 7) & 0xF8) + ((temp >> 2) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (((temp >> 7) & 0xF8) + ((temp >> 2) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
}

NGL_API void nglCopyLine15To15 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  if (Invert)
  {
    int32 i;
    for (i=0; i<PixelCount; i++)
      ((int16*)pDst)[i] = ((int16*)pSrc)[PixelCount-i-1];
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*2);
  }
}

NGL_API void nglCopyLine15To16 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  uint8 r,g,b;
  int16 temp;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      r = (temp >> 10) & 0x1F;
      g = (temp >> 5 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 11) + (g <<6) + b;
      ((int16*)pDst)[PixelCount - i -1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      r = (temp >> 10) & 0x1F;
      g = (temp >> 5 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 11) + (g <<6) + b;
      ((short*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine15To24 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
}

NGL_API void nglCopyLine15To32 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
}


NGL_API void nglCopyLine15To32ARGB (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = 0; 
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = 0;
    }
  }
}

/// From 16 BPP
NGL_API void nglCopyLine16To8 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  uint16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (((temp >> 8) & 0xF8) + ((temp >> 3) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (((temp >> 8) & 0xF8) + ((temp >> 3) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
}


NGL_API void nglCopyLine16To15 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  uint8 r,g,b;
  int16 temp;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      r = (temp >> 11) & 0x1F;
      g = (temp >> 6 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 10) + (g <<5) + b;
      ((int16*)pDst)[PixelCount - i -1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      r = (temp >> 11) & 0x1F;
      g = (temp >> 6 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 10) + (g <<5) + b;
      ((int16*)pDst)[i] = temp;
    }
  }
}
              
NGL_API void nglCopyLine16To16 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  if (Invert)
  {
    int32 i;
    for (i=0; i<PixelCount; i++)
      ((int16*)pDst)[i] = ((int16*)pSrc)[PixelCount-i-1];
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*2);
  }
}

NGL_API void nglCopyLine16To24 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
}

NGL_API void nglCopyLine16To32 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i = 0; i < PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = 0;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = 0;
    }
  }
}


NGL_API void nglCopyLine16To32ARGB (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pDest = (uint8*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((int16*)pSrc)[i];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
}


/// From 24 BPP
NGL_API void nglCopyLine24To8 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  char r,g,b;
  int32 i;
  uint8* pSource = (uint8*) pSrc;
  if (Invert)
  {
    uint8* pDest   = (uint8*) pDst + (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest-- = (b + g + r)/3;
    }
  }
  else
  {
    uint8* pDest   = (uint8*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest++ = (b + g + r)/3;
    }
  }
}                                          

NGL_API void nglCopyLine24To15 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  int16 temp;
  uint8* pSource = (uint8*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      ((int16*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      ((int16*)pDst)[i] = temp;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine24To16 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pSource = (uint8*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++)&0xF8) << 8;
      temp |= ((*pSource++)&0xF8) << 3;
      temp |= ((*pSource++)&0xF8) >> 3;
      ((int16*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++)&0xF8) << 8;
      temp |= ((*pSource++)&0xF8) << 3;
      temp |= ((*pSource++)&0xF8) >> 3;
      ((int16*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine24To24 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    uint8 r,g,b;
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst + 3 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*3);
  }
}                                          
                                           
NGL_API void nglCopyLine24To32 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  uint8* pSource = (uint8*) pSrc;
  if (Invert)
  {
    uint8 r,g,b;
    uint8* pDest   = (uint8*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      pDest--;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    uint8* pDest   = (uint8*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      pDest++;
    }
  }
}      

NGL_API void nglCopyLine24To32ARGB (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  uint8* pSource = (uint8*) pSrc;
  if (Invert)
  {
    uint8 r,g,b;
    uint8* pDest   = (uint8*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      pDest--;
      *pDest-- = r;
      *pDest-- = g;
      *pDest-- = b;
    }
  }
  else
  {
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst;
    //memcpy(pDst,pSrc,PixelCount*4);
    for (int32 i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = pSource[i*3+2];
      pDest[i*4+1] = pSource[i*3+1];
      pDest[i*4+2] = pSource[i*3+0];
      pDest[i*4+3] = 255;
    }
  }
}   


/// From 32 BPP
NGL_API void nglCopyLine32To8 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  uint8* pSource = (uint8*) pSrc;
  if (Invert)
  {
    uint8 r,g,b;
    uint8* pDest   = (uint8*) pDst + (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      pSource++;
      *pDest-- = (b + g + r )/3;
    }
  }
  else
  {
    uint8* pDest   = (uint8*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      int32 average;
      average  = *pSource++;
      average += *pSource++;
      average += *pSource++;
      pSource++;
      *pDest++ = average / 3; 
    }
  }
}                                          
                                           

NGL_API void nglCopyLine32To15 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  int16 temp;
  uint8* pSource = (uint8*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      pSource++;
      ((int16*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      pSource++;
      ((int16*)pDst)[i] = temp;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine32To16 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{
  int32 i;
  int16 temp;
  uint8* pSource = (uint8*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++)&0xF8) >> 3;
      temp |= ((*pSource++)&0xF8) << 3;
      temp |= ((*pSource++)&0xF8) << 8;
      pSource++;
      ((int16*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 3;
      temp |= ((*pSource++) & 0xF8) << 8;
      pSource++;
      ((int16*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine32To24 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  int32 i;
  uint8* pSource = (uint8*) pSrc;
  if (Invert)
  {
    uint8 r,g,b;
    uint8* pDest   = (uint8*) pDst + 3 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      pSource++;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    uint8* pDest   = (uint8*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      pSource++;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine32To32 (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    uint8 r,g,b,a;
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      a = *pSource++;
      *pDest-- = a;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*4);
  }
}     

NGL_API void nglCopyLine32To32ARGB (void* pDst, void* pSrc, int32 PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int32 i;
    uint8 r,g,b,a;
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      a = *pSource++;
      *pDest-- = a;
      *pDest-- = r;
      *pDest-- = g;
      *pDest-- = b;
    }
  }
  else
  {
    uint8* pSource = (uint8*) pSrc;
    uint8* pDest   = (uint8*) pDst;
    //memcpy(pDst,pSrc,PixelCount*4);
    for (int32 i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = pSource[i*4+2];
      pDest[i*4+1] = pSource[i*4+1];
      pDest[i*4+2] = pSource[i*4+0];
      pDest[i*4+3] = pSource[i*4+3];
    }
  }
} 

/// Stubber:
NGL_API nglCopyLineFn nglGetCopyLineFn (int32 DstBPP, int32 SrcBPP)
{
  switch (SrcBPP)
  {
  case 8:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine8To8;
      break;
    case 15:
      return &nglCopyLine8To15;
      break;
    case 16:
      return &nglCopyLine8To16;
      break;
    case 24:
      return &nglCopyLine8To24;
      break;
    case 32:
      return &nglCopyLine8To32;
      break;
    }
    break;
  case 15:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine15To8;
      break;
    case 15:
      return &nglCopyLine15To15;
      break;
    case 16:
      return &nglCopyLine15To16;
      break;
    case 24:
      return &nglCopyLine15To24;
      break;
    case 32:
      return &nglCopyLine15To32;
      break;
    }
    break;
  case 16:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine16To8;
      break;
    case 15:
      return &nglCopyLine16To15;
      break;
    case 16:
      return &nglCopyLine16To16;
      break;
    case 24:
      return &nglCopyLine16To24;
      break;
    case 32:
      return &nglCopyLine16To32;
      break;
    }
    break;
  case 24:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine24To8;
      break;
    case 15:
      return &nglCopyLine24To15;
      break;
    case 16:
      return &nglCopyLine24To16;
      break;
    case 24:
      return &nglCopyLine24To24;
      break;
    case 32:
      return &nglCopyLine24To32;
      break;
    }
    break;
  case 32:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine32To8;
      break;
    case 15:
      return &nglCopyLine32To15;
      break;
    case 16:
      return &nglCopyLine32To16;
      break;
    case 24:
      return &nglCopyLine32To24;
      break;
    case 32:
      return &nglCopyLine32To32;
      break;
    }
    break;
  }
  return NULL; // There is no line copy function for the given pixel types...
}

NGL_API void nglCopyImage(void* pDst, int32 dstwidth, int32 dstheight, int32 dstbpp, void* pSrc, int32 srcwidth, int32 srcheight, int32 srcbpp, bool vmirror, bool hmirror)
{
  int32 i;
  int32 sizex = 0, sizey = 0;
  int32 slinesize = srcwidth * ((srcbpp+1)/8);
  int32 dlinesize = dstwidth * ((dstbpp+1)/8);

  nglCopyLineFn pCpFn = nglGetCopyLineFn(dstbpp,srcbpp);

  if (!pCpFn)
    return;

  if ( dstwidth > srcwidth )
  {
    sizex = srcwidth;
  }
  else if ( dstwidth < srcwidth )
  {
    sizex = dstwidth;
  }
  else
    sizex = dstwidth;

  if ( dstheight > srcheight )
    sizey = srcheight;
  else
    sizey = dstheight;

  uint8* pSource = (uint8*) pSrc;
  uint8* pDest = (uint8*) pDst;
  if (vmirror)
  {
    pSource += slinesize * (sizey-1);
    for (i = 0; i< sizey; i++)
    {
      pCpFn(pDest,pSource, sizex, hmirror);
      pSource -= slinesize;
      pDest += dlinesize;
    }
  }
  else
  {
    for (i = 0; i< sizey; i++)
    {
      pCpFn(pDest,pSource, sizex, hmirror);
      pSource += slinesize;
      pDest += dlinesize;
    }
  }
}




NGL_API void nglPreMultLine16LumA(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 2;
    const uint8 alpha = pS[o + 1];
    pD[o + 0] = (pS[o + 0] * alpha) / 255;
  }
}

NGL_API void nglPreMultLine16ALum(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 2;
    const uint8 alpha = pS[o + 0];
    pD[o + 1] = (pS[o + 1] * alpha) / 255;
  }
}


NGL_API void nglPreMultLine32RGBA(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 4;
    const uint8 alpha = pS[o + 3];
    pD[o + 0] = (pS[o + 0] * alpha) / 255;
    pD[o + 1] = (pS[o + 1] * alpha) / 255;
    pD[o + 2] = (pS[o + 2] * alpha) / 255;
  }
}

NGL_API void nglPreMultLine32ARGB(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 4;
    const uint8 alpha = pS[o + 0];
    pD[o + 1] = (pS[o + 1] * alpha) / 255;
    pD[o + 2] = (pS[o + 2] * alpha) / 255;
    pD[o + 3] = (pS[o + 3] * alpha) / 255;
  }
}


/// Unpremultiply Alpha:
NGL_API void nglUnPreMultLine16LumA(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 2;
    const uint8 alpha = pS[o + 1];
    if (alpha)
    {
      pD[o + 0] = (pS[o + 0] * 255) / alpha;
    }
  }
}

NGL_API void nglUnPreMultLine16ALum(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 2;
    const uint8 alpha = pS[o + 0];
    if (alpha)
    {
      pD[o + 1] = (pS[o + 1] * 255) / alpha;
    }
  }
}

NGL_API void nglUnPreMultLine32RGBA(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 4;
    const uint8 alpha = pS[o + 3];
    if (alpha)
    {
      pD[o + 0] = (pS[o + 0] * 255) / alpha;
      pD[o + 1] = (pS[o + 1] * 255) / alpha;
      pD[o + 2] = (pS[o + 2] * 255) / alpha;
    }
  }
}

NGL_API void nglUnPreMultLine32ARGB(void* pDst, void* pSrc, int32 PixelCount)
{
  uint8* pD = (uint8*)pDst;
  uint8* pS = (uint8*)pSrc;
  for (int32 i = 0; i < PixelCount; i++)
  {
    const uint32 o = i * 4;
    const uint8 alpha = pS[o + 0];
    if (alpha)
    {
      pD[o + 1] = (pS[o + 1] * 255) / alpha;
      pD[o + 2] = (pS[o + 2] * 255) / alpha;
      pD[o + 3] = (pS[o + 3] * 255) / alpha;
    }
  }
}


