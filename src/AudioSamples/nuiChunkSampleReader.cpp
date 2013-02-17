/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#define CHUNK_ID_BYTES 4
#define CHUNK_SIZE_BYTES 4
#define CHUNK_HEADER_BYTES (CHUNK_ID_BYTES + CHUNK_SIZE_BYTES)
#define FIRST_CHUNK_DATA_BYTES 4 //4 bytes to represent RIFF type (WAVE, AIFF...)
//
//
//nuiChunkSampleReader
//
//

nuiChunkSampleReader::nuiChunkSampleReader(nglIStream& rStream) : 
nuiSampleReader(rStream)
{
}

nuiChunkSampleReader::nuiChunkSampleReader(const nuiChunkSampleReader& rReader, nglIStream& rStream) :
nuiSampleReader(rReader, rStream)
{
  for (int32 i = 0; i < rReader.mChunks.size(); i++)
  {
    Chunk* pChunk = new Chunk(rReader.mChunks[i]->mPosition, rReader.mChunks[i]->mDataSize, rReader.mChunks[i]->mDataPosition, rReader.mChunks[i]->mId);
    mChunks.push_back(pChunk);
  }
}

nuiChunkSampleReader::~nuiChunkSampleReader()
{
  ClearChunks();
}


bool nuiChunkSampleReader::GoToChunk(char* pId)
{
  bool found = false;
  int32 index = 0;
  for (int32 i = 0; i < mChunks.size() && !found; i++)
  {
    found = mChunks[i]->CompareId(pId);
    if (found)
      index = i;
  }
  
  if (found)
  {
    //Set Stream position
    nglFileOffset pos = mChunks[index]->mDataPosition;
    mrStream.SetPos(pos);
  }
  
  return found;
}

nuiChunkSampleReader::Chunk* nuiChunkSampleReader::GetChunk(char* Id)
{
  bool found = false;
  int32 index = 0;
  for (int32 i = 0; i < mChunks.size() && !found; i++)
  {
    found = mChunks[i]->CompareId(Id);
    if (found)
      index = i;
  }
  
  if (found)
  {
    Chunk* pChunk = mChunks[index];
    return pChunk;
  }
  
  return NULL;
}

bool nuiChunkSampleReader::ScanAllChunks()
{  
  ClearChunks();
  
  /////////////
  //FIRST CHUNK
  mrStream.SetPos(0);
  
  //get Id
  //std::vector<char> Id(4);
  char* Id = new char[5];
  for(int32 k = 0; k < 4; k++)
  {
    if( 1 != mrStream.ReadUInt8((uint8*)&(Id[k]), 1))
    {
      delete[] Id;
      return false;
    }
  }
  Id[4] = '\0';
  //get first chunk size
  uint32 utilSize = 0; // = (total size of the stream) - (first chunk header size)
  if (1 != mrStream.ReadUInt32(&utilSize, 1))
  {
    delete[] Id;
    return false;
  }
  int32 firstChunkSize = FIRST_CHUNK_DATA_BYTES;

  Chunk* pFirstChunk = new Chunk(0, firstChunkSize, CHUNK_HEADER_BYTES, Id);
  mChunks.push_back(pFirstChunk);
  nglFileOffset TotalSize = CHUNK_HEADER_BYTES  + firstChunkSize + utilSize;
  
  do
  {
    ////////////
    //NEXT CHUNK
    nglFileOffset ChunkPosition = mChunks.back()->mDataPosition + mChunks.back()->mDataSize;
    mrStream.SetPos(ChunkPosition);
    uint32 ChunkSize = 0;
    for(int32 k = 0; k < 4; k++)
    {
      if( 1 != mrStream.ReadUInt8((uint8*)&(Id[k]), 1))
      {
        delete[] Id;
        return false;
      }
    }
    //get chunk size
    if (1 != mrStream.ReadUInt32(&ChunkSize, 1))
    {
      delete[] Id;
      return false;
    }
    if (0 != ChunkSize % 2)
          ChunkSize += 1; // the chunk size is always even
    Chunk* pNextChunk = new Chunk(ChunkPosition, ChunkSize, ChunkPosition + CHUNK_HEADER_BYTES, Id);
    mChunks.push_back(pNextChunk);
  }
  while (mChunks.back()->mDataPosition + mChunks.back()->mDataSize < TotalSize);

  delete[] Id;
  return true;
}

void nuiChunkSampleReader::ClearChunks()
{
  for (int32 i = 0; i < mChunks.size(); i++)
  {
    delete mChunks[i];
    mChunks[i] = NULL;
  }
  mChunks.clear();
}


int32 nuiChunkSampleReader::ReadDE(std::vector<void*> buffers, int32 sampleframes, nuiSampleBitFormat format)
{
  //don't increment mPosition: it's already done in ReadIN
  const int32 channels = mInfo.GetChannels();
  int32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  sampleframes = MIN(sampleframes, length - mPosition);
  
  if (buffers.size() != channels)
  {
    return 0;
  }
  
  bool deleteBuffer = false;
  int32 SampleFramesRead = 0;
  switch (format)
  {
    case eSampleFloat32:
    {
      float* pFloatBuffer = NULL;
      if (channels == 1)
      {
        pFloatBuffer = (float*)(buffers[0]);
        deleteBuffer = false;
      }
      else
      {
        pFloatBuffer = new float[sampleframes * channels];
        deleteBuffer = true;
      }
      
      SampleFramesRead = ReadIN((void*)pFloatBuffer, sampleframes, format); //mPosition is incremented inside
      for (int32 s = 0; s < SampleFramesRead; s++)
      {
        for (int32 c = 0; c < channels; c++)
        {
          float* pDest = (float*)buffers[c];
          pDest[s] = pFloatBuffer[channels * s + c];
        }
      }
      if (deleteBuffer)
        delete[] pFloatBuffer;
    }
      break;
      
    case eSampleInt16:
    {
      int16* pIntBuffer = NULL;
      if (channels == 1)
      {
        pIntBuffer = (int16*)(buffers[0]);
        deleteBuffer = false;
      }
      else
      {
        pIntBuffer = new int16[sampleframes * channels];
        deleteBuffer = true;
      }
      
      SampleFramesRead = ReadIN((void*)pIntBuffer, sampleframes, format);//mPosition is incremented inside
      for (int32 s = 0; s < SampleFramesRead; s++)
      {
        for (int32 c = 0; c < channels; c++)
        {
            ((int16*)(buffers[c]))[s] = pIntBuffer[channels * s + c];
        }
      }
      if (deleteBuffer)
        delete[] pIntBuffer;
    }
      break;
      
    default:
      break;
  }
  
  
  //don't increment mPosition: it's already done in ReadIN
  return SampleFramesRead;
}


//
//
//Chunk
//
//
nuiChunkSampleReader::Chunk::Chunk():
mPosition(0),
mDataSize(0),
mDataPosition(0),
mId(NULL)
{
}

nuiChunkSampleReader::Chunk::Chunk(nglFileOffset position, nglFileOffset dataSize, nglFileOffset dataPosition, const char* Id):
mPosition(position),
mDataSize(dataSize),
mDataPosition(dataPosition),
mId(NULL)
{
  if (Id)
  {
    mId = new char[strlen(Id) + 1];
    char* res = strcpy(mId, Id);
    int i = 0;
  }
}

nuiChunkSampleReader::Chunk::~Chunk()
{
  if (mId)
  {
    delete[] mId;
    mId = NULL;
  }
}


bool nuiChunkSampleReader::Chunk::CompareId(const char* Id) const
{
  const int compare = strcmp(mId, Id);
  return (compare == 0);
}

