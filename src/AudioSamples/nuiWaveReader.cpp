/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiWaveReader.h"


/*
 ConvertUnsigned8bitsBufferTo16Bits
 */
//////////////////////////////////////
void ConvertUnsigned8bitsBufferTo16Bits(int16* pBuffer, uint64 SizeToRead)
{
  static const float mult = 2.0 / 255.0f;
  
  float Temp;
  
  uint64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    Temp = ((uint8*)pBuffer)[ SizeToRead + i ] * mult - 1.0;
    
    if (Temp < 0)
      pBuffer[i] = (int16)ToBelow(Temp * 32768.f);
    else
      pBuffer[i] = (int16)ToBelow(Temp * 32767.f);
  }
}

/*
 ConvertUnsigned8bitsBufferToFloat
 */
//////////////////////////////////////
void ConvertUnsigned8bitsBufferToFloat(float* pBuffer, uint64 SizeToRead)
{
  static const float mult = 2.0 / 255.0f;
  
  uint64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    pBuffer[i] = ((uint8*)pBuffer)[ 3 * SizeToRead + i ] * mult - 1.0;
  }
}



//
//ConvertSigned8bitsBufferTo16Bits
//
//////////////////////////////////////
void ConvertSigned8bitsBufferTo16Bits(int16* pBuffer, uint64 SizeToRead)
{
  static const float mult1 = 1.0 / 128.0f;
  static const float mult2 = 1.0 / 127.0f;
  
  float Temp;
  
  uint64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    Temp = ((int8*)pBuffer)[ SizeToRead + i ];
    
    if (Temp < 0)
      pBuffer[i] = (int16)ToBelow(Temp * mult1 * 32768);
    else
      pBuffer[i] = (int16)ToBelow(Temp * mult2 * 32767);
  }
}

//
//ConvertSigned8bitsBufferToFloat
//
//////////////////////////////////////
void ConvertSigned8bitsBufferToFloat(float* pBuffer, uint64 SizeToRead)
{
  static const float mult1 = 1.0 / 128.0f;
  static const float mult2 = 1.0 / 127.0f;
  
  float Temp;
  
  uint64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    Temp = ((int8*)pBuffer)[ 3 * SizeToRead + i ];
    if ( Temp < 0 )
      pBuffer[i] = Temp * mult1;
    else
      pBuffer[i] = Temp * mult2;
      
  }
}


/*
 Convert16bitsBufferToFloat
 */
//////////////////////////////////////
void Convert16bitsBufferToFloat(float* pBuffer, uint64 SizeToRead)
{
  static const float mult1 = 1.0 / 32768.0f;
  static const float mult2 = 1.0 / 32767.0f;
  
  float Temp;
  
  uint64 i;
  for (i = 0; i< SizeToRead; i++)
  {
    Temp = ((int16*)pBuffer)[ SizeToRead + i ];
    if (Temp < 0)
      pBuffer[i] = Temp * mult1;
    else
      pBuffer[i] = Temp * mult2;
  }
}




float Convert24bitsToFloatFromLittleEndian(uint8* pBytesBuf)
{
  static const float mult1 = 1.0 / 8388608.0f ;
  static const float mult2 = 1.0 / 8388607.0f ;

  int32 TempInt32 = ((pBytesBuf[2]<<16) + (pBytesBuf[1]<<8) + pBytesBuf[0])<<8 ;
  TempInt32 >>= 8;
  
  if (TempInt32 < 0)
    return (float)(TempInt32 * mult1);
  else
    return (float)(TempInt32 * mult2);
}

//#fixme
//not tested
float Convert24bitsToFloatFromBigEndian(uint8* pBytesBuf)
{
  static const float mult1 = 1.0 / 8388608.0f ;
  static const float mult2 = 1.0 / 8388607.0f ;
  
  int32 TempInt32 = ((pBytesBuf[0]<<16) + (pBytesBuf[1]<<8) + pBytesBuf[2])<<8 ;
  TempInt32 >>= 8;
  
  if (TempInt32 < 0)
    return (float)(TempInt32 * mult1);
  else
    return (float)(TempInt32 * mult2);
  return 0.0;
}

//
//ConvertFloatTo24bitsFromLittleEndian
//
void ConvertFloatTo24bitsLittleEndian(float* pInBuffer, uint8* pOutBuffer, uint64 SizeToRead)
{
  static const float mult1 = 8388608.0f ;
  static const float mult2 = 8388607.0f ;
  
  int32 TempInt32;
  
  uint64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    if ( pInBuffer[i] < 0 )
      TempInt32 = ToBelow( pInBuffer[i] * mult1 );
    else
      TempInt32 = ToBelow( pInBuffer[i] * mult2 );
    
    pOutBuffer[ 3 * i] = (uint8)(TempInt32);
    pOutBuffer[ 3 * i + 1] = (uint8)(TempInt32>>8);
    pOutBuffer[ 3 * i + 2] = (uint8)(TempInt32>>16);
  }
}

