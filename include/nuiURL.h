/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

class nuiURL
{
public:
  nuiURL(const nglString& rUrl);
  virtual ~nuiURL();
  
  bool OpenBrowser();
  bool IsNull();
  
private:
  nglString mUrl;
};

