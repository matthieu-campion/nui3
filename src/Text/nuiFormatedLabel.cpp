/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"



nuiFormater nuiFormatedLabel::DefaultFormater;


//*********************************************************************
//
// Constr
//
//
nuiFormatedLabel::nuiFormatedLabel(const nuiFormater& rFormater, double value, nuiTheme::FontStyle FontStyle)
: nuiLabel (nglString::Null, FontStyle), 
  mFormater(rFormater)
{
  NGL_ASSERT(&mFormater);
  SetObjectClass(_T("nuiFormatedLabel"));
  SetValue (value);
}



//*********************************************************************
//
// Constr
//
//
nuiFormatedLabel::nuiFormatedLabel(const nuiFormater& rFormater, double value, nuiFont* pFont, bool AlreadyAcquired)
: nuiLabel (nglString::Null, pFont, AlreadyAcquired), 
  mFormater(rFormater)
{
  //#FIXME waiting for me to know if there is a default font pointer...
  NGL_ASSERT(&mFormater);
  
  SetObjectClass(_T("nuiFormatedLabel"));
  SetValue (value);
}



//*********************************************************************
//
// Destr. virtual
//
//
nuiFormatedLabel::~nuiFormatedLabel()
{

}



//*********************************************************************
//
// SetValue
//
//
void nuiFormatedLabel::SetValue (double value)
{
  nglString res;
  
  mFormater.Format (value, res);
  SetText (res);
}


double nuiFormatedLabel::GetValue()
{
  return mFormater.GetCDouble(GetText());
}


