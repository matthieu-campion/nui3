/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiSampleReader.h"
#include "nuiSampleWriter.h"
#include "nuiAudioResampler.h"

///Class that encapsulates and manipulates a sound samples buffer
class  nuiSample
{
public:
  nuiSample(const nuiSampleInfo& rInfos); ///< Constructor that just initializes infos \param rInfos nuiSampleInfo object used to init infos
  nuiSample(nuiSampleReader& rReader);  ///< Constructor that initialize infos and sound samples vector with a nuiSampleReader object \param rReader nuiSampleReader object used to init infos and samples vector
  void Reset();
  virtual ~nuiSample();
  
  bool Save(nuiSampleWriter& rWriter, int32 BitsPerSample); ///< Method that saves infos and samples in a nuiSamplewriter object \param rWriter nuiSampleWriter object in which we write \param BitsPerSampleNumber of bits per sample in the output nuiSampleWriter object \return True if succeeded
  
  float* GetData(int8 channel);
  const float* GetData(int8 channel) const;
  
  const nuiSampleInfo& GetInfo() const;

  bool Insert(int32& Pos, int32 NbSamples); ///< Method that Inserts null samples at a user-defined position \param Pos Position where write the null samples \param NbSamplesNumber of null samples to write \return True if succeeded
  bool Insert(int32 Pos, nuiSample* pSample = NULL);  ///< Method that Inserts given samples from nuiSample Object at a user-defined position. If the numbers of channels do not match, the method fails. If sample rates do not match, samples to insert are resampled with the InterpolateChannel method \param Pos Position where write samples \param pSample nuiSample Object to copy from \return True if succeeded
  bool Delete(int32 Pos, int32 NbSamples);  ///< Method that deletes a user-defined number of samples at a user-defined position \param Pos Position where delete samples \param NbSamples Number of samples to delete \retun True i succeeded

  nuiSample* Clone() const;  ///< Method that copies the nuiSample into another \return A Pointer to the copy of the nuiSample object
  nuiSample* Clone(double SampleRate) const; ///< Method that copies the nuiSample into another whose nuiSample rate is user-defined \param SampleRate Value of the nuiSample Rate for the sample object copy \return A pointer to the copy of the nuiSample object
  
  bool InterpolateChannel(nuiInterpolationMethod method, double& rPosition, int8 Channel, double increment, float* pDest, int64 DestSize, double multVolume = 1.0) const;
  bool InterpolateChannelAdd(nuiInterpolationMethod method, double& rPosition, int8 Channel, double increment, float* pDest, int64 DestSize, double multVolume = 1.0) const;

protected:
  nuiSampleInfo mInfos;
  std::vector<std::vector<float> > mSamples;
  
};