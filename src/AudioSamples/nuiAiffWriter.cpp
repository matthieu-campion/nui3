/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiChunksDefinitions.h"

nuiAiffWriter::nuiAiffWriter(nglOStream& rStream) : 
  nuiSampleWriter(rStream),
  mInfoWritten(false)
{
  mrStream.SetEndian(eEndianBig);
}

nuiAiffWriter::~nuiAiffWriter()
{
  
}

bool nuiAiffWriter::WriteInfo(const nuiSampleInfo& rInfos)
{
  if ( true == mInfoWritten )
    return false;
  
  int32 Data32;
  int16 Data16;
  
  mrSampleInfo = rInfos;
  
  mrSampleInfo.SetSampleFrames(0);
  
  //Write first chunk ID : "FORM" 
  if ( 4 != mrStream.WriteUInt8((uint8*)"FORM",4))
    return false;
  
  //we can't write file size!!!
  mrStream.SetPos(4, eStreamForward);
  
  //Write File type : "AIFF"
  if ( 4 != mrStream.WriteUInt8((uint8*)"AIFF",4))
    return false;
  
  //Write second chunk : "COMM"
  if ( 4 != mrStream.WriteUInt8((uint8*)"COMM ",4))
    return false;
  
  
  //Write "COMM" chunk size : 18  
  Data32 = 18;
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  
  
  //Write Number of Channels
  Data16 = rInfos.GetChannels();
  if ( 1 != mrStream.WriteInt16(&Data16,1))
    return false;
  
  //we can't write Number of Sample frames!!!
  mrStream.SetPos(4, eStreamForward);
  
  //Write Number of bits per Sample
  Data16 = rInfos.GetBitsPerSample();
  if ( 1 != mrStream.WriteInt16(&Data16,1))
    return false;
  
  //
  //Write Sample rate
  //
  extended SampleRate = DoubleToExtended(rInfos.GetSampleRate());
  
  //Write exponent
  if ( 1 != mrStream.WriteInt16((int16*)(SampleRate.exp),1))
    return false;
  //Write mantisse
  if ( 4 != mrStream.WriteInt16((int16*)(SampleRate.man),4))
    return false;
  
  //#fixme  
  //Write "SSND" chunk ID
  if ( 4 != mrStream.WriteUInt8((uint8*)"SSND",4))
    return false;
  
  ////we can't write "SSND" chunk size!!!
  mrStream.SetPos(4, eStreamForward);
  
  Data32 = 0;
  //Write offset (unused here)
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  
  //Write blockSize (unused here)
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  
  mInfoWritten = true;
  return true;
}

bool nuiAiffWriter::Finalize()
{
  int32 Size = mrSampleInfo.GetSampleFrames() * mrSampleInfo.GetChannels();
  
  
  //position of datas depends on wave format
  
  //Go to "SSND" Chunk Data Size position in stream
  mrStream.SetPos( CHUNK_HEADER_SIZE + FORM_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + COMM_CHUNK_DATA_SIZE + 4, eStreamFromStart);
  
  //Write wave data Size
  Size = mrSampleInfo.GetSampleFrames() * mrSampleInfo.GetBitsPerSample() / 8 * mrSampleInfo.GetChannels();
  
  if ( 1 != mrStream.WriteInt32(&Size,1))
    return false;
  
  //Go to "FORM" Chunk Data Size position in stream
  mrStream.SetPos(4,eStreamFromStart);
  
  //Write file data Size
  Size += FORM_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + COMM_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE;
  
  //Write
  if ( 1 != mrStream.WriteInt32(&Size,1))
    return false;
  
  
  //
  //Write Number of Sample frames in "COMM" chunk
  //
  //Go to Sample Frames data location in "COMM" Chunk
  mrStream.SetPos( CHUNK_HEADER_SIZE + FORM_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + 2, eStreamFromStart); // "+2" -> to go after numChannels
  //
  //Write Number of Sample frames
  int32 Data32 = mrSampleInfo.GetSampleFrames();
  
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  
  return true;
}



int32 nuiAiffWriter::Write(const void* pBuffer, int32 SampleFrames, nuiSampleBitFormat format)
{
  int32 channels = mrSampleInfo.GetChannels();
  const int64 NbSamplePointsToRead = SampleFrames * mrSampleInfo.GetChannels();
  int32 SampleFramesRead = 0;
  
  switch(format)
  {
    case eSampleInt16 :
    {
      switch ( mrSampleInfo.GetBitsPerSample() )
      {
        case 16 :
        {
          SampleFramesRead = mrStream.WriteInt16( (int16*)pBuffer, NbSamplePointsToRead) / channels;
        }
          break;
          
        default:
          return false;
          break;
      }
    }
      break;
      
    case eSampleFloat32 :
    {
      switch ( mrSampleInfo.GetBitsPerSample() )
      {
        case 24 :
        {
          float* pTempFloat = (float*)pBuffer;
          
          std::vector<uint8> TempBuffer;
          TempBuffer.resize(NbSamplePointsToRead * 3); //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
          
          nuiAudioConvert_FloatTo24bitsLittleEndian(pTempFloat, &TempBuffer[0], NbSamplePointsToRead);
          
          SampleFramesRead = mrStream.WriteUInt8(&TempBuffer[0], NbSamplePointsToRead * 3) / (channels * 3);
        }
          break;
          
        case 32 :
        {
          float* pTempFloat = (float*)pBuffer;
          
          std::vector<int32> TempBuffer;
          TempBuffer.resize(NbSamplePointsToRead); //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
          
          nuiAudioConvert_FloatTo32bits(pTempFloat, &TempBuffer[0],NbSamplePointsToRead);
          
          SampleFramesRead = mrStream.WriteInt32(&TempBuffer[0], NbSamplePointsToRead) / channels;
        }
          break;
          
        default:
          break;
      }
    }
      break;
      
    default :
      break;
  }
  
  //add size read To size already read
  int64 NbFrames = mrSampleInfo.GetSampleFrames() + SampleFramesRead;
  mrSampleInfo.SetSampleFrames(NbFrames);
  return SampleFramesRead;
}

