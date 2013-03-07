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

  //#FIXME to be done
  return NULL;
}
