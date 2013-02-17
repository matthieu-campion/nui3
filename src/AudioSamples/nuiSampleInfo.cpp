/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
#include "nui.h"

nuiSampleInfo::nuiSampleInfo()
{
  mFileFormat = eAudioUnknown;
  mSampleRate = 0.0;
  mChannels = 0;
  mBitsPerSample = 0;
  mSampleFrames = 0;
  mStartFrame = 0;
  mStopFrame = 0;
  mTempo = 120.0;
  mFormatTag = eWaveFormatUnknown;
  mTimeSignNom = 4;
  mTimeSignDenom = 4;
}

nuiSampleInfo::~nuiSampleInfo()
{
}

/*
FileFormat
 */

nuiAudioFileFormat nuiSampleInfo::GetFileFormat() const
{
  return mFileFormat;
}

void nuiSampleInfo::SetFileFormat(nuiAudioFileFormat FileFormat)
{
  mFileFormat = FileFormat;
}



/*
SampleRate
*/

double nuiSampleInfo::GetSampleRate() const
{
  return mSampleRate;
}

void nuiSampleInfo::SetSampleRate(double SampleRate)
{
  mSampleRate = SampleRate;
}


/*
Channels
*/

int32 nuiSampleInfo::GetChannels() const
{
  return mChannels;
}

void nuiSampleInfo::SetChannels(int32 Channels)
{
  mChannels = Channels;
}


/*
BitsPerSample
*/

int32 nuiSampleInfo::GetBitsPerSample() const
{
  return mBitsPerSample;
}


void nuiSampleInfo::SetBitsPerSample(int32 BPS)
{
  mBitsPerSample = BPS;
}


/*
SampleFrames
*/

int64 nuiSampleInfo::GetSampleFrames() const
{
  return mSampleFrames;
}

void nuiSampleInfo::SetSampleFrames(int64 SampleFrames)
{
  mSampleFrames = SampleFrames;
}


/*
StartFrame
*/

int64 nuiSampleInfo::GetStartFrame() const
{
  return mStartFrame;
}

void nuiSampleInfo::SetStartFrame(int64 StartFrame)
{
  mStartFrame = StartFrame;
}


/*
StopFrame
*/

int64 nuiSampleInfo::GetStopFrame() const
{
  return mStopFrame;
}

void nuiSampleInfo::SetStopFrame(int64 StopFrame)
{
  mStopFrame = StopFrame;
}


/*
Tempo
*/

double nuiSampleInfo::GetTempo() const
{
  return mTempo;
}

void nuiSampleInfo::SetTempo(double Tempo)
{
  mTempo = Tempo;
}

/*
 WaveFormat
 */

uint8 nuiSampleInfo::GetFormatTag() const
{
  return mFormatTag;
}

void nuiSampleInfo::SetFormatTag(uint8 FormatTag)
{
  mFormatTag = FormatTag;
}

/*
TimeSignNom
 */
int8 nuiSampleInfo::GetTimeSignNom() const
{
  return mTimeSignNom;
}

void nuiSampleInfo::SetTimeSignNom(int8 TimeSignNom)
{
  mTimeSignNom = TimeSignNom;
}

/*
 TimeSignDenom
 */
int8 nuiSampleInfo::GetTimeSignDenom() const
{
  return mTimeSignDenom;
}

void nuiSampleInfo::SetTimeSignDenom(int8 TimeSignDenom)
{
  mTimeSignDenom = TimeSignDenom;
}

const std::vector<SliceInfo>& nuiSampleInfo::GetSlices() const
{
  return mSlices;
}

void nuiSampleInfo::SetSlices(const std::vector<SliceInfo>& rSlices)
{
  mSlices = rSlices;
}

void nuiSampleInfo::SetBeats(double Beats)
{
  mBeats = Beats;
}

double nuiSampleInfo::GetBeats() const
{
  return mBeats;
}

