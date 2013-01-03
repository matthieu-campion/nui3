/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

void nuiAudioConvert_INint16ToDEfloat(const int16* input, float* output, int32 curChannel, int32 nbChannels, int32 nbSampleFrames); // interlaced int16 to de-interlaced float

void nuiAudioConvert_DEfloatToINint16(const float* input, int16* output, int32 curChannel, int32 nbChannels, int32 nbSampleFrames); // de-interlaced float to interlaced int16

//
void nuiAudioConvert_Unsigned8bitsBufferTo16Bits(int16* pBuffer, int64 SizeToRead);  ///< Convert a buffer of unsigned 8 bits numbers in a buffer of signed 16 bits int numbers \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
void nuiAudioConvert_Unsigned8bitsBufferToFloat(float* pBuffer, int64 SizeToRead);  ///< Convert a buffer of unsigned 8 bits numbers in a buffer of float numbers (32 bits) \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
//
void nuiAudioConvert_Signed8bitsBufferTo16Bits(int16* pBuffer, int64 SizeToRead); ///< Convert a buffer of signed 8 bits numbers in a buffer of signed 16 bits int numbers \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
void nuiAudioConvert_Signed8bitsBufferToFloat(float* pBuffer, int64 SizeToRead);  ///< Convert a buffer of signed 8 bits numbers in a buffer of float numbers (32 bits) \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
//
void nuiAudioConvert_16bitsBufferToFloat(float* pBuffer, int64 SizeToRead); ///< Convert a buffer of signed 16 bits int numbers in a buffer of float numbers (32 bits) \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
void nuiAudioConvert_FloatBufferTo16bits(float* pFloatBuffer, int16* pInt16Buffer, int64 SizeToRead);
//
float nuiAudioConvert_24bitsToFloatFromLittleEndian(uint8* pBytesBuf); ///< Convert 24 bits int Little Endian number to float number (32 bits) \param pBytesBuf pointer to a buffer of 3 unsigned 8 bits int which represent the 24 bit int number to convert \return The float converted number
float nuiAudioConvert_24bitsToFloatFromBigEndian(uint8* pBytesBuf);  ///< Convert 24 bits int Big Endian number to float number (32 bits) \param pBytesBuf pointer to a buffer of 3 unsigned 8 bits int which represent the 24 bit int number to convert \return The float converted number
//
void nuiAudioConvert_FloatTo24bitsLittleEndian(float* pInBuffer, uint8* pOutBuffer, int64 SizeToRead);  ///< Convert a float (32 bits) numbers buffer to a 24 bits Little Endian intnumbers buffer \param pInBuffer Pointer to a buffer that contains float numbers to convert \param pOutBuffer Pointer to a buffer that receives converted 24 bits int numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
void nuiAudioConvert_FloatTo24bitsBigEndian(float* pInBuffer, uint8* pOutBuffer, int64 SizeToRead); ///< Convert a float (32 bits) numbers buffer to a 24 bits Big Endian int numbers buffer \param pInBuffer Pointer to a buffer that contains float numbers to convert \param pOutBuffer Pointer to a buffer that receives converted 24 bits int numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
//
void nuiAudioConvert_FloatTo32bits(float* pInBuffer, int32* pOutBuffer, int64 SizeToRead);  ///< Convert a float (32 bits) numbers buffer to a 32 bits intnumbers buffer \param pInBuffer Pointer to a buffer that contains float numbers to convert \param pOutBuffer Pointer to a buffer that receives converted 32 bits int numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
//
void nuiAudioConvert_32bitsToFloat(int32* pInBuffer, float* pOutBuffer, int64 SizeToRead);  ///< Convert a 32 bits int numbers buffer to a float (32 bits) numbers buffer \param pInBuffer Pointer to a buffer that contains 32 bits int numbers to convert \param pOutBuffer Pointer to a buffer that receives converted float numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
//

