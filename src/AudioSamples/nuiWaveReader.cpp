/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


nuiWaveReader::nuiWaveReader(nglIStream& rStream) : nuiChunkSampleReader(rStream)
{
  mrStream.SetEndian(eEndianLittle);
}

nuiWaveReader::nuiWaveReader(const nuiWaveReader& rReader, nglIStream& rStream) : 
nuiChunkSampleReader(rReader, rStream)
{
  mrStream.SetEndian(eEndianLittle);
}

nuiWaveReader::~nuiWaveReader()
{
}

nuiSampleReader* nuiWaveReader::Clone(nglIStream& rStream) const
{
  nuiWaveReader* pReader = new nuiWaveReader(*this, rStream);
  pReader->SetPosition(0);
  return pReader;
}

void nuiWaveReader::SetPosition(int64 position)
{
  if (!mInitialized)
    return;
  
  Chunk* pDataChunk = GetChunk("data");
  
  nglFileOffset newStreamPosition = pDataChunk->mDataPosition + mInfo.GetChannels() * (mInfo.GetBitsPerSample() / 8) * position;
  nglFileOffset newPos = mrStream.SetPos(newStreamPosition);
  NGL_ASSERT(newPos == newStreamPosition);
  mPosition = position;
}

bool nuiWaveReader::ReadInfo()
{
  ScanAllChunks();
  
  uint16 Format = eWaveFormatUnknown;
  uint16 NbChannels = 0;
  uint32 SamplesPerSec = 0;
  uint32 AvgBytesPerSec = 0;
  uint16 BlockAlign = 0;
  uint16 BitsPerSample = 0;
  uint32 NbSamples = 0;
  
  bool res = false;
  
  res = GoToChunk("RIFF");
  if (!res)
    return false;
  std::vector<char> fileFormat(4);
  char* waveFormat = "WAVE";
  for (int32 i = 0; i < 4; i++)
  {
    if( 1 != mrStream.ReadUInt8((uint8*)&(fileFormat[i]), 1))
      return false;
    if (fileFormat[i] != waveFormat[i])
      return false; // this stream is not a wave stream
  }
  mInfo.SetFileFormat(eAudioWave);

  res = GoToChunk("fmt ");
  if (!res)
    return false;
   
  //Read all parameters given in "fmt" chunk
  if( 1 != mrStream.ReadUInt16(&Format,1))
    return false;
  if( 1 != mrStream.ReadUInt16(&NbChannels,1))
    return false;
  if( 1 != mrStream.ReadUInt32(&SamplesPerSec,1))
    return false;
  if( 1 != mrStream.ReadUInt32(&AvgBytesPerSec,1))
    return false;
  if( 1 != mrStream.ReadUInt16(&BlockAlign,1))
    return false;
  if( 1 != mrStream.ReadUInt16(&BitsPerSample,1))
    return false;
  
  
  mInfo.SetFormatTag(Format);
  mInfo.SetSampleRate(SamplesPerSec);
  mInfo.SetChannels(NbChannels);
  mInfo.SetBitsPerSample(BitsPerSample);
  
  //get number of sample frames
  for(int32 i = 0; i < mChunks.size(); i++)
  {
    if(mChunks[i]->CompareId("data"))  //attempt to reach "data" Chunk
    {
      NbSamples = (int32)mChunks[i]->mDataSize;
      mInfo.SetSampleFrames(NbSamples * 8 / BitsPerSample / NbChannels);
      break;
    }
  }

  mInfo.SetStartFrame(0);
  mInfo.SetStopFrame(mInfo.GetSampleFrames());
  
  return true;
}



int32 nuiWaveReader::ReadIN(void* pBuffer, int32 sampleframes, nuiSampleBitFormat format)
{
  if (!sampleframes)
    return 0;
  SetPosition((int32)mPosition);
  
  int32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  sampleframes = MIN(sampleframes, length - mPosition);
  
  const int64 SamplePointsToRead = sampleframes * mInfo.GetChannels();
  int64 SampleFramesRead = 0;
  const int32 channels = mInfo.GetChannels();
  switch(format)
  {
    case eSampleInt16 :
    {
      switch (mInfo.GetBitsPerSample())
      {
        case 8:
        {
          int16* pTempInt16 = (int16*)pBuffer;
          int32 sizeRead = (int32)mrStream.ReadUInt8(((uint8*)pTempInt16) + SamplePointsToRead , SamplePointsToRead);
          nuiAudioConvert_Unsigned8bitsBufferTo16Bits(pTempInt16, sizeRead);
          SampleFramesRead = sizeRead / channels;
        }
          break;
          
        case 16:
        {
          int32 sizeRead = (int32)mrStream.ReadInt16((int16*)pBuffer, SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
        }
          break;
          
        case 24:
        case 32:
        default:
          NGL_ASSERT(0);
          SampleFramesRead = 0;
          break;
      }
    }
      break;
      
    case eSampleFloat32 :
    {
      switch (mInfo.GetBitsPerSample())
      {
        case 8:
        {
          float* pTempFloat = (float*)pBuffer;
          int32 sizeRead = (int32)mrStream.ReadUInt8((uint8*)pTempFloat + 3 * SamplePointsToRead , SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
          nuiAudioConvert_Unsigned8bitsBufferToFloat(pTempFloat, sizeRead);
        }
          break;
          
        case 16:
        {
          float* pTempFloat = (float*)pBuffer;

          int32 sizeRead = (int32)mrStream.ReadInt16((int16*)pTempFloat + SamplePointsToRead, SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
          nuiAudioConvert_16bitsBufferToFloat(pTempFloat, sizeRead);
        }
          break;
          
        case 24:
        {
          float* pTempFloat = (float*)pBuffer;
          uint8* pTempBuffer;
          pTempBuffer = new uint8[SamplePointsToRead * 3]; //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
          
          int32 sizeRead = (int32)mrStream.ReadUInt8(pTempBuffer, SamplePointsToRead * 3);
          SampleFramesRead = (sizeRead / channels) / 3;
          for (int64 i = 0; i < sizeRead / 3; i++)
          {
            pTempFloat[i] = nuiAudioConvert_24bitsToFloatFromLittleEndian(pTempBuffer + (3 * i));
          }
          
          delete[] pTempBuffer;
        }
          break;
          
        case 32:
        {
          int32 sizeRead = (int32)mrStream.ReadFloat((float*)pBuffer, SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
        }
          break;
          
        default:
          NGL_ASSERT(0);
          SampleFramesRead = 0;
          break;
      }
    }
      break;
    default:
      NGL_ASSERT(0);
      SampleFramesRead = 0;
      break;
  }
  
  mPosition += SampleFramesRead;
  return (int32)SampleFramesRead;
}


