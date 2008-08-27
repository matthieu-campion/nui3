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
  
  std::string str = mUrl.GetStdString();
 ShellExecuteA(NULL, "open", str.c_str(), NULL, NULL, SW_SHOWNORMAL);

 return true;
}