/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
  \file  nglImageCodec.cpp
  \brief Base class for nglImage pluggable loader/exporter
*/

#include "nui.h"

nglImageCodecInfo::nglImageCodecInfo()
{
}

nglImageCodecInfo::~nglImageCodecInfo()
{
}

bool nglImageCodecInfo::ExtensionMatch(nglString& rFileName)
{
  std::vector<nglString>::iterator ext;
  int filename_len = rFileName.GetLength();

  for (ext = mExtensions.begin(); ext < mExtensions.end(); ++ext)
  {
    int ext_size = (*ext).GetLength();

    if (!rFileName.Compare (*ext, filename_len - ext_size, ext_size, false))
      return true;
  }
  return false;
}

///////
nglImageCodec::~nglImageCodec()
{
}

bool nglImageCodec::Init(nglImage* pImage)      ///< Codec init is decoupled from construction
{
  mpImage = pImage;
  return true;
}

bool nglImageCodec::SendInfo (nglImageInfo& rInfo)  ///< Send image description to image object. The image object will allocate the image buffer.
{
  if (mpImage)
    return mpImage->OnCodecInfo (rInfo);
}

bool nglImageCodec::SendData (float Completion)  ///< Acknowledge that more data was decoded to image buffer
{
  if (mpImage)
    return mpImage->OnCodecData (Completion);
}

bool nglImageCodec::SendError()  ///< Signals an encoding/decoding error
{
  return (mpImage) ? mpImage->OnCodecError() : false;
}
