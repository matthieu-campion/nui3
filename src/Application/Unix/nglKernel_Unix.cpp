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
#ifndef _MINUI3_
#include "nglWindow.h"
#endif
#include "ngl_unix.h"
#include <signal.h>
#include <locale.h>

#ifdef _LINUX_
#include <gcrypt.h> 
GCRY_THREAD_OPTION_PTHREAD_IMPL;
#endif

#include <curl/curl.h>

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


#ifndef _MINUI3_

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

#endif // ifndef _MINUI3_

/*
 * Internals (generic kernel setup)
 */

bool nglKernel::SysInit()
{
  int signals[] = {
    SIGSEGV,
    SIGHUP,
    SIGINT,
    SIGQUIT,
    //SIGPIPE,
    SIGTERM,
    -1
  };
  int sig;

  for (sig = 0; signals[sig] != -1; sig++)
    CatchSignal (signals[sig], OnSignal);

  // Set locale (for strtoup(), time/date formatting and so on)
  setlocale (LC_ALL, "");

#ifdef _LINUX_
  gcry_control(GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
#endif
  curl_global_init(CURL_GLOBAL_ALL);
  return true;
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
      exit(2);
    }

    case SIGHUP:
    case SIGINT:
    case SIGQUIT:
    case SIGPIPE:
    case SIGTERM:
    {
      NGL_DEBUG( NGL_LOG(_T("kernel"), NGL_LOG_ERROR, _T("** Caught signal %d\n"), Signal); )
#ifndef _MINUI3_
      App->Quit(1);
#else
      exit(1);
#endif
    }
  }
}

void nglKernel::OnEvent(uint Flags)
{
}

#ifndef _MINUI3_
void nglKernel::EnterModalState()
{
}

void nglKernel::ExitModalState()
{
}
#endif

void nglKernel::NonBlockingHeartBeat()
{
}
