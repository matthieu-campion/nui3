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
  Handler(nuiAsyncIStream* pStream, const nglString& rURL)
  : mpStream(pStream), mURL(rURL), nuiCommand(_T("nuiAsynIStream::Handler"), _T("nuiAsynIStream::Handler command. It should only be used by the kernel"), false, false, false)
  {
  }
  
  void Cancel()
  {
    mCancel = true;
  }

  virtual bool DoStream() = 0;
protected:
  void OnStart()
  {
    DoStream();
    
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
      mpStream->HandlerDone();
    return true;
  }
  
protected:
  nglString mURL;
  nuiAsyncIStream* mpStream;
  bool mCancel;
};

nuiAsyncIStream::nuiAsyncIStream(const nglString& rURL, bool AutoStart)
: mURL(rURL), mpHandler(NULL), mpMemory(NULL)
{
  nglPath p(mURL);
  nglString protocol(p.GetVolumeName());

  if (protocol.IsEmpty())
  {
    
  }
  
  if (AutoStart)
    Start();
}

nuiAsyncIStream::~nuiAsyncIStream()
{
  delete mpHandler;
}

bool nuiAsyncIStream::Start()
{
  return false;
}

bool nuiAsyncIStream::Join()
{
  return false;
}

bool nuiAsyncIStream::IsDone() const
{
  return false;
}

float nuiAsyncIStream::GetCompletion() const
{
  return 0;
}

nglStreamState nuiAsyncIStream::GetState() const
{
  if (mpMemory)
    return mpMemory->GetState();
  return eStreamError;
}

uint nuiAsyncIStream::GetError() const
{
  if (mpMemory)
    return mpMemory->GetError();
  return 0;
}

const nglChar* nuiAsyncIStream::GetErrorStr(uint Error) const
{
  return _T("Unknown Error");
}

nglFileOffset nuiAsyncIStream::GetPos() const
{
  if (mpMemory)
    return mpMemory->GetPos();
  return 0;
}

nglFileOffset nuiAsyncIStream::SetPos(nglFileOffset Where, nglStreamWhence Whence)
{
  if (mpMemory)
    return mpMemory->SetPos(Where, Whence);
  return 0;
}

nglFileSize nuiAsyncIStream::Available(uint WordSize)
{
  if (mpMemory)
    return mpMemory->Available(WordSize);
  return 0;
}

int64 nuiAsyncIStream::Read(void* pData, int64 WordCount, uint WordSize)
{
  if (mpMemory)
    return mpMemory->Read(pData, WordCount, WordSize);
  return 0;
}

void nuiAsyncIStream::HandlerDone()
{
  
}


