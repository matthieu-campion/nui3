/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


// the minimum free space (in sampleFrames) in a ringbuffer to call a reading
#define AUDIOTRACK_READ_MIN 64






//*******************************************************************************************************
//*******************************************************************************************************
//
// nuiAudioTrack Class
//
//*******************************************************************************************************
//*******************************************************************************************************

nuiAudioTrack::nuiAudioTrack()
: mPausedCS(nglString(_T("nuiAudioTrack_mPausedCS")))
{
	// to be set when registered by nuiAudioFifo
	mpRingBuf	= NULL;	
  
	mPaused		= true;
	mSampleRate	= 44100.0f; // to be set when registered
	mNbChannels	= 2;	 // to be set when registered
	mVolume		= 1.0f;
  mBufferingEnabled = true;
}


nuiAudioTrack::~nuiAudioTrack ()
{
	mPaused	= true;
	delete mpRingBuf;
  
}


void nuiAudioTrack::Init(int32 bufSize, double sampleRate, int32 nbChannels, float volume, bool bufferingEnabled)
{
	mNbChannels	= nbChannels;
	mSampleRate	= sampleRate;
  mBufferingEnabled = bufferingEnabled;
	mVolume = volume;
	delete mpRingBuf; // in case of register/unregister/register on the same track
  
  // init ringbuffer
  mpRingBuf = NULL;
  if (bufSize)
  {
    mpRingBuf = new nglRingBuffer (bufSize, sizeof(float), mNbChannels);
  }
	mPaused = false; // start the stuff
	
	mBufSize = bufSize;
}







int32 nuiAudioTrack::CanRead()
{
  int32 value;
  if (mpRingBuf)
  {
    value = mpRingBuf->GetWritable();
  }
  else
  {
    return 0; // cannot "read" into ringbuffer since it has none
  }
  
	if (value < AUDIOTRACK_READ_MIN)
		return 0;
  
	return value;
}


// virtual. to be overloaded
int32 nuiAudioTrack::ReadSamples(int32 sampleFrames, std::vector<float*>& rBuffer)
{
  // does nothing
  return 0; /* nbSampleFrames */
}










void nuiAudioTrack::Read(int32 sampleFrames)
{
  NGL_ASSERT(mpRingBuf);
  int32 nbRead=sampleFrames;
  int32 nbWrite=0;
  int32 nbWrite2=0;
  std::vector<float*> rbuf(mNbChannels);
  int32 c;
  
  
  
  
  //NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("\nWrite : read %d from source\n"), nbRead);
  
  
  
  //*******************************************
  //
  // first pass writing in ringBuffer
  //
  nbWrite  = mpRingBuf->GetWritableToEnd ();
  
  // the writable space may have grown since the last call of GetWritable, check that
  nbWrite  = (nbWrite > nbRead) ? nbRead : nbWrite;
  
  for (c=0; c<mNbChannels; c++)
    rbuf[c] = (float*)mpRingBuf->GetWritePointer(c);
  
  
  //NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("\nBEFORE BUFFERING %d\n"), nbWrite);  
  
  // read audio samples from the source
  int32 nbReadSamples = ReadSamples(nbWrite, rbuf/*ref for output*/);
  if (!nbReadSamples) 
    return; 
  
  
  
  
  //NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("\nAFTER BUFFERING %d\n"), nbWrite);
  
  
  mpRingBuf->AdvanceWriteIndex(nbWrite);
  
  //NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("\nWrite %d\n"), nbWrite);
  
  
  
  // everything has been written. no need of 2nd pass
  if (nbWrite == nbRead) 
    return;
  
  
  
  //*******************************************
  //
  // second pass writing in ringBuffer
  //
  nbWrite2  = mpRingBuf->GetWritableToEnd();
  if (nbWrite2 < (nbRead - nbWrite))
  {
    NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_ERROR, _T("ERROR : ringbuffer could not locked enough space to write data (%d requested, %d provided!)"), (nbRead-nbWrite), nbWrite2);
    NGL_ASSERT(0);
    return;
  }
  nbWrite2 = nbRead - nbWrite;
  
  for (c=0; c<mNbChannels; c++)
    rbuf[c] = (float*)mpRingBuf->GetWritePointer(c);
  
  
  // read audio samples from the source
  nbReadSamples = ReadSamples(nbWrite2, rbuf/*ref for output*/);
  if (!nbReadSamples) 
    return; 
  
  
  mpRingBuf->AdvanceWriteIndex (nbWrite2);
  
  //NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_DEBUG, _T("\nWrite %d\n"), nbWrite);
}









