/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
#include "nui.h"
#include "nuiSampleReader.h"


nuiSampleReader::nuiSampleReader(nglIStream& rStream) : mrStream(rStream)
{
  mIsSampleInfoRead = false;
  
  mPositionInStream = 0;
  mIsChunksScanned = false;
  mDataChunkIndex = 2; //third chunk is often data chunk
}

nuiSampleReader::nuiSampleReader(const nuiSampleReader& rReader, nglIStream& rStream)
: mrStream(rStream), mSampleInfo(rReader.mSampleInfo), mIsChunksScanned(rReader.mIsChunksScanned), mPosition(rReader.mPosition), mDataChunkIndex(rReader.mDataChunkIndex), mIsSampleInfoRead(rReader.mIsSampleInfoRead), mChunkIDPosition(rReader.mChunkIDPosition),
mChunkDataPosition(rReader.mChunkDataPosition), mChunkSize(rReader.mChunkSize), mChunkID(rReader.mChunkID), mPositionInStream(rReader.mPositionInStream)
{
}

nuiSampleReader::~nuiSampleReader()
{
}

uint64 nuiSampleReader::GetPosition() const
{
  return mPosition;
}

bool nuiSampleReader::SetPosition(uint64 SamplePos)
{
  //Make sure SamplePos is between 0 and SampleFrames
  if (0 <= SamplePos && mSampleInfo.GetSampleFrames() >= SamplePos)
  {
    mPosition = SamplePos;
    //#Mat Modif 050907
    if (mIsSampleInfoRead)
    {
      mrStream.SetPos(mChunkDataPosition[mDataChunkIndex] + (mPosition * mSampleInfo.GetChannels() * mSampleInfo.GetBitsPerSample() / 8),eStreamFromStart);
      return true;
    }
    else
        return false;
  }
  else
    return false;
}


//
//ReadChunkSize
//
//
//Read Selected Chunk Dta section Size and go to start of data section
//
bool nuiSampleReader::ReadChunkSize(uint8 ChunkNum, uint32& rChunkSize)
{
  if (ChunkNum > mChunkIDPosition.size())
    return false;
  
  mrStream.SetPos(mChunkIDPosition[ChunkNum] + 4);
  mPositionInStream = mChunkIDPosition[ChunkNum] + 4;
  
  if (1 != mrStream.ReadUInt32(&rChunkSize, 1))
    return false;
  if (0 != rChunkSize % 2)
    rChunkSize += 1;
	
  return true;
}


//
//ScanAllChunks
//
//
//Scan file to find all:
//                      -Position Of Chunk ID
//                      -Position Of Chunk Data
//                      -Size of Chunk Data
//                      -Name of Chunk ID
//
bool nuiSampleReader::ScanAllChunks()
{
  uint32 StreamSize = 0;
  uint32 CurrentChunkSize = 0;
  
  
  //uint8 ChunkHeaderSize = 4+4;  //size of ID + Size
  
  //find Strea Size
  mrStream.SetPos(0 + 4);
  mPositionInStream = 0 + 4;
  if (1 != mrStream.ReadUInt32(&StreamSize, 1))
    return false;
  
  //Set first chunk ID position (always 0)
  mChunkIDPosition.push_back(0);
  
  //Set first chunk Size(size of rest of file)
  mChunkSize.push_back(StreamSize);
  
  //Set second chunk ID position (always always 12)
  mChunkIDPosition.push_back(CHUNK_HEADER_SIZE+4);
  
  //Set second chunk Size(size of parameters section)
  if ( false == ReadChunkSize(1, CurrentChunkSize))
    return false;
  mChunkSize.push_back(CurrentChunkSize);
  
  //Set next Chunk ID positions and next Chunk Sizes
  uint8 i = 2;
  while ((mChunkIDPosition.back() + mChunkSize.back())< StreamSize)
  {
    mChunkIDPosition.push_back(mChunkIDPosition[i-1] + mChunkSize[i-1] + CHUNK_HEADER_SIZE);
    if ( false == ReadChunkSize(i, CurrentChunkSize))
	    return false;
    mChunkSize.push_back(CurrentChunkSize);
    i++;
  }
  
  
  std::vector<char> CurrentChunkID(4);
  
  uint8 j;
  uint8 k;
  
  for(j = 0; j < mChunkIDPosition.size(); j++)
  {
    //Set Data Positions
    mChunkDataPosition.push_back(mChunkIDPosition[j] + CHUNK_HEADER_SIZE);
    
    //Set Chunks IDs(name)
    mrStream.SetPos(mChunkIDPosition[j]);
    
    for(k = 0; k < 4; k++)
    {
      if( 1 != mrStream.ReadUInt8((uint8*)&(CurrentChunkID[k]), 1))
        return false;
    }
    
    mChunkID.push_back(CurrentChunkID);
    
  }
  
  return true;
}

//
//GoToChunk
//
//
//Go to Selected(by ChunkNum) Chunk Data Start Position
//
bool nuiSampleReader::GoToChunk(uint8 ChunkNum)
{
  if (ChunkNum < mChunkIDPosition.size())
  {
    mPositionInStream = mChunkDataPosition[ChunkNum];
    mrStream.SetPos(mPositionInStream);
    return true;
  }
  else
    return false;
}


bool nuiSampleReader::IsInfoRead()
{
  return mIsSampleInfoRead;
}

const nuiSampleInfo& nuiSampleReader::GetSampleInfo() const
{
  return mSampleInfo;
}