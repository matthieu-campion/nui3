/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#include "nglKernel.h"
#include "nglDeviceInfo.h"

/*
bool nglKernel::GetGamma(float* pRed, float* pGreen, float* pBlue)
{
  if (pRed)   *pRed   = 1.0;
  if (pGreen) *pGreen = 1.0;
  if (pBlue)  *pBlue  = 1.0;
  return false;
}

bool nglKernel::SetGamma(float Red, float Green, float Blue)
{
  return false;
}

bool nglKernel::GetResolution(float* width, float* height)
{
  // FIXME: Shamelessly hardcoded resolution!
  *width = 1024;
  *height = 768;
  return true;
}
*/

using namespace std;


const nglChar* gpKernelErrorTable[] =
{
/*  0 */ _T("No error"),
  NULL
};


/*
 * Life cycle
 */

nglKernel::nglKernel()
: mKernelEventSink(this)
{
  mEventHandlerRef = NULL;
  mEventHandlerUPP = NULL;
  Init();

/*
  // If we're attached to a tty, save its settings for segfault handler
  if (isatty(STDIN_FILENO)) tcgetattr (STDIN_FILENO, &mTermInfo);
*/
}

nglKernel::~nglKernel()
{
//  printf("nglKernel::dtor\n");
  if (mEventHandlerRef)
	  RemoveEventHandler(mEventHandlerRef);
  if (mEventHandlerUPP)
    DisposeEventHandlerUPP(mEventHandlerUPP);

  Exit();

/*
  if (isatty(STDIN_FILENO)) tcsetattr (STDIN_FILENO, TCSANOW, &mTermInfo);
*/
//  printf("nglKernel::dtor OK\n");
}



/*
 * Clipboard
 */

void nglKernel::GetClipboard(nglString& rClipBoard)
{
  // #FIXME
  rClipBoard = _T("*clipboard code not implemented*");
}

bool nglKernel::SetClipboard(const nglString& rString)
{
  // #FIXME
  return false;
}



/*
 * Internals (generic kernel setup)
 */

pascal OSStatus nglVolumeEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData)
{
//  printf("nglVolumeEventHandler 0x%x\n", userData);
  nglKernel* pKernel = (nglKernel*)userData;
  if (!pKernel)
    return noErr;
  
  UInt32 eventKind = GetEventKind(eventRef);
  UInt32 eventClass = GetEventClass (eventRef);
    
  switch (eventClass)
  {
    case kEventClassVolume:
    {
      FSVolumeRefNum refNum = 0;
      GetEventParameter (eventRef, kEventParamDirectObject, typeFSVolumeRefNum, 
                         NULL, sizeof(refNum), NULL, &refNum);
      
      switch (eventKind)
      {
        case kEventVolumeMounted:
        {
//          NGL_OUT("Volume Mounted: %d\n", refNum);
          nglPathVolume vol(nglPathVolume::AddVolume(pKernel->mVolumes, refNum));
          nglPathVolumeDeviceInfo info(vol);
          
          pKernel->OnDeviceAdded(&info);
        }
        break;
        case kEventVolumeUnmounted:
        {
//          NGL_OUT("Volume Unmounted: %d\n", refNum);
          nglPathVolume vol(nglPathVolume::DelVolume(pKernel->mVolumes, refNum));
          nglPathVolumeDeviceInfo info(vol);
          pKernel->OnDeviceRemoved(&info);
        }
        break;
      }
    }
  }
  return noErr;
}

bool nglKernel::SysInit()
{
  // Set locale (for strtoup(), time/date formatting and so on)
  setlocale (LC_ALL, "");

  EventTypeSpec windowEvents[]={ { kEventClassVolume, kEventVolumeMounted}, { kEventClassVolume, kEventVolumeUnmounted} };
  
  nglPathVolume::UpdateVolumes(mVolumes);
//  printf("nglKernel::SysInit 0x%x\n", this);
  mEventHandlerUPP = NewEventHandlerUPP(::nglVolumeEventHandler);
  OSErr err = InstallApplicationEventHandler(mEventHandlerUPP, 
                                 GetEventTypeCount(windowEvents),
                                 windowEvents, 
                                 (void*) this, 
                                 &mEventHandlerRef);
  
  return true;
}

void nglKernel::NonBlockingHeartBeat()
{
  EventRef event;
  EventTargetRef eventDispatcher = GetEventDispatcherTarget();
  
  while (ReceiveNextEvent(0, NULL, 0.0, TRUE, &event) == noErr)
  {
    SendEventToEventTarget (event, eventDispatcher);
    ReleaseEvent(event);
  }
}
