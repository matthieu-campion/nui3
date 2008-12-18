/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once



#define NUI_VERSION_MAJOR 3
#define NUI_VERSION_MINOR 0
#define NUI_VERSION_RELEASE 4147
#define NUI_VERSION_BETA (nglString(_T("beta")))

#define NUI_VERSION_MAKE(major, minor, release) ((major << 24) | (minor << 16) | release)

#define NUI_VERSION \
NUI_VERSION_MAKE(NUI_VERSION_MAJOR, NUI_VERSION_MINOR, NUI_VERSION_RELEASE)

#define NUI_VERSION_COMPARE(major, minor, release) \
(NUI_VERSION_MAKE(major, minor, release) - NUI_VERSION)


/*
 * NGL version
 */

#define NGL_VERSION_MAJOR NUI_VERSION_MAJOR
#define NGL_VERSION_MINOR NUI_VERSION_MINOR
#define NGL_VERSION_RELEASE NUI_VERSION_RELEASE

#define NGL_VERSION NUI_VERSION

