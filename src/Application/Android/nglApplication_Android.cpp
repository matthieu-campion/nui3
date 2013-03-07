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
#include "nglApplication.h"
#include "nglTimer.h"
#include "nglWindow.h"
#include <math.h>


using namespace std;


// #define NGL_APP_ENONE  0 (in nglApplication.h)
// Other defines in ./ngl_unix.h

#define NGL_APP_EXOPEN   1

const nglChar* gpApplicationErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Couldn't open X display"),
  NULL
};


/*
 * Life cycle
 */

nglApplication::nglApplication()
{
  mExitReq = false;
  mExitCode = 0;
  mUseIdle = false;
  mLastIdleCall = 0.0f;

  // nglApplication is a kernel's client, just as plugin instances
  IncRef();
}

nglApplication::~nglApplication()
{
}

void nglApplication::Quit(int Code)
{
  mExitReq = true;
  mExitCode = Code;
}



/*
 * Application entry point
 */

int nglApplication::Main (int ArgCnt, char const ** pArg)
{
  if (!Init(ArgCnt, pArg))
    return 1;

  CallOnInit();           // Call user OnInit() call back
  SysLoop();              // Run event pump till application ending
  CallOnExit(mExitCode);  // Call user OnExit() call back

  Exit();
  return mExitCode;
}

bool nglApplication::Init (int ArgCnt, char const** pArg)
{
  int i;

  if (!SysInit())
    return false;

  /* Register ourselves for Idle and (optional) X11 events
  */
  AddEvent(this);

  // Store user args in mArgs
  for (i = 1; i < ArgCnt; i++)
    AddArg(nglString(pArg[i]));

  // Fetch application's name from argv[0]
  nglString arg0 = nglString(pArg[0]);
  nglString name = arg0;

  i = name.FindLast (_T('/'));
  if (i != -1)
    name.DeleteLeft (i + 1); // Only keep file name if it's a full path
  SetName(name);

  // Fetch application's executable path
  nglPath path;

  /* Let's try the proc interface, and fallback to argv[0]
   */
  char buffer[PATH_MAX + 1];
  int  buffer_len = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (buffer_len > 0)
  {
    buffer[buffer_len] = 0;
    path = nglPath(buffer);
  }
  else
  {
    // Build application binary's file path from current dir and argv[0]
    path = nglPath(ePathCurrent) + arg0;
    path.Canonize();
  }
  SetPath(path);

  return true;
}

void nglApplication::Exit()
{
  DelEvent(this);

  mTimers.clear();
  mEvents.clear();

  nglKernel::Exit(0);
}


/*
 * Event management
 */

void nglApplication::AddEvent (class nglEvent* pEvent)
{
  mEvents.push_front (pEvent);
}

void nglApplication::DelEvent (class nglEvent* pEvent)
{
  mEvents.remove (pEvent);
}

void nglApplication::AddTimer (nglTimer* pTimer)
{
  mTimers.push_front (pTimer);
}

void nglApplication::DelTimer (nglTimer* pTimer)
{
  mTimers.remove (pTimer);
}

#define DBG_EVENT(x)

