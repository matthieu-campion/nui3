/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once
#include <limits.h>
#include "nuiNonCopyable.h"

class nglConditionPrivate;

class nglCondition : nuiNonCopyable
{

public:
  nglCondition();
  ~nglCondition();

  void WakeOne();
  void WakeAll();

  bool Wait(uint32 time =  0xffffffff);

private:
  nglConditionPrivate *mpData;
};
