/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
#pragma once

#include "nui.h"
#include "nuiSampleInfo.h"

/// Generic Class that writes in a sound file
class nuiSampleWriter
{
public:
  nuiSampleWriter(nglOStream& rStream);
  virtual ~nuiSampleWriter();

  virtual bool WriteInfo(const nuiSampleInfo& rInfos) = 0; ///< Method that writes infos of sound datas in sound file. Must be called before Write multiple calls and Finalize call \param rInfos SampleInfo object that gives infos to write \return True if method succeeded
  virtual int32 Write(const void* pBuffer, int32 SampleFrames, nuiSampleBitFormat format) = 0; ///< Method that Writes sound samples in sound file. WriteInfo must have been called before. Can be called several times successively \param pBuffer Pointer to samples to write \param SampleFrames Number of sample frames to write \param format format of datas in pBuffer (eInt16, eSampleFloat32) \return True if method succeeded
  virtual bool Finalize() = 0;  ///< Method that writes infos not yet known before the last Write call end. Close the file. Must be called after WriteInfo call and Write multiple calls. \return True if method succeeded
  
protected:
  nglOStream& mrStream;
  nuiSampleInfo mrSampleInfo;
};


