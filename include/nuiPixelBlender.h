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
  static inline uint32 lerpRGBA(const uint32 d, const uint32 s, uint32 ti)
  {
    uint32 dstga = d      & 0xFF00FF;
    uint32 dstrb = d >> 8 & 0xFF00FF;
    uint32 srcga = s      & 0xFF00FF;
    uint32 srcrb = s >> 8 & 0xFF00FF;
    uint32 dga = srcga - dstga;
    uint32 drb = srcrb - dstrb;
    dga = (dga * ti) >> 8;  
    drb = (drb * ti) >> 8;  
    const uint32 ga  = (dga + dstga)      & 0x00FF00FF;
    const uint32 rb  = (drb + dstrb) << 8 & 0xFF00FF00;
    return ga | rb;
  }

  static void Blend(uint32& dest_color, const uint32 src_color)
  {
    const uint8 Sa = 255 - ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_A];
    if (Sa == 0)
    {
      dest_color = src_color;
      return;
    }
    if (Sa == 255)
    {
      return;
    }
    
    dest_color = lerpRGBA(src_color, dest_color, Sa + 1);
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

    const uint8 Sa = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_A];
    if (Sa == 0)
      return;
    const uint8 Sr = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_R];
    const uint8 Sg = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_G];
    const uint8 Sb = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_B];
    
    uint8& Dr = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_R];
    uint8& Dg = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_G];
    uint8& Db = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_B];
    
    if (Sa == 255)
    {
      Dr = MIN(255, Sr + Dr);
      Dg = MIN(255, Sg + Dg);
      Db = MIN(255, Sb + Db);
      return;
    }

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
    //const uint8 Sa = ((const uint8*)&src_color)[NUI_RGBA_ENDIANSAFE_A];
    
    uint8& Dr = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_R];
    uint8& Dg = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_G];
    uint8& Db = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_B];
    //uint8& Da = ((uint8*)&dest_color)[NUI_RGBA_ENDIANSAFE_A];
    
    Dr = MIN(255, Dr + Sr);
    Dg = MIN(255, Dg + Sg);
    Db = MIN(255, Db + Sb);
    //Da = MIN(255, Da + Sa);
  }

  static bool CanOptimize()
  {
    return false;
  }
};