//*********************************************************
//
// ResetBuffer
//
// !! TAKE CARE !! don't directly call this function, or the whole audio system will mess up!
// call nuiAudioFifo::ResetTrackBuffer
//
bool nuiAudioTrack::ResetBuffer ()
{
	delete  (mpRingBuf); // in case of register/unregister/register on the same track
  if (mBufSize)
  {
    mpRingBuf		= new nglRingBuffer(mBufSize, sizeof(float), mNbChannels);
  }
  
  
  return true;
}




bool nuiAudioTrack::CanProcess (int32 SampleFrames)
{
  if (!mBufferingEnabled)
    return true;
  
  int32 curValue = mpRingBuf->GetReadable();
  
	// not enough data in the ringbuffer
	if (curValue < SampleFrames)
	{
    //#FIXME : comment this message once you have ensure everything goes right. Otherwise, you'll always get this message when the audio fifo starts (waiting for the data to come...)
		NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_WARNING, _T("\nAUDIO SYSTEM WARNING : NOT ENOUGH AUDIO DATA BUFFERED FOR THE CURRENT TRACK : currently %d buffered samples, it's not enough to read %d samples!\n"), curValue, SampleFrames);
		return false;
	}
  
	return true;
}



void nuiAudioTrack::Process(int32 SampleFrames, std::vector<float*>& rOutputBuffer)
{
  float	*rbuf;
  int32	nbRead=0;
  int32	nbRead2=0;
  int32	totalRead=0;
  float	l_volume = mVolume;
  int32	ch, i;
  
  //
  // we don't want to buffer the audio
  //
  if (!mBufferingEnabled)
  {
    // read directly from the samples source to the AudioFifo buffer. Don't use ringbufer
    ReadSamples (SampleFrames, rOutputBuffer);
    
    // set the volume
    for (ch = 0; ch < rOutputBuffer.size(); ch++)
    {
      for (i=0; i < SampleFrames; i++) 
      {
        rOutputBuffer[ch][i] = rOutputBuffer[ch][i] * l_volume;
      }
    }
    
    // event callback
    ProcessedSamples(SampleFrames, 0, 0);
    return;
  }
  
  
  //
  // we do want to stream the audio
  //
  
  //*******************************************************
  //
  // first pass reading from ringBuffer
  //
  nbRead = mpRingBuf->GetReadableToEnd();
  
  
  // read what you need. maybe less, but not more!
  if (nbRead > SampleFrames) nbRead = SampleFrames;
  
  for (ch = 0; ch < mNbChannels; ch++)
  {
    rbuf = (float*)mpRingBuf->GetReadPointer(ch);
    
    for (i=0; i < nbRead; i++)
    {
      rOutputBuffer[ch][i] = rbuf[i] * l_volume;
    }
  }
  
  
  mpRingBuf->AdvanceReadIndex (nbRead);
  
  // NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_DEBUG, _T("\n\t\t\t\tRead %d\n"), nbRead);
  
  
  //*******************************************************
  //
  // second pass reading from ringBuffer
  //
  
  // do you need some more ?
  if (nbRead >= SampleFrames) 
  {
    totalRead += nbRead;
    
    // NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_DEBUG, _T("\n\t\t\t\tReadTh : %d samples read"), totalRead);
    
    // event callback
    ProcessedSamples(nbRead, mBufSize, mpRingBuf->GetReadable());
    
    return;
  }
  
  
  // yes, I do
  nbRead2 = mpRingBuf->GetReadableToEnd();
  if (nbRead2 > (SampleFrames - nbRead)) nbRead2 = (SampleFrames - nbRead);
  
  for (ch = 0; ch < mNbChannels; ch++)
  {
    rbuf = (float*)mpRingBuf->GetReadPointer(ch);
    
    for (i=0; i < nbRead; i++)
    {
      rOutputBuffer[ch][i] = rbuf[i] * l_volume;
    }
  }
  
  mpRingBuf->AdvanceReadIndex(nbRead2);    
  
  // NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_DEBUG, _T("\n\t\t\t\tRead %d\n"), nbRead2);    
  
  totalRead += (nbRead + nbRead2);
  
  // NGL_LOG(_T("nuiAudioTrack"), NGL_LOG_DEBUG, _T("\n\t\t\t\tReadTh : %d samples read"), totalRead);  
  
  // event callback
  ProcessedSamples(totalRead, mBufSize, mpRingBuf->GetReadable ());
}




//virtual method, to be inherited, optionnaly
void nuiAudioTrack::ProcessedSamples(int32 sampleFrames, int32 bufSize, int32 bufPos)
{
  // does nothing
}




