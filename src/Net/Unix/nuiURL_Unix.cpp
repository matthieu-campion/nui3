/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiURL.h"

nuiURL::nuiURL(const nglString& rUrl)
{
  mUrl = rUrl;
}

nuiURL::~nuiURL()
{
  
}

bool nuiURL::IsNull()
{
  return (mUrl == nglString::Null);
}

bool nuiURL::OpenBrowser()
{
  if (mUrl == nglString::Null)
    return false;
  
  //#TODO need to launch the default browser with the given URL here... 
  return true;
}
