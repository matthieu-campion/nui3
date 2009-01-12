/*
 *  DecoderTest.cpp
 *  nuiAudioDecoderTest
 *
 *  Created by mat on 1/5/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "DecoderTest.h"
#include "nuiAudioDecoder.h"
#include "nuiWaveWriter.h"
#include "nuiWaveReader.h"

#define FRAMES_PACKET 20000


bool ConvertCompressedAudioFile(nglPath inPath, nglPath outPath)
{
  if (!inPath.Exists())
    return false;
  
  bool res = false;
  
  //INPUT
  nglIFile inFile(inPath);
  nuiAudioDecoder* pDecoder = new nuiAudioDecoder(inFile);
  
  //OUTPUT
  nglOFile outFile(outPath, eOFileCreate);
  nuiWaveWriter* pWriter = new nuiWaveWriter(outFile);
  
  nuiSampleInfo inInfo;
  nuiSampleInfo outInfo;
  
  if (pDecoder->GetInfo(inInfo))
  {
    NGL_OUT(_T("********************************************************\Audio infos:\n"));
    PrintAudioInfos(inInfo);
    
    uint32 channels = inInfo.GetChannels();
    std::vector<float*> buffers(channels);
    for (uint32 c = 0; c < channels; c++)
    {
      buffers[c] = new float[FRAMES_PACKET];
    }
	float* pDeInterleavedBuffer = new float[channels * FRAMES_PACKET];
    
    //write output samples infos
    outInfo.SetChannels(inInfo.GetChannels());
    outInfo.SetSampleRate(inInfo.GetSampleRate());
    outInfo.SetBitsPerSample(inInfo.GetBitsPerSample());
    outInfo.SetFileFormat(eAudioWave);
    outInfo.SetFormatTag(eWaveFormatPcm);
    pWriter->WriteInfo(outInfo);
    
    uint32 FramesToRead = FRAMES_PACKET;
    uint32 FramesRead = 0;
    uint32 FramesWritten = 0;
    uint32 TotalFramesRead = 0;
    uint32 TotalFramesWritten = 0;
    do
    {
      FramesRead = pDecoder->Read(buffers, FramesToRead);
      
      //deinterleave samples
      for (uint32 s = 0; s < FramesRead; s++)
      {
        for (uint32 c = 0; c < channels; c++)
        {
          pDeInterleavedBuffer[s * channels + c] = buffers[c][s];
        }
      }
      
      FramesWritten = pWriter->Write(pDeInterleavedBuffer, FramesRead, eSampleFloat32);
      
      TotalFramesRead += FramesRead;
      TotalFramesWritten += FramesWritten;
    }
    while (FramesRead);
    
    NGL_OUT(_T("SUCCESS: FILE DECOMPRESSED\n"));
    NGL_OUT(_T("%ld samples read and %ld samples written\n"), TotalFramesRead, TotalFramesWritten);
    
    //finalize wave file writing
    if (!pWriter->Finalize())
      NGL_OUT(_T("nuiWaveWriter: Failed to finalize!!!!!!!\n"));
      
    //deallocate pointers
    delete[] pDeInterleavedBuffer;
    for (uint32 c = 0; c < channels; c++)
    {
      delete[] buffers[c];
    }
    res = true;
  }
  else
  {
    NGL_OUT(_T("nuiAudioDecoder: Failed to read Sample Info!!!!!!!\n"));
  }
  
  
  delete pDecoder;
  return res;
}


void PrintAudioInfos(nuiSampleInfo info)
{
  NGL_OUT(_T("*************************************\n"));
  NGL_OUT(_T("SAMPLE INFOS\n"));
  NGL_OUT(_T("nb of channels = %ld\n"), info.GetChannels());
  NGL_OUT(_T("sample rate = %lf\n"), info.GetSampleRate());
  NGL_OUT(_T("nb of bits per sample = %ld\n"), info.GetBitsPerSample());
  NGL_OUT(_T("nb of sample frames = %lld\n"), info.GetSampleFrames());
  NGL_OUT(_T("*************************************\n\n"));
}

bool TestSeek(nglPath inPath, nglPath outPath)
{
  if (!inPath.Exists())
    return false;

  NGL_OUT(_T("TEST SEEK\n"));
  
  bool res = false;
  
  //INPUT
  nglIFile inFile(inPath);
  nuiAudioDecoder* pDecoder = new nuiAudioDecoder(inFile);
  
  //OUTPUT
  nglOFile outFile(outPath, eOFileCreate);
  nuiWaveWriter* pWriter = new nuiWaveWriter(outFile);
  
  nuiSampleInfo inInfo;
  nuiSampleInfo outInfo;
  
  if (pDecoder->GetInfo(inInfo))
  {
    NGL_OUT(_T("********************************************************\Audio infos:\n"));
    PrintAudioInfos(inInfo);
    
    uint32 channels = inInfo.GetChannels();
    std::vector<float*> buffers(channels);
    for (uint32 c = 0; c < channels; c++)
    {
      buffers[c] = new float[FRAMES_PACKET];
    }
    float* pDeInterleavedBuffer = new float[channels * FRAMES_PACKET];
    
    //write output samples infos
    outInfo.SetChannels(inInfo.GetChannels());
    outInfo.SetSampleRate(inInfo.GetSampleRate());
    outInfo.SetBitsPerSample(inInfo.GetBitsPerSample());
    outInfo.SetFileFormat(eAudioWave);
    outInfo.SetFormatTag(eWaveFormatPcm);
    pWriter->WriteInfo(outInfo);
    
    uint32 FramesToRead = FRAMES_PACKET;
    uint32 FramesRead = 0;
    uint32 FramesWritten = 0;
    uint32 TotalFramesRead = 0;
    uint32 TotalFramesWritten = 0;

    for (uint32 i= 0; i < 5; i++)
    {
      FramesRead = pDecoder->Read(buffers, FramesToRead);
      
      //deinterleave samples
      for (uint32 s = 0; s < FramesRead; s++)
      {
        for (uint32 c = 0; c < channels; c++)
        {
          pDeInterleavedBuffer[s * channels + c] = buffers[c][s];
        }
      }
      
      FramesWritten = pWriter->Write(pDeInterleavedBuffer, FramesRead, eSampleFloat32);
      
      TotalFramesRead += FramesRead;
      TotalFramesWritten += FramesWritten;
    }
    
    pDecoder->SetPosition(inInfo.GetSampleRate() * 10); // try to seek to 10 sec from beginning
    
    for (uint32 i= 0; i < 5; i++)
    {
      FramesRead = pDecoder->Read(buffers, FramesToRead);
      
      //deinterleave samples
      for (uint32 s = 0; s < FramesRead; s++)
      {
        for (uint32 c = 0; c < channels; c++)
        {
          pDeInterleavedBuffer[s * channels + c] = buffers[c][s];
        }
      }
      
      FramesWritten = pWriter->Write(pDeInterleavedBuffer, FramesRead, eSampleFloat32);
      
      TotalFramesRead += FramesRead;
      TotalFramesWritten += FramesWritten;
    }
      
    
    NGL_OUT(_T("SUCCESS: FILE DECOMPRESSED\n"));
    NGL_OUT(_T("%ld samples read and %ld samples written\n"), TotalFramesRead, TotalFramesWritten);
    
    //finalize wave file writing
    if (!pWriter->Finalize())
      NGL_OUT(_T("nuiWaveWriter: Failed to finalize!!!!!!!\n"));
    
    //deallocate pointers
    delete[] pDeInterleavedBuffer;
    for (uint32 c = 0; c < channels; c++)
    {
      delete[] buffers[c];
    }
    res = true;
  }
  else
  {
    NGL_OUT(_T("nuiAudioDecoder: Failed to read Sample Info!!!!!!!\n"));
  }
  
  
  delete pDecoder;
  return res;
}






