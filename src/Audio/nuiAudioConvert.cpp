/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


void nuiAudioConvert_INint16ToDEfloat(const int16* input, float* output, int32 curChannel, int32 nbChannels, int32 nbSampleFrames)
{
  static const float mult1 = 1.0 / 32768.0f;
  static const float mult2 = 1.0 / 32767.0f;
  int32 j;

  input += curChannel;
  for (j = 0; j < nbSampleFrames; j++)
  {
    int16 in = *input;

    if (in < 0)
      *output++ = (float)in * mult1;
    else
      *output++ = (float)in * mult2;

    input += nbChannels;
  }
}



void nuiAudioConvert_DEfloatToINint16(const float* input, int16* output, int32 curChannel, int32 nbChannels, int32 nbSampleFrames) // de-interlaced float to interlaced int16
{
  static const float mult1 = 32768.0f ;
  static const float mult2 = 32767.0f ;
  int32 j;

  output += curChannel;
  for (j=0; j < nbSampleFrames; j++)
  {
    float in = *input++;
    in = nuiClamp(in, -1.0f, 1.0f);
    if (in < 0)
      *output = ToZero(in * mult1);
    else
      *output = ToZero(in * mult2);

    output += nbChannels;
  } 
}

/*
 nuiAudioConvert_Unsigned8bitsBufferTo16Bits
 */
//////////////////////////////////////
void nuiAudioConvert_Unsigned8bitsBufferTo16Bits(int16* pBuffer, int64 SizeToRead)
{
  static const float mult = 2.0 / 255.0f;
  
  float Temp;
  
  int64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    Temp = ((int8*)pBuffer)[ SizeToRead + i ] * mult - 1.0;
    
    if (Temp < 0)
      pBuffer[i] = (int16)ToZero(Temp * 32768.f);
    else
      pBuffer[i] = (int16)ToZero(Temp * 32767.f);
  }
}

/*
 nuiAudioConvert_Unsigned8bitsBufferToFloat
 */
//////////////////////////////////////
void nuiAudioConvert_Unsigned8bitsBufferToFloat(float* pBuffer, int64 SizeToRead)
{
  static const float mult = 2.0 / 255.0f;
  
  int64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    pBuffer[i] = ((int8*)pBuffer)[ 3 * SizeToRead + i ] * mult - 1.0;
  }
}



//
//nuiAudioConvert_Signed8bitsBufferTo16Bits
//
//////////////////////////////////////
void nuiAudioConvert_Signed8bitsBufferTo16Bits(int16* pBuffer, int64 SizeToRead)
{
  static const float mult1 = 1.0 / 128.0f;
  static const float mult2 = 1.0 / 127.0f;
  
  float Temp;
  
  int64 i;
  for (i = 0; i < SizeToRead; i++)
  {
    Temp = ((int8*)pBuffer)[ SizeToRead + i ];
    
    if (Temp < 0)
      pBuffer[i] = (int16)ToZero(Temp * mult1 * 32768);
    else
      pBuffer[i] = (int16)ToZero(Temp * mult2 * 32767);
  }
}

