/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglCPUInfo.h
\brief CPU identification and features detection
*/

#ifndef __nglCPUInfo_h__
#define __nglCPUInfo_h__

//#include "nui.h"
#include "nglString.h"


//! CPU identification and features detection
/*!
This class is a holder for static methods which identifies the hosting
architecture (see GetType()), the number of CPUs (see GetCount()) and
the different proprietary extensions.
*/
class NGL_API nglCPUInfo
{
public:
  /// CPU family
  enum Family
  {
    eUnknown,
    eIA32,
    eIA64,
    ePPC,
    eAlpha,
    eMIPS
  };

  static Family GetFamily();   ///< return the host CPU family
  static uint   GetCount();    ///< return the number of running CPU(s)
  static bool   HasMMX();      ///< return true if MMX extensions are available
  static bool   HasSSE();      ///< return true if SSE extensions are available
  static bool   HasSSE2();     ///< return true if SSE2 extensions are available
  static bool   Has3DNow();    ///< return true if 3DNow extensions are available
  static bool   HasAltivec();  ///< return true if Altivec extensions are available

  static nglString Dump();     ///< returns a human readable description of the host CPU(s)

private:
  static Family mFamily;
  static uint   mCount;
  
  static bool mMMX;
  static bool mSSE;
  static bool mSSE2;
  static bool m3DNow;
  static bool mAltivec;

  nglCPUInfo();
  static void FillCPUInfo();
};

#endif // __nglCPUInfo_h__
