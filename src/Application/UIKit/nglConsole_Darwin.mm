/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define NGL_CONSOLE_PROMPT _T("> ")


nglConsole::nglConsole(bool IsVisible)
{
  mIsVisible = IsVisible;
  if (mIsVisible)
    NGL_OUT(NGL_CONSOLE_PROMPT);
}

nglConsole::~nglConsole()
{
}


/*
 * Public methods
 */

void nglConsole::Show(bool IsVisible)
{
  mIsVisible = IsVisible;
}

/*
 * Internals
 */

void nglConsole::OnOutput(const nglString& rText)
{
  printf("%s", rText.GetChars());
  fflush(stdout);
}

#define BUFFER_MAX 4096

