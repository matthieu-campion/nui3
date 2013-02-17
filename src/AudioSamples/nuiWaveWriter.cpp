/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiChunksDefinitions.h"



nuiWaveWriter::nuiWaveWriter(nglOStream& rStream) : nuiSampleWriter(rStream)
{
  mrStream.SetEndian(eEndianLittle);
  
  mIsInfosWritten = false;
}

nuiWaveWriter::~nuiWaveWriter()
{

}


bool nuiWaveWriter::WriteInfo(const nuiSampleInfo& rInfos)
{
  if ( true == mIsInfosWritten )
    return false;
  
  mrSampleInfo = rInfos;
  mrSampleInfo.SetSampleFrames(0);
  
  if (32 == rInfos.GetBitsPerSample())
  {
    mrSampleInfo.SetFormatTag(eWaveFormatIEEEfloat);
  }
  else // if (16 == rInfos.GetBitsPerSample())
  {
    mrSampleInfo.SetFormatTag(eWaveFormatPcm);
  }
  
  
  int32 Data32;
  int16 Data16;
  
  //Write first chunk ID : "RIFF" (0x52494646)
  if ( 4 != mrStream.WriteUInt8((uint8*)"RIFF",4))
    return false;

  //we can't write file size!!! : write an uint32 to move 4 bytes forward
  uint32 fake = 1;
  if ( 1 != mrStream.WriteUInt32(&fake,1))
    return false;
  //mrStream.SetPos(4, eStreamForward);
  
  //Write RIFF type : "WAVE" (0x57415645)
  if ( 4 != mrStream.WriteUInt8((uint8*)"WAVE",4))
    return false;
  
  //Write second chunk : "fmt " (0x666D7420)
  if ( 4 != mrStream.WriteUInt8((uint8*)"fmt ",4))
    return false;
  
  
  //Write "fmt " chunk size : 16 if format code = PCM
  //                          18 if format code = IEEE Float
  
  switch(mrSampleInfo.GetFormatTag())
  {
    case eWaveFormatPcm :
    {
      Data32 = FMT_CHUNK_DATA_SIZE_PCM;
      if ( 1 != mrStream.WriteInt32(&Data32,1))
        return false;
    }
      break;
      
    case eWaveFormatIEEEfloat :
    {
      Data32 = FMT_CHUNK_DATA_SIZE_IEEE_FLOAT;
      if ( 1 != mrStream.WriteInt32(&Data32,1))
        return false;
    }
      break;
        
    default :
      return false;
      break;
  }
    /*
  Data32 = 16;
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  */
  
  //Write Compression code 
  Data16 = mrSampleInfo.GetFormatTag();
  if ( 1 != mrStream.WriteInt16(&Data16,1))
    return false;
  
  //Write Numbers of channels
  Data16 = rInfos.GetChannels();
  if ( 1 != mrStream.WriteInt16(&Data16,1))
    return false;
  
  //Write Sample Rate
  Data32 = ToNearest(rInfos.GetSampleRate());
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  
  //Write Average bytes per second
  Data32 = ToNearest(rInfos.GetSampleRate()) * rInfos.GetBitsPerSample() / 8 * rInfos.GetChannels();
  if ( 1 != mrStream.WriteInt32(&Data32,1))
    return false;
  
  //Write BlockAlign
  Data16 = rInfos.GetBitsPerSample() / 8 * rInfos.GetChannels();
  if ( 1 != mrStream.WriteInt16(&Data16,1))
    return false;
  
  //Write Bits per Sample
  Data16 = rInfos.GetBitsPerSample();
  if ( 1 != mrStream.WriteInt16(&Data16,1))
    return false;
  
  //
  if (eWaveFormatPcm != mrSampleInfo.GetFormatTag())
  {
    Data16 = 0; //Size of the extension : 0
    if ( 1 != mrStream.WriteInt16(&Data16,1))
      return false;
    
    //Write "fact" Chunk
    /////////////////////////////////
    //Write "data" chunk ID
    if ( 4 != mrStream.WriteUInt8((uint8*)"fact",4))
      return false;
    
    //Write "fact" chunk size
    Data32 = FACT_CHUNK_DATA_SIZE;
    if ( 1 != mrStream.WriteInt32(&Data32,1))
      return false;
    
    //We can't write Number of Channels * Number of Frames
    //Go Position + 4 : write an uint32 to move 4 bytes forward
    if ( 1 != mrStream.WriteUInt32(&fake,1))
      return false;
  }
  
  //Write "data" chunk ID
  if ( 4 != mrStream.WriteUInt8((uint8*)"data",4))
    return false;
  
  ////we can't write "data" chunk size!!! : write an uint32 to move 4 bytes forward
  if ( 1 != mrStream.WriteUInt32(&fake,1))
    return false;
  
  return true;
}




