/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiFormatedLabel_h__
#define __nuiFormatedLabel_h__

//#include "nui.h"
#include "nglString.h"
#include "nuiFont.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nuiLabel.h"
#include "nuiFormater.h"

/// This widget implements a formated text label. It's simply based on a nuiLabel and takes a nuiFormater object as an additional parameter
class NUI_API nuiFormatedLabel : public nuiLabel
{
public:
  nuiFormatedLabel(const nuiFormater& rFormater = nuiFormatedLabel::DefaultFormater, double value=0.0F, nuiTheme::FontStyle FontStyle=nuiTheme::Default);
  nuiFormatedLabel(const nuiFormater& rFormater, double value=0.0F, nuiFont* pFont=NULL, bool AlreadyAcquired = false);
  virtual ~nuiFormatedLabel();

  void SetValue (double value); 
  double GetValue();
  //#FIXME : implement all the SetValue methods
  
  static nuiFormater DefaultFormater;

protected:

  const nuiFormater& mFormater;
};

#endif // __nuiFormatedLabel_h__
