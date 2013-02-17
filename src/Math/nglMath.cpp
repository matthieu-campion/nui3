/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


uint Log2Ceil(uint Value)
{
  uint val, bit = 0;

  do
  {
    bit++;
    val = 1 << bit;
  }
  while (val && Value > val);

  return bit;
}

uint Log2Floor(uint Value)
{
  uint val, bit = 0;

  do
  {
    bit++;
    val = 1 << bit;
  }
  while (val && Value > val);

  if (val != Value)
     bit--;

  return bit;
}
