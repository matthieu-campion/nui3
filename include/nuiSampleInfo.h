/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
#pragma once

#include "nui.h"
#include "nuiSampleDefines.h"


class SliceInfo
{
public:
  uint64 mStartFrame;
  uint64 mSampleFrames;
  double mBeat;
};

/// Class that Contains informations on a Sound File
class nuiSampleInfo
{
public:
  nuiSampleInfo(); 
  ~nuiSampleInfo();
  
  nuiAudioFileFormat GetFileFormat() const;  
  double GetSampleRate() const;
  uint32 GetChannels() const;
  uint32 GetBitsPerSample() const;
  uint64 GetSampleFrames() const;
  uint64 GetStartFrame() const;
  uint64 GetStopFrame() const;
  double GetTempo() const;
  uint8 GetFormatTag() const;
  uint8 GetTimeSignNom() const;
  uint8 GetTimeSignDenom() const;
  double GetBeats() const;
  
  void SetFileFormat(nuiAudioFileFormat FileFormat);
  void SetSampleRate(double SampleRate);
  void SetChannels(uint32 Channels);
  void SetBitsPerSample(uint32 BPS);
  void SetSampleFrames(uint64 SampleFrames);
  void SetStartFrame(uint64 StartFrame);
  void SetStopFrame(uint64 StopFrame);
  void SetTempo(double Tempo);
  void SetFormatTag(uint8 FormatTag);
  void SetTimeSignNom(uint8 TimeSignNom);
  void SetTimeSignDenom(uint8 TimeSignDenom);
  void SetBeats(double Beats);
  
  //Sizes in Sample Frames
  const std::vector<SliceInfo>& GetSlices() const;
  void SetSlices(const std::vector<SliceInfo>& rSlices);
  
private:
  nuiAudioFileFormat mFileFormat;
  double mSampleRate;
  uint32 mChannels;
  uint32 mBitsPerSample;
  uint64 mSampleFrames;
  uint64 mStartFrame;
  uint64 mStopFrame;
  double mTempo;
  uint8 mFormatTag;
  uint8 mTimeSignNom;
  uint8 mTimeSignDenom;
  double mBeats;

  //Sizes in Sample Frames
  std::vector<SliceInfo> mSlices;
  
};
