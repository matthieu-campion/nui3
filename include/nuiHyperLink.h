/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiLabel.h"

class nuiHyperLink : public nuiLabel
{
public:
  nuiHyperLink();
  nuiHyperLink(const nglString& rURL, const nglString& rLabel);
  
  void SetURL(const nglString& rURL);
  const nglString& GetURL() const;

  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual void SetHover(bool Hover);
  
  nuiSimpleEventSource<0> Activated;
  
protected:
  
  nglString mURL;
  uint32 mClicked;
  
private:
  
  void InitAttributes();
  
};