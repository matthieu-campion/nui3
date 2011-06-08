/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiBindingManager.h"
#include "nuiBindings.h"
#include "nuiScriptEngine.h"




class nuiSpiderMonkey : public nuiScriptEngine
{
public:
  nuiSpiderMonkey(uint32 MaxBytes = 8L * 1024L * 1024L);
  virtual ~nuiSpiderMonkey();

protected:

};


