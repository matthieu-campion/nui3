/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiHTTP.h"

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
    str.Append(it->first + _T(" : ") + it->second);
    str.AddNewLine();
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

