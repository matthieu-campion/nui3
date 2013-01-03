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
#include "Schnlsp.h"

#pragma comment( lib , "Crypt32.lib" )


using namespace std;

static void ShowError()
{
  DWORD dwError = GetLastError();
  TCHAR *errBuf = 0;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errBuf,0,NULL);
  OutputDebugString(errBuf);
  GlobalFree(errBuf);

}

static void ShowErrorRequest(DWORD err)
{
#define BUFFER_LEN 1023
  DWORD len;
  nglChar buffer[BUFFER_LEN+1];
  memset(buffer, 0, sizeof(buffer));
  nglString errormsg;
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
    NULL, 
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    buffer,
    BUFFER_LEN,
    NULL);
  errormsg = buffer;
  NGL_LOG(_T("httpclient"), 0, errormsg.GetChars());
}

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
  int32 slashpos = url.Find('/');
  if (pos != -1)
  {
    server = url.Extract(0, pos);
    nglString portstr;
    if (slashpos != -1)
    {
      portstr = url.Extract(pos + 1, slashpos - pos - 1);
      objectName = url.Extract(slashpos);
    }
    else
    {
      portstr = url.Extract(pos + 1);
      objectName = nglString::Null;
    }
    port = portstr.GetCInt();
  }
  else
  {
    if (slashpos != -1)
    {
      server = url.Extract(0, slashpos);
      objectName = url.Extract(slashpos);
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

  LPWSTR wstr = (LPWSTR)server.Export(eUCS2);
  HINTERNET hConnect = WinHttpConnect(hSession, wstr, port, 0);
  free(wstr);
  if(!hConnect)
    return NULL;

  LPCWSTR types[] = {L"*/*", NULL};
  LPCWSTR wstr1 = (LPCWSTR)mMethod.Export(eUCS2);
  LPCWSTR wstr2 = (LPCWSTR)objectName.Export(eUCS2);
  HINTERNET hRequest = WinHttpOpenRequest(hConnect, wstr1, wstr2, NULL, NULL, types, WINHTTP_FLAG_ESCAPE_PERCENT | (secure ? WINHTTP_FLAG_SECURE : 0));

  nglString headers = GetHeadersRep();
  if (!headers.IsEmpty())
  {
    LPWSTR pHeaders = (LPWSTR)headers.Export(eUCS2);
    uint32 HeaderLength = -1;
    if (!WinHttpAddRequestHeaders(hRequest, pHeaders, HeaderLength, WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE))
    {
      NGL_OUT(_T("WinHttpAddRequestHeaders error: %d (0x%x)\n"), GetLastError(), GetLastError());
      ShowError();
      free(pHeaders);
      return NULL;
    }
    free(pHeaders);
  }

  BOOL success = FALSE;

  if (mBody.size())
  {
    success = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, &mBody[0], mBody.size(), mBody.size(), NULL);
    if (!success)
    {
      NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
      ShowError();
      return NULL;
    }
  }
  else
  {
    success = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, NULL, 0, 0, NULL);
    if (!success)
    {
      NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
      ShowError();
      return NULL;
    }
  }

  if (!success)
  {
    NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
    ShowError(); 
    return NULL;
  }

  success = WinHttpReceiveResponse(hRequest, NULL);
  if (!success)
  {
    if( GetLastError( ) == ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED )
    {
      //MY is the store the certificate is in.
      HCERTSTORE hMyStore = CertOpenSystemStore( 0, TEXT("MY") );
      if( hMyStore )
      {
        SecPkgContext_IssuerListInfoEx* pIssuerList = NULL;
        DWORD dwBufferSize = sizeof(SecPkgContext_IssuerListInfoEx*);

        if (WinHttpQueryOption(hRequest,
          WINHTTP_OPTION_CLIENT_CERT_ISSUER_LIST,
          &pIssuerList,
          &dwBufferSize) == TRUE)
        {
          // Use the pIssuerList for cert store filtering.


          PCCERT_CONTEXT pCertContext = CertFindCertificateInStore( hMyStore,
            X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
            0,
            CERT_FIND_SUBJECT_STR,
            pIssuerList->aIssuers, //Subject string in the certificate.
            NULL );
          if (pCertContext)
          {
            WinHttpSetOption(hRequest,
              WINHTTP_OPTION_CLIENT_CERT_CONTEXT,
              (LPVOID) pCertContext, 
              sizeof(CERT_CONTEXT) );
            CertFreeCertificateContext( pCertContext );
          }
          CertCloseStore( hMyStore, 0 );

          // NOTE: Application should now resend the request.
          if (mBody.size())
          {
            success = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, &mBody[0], mBody.size(), mBody.size(), NULL);
            if (!success)
            {
              NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
              ShowError();
              return NULL;
            }
          }
          else
          {
            success = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, NULL, 0, 0, NULL);
            if (!success)
            {
              NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
              ShowError();
              return NULL;
            }
          }

          if (!success)
          {
            NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
            ShowError(); 
            return NULL;
          }

          success = WinHttpReceiveResponse(hRequest, NULL);
          if (!success)
          {
            ShowErrorRequest(GetLastError());
            NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
            return NULL;
          }

          GlobalFree(pIssuerList); // Free the issuer list when done.
        }

      }
    }
    else
    {
      ShowErrorRequest(GetLastError());
      NGL_OUT(_T("WinHttpSendRequest error: %d (0x%x)\n"), GetLastError(), GetLastError());
      return NULL;
    }
  }

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