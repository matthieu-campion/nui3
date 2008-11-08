/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
#include "nui.h"
#include "nuiSampleInfo.h"

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

uint32 nuiSampleInfo::GetChannels() const
{
  return mChannels;
}

void nuiSampleInfo::SetChannels(uint32 Channels)
{
  mChannels = Channels;
}


/*
BitsPerSample
*/

uint32 nuiSampleInfo::GetBitsPerSample() const
{
  return mBitsPerSample;
}


void nuiSampleInfo::SetBitsPerSample(uint32 BPS)
{
  mBitsPerSample = BPS;
}


/*
SampleFrames
*/

uint64 nuiSampleInfo::GetSampleFrames() const
{
  return mSampleFrames;
}

void nuiSampleInfo::SetSampleFrames(uint64 SampleFrames)
{
  mSampleFrames = SampleFrames;
}


/*
StartFrame
*/

uint64 nuiSampleInfo::GetStartFrame() const
{
  return mStartFrame;
}

void nuiSampleInfo::SetStartFrame(uint64 StartFrame)
{
  mStartFrame = StartFrame;
}


/*
StopFrame
*/

uint64 nuiSampleInfo::GetStopFrame() const
{
  return mStopFrame;
}

void nuiSampleInfo::SetStopFrame(uint64 StopFrame)
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
uint8 nuiSampleInfo::GetTimeSignNom() const
{
  return mTimeSignNom;
}

void nuiSampleInfo::SetTimeSignNom(uint8 TimeSignNom)
{
  mTimeSignNom = TimeSignNom;
}

/*
 TimeSignDenom
 */
uint8 nuiSampleInfo::GetTimeSignDenom() const
{
  return mTimeSignDenom;
}

void nuiSampleInfo::SetTimeSignDenom(uint8 TimeSignDenom)
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

