#include "nui.h"
#include "nuiAudioDecoder.h"
#include "nuiAudioConvert.h"

#include "windows.h"
// If visual studio complains about this header you need to update to the latest windows SDK
// http://www.microsoft.com/downloads/details.aspx?FamilyID=4377f86d-c913-4b5c-b87e-ef72e5b4e065&displaylang=en
#include "wmsdk.h" 
#include "objidl.h"

#pragma comment (lib, "wmvcore.lib")

#define PCM_AUDIO_FORMAT_TAG 0x0001

#define REQUESTED_BITS_PER_SAMPLE 32 //we want to retrieve 32 bits audio samples

#define ONE_SECOND 10000000 //one second in 100 nano sec unit (unit used by Windows Media Reader)

class SamplesQueue
  {
  public:
    SamplesQueue();
    virtual ~SamplesQueue();
    
    uint32 GetUtilSize() const;
    uint32 GetMemorySize() const;
    
    void Reset();
    
    void Append(uint8* pSrc, uint32 size);
    uint32 ReadDeInterleavedFloat32(std::vector<float*>& rBuffers, uint32 FramesRequested);
    
    void SetInputBytesPerSample(uint32 bytes);
    uint32 GetInputBytesPerSample() const;
  private:
    std::vector<uint8> mBuffer;
    uint32 mUtilSize;
    uint32 mInputBytesPerSample;
  };

class nglWindowsMediaIStream : public IStream
  {
  public:
    nglWindowsMediaIStream(nglIStream& rStream);
    virtual ~nglWindowsMediaIStream();
    
    //from IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    
    //from ISequentialStream
    virtual HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead);
    virtual HRESULT STDMETHODCALLTYPE Write(const void *pv, ULONG cb, ULONG *pcbWritten);
    
    //from IStream
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
    virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize);
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
    virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);
    virtual HRESULT STDMETHODCALLTYPE Revert();
    virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
    virtual HRESULT STDMETHODCALLTYPE Stat(__RPC__out STATSTG *pstatstg, DWORD grfStatFlag);
    virtual HRESULT STDMETHODCALLTYPE Clone(__RPC__deref_out_opt IStream **ppstm);
    
  private:
    ULONG mRefCount;
    nglIStream& mrStream;
  };

class nuiAudioDecoderPrivate
  {
  public:
    nglWindowsMediaIStream* mpWMStream;
    IWMSyncReader* mpWMReader;
    SamplesQueue mQueue;
  };



void nuiAudioDecoder::Clear()
{
  if (mpPrivate->mpWMReader)
  {
    mpPrivate->mpWMReader->Close();
    mpPrivate->mpWMReader->Release();
    mpPrivate->mpWMReader = NULL;
  }
  
  if (mpPrivate->mpWMStream)
  {
    mpPrivate->mpWMStream->Release();
    mpPrivate->mpWMStream = NULL;
  }
  
  CoUninitialize();
  
  if (mpPrivate)
    delete mpPrivate;
}

bool nuiAudioDecoder::CreateAudioDecoderPrivate()
{
  if (mpPrivate)
  {
    if (mpPrivate->mpWMReader)
      delete mpPrivate->mpWMReader;
    delete mpPrivate;
  }

  CoInitialize(NULL);
  mpPrivate = new nuiAudioDecoderPrivate();
  HRESULT hr = S_OK;
  mpPrivate->mpWMStream = new nglWindowsMediaIStream(mrStream);

  hr = WMCreateSyncReader(NULL, WMT_RIGHT_PLAYBACK, &mpPrivate->mpWMReader);
  if (SUCCEEDED(hr))
  {
    hr = mpPrivate->mpWMReader->OpenStream(mpPrivate->mpWMStream);
    if (FAILED(hr))
    {
      return false;
    }
  }
  return false;
}

bool nuiAudioDecoder::Seek(uint64 SampleFrame)
{
  QWORD StartTime = (QWORD)(SampleFrame / mInfo.GetSampleRate() * ONE_SECOND);
  HRESULT hr = mpPrivate->mpWMReader->SetRange(StartTime, 0/*no duration set*/);
  
  if (SUCCEEDED(hr))
  {
    mpPrivate->mQueue.Reset(); //nullify the datas already stored in the Queue
    return true;
  }
  
  return false;
}

