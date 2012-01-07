/*
 NGL - C++ cross-platform framework for OpenGL based applications
 Copyright (C) 2000-2003 NGL Team

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "nui.h"
#include "nuiHTTP.h"

#include <curl/curl.h>
using namespace std;
static size_t write_data(char *data, size_t size, size_t nmemb,
                            nglString *buffer)
{
  for(int i = 0; i < size*nmemb; i++)
  {
    if (data[i] > 0)
      buffer->Append(nglChar(data[i]));
  }
  return size * nmemb;
}


nuiHTTPResponse* nuiHTTPRequest::SendRequest(const nuiHTTPResponseReceivedDelegate& rResponseReceived, const nuiHTTPDataReceivedDelegate& rDataReceived)
{
  // parse address string
  nglString url(mUrl);

  nglString server, objectName;
  nglString buffer;
  nglString header;

  uint port = 80;
  CURLcode res;

  NGL_OUT(_T("%s\n"), __FILE__);

  if (url.CompareLeft(_T("http://")) == 0)
  {
    url.DeleteLeft(7);
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

  CURL *easyhandle = curl_easy_init();
  if (mMethod == _T("POST"))
  {
    res = curl_easy_setopt(easyhandle, CURLOPT_URL, url.GetStdString().c_str());
    res = curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, objectName.GetStdString().c_str());
  }
  else if (mMethod == _T("GET"))
  {
    NGL_OUT(_T("url = %ls\n"), mUrl.GetChars());
    res = curl_easy_setopt(easyhandle, CURLOPT_URL, mUrl.GetStdString().c_str());
  }

  curl_easy_setopt(easyhandle, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &buffer);
  curl_easy_setopt(easyhandle, CURLOPT_WRITEHEADER, &header);
  curl_easy_perform(easyhandle);

  long statusCode = 0;
  curl_easy_getinfo (easyhandle, CURLINFO_RESPONSE_CODE, &statusCode);
  vector<nglString> headersVec;

  header.Tokenize(headersVec, _T("\r\n"));

  nglString statusLine;
  vector<nglString>::const_iterator it = headersVec.begin();
  vector<nglString>::const_iterator end = headersVec.end();
  if (it != end)
  {
    statusLine = *it;
    ++it;
  }

  nuiHTTPResponse* pResponse = new nuiHTTPResponse((uint16) statusCode, statusLine);

  for (; it != end; ++it)
  {
    pos = it->Find(_T(":"));
    nglString fieldName(it->Extract(0, pos));
    fieldName.Trim();
    nglString fieldValue(it->Extract(pos+1));
    fieldValue.Trim();
    pResponse->AddHeader(fieldName, fieldValue);
  }

  if (rResponseReceived)
    rResponseReceived(pResponse);

  //NGL_OUT(_T("body = %s\n"), buffer.GetStdString().c_str());
  pResponse->AddToBody(buffer.GetStdString().c_str(), buffer.GetLength());

  curl_easy_cleanup(easyhandle);
  return pResponse;
}
