/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

using namespace std;

/*
 * Internals
 */

#include "nglApplication.h"

/*
 * Carbon resource IDs
 */

#define rMenuBar        128 /* menu bar */

#define mApple          128 /* Apple menu */
#define iAbout          1

#define mFile           129 /* File menu */
#define iNew            1
#define iClose          4
#define iQuitSeparator  10
#define iQuit           11

#define mEdit           130 /* Edit menu */

#define kAboutBox       200 /* Dialog resource for About box */

#define kSleepTime  0


// #define NGL_APP_ENONE  0 (in nglApplicationBase.h)

const nglChar* gpApplicationErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Couldn't open the default X display. Is the DISPLAY environment variable set ?"),
  NULL
};

nglApplication::nglApplication()
{
  mExitPosted = false;
  mUseIdle = false;
  mIdleTimer = NULL;

  // nglApplication is a kernel's client, just as plugin instances
  IncRef();
}

nglApplication::~nglApplication()
{
  SetIdle(false);
}


/*
 * Public methods
 */

void nglApplication::Quit (int Code)
{
  mExitPosted = true;
  mExitCode = Code;
  QuitApplicationEventLoop();
}


/*
 * Internals
 */

/* Apple Menu
 */

void nglApplication::MakeMenu()   /* Put up a menu */
{
  Handle  menuBar;
  MenuRef menu;
  long  response;
  OSErr err;

  menuBar = GetNewMBar(rMenuBar); /* read menus into menu bar */
  if ( menuBar != nil )
  {
    SetMenuBar(menuBar);  /* install menus */

    err = Gestalt(gestaltMenuMgrAttr, &response);
    if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask))
    {
      menu = GetMenuHandle(mFile);
      DeleteMenuItem(menu, iQuit);
      DeleteMenuItem(menu, iQuitSeparator);
    }

    DrawMenuBar();
  }
  else
  {
    long response;
    MenuHandle menu;
    // Init Menus
    menu = NewMenu(mFile, (unsigned char*)"\004File");// new menu
    InsertMenu(menu, 0);// add menu to end
    SetRootMenu(menu);
    // insert application menus here
    // add quit if not under Mac OS X
    err = Gestalt (gestaltMenuMgrAttr, &response);
    if ((err == noErr) && !(response & gestaltMenuMgrAquaLayoutMask))
      AppendMenu (menu, (unsigned char*)"\006Quit/Q");          // add quit
    DrawMenuBar();

  }

}

void nglApplication::DoMenuCommand(long menuResult)
{
  short menuID;   /* the resource ID of the selected menu */
  short menuItem; /* the item number of the selected menu */

  menuID = HiWord(menuResult);    /* use macros to get item & menu number */
  menuItem = LoWord(menuResult);

  switch (menuID)
  {
    case mApple:
      switch (menuItem)
      {
        case iAbout:
          (void) Alert(kAboutBox, nil);  // simple alert dialog box
          break ; 

        case iQuit:
          mExitPosted = true;
          break;

        default:
          break;
      }
      break;

    case mFile:
      break;

    case mEdit:
      break;
  }
  HiliteMenu(0);  /* unhighlight what MenuSelect (or MenuKey) hilited */
}


/* Startup
 */

int nglApplication::Main(int argc, const char** argv)
{
  if (!Init (argc, argv))
    return 1;
  CallOnInit();
  int res = Run();
  return res;
}

