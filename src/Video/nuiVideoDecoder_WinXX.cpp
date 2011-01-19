#include "nui.h"
#include "nuiVideoDecoder.h"

#include "windows.h"
// If visual studio complains about this header you need to update to the latest windows SDK
// http://www.microsoft.com/downloads/details.aspx?FamilyID=4377f86d-c913-4b5c-b87e-ef72e5b4e065&displaylang=en
#include "wmsdk.h" 
#include "objidl.h"

#pragma comment (lib, "wmvcore.lib")

class nglVideoWindowsMediaIStream : public IStream
  {
  public:
    nglVideoWindowsMediaIStream(nglIStream& rStream);
    virtual ~nglVideoWindowsMediaIStream();
    
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



class nuiVideoDecoderPrivate
{
public:
	nglVideoWindowsMediaIStream* mpWMStream;
  IWMSyncReader* mpWMReader;
	nglIStream* mpStream;
	WORD mStreamNumber;
  QWORD mPosition;
  BYTE* mpBuffer;
};

nuiVideoDecoder::nuiVideoDecoder(const nglPath& path)
: mDuration(0),
  mWidth(0),
  mHeight(0),
  mRate(0),
  mPath(path),
  mpImage(NULL),
  mpTexture(NULL)
{
  Init();
}

nuiVideoDecoder::~nuiVideoDecoder()
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

  if (mpPrivate->mpStream)
	  delete mpPrivate->mpStream;
  
  CoUninitialize();

  delete[] mpPrivate->mpBuffer;
  
  if (mpPrivate)
    delete mpPrivate;
}

bool nuiVideoDecoder::Init()
{
  CoInitialize(NULL);
  HRESULT hr = S_OK;

  mpPrivate = new nuiVideoDecoderPrivate();
  mpPrivate->mpStream = NULL;
  mpPrivate->mpWMStream = NULL;
  mpPrivate->mpWMReader = NULL;
  mpPrivate->mpBuffer = NULL;

  mpPrivate->mpStream = mPath.OpenRead();
  if (!mpPrivate->mpStream)
	  return false;
  mpPrivate->mpWMStream = new nglVideoWindowsMediaIStream(*(mpPrivate->mpStream));

  hr = WMCreateSyncReader(NULL, WMT_RIGHT_PLAYBACK, &(mpPrivate->mpWMReader));
  if (SUCCEEDED(hr))
  {
    hr = mpPrivate->mpWMReader->OpenStream(mpPrivate->mpWMStream);
  }

  if (FAILED(hr))
  {
	  if (mpPrivate->mpWMReader)
	  {
		mpPrivate->mpWMReader->Close();
		mpPrivate->mpWMReader->Release();
		mpPrivate->mpWMReader = NULL;
	  }
	  return false;
  }

  bool ok = false;
  IWMSyncReader* pReader = mpPrivate->mpWMReader;
  DWORD outputCount = 0;
  hr = pReader->GetOutputCount(&outputCount);
  for (uint32 output = 0; output < outputCount && !ok; output++)
  {
	  DWORD formatCount = 0;
	  hr = pReader->GetOutputFormatCount(output, &formatCount);
	  for (uint32 format = 0; format < formatCount && !ok; format++)
	  {
		  IWMOutputMediaProps* pMediaProps = NULL;
		  hr = pReader->GetOutputFormat(output, format, &pMediaProps);


		  //
		  DWORD AllocSize = 0;
		  hr = pMediaProps->GetMediaType(NULL, &AllocSize);        
        
		  WM_MEDIA_TYPE* pType = (WM_MEDIA_TYPE*)malloc(AllocSize);
		  hr = pMediaProps->GetMediaType(pType, &AllocSize);

		  if ((pType->majortype != WMMEDIATYPE_Video) || (pType->formattype != WMFORMAT_VideoInfo))
			  continue;

		  if (pType->subtype != WMMEDIASUBTYPE_RGB24)
			  continue;

		  WMVIDEOINFOHEADER* pVideoInfo = (WMVIDEOINFOHEADER*)pType->pbFormat;

		  BITMAPINFOHEADER& rBitmapInfo = pVideoInfo->bmiHeader;
		  mWidth = rBitmapInfo.biWidth;
		  mHeight = rBitmapInfo.biHeight;
		  DWORD compression = rBitmapInfo.biCompression;
		  WORD bitCount = rBitmapInfo.biBitCount;

		  if (rBitmapInfo.biCompression != BI_RGB || rBitmapInfo.biBitCount != 24)
			  continue;

		  hr = pReader->SetOutputProps(output, pMediaProps);			
		  if (FAILED(hr))
      {
			  printf("can't set output props\n");
        continue;
      }

		  hr = pReader->GetStreamNumberForOutput(output, &mpPrivate->mStreamNumber);
		  WMT_STREAM_SELECTION streamSelection= WMT_ON;
		  hr = pReader->SetStreamsSelected(1, &mpPrivate->mStreamNumber, &streamSelection);
		  if (FAILED(hr))
      {
			  printf("can't set stream selected\n");
        continue;
      }

		  hr = pReader->SetReadStreamSamples(mpPrivate->mStreamNumber, FALSE);
		  if (FAILED(hr))
      {
			  printf("can't set read stream samples\n");
        continue;
      }
		  ok = true;
	  }
  }

  mpPrivate->mpBuffer = new BYTE[mWidth * mHeight * 3];
  memset(mpPrivate->mpBuffer, 0, mWidth * mHeight * 3 * sizeof(BYTE));
  GoToNextFrame();

  return ok;
}
  
