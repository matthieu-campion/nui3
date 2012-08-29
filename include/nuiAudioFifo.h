/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAudioDevice.h"
#include "nuiAudioResampler.h"
#include "nglRingBuffer.h"
#include "nglThread.h"

#pragma once




//*******************************************************************************************************
//
// nuiAudioTrack
//
class nuiAudioTrack
{
  friend class nuiAudioFifo;
  
public:
					
  nuiAudioTrack();
	virtual ~nuiAudioTrack();

	// inline
	void Pause(bool pause = true);
	bool IsPaused() const;
	void SetVolume(float volume);
	float GetVolume() const;
  double GetSampleRate() const;
  int32 GetNbChannels() const;
  

protected : 

	// virtual method : audio input : to be implemented in inherited classes. The buffer contents is uknown, maybe you should reset the buffer before filling it.
  virtual int32 ReadSamples(int32 sampleFrames, std::vector<float*>& rBuffer);
  

	// virtual method : give information about the process
	//	-> sampleFrames : nb of sampleFrames just processed by audio system for this track (<=> reading progress bar)
	//	-> bufSize : size of reading buffer 
	//  -> bufPos : current position in the reading buffer 
	virtual	void	ProcessedSamples(int32 sampleFrames, int32 bufSize, int32 bufPos);

private:

  bool  ResetBuffer ();
	void	Init(int32 bufSize, double sampleRate, int32 nbChannels, float volume, bool bufferingEnabled);
  
	int32 CanRead();
  void Read(int32 sampleFrames);
  
  bool CanProcess(int32 SampleFrames);
  void Process(int32 SampleFrames, std::vector<float*>& rOutputBuffer);
  
	nglRingBuffer*	mpRingBuf;		// ring buffer for audio data
	int32	mBufSize;
	float	mVolume;
  bool mBufferingEnabled;

	bool mPaused;
	nglCriticalSection mPausedCS;

	// audio params
	double mSampleRate; 
	int32 mNbChannels;
};




//*******************************************************************************************************
//
//  nuiAudioFifo
//
// handle a list of nuiAudioTrack and do the mix to output the whole thing
//
class nuiAudioFifo : public nglThread
{
public : 

	// Constr.
	nuiAudioFifo(int32 inbufSize, int32 outbufSize, double sampleRate, int32 nbChannels, const nglString& inDeviceName = nglString::Empty, const nglString& outDeviceName = nglString::Empty, const nglString& apiName = nglString::Empty);
	~nuiAudioFifo();

	bool Start();
	void Reset();
	void Close();
	bool IsStarted() {return mStarted;};

  // Register a track with the fifo
  // Set streamed to true if the track handles its own streaming
	bool RegisterTrack(nuiAudioTrack* track, double sampleRate, int32 nbChannels, float volume, bool bufferingEnabled = true);
	bool UnregisterTrack(nuiAudioTrack* track);
  bool ResetTrackBuffer(nuiAudioTrack* track, bool makePause);
  
  const nglString& GetInDeviceName() const;
  const nglString& GetOutDeviceName() const;
  const nglString& GetAPIName() const;
  int32 GetRingBufferSize() const;
  int32 GetOutBufferSize() const;
  double GetSampleRate() const;
  int32 GetNbChannels() const;


private : 

 	void SetStopRequest(bool req);
	bool GetStopRequest ();


	bool mStarted;
	int32 mNbChannels;
	double mSampleRate;

  nglString mInDeviceName;
  nglString mOutDeviceName;
  nglString mAPIName;
  nuiAudioDevice*	mpAudioDevice;			// to output audio
	std::vector<nuiAudioTrack*> mAudioTracks;			// list of audio items to mix and play
	nglCriticalSection mInputCS;				// locked when samples are read and buffered in the ring buffers
	nglCriticalSection mOutputCS;				// locked when ring buffers are read, mix, and output to audio device


	bool mStopRequest;	// tells the reading thread to end
	nglCriticalSection	mStopRequestCS;
	
	int32 mRingbufSize;
	int32 mOutbufSize;

  
	std::vector<float*>	mOutputReadingBuffer;	// usefull to mix the tracks
  std::vector<std::vector<nuiAudioResampler<float>* > >  mResamplers;



	// reading Thread
	virtual void OnStart ();

	// output audio callback
  void Process(const std::vector<const float*>& rInputBuffers, const std::vector<float*>& rOutputBuffers, int32 SampleFrames);
};


