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
  mpNSApplication = NULL;

  Init();
  
/*
  // If we're attached to a tty, save its settings for segfault handler
  if (isatty(STDIN_FILENO)) tcgetattr (STDIN_FILENO, &mTermInfo);
*/
}

nglKernel::~nglKernel()
{
//  printf("nglKernel::dtor\n");
/*
  if (isatty(STDIN_FILENO)) tcsetattr (STDIN_FILENO, TCSANOW, &mTermInfo);
*/
  Exit(0);

//  printf("nglKernel::dtor OK\n");
}


/*
 * Internals (generic kernel setup)
 */

bool nglKernel::SysInit()
{
  // Set locale (for strtoup(), time/date formatting and so on)
  setlocale (LC_ALL, "");

//#FIXME: use the new apple File System Events API here

  return true;
}

void nglKernel::NonBlockingHeartBeat()
{
  // #TODO #FIXME
  // How can we keep the UI alive in cocoa?
}


void objCCallOnInit(void* pUIApplication)
{
  NGL_ASSERT(App);
  App->mpNSApplication = pUIApplication;
  App->CallOnInit();
}

void objCCallOnInitWithURL(void* pUIApplication, const nglString &url)
{
	NGL_ASSERT(App);
	App->mpNSApplication = pUIApplication;
	App->AddArg(_T("--openurl"));
	App->AddArg(url);
	App->CallOnInit();
}

void objCCallOnWillExit()
{
  NGL_ASSERT(App);
	//App->CallOnWillExit();
}

void objCCallOnExit(int Code)
{
  NGL_ASSERT(App);
  App->CallOnExit(Code);
/*
NGL_OUT(_T("objCCallOnExit trying to release our window\n"));
///< release our Window
NGL_ASSERT(App->mpNSApplication);
UIWindow* pWindow = [(UIApplication*)App->mpNSApplication keyWindow];
NGL_ASSERT(pWindow);
if (pWindow) {
  NGL_OUT(_T("objCCallOnExit about to release our window\n"));
  [pWindow release];
}

NGL_OUT(_T("objCCallOnExit App->CallOnExit\n"));
*/
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

