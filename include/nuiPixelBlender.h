/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

class nuiPixelBlender_Copy
{
public:
  static void Blend(uint32& dest_color, const uint32 src_color)
  {
    dest_color = src_color;
  }

  static bool CanOptimize()
  {
    return true;
  }
};

class nuiPixelBlender_Add32
{
public:
  static void Blend(uint32& dest_color, const uint32 src_color)
  {
    dest_color += src_color;
  }

  static bool CanOptimize()
  {
    return false;
  }
};

class nuiPixelBlender_Transp
{
public:
  static void Blend(uint32& dest_color, const uint32 src_color)
  {
    //dest_color = src_color; return; //#TEST

    const uint8 Sr = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_R];
    const uint8 Sg = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_G];
    const uint8 Sb = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_B];
    const uint8 Sa = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_A];
    
    uint8& Dr = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_R];
    uint8& Dg = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_G];
    uint8& Db = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_B];
    
    const int16 SA = Sa + 1;
    const int16 SMA = (1 << 8) - SA;
    
    Dr = (SMA * Dr + SA * Sr) >> 8;
    Dg = (SMA * Dg + SA * Sg) >> 8;
    Db = (SMA * Db + SA * Sb) >> 8;
  }
  
  static bool CanOptimize()
  {
    return false;
  }
};

class nuiPixelBlender_TranspAdd
{
public:
  static void Blend(uint32& dest_color, const uint32 src_color)
  {
    //dest_color = src_color; return; //#TEST

    const uint8 Sr = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_R];
    const uint8 Sg = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_G];
    const uint8 Sb = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_B];
    const uint8 Sa = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_A];
    
    uint8& Dr = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_R];
    uint8& Dg = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_G];
    uint8& Db = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_B];
    
    const int16 SA = Sa + 1;
    
    Dr = MIN(255, Dr + ((SA * Sr) >> 8));
    Dg = MIN(255, Dg + ((SA * Sg) >> 8));
    Db = MIN(255, Db + ((SA * Sb) >> 8));
  }

  static bool CanOptimize()
  {
    return false;
  }
};

class nuiPixelBlender_Add
{
public:
  static void Blend(uint32& dest_color, const uint32 src_color)
  {
    //dest_color = src_color; return; //#TEST

    const uint8 Sr = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_R];
    const uint8 Sg = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_G];
    const uint8 Sb = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_B];
    const uint8 Sa = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_A];
    
    uint8& Dr = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_R];
    uint8& Dg = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_G];
    uint8& Db = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_B];
    uint8& Da = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_A];
    
    Dr = MIN(255, Dr + Sr);
    Dg = MIN(255, Dg + Sg);
    Db = MIN(255, Db + Sb);
    Da = MIN(255, Da + Sa);
  }

  static bool CanOptimize()
  {
    return false;
  }
};

