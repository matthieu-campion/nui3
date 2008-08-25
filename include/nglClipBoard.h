/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglClipBoard.h
\brief Portable clipboard handling
*/

#ifndef __nglClipBoard_h__
#define __nglClipBoard_h__

class nglString;
#include "nglMimeSource.h"

class NGL_API nglClipBoard
{
  friend class nglKernel;

public:

  void Clear();

  void SetData(const nglMimeSource &src);
  void GetData(nglMimeSource &dst);

  bool IsAvailable(const nglMimeSource::nglMimeDataType &mimeType);

protected:

  nglClipBoard();
  ~nglClipBoard();

};


#endif // __nglClipBoard_h__
