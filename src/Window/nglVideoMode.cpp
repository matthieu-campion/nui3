/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"


bool nglVideoMode::Find (nglVideoMode& rMode, uint Width, uint Height, uint Depth, uint Frequency)
{
#ifdef _WIN32_
  if (!Depth || ! Frequency)
  {
    DEVMODE mode;

    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);
    if (!Depth) Depth = mode.dmBitsPerPel;
    if (!Frequency) Frequency = mode.dmDisplayFrequency;
  }
#endif  // _WIN32_

  uint i = 0;
  bool have_match = false;
  nglVideoMode mode;

  while (Enum(mode, i++))
  {
    if (Width  == mode.GetWidth() &&
        Height == mode.GetHeight() &&
        Depth  == mode.GetDepth())
    {
      uint freq = mode.GetFrequency();

      if (freq >= Frequency &&
          (!have_match || freq < rMode.GetFrequency()))
      {
        rMode = mode;
        have_match = true;
      }
    }
  }

  return have_match;
}

nglString nglVideoMode::Dump() const
{
  nglString str;
  uint freq = GetFrequency();

  str.Format(_T("%dx%dx%d"), GetWidth(), GetHeight(), GetDepth());
  if (freq)
  {
    nglString freq_str;
    
    freq_str.Format(_T(" (%d Hz)"), freq);
    str += freq_str;
  }
  return str;
}

