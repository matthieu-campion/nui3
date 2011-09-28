/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiInit.h"

#include "nglConsole.h"
#include "nuiAudioDecoder.h"

int main(int argc, const char** argv)
{
  nuiInit(NULL);
  
  printf("=================================\nnuiAudioDecoder tutorial\n");
  
  if (argc < 2)
  {
    printf("usage:\n%s <path to mp3 file>\n", argv[0]);
    return -1;
  }  

  nglPath p(argv[1]);
  nglIStream* pStream = p.OpenRead();
  
  if (!pStream)
  {
    printf("Unable to open file '%s'\n", p.GetChars());
    return -2;
  }
 
  nuiAudioDecoder decoder(*pStream);
  
  nuiSampleInfo info;
  
  nglTime start;
  decoder.GetInfo(info);
  nglTime stop;
  
  printf("nuiAudioDecoder::GetInfo() took %f seconds\n", (double)stop - (double)start);
  
  //nuiAudioFileFormat GetFileFormat() const;  
  double sr = info.GetSampleRate();
  uint32 channels = info.GetChannels();
  uint32 bps = info.GetBitsPerSample();
  uint64 sampleframes = info.GetSampleFrames();
  uint64 startframe = info.GetStartFrame();
  uint64 stopframe = info.GetStopFrame();
  double tempo = info.GetTempo();
  uint8 tag = info.GetFormatTag();
  uint8 tsn = info.GetTimeSignNom();
  uint8 tsd = info.GetTimeSignDenom();
  double beats = info.GetBeats();

  printf("Sample rate     : %f\n", sr);
  printf("Channels        : %d\n", channels);
  printf("bits per sample : %d\n", bps);
  printf("Sample frames   : %d\n", sampleframes);
  printf("Start frame     : %d\n", startframe);
  printf("Stop frame      : %d\n", stopframe);
  printf("Tempo           : %f\n", tempo);
  printf("Tag             : %d\n", tag);
  printf("Time sig num    : %d\n", tsn);
  printf("Time sig denum  : %d\n", tsd);
  printf("Beats           : %f\n", beats);
  
  printf("\n\nDecoding:\n\n");
 
  uint32 frames = 4096;
  float buffer[frames * sizeof(float) * channels];
  uint32 done = 0;
  uint32 r = 0;
  {
    nglTime start;
    do
    {
      r = decoder.ReadIN(buffer, frames, eSampleFloat32);
      //printf("%d (%d)\n", done, r);
      printf(".");
      done += r;
    } 
    while (done < sampleframes && r == 4096);
    nglTime stop;
    printf("\nDone in %f seconds (%d samples read)\n", done, (double)stop - (double)start);
  }


  delete pStream;

  nuiUninit();
  
  return 0;
}
