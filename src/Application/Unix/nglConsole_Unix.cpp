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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define NGL_CONSOLE_PROMPT "> "


nglConsole::nglConsole(bool IsVisible)
#ifdef USE_WCHAR
  : mOutputConv(eEncodingInternal, eEncodingNative)
#endif
{
  Setup(); // Portable code init

  mIsVisible = IsVisible;
  mFD = STDIN_FILENO;
  if (!isatty (mFD))
  {
    mFlags = 0;
    NGL_LOG(_T("console"), NGL_LOG_WARNING, _T("Warning: not connected to a tty, interactive console disabled\n"));
    return;
  }
  mFlags = nglEvent::Read | nglEvent::Error;
  App->AddEvent (this);

  if (mIsVisible) NGL_OUT (nglString(NGL_CONSOLE_PROMPT));
}

nglConsole::~nglConsole()
{
  App->DelEvent (this);
}


/*
 * Public methods
 */

void nglConsole::Show (bool IsVisible)
{
  mIsVisible = IsVisible;
}


/*
 * Internals
 */

void nglConsole::OnOutput (const nglString& rText)
{
#ifdef USE_WCHAR
  #define NGL_OUTPUT_BUFFER_SIZE 1024

  // 'wchar_t' mode : string buffer is in unicode, convert with fault tolerance
  int offset = 0;
  char buffer[NGL_OUTPUT_BUFFER_SIZE + 1];

  do
  {
    int to_write = NGL_OUTPUT_BUFFER_SIZE;
    int to_write0 = to_write;

    rText.Export(offset, buffer, to_write, mOutputConv);
    buffer[to_write0 - to_write] = '\0';
    printf (buffer);
  }
  while (mOutputConv.GetState() != eStringConv_OK);
#else
  // 'char' mode : string buffer is considered to use the locale's encoding
  printf (rText.GetChars());
#endif
  fflush (stdout);
}

#define IN_BUFFER_SIZE 1024

void nglConsole::OnEvent(uint Flags)
{
  char buffer[IN_BUFFER_SIZE+1];
  int count;

  count = read (mFD, buffer, IN_BUFFER_SIZE);
  if (count > 0)
  {
    buffer[count] = '\0';
    mInputBuffer = buffer;
    Input (mInputBuffer);
    if (mIsVisible)
      NGL_OUT (nglString(NGL_CONSOLE_PROMPT)); // FIXME
  }
}
