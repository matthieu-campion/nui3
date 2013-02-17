/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiChunksDefinitions.h"

#ifndef HiWord
#define HiWord(x) ((short)((long)(x) >> 16))
#define LoWord(x) ((short)(x))
#endif


double ExtendedToDouble(extended x)
{
  double q;
  uint32 mant,mant2;
  int16 sign;
  
  sign = x.exp[0] & 0x8000;
  sign = sign ? -1 : 1;
  
  x.exp[0] &= 0x7FFF;
  
  mant = ((uint32)x.man[0]) << 16;
  mant += (uint16)x.man[1];
  mant2 = ((uint32)x.man[2]) << 16;
  mant2 += (uint16)x.man[3];
  
  q = sign * (double)mant * pow(2.,(double)(x.exp[0] - 0x401E));
  q += sign * (double)mant2 * pow(2.,(double)(x.exp[0] - 0x403E));
  
  return q;
}

extended DoubleToExtended(double q)
{
  extended x;
  uint32 mant, mant2;
  int16 sign;
  uint16 exp;
  
  sign = q<0 ? 0x8000 : 0;
  
  if (sign)
  {
    q = -q;
  }
  
  exp = (uint16) (log(q)/log(2.));
  
  mant = (uint32)(q/pow(2., (double)exp-31.));
  
  q -= (double )mant * pow(2.,(double)exp-31.);
  mant2 = (uint32)(q/pow(2.,(double)exp-63));
  
  exp += 0x3FFF;
  exp |= sign;
  
  x.exp[0] = exp;
  x.man[0] = HiWord(mant);
  x.man[1] = LoWord(mant);
  x.man[2] = HiWord(mant2);
  x.man[3] = LoWord(mant2);
  
  return x;
}


nuiAiffReader::nuiAiffReader(nglIStream& rStream) :
nuiChunkSampleReader(rStream)
{
  mrStream.SetEndian(eEndianBig);
}

nuiAiffReader::nuiAiffReader(const nuiAiffReader& rReader, nglIStream& rStream) :
nuiChunkSampleReader(rReader, rStream)
{
  mrStream.SetEndian(eEndianBig);
}

nuiAiffReader::~nuiAiffReader()
{
}

nuiSampleReader* nuiAiffReader::Clone(nglIStream& rStream) const
{
  nuiAiffReader* pReader = new nuiAiffReader(*this, rStream);
  pReader->SetPosition(0);
  return pReader;
}

void nuiAiffReader::SetPosition(int64 position)
{
  if (!mInitialized)
    return;
  
  Chunk* pDataChunk = GetChunk("SSND");
  NGL_ASSERT(pDataChunk);
  nglFileOffset StreamPosition = pDataChunk->mDataPosition + mInfo.GetChannels() * (mInfo.GetBitsPerSample() / 8) * position;
  mrStream.SetPos(StreamPosition);
  mPosition = position;
}

bool nuiAiffReader::ReadInfo()
{
  ScanAllChunks();
  
  //there are 8 unused bytes at the beginning of the "SSND" chunk in an AIFF file
  Chunk* pSSNDChunk = GetChunk("SSND");
  if (!pSSNDChunk)
    return false;
  pSSNDChunk->mDataPosition += 8;
  
  bool res = false;
  
  res = GoToChunk("FORM");
  if (!res)
    return false;
  std::vector<char> fileFormat(4);
  char* waveFormat = "AIFF";
  for(int32 i = 0; i < 4; i++)
  {
    if( 1 != mrStream.ReadUInt8((uint8*)&(fileFormat[i]), 1))
      return false;
    if (fileFormat[i] != waveFormat[i])
      return false; // this stream is not a wave stream
  }
  mInfo.SetFileFormat(eAudioWave);
  
  res = GoToChunk("COMM");
  if (!res)
    return false;
  
  int16 Channels = 0;
  uint32 SampleFrames = 0;
  int16 SampleSize = 0;
  extended SampleRate;
  
  //Read all parameters given in "COMM" chunk
  if( 1 != mrStream.ReadInt16(&Channels,1))
    return false;
  if( 1 != mrStream.ReadUInt32(&SampleFrames,1))
    return false;
  if( 1 != mrStream.ReadInt16(&SampleSize,1))
    return false;
  
  //read SampleRate exponent
  if( 1 != mrStream.ReadInt16(SampleRate.exp,1))
    return false;
  //read Samplerate Mantisse
  if( 4 != mrStream.ReadInt16(SampleRate.man,4))
    return false;
  
  
  mInfo.SetFormatTag(AIFF_FORMAT_TAG);
  
  mInfo.SetSampleRate(ExtendedToDouble(SampleRate));
  mInfo.SetChannels(Channels);
  mInfo.SetBitsPerSample(SampleSize);
  mInfo.SetSampleFrames(SampleFrames);
  
  mInfo.SetStartFrame(0);
  mInfo.SetStopFrame(mInfo.GetSampleFrames());
  
  return true;
}

