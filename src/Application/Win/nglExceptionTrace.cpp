
#if (defined WIN32) && (!defined _WIN64)
#undef UNICODE
#undef _UNICODE
#define MAX(X,Y) ((X>Y)?X:Y)
#include "nglExceptionTrace.h"

void nglInitExceptionHandler()
{
  // Additional information flags
  UINT64 t = TraceSystemTime
  |  TraceLocation 
  |  TracePID
  |  TraceTID
  |  TracePrTime
  |  TraceThrTime
  |  TraceRealPrThrTime
  |  TraceAddInfoMask
    // Functions information flags
  |  TraceFnName
  |  TraceFnArgTypes
  |  TraceFnArgs
  |  TraceFnCallDisp
  //|  TraceFnStripTemplArgs
  //|  TraceFnStripStringArgs
  //|  TraceFnNoRawWideStrings
  //|  TraceFnDontUndecorate
    // Representation flags
  |  TraceMicroseconds
  |  TraceRtiHex
  |  TraceFnDecIntArgs
  |  TraceFnHexIntArgs
  |  TraceFnDispHex;

  SET_STACK_TRACE_OPTS(TraceAll | t);
}

#if 1

void SendSomeMail()
{
  nglMail mailer;

  char pStrSubject[1024];
  _snprintf(pStrSubject, 1024, "Crash in nui app");

  char pStrMessage[1024];
  _snprintf(pStrMessage, 1024, "pouet! :-)");

  //char pStrAttachmentFilePath[1024];
  //_snprintf(pStrAttachmentFilePath, 1024, "%s", pcszRptPath);;
  //char* pStrAttachmentFilePath = NULL;
  char* pStrAttachmentFilePath = "C:\\Users\\meeloo\\Documents\\work\\mxp4\\MXEditor-v2\\Release\\CrashReport.txt";

  char* pStrAttachmentFile = NULL;
//  char pStrAttachmentFile[1024];
 // _snprintf(pStrAttachmentFile, 1024, "%s", pcszRptFileName);;

  char pStrRecipient[1024];
  _snprintf(pStrRecipient, 1024, "meeloo@meeloo.net");;

  char pStrEmailAdress[1024];
  _snprintf(pStrEmailAdress, 1024, "meeloo@meeloo.net");;

  mailer.Send(pStrSubject, pStrMessage, pStrAttachmentFilePath, pStrRecipient, pStrEmailAdress);
}

#define MAPI_INSTALLED 0
nglMail::nglMail()
{
  mlhSession = 0;

  // MAPI function pointers
  mMAPIAddress = NULL;
  mMAPIDetails = NULL;		
  mMAPIFindNext = NULL;		
  mMAPIFreeBuffer = NULL;
  mMAPILogoff = NULL;		
  mMAPILogon = NULL;			
  mMAPIReadMail = NULL;		
  mMAPIResolveName = NULL;	
  mMAPISendDocuments = NULL;	
  mMAPISendMail = NULL;
  mMAPISaveMail = NULL;

  InitMapi();
}

nglMail::~nglMail()
{

}

bool nglMail::InitMapi()
{
  bool unlResult;

  unlResult = IsMapiInstalled() ;
  if(!unlResult)
    return false;

  // Get instance handle of MAPI32.DLL
  HINSTANCE			hlibMAPI		= LoadLibrary ( _T("MAPI32.dll") );	

  //  Get the addresses of all the API's supported by this object
  mMAPILogon			= ( LPMAPILOGON			)	GetProcAddress ( hlibMAPI, "MAPILogon"			);
  mMAPISendMail		= ( LPMAPISENDMAIL		)	GetProcAddress ( hlibMAPI, "MAPISendMail"		);
  mMAPISendDocuments	= ( LPMAPISENDDOCUMENTS )	GetProcAddress ( hlibMAPI, "MAPISendDocuments"	);
  mMAPIFindNext		= ( LPMAPIFINDNEXT		)	GetProcAddress ( hlibMAPI, "MAPIFindNext"		);
  mMAPIReadMail		= ( LPMAPIREADMAIL		)	GetProcAddress ( hlibMAPI, "MAPIReadMail"		);
  mMAPIResolveName	= ( LPMAPIRESOLVENAME	)	GetProcAddress ( hlibMAPI, "MAPIResolveName"	);
  mMAPIAddress		= ( LPMAPIADDRESS		)	GetProcAddress ( hlibMAPI, "MAPIAddress"		);
  mMAPILogoff		= ( LPMAPILOGOFF		)	GetProcAddress ( hlibMAPI, "MAPILogoff"			);
  mMAPIFreeBuffer	= ( LPMAPIFREEBUFFER	)	GetProcAddress ( hlibMAPI, "MAPIFreeBuffer"		);   
  mMAPIDetails		= ( LPMAPIDETAILS		)	GetProcAddress ( hlibMAPI, "MAPIDetails"		);
  mMAPISaveMail		= ( LPMAPISAVEMAIL		)	GetProcAddress ( hlibMAPI, "MAPISaveMail"		);

  return true;
}

