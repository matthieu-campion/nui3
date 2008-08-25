/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"


const int32 NUI_FP_SHIFT = 16;
const int32 NUI_FP_ONE = (1 << NUI_FP_SHIFT);
const int32 NUI_FP_MASK = NUI_FP_ONE - 1;
const int32 NUI_FP_HALF = (NUI_FP_ONE >> 1);

typedef int32 ifp32;

inline ifp32 Frac(ifp32 i)
{
  return i & NUI_FP_MASK;
}

inline ifp32 Trunc(ifp32 i)
{
  return i & (~NUI_FP_MASK);
}

inline ifp32 ToZero(int32 i)
{
  return i >> NUI_FP_SHIFT;
}

inline int32 ToBelow(ifp32 i)
{
  return i >> NUI_FP_SHIFT;
}

inline int32 ToAbove(ifp32 i)
{
  return (i + NUI_FP_MASK) >> NUI_FP_SHIFT;
}

inline int32 ToNearest(ifp32 i)
{
  int32 frac = Frac(i);
  int32 res = ToZero(i);
  if (frac > NUI_FP_HALF)
    return res + 1;
  return res;
}


inline int32 nuiFPDiv(ifp32 a, ifp32 b)
{
  if (b <= NUI_FP_ONE)
  {
    int64 cc = 0x40000000 / b;
    cc = (cc * a) >> 14;
    ifp32 ret = (int32)(cc & 0xffffffff);
    return ret;
  } 
  
  int64 aa = a;
  int64 bb = b;
  aa <<= NUI_FP_SHIFT;
  aa /= bb;
  ifp32 ret = (ifp32)(aa & 0xffffffff);
  return ret;
}

inline ifp32 nuiFPMul(ifp32 a, ifp32 b)
////////////////////////////////////////
{
  int64 aa = a;
  int64 bb = b;
  int64 cc = aa * bb;
  cc >>= NUI_FP_SHIFT;
  return (ifp32)(cc & 0xffffffff);
}