/*
Finalize
*/
////////////////////////////////////////////////////////////////////
//Write wave data size and file size in Stream 
//Finalize() is called after WriteInfo call and all Write calls
////////////////////////////////////////////////////////////////////

#define CUE_CHUNK_DATA_SIZE 4

bool nuiWaveWriter::Finalize()
{
  // write cue chunk
  if(mrSampleInfo.GetSlices().size() > 0)
  { 
    const std::vector<SliceInfo> slices = mrSampleInfo.GetSlices();
    if ( 4 != mrStream.WriteUInt8((uint8*)"cue ",4))
      return false;  
    
    int32 numCues = slices.size();  
    int32 cueChunkSize = CUE_CHUNK_DATA_SIZE + (24 * numCues);
    
    if ( 1 != mrStream.WriteInt32(&cueChunkSize,1))
      return false;  
    
    if ( 1 != mrStream.WriteInt32(&numCues,1))
      return false; 
    
    int32 data32;
    
    for(int32 i=0; i<numCues; i++)
    {
      // cue ID
      if ( 1 != mrStream.WriteInt32(&i,1))
        return false;    
      
      // play order position..
      data32 = (int32)slices[i].mStartFrame;
      if ( 1 != mrStream.WriteInt32(&data32,1))
        return false;
      
      if ( 4 != mrStream.WriteUInt8((uint8*)"data",4))
        return false;
      
      data32 = (int32)slices[i].mStartFrame * mrSampleInfo.GetChannels() * (mrSampleInfo.GetBitsPerSample() / 8);
      if ( 1 != mrStream.WriteInt32(&data32,1))
        return false; 
      
      
      data32 = (int32)slices[i].mStartFrame * mrSampleInfo.GetChannels() * (mrSampleInfo.GetBitsPerSample() / 8);
      if ( 1 != mrStream.WriteInt32(&data32,1))
        return false;
      
      data32 = 0;
      if ( 1 != mrStream.WriteInt32(&data32,1))
        return false;   
    }
  }
  
  
  int32 Size = (int32)mrSampleInfo.GetSampleFrames() * mrSampleInfo.GetChannels();
  
  //position of datas depends on wave format
  
  //Go to "data" Chunk Data Size position in stream
  if (eWaveFormatPcm == mrSampleInfo.GetFormatTag())
  {
    mrStream.SetPos( CHUNK_HEADER_SIZE + RIFF_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + FMT_CHUNK_DATA_SIZE_PCM + 4, eStreamFromStart);
  }
  
  else if (eWaveFormatIEEEfloat == mrSampleInfo.GetFormatTag())
  {
    //Go to "fact" chunk data position
    mrStream.SetPos( CHUNK_HEADER_SIZE + RIFF_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + FMT_CHUNK_DATA_SIZE_IEEE_FLOAT + CHUNK_HEADER_SIZE , eStreamFromStart);
    
    //Write "fact" datas : Number of frames * number of channels
    if ( 1 != mrStream.WriteInt32(&Size,1))
      return false;
    
    //Go to "data" chunk position
    mrStream.SetPos( CHUNK_HEADER_SIZE + RIFF_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + FMT_CHUNK_DATA_SIZE_IEEE_FLOAT + CHUNK_HEADER_SIZE + FACT_CHUNK_DATA_SIZE + 4, eStreamFromStart);
  }
  else
    return false;
  
  //Write wave data Size
  Size = (int32)mrSampleInfo.GetSampleFrames() * mrSampleInfo.GetBitsPerSample() / 8 * mrSampleInfo.GetChannels();
  
  if ( 1 != mrStream.WriteInt32(&Size,1))
    return false;
  
  // add chunk size.. 
  if(mrSampleInfo.GetSlices().size() > 0)
  {
    Size += CUE_CHUNK_DATA_SIZE;
    Size += mrSampleInfo.GetSlices().size() * 24;
  }
  
  //Write file data Size
  if (eWaveFormatPcm == mrSampleInfo.GetFormatTag())
    Size += RIFF_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + FMT_CHUNK_DATA_SIZE_PCM + CHUNK_HEADER_SIZE;
  
  else if (eWaveFormatIEEEfloat == mrSampleInfo.GetFormatTag())
    Size += RIFF_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE + FMT_CHUNK_DATA_SIZE_IEEE_FLOAT + CHUNK_HEADER_SIZE + FACT_CHUNK_DATA_SIZE + CHUNK_HEADER_SIZE;
  else
    return false;
  
  //Write

  //Go to "RIFF" Chunk Data Size position in stream
  mrStream.SetPos(4,eStreamFromStart);
  
  if ( 1 != mrStream.WriteInt32(&Size,1))
    return false;
  
  return true;
}