bool nuiAudioDecoder::ReadInfo()
{
  if (!mpPrivate)
	return false;

  HRESULT hr = S_OK;
  bool result = false;
  DWORD OutputNb = 0;
  DWORD FormatNb = 0;
  WORD StreamNb = 0;
  
  DWORD OutputCount = 0;
  hr = mpPrivate->mpWMReader->GetOutputCount(&OutputCount);
  
  if (FAILED(hr))
    return false;
  
  if (OutputCount != 1)
    return false;
  
  for (DWORD out = 0; out < OutputCount; out++)
  {
    DWORD FormatCount = 0;
    hr = mpPrivate->mpWMReader->GetOutputFormatCount(out, &FormatCount);
    
    for (DWORD format = 0; format < FormatCount; format++)
    {
      IWMOutputMediaProps* pMediaProps = NULL;
      hr = mpPrivate->mpWMReader->GetOutputFormat(out, format, &pMediaProps);
      
      if (FAILED(hr))
      {
        result = false;
        break;
      }
      
      GUID typeGUID;
      hr= pMediaProps->GetType( &typeGUID );
      if (typeGUID == WMMEDIATYPE_Audio)
      {
        DWORD AllocSize = 0;
        hr = pMediaProps->GetMediaType(NULL, &AllocSize);
        if (FAILED(hr))
        {
          result = false;
          break;
        }
        
        WM_MEDIA_TYPE* pType = (WM_MEDIA_TYPE*)malloc(AllocSize);
        if (!pType)
          break;
        hr = pMediaProps->GetMediaType(pType, &AllocSize);
        if (FAILED(hr))
        {
          free(pType);
          result = false;
          break;
        }
        
        //we want non compressed audio data
        if (pType->subtype == WMMEDIASUBTYPE_PCM && pType->formattype == WMFORMAT_WaveFormatEx)
        {
          WAVEFORMATEX* pWFmt = (WAVEFORMATEX*)(pType->pbFormat);
          
          //Check if the samples are PCM audio
          if (pWFmt->wFormatTag != PCM_AUDIO_FORMAT_TAG)
          {
            free(pType);
            result = false;
            break;
          }
          WORD nbChannels = pWFmt->nChannels;     //retrieve the number of channels
          DWORD SampleRate = pWFmt->nSamplesPerSec;   //retrieve the sample rate
          WORD bitsPerSample = pWFmt->wBitsPerSample;   //retrieve the number of bits per sample
          
          //Get the stream number
          hr= mpPrivate->mpWMReader->GetStreamNumberForOutput(out, &StreamNb);
          if (FAILED(hr))
          {
            free(pType);
            result = false;
            break;
          }
          
          // Specify this current format as the desired output format
          hr= mpPrivate->mpWMReader->SetOutputProps(out, pMediaProps);
          if (FAILED(hr))
          {
            free(pType);
            result = false;
            break;
          }
          
          // Set the stream number for reading
          WMT_STREAM_SELECTION streamSelection= WMT_ON;
          hr= mpPrivate->mpWMReader->SetStreamsSelected( 1, &StreamNb, &streamSelection );
          if (FAILED(hr))
          {
            free(pType);
            result = false;
            break;
          }
          
          // Specify that we want uncompressed samples
          hr= mpPrivate->mpWMReader->SetReadStreamSamples(StreamNb, FALSE );
          if (FAILED(hr))
          {
            free(pType);
            result = false;
            break;
          }
          
          //estimate the length of the stream in sample using the length in time unit
          uint64 SampleFrames = 0;
          IWMHeaderInfo* pHeaderInfo = NULL;
          mpPrivate->mpWMReader->QueryInterface(IID_IWMHeaderInfo, (void**)(&pHeaderInfo));
          if (pHeaderInfo)
          {
            WORD AnyStream = 0;
            WMT_ATTR_DATATYPE dataType= WMT_TYPE_QWORD;
            WORD length= 0;
            hr = pHeaderInfo->GetAttributeByName(&AnyStream, g_wszWMDuration, &dataType, NULL, &length);
            if (SUCCEEDED(hr) && dataType == WMT_TYPE_QWORD && length == sizeof(QWORD))
            {
              QWORD TimeLength;
              hr = pHeaderInfo->GetAttributeByName(&AnyStream, g_wszWMDuration, &dataType, (BYTE*)(&TimeLength), &length);
              if (SUCCEEDED(hr))
              {
                SampleFrames = TimeLength / ONE_SECOND * SampleRate;
              }
            }
            pHeaderInfo->Release();
          }
          
          mInfo.SetChannels(nbChannels);
          mInfo.SetSampleRate(SampleRate);
          mInfo.SetBitsPerSample(REQUESTED_BITS_PER_SAMPLE); // we want to convert to 32 bits float samples
          mInfo.SetSampleFrames(SampleFrames);
		      mInfo.SetStartFrame(0);
		      mInfo.SetStopFrame(mInfo.GetSampleFrames());
          mInfo.SetFileFormat(eAudioCompressed);
          
          mpPrivate->mQueue.SetInputBytesPerSample(bitsPerSample / 8);
          
          free(pType);
          result = true;
          break; //we have found the right output and format, and the Sample info object is filled
        }
      }
      
    }
  }
  
  return result;
}

