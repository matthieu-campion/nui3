/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiFormater_h__
#define __nuiFormater_h__

//#include "nui.h"
#include "nglString.h"

/// This is an abstract object that formats a text string. It can be used as a parameter of a nuiFormatedLabel, for instance...
class NUI_API nuiFormater
{
public:
  nuiFormater();
  virtual ~nuiFormater();

  virtual bool Format (double dValue, nglString& rResult) const;
  virtual double GetCDouble (const nglString& text) const;
};

static nuiFormater DefaultFormater;

#endif // __nuiFormater_h__
