/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglThread.h"
#include "nuiCommand.h"

class nuiHTTPResponse;
class nuiHTTPRequest_Thread;

typedef std::map<nglString, nglString, nglString::CaseInsensitiveLessFunctor> nuiHTTPHeaderMap;
typedef nuiFastDelegate1<nuiHTTPResponse*> nuiHTTPResponseReceivedDelegate;
typedef nuiFastDelegate2<char*, nglSize, bool> nuiHTTPDataReceivedDelegate;

class nuiHTTPMessage
{
public:
  virtual ~nuiHTTPMessage();

  const nuiHTTPHeaderMap& GetHeaders() const;
  nglString GetHeadersRep() const; ///< Return a representation of headers as a multi-line string. 
  void AddHeader(const nglString& rFieldName, const nglString& rFieldValue); ///< Add new or replace existing header.
  bool AddHeader(const nglString& rHeader); ///< Add new or replace existing header. String must be of form "<field-name>: <field-value>" as per HTTP 1.1 specification (IETF RFC 2616). Returns true in case of success.

  void SetBody(const char* pBuffer, nglSize ByteCnt);
  void AddToBody(const char* pBuffer, nglSize ByteCnt);
  const std::vector<char>& GetBody() const;
  nglString GetBodyStr() const;

protected:
  nuiHTTPMessage();

  nuiHTTPHeaderMap mHeaders;
  std::vector<char> mBody;
};

class nuiHTTPRequest : public nuiHTTPMessage
{
public:
  typedef nuiFastDelegate2<nuiHTTPRequest*, nuiHTTPResponse*> Delegate;
  nuiHTTPRequest(const nglString& rUrl, const nglString& rMethod = _T("GET"));
  virtual ~nuiHTTPRequest();

  nuiHTTPResponse* SendRequest();
  nuiHTTPResponse* SendRequest(const nuiHTTPResponseReceivedDelegate& rResponseReceived, const nuiHTTPDataReceivedDelegate& rDataReceived);
  nuiHTTPRequest_Thread* SendRequest(const Delegate& rDelegate);

  const nglString& GetURL() const;
  const nglString& GetMethod() const;
protected:
  nglString mUrl;
  nglString mMethod;
};

class nuiHTTPResponse : public nuiHTTPMessage
{
public:
  nuiHTTPResponse(uint16 StatusCode, const nglString& StatusLine);
  virtual ~nuiHTTPResponse();

  uint16 GetStatusCode() const;
  const nglString& GetStatusLine() const;

private:
  uint16 mStatusCode;
  nglString mStatusLine;
};

class nuiHTTPRequest_Thread : public nglThread, public nuiCommand
{
public:
  void Cancel();
  
protected:
  nuiHTTPRequest_Thread(nuiHTTPRequest* pRequest, const nuiHTTPRequest::Delegate& rDelegate);
  ~nuiHTTPRequest_Thread();
  
  void OnStart();
  bool SetArgs(const std::vector<nglString, std::allocator<nglString> >&);
  bool ExecuteDo();
  
private:
  nuiHTTPRequest* mpRequest;
  nuiHTTPRequest::Delegate mDelegate;
  nuiHTTPResponse* mpResponse;
  bool mCancel;
  
  friend class nuiHTTPRequest;
};

