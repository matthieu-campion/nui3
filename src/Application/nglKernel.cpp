/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglConsole.h"
#include "nglLog.h"
#include "nuiCommand.h"

#include "nglDataObjects.h"

#include "nuiNativeResourceVolume.h"
#include "nuiNotification.h"

#include "ucdata.h"


/* Defined in <platform>/nglKernel.cpp
 */
extern const nglChar* gpKernelErrorTable[];

/* The kernel global instance is referenced here (and only here)
 */
NGL_API nglKernel * App = NULL;



/*
 * Life cycle
 */

void nglKernel::Quit (int Code)
{
  // Pure nglKernel instance : does not handle event loop, can't quit by its own will.
}

void nglKernel::AddExit (ExitFunc Func)
{
  mExitFuncs.push_front(Func);
}

void nglKernel::DelExit (ExitFunc Func)
{
  mExitFuncs.remove(Func);
}


/*
 * Runtime debugging
 */

// bool nglKernel::GetDebug() [inlined in header]

void nglKernel::SetDebug (bool Debug)
{
  mDebug = Debug;
}


/*
 * Logging and output
 */

nglLog& nglKernel::GetLog()
{
  if (!mpLog)
  {
    mpLog = new nglLog();
  }
  return *mpLog;
}


nglConsole& nglKernel::GetConsole()
{
  //#HACH This is a hack to have NGL_OUT working event when nuiInit hasn't been called yet
  if (!this)
    return *(nglConsole*)NULL;

  if (!mpCon)
  {
    mpCon = new nglConsole();
    mOwnCon = true;
  }
  return *mpCon;
}

void nglKernel::SetConsole (nglConsole* pConsole)
{
  if (mpCon && mOwnCon)
    delete mpCon;
  mpCon = pConsole;
  mOwnCon = false;
}

/*
 * Application context
 */

const nglPath& nglKernel::GetPath()
{
  return mPath;
}

const nglString& nglKernel::GetName()
{
  return mName;
}

int nglKernel::GetArgCount()
{
  return (int)mArgs.size();
}

const nglString& nglKernel::GetArg (int Index)
{
  return (Index < (int)mArgs.size()) ? mArgs[Index] : nglString::Null;
}


/* Clipboard (platform specific)
 *
nglString GetClipboard();
bool SetClipboard(const nglString& rString);
 */
nglClipBoard& nglKernel::GetClipBoard()
{
  return mClipboard;
}

/*
 * DataTypesRegistry
 */
nglDataTypesRegistry& nglKernel::GetDataTypesRegistry()
{
  return mDataTypesRegistry;
}

/*
 * User callbacks
 */

// Init/Exit:
void nglKernel::OnInit()
{
}

void nglKernel::OnExit(int Code)
{
}

// Device management:
void nglKernel::OnDeviceAdded(const nglDeviceInfo* pDeviceInfo)
{
}

void nglKernel::OnDeviceRemoved(const nglDeviceInfo* pDeviceInfo)
{
}


/*
 * Life cycle (private)
 */

void nglKernel::IncRef()
{
  mRefCount++;
}

void nglKernel::DecRef()
{
  NGL_ASSERT(mRefCount > 0);

  mRefCount--;
  if (mRefCount == 0)
    delete this;
}

void SendSomeMail();

void nglKernel::Init()
{
  //SendSomeMail();
  mRefCount = 0;
  App = this;
#ifdef _DEBUG_
  mDebug = true;
#else
  mDebug = false;
#endif
  mpLog = NULL;
  mpCon = NULL;
  mOwnCon = false;
  mActive = true;
}

void nglKernel::Exit(int32 ExitCode)
{
  nuiMainWindow::DestroyAllWindows();
  mKernelEventSink.DisconnectAll();
  nglVolume::UnmountAll();
  nuiAnimation::ReleaseTimer();

  ExitFuncList::iterator func_i;
  
  for (func_i = mExitFuncs.begin(); func_i != mExitFuncs.end(); ++func_i)
  {
    ExitFunc func;

    func = *func_i;
    if (func)
      (*func)();
  }
  mExitFuncs.clear();

#ifdef NGL_STRING_STATS
  nglString::DumpStats(NGL_LOG_INFO);
#endif

  if (mpLog)
  {
    delete mpLog;
    mpLog = NULL;
  }

  if (mpCon && mOwnCon)
  {
    delete mpCon;
    mpCon = NULL;
  }
  
  nglString::ReleaseStringConvs();
}

void nglKernel::SetName(const nglString& rName)
{
  mName = rName;
}