//
//nuiAudioConvert_Signed8bitsBufferToFloat
//
//////////////////////////////////////
void nuiAudioConvert_Signed8bitsBufferToFloat(float* pBuffer, int64 SizeToRead)
{
  static const float mult1 = 1.0 / 128.0f;
  static const float mult2 = 1.0 / 127.0f;
  
  float Temp;
  
  int64 i;
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
 nuiAudioConvert_16bitsBufferToFloat
 */
//////////////////////////////////////
void nuiAudioConvert_16bitsBufferToFloat(float* pBuffer, int64 SizeToRead)
{
  static const float mult1 = 1.0 / 32768.0f;
  static const float mult2 = 1.0 / 32767.0f;
  
  float Temp;
  
  int64 i;
  for (i = 0; i< SizeToRead; i++)
  {
    Temp = ((int16*)pBuffer)[ SizeToRead + i ];
    if (Temp < 0)
      pBuffer[i] = Temp * mult1;
    else
      pBuffer[i] = Temp * mult2;
  }
}

void nuiAudioConvert_FloatBufferTo16bits(float* pFloatBuffer, int16* pInt16Buffer, int64 SizeToRead)
{
  static const float mult1 = 32768.0f;
  static const float mult2 = 32767.0f;
  float Temp;
  for (int64 i = 0; i< SizeToRead; i++)
  {
    Temp = pFloatBuffer[i];
    Temp = nuiClamp(Temp, -1.0f, 1.0f);
    if (Temp < 0)
      pInt16Buffer[i] = ToZero(Temp * mult1);
    else
      pInt16Buffer[i] = ToZero(Temp * mult2);
  }
}


float nuiAudioConvert_24bitsToFloatFromLittleEndian(uint8* pBytesBuf)
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
float nuiAudioConvert_24bitsToFloatFromBigEndian(uint8* pBytesBuf)
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
//nuiAudioConvert_FloatTo24bitsFromLittleEndian
//
void nuiAudioConvert_FloatTo24bitsLittleEndian(float* pInBuffer, uint8* pOutBuffer, int64 SizeToRead)
{
  static const float mult1 = 8388608.0f ;
  static const float mult2 = 8388607.0f ;
  
  int32 TempInt32;
  
  int64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    float value = pInBuffer[i];
    value = nuiClamp(value, -1.0f, 1.0f);
    if ( pInBuffer[i] < 0 )
    {
      TempInt32 = ToZero( value * mult1 );
      NGL_ASSERT(TempInt32 <= 0);
      NGL_ASSERT(TempInt32 >= -8388608);
    }
    else
    {
      TempInt32 = ToZero( value * mult2 );
      NGL_ASSERT(TempInt32 >= 0);
      NGL_ASSERT(TempInt32 <= 8388607);
    }
      
    
    pOutBuffer[ 3 * i] = (uint8)(TempInt32);
    pOutBuffer[ 3 * i + 1] = (uint8)(TempInt32>>8);
    pOutBuffer[ 3 * i + 2] = (uint8)(TempInt32>>16);
  }
}

void nuiAudioConvert_FloatTo24bitsBigEndian(float* pInBuffer, uint8* pOutBuffer, int64 SizeToRead)
{
  static const float mult1 = 8388608.0f ;
  static const float mult2 = 8388607.0f ;
  
  int32 TempInt32;
  
  int64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    float value = pInBuffer[i];
    value = nuiClamp(value, -1.0f, 1.0f);
    if ( pInBuffer[i] < 0 )
    {
      TempInt32 = ToZero( value * mult1 );
      NGL_ASSERT(TempInt32 <= 0);
      NGL_ASSERT(TempInt32 >= -8388608);
    }
    else
    {
      TempInt32 = ToZero( value * mult2 );
      NGL_ASSERT(TempInt32 >= 0);
      NGL_ASSERT(TempInt32 <= 8388607);
    }
    
    pOutBuffer[ 3 * i] = (uint8)(TempInt32>>16);
    pOutBuffer[ 3 * i + 1] = (uint8)(TempInt32>>8);
    pOutBuffer[ 3 * i + 2] = (uint8)(TempInt32);
  }  
}

void nuiAudioConvert_FloatTo32bits(float* pInBuffer, int32* pOutBuffer, int64 SizeToRead)
{
  static const float mult1 = 2147483648.0f ;
  static const float mult2 = 2147483647.0f ;
  
  int64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    float value = nuiClamp(pInBuffer[i], -1.0f, 1.0f);
    if (value < 0 )
      pOutBuffer[i] = ToZero(value * mult1);
    else
      pOutBuffer[i] = ToZero(value * mult2);
  }  
}


void nuiAudioConvert_32bitsToFloat(int32* pInBuffer, float* pOutBuffer, int64 SizeToRead)
{
  static const float mult1 =  1.0 / 2147483648.0f ;
  static const float mult2 = 1.0 / 2147483647.0f ;
  
  int64 i;
  for ( i = 0; i < SizeToRead; i++)
  {
    if (pInBuffer[i] < 0)
      pOutBuffer[i] = pInBuffer[i] * mult1;
    else
      pOutBuffer[i] = pInBuffer[i] * mult2;
  }
}

