/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

class nuiHTTPResponse;

typedef std::map<nglString, nglString, nglString::CaseInsensitiveLessFunctor> nuiHTTPHeaderMap;
class nuiHTTPMessage
{
public:
  virtual ~nuiHTTPMessage();

  const nuiHTTPHeaderMap& GetHeaders() const;
  nglString GetHeadersRep() const; ///< Return a representation of headers as a multi-line string. 
  void AddHeader(const nglString& rFieldName, const nglString& rFieldValue); ///< Add new or replace existing header.
  bool AddHeader(const nglString& rHeader); ///< Add new or replace existing header. String must be of form "<field-name>: <field-value>" as per HTTP 1.1 specification (IETF RFC 2616). Returns true in case of success.

  void SetBody(const char* pBuffer, nglSize ByteCnt);
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
  void SendRequest(const Delegate& rDelegate);

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

