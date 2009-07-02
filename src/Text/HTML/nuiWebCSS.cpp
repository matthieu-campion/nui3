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

bool nuiWebCSS::Load(const nglString& rString, const nglString& rSourceURL)
{
  bool res = false;
  char* utf8 = rString.Export(eUTF8);
  nglIMemory* pMem = new nglIMemory(utf8, strlen(utf8));
  mpParser = new nuiParser(pMem, rSourceURL);
  
  res = ParseBody();
  
  delete mpParser;
  mpParser = NULL;
  delete utf8;
  return res;
}

bool nuiWebCSS::ParseComment()
{
  return true;
}

bool nuiWebCSS::ParseProperty()
{
  return true;
}

bool nuiWebCSS::ParseValue()
{
  return true;
}

bool nuiWebCSS::ParseAction()
{
  if (!ParseProperty())
    return false;
  if (!ParseValue())
    return false;
  return true;
}


bool nuiWebCSS::ParseActions()
{
  while (1)
  {
    if (!ParseAction())
      return false;
  }
  return true;
}

bool nuiWebCSS::ParseSelector()
{
  return true;
}

bool nuiWebCSS::ParseSelectors()
{
  while (1)
  {
    if (!ParseSelector())
      return false;
  }
  return true;
}

bool nuiWebCSS::ParseRule()
{
  if (!ParseSelectors())
    return false;
  if (!ParseActions())
    return false;
  return true;
}

bool nuiWebCSS::ParseCharset()
{
  return true;
}

bool nuiWebCSS::ParseBody()
{
  if (!ParseCharset())
    return false;
  while (!mpParser->IsDone())
  {
    if (!mpParser->SkipBlank())
      return false;
    if (!ParseRule())
      return false;
  }
  return true; 
}

