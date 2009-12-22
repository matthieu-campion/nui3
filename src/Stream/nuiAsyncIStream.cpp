/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiAsyncIStream.h"
#include "nglThread.h"
#include "nuiCommand.h"

class nuiAsyncIStream::Handler : public nglThread, public nuiCommand
{
public:
  Handler(nuiAsyncIStream* pStream, const nglString& rURL)
  : mpASStream(pStream), mURL(rURL), nuiCommand(_T("nuiAsynIStream::Handler"), _T("nuiAsynIStream::Handler command. It should only be used by the kernel"), false, false, false),
    mCompletion(0), mpStream(NULL)
  {
  }

  ~Handler()
  {
    delete mpStream;
  }

  void Cancel()
  {
    mCancel = true;
  }

  float GetCompletion() const
  {
    return mCompletion;
  }
  
  virtual nglIStream* DoStream() = 0;
protected:
  void OnStart()
  {
    mpStream = DoStream();

    if (mCancel)
    {
      SetAutoDelete(true);
      return;
    }

    nuiNotification* pNotif = new nuiNotification(_T("nuiAsyncIStream_Handler"));
    pNotif->SetToken(new nuiToken<nuiCommand*>(this, true));
    App->Post(pNotif);
  }

  bool SetArgs(const std::vector<nglString, std::allocator<nglString> >&)
  {
    return true;
  }

  bool ExecuteDo()
  {
    if (!mCancel)
      mpASStream->HandlerDone(mpStream);
    else
      delete mpStream;

    mpStream = NULL;
    return true;
  }

protected:
  nglString mURL;
  nuiAsyncIStream* mpASStream;
  bool mCancel;
  nglIStream* mpStream;
  float mCompletion;
};

class nuiAsyncIStream::FileHandler : public nuiAsyncIStream::Handler
{
public:
  FileHandler(nuiAsyncIStream* pASStream, const nglString& rURL)
  : nuiAsyncIStream::Handler(pASStream, rURL)
  {
    
  }
  
  nglIStream* DoStream()
  {
    return NULL;
  }
protected:
};

class nuiAsyncIStream::HttpHandler : public nuiAsyncIStream::Handler
{
public:
  HttpHandler(nuiAsyncIStream* pASStream, const nglString& rURL)
  : nuiAsyncIStream::Handler(pASStream, rURL)
  {
    
  }
  
  nglIStream* DoStream()
  {
    return NULL;
  }
protected:
};

nuiAsyncIStream::nuiAsyncIStream(const nglString& rURL, bool AutoStart)
: mURL(rURL), mpHandler(NULL), mpStream(NULL), mCancel(false)
{
  nglPath p(mURL);
  nglString protocol(p.GetVolumeName());

  if (protocol.IsEmpty())
  {
    protocol = _T("file");
  }

  if (protocol == _T("file"))
  {
    mpHandler = new FileHandler(this, mURL);
  }
  else if (protocol == _T("http") || protocol == _T("https"))
  {
    mpHandler = new HttpHandler(this, mURL);
  }
  else
  {
    // Error
  }

  if (AutoStart)
    Start();
}

nuiAsyncIStream::~nuiAsyncIStream()
{
  if (mpHandler)
  {
    if (IsDone())
    {
      delete mpHandler;
    }
    else
    {
      mpHandler->Cancel();
    }
  }
}

bool nuiAsyncIStream::Start()
{
  if (!mpHandler)
    return false;
  
  mpHandler->Start();
  return true;
}

bool nuiAsyncIStream::IsDone() const
{
  if (mCancel || !mpHandler)
    return true;
  return mpHandler->IsDone();
}

float nuiAsyncIStream::GetCompletion() const
{
  return mCompletion;
}

nglStreamState nuiAsyncIStream::GetState() const
{
  if (mpStream)
    return mpStream->GetState();
  return eStreamError;
}

nglFileOffset nuiAsyncIStream::GetPos() const
{
  if (mpStream)
    return mpStream->GetPos();
  return 0;
}

nglFileOffset nuiAsyncIStream::SetPos(nglFileOffset Where, nglStreamWhence Whence)
{
  if (mpStream)
    return mpStream->SetPos(Where, Whence);
  return 0;
}

nglFileSize nuiAsyncIStream::Available(uint WordSize)
{
  if (mpStream)
    return mpStream->Available(WordSize);
  return 0;
}

int64 nuiAsyncIStream::Read(void* pData, int64 WordCount, uint WordSize)
{
  if (mpStream)
    return mpStream->Read(pData, WordCount, WordSize);
  return 0;
}

void nuiAsyncIStream::HandlerDone(nglIStream* pStream)
{
  mpStream = pStream;
}


