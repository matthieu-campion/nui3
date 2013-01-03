/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiSampleWriter.h"

///Class that writes Wave file
class nuiWaveWriter : public virtual nuiSampleWriter
{
public:
  nuiWaveWriter(nglOStream& rStream);
  virtual ~nuiWaveWriter();
  
  bool WriteInfo(const nuiSampleInfo& rInfos); ///< Method that writes infos of Wave datas in sound file. Must be called before Write multiple calls and Finalize call \param rInfos SampleInfo object that gives infos to write \return True if method succeeded
  int32 Write(const void* pBuffer, int32 SampleFrames, nuiSampleBitFormat format); ///< Method that Writes Wave samples in sound file. WriteInfo must have been called before. Can be called several times successively \param pBuffer Pointer to samples to write \param SampleFrames Number of sample frames to write \param format format of datas in pBuffer (eInt16, eSampleFloat32) \return True if method succeeded
  bool Finalize();  ///< Method that writes infos not yet known before the last Write call end. Close the Wave file. Must be called after WriteInfo call and Write multiple calls. \return True if method succeeded
  
private:
    bool mIsInfosWritten;
};

