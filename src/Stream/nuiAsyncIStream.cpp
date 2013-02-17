/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

class nuiAsyncIStream::Handler : public nglThread, public nuiCommand
{
public:
  Handler(nuiAsyncIStream* pStream, const nglString& rURL)
  : mpASStream(pStream), mURL(rURL), nuiCommand(_T("nuiAsynIStream::Handler"), _T("nuiAsynIStream::Handler command. It should only be used by the kernel"), false, false, false),
    mCompletion(0), mpStream(NULL), mCancel(false)
  {
    mURL.DecodeUrl();
  }

  virtual ~Handler()
  {
    Cancel();
    delete mpStream;
    NGL_LOG(_T("nuiAsyncIStream"), NGL_LOG_DEBUG, _T("Handler dtor"));
  }

  void Cancel()
  {
    mCancel = true;
    NGL_OUT(_T("\tCanceling nuiAsyncIStream::Handler 0x%p\n"), this);
  }

  float GetCompletion() const
  {
    return mCompletion;
  }
  
  virtual nglIStream* DoStream() = 0;
  virtual const nuiHTTPResponse* GetHTTPResponse() const
  {
    return NULL;
  }
protected:
  void OnStart()
  {
    mpStream = DoStream();

    if (mCancel)
    {
      SetAutoDelete(true);
      NGL_LOG(_T("nuiAsyncIStream::Handler"), NGL_LOG_DEBUG, _T("Cancel (auto delete thread)"));
      return;
    }

    nuiNotification* pNotif = new nuiNotification(_T("nuiAsyncIStream_Handler"));
    pNotif->SetToken(new nuiToken<nuiCommand*>(this, true));
    App->Post(pNotif);
    NGL_LOG(_T("nuiAsyncIStream::Handler"), NGL_LOG_DEBUG, _T("Posted notification"));
  }

  bool SetArgs(const std::vector<nglString, std::allocator<nglString> >&)
  {
    return true;
  }

  bool ExecuteDo()
  {
    NGL_LOG(_T("nuiAsyncIStream::Handler"), NGL_LOG_DEBUG, _T("ExecuteDo"));
    if (!mCancel)
      mpASStream->HandlerDone(mpStream);

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
    nglString url(mURL);
    if (!url.CompareLeft(_T("file://"), false))
      url.DeleteLeft(7);
    url.TrimLeft('/');
    if (url.IsEmpty())
      url = _T("/");
    else
    {
      nglPath p(url);
      if (p.GetVolumeName().IsEmpty())
        url.Insert('/', 0);
    }
      
    nglPath p(url);
    if (!p.Exists())
      return NULL;
    
    if (p.IsLeaf())
      return p.OpenRead();

    // This is a directory, let's fake some HTML page:
    
    nglString dir;
    dir.CFormat(_T("<p>Directory %s:</p><br>\n"), p.GetChars());
    
    dir.Add("<a href=\"").Add(p.GetParent().GetChars()).Add("\">Parent Folder</a><br>\n");

    std::list<nglPath> children;
    p.GetChildren(children);
    
    std::list<nglPath>::const_iterator it = children.begin();
    std::list<nglPath>::const_iterator end = children.end();

    int32 count = 0;
    while (it != end)
    {
      nglString l;
      nglPath c(*it);
      if (c.IsLeaf())
        l.CFormat(_T("<a href=\"%s\">%s</a> %d bytes<br>\n"), c.GetChars(), c.GetNodeName().GetChars(), c.GetSize());
      else
        l.CFormat(_T("<a href=\"%s\">%s</a> (folder)<br>\n"), c.GetChars(), c.GetNodeName().GetChars());
      dir += l;
      count++;
      ++it;
    }
    
    dir.Add("<p>").Add(count).Add(" items</p><br>\n");
    
    char* pChars = dir.Export();
    nglIMemory* pMem = new nglIMemory(pChars, strlen(pChars));
    return pMem;
  }
protected:
};

class nuiAsyncIStream::HttpHandler : public nuiAsyncIStream::Handler
{
public:
  HttpHandler(nuiAsyncIStream* pASStream, const nglString& rURL)
  : nuiAsyncIStream::Handler(pASStream, rURL), mpResponse(NULL)
  {
    
  }
  
  virtual ~HttpHandler()
  {
    delete mpResponse;
  }
  
  nglIStream* DoStream()
  {
    nuiHTTPRequest request(mURL);
    mpResponse = request.SendRequest();
    if (!mpResponse)
      return NULL;
    
    nglIMemory* pMem = new nglIMemory(&mpResponse->GetBody()[0], mpResponse->GetBody().size());
    return pMem;
  }
  
  virtual const nuiHTTPResponse* GetHTTPResponse() const
  {
    return mpResponse;
  }
  
protected:
  nuiHTTPResponse* mpResponse;
};

nuiAsyncIStream::nuiAsyncIStream(const nglString& rURL, bool AutoStart, const nuiSignal1<nuiAsyncIStream*>::Slot& rDelegate)
: mURL(rURL), mpHandler(NULL), mpStream(NULL), mCancel(false)
{
  mSlotSink.Connect(StreamReady, rDelegate);
  nglPath p(mURL);
  nglString protocol(p.GetVolumeName());

  if (protocol.IsEmpty() && mURL.Compare(_T("file:///"), false) != 0)
  {
    protocol = _T("file");
    mURL.TrimLeft(_T('/'));
    mURL.Insert(_T("://"), 0);
    mURL.Insert(protocol, 0);
    p = mURL;
  }

  if (protocol == _T("file"))
  {
    mpHandler = new FileHandler(this, mURL);
  }
  else if (protocol == _T("http") || protocol == _T("https") || protocol == _T("shttp"))
  {
    mpHandler = new HttpHandler(this, mURL);
  }
  else
  {
    // Error
    NGL_ASSERT(0);
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
      Cancel();
    }
  }
}

const nglString& nuiAsyncIStream::GetURL() const
{
  return mURL;
}

bool nuiAsyncIStream::Start()
{
  if (!mpHandler)
    return false;

  if(mCancel)
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
  NGL_LOG(_T("nuiAsyncIStream"), NGL_LOG_DEBUG, _T("HandlerDone"));
  mpStream = pStream;
  StreamReady(this);
}


const nuiHTTPResponse* nuiAsyncIStream::GetHTTPResponse() const
{
  if (!mpHandler)
    return NULL;
  return mpHandler->GetHTTPResponse();
}

void nuiAsyncIStream::Cancel()
{
  NGL_OUT(_T("Canceling nuiAsyncIStream 0x%p"), this);
  mCancel = true;
  if (mpHandler)
    mpHandler->Cancel();
}
