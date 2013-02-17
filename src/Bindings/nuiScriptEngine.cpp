/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

static nglIStream* GetSource(const nglString& rName)
{
  return nglPath(rName).OpenRead();
}

nuiScriptEngine::nuiScriptEngine(const nglString& rName, const nglString& rLanguage)
: mName(rName), mLanguage(rLanguage)
{
  SetSourceGetterDelegate(::GetSource);
}

nuiScriptEngine::~nuiScriptEngine()
{
  
}

void nuiScriptEngine::SetSourceGetterDelegate(const SourceGetterDelegate& rDelegate)
{
  mSourceGetterDelegate = rDelegate;
}

bool nuiScriptEngine::CompileProgram(const nglPath& rSourceName)
{
  nglIStream* pStream = mSourceGetterDelegate(rSourceName.GetPathName());
  if (!pStream)
    return false;
  
  return CompileProgram(rSourceName.GetPathName(), pStream);
}

bool nuiScriptEngine::CompileProgram(const nglString& rSourceName, nglIStream* pStream)
{
  nglString s;
  pStream->ReadText(s);
  bool res = CompileString(rSourceName, s);
  delete pStream;
  return res;
}

const nglString& nuiScriptEngine::GetName() const
{
  return mName;
}

const nglString& nuiScriptEngine::GetLanguage() const
{
  return mLanguage;
}
