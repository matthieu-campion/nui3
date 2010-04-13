/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiScriptEngine
{
public:
  nuiScriptEngine(const nglString& rName, const nglString& rLanguage);
  virtual ~nuiScriptEngine();
  
  const nglString& GetName() const;
  const nglString& GetLanguage() const;
  
  typedef nuiFastDelegate1<const nglString&, nglIStream*> SourceGetterDelegate;
  void SetSourceGetterDelegate(const SourceGetterDelegate& rDelegate);
  
  virtual void SetGlobal(const nglString& rName, const nuiVariant& rVariant) = 0;
  virtual nuiVariant GetGlobal(const nglString& rName) const = 0;
  
  virtual nuiVariant ExecuteExpression(const nglString& rExpression) = 0;
  virtual bool CompileProgram(const nglString& rSourceName, const nglString& rProgram) = 0;
  bool CompileProgram(const nglPath& rSourceName);
  bool CompileProgram(const nglString& rSourceName, nglIStream* pStream);
  
protected:
  nglString mName;
  nglString mLanguage;
  SourceGetterDelegate mSourceGetterDelegate;
  
};