uint32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return 0;
  
  uint32 BytesToRead    = sampleframes * mInfo.GetChannels() * mpPrivate->mQueue.GetInputBytesPerSample();
  HRESULT hr        = S_OK;
  INSSBuffer* pTempBuffer = NULL;
  QWORD SampleTime      = 0;
  QWORD SampleDuration    = 0;
  DWORD flags       = 0;
  DWORD outputNum     = 0;
  
  while (SUCCEEDED(hr) && mpPrivate->mQueue.GetUtilSize() < BytesToRead)
  {
    hr = mpPrivate->mpWMReader->GetNextSample(0, &pTempBuffer, &SampleTime, &SampleDuration, &flags, &outputNum, NULL);
    
    if (SUCCEEDED(hr))
    {
      BYTE* pBytes = NULL;
      DWORD length = 0;
      hr = pTempBuffer->GetBufferAndLength(&pBytes, &length);
      if (SUCCEEDED(hr))
      {
        mpPrivate->mQueue.Append((uint8*)pBytes, length);
      }
      pTempBuffer->Release();
      pTempBuffer = NULL;
    }
    
    SampleTime      = 0;
    SampleDuration    = 0;
    flags       = 0;
  }
  
  uint32 channels = mInfo.GetChannels();
  std::vector<float*> temp(channels);
  if (format == eSampleFloat32)
  {
    for (uint32 i = 0; i < channels; i++)
    {
      temp[i] = (float*)(buffers[i]);
    }
  }
  else
  {
    for (uint32 i = 0; i < channels; i++)
    {
      temp[i] = new float[sampleframes];
    }
  }
  
  uint32 FramesRead = mpPrivate->mQueue.ReadDeInterleavedFloat32(temp, sampleframes); // we want de-interleaved samples
  
  if (format == eSampleInt16)
  {
    for (uint32 i = 0; i < channels; i++)
    {
      nuiAudioConvert_FloatBufferTo16bits(temp[i], (int16*)(buffers[i]), FramesRead);
      delete[] temp[i];
    }
  }
  
  mPosition += FramesRead;
  return FramesRead;
}


uint32 nuiAudioDecoder::ReadIN(void* pBuffer, uint32 sampleframes, nuiSampleBitFormat format)
{
  //don't increment mPosition: it's already done in ReadDE
  uint32 channels = mInfo.GetChannels();
  
  uint32 length = mInfo.GetSampleFrames();
  if (mPosition >= length)
    return 0;
  sampleframes = MIN(sampleframes, length - mPosition);
  
  std::vector<float*> temp(channels);
  std::vector<void*> tempVoid(channels);
  for (uint32 c= 0; c < channels; c++)
  {
    temp[c] = new float[sampleframes];
    tempVoid[c] = (void*)(temp[c]);
  }
  
  uint32 sampleFramesRead = ReadDE(tempVoid, sampleframes, eSampleFloat32);
  if (format == eSampleFloat32)
  {
    float* pFloatBuffer = (float*)pBuffer;
    //just interleave samples
    for (uint32 c = 0; c < channels; c++)
    {
      for (uint32 s = 0; s < sampleFramesRead; s++)
      {
        pFloatBuffer[s * channels + c] = temp[c][s];
      }
    }
  }
  else
  {
    //16 bits int are required, so interleave samples and convert them into float
    int16* pInt16Buffer = (int16*)pBuffer;
    for (uint32 c = 0; c < channels; c++)
    {
      nuiAudioConvert_DEfloatToINint16(temp[c], pInt16Buffer, c, channels, sampleFramesRead);
    }
    
  }
  
  for (uint32 c= 0; c < channels; c++)
  {
    delete[] temp[c];
  }
  
  return sampleFramesRead;
}