int32 nuiWaveWriter::Write(const void* pBuffer, int32 SampleFrames, nuiSampleBitFormat format)
{
  //add size to read To size already read
  int64 NbFrames = mrSampleInfo.GetSampleFrames() + SampleFrames;
  mrSampleInfo.SetSampleFrames( NbFrames );
  
  const int32 SamplePointsToWrite = SampleFrames * mrSampleInfo.GetChannels();
  int32 SampleFramesWritten = 0;
  
  switch(format)
  {
    case eSampleInt16 :
    {
      switch ( mrSampleInfo.GetBitsPerSample() )
      {
        case 16 :
        {
          SampleFramesWritten = (int32)mrStream.WriteInt16( (int16*)pBuffer, SamplePointsToWrite) / mrSampleInfo.GetChannels();
        }
          break;
          
        default:
          SampleFramesWritten = 0;
          break;
      }
    }
      break;
      
    case eSampleFloat32 :
    {
      switch ( mrSampleInfo.GetBitsPerSample() )
      {
        case 16 :
        {
          float* pTempFloat = (float*)pBuffer;
          
          int16* pInt16Buffer = new int16[SamplePointsToWrite];
          nuiAudioConvert_FloatBufferTo16bits(pTempFloat, pInt16Buffer, SamplePointsToWrite);
          
          SampleFramesWritten = (int32)mrStream.WriteInt16(pInt16Buffer, SamplePointsToWrite) / mrSampleInfo.GetChannels();
        }
          break;
          
        case 24 :
        {
          float* pTempFloat = (float*)pBuffer;
          
          std::vector<uint8> TempBuffer;
          TempBuffer.resize(SamplePointsToWrite * 3); //nb of sample points * 3 bytes (24 bits) = nb of bytes to read
          
          nuiAudioConvert_FloatTo24bitsLittleEndian(pTempFloat,&TempBuffer[0],SamplePointsToWrite);
          
          SampleFramesWritten = (int32)mrStream.WriteUInt8(&TempBuffer[0], SamplePointsToWrite * 3) / (3 * mrSampleInfo.GetChannels());
        }
          break;
          
        case 32 :
        {
          float* pTempFloat = (float*)pBuffer;
          SampleFramesWritten = (int32)mrStream.WriteFloat(pTempFloat, SamplePointsToWrite) / mrSampleInfo.GetChannels();
        }
          break;
          
        default:
          SampleFramesWritten = 0;
          break;
      }
    }
      break;
      
    default :
      SampleFramesWritten = 0;
      break;
  }
  
  
  return SampleFramesWritten;
}


