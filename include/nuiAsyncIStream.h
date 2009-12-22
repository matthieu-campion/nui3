/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

class nuiAsyncIStream : public nglIStream
{
public:
  nuiAsyncIStream(const nglString& rURL, bool AutoStart);
  
  ~nuiAsyncIStream();
  bool Start();
  bool Join();
  bool IsDone() const;
  float GetCompletion() const;
  nglStreamState GetState() const;
  uint GetError() const;
  const nglChar* GetErrorStr(uint Error) const;
  nglFileOffset GetPos() const;
  nglFileOffset SetPos(nglFileOffset Where, nglStreamWhence Whence);
  nglFileSize Available(uint WordSize);
  int64 Read(void* pData, int64 WordCount, uint WordSize);
  
protected:
  class Handler;
  friend class Handler;
  void HandlerDone();
  
  Handler* mpHandler;
  nglString mURL;
  nglIMemory* mpMemory;
  
};