//##//##/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ULONG nglMail::IsMapiInstalled()
//
//    This module checks if the MAPI is installed on the system
//
//  FUNCTION RETURN:  error code if any
//
//	Added by:  Aisha Ikram 
//
//##//##/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool nglMail::IsMapiInstalled()
{
  DWORD  SimpleMAPIInstalled;
  char   szAppName[32];
  char   szKeyName[32];
  char   szDefault = {'0'};
  char   szReturn = {'0'};
  DWORD  nSize = 0L;
  char   szFileName[256];


  strcpy ( szAppName, "MAIL" );
  strcpy ( szKeyName, "MAPI" );

  nSize = 1;
  strcpy ( szFileName, "WIN.INI" );


  SimpleMAPIInstalled = GetPrivateProfileString ( (TCHAR*) szAppName, 
    (TCHAR*) szKeyName, 
    (TCHAR*)&szDefault,
    (TCHAR*)&szReturn, 
    nSize, 
    (TCHAR*)szFileName);


  if ( MAPI_INSTALLED == strcmp ( &szDefault, &szReturn ) )
  {
    return false;
  }

  return true;
}



bool nglMail::Logon()
{
  ULONG unlResult;
  FLAGS	flFlags = 0L;
  ULONG	ulReserved = 0L;

  LPSTR	lpszPassword = NULL;

  if ( !mlhSession )	  // Always ask if there is an active session
  {
    flFlags = MAPI_NEW_SESSION;  // Logon with a new session and force display of UI.

    unlResult = mMAPILogon (
      0L, 				// Handle to parent window or 0.
      NULL, //(LPTSTR)pStrProfileName.Copy(), Default profile name to use for MAPI session.
      lpszPassword,		// User password for MAPI session.
      flFlags,			// Various session settings
      ulReserved,		// Reserved.  Must be 0L.
      &mlhSession		// Return handle to MAPI Session
      );


    if(unlResult != SUCCESS_SUCCESS)
      return false;
  }

  return true;
}

bool nglMail::Send(const char* pStrSubject,
                   const char* pStrMessage,
                   const char* pStrAttachmentFilePath,
                   const char* pStrRecipient,
                   const char* pStrEmailAdress)
{
  ULONG unlResult = 1; 
  MapiMessage oMapiMessage;
  MapiFileDesc oAttachment;

  ZeroMemory ( &oMapiMessage, sizeof ( MapiMessage ) );
  ZeroMemory ( &oAttachment, sizeof ( MapiFileDesc ) );

  lpMapiRecipDesc pRecips = NULL;
  //MapiRecipDesc arMailRecipients[1], *tempRecip[1];     
  MapiRecipDesc arMailRecipients[1];

  if (!Logon())
    return false;

  std::string bstrAddress;
  bstrAddress = "SMTP:";
  bstrAddress += pStrEmailAdress;

  std::string bstrRec, bstrAdd, bstrAttFile, bstrAttPath, bstrSubj, bstrMesg;


  arMailRecipients[0].ulReserved   = 0;
  arMailRecipients[0].ulRecipClass = MAPI_TO;

  // recipient name
  bstrRec = pStrRecipient;
  arMailRecipients[0].lpszName	  = (char*)bstrRec.c_str();

  // email address
  bstrAdd = bstrAddress;
  arMailRecipients[0].lpszAddress  = (char*)bstrAdd.c_str();

  arMailRecipients[0].ulEIDSize    = 0;
  arMailRecipients[0].lpEntryID    = NULL;


  //std::string bstrFullPath = pStrAttachmentFilePath;
  //bstrFullPath += pStrAttachmentFile;

  //oAttachment.lpszFileName = pStrAttachmentFile;

  oAttachment.lpszPathName = (LPSTR)pStrAttachmentFilePath;

  oMapiMessage.nRecipCount	= 1;		// Must be set to the correct number of recipients.
  oMapiMessage.lpRecips		= arMailRecipients;	// Address of list of names returned from MAPIAddress.		
  oMapiMessage.ulReserved		= 0L;

  bstrSubj = pStrSubject;
  oMapiMessage.lpszSubject	= (char *)bstrSubj.c_str();

  bstrMesg = pStrMessage;
  oMapiMessage.lpszNoteText	= (char *)bstrMesg.c_str();

  oMapiMessage.lpOriginator	= NULL;	
  if (pStrAttachmentFilePath)
  {
    oMapiMessage.nFileCount		= 1;
    oMapiMessage.lpFiles		= &oAttachment;
  }
  else
  {
    oMapiMessage.nFileCount		= 0L;
    oMapiMessage.lpFiles		= NULL;
  }


  unlResult = mMAPISendMail (	mlhSession,	// Global session handle.
    0L,				// Parent window.  Set to 0 since console app.
    &oMapiMessage,		// Address of Message structure
    MAPI_DIALOG,		
    0L		// Reserved.  Must be 0L.
    );	


  if (unlResult != SUCCESS_SUCCESS)
    return false;


  //mMAPIFreeBuffer(tempRecip);  // release the recipient descriptors
  if (!Logoff())
    return false;

  return true;
}

//##//##/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  STDMETHODIMP nglMail::Logoff()
//
//    This module logs off the outlook profile and closes the session
//
//  FUNCTION RETURN: standard HRESULT
//
//##//##/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool nglMail::Logoff()
{
  ULONG	unlResult;
  FLAGS	flFlags = 0L;
  ULONG	ulReserved = 0L;

  // Always check to make sure there is an active session
  if ( mlhSession )	 
  {
    //  If there is a valid session handle, attempt to logoff.
    unlResult = mMAPILogoff (
      mlhSession,	// Global session handle.
      0L,				// Parent window.  Set to 0 since console app.
      flFlags,		// Ignored by MAPILogoff.
      ulReserved		// Reserved.  Must be 0L.								
      );				
    if(unlResult != SUCCESS_SUCCESS)
      return false;
  }

  mlhSession = NULL;
  return true;
}

#endif

#endif
