/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSampleReader.h"


class nuiChunkSampleReader : public nuiSampleReader
{
public:
  
  class Chunk 
  {
  public:
    Chunk();
    Chunk(nglFileOffset position, nglFileOffset DataSize, nglFileOffset dataPositon, const char* Id);
    virtual ~Chunk();
    
    nglFileOffset mPosition;
    nglFileOffset mDataSize;
    nglFileOffset mDataPosition;
    char* mId;
    
    bool CompareId(const char* Id) const;
  };
  
  nuiChunkSampleReader(nglIStream& rStream);
  nuiChunkSampleReader(const nuiChunkSampleReader& rReader, nglIStream& rStream);
  virtual ~nuiChunkSampleReader();
  
  virtual int32 ReadDE(std::vector<void*> buffers, int32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  
protected:
  bool GoToChunk(char* pId);
  bool ScanAllChunks();
  Chunk* GetChunk(char* Id);
  void ClearChunks();
  
  std::vector<Chunk*> mChunks;
};




