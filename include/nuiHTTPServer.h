/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiTCPServer.h"
#include "nuiTCPClient.h"
#include "nuiRegExp.h"

class nuiHTTPHandler;

class nuiURLHandler
{
public:
  typedef nuiFastDelegate2<const nglString&, std::vector<nglString>, nuiHTTPHandler* > HandlerDelegate;
  nuiURLHandler(const nglString& rRegExp, const HandlerDelegate& rDelegate);
  virtual ~nuiURLHandler();

  nuiHTTPHandler* Handle(const nglString& rURL);

  const nglString& GetRegExp() const;

private:
  nuiRegExp mRegExp;
  HandlerDelegate mDelegate;
};

class nuiURLDispatcher
{
public:
  nuiURLDispatcher();
  virtual ~nuiURLDispatcher();
  
  void AddHandler(nuiURLHandler* pHandler);
  void AddHandler(const nglString& rRegExp, const nuiURLHandler::HandlerDelegate& rDelegate);
  
  nuiHTTPHandler* Dispatch(const nglString& rURL);
  
private:
  std::vector<nuiURLHandler*> mpHandlers;
};

class nuiHTTPHandler
{
public:
  nuiHTTPHandler(nuiTCPClient* pClient);
  virtual ~nuiHTTPHandler();

  void SynchronousParse(); ///< Handle the http stream by blocking on read
  void ParseData(const std::vector<uint8>& rData); ///< Parse incomming data. This method doesn't block.

  virtual bool OnMethod(const nglString& rValue);
  virtual bool OnURL(const nglString& rValue);
  virtual bool OnProtocol(const nglString& rValue, const nglString rVersion);
  virtual bool OnHeader(const nglString& rKey, const nglString& rValue);
  virtual bool OnBodyStart();
  virtual bool OnBodyData(const std::vector<uint8>& rData);
  virtual void OnBodyEnd();
  bool ReplyLine(const nglString& rString);
  bool ReplyHeader(const nglString& rKey, const nglString& rValue);
  bool ReplyError(int32 code, const nglString& rErrorStr);
  bool Log(int32 code);


protected:
  enum State
  {
    Request,
    Header,
    Body
  };
  nuiTCPClient* mpClient;
  nglString mCurrentLine;
  nglString mURL;
  nglString mMethod;
  nglString mProtocol;
  nglString mVersion;
  State mState;

  std::map<nglString, nglString> mHeaders;
  std::vector<uint8> mBody;
};

class nuiHTTPServerThread : public nglThread
{
public:
  nuiHTTPServerThread(nuiHTTPHandler* pHandler, size_t StackSize = 0);
  virtual ~nuiHTTPServerThread();
  void OnStart();

private:
  nuiHTTPHandler* mpHandler;
};



class nuiHTTPServer : public nuiTCPServer
{
public:
  nuiHTTPServer();
  virtual ~nuiHTTPServer();

  void AcceptConnections();
  void OnNewClient(nuiTCPClient* pClient);
  void SetHandlerDelegate(const nuiFastDelegate1<nuiTCPClient*, nuiHTTPHandler*>& rDelegate);

  void SetClientStackSize(size_t StackSize);
  size_t GetClientStackSize() const;
protected:
  nuiFastDelegate1<nuiTCPClient*, nuiHTTPHandler*> mDelegate;

  nuiHTTPHandler* DefaultHandler(nuiTCPClient* pClient);
  
  size_t mClientStackSize;
};