int32 nuiAiffReader::ReadIN(void* pBuffer, int32 SampleFrames, nuiSampleBitFormat format)
{
  if (!SampleFrames)
    return 0;
  SetPosition((uint32)mPosition);
  const int32 channels = mInfo.GetChannels();
  int32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  SampleFrames = MIN(SampleFrames, length - mPosition);
  
  const int64 SamplePointsToRead = SampleFrames * channels;
  int32 SampleFramesRead = 0;
  
  switch(format)
  {
    case eSampleInt16 :
    {
      switch (mInfo.GetBitsPerSample())
      {
        case 8 :
        {
          int16* pTempInt16 = (int16*)pBuffer;
          int32 sizeRead = mrStream.ReadUInt8( ((uint8*)pTempInt16) + SamplePointsToRead , SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
          nuiAudioConvert_Signed8bitsBufferTo16Bits(pTempInt16, sizeRead);
        }
          break;
          
        case 16 :
        {
          int32 sizeRead = mrStream.ReadInt16( (int16*)pBuffer, SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
        }
          break;
          
        case 24 :
        case 32 :
        default:
          SampleFramesRead = 0;
          break;
      }
    }
      break;
      
    case eSampleFloat32 :
    {
      switch ( mInfo.GetBitsPerSample() )
      {
        case 8 :
        {
          float* pTempFloat = (float*)pBuffer;
          int32 sizeRead = mrStream.ReadUInt8( (uint8*)pTempFloat + 3 * SamplePointsToRead , SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
          nuiAudioConvert_Signed8bitsBufferToFloat(pTempFloat, sizeRead);
        }
          break;
          
        case 16 :
        {
          float* pTempFloat = (float*)pBuffer;
          int32 sizeRead = mrStream.ReadInt16( (int16*)pTempFloat + SamplePointsToRead, SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
          nuiAudioConvert_16bitsBufferToFloat(pTempFloat, SamplePointsToRead);
        }
          break;
          
        case 24 :
        {
          float* pTempFloat = (float*)pBuffer;
          uint8* pTempBuffer;
          pTempBuffer = new uint8[SamplePointsToRead * 3]; //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
          
          int32 sizeRead = mrStream.ReadUInt8(pTempBuffer, SamplePointsToRead * 3);
          SampleFramesRead = sizeRead / channels / 3;
          for (int32 i = 0; i < sizeRead / 3; i++)
          {
            pTempFloat[i] = nuiAudioConvert_24bitsToFloatFromBigEndian(pTempBuffer + (3 * i));
          }
          
          delete[] pTempBuffer;
        }
          break;
          
        case 32 :
        {
          int32 sizeRead = mrStream.ReadFloat((float*)pBuffer, SamplePointsToRead);
          SampleFramesRead = sizeRead / channels;
          mPosition += SampleFramesRead;
        }
          break;
          
        default:
          SampleFramesRead = 0;
          break;
      }
    }
      break;
      
    default :
      SampleFramesRead = 0;
      break;
  }
  
  mPosition += SampleFramesRead;
  return SampleFramesRead;
}