//**************************************************************
//**************************************************************
//
//SamplesQueue
//
//**************************************************************
//**************************************************************

SamplesQueue::SamplesQueue() :
mUtilSize(0),
mInputBytesPerSample(1)
{
}

SamplesQueue::~SamplesQueue()
{
}

uint32 SamplesQueue::GetUtilSize() const
{
  return mUtilSize;
}

uint32 SamplesQueue::GetMemorySize() const
{
  return (uint32)mBuffer.size();
}

void SamplesQueue::Reset()
{
  mUtilSize = 0;
}

void SamplesQueue::Append(uint8* pSrc, uint32 size)
{
  uint32 oldUtilSize = mUtilSize;
  mUtilSize += size;
  if (mBuffer.size() < mUtilSize)
  {
    mBuffer.resize(mUtilSize);
  }
  memcpy(&(mBuffer[oldUtilSize]), pSrc, size);
}

uint32 SamplesQueue::ReadDeInterleavedFloat32(std::vector<float*>& rBuffers, uint32 FramesRequested)
{
  uint32 channels = (uint32)rBuffers.size();
  uint32 StoredSamples = mUtilSize / mInputBytesPerSample;
  uint32 StoredFrames = StoredSamples / channels;
  
  uint32 FramesToRead = MIN(FramesRequested, StoredFrames);
  uint32 SamplesToRead = FramesToRead * channels;
  uint32 BytesToRead = SamplesToRead * mInputBytesPerSample;
  
  float* pFloatBuffer = new float[SamplesToRead];
  
  if (mInputBytesPerSample == 1)
  {
    memcpy((int8*)pFloatBuffer + 3*SamplesToRead, &(mBuffer[0]), SamplesToRead);
    nuiAudioConvert_Signed8bitsBufferToFloat(pFloatBuffer, SamplesToRead);
  }
  else if (mInputBytesPerSample == 2)
  {
    memcpy((int16*)pFloatBuffer + SamplesToRead, &(mBuffer[0]), SamplesToRead * mInputBytesPerSample);
    nuiAudioConvert_16bitsBufferToFloat(pFloatBuffer, SamplesToRead);
  }
  else if (mInputBytesPerSample == 3)
  {
    for (uint32 i = 0; i < SamplesToRead; i++)
    {
      pFloatBuffer[i] = nuiAudioConvert_24bitsToFloatFromLittleEndian(&(mBuffer[3 * i]));
    }
  }
  else if (mInputBytesPerSample == 4)
  {
    nuiAudioConvert_32bitsToFloat((int32*)&(mBuffer[0]), pFloatBuffer, SamplesToRead);
  }
  
  for (uint32 s = 0; s < FramesToRead; s++)
  {
    for (uint32 c = 0; c < channels; c++)
    {
      rBuffers[c][s] = pFloatBuffer[s * channels + c];
    }
  }
  
  mUtilSize -= BytesToRead;
  if (mUtilSize > 0)
  {
    memmove(&(mBuffer[0]), &(mBuffer[BytesToRead]), mUtilSize);
  }
  
  delete[] pFloatBuffer;
  
  return FramesToRead;
}

void SamplesQueue::SetInputBytesPerSample(uint32 bytes)
{
  mInputBytesPerSample = bytes;
}

uint32 SamplesQueue::GetInputBytesPerSample() const
{
  return mInputBytesPerSample;
}




//**************************************************************
//**************************************************************
//
//nglWindowsMediaIStream
//
//**************************************************************
//**************************************************************


nglWindowsMediaIStream::nglWindowsMediaIStream(nglIStream& rStream) :
mrStream(rStream),
mRefCount(1)
{
}

nglWindowsMediaIStream::~nglWindowsMediaIStream(void)
{
}

//from IUnknown
HRESULT nglWindowsMediaIStream::QueryInterface(REFIID iid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
  if(ppvObject == NULL) 
  {
    return E_INVALIDARG;
  }
  
  *ppvObject = NULL;
  
  if(iid == IID_IUnknown)
  {
    *ppvObject = this;
  }
  else if(iid == IID_ISequentialStream)
  {
    *ppvObject = (ISequentialStream*)this;
  }
  else if(iid == IID_IStream)
  {
    *ppvObject = (IStream*)this;
  }
  
  
  if(*ppvObject == NULL) 
  {
    return E_NOINTERFACE;
  }
  
  AddRef();
  return S_OK;
}