bool nuiVideoDecoder::IsValid() const
{
	if (!mpPrivate->mpStream)
		return false;

	if (!mpPrivate->mpWMReader)
		return false;

	if (!mpPrivate->mpWMStream)
		return false;

	return true;
}

double nuiVideoDecoder::GetPosition() const
{
  return mpPrivate->mPosition;
}

void nuiVideoDecoder::SetPosition(double TimePosition)
{
	double onesecond = 10000000; // unit is 100 nanoseconds
	double StartTime = TimePosition * onesecond;
	HRESULT hr = mpPrivate->mpWMReader->SetRange(StartTime, 0/*no duration set*/);
	if (FAILED(hr))
		printf("can't set position");

  mpPrivate->mPosition = TimePosition;
}
  
bool nuiVideoDecoder::GoToNextFrame()
{
  HRESULT hr = S_OK;
  INSSBuffer* pTempBuffer = NULL;
  QWORD SampleDuration = 0;
  DWORD flags = 0;
  hr = mpPrivate->mpWMReader->GetNextSample(mpPrivate->mStreamNumber, &pTempBuffer, &mpPrivate->mPosition, &SampleDuration, &flags, NULL, NULL);
  if (FAILED(hr))
    return false;

  BYTE* pBuffer = NULL;
  DWORD length = 0;
  pTempBuffer->GetBufferAndLength(&pBuffer, &length);

  NGL_ASSERT(length == (mWidth * mHeight * 3));
  memcpy(mpPrivate->mpBuffer, pBuffer, length);

  pTempBuffer->Release();
	return true;
}
  
nglImage* nuiVideoDecoder::UpdateImage()
{
  if (!mpImage)
  {
    nglImageInfo info(false/*buffer not managed*/);
	  info.mWidth = mWidth;
	  info.mHeight = mHeight;

	  info.mBitDepth = 24;
	  info.mPixelFormat = eImagePixelBGR;

	  info.mBufferFormat = eImageFormatRaw;

	  info.mBytesPerPixel = (info.mBitDepth + 1) / 8;
	  info.mBytesPerLine = info.mWidth * info.mBytesPerPixel;	
    info.mpBuffer = (char*)(mpPrivate->mpBuffer);
	  
    mpImage = new nglImage(info);
  }
  else
  {
    char* pImgInfoBuffer = mpImage->GetBuffer();
    memcpy(pImgInfoBuffer, mpPrivate->mpBuffer, mWidth * mHeight * 3 * sizeof(BYTE));
  }

	return mpImage;
}