int nglApplication::SysLoop()
{
  while (!mExitReq)
  {
DBG_EVENT( NGL_OUT(_T("\nEvent loop entry\n")); )

    /* Compose fd sets
     */
    int fd_max = -1;
    bool use_idle = false;
    fd_set r_set, w_set, e_set;
    EventList::iterator event;

    FD_ZERO (&r_set);
    FD_ZERO (&w_set);
    FD_ZERO (&e_set);
DBG_EVENT( NGL_OUT(_T(" building fd sets:\n")); )
    for (event = mEvents.begin(); event != mEvents.end(); event++)
    {
      nglEvent* e = *event;

      // Collect select flags
      int fd = e->GetFD();
      int flags = e->GetFlags();
      if (fd >= 0)
      {
DBG_EVENT( NGL_OUT("  fd=%d  flags=%c%c%c%c\n", fd, (flags & nglEvent::Read)?'R':'_', (flags & nglEvent::Write)?'W':'_', (flags & nglEvent::Error)?'E':'_', (flags & nglEvent::Idle)?'I':'_'); )
        if (flags & nglEvent::Read) FD_SET (fd, &r_set);
        if (flags & nglEvent::Write) FD_SET (fd, &w_set);
        if (flags & nglEvent::Error) FD_SET (fd, &e_set);
        if (fd > fd_max) fd_max = fd;
      }
      if (flags & nglEvent::Idle)
        use_idle = true;
    }

    /* Find out select() timeout
     */
    struct timeval tv;
    struct timeval* ptv = &tv;
    bool use_timer = false;

    if (use_idle)
    {
      tv.tv_sec = 0;
      tv.tv_usec = 0;
DBG_EVENT( NGL_OUT(_T(" idle used\n")); )
    }
    else
    {
      TimerList::iterator timer;
      nglTime now;
      nglTime timeout = 1E10;

      for (timer = mTimers.begin(); timer != mTimers.end(); timer++)
      {
        nglTimer* t = *timer;

        if (t->IsRunning())
        {
          nglTime self = t->GetTimeOut(now);

          if (timeout > self)
            timeout = self;
          use_timer = true;
DBG_EVENT( NGL_OUT(_T(" timer: self.timeout=%.3f  loop.timeout=%.3f\n"), (double)self, (double)timeout); )
        }
      }

      if (use_timer)
      {
        double sec;

        tv.tv_usec = (long) (modf(timeout, &sec) * 1E6f);
        tv.tv_sec = (long) sec;
        ptv = &tv;
      }
      else
        ptv = NULL; // No idle, no timer, block indefinitely
    }

    /* Wait for event
     */
DBG_EVENT( NGL_OUT(" select(): "); )
    int fd_count = select (fd_max+1, &r_set, &w_set, &e_set, ptv);
DBG_EVENT( NGL_OUT("fd_count=%d\n", fd_count); )

    /* Dispatch 'regular' event
     */
    if (fd_count > 0)
    {
DBG_EVENT( NGL_OUT(" dispatching events:\n"); )
      int fd_todo = fd_count;

      for (event = mEvents.begin(); fd_todo > 0 && event != mEvents.end(); event++)
      {
        int fd = (*event)->GetFD();
        if (fd >= 0)
        {
          int flags = 0;

          if (FD_ISSET(fd, &r_set)) flags |= nglEvent::Read;
          if (FD_ISSET(fd, &w_set)) flags |= nglEvent::Write;
          if (FD_ISSET(fd, &e_set)) flags |= nglEvent::Error;
          if (flags)
          {
DBG_EVENT( NGL_OUT("  fd=%d  flags=%c%c%c%c\n", fd, (flags & nglEvent::Read)?'R':'_', (flags & nglEvent::Write)?'W':'_', (flags & nglEvent::Error)?'E':'_', (flags & nglEvent::Idle)?'I':'_'); )
            (*event)->CallOnEvent (flags);
            fd_todo--;
          }
        }
      }
    }

    /* Dispatch timer events
     */
    if (use_timer)
    {
DBG_EVENT( NGL_OUT(" dispatching timer events\n"); )
      TimerList::iterator timer;

      for (timer = mTimers.begin(); timer != mTimers.end(); timer++)
        (*timer)->Update();
    }

    /* Dispatch idle events
     */
    if (fd_count == 0 && !use_timer && use_idle)
    {
DBG_EVENT( NGL_OUT(" dispatching idle event\n"); )
      for (event = mEvents.begin(); event != mEvents.end(); event++)
      {
        nglEvent* e = *event;

        if (e->GetFlags() & nglEvent::Idle)
          e->CallOnEvent(nglEvent::Idle);
      }
    }

DBG_EVENT( NGL_OUT(" event loop done\n"); )
  }

  return mExitCode;
}

/* The application registers itself to the event loop with :
 * - nglEvent::Idle set if UseIdle() was called
 * - nglEvent::{Rear,Errpr}, FD = X connection descriptor (X11 only)
 */
void nglApplication::OnEvent(uint Flags)
{
  if (Flags & nglEvent::Idle)
    CallOnIdle();

#ifdef _X11_
  if (!(Flags & nglEvent::Read)) return;

  int count = XPending (mpDisplay);
  if (count == 0)
    return;

  int i;
  for (i = 0; i < count; i++)
  {
    XEvent event;
    nglWindow* win;

    XNextEvent (mpDisplay, &event);
    win = mWindows[event.xany.window];
    if (win)
      win->OnXEvent(event);
  }
#else
  NGL_DEBUG( NGL_LOG("app", NGL_LOG_INFO, _T("OnEvent(Flags: %d)\n"), Flags); )
#endif // _X11_
}



/*
 * X11 event management
 */

#ifdef _X11_
void* nglApplication::GetDisplay()
{
  if (!mpDisplay)
  {
    /* Create X connection on first need
     */
    mpDisplay = XOpenDisplay (NULL);
    if (mpDisplay == NULL)
    {
      SetError (_T("app"), NGL_APP_EXOPEN);
      return NULL;
    }
    mFD = ConnectionNumber (mpDisplay);
    mFlags |= nglEvent::Read | nglEvent::Error;
  }

  return mpDisplay;
}

void nglApplication::AddWindow (class nglWindow* pWin)
{
  Window handle = pWin->GetHandle();

  if (handle)
    mWindows[handle] = pWin;
}

void nglApplication::DelWindow (class nglWindow* pWin)
{
  Window handle = pWin->GetHandle();

  if (handle)
    mWindows.erase(mWindows.find(handle));
}
#endif // _X11_

