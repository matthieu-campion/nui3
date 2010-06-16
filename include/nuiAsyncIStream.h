/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSignalsSlots.h"

class nuiHTTPResponse;

class nuiAsyncIStream : public nglIStream
{
public:
  nuiAsyncIStream(const nglString& rURL, bool AutoStart, const nuiSignal1<nuiAsyncIStream*>::Slot& rDelegate);
  
  ~nuiAsyncIStream();
  
  const nglString& GetURL() const;
  
  bool Start();
  bool IsDone() const;
  float GetCompletion() const;
  nglStreamState GetState() const;
  nglFileOffset GetPos() const;
  nglFileOffset SetPos(nglFileOffset Where, nglStreamWhence Whence);
  nglFileSize Available(uint WordSize);
  int64 Read(void* pData, int64 WordCount, uint WordSize);
  
  nuiSignal1<nuiAsyncIStream*> StreamReady;
  
  const nuiHTTPResponse* GetHTTPResponse() const;
  
  void Cancel();
protected:
  class Handler;
  class FileHandler;
  class HttpHandler;
  friend class Handler;
  friend class FileHandler;
  friend class HttpHandler;
  void HandlerDone(nglIStream* pStream);
  
  Handler* mpHandler;
  nglString mURL;
  nglIStream* mpStream;

  bool mCancel;
  
  float mCompletion;
  
  nuiSlotsSink mSlotSink;
};
