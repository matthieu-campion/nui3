/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  ngl_all.h
\brief All includes in one header file.

This file includes all ngl headers to ease the creation of precompiled
headers outside of the ngl project in VC. Just include it in your
precompiled header central file and everything should be rolling.
Lookup precompiled headers in the VC doc to learn how to use them.
*/

#ifndef __ngl_all_h__
#define __ngl_all_h__

//#include "nui.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>        // for FLT_MAX
#include <limits.h>        // for INT_MAX
#include <locale.h>
#include <math.h>        // for log exp sqrt etc, and maybe sqrtf
#ifndef __LINUX__
#include <setjmp.h>
#endif
#include <stdarg.h>
#include <stdio.h>        // for printf debugging
#include <stdlib.h>        // for malloc
#include <string.h>        // for strstr, because gluCheckExtension isn't available in libglu32 on windows
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>
//#include <hash_map>

#ifdef _MACOSX_
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/mount.h>
#endif

#if (defined _WIN32_) && !(defined WINCE)
#include <shlobj.h>
#endif

#if (defined _WIN32_) && !(defined WINCE)
#include <io.h>
#include <direct.h>
#include <shlobj.h>
#include <shellapi.h>
#include <winioctl.h>
#define PATH_MAX MAX_PATH
#define F_OK 0
#define R_OK 4
#define W_OK 2
#pragma warning(disable: 4996)
#elif __APPLE__
#include <unistd.h>
#include <sys/types.h>
#elif WINCE
#include <shlobj.h>
#include <winioctl.h>
#define PATH_MAX MAX_PATH
#define F_OK 0
#define R_OK 4
#define W_OK 2
#pragma warning(disable: 4996)
#else
#include <unistd.h>
#include <sys/types.h>
#endif // _WIN32_


#if defined(__APPLE__)     // for ntohl byteswapping, should be a no-op on PPC and __bswap on x86.
#include <arpa/inet.h>
#else
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#endif

#ifndef WIN32
#include <dirent.h>
#endif

//#if (((defined _DEBUG) || (defined DEBUG)) || (defined _DEBUG_)) 
//#include "mmgr.h"
//#endif


#include "nglKernel.h"
#include "nglTime.h"
#include "nglPath.h"

#include "ngl3DSLoader.h"
#include "nglApplication.h"
#include "nglBitmapTools.h"
#include "nglCPUInfo.h"

#ifndef _NOCLIPBOARD_
#include "nglClipBoard.h"
#endif

#include "nglConsole.h"
#include "nglContext.h"

#if (!defined _NOCLIPBOARD_ || !defined _NODND_)
#include "nglDataObjects.h"
#include "nglDataTypesRegistry.h"
#endif

#include "nglDeviceInfo.h"

#ifndef _NODND_
#include "nglDragAndDropObjects.h"
#endif

#include "nglEndian.h"
#include "nglError.h"
#include "nglEvent.h"
#include "nglFile.h"
#include "nglFont.h"
#include "nglFontBase.h"
#include "nglFontCache.h"
#include "nglFontLayout.h"
#include "nglIFile.h"
#include "nglIMemory.h"
#include "nglIOFile.h"
#include "nglIOStream.h"
#include "nglIStream.h"
#include "nglIZip.h"
#include "nglImage.h"
#include "nglImageCodec.h"
#include "nglInputDevice.h"
#include "nglKeyboard.h"
#include "nglLog.h"
#include "nglMath.h"
#include "nglMatrix.h"
#include "nglMimeSource.h"
#include "nglModule.h"
#include "nglOFile.h"
#include "nglOMemory.h"
#include "nglOStream.h"
#include "nglPlugin.h"
#include "nglQuaternion.h"
#include "nglStream.h"
#include "nglString.h"
#include "nglTimer.h"
#include "nglVector.h"
#include "nglVideoMode.h"
#include "nglWindow.h"
#include "nglZipFS.h"

#include "nglMath.h"

#endif // __ngl_all_h__