void nglApplication::EnterModalState()
{
	mModalState = true;
	while (!mExitReq && mModalState)
  {
DBG_EVENT( NGL_OUT(_T("\nEvent loop entry\n")); )

#ifdef _X11_
    /* This XPending call is there for two reasons :
     *  - it flushes the output buffer (sending all pending X commands)
     *  - it checks if some events are _already_ read from server (ie. in client
     *    queue), since such prefetched events would not raise a read event on the
     *    X connection descriptor
     */
    if (mpDisplay)
    {
      int pending = XPending(mpDisplay);
      if (pending > 0)
      {
DBG_EVENT( NGL_OUT(_T(" XPending: %d\n"), pending); )
         OnEvent(nglEvent::Read); // Fake a read event on X's connection descriptor
         continue; // Apply to the topmost while() scope here
      }
    }
#endif // _X11_

    /* Compose fd sets
     */
    int fd_max = -1;
    bool use_idle = false;
    fd_set r_set, w_set, e_set;
    EventList::iterator event;

    FD_ZERO (&r_set);
    FD_ZERO (&w_set);
    FD_ZERO (&e_set);
DBG_EVENT( NGL_OUT(_T(" building fd sets:\n")); )
    for (event = mEvents.begin(); event != mEvents.end(); event++)
    {
      nglEvent* e = *event;

      // Collect select flags
      int fd = e->GetFD();
      int flags = e->GetFlags();
      if (fd >= 0)
      {
DBG_EVENT( NGL_OUT("  fd=%d  flags=%c%c%c%c\n", fd, (flags & nglEvent::Read)?'R':'_', (flags & nglEvent::Write)?'W':'_', (flags & nglEvent::Error)?'E':'_', (flags & nglEvent::Idle)?'I':'_'); )
        if (flags & nglEvent::Read) FD_SET (fd, &r_set);
        if (flags & nglEvent::Write) FD_SET (fd, &w_set);
        if (flags & nglEvent::Error) FD_SET (fd, &e_set);
        if (fd > fd_max) fd_max = fd;
      }
      if (flags & nglEvent::Idle)
        use_idle = true;
    }

    /* Find out select() timeout
     */
    struct timeval tv;
    struct timeval* ptv = &tv;
    bool use_timer = false;

    if (use_idle)
    {
      tv.tv_sec = 0;
      tv.tv_usec = 0;
DBG_EVENT( NGL_OUT(_T(" idle used\n")); )
    }
    else
    {
      TimerList::iterator timer;
      nglTime now;
      nglTime timeout = 1E10;

      for (timer = mTimers.begin(); timer != mTimers.end(); timer++)
      {
        nglTimer* t = *timer;

        if (t->IsRunning())
        {
          nglTime self = t->GetTimeOut(now);

          if (timeout > self)
            timeout = self;
          use_timer = true;
DBG_EVENT( NGL_OUT(_T(" timer: self.timeout=%.3f  loop.timeout=%.3f\n"), (double)self, (double)timeout); )
        }
      }

      if (use_timer)
      {
        double sec;

        tv.tv_usec = (long) (modf(timeout, &sec) * 1E6f);
        tv.tv_sec = (long) sec;
        ptv = &tv;
      }
      else
        ptv = NULL; // No idle, no timer, block indefinitely
    }

    /* Wait for event
     */
DBG_EVENT( NGL_OUT(" select(): "); )
    int fd_count = select (fd_max+1, &r_set, &w_set, &e_set, ptv);
DBG_EVENT( NGL_OUT("fd_count=%d\n", fd_count); )

    /* Dispatch 'regular' event
     */
    if (fd_count > 0)
    {
DBG_EVENT( NGL_OUT(" dispatching events:\n"); )
      int fd_todo = fd_count;

      for (event = mEvents.begin(); fd_todo > 0 && event != mEvents.end(); event++)
      {
        int fd = (*event)->GetFD();
        if (fd >= 0)
        {
          int flags = 0;

          if (FD_ISSET(fd, &r_set)) flags |= nglEvent::Read;
          if (FD_ISSET(fd, &w_set)) flags |= nglEvent::Write;
          if (FD_ISSET(fd, &e_set)) flags |= nglEvent::Error;
          if (flags)
          {
DBG_EVENT( NGL_OUT("  fd=%d  flags=%c%c%c%c\n", fd, (flags & nglEvent::Read)?'R':'_', (flags & nglEvent::Write)?'W':'_', (flags & nglEvent::Error)?'E':'_', (flags & nglEvent::Idle)?'I':'_'); )
            (*event)->CallOnEvent (flags);
            fd_todo--;
          }
        }
      }
    }

    /* Dispatch timer events
     */
    if (use_timer)
    {
DBG_EVENT( NGL_OUT(" dispatching timer events\n"); )
      TimerList::iterator timer;

      for (timer = mTimers.begin(); timer != mTimers.end(); timer++)
        (*timer)->Update();
    }

    /* Dispatch idle events
     */
    if (fd_count == 0 && !use_timer && use_idle)
    {
DBG_EVENT( NGL_OUT(" dispatching idle event\n"); )
      for (event = mEvents.begin(); event != mEvents.end(); event++)
      {
        nglEvent* e = *event;

        if (e->GetFlags() & nglEvent::Idle)
          e->CallOnEvent(nglEvent::Idle);
      }
    }

DBG_EVENT( NGL_OUT(" event loop done\n"); )
  }
}

void nglApplication::ExitModalState()
{
	mModalState = false;
}
