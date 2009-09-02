/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiHTTP.h"

// If this header is missing update your windows SDK to the latest:
// http://www.microsoft.com/downloads/details.aspx?FamilyID=4377f86d-c913-4b5c-b87e-ef72e5b4e065&displaylang=en
#include "WinHttp.h"

using namespace std;

nuiHTTPResponse* nuiHTTPRequest::SendRequest()
{
  // parse address string
  nglString url(mUrl);
  nglString server, objectName;
  uint port = INTERNET_DEFAULT_PORT;
  bool secure = false; // WINHTTP_FLAG_SECURE

  if (url.CompareLeft(_T("http://")) == 0)
  {
    url.DeleteLeft(7);
    port = INTERNET_DEFAULT_HTTP_PORT;
  }
  else if (url.CompareLeft(_T("https://")) == 0)
  {
    url.DeleteLeft(8);
    secure = true;
    port = INTERNET_DEFAULT_HTTPS_PORT;
  }

  int32 pos = url.Find(':');
  if (pos != -1)
  {
    server = url.Extract(0, pos);
    port = url.Extract(pos).GetCInt();
    pos = url.Find('/');
    if (pos != -1)
    {
      objectName = url.Extract(pos);
    }
    else
    {
      objectName = nglString::Null;
    }
  }
  else
  {
    pos = url.Find('/');
    if (pos != -1)
    {
      server = url.Extract(0, pos);
      objectName = url.Extract(pos);
    }
    else
    {
      server = url;
      objectName = nglString::Null;
    }
  }

  HINTERNET hSession = WinHttpOpen(L"nuiHTTP/2.0", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
  if (!hSession)
    return NULL;

  HINTERNET hConnect = WinHttpConnect(hSession, server.GetChars(), port, 0);
  if(!hConnect)
    return NULL;

  LPCWSTR types[] = {L"*/*", NULL};
  HINTERNET hRequest = WinHttpOpenRequest(hConnect, mMethod.GetChars(), objectName.GetChars(), NULL, NULL, types, secure ? WINHTTP_FLAG_SECURE : 0);

  nglString headers = GetHeadersRep();
  WinHttpAddRequestHeaders(hRequest, headers.GetChars(), -1L, WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);

  bool success;
  if (mBody.size())
  {
    success = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, &mBody[0], mBody.size(), mBody.size(), NULL);
  }
  else
  {
    success = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, NULL, 0, 0, NULL);
  }

  if (!success)
    return NULL;

  success = WinHttpReceiveResponse(hRequest, NULL);
  if (!success)
    return NULL;

  vector<char> buf;
  DWORD size = 0;
  WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, WINHTTP_NO_OUTPUT_BUFFER, &size, WINHTTP_NO_HEADER_INDEX);
  buf.resize(size);
  WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, &buf[0], &size, WINHTTP_NO_HEADER_INDEX);
  nglString statusCodeLine(&buf[0], buf.size(), eUCS2);
  uint16 statusCode = statusCodeLine.GetCInt();

  nglString statusLine;

  // retrieve the headers size then retrieve the headers as chars separated by NULL
  size = 0;
  WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, WINHTTP_NO_OUTPUT_BUFFER, &size, WINHTTP_NO_HEADER_INDEX);
  buf.resize(size);
  WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, &buf[0], &size, WINHTTP_NO_HEADER_INDEX);
  nglString rawHeaders(&buf[0], buf.size(), eUCS2);
  vector<nglString> headersVec;
  rawHeaders.Tokenize(headersVec, _T("\r\n"));

  vector<nglString>::const_iterator it = headersVec.begin();
  vector<nglString>::const_iterator end = headersVec.end();
  if (it != end)
  {
    statusLine = *it;
    ++it;
  }

  nuiHTTPResponse* pResponse = new nuiHTTPResponse(statusCode, statusLine);

  for (; it != end; ++it)
  {
    pos = it->Find(_T(":"));
    nglString fieldName(it->Extract(0, pos));
    fieldName.Trim();
    nglString fieldValue(it->Extract(pos+1));
    fieldValue.Trim();
    pResponse->AddHeader(fieldName, fieldValue);
  }

  size = 0;
  pos = 0;
  do 
  {
    WinHttpQueryDataAvailable(hRequest, &size);
    if (size > 0)
    {
      buf.resize(pos+size);

      WinHttpReadData(hRequest, (void*)&buf[pos], size, NULL);
      pos += size;
    }
  } while( size > 0 );

  pResponse->SetBody(&buf[0], buf.size());

  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  return pResponse;
}