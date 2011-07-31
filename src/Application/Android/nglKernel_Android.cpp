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
#include "nglKernel.h"
#include "nglWindow.h"
#include "ngl_unix.h"
#include <signal.h>
#include <locale.h>

#include <android_native_app_glue.h>

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
  Init();

/*
  // If we're attached to a tty, save its settings for segfault handler
  if (isatty(STDIN_FILENO)) tcgetattr (STDIN_FILENO, &mTermInfo);
*/
}

nglKernel::~nglKernel()
{
/*
  if (isatty(STDIN_FILENO)) tcsetattr (STDIN_FILENO, TCSANOW, &mTermInfo);
*/
  Exit(0);

}



/*
 * Clipboard
 */

//changed from nglString nglKernel::GetClipboard() to this, according to nglKernel.h 
void nglKernel::GetClipboard(nglString& rClipboard)
{
  nglString result(_T("*clipboard code not implemented*"));

  // FIXME
  //now returns void, according to nglKernel.h
  //return result;
}

bool nglKernel::SetClipboard(const nglString& rString)
{
  // FIXME
  return false;
}


/*
 * Event handling (stubs rather than pure virtual methods)
 */

void nglKernel::AddEvent (nglEvent* pEvent)
{
}

void nglKernel::DelEvent (nglEvent* pEvent)
{
}

void nglKernel::AddTimer (nglTimer* pTimer)
{
}

void nglKernel::DelTimer (nglTimer* pTimer)
{
}

void* nglKernel::GetDisplay()
{
  return NULL;
}

void nglKernel::AddWindow (class nglWindow* pWin)
{
}

void nglKernel::DelWindow (class nglWindow* pWin)
{
}

android_app* nglKernel::GetAndroidApp()
{
  return mpAndroidApp;
}


/*
 * Internals (generic kernel setup)
 */

bool nglKernel::SysInit(android_app* app)
{
  mpAndroidApp = app;
  
  int signals[] = {
    SIGSEGV,
    SIGHUP, 
    SIGINT, 
    SIGQUIT,
    SIGPIPE,
    SIGTERM,
    -1
  };
  int sig;

  for (sig = 0; signals[sig] != -1; sig++)
    CatchSignal (signals[sig], OnSignal);

  // Set locale (for strtoup(), time/date formatting and so on)
  setlocale (LC_ALL, "");

  return true;
}

void nglKernel::CatchSignal (int Signal, void (*pHandler)(int))
{
  struct sigaction act;

  act.sa_handler = pHandler;
  sigemptyset (&act.sa_mask);
  act.sa_flags = (Signal == SIGCHLD) ? SA_NOCLDSTOP : 0;
  sigaction (Signal, &act, NULL);
}

void nglKernel::OnSignal(int Signal) /* static method */
{
  switch (Signal)
  {
    case SIGSEGV:
    {
/*
      if (isatty(STDIN_FILENO) && (crashcount < 1))
        tcsetattr (STDIN_FILENO, TCSANOW, &App->mTermInfo);
*/
      NGL_DEBUG( NGL_LOG(_T("kernel"), NGL_LOG_ERROR, _T("** Segmentation fault\n")); )
      _exit(2);
    }

    case SIGHUP:
    case SIGINT:
    case SIGQUIT:
    case SIGPIPE:
    case SIGTERM:
    {
      NGL_DEBUG( NGL_LOG(_T("kernel"), NGL_LOG_ERROR, _T("** Caught signal %d\n"), Signal); )
      App->Quit(1);
    }
  }
}

void nglKernel::OnEvent(uint Flags)
{
}

void nglKernel::EnterModalState()
{
}

void nglKernel::ExitModalState()
{
}

void nglKernel::NonBlockingHeartBeat()
{
}
