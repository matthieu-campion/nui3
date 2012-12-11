/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nui_all.h
\brief NUI / VC Precompiled headers files.

This file includes all nui needed headers to ease the creation of precompiled headers outside of the ngl project in VC.
*/

#ifndef __nui_h__
#define __nui_h__

#ifndef NGL_STATIC_BUILD
#define NGL_STATIC_BUILD // Fake the static build! Always!
#endif

#ifdef _WIN32_
#ifdef NGL_STATIC_BUILD
#define NUI_API
#else
#ifdef NUI_EXPORTS
#define NUI_API __declspec(dllexport)
#else
#define NUI_API __declspec(dllimport)
#endif
#endif
#else
#define NUI_API
#endif

#include "ngl.h"

#ifdef _WIN32_
//#include <crtdbg.h>
#endif // _WIN32_

//#define _TU_USE_STL 1

#define NUI_FONTDB_PATH _T("nuiFonts.db5")
#include "nui_all.h"

#endif // __nui_h__
