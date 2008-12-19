/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiWaveReader.h"
#include "nuiAudioConvert.h"
#include "nuiChunksDefinitions.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CompareID(std::vector<char> ID,  char* IDname) 
{
  uint8 i;
  for(i = 0; i < 4; i++)
  {
    if (ID[i] != IDname[i])
      return false;
  }
  return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

nuiWaveReader::nuiWaveReader(nglIStream& rStream) : nuiSampleReader(rStream)
{
  mrStream.SetEndian(eEndianLittle);
  
  /*
  mPositionInStream = 0;
  mIsChunksScanned = false;
   */
}

nuiWaveReader::nuiWaveReader(const nuiSampleReader& rReader, nglIStream& rStream)
: nuiSampleReader(rReader, rStream)
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


//
//ReadInfo
//
//
//Read all wave file infos given in "fmt" chunk
//
bool  nuiWaveReader::ReadInfo(nuiSampleInfo& rInfos)
{
  uint16 Format = eWaveFormatUnknown;
  uint16 NbChannels = 0;
  uint32 SamplesPerSec = 0;
  uint32 AvgBytesPerSec = 0;
  uint16 BlockAlign = 0;
  uint16 BitsPerSample = 0;
  
  uint32 NbSamples = 0;
  
  //Go to first Chunk "RIFF"
  mrStream.SetPos(8);
  //Read File Format
  std::vector<char> TempFileFormat(4);

  uint8 i;
  for(i = 0; i < 4; i++)
  {
    if( 1 != mrStream.ReadUInt8((uint8*)&(TempFileFormat[i]), 1))
      return false;
  }
  //Compare File format with "WAVE"
  if ( false == CompareID(TempFileFormat, "WAVE") )
    return false;
  else
    rInfos.SetFileFormat(eAudioWave);
  
  // This is a wav file: scan chunks
  if (false == mIsChunksScanned)
  {
    ScanAllChunks();
    mIsChunksScanned = true;
  }
  
  //Go to "fmt " Chunk
  for(i = 0; i < mChunkID.size(); i++)
  {
    if( true == CompareID(mChunkID[i], "fmt "))  //attempt to reach "fmt " Chunk
    {
      GoToChunk(i);
      break;
    }
  }
 
  
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
  
  
  rInfos.SetFormatTag(Format);
  rInfos.SetSampleRate(SamplesPerSec);
  rInfos.SetChannels(NbChannels);
  rInfos.SetBitsPerSample(BitsPerSample);
  
  
  for(i = 0; i < mChunkID.size(); i++)
  {
    if( true == CompareID(mChunkID[i], "data"))  //attempt to reach "data" Chunk
    {
      ReadChunkSize(i, NbSamples);  // Size given in data Chunk is number of sample frames
      rInfos.SetSampleFrames(NbSamples * 8 / BitsPerSample / NbChannels);
      
      mDataChunkIndex = i;
      
      break;
    }
  }
  
  
  rInfos.SetStartFrame(0);
  rInfos.SetStopFrame(rInfos.GetSampleFrames());
  
  
  
  // place Wave File infos in SampleInfo member (mSampleInfo)
  mSampleInfo = rInfos;
  mIsSampleInfoRead = true;
  
  return true;
}



//
//Read
//
uint32 nuiWaveReader::Read(void* pBuffer, uint32 SampleFrames, nuiSampleBitFormat format)
{
  const uint64 NbSamplePointsToRead = SampleFrames * mSampleInfo.GetChannels();
  uint64 nb = 0;
  
  switch(format)
  {
  case eSampleInt16 :
    {
      switch (mSampleInfo.GetBitsPerSample())
      {
      case 8:
        {
          int16* pTempInt16 = (int16*)pBuffer;
          
          int64 sizeRead;
          sizeRead = mrStream.ReadUInt8(((uint8*)pTempInt16) + NbSamplePointsToRead , NbSamplePointsToRead);
          mPosition += sizeRead / mSampleInfo.GetChannels();
          nuiAudioConvert_Unsigned8bitsBufferTo16Bits(pTempInt16, sizeRead);
          nb = sizeRead / mSampleInfo.GetChannels();
        }
        break;
        
      case 16:
        {
          int64 sizeRead;
          sizeRead = mrStream.ReadInt16((int16*)pBuffer, NbSamplePointsToRead);
          mPosition += sizeRead / mSampleInfo.GetChannels();
          nb = sizeRead / mSampleInfo.GetChannels();
        }
        break;
        
      case 24:
      case 32:
      default:
        NGL_ASSERT(0);
        nb = 0;
        break;
      }
    }
    break;
    
  case eSampleFloat32 :
    {
      switch (mSampleInfo.GetBitsPerSample())
      {
      case 8:
        {
          float* pTempFloat = (float*)pBuffer;
          int64 sizeRead;
          
          sizeRead = mrStream.ReadUInt8((uint8*)pTempFloat + 3 * NbSamplePointsToRead , NbSamplePointsToRead);
          mPosition += sizeRead / mSampleInfo.GetChannels();
          nuiAudioConvert_Unsigned8bitsBufferToFloat(pTempFloat, sizeRead);
          nb = sizeRead / mSampleInfo.GetChannels();
        }
        break;
        
      case 16:
        {
          float* pTempFloat = (float*)pBuffer;
          int64 sizeRead;
          
          sizeRead = mrStream.ReadInt16((int16*)pTempFloat + NbSamplePointsToRead, NbSamplePointsToRead);
          mPosition += sizeRead / mSampleInfo.GetChannels();
          
          nuiAudioConvert_16bitsBufferToFloat(pTempFloat, NbSamplePointsToRead);
          nb = sizeRead / mSampleInfo.GetChannels();
        }
        break;
        
      case 24:
        {
          float* pTempFloat = (float*)pBuffer;
          int64 sizeRead;
          
          uint8* pTempBuffer;
          pTempBuffer = new uint8[NbSamplePointsToRead * 3]; //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
          
          sizeRead = mrStream.ReadUInt8(pTempBuffer, NbSamplePointsToRead * 3) / 3; //divide by 3 to get actual number of read samples
          mPosition += sizeRead / mSampleInfo.GetChannels();
          
          uint64 i;
          for (i = 0; i < sizeRead; i++)
          {
            pTempFloat[i] = nuiAudioConvert_24bitsToFloatFromLittleEndian(pTempBuffer + (3 * i));
          }

          delete[] pTempBuffer;
          
          nb = sizeRead / mSampleInfo.GetChannels();
        }
        break;
        
      case 32:
        {
          int64 sizeRead;
          sizeRead = mrStream.ReadFloat((float*)pBuffer, NbSamplePointsToRead);
          mPosition += sizeRead / mSampleInfo.GetChannels();
          nb = sizeRead / mSampleInfo.GetChannels();
        }
        break;
        
      default:
        NGL_ASSERT(0);
        nb = 0;
        break;
      }
    }
    break;
  default:
    NGL_ASSERT(0);
    nb = 0;
    break;
  }
  
  return nb;
}