void nuiAudioTrack::Pause(bool pause)
{
  nglCriticalSectionGuard guard(mPausedCS); 
  mPaused=pause;
}

bool	nuiAudioTrack::IsPaused() const
{
  return mPaused;
}

void nuiAudioTrack::SetVolume(float volume)
{
  mVolume = volume;
}

float nuiAudioTrack::GetVolume() const
{
  return mVolume;
}

double nuiAudioTrack::GetSampleRate() const
{
  return mSampleRate;
}

int32 nuiAudioTrack::GetNbChannels() const
{
  return mNbChannels;
}




//*******************************************************************************************************
//*******************************************************************************************************
//
// nuiAudioFifo CLASS
//
//*******************************************************************************************************
//*******************************************************************************************************




//*****************************************************************************************
//
// CONSTR.
//
//*****************************************************************************************
nuiAudioFifo::nuiAudioFifo (int32 inbufSize, int32 outbufSize, double sampleRate, int32 nbChannels, const nglString& inDeviceName /* = nglString::Empty*/, const nglString& outDeviceName /*= nglString::Empty*/, const nglString& apiName /*= nglString::Empty*/)
: nglThread(Highest), mpAudioDevice(NULL), mInputCS(nglString(_T("nuiAudioFifo_mInputCS"))), mOutputCS(nglString(_T("nuiAudioFifo_mOutputCS"))), mStopRequestCS(nglString(_T("nuiAudioFifo_mStopRequestCS")))
{    
  
	mStarted		= false;
	mNbChannels	= nbChannels;
	mSampleRate	= sampleRate;
  mInDeviceName  = inDeviceName;
  mOutDeviceName = outDeviceName;  
  mAPIName = apiName;
  
	// ringbuffer size must be x = y^2
	mRingbufSize = inbufSize;
	NGL_MAKE_POWER_OF_TWO_RINGBUFFER(mRingbufSize);
	assert (mRingbufSize == inbufSize);
  
	mOutbufSize = outbufSize;
  
	// we need a reading buffer for output mixing
  mOutputReadingBuffer.resize(mNbChannels);
  for (int32 ch=0; ch < mNbChannels; ch++)
    mOutputReadingBuffer[ch] = new float[mOutbufSize];
}



nuiAudioFifo::~nuiAudioFifo()
{
	Close();
	for (size_t i=0; i < mAudioTracks.size(); i++)
		UnregisterTrack (mAudioTracks[i]);
	mAudioTracks.clear ();
  
	for (int32 ch=0; ch < mNbChannels; ch++)
    delete mOutputReadingBuffer[ch];
  mOutputReadingBuffer.clear();
}





//*****************************************************************************************
//
// AUDIO CONTROLS
//
//*****************************************************************************************
bool nuiAudioFifo::Start()
{
	if (mStarted) 
    return false;
  
  nuiAudioDeviceManager::Get().Update();
  if (mOutDeviceName.IsEmpty() || mAPIName.IsEmpty())
  {
    mpAudioDevice = nuiAudioDeviceManager::Get().GetDefaultOutputDevice();
  }
  else
  {
    mpAudioDevice = nuiAudioDeviceManager::Get().GetDeviceWithNameAndAPI(mOutDeviceName, mAPIName);
  }
  
  if (!mpAudioDevice)
  {
    NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_ERROR, _T("Start ERROR : could not retrieve audio device!\n"));
    return false;
  }
  
  NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("start\n"));
  
  
  
  // means, no input channels for now, and all output channels are active for now...
  std::vector<int32> inputChannels;
  std::vector<int32> outputChannels;
  outputChannels.resize(mNbChannels);
  for (int i=0; i < mNbChannels; i++)
    outputChannels[i] = i; 
  
  //#FIXME : ensure that samplerate and outbufsize are acceptable values for nuiAudioDevice (should enumerate possible value somewhere in the application)
  
  // open device access and start process
  bool res = mpAudioDevice->Open(inputChannels/*<=> mNbInputChannels, #FIXME ? do we provide some inputs?*/, outputChannels, mSampleRate, mOutbufSize, nuiMakeDelegate(this, &nuiAudioFifo::Process));
  
  if (!res)
  {
    NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_ERROR, _T("Start ERROR : could not open audio device!\n"));
    return false;
  }
  
	// start input thread
	nglThread::Start();  
  
	mStarted = true;
  return true;
}



void nuiAudioFifo::Reset()
{
	if (!mStarted)
    return;
  
	for (size_t i=0; i < mAudioTracks.size(); i++)
		UnregisterTrack (mAudioTracks[i]);
	mAudioTracks.clear ();
  
}



