/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiSample::nuiSample(const nuiSampleInfo& rInfos)
{
  mInfos = rInfos;
  
  mSamples.resize(mInfos.GetChannels());
  
  int32 i;
  for (i = 0; i < mSamples.size(); i++)
  {
    mSamples[i].resize(mInfos.GetSampleFrames());
  }
}

 

nuiSample::nuiSample(nuiSampleReader& rReader)
{
  rReader.GetInfo(mInfos);
  
  int32 Channels = mInfos.GetChannels();
  int32 SampleFrames = (int32)mInfos.GetSampleFrames();
  
  mSamples.resize(Channels);
  
  float* TempBuffer = new float[SampleFrames*Channels];
  rReader.SetPosition(0);
  rReader.ReadIN(TempBuffer,SampleFrames,eSampleFloat32);
  
  int32 c;
  int32 s;
  for (c = 0; c < Channels; c++)
  {
    mSamples[c].resize(SampleFrames);
 
    for (s = 0; s < SampleFrames; s++)
    {
      mSamples[c][s] = TempBuffer[Channels * s + c];
    }
  }
  delete TempBuffer;
}


void nuiSample::Reset()
{
  mSamples.clear();
}



nuiSample::~nuiSample()
{
}

//
//Save
//
bool nuiSample::Save(nuiSampleWriter& rWriter, int32 BitsPerSample)
{
  //Write Infos
  nuiSampleInfo MyInfo = mInfos;
  MyInfo.SetBitsPerSample(BitsPerSample);
  MyInfo.SetFileFormat(eAudioWave);
  MyInfo.SetFormatTag(eWaveFormatPcm);
  if (false == rWriter.WriteInfo(MyInfo))
    return false;
  
  
  int32 Channels = mInfos.GetChannels();
  int32 SampleFrames = (int32)mInfos.GetSampleFrames();
  
  //Datas Buffer
  float* TempBuffer = new float[SampleFrames * Channels];
  //
  int32 c;
  int32 s;
  for (c = 0; c < Channels; c++)
  {
    for (s = 0; s < SampleFrames; s++)
    {
      TempBuffer[Channels * s + c] = mSamples[c][s];
    }
  }
  //Write Datas Buffer
  if (false == rWriter.Write(TempBuffer,SampleFrames,eSampleFloat32))
    return false;
  //Finalize
  if (false == rWriter.Finalize())
    return false;
  //
  delete TempBuffer;
  //
  return true;
}


//
//GetData
//
float* nuiSample::GetData(int8 channel)
{
  return &mSamples[channel][0];
}
//
const float* nuiSample::GetData(int8 channel) const
{
  return &mSamples[channel][0];
}


//
//Insert(int32 Pos, int32 NbSamples)
//
bool nuiSample::Insert(int32& Pos, int32 NbSamples)
{
  int32 TempPos = Pos;
  
  int8 c;
  for (c = 0; c < mInfos.GetChannels(); c++)
  {
    std::vector< float >::iterator MyIterator = mSamples[c].begin();
    
    while (TempPos > mSamples[c].size())
    {
      mSamples[c].push_back(0);
      Pos++;
    }
    mSamples[c].insert(MyIterator + Pos, NbSamples, 0);
  }
  mInfos.SetSampleFrames(mSamples[0].size());
  
  return true;
}


//
//Insert(int32 Pos, nuiSample* pSample)
//
bool nuiSample::Insert(int32 Pos, nuiSample* pSample)
{
  if (NULL == pSample)
    return false;
  if (pSample->mInfos.GetChannels() != mInfos.GetChannels())
    return false;
  
  int32 TempPos = Pos;
  double PosRead = 0;
  
  int32 SamplesToWrite = (int32)(pSample->mInfos.GetSampleFrames() / pSample->mInfos.GetSampleRate() * mInfos.GetSampleRate());
  
  float* pDest = new float[SamplesToWrite];
  
  Insert(Pos,SamplesToWrite);
  
  int32 c;
  for (c = 0; c < pSample->mInfos.GetChannels(); c++)
  {   
    pSample->InterpolateChannel(eInterpolLinear,PosRead,c,pSample->mInfos.GetSampleRate() / mInfos.GetSampleRate(),pDest,SamplesToWrite);
    
    memcpy((void*)(GetData(c) + TempPos),pDest,SamplesToWrite * sizeof(float));
  }
  mInfos.SetSampleFrames(mSamples[0].size());
  
  delete[] pDest;
  
  return true;
}


//
//Delete
//
bool nuiSample::Delete(int32 Pos, int32 NbSamples)
{
  int64 SampleFrames;
  int32 c;
  for (c = 0; c < mInfos.GetChannels(); c++)
  {
    std::vector< float >::iterator MyIterator = mSamples[c].begin();
    mSamples[c].erase(MyIterator + Pos, MyIterator + Pos + NbSamples);
  }
  //
  SampleFrames = mInfos.GetSampleFrames() - NbSamples;
  mInfos.SetSampleFrames(SampleFrames);
  return true;
}


//
//Clone() const
//
nuiSample* nuiSample::Clone() const
{
  nuiSample* pSample = new nuiSample(mInfos);
  
  int32 Channels = mInfos.GetChannels();
  int32 SampleFrames = (int32)mInfos.GetSampleFrames();
  //
  int32 c;
  for (c = 0; c < Channels; c++)
  {
    memcpy(pSample->GetData(c), GetData(c), sizeof(float) * SampleFrames);
  }
  return pSample;
}


//
//Clone(double SampleRate) const
//
nuiSample* nuiSample::Clone(double SampleRate) const
{
  nuiSample* pSample = new nuiSample(mInfos);
  pSample->mInfos.SetSampleRate(SampleRate);
  pSample->mInfos.SetSampleFrames((int32)(mInfos.GetSampleFrames() / (mInfos.GetSampleRate()) * SampleRate));
  
  int8 c;
  for (c = 0; c < mInfos.GetChannels(); c++)
  {
    double position = 0;
    pSample->mSamples[c].resize(pSample->mInfos.GetSampleFrames());
    InterpolateChannel(eInterpolLinear, position, c, mInfos.GetSampleRate()/SampleRate, pSample->GetData(c), pSample->mInfos.GetSampleFrames());
  }  
  return pSample;
}

//
//InterpolateChannel
//
bool nuiSample::InterpolateChannel(nuiInterpolationMethod method, double& rPosition, int8 Channel, double increment, float* pDest, int64 DestSize, double multVolume) const
{
  nuiAudioResampler<float>* resampler = new nuiAudioResampler<float>(eInterpolLinear);
  int32 samplesWritten = (int32)resampler->Process(pDest, &mSamples[Channel][0], (int32)DestSize, (int32)DestSize, rPosition, true, multVolume);
  delete resampler;
  
  return (samplesWritten == DestSize);
}


//
//InterpolateChannelAdd
//
bool nuiSample::InterpolateChannelAdd(nuiInterpolationMethod method, double& rPosition, int8 Channel, double increment, float* pDest, int64 DestSize, double multVolume) const
{
  nuiAudioResampler<float>* resampler = new nuiAudioResampler<float>(eInterpolLinear);
  int32 samplesWritten = (int32)resampler->Process(pDest, &mSamples[Channel][0], (int32)DestSize, (int32)DestSize, rPosition, false, multVolume);
  delete resampler;
  
  return (samplesWritten == DestSize);
}



const nuiSampleInfo& nuiSample::GetInfo() const
{
  return mInfos;
}

