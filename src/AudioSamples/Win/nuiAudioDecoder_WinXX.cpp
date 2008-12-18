#include "nui.h"
#include "nuiAudioDecoder.h"

#include "windows.h"
#include "wmsdk.h"
#include "objidl.h"

#define PCM_AUDIO_FORMAT_TAG 0x0001

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
  uint32 Read(uint8* pDest, uint32 size);

private:
  std::vector<uint8> mBuffer;
  uint32 mUtilSize;
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

bool nuiAudioDecoder::Init()
{
  if (mInitialized) //already initialized
	return false;

  CoInitialize(NULL);

  mpPrivate = new nuiAudioDecoderPrivate();


  bool result = false;
  HRESULT hr = S_OK;
  mpPrivate->mpWMStream = new nglWindowsMediaIStream(mrStream);

  hr = WMCreateSyncReader(NULL, WMT_RIGHT_PLAYBACK, &mpPrivate->mpWMReader);
  if (SUCCEEDED(hr))
  {
	hr = mpPrivate->mpWMReader->OpenStream(mpPrivate->mpWMStream);
	if (SUCCEEDED(hr))
	{
	  result = ReadInfo();
	  SetPosition(0);
	}
  }

  return result;
}

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

bool nuiAudioDecoder::Seek(uint64 SampleFrame)
{
  QWORD StartTime = SampleFrame / mInfo.GetSampleRate() * ONE_SECOND;
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
  bool result = false;
  HRESULT hr = S_OK;
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
		  WORD nbChannels = pWFmt->nChannels;			//retrieve the number of channels
		  DWORD SampleRate = pWFmt->nSamplesPerSec;		//retrieve the sample rate
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
		  mInfo.SetBitsPerSample(bitsPerSample);
		  mInfo.SetSampleFrames(SampleFrames);

		  free(pType);
		  result = true;
		  break; //we have found the right output and format, and yhe Sample info object is filled
		}
	  }

	}
  }

  return result;
}

uint32 nuiAudioDecoder::Read(std::vector<float*> buffers, uint32 SampleFrames)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
	return 0;

  return 0;
  //it doesn't work for the moment
  //I must find a way to get non-interleaved buffers 

//   uint32 BytesToRead		= 0;
//   SampleFramesToBytes(SampleFrames, BytesToRead);
//   HRESULT hr				= S_OK;
//   INSSBuffer* pTempBuffer	= NULL;
//   QWORD SampleTime			= 0;
//   QWORD SampleDuration		= 0;
//   DWORD flags				= 0;
//   DWORD outputNum			= 0;
// 
//   while (SUCCEEDED(hr) && mpPrivate->mQueue.GetUtilSize() < BytesToRead)
//   {
// 	hr = mpPrivate->mpWMReader->GetNextSample(0, &pTempBuffer, &SampleTime, &SampleDuration, &flags, &outputNum, NULL);
// 
// 	if (SUCCEEDED(hr))
// 	{
// 	  BYTE* pBytes = NULL;
// 	  DWORD length = 0;
// 	  hr = pTempBuffer->GetBufferAndLength(&pBytes, &length);
// 	  if (SUCCEEDED(hr))
// 	  {
// 		mpPrivate->mQueue.Append((uint8*)pBytes, length);
// 	  }
// 	  pTempBuffer->Release();
// 	  pTempBuffer = NULL;
// 	}
// 
// 	SampleTime		  = 0;
// 	SampleDuration	  = 0;
// 	flags			  = 0;
//   }
// 
//   //
//   //#FIXME: de-interlace buffers or find a way to say to WMReader to give non-interleaved buffers
//   //
//   //
//   uint32 BytesRead	= mpPrivate->mQueue.Read((uint8*)pBuffer, BytesToRead);
//   uint32 FramesRead = 0;
//   BytesToSampleFrames(BytesRead, FramesRead);
//   mPosition += FramesRead;
// 
//   return FramesRead;
}



//**************************************************************
//**************************************************************
//
//SamplesQueue
//
//**************************************************************
//**************************************************************

SamplesQueue::SamplesQueue() :
mUtilSize(0)
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
  return mBuffer.size();
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

uint32 SamplesQueue::Read(uint8* pDest, uint32 size)
{
  uint32 SizeToRead = MIN(size, mUtilSize);
  memcpy(pDest, &(mBuffer[0]), SizeToRead);

  mUtilSize -= SizeToRead;
  if (mUtilSize > 0)
  {
	memmove(&(mBuffer[0]), &(mBuffer[SizeToRead]), mUtilSize);
  }

  return SizeToRead;
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
  *pcbRead = sizeRead;
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
	swprintf(wname, L"0x%x", this);
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