void nuiAudioFifo::Close()
{
	if (!mStarted)
    return;
  
	Reset();
  
	//close audio here
  delete mpAudioDevice;
  mpAudioDevice = NULL;
  
	SetStopRequest(true);
  
	nglThread::Join();
  
  NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("Close ok\n"));
}




//*****************************************************************************************
//
// AUDIOTRACKS MANAGEMENT
//
//*****************************************************************************************

bool nuiAudioFifo::RegisterTrack (nuiAudioTrack* track, double sampleRate, int32 nbChannels, float volume, bool mBufferingEnabled /*= true*/)
{
  
	// don't forget you may have two threads running and accessing those data
	nglCriticalSectionGuard inputGuard(mInputCS);
	nglCriticalSectionGuard outputGuard(mOutputCS);
	
	mAudioTracks.push_back(track);
	track->Init(mBufferingEnabled?mRingbufSize:0, sampleRate, nbChannels, volume, mBufferingEnabled);
  
  if (sampleRate != mSampleRate)
  {
    mResamplers.push_back(std::vector<nuiAudioResampler<float>* >());
    mResamplers[mResamplers.size()-1].reserve(nbChannels);
    for (int32 ch=0;ch<nbChannels; ch++)
      mResamplers[mResamplers.size()-1].push_back(new nuiAudioResampler<float>(eInterpolCubic));
  }
  else
  {
    mResamplers.push_back(std::vector<nuiAudioResampler<float>* >());
  }
  
	return true;
}





bool nuiAudioFifo::UnregisterTrack (nuiAudioTrack* track)
{
  
  
	track->Pause (true); 
  
	// don't forget you may have two threads running and accessing those data
	nglCriticalSectionGuard inputGuard(mInputCS);
	nglCriticalSectionGuard outputGuard(mOutputCS);
  
  std::vector<std::vector<nuiAudioResampler<float>* > >::iterator it = mResamplers.begin();
	for(std::vector<nuiAudioTrack*>::iterator  itl=mAudioTracks.begin(); itl!=mAudioTracks.end(); ++itl, ++it)
	{
		nuiAudioTrack* cur = *itl;
    
		if (cur == track)
		{
			mAudioTracks.erase (itl);
      if (track->mSampleRate != mSampleRate)
      {
        for (int32 i=0;i<track->mNbChannels;i++)
        {
          delete (*it)[i];
        }
      }
      mResamplers.erase(it);
      
			// to anticipate an optional following RegisterTrack of the same track
			track->Pause (false);
			return true;
		}
	}
  
	// to anticipate an optional following RegisterTrack of the same track
	track->Pause (false);
	return false;
}




bool nuiAudioFifo::ResetTrackBuffer (nuiAudioTrack* track, bool makePause)
{
  track->Pause ();
  
	// don't forget you may have two threads running and accessing those data
	nglCriticalSectionGuard inputGuard(mInputCS);
	nglCriticalSectionGuard outputGuard(mOutputCS);
  
  track->ResetBuffer ();
  
	// go back to the normal state
  if(makePause==false)		
    track->Pause (false);
  return true;
  
}


//*****************************************************************************************
//
// AUDIO BUFFERING MANAGEMENT
//
//*****************************************************************************************



void nuiAudioFifo::SetStopRequest (bool req)
{
	mStopRequestCS.Lock ();
	mStopRequest = req;
	mStopRequestCS.Unlock ();
}


bool nuiAudioFifo::GetStopRequest ()
{
  return mStopRequest;
}










//**************************************************
//
// reading Thread
//
void nuiAudioFifo::OnStart ()
{
  int32 nbSampleFrames=0;
  bool fifoReadFlag=true;
  
	SetStopRequest (false);
  
  
  // NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("Reading Thread start\n"));
  
	while (!GetStopRequest ())
	{
    
    
		// all tracks had a full buffer in the previous pass. No need to read. Sleep for a while.
		if (!fifoReadFlag)
			MsSleep (1);
    
    
    
		{
			// you may now acces the tracks to read
			nglCriticalSectionGuard guard(mInputCS);
      
			// init. <=> suppose at least one track will have something to read (<=>its buffer isn't full already)
			fifoReadFlag = false;
      
			// ask each audio track to read samples and store them in their ringbuffers
			for (size_t i=0; i < mAudioTracks.size (); i++)
			{
				// LOCK pause flag before reading audioBuffer and then other thread must wait to change this flag
				nglCriticalSectionGuard guard(mAudioTracks[i]->mPausedCS);
        
				if (mAudioTracks[i] && !mAudioTracks[i]->IsPaused ())
				{
					// check if it's necessary to read for this track (the buffer may already be full)
					nbSampleFrames  = mAudioTracks[i]->CanRead ();
					fifoReadFlag  |= (nbSampleFrames != 0);
					// yes it is : do read
					if (nbSampleFrames)	
					{
            mAudioTracks[i]->Read(nbSampleFrames); 
					}
				}
			}
		}
    
		// mInputCS is automatically unlocked by the guard
	}
  
  // NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("Reading Thread stopped\n"));
  
	mStarted = false;
}