bool nglApplication::Init(int ArgCnt, const char** pArg)
{
  int i;

  #if macintosh
  nglString arg0("nglapplication");
  nglPath Path(ePathCurrent);
  #else
  // Fetch application's name (App->mName) from argv[0]
  nglString arg0(pArg[0]);
  nglString Name = arg0;
  // Only keep file name if it's a full path
  i = Name.Find ('/');
  if (i != -1)
    Name = Name.Extract (i + 1, Name.GetLength());
  SetName(Name);

  // Build application binary's file path from current dir and argv[0]
  #ifdef __MWERKS__
  // When compiled as a bundle an ngl application will be launched from inside the directory of 
  // the bundle so we need to go back to the real bundle path:
  nglPath temppath(arg0);
  nglPath Path(temppath.GetParent());
  Path = Path.GetParent();
  Path = Path.GetParent();
  Path = Path.GetParent();
  if (chdir(Path.GetChars()))
  {
    NGL_DEBUG( NGL_LOG("app", NGL_LOG_INFO,"There was an error getting back to the bundle directory!\n"));
  }
  Path += temppath.GetNodeName();
  #else
  nglPath Path = nglPath(ePathCurrent) + arg0;
  #endif
  #endif // macintosh classic?

  Path.Canonize();
  SetPath(Path);
  // Store user args in mArgs
  for (i = 1; i < ArgCnt; i++)
    AddArg( nglString(pArg[i]) );

  // TODO: Implement Carbon init
  OSErr err;

  err = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP((AEEventHandlerProcPtr)QuitAppleEventHandler), 0, false );
  if (err != noErr)
    return false; /*ExitToShell();*/
  
  err = AEInstallEventHandler( kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP((AEEventHandlerProcPtr)OpenDocumentsAppleEventHandler), 0, false);
  if (err != noErr)
    return false; /*ExitToShell();*/
  
  MakeMenu();
  
  SysInit();
  return true;
}


extern TSMDocumentID nuiTSMDocument;

int nglApplication::Run()
{
#ifndef __CFM_CLASSIC__

  RunApplicationEventLoop();

  if (nuiTSMDocument)
  {
    FixTSMDocument(nuiTSMDocument);
    DeactivateTSMDocument(nuiTSMDocument);
    DeleteTSMDocument(nuiTSMDocument);
  }
  
  //ExitToShell();
  
#endif // __CFM_CLASSIC__
  return 0;
}


/* Event management / main loop
 */

#define DBG_EVENT(x)

OSErr nglApplication::QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
  nglApplication* pApp = ((nglApplication*)App);
  if (pApp->OnQuitRequested())
    pApp->Quit(0);

  return noErr;
}

OSErr nglApplication::OpenDocumentsAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
  AEDescList descList;
  long count;
  
  OSErr err = AEGetParamDesc(appleEvt, keyDirectObject, typeAEList, &descList);
  
  err = AECountItems(&descList, &count);
  
  std::list<nglPath> filePaths;
  
  for(int i = 1; i <= count; i++)
  {
    FSRef ref;
    err = AEGetNthPtr(&descList, i, typeFSRef, NULL, NULL, &ref, sizeof(ref), NULL);
    
    char pBuf[1024];
    err = FSRefMakePath(&ref, (UInt8*)pBuf, 1023);
    
    filePaths.push_back(nglPath(pBuf));
  }
  
  ((nglApplication*)App)->OpenDocuments(filePaths);
  
  return err;
}

#ifdef __CFM_CLASSIC__
// Carbon Stubs for freetype in CFM Classic Mode:
pascal OSErr FSOpenResourceFile ( const FSRef * ref, UniCharCount forkNameLength, const UniChar * forkName, SInt8 permissions, SInt16 * refNum)
{
  return -1;
}

pascal OSStatus FSPathMakeRef ( const UInt8 * path, FSRef * ref, Boolean * isDirectory )
{
  return -1;
}

#endif


///////////////////////////////////////////////
/*
nglApplication::nglApplication(nglApplication* pApp);
nglApplication::~nglApplication();
void nglApplication::Quit (int Code);
void nglApplication::MakeMenu();
void nglApplication::DoMenuCommand(long menuResult);
int nglApplication::Run();
void nglApplication::OnEvent(int Flags);
void nglApplication::AddEvent(nglEvent* pEvent);
void nglApplication::DelEvent(nglEvent* pEvent);
bool nglApplication::AddTimer(nglTimer* pTimer);
bool nglApplication::DelTimer(nglTimer* pTimer);
bool nglApplication::AddWindow (nglWindow* pWin);
bool nglApplication::DelWindow (nglWindow* pWin);
void nglApplication::DoEvent(EventRecord *event);
OSErr nglApplication::QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon );
pascal void TimerAction (EventLoopTimerRef  theTimer, void* userData);
nglString nglApplication::GetClipboard();
bool nglApplication::SetClipboard(const nglString& rString);
*/
