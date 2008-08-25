/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
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