nuiTexture* nuiVideoDecoder::UpdateTexture()
{
  if (!mpTexture)
  {
	  nglImageInfo info(false/*buffer not managed*/);
	  info.mWidth = mWidth;
	  info.mHeight = mHeight;

	  info.mBitDepth = 24;
	  info.mPixelFormat = eImagePixelBGR;

	  info.mBufferFormat = eImageFormatRaw;

	  info.mBytesPerPixel = (info.mBitDepth + 1) / 8;
	  info.mBytesPerLine = info.mWidth * info.mBytesPerPixel;	
    info.mpBuffer = (char*)(mpPrivate->mpBuffer);
	  mpTexture = nuiTexture::GetTexture(info);
  }
  else
  {
	  char* pImgInfoBuffer = mpTexture->GetImage()->GetBuffer();
    memcpy(pImgInfoBuffer, mpPrivate->mpBuffer, mWidth * mHeight * 3 * sizeof(BYTE));
	  mpTexture->ForceReload();
  }

  return mpTexture;
}













//**************************************************************
//**************************************************************
//
//nglVideoWindowsMediaIStream
//
//**************************************************************
//**************************************************************


nglVideoWindowsMediaIStream::nglVideoWindowsMediaIStream(nglIStream& rStream) :
mrStream(rStream),
mRefCount(1)
{
}

nglVideoWindowsMediaIStream::~nglVideoWindowsMediaIStream(void)
{
}

//from IUnknown
HRESULT nglVideoWindowsMediaIStream::QueryInterface(REFIID iid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
  if(ppvObject == NULL) 
  {
    return E_INVALIDARG;
  }
  
  *ppvObject = NULL;

  GUID unknownID;
  CLSIDFromString(L"{94BC0598-C3D2-11D3-BEDF-00C04F612986}", &unknownID);
 
  
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
  else if (iid == IID_IWMIStreamProps)
  {
	  return E_NOTIMPL;
  }
  else if (iid == unknownID)
  {
	  return E_NOTIMPL;
  }
  else
  {
	  return E_NOINTERFACE;
  }
  
  AddRef();
  return S_OK;
}

ULONG nglVideoWindowsMediaIStream::AddRef()
{
  mRefCount++;
  return mRefCount;
}

ULONG nglVideoWindowsMediaIStream::Release()
{
  mRefCount--;
  
  if (mRefCount == 0)
  {
    delete this;
  }
  
  return mRefCount;
}

//from ISequentialStream
HRESULT nglVideoWindowsMediaIStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
  int64 sizeRead = mrStream.ReadInt8((int8*)pv, cb);
  *pcbRead = (ULONG)sizeRead;
  return S_OK;
}

HRESULT nglVideoWindowsMediaIStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
  return S_OK;
}

//from IStream
HRESULT nglVideoWindowsMediaIStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
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

HRESULT nglVideoWindowsMediaIStream::SetSize(ULARGE_INTEGER libNewSize)
{
  return S_OK;
}

HRESULT nglVideoWindowsMediaIStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
  return S_OK;
}

HRESULT nglVideoWindowsMediaIStream::Commit(DWORD grfCommitFlags)
{
  return S_OK;
}

HRESULT nglVideoWindowsMediaIStream::Revert()
{
  return S_OK;
}

HRESULT nglVideoWindowsMediaIStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
  return S_OK;
}
HRESULT nglVideoWindowsMediaIStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
  return S_OK;
}

HRESULT nglVideoWindowsMediaIStream::Stat(__RPC__out STATSTG *pstatstg, DWORD grfStatFlag)
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

HRESULT nglVideoWindowsMediaIStream::Clone(__RPC__deref_out_opt IStream **ppstm)
{
  return S_OK;
}