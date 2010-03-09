/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once
#include <limits.h>

class nglConditionPrivate;

class nglCondition
{

public:
  nglCondition();
  ~nglCondition();

  void WakeOne();
  void WakeAll();

  bool Wait(uint32 time = ULONG_MAX);

private:
  nglConditionPrivate *mpData;
};
