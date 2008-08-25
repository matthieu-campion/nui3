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

#ifndef __ngl_win32__
#define __ngl_win32__

#include <windowsx.h>

/* NGL main window class name
 */
#define NGL_CORE_CLASS    _T("nglCoreClass")
#define NGL_WINDOW_CLASS  _T("nglWindowClass")


/* System font settings (used for console)
 */
#define SYSTEMFONT_NAME  _T("lucida console")
#define SYSTEMFONT_SIZEX 10
#define SYSTEMFONT_SIZEY 6   // unused


/* If it's not in your headers, let's add the mouse wheel code
 */
#ifndef WM_MOUSEWHEEL
# define WM_MOUSEWHEEL  0x020A
# undef  WM_MOUSELAST
# define WM_MOUSELAST   0x020A
#endif // WM_MOUSEWHEEL


/* This is the mysterious 'Unicode codepage' the doc talks about everywhere
 * without telling you which code it is (sigh).
 */
#define CP_WCHAR 1200

/* nglContext last error
 */
#define NGL_CONTEXT_ELAST    4

//#define NGL_USE_DIRECTINPUT
#define _CRT_SECURE_NO_DEPRECATE

#endif // __ngl_win32__
