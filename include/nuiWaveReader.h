/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
 #include "nui.h"
 #include "nuiSampleReader.h"
 



#pragma once




/// Class that reads an Wave Sound File
class nuiWaveReader : public nuiSampleReader
{
public:
  nuiWaveReader(nglIStream& rStream); ///< nuiWaveReader Constructor \param rStream The nglIStream in which we read
  nuiWaveReader(const nuiSampleReader& rReader, nglIStream& rStream);
  virtual ~nuiWaveReader();
  virtual nuiSampleReader* Clone(nglIStream& rStream) const;
  virtual bool ReadInfo(nuiSampleInfo& rInfos);  ///< Method that reads Infos of the Wave Sound \param rInfos Variable that receives Sample infos read in stream \return True if method succeeded
  virtual uint32 Read(void* pBuffer, uint32 SampleFrames, nuiSampleBitFormat format);  ///< Method that reads Wave samples from stream \param pBuffer Pointer to a buffer that retrieves samples \param SampleFrames Number of sample frames to read \param SampleBitFormat Wanted format of samples in pBuffer \return True if method succeeded
    
private:
};

