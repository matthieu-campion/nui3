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
  switch (err)
  {
  case ERROR_WINHTTP_CANNOT_CONNECT:
      NGL_OUT(_T("Returned if connection to the server failed."));
      break;
  case ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW:
      NGL_OUT(_T("Returned when an overflow condition is encountered in the course of parsing chunked encoding."));
      break;
  case ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED:
      NGL_OUT(_T("Returned when the server requests client authentication."));
      break;
  case ERROR_WINHTTP_CONNECTION_ERROR:
      NGL_OUT(_T("The connection with the server has been reset or terminated, or an incompatible SSL protocol was encountered. For example, WinHTTP version 5.1 does not support SSL2 unless the client specifically enables it."));
      break;
  case ERROR_WINHTTP_HEADER_COUNT_EXCEEDED:
      NGL_OUT(_T("Returned when a larger number of headers were present in a response than WinHTTP could receive."));
      break;
  case ERROR_WINHTTP_HEADER_SIZE_OVERFLOW:
      NGL_OUT(_T("Returned by WinHttpReceiveResponse when the size of headers received exceeds the limit for the request handle."));
      break;
  case ERROR_WINHTTP_INCORRECT_HANDLE_STATE:
      NGL_OUT(_T("The requested operation cannot be carried out because the handle supplied is not in the correct state."));
      break;
  case ERROR_WINHTTP_INCORRECT_HANDLE_TYPE:
      NGL_OUT(_T("The type of handle supplied is incorrect for this operation."));
      break;
  case ERROR_WINHTTP_INTERNAL_ERROR:
      NGL_OUT(_T("An internal error has occurred."));
      break;
  case ERROR_WINHTTP_INVALID_SERVER_RESPONSE:
      NGL_OUT(_T("The server response could not be parsed."));
      break;
  case ERROR_WINHTTP_INVALID_URL:
      NGL_OUT(_T("The URL is invalid."));
      break;
  case ERROR_WINHTTP_LOGIN_FAILURE:
      NGL_OUT(_T("The login attempt failed. When this error is encountered, the request handle should be closed with WinHttpCloseHandle. A new request handle must be created before retrying the function that originally produced this error."));
      break;
  case ERROR_WINHTTP_NAME_NOT_RESOLVED:
      NGL_OUT(_T("The server name could not be resolved."));
      break;
  case ERROR_WINHTTP_OPERATION_CANCELLED:
      NGL_OUT(_T("The operation was canceled, usually because the handle on which the request was operating was closed before the operation completed."));
      break;
  case ERROR_WINHTTP_REDIRECT_FAILED:
      NGL_OUT(_T("The redirection failed because either the scheme changed or all attempts made to redirect failed (default is five attempts)."));
      break;
  case ERROR_WINHTTP_RESEND_REQUEST:
      NGL_OUT(_T("The WinHTTP function failed. The desired function can be retried on the same request handle."));
      break;
  case ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW:
      NGL_OUT(_T("Returned when an incoming response exceeds an internal WinHTTP size limit."));
      break;
  case ERROR_WINHTTP_SECURE_FAILURE:
      NGL_OUT(_T("One or more errors were found in the Secure Sockets Layer (SSL) certificate sent by the server. To determine what type of error was encountered, check for a WINHTTP_CALLBACK_STATUS_SECURE_FAILURE notification in a status callback function. For more information, see WINHTTP_STATUS_CALLBACK."));
      break;
  case ERROR_WINHTTP_TIMEOUT:
      NGL_OUT(_T("The request has timed out."));
      break;
  case ERROR_WINHTTP_UNRECOGNIZED_SCHEME:
      NGL_OUT(_T("The URL specified a scheme other than \"http:\" or \"https:\"."));
      break;
  case ERROR_NOT_ENOUGH_MEMORY:
      NGL_OUT(_T("Not enough memory was available to complete the requested operation. (Windows error code)"));
      break;
  }
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
  HINTERNET hRequest = WinHttpOpenRequest(hConnect, mMethod.GetChars(), objectName.GetChars(), NULL, NULL, types, WINHTTP_FLAG_ESCAPE_PERCENT | (secure ? WINHTTP_FLAG_SECURE : 0));

  nglString headers = GetHeadersRep();
  if (!headers.IsEmpty())
    WinHttpAddRequestHeaders(hRequest, headers.GetChars(), -1L, WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);

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