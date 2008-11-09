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

#include <map>
#include <list>
#include <stack>
#include <vector>
#include <map>
#include <string>

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