//**************************************************
//
// output audio callback
//
void nuiAudioFifo::Process(const std::vector<const float*>& rInputBuffers, const std::vector<float*>& rOutputBuffers, int32 SampleFrames)
{
  
	if (GetStopRequest()) 
    return;
  
  // NGL_LOG(_T("nuiAudioFifo"), NGL_LOG_DEBUG, _T("\n\t\t\t\tReadTh\n"));
  
  
	{
		// you may now acces the tracks to read
		nglCriticalSectionGuard guard(mOutputCS);
    
    
		// reset outputbuffer for mixing with silence
		for (int32 i=0; i < mNbChannels; i++)
		{
			memset (rOutputBuffers[i], 0, SampleFrames * sizeof(float));
		}
    
    
		// now, for each track
		for (size_t i=0; i < mAudioTracks.size(); i++)
		{
			if (mAudioTracks[i] && !mAudioTracks[i]->IsPaused())
			{
        
        
				// is there enough audio to play in the buffer of this track?
				if (!mAudioTracks[i]->CanProcess(SampleFrames))
          
          // no, there is not. play silence instead of the expected sound
          //
          // #FIXME  : > in a real mixing system, we should count the silent sampleFrames 
          //             in order to skip the same number of sampleFrames in the next playing pass : 
          //			   that's necessary to keep the mix synchronized. 
          //           > for now, we don't take care about that, because we don't need the tracks to be synchronized
          //             (because the synchronized mix is already made by the Audio engine, and is handled here as a single track)
          //          
				{
					// actually, we don't have to generate silence, we simply don't have to mix anything :)
					// GenerateSilence (m_outputReadingBuffer, SampleFrames);
					continue;
				}
        
        
        
				// yes, there is. this tracks does have something to play
        
				// get buffered audio data from the track
        // resample if needed
        int32 nbRead = (int32)((double)SampleFrames * mAudioTracks[i]->mSampleRate) / mSampleRate;
        if (nbRead == SampleFrames)
        {
          mAudioTracks[i]->Process(nbRead, mOutputReadingBuffer);
        }
        else
        {
          std::vector<float*> tempBuf;
          tempBuf.resize(mAudioTracks[i]->mNbChannels);
          for (int32 ch=0; ch<mAudioTracks[i]->mNbChannels; ch++)
            tempBuf[ch] = new float[nbRead];
          
          mAudioTracks[i]->Process(nbRead, tempBuf);
          
          for (int32 ch = 0; ch<mNbChannels; ch++)
          {
            mResamplers[i][ch]->Process(mOutputReadingBuffer[ch], tempBuf[ch], nbRead, SampleFrames);
            delete tempBuf[ch];
          }
          
          tempBuf.clear();
          
        }
        
				// mix it in the output buffer
        float *pOutputBuffer, *pInputBuffer;
        for (int32 ch=0; ch < mNbChannels; ch++)
        {
          pOutputBuffer = rOutputBuffers[ch];
          pInputBuffer = mOutputReadingBuffer[ch];
          for (int32 j=0; j < mOutbufSize; j++)
          {
            *(pOutputBuffer++) += *(pInputBuffer++);
          }
				}
        
			}
      
			
		} // end for
    
		// mOutputCS is automatically unlocked by the guard
	}    	
}



const nglString& nuiAudioFifo::GetInDeviceName() const
{
  return mInDeviceName;
}

const nglString& nuiAudioFifo::GetOutDeviceName() const
{
  return mOutDeviceName;
}

const nglString& nuiAudioFifo::GetAPIName() const
{
  return mAPIName;
}

int32 nuiAudioFifo::GetRingBufferSize() const
{
  return mRingbufSize;
}

int32 nuiAudioFifo::GetOutBufferSize() const
{
  return mOutbufSize;
}

double nuiAudioFifo::GetSampleRate() const
{
  return mSampleRate;
}

int32 nuiAudioFifo::GetNbChannels() const
{
  return mNbChannels;
}


