/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
 #include "nui.h"
 #include "nuiSampleReader.h"
 
//
void ConvertUnsigned8bitsBufferTo16Bits(int16* pBuffer, uint64 SizeToRead);  ///< Convert a buffer of unsigned 8 bits numbers in a buffer of signed 16 bits int numbers \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
void ConvertUnsigned8bitsBufferToFloat(float* pBuffer, uint64 SizeToRead);  ///< Convert a buffer of unsigned 8 bits numbers in a buffer of float numbers (32 bits) \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
//
void ConvertSigned8bitsBufferTo16Bits(int16* pBuffer, uint64 SizeToRead); ///< Convert a buffer of signed 8 bits numbers in a buffer of signed 16 bits int numbers \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
void ConvertSigned8bitsBufferToFloat(float* pBuffer, uint64 SizeToRead);  ///< Convert a buffer of signed 8 bits numbers in a buffer of float numbers (32 bits) \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
//
void Convert16bitsBufferToFloat(float* pBuffer, uint64 SizeToRead); ///< Convert a buffer of signed 16 bits int numbers in a buffer of float numbers (32 bits) \param pBuffer Buffer that gives Input and receives Output numbers \param SizeToRead Number of elements to convert
//
float Convert24bitsToFloatFromLittleEndian(uint8* pBytesBuf); ///< Convert 24 bits int Little Endian number to float number (32 bits) \param pBytesBuf pointer to a buffer of 3 unsigned 8 bits int which represent the 24 bit int number to convert \return The float converted number
float Convert24bitsToFloatFromBigEndian(uint8* pBytesBuf);  ///< Convert 24 bits int Big Endian number to float number (32 bits) \param pBytesBuf pointer to a buffer of 3 unsigned 8 bits int which represent the 24 bit int number to convert \return The float converted number
//
void ConvertFloatTo24bitsLittleEndian(float* pInBuffer, uint8* pOutBuffer, uint64 SizeToRead);  ///< Convert a float (32 bits) numbers buffer to a 24 bits Little Endian intnumbers buffer \param pInBuffer Pointer to a buffer that contains float numbers to convert \param pOutBuffer Pointer to a buffer that receives converted 24 bits int numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
void ConvertFloatTo24bitsBigEndian(float* pInBuffer, uint8* pOutBuffer, uint64 SizeToRead); ///< Convert a float (32 bits) numbers buffer to a 24 bits Big Endian int numbers buffer \param pInBuffer Pointer to a buffer that contains float numbers to convert \param pOutBuffer Pointer to a buffer that receives converted 24 bits int numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
//
void ConvertFloatTo32bits(float* pInBuffer, int32* pOutBuffer, uint64 SizeToRead);  ///< Convert a float (32 bits) numbers buffer to a 32 bits intnumbers buffer \param pInBuffer Pointer to a buffer that contains float numbers to convert \param pOutBuffer Pointer to a buffer that receives converted 32 bits int numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
//
void Convert32bitsToFloat(int32* pInBuffer, float* pOutBuffer, uint64 SizeToRead);  ///< Convert a 32 bits int numbers buffer to a float (32 bits) numbers buffer \param pInBuffer Pointer to a buffer that contains 32 bits int numbers to convert \param pOutBuffer Pointer to a buffer that receives converted float numbers /param SizeToRead Number of numbers to convert from pInBuffer to pOutBuffer
//


//
//CompareID
//
bool CompareID(std::vector<char> ID,  char* IDname); ///< function that compare 2 IDs differently formatted \param ID an ID represented by a vector of char \param IDname ID represented by a pointer to a char buffer \return True if characters of the 2 IDs are the same


#pragma once




/// Class that reads an Wave Sound File
class nuiWaveReader : public nuiSampleReader
{
public:
  nuiWaveReader(nglIStream& rStream); ///< nuiWaveReader Constructor \param rStream The nglIStream in which we read
  nuiWaveReader(const nuiSampleReader& rReader, nglIStream& rStream);
  virtual ~nuiWaveReader();
  virtual nuiSampleReader* Clone(nglIStream& rStream) const;
  virtual bool ReadInfo(nuiSampleInfo& rInfos);  ///< Method that reads Infos of the Wave Sound \param rInfos Variable that receives Sample infos read in stream \return True if method succeeded
  virtual uint32 Read(void* pBuffer, uint32 SampleFrames, nuiSampleBitFormat format);  ///< Method that reads Wave samples from stream \param pBuffer Pointer to a buffer that retrieves samples \param SampleFrames Number of sample frames to read \param SampleBitFormat Wanted format of samples in pBuffer \return True if method succeeded
    
private:
};

