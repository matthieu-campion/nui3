/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglBitmapTools.h
\brief helper for scanline color format conversions
*/

#ifndef __nglBitmapTools_h__
#define __nglBitmapTools_h__

//#include "nui.h"


typedef void (*nglCopyLineFn)(void* pDst, void* pSrc, int PixelCount, bool Invert);

NGL_API nglCopyLineFn nglGetCopyLineFn (int DstBPP, int SrcBPP); ///< Retreive a pointer to a function that can copy any BPP to any BPP.
NGL_API void nglCopyImage(void* pDst, int dstwidth, int dstheight, int dstbpp, void* pSrc, int srcwidth, int srcheight, int srcbpp, bool vmirror, bool hmirror);

NGL_API void nglInvertLineSwap32 (char* pDst, char* pSrc, unsigned int pixelcount);
NGL_API void nglInvertLineSwap24 (char* pDst, char* pSrc, unsigned int pixelcount);
NGL_API void nglLineSwap24 (char* pDst, char* pSrc, unsigned int pixelcount);
NGL_API void nglInvertLine32 (unsigned int* pDst, unsigned int* pSrc, unsigned int pixelcount);
NGL_API void nglInvertLine24 (char* pDst, char* pSrc, unsigned int pixelcount);
NGL_API void nglInvertLine16 (short* pDst, short* pSrc, unsigned int pixelcount);

//D3D ARGB functions
NGL_API void nglCopyLine15To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert);
NGL_API void nglCopyLine16To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert);
NGL_API void nglCopyLine32To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert);


#endif // __nglBitmapTools_h__