void ConvertFloatTo24bitsBigEndian(float* pInBuffer, uint8* pOutBuffer, uint64 SizeToRead)
{
  static const float mult1 = 8388608.0f ;
  static const float mult2 = 8388607.0f ;
  
  int32 TempInt32;
  
  uint64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    if ( pInBuffer[i] < 0 )
      TempInt32 = ToBelow( pInBuffer[i] * mult1 );
    else
      TempInt32 = ToBelow( pInBuffer[i] * mult2 );
    
    pOutBuffer[ 3 * i] = (uint8)(TempInt32>>16);
    pOutBuffer[ 3 * i + 1] = (uint8)(TempInt32>>8);
    pOutBuffer[ 3 * i + 2] = (uint8)(TempInt32);
  }  
}

void ConvertFloatTo32bits(float* pInBuffer, int32* pOutBuffer, uint64 SizeToRead)
{
  static const float mult1 = 2147483648.0f ;
  static const float mult2 = 2147483647.0f ;
  
  uint64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    if ( pInBuffer[i] < 0 )
      pOutBuffer[i] = ToBelow(pInBuffer[i] * mult1);
    else
      pOutBuffer[i] = ToBelow(pInBuffer[i] * mult2);
  }  
}


void Convert32bitsToFloat(int32* pInBuffer, float* pOutBuffer, uint64 SizeToRead)
{
  static const float mult1 =  1.0 / 2147483648.0f ;
  static const float mult2 = 1.0 / 2147483647.0f ;
  
  uint64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    if (pInBuffer[i] < 0)
      pOutBuffer[i] = pInBuffer[i] * mult1;
    else
      pOutBuffer[i] = pInBuffer[i] * mult2;
  }
}


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
  
  switch(format)
  {
    case eSampleInt16 :
      {
        switch ( mSampleInfo.GetBitsPerSample() )
        {
          case 8 :
            {
              int16* pTempInt16 = (int16*)pBuffer;
              
              int64 sizeRead;
              sizeRead = mrStream.ReadUInt8( ((uint8*)pTempInt16) + NbSamplePointsToRead , NbSamplePointsToRead);
              mPosition += sizeRead / mSampleInfo.GetChannels();
              ConvertUnsigned8bitsBufferTo16Bits(pTempInt16, sizeRead);
              return sizeRead / mSampleInfo.GetChannels();
            }
            break;
            
          case 16 :
            {
              int64 sizeRead;
              sizeRead = mrStream.ReadInt16( (int16*)pBuffer, NbSamplePointsToRead);
              mPosition += sizeRead / mSampleInfo.GetChannels();
              return sizeRead / mSampleInfo.GetChannels();
            }
            break;
            
          case 24 :
          case 32 :
          default:
            return 0;
            break;
        }
      }
      break;
      
    case eSampleFloat32 :
      {
        switch ( mSampleInfo.GetBitsPerSample() )
        {
          case 8 :
            {
              float* pTempFloat = (float*)pBuffer;
              int64 sizeRead;
              
              sizeRead = mrStream.ReadUInt8( (uint8*)pTempFloat + 3 * NbSamplePointsToRead , NbSamplePointsToRead);
              mPosition += sizeRead / mSampleInfo.GetChannels();
              ConvertUnsigned8bitsBufferToFloat(pTempFloat, sizeRead);
              return sizeRead / mSampleInfo.GetChannels();
            }
            break;
            
          case 16 :
            {
              float* pTempFloat = (float*)pBuffer;
              int64 sizeRead;
              
              sizeRead = mrStream.ReadInt16( (int16*)pTempFloat + NbSamplePointsToRead, NbSamplePointsToRead);
              mPosition += sizeRead / mSampleInfo.GetChannels();
              
              Convert16bitsBufferToFloat(pTempFloat, NbSamplePointsToRead);
              return sizeRead / mSampleInfo.GetChannels();
            }
            break;
            
          case 24 :
            {
              float* pTempFloat = (float*)pBuffer;
              int64 sizeRead;
              
              uint8* pTempBuffer;
              pTempBuffer = new uint8[NbSamplePointsToRead * 3]; //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
              
              sizeRead = mrStream.ReadUInt8(pTempBuffer, NbSamplePointsToRead * 3)/3; //divide by 3 to get actual number of read samples
              mPosition += sizeRead / mSampleInfo.GetChannels();
              
              uint64 i;
              for (i = 0; i < sizeRead; i++)
              {
                pTempFloat[i] = Convert24bitsToFloatFromLittleEndian(pTempBuffer + (3 * i));
              }

              delete[] pTempBuffer;
              
              return sizeRead / mSampleInfo.GetChannels();
            }
            break;
            
          case 32 :
            {
              int64 sizeRead;
              sizeRead = mrStream.ReadFloat( (float*)pBuffer, NbSamplePointsToRead);
              mPosition += sizeRead / mSampleInfo.GetChannels();
              return sizeRead / mSampleInfo.GetChannels();
            }
            break;
            
          default:
              return false;
              break;
        }
      }
      break;
      
    default :
      return false;
      break;
  }
}
