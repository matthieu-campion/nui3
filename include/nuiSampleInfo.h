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
  int32 GetChannels() const;
  int32 GetBitsPerSample() const;
  int64 GetSampleFrames() const;
  int64 GetStartFrame() const;
  int64 GetStopFrame() const;
  double GetTempo() const;
  uint8 GetFormatTag() const;
  int8 GetTimeSignNom() const;
  int8 GetTimeSignDenom() const;
  double GetBeats() const;
  
  void SetFileFormat(nuiAudioFileFormat FileFormat);
  void SetSampleRate(double SampleRate);
  void SetChannels(int32 Channels);
  void SetBitsPerSample(int32 BPS);
  void SetSampleFrames(int64 SampleFrames);
  void SetStartFrame(int64 StartFrame);
  void SetStopFrame(int64 StopFrame);
  void SetTempo(double Tempo);
  void SetFormatTag(uint8 FormatTag);
  void SetTimeSignNom(int8 TimeSignNom);
  void SetTimeSignDenom(int8 TimeSignDenom);
  void SetBeats(double Beats);
  
  //Sizes in Sample Frames
  const std::vector<SliceInfo>& GetSlices() const;
  void SetSlices(const std::vector<SliceInfo>& rSlices);
  
private:
  nuiAudioFileFormat mFileFormat;
  double mSampleRate;
  int32 mChannels;
  int32 mBitsPerSample;
  int64 mSampleFrames;
  int64 mStartFrame;
  int64 mStopFrame;
  double mTempo;
  uint8 mFormatTag;
  int8 mTimeSignNom;
  int8 mTimeSignDenom;
  double mBeats;

  //Sizes in Sample Frames
  std::vector<SliceInfo> mSlices;
  
};
