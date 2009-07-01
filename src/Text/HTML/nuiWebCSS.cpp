/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiWebCSS.h"
#include "nuiParser.h"

nuiWebCSS::nuiWebCSS()
: mpParser(NULL)
{
  
}

nuiWebCSS::~nuiWebCSS()
{
  delete mpParser;
}

bool nuiWebCSS::Load(nglIStream* pStream, const nglString& rSourceURL)
{
  bool res = false;
  mpParser = new nuiParser(pStream, rSourceURL);
  
  res = ParseBody();
  
  delete mpParser;
  mpParser = NULL;
  
  return res;
}

bool nuiWebCSS::Load(const nglString& rString)
{
  bool res = false;
  char* utf8 = rString.Export(eUTF8);
  nglIMemory* pMem = new nglIMemory(utf8, strlen(utf8));
  mpParser = new nuiParser(pMem, _T("inline"));
  
  res = ParseBody();
  
  delete mpParser;
  mpParser = NULL;
  delete utf8;
  return res;
}


bool nuiWebCSS::ParseBody()
{
  while (!mpParser->IsDone())
  {
    if (!mpParser->SkipBlank())
      return false;
//    if (!mpParser->())
      return false;
  }
  return true; 
}

