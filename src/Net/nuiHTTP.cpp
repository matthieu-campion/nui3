/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

using namespace std;

nuiHTTPMessage::nuiHTTPMessage()
{
}

nuiHTTPMessage::~nuiHTTPMessage()
{
}

const nuiHTTPHeaderMap& nuiHTTPMessage::GetHeaders() const
{
  return mHeaders;
}

nglString nuiHTTPMessage::GetHeadersRep() const
{
  nglString str;
  nuiHTTPHeaderMap::const_iterator end = mHeaders.end();
  for (nuiHTTPHeaderMap::const_iterator it = mHeaders.begin(); it != end; ++it)
  {
    str.Append(it->first + _T(": ") + it->second);
    str.Add(_T("\r\n"));
  }
  return str;
}

void nuiHTTPMessage::AddHeader(const nglString& rFieldName, const nglString& rFieldValue)
{
  mHeaders.insert(pair<nglString, nglString>(rFieldName, rFieldValue));
}

bool nuiHTTPMessage::AddHeader(const nglString& rHeader)
{
  int32 pos = rHeader.Find(':');
  
  if (pos < 1)
  {
    return false; // no ':' in string or ':' as 1st character, invalid header field
  }
  
  nglString fieldName = rHeader.Extract(0, pos - 1);
  fieldName.Trim();
  nglString fieldValue = rHeader.Extract(pos + 1);
  fieldValue.Trim();
  AddHeader(fieldName, fieldValue);

  return true;
}

void nuiHTTPMessage::SetBody(const char* pBuffer, nglSize ByteCnt)
{
  mBody.resize(ByteCnt);
  memcpy(&mBody[0], pBuffer, ByteCnt);
}

void nuiHTTPMessage::AddToBody(const char* pBuffer, nglSize ByteCnt)
{
  nglSize pos = mBody.size();
  mBody.resize(pos + ByteCnt);
  memcpy(&mBody[pos], pBuffer, ByteCnt);
}

const std::vector<char>& nuiHTTPMessage::GetBody() const
{
  return mBody;
}

nglString nuiHTTPMessage::GetBodyStr() const
{
  return nglString(&mBody[0], mBody.size(), eUTF8);
}

nuiHTTPRequest::nuiHTTPRequest(const nglString& rUrl, const nglString& rMethod)
: mUrl(rUrl), mMethod(rMethod)
{
  mMethod.ToUpper();
}

nuiHTTPRequest::~nuiHTTPRequest()
{
}

nuiHTTPRequest_Thread* nuiHTTPRequest::SendRequest(const nuiHTTPRequest::Delegate& rDelegate)
{
  return new nuiHTTPRequest_Thread(this, rDelegate);
}

const nglString& nuiHTTPRequest::GetURL() const
{
  return mUrl;
}

const nglString& nuiHTTPRequest::GetMethod() const
{
  return mMethod;
}


nuiHTTPResponse::nuiHTTPResponse(uint16 StatusCode, const nglString& StatusLine)
: mStatusCode(StatusCode), mStatusLine(StatusLine)
{
}

nuiHTTPResponse::~nuiHTTPResponse()
{
}

uint16 nuiHTTPResponse::GetStatusCode() const
{
  return mStatusCode;
}

const nglString& nuiHTTPResponse::GetStatusLine() const
{
  return mStatusLine;
}

nuiHTTPResponse* nuiHTTPRequest::SendRequest()
{
  return SendRequest(NULL, NULL);
}



////////////////
// nuiHTTPRequest_Thread
void nuiHTTPRequest_Thread::Cancel()
{
  mCancel = true;
}

nuiHTTPRequest_Thread::nuiHTTPRequest_Thread(nuiHTTPRequest* pRequest, const nuiHTTPRequest::Delegate& rDelegate)
: nuiCommand(_T("nuiHTTPRequest_Thread"), _T("HTTP Threaded Request Reply Command"), false, false, false),
  mpRequest(pRequest), mDelegate(rDelegate), mpResponse(NULL), mCancel(false)
{
  Start();
}

nuiHTTPRequest_Thread::~nuiHTTPRequest_Thread()
{
  delete mpRequest;
  delete mpResponse;
}

void nuiHTTPRequest_Thread::OnStart()
{
  mpResponse = mpRequest->SendRequest();
  
  if (mCancel)
  {
    SetAutoDelete(true);
    return;
  }
  
  nuiNotification* pNotif = new nuiNotification(_T("nuiHTTPRequest_Thread"));
  pNotif->SetToken(new nuiToken<nuiCommand*>(this, true));
  App->Post(pNotif);
}

bool nuiHTTPRequest_Thread::SetArgs(const std::vector<nglString, std::allocator<nglString> >&)
{
  return true;
}

bool nuiHTTPRequest_Thread::ExecuteDo()
{
  if (!mCancel)
    mDelegate(mpRequest, mpResponse);
  return true;
}