ULONG nglWindowsMediaIStream::AddRef()
{
  mRefCount++;
  return mRefCount;
}

ULONG nglWindowsMediaIStream::Release()
{
  mRefCount--;
  
  if (mRefCount == 0)
  {
    delete this;
  }
  
  return mRefCount;
}

//from ISequentialStream
HRESULT nglWindowsMediaIStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
  int64 sizeRead = mrStream.ReadInt8((int8*)pv, cb);
  *pcbRead = (ULONG)sizeRead;
  return S_OK;
}

HRESULT nglWindowsMediaIStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
  return S_OK;
}

//from IStream
HRESULT nglWindowsMediaIStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
  nglStreamWhence fromWhere = eStreamFromStart;
  if (dwOrigin == STREAM_SEEK_SET)
  {
    fromWhere = eStreamFromStart;
  }
  else if (dwOrigin == STREAM_SEEK_CUR)
  {
    fromWhere = eStreamForward;
  }
  else if (dwOrigin = STREAM_SEEK_END)
  {
    fromWhere = eStreamFromEnd;
  }
  
  nglFileOffset move = dlibMove.QuadPart;
  mrStream.SetPos(move, fromWhere);
  nglFileOffset newPos = mrStream.GetPos();
  
  if (plibNewPosition)
    plibNewPosition->QuadPart = newPos;
  
  return S_OK;
}

HRESULT nglWindowsMediaIStream::SetSize(ULARGE_INTEGER libNewSize)
{
  return S_OK;
}

HRESULT nglWindowsMediaIStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
  return S_OK;
}

HRESULT nglWindowsMediaIStream::Commit(DWORD grfCommitFlags)
{
  return S_OK;
}

HRESULT nglWindowsMediaIStream::Revert()
{
  return S_OK;
}

HRESULT nglWindowsMediaIStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
  return S_OK;
}
HRESULT nglWindowsMediaIStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
  return S_OK;
}

HRESULT nglWindowsMediaIStream::Stat(__RPC__out STATSTG *pstatstg, DWORD grfStatFlag)
{
  if (!pstatstg)
    return STG_E_INVALIDPOINTER;
  
  LPOLESTR pName = NULL;
  if (grfStatFlag & STATFLAG_DEFAULT)
  {
    //fill the name
    WCHAR wname[256];
    swprintf(wname, L"0x%p", this);
    size_t length = wcslen(wname) + 1;
    pName = (LPOLESTR)CoTaskMemAlloc(length * sizeof(WCHAR));
    if (!pName)
      return STG_E_INSUFFICIENTMEMORY;
    memcpy(pName, wname, length);
  }
  else if (grfStatFlag & STATFLAG_NONAME)
  {
    pName = NULL;
  }
  else
  {
    return STG_E_INVALIDFLAG;
  }
  
  nglFileSize StreamSize = mrStream.Available(1);
  DWORD StorageType = STGTY_STORAGE;
  FILETIME Mtime;
  Mtime.dwHighDateTime = 0;
  Mtime.dwLowDateTime = 0;
  FILETIME Ctime;
  Ctime.dwHighDateTime = 0;
  Ctime.dwLowDateTime = 0;
  FILETIME Atime;
  Atime.dwHighDateTime = 0;
  Atime.dwLowDateTime = 0;
  DWORD AccessMode = 0;
  DWORD LockSupported = LOCK_WRITE;
  CLSID ClassID = CLSID_NULL;
  
  
  pstatstg->pwcsName = pName;
  pstatstg->type = StorageType;
  pstatstg->cbSize.QuadPart = StreamSize;
  pstatstg->mtime = Mtime;
  pstatstg->ctime = Ctime;
  pstatstg->atime = Atime;
  pstatstg->grfMode = AccessMode;
  pstatstg->grfLocksSupported = LockSupported;
  pstatstg->clsid = ClassID;
  
  
  
  return S_OK;
}

HRESULT nglWindowsMediaIStream::Clone(__RPC__deref_out_opt IStream **ppstm)
{
  return S_OK;
}