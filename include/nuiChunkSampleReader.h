/*
 *  nuiChunkSampleReader.h
 *  nui3
 *
 *  Created by mat on 1/19/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
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
  
  virtual uint32 ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
  
protected:
  bool GoToChunk(char* pId);
  bool ScanAllChunks();
  Chunk* GetChunk(char* Id);
  void ClearChunks();
  
  std::vector<Chunk*> mChunks;
};