void nglKernel::SetPath(const nglPath& rPath)
{
  mPath = rPath;
}

void nglKernel::ParseCmdLine (char* pCmdLine)
{
  enum { skipping, reading_token, reading_quoted_token } state = skipping;
  nglString arg;
  char* p = pCmdLine;
  char* token_p = NULL;
  bool eos;

  do
  {
    eos = (*p == 0);

    switch (state)
    {
      case skipping:
        if (*p == '"') { state = reading_quoted_token; token_p = p + 1; } else
        if (*p != ' ') { state = reading_token; token_p = p; }
        /* else keep skipping */
        break;
      case reading_token:
      case reading_quoted_token:
        if (eos ||
            (state == reading_token && *p == ' ') ||
            (state == reading_quoted_token && *p == '"'))
        {
          *p = 0;
          arg = token_p;
          mArgs.push_back (arg);
          state = skipping;
        }
        break;
    }
    p++;
  }
  while (!eos);
}

void nglKernel::AddArg (const nglString& rArg)
{
  uint size = mArgs.size();

  mArgs.resize(size + 1);
  mArgs[size++] = rArg;
}

void nglKernel::DelArg (int Pos, int Count)
{
  if (Count > 1)
    mArgs.erase (mArgs.begin() + Pos, mArgs.begin() + Pos + Count);
  else
    mArgs.erase (mArgs.begin() + Pos);
}


/*
 * Internal callbacks
 */

const nglChar* nglKernel::OnError (uint& rError) const
{
  return FetchError(gpKernelErrorTable, NULL, rError);
}

void nglKernel::CallOnInit()
{
  ucdata_init_static();
  NGL_DEBUG( NGL_LOG(_T("kernel"), NGL_LOG_INFO, _T("Init (%d parameter%ls)"), GetArgCount(), (GetArgCount() > 1) ? _T("s") : _T("")); )
  nglVolume* pResources = new nuiNativeResourceVolume();
  nglVolume::Mount(pResources);
  nuiTimer* pTimer = nuiAnimation::AcquireTimer();
  mKernelEventSink.Connect(pTimer->Tick, &nglKernel::ProcessMessages);
  mpNotificationManager = new nuiNotificationManager();
  
  OnInit();
}

void nglKernel::CallOnExit(int Code)
{
  NGL_DEBUG( NGL_LOG(_T("kernel"), NGL_LOG_INFO, _T("Exit (code: %d)"), Code); )
  OnExit (Code);
  nuiAnimation::ReleaseTimer();
}

void nglKernel::CallOnWillExit()
{
  OnWillExit();
}

void nglKernel::OnWillExit()
{

}

void nglKernel::CallOnActivation()
{
  mActive = true;
  OnActivation();
}

void nglKernel::CallOnDeactivation()
{
  mActive = false;
  OnDeactivation();
}

void nglKernel::OnActivation()
{
}

void nglKernel::OnDeactivation()
{
}

bool nglKernel::IsActive() const
{
  return mActive;
}

///< Memory-warning methods (only availble on iPhone OS/UIKit)
void nglKernel::OnMemoryWarning()
{
}

void nglKernel::CallOnMemoryWarning()
{
	OnMemoryWarning();
}



char* nglCrashEmail = NULL;
void nglKernel::SetCrashReportEmail(const nglString& rEmail)
{
  nglCrashEmail = rEmail.Export();
}


void nglKernel::ProcessMessages(const nuiEvent& rEvent)
{
  nuiNotification* pNotif;
  while (pNotif = Get(0))
  {
    nuiCommand* pCommand = NULL;
    nuiGetTokenValue<nuiCommand*>(pNotif->GetToken(), pCommand);
    if (pCommand)
      pCommand->Do();
    delete pNotif;
  }

  mpNotificationManager->BroadcastQueuedNotifications();
}

void nglKernel::PostNotification(nuiNotification* pNotification)
{
  mpNotificationManager->PostNotification(pNotification);
}

void nglKernel::BroadcastNotification(const nuiNotification& rNotification)
{
  mpNotificationManager->BroadcastNotification(rNotification);
}

void nglKernel::RegisterObserver(const nglString& rNotificationName, nuiNotificationObserver* pObserver)
{
  mpNotificationManager->RegisterObserver(rNotificationName, pObserver);
}

void nglKernel::UnregisterObserver(nuiNotificationObserver* pObserver, const nglString& rNotificationName)
{
  mpNotificationManager->UnregisterObserver(pObserver, rNotificationName);
}




