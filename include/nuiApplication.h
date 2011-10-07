/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglApplication.h"

//class nuiScriptEngine;

class nuiApplication : public nglApplication
{
public:
  nuiApplication();
  virtual ~nuiApplication();
  
  /*
  void SetScriptEngine(nuiScriptEngine* pEngine);
  nuiScriptEngine* GetScriptEngine() const;

  void EnableDefaultScriptEngine();
  */
  
protected:
//  nuiScriptEngine* mpScriptEngine;
};

