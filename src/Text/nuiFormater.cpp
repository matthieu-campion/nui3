/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


//*********************************************************************
//
// Constr
//
//
nuiFormater::nuiFormater ()
{


}



//*********************************************************************
//
// Destr. virtual
//
//
nuiFormater::~nuiFormater()
{

}



//*********************************************************************
//
// Format. virtual
//
//
bool nuiFormater::Format (double dValue, nglString& rResult) const
{
  // identity (default formater)
  rResult.CFormat ("%.0f", dValue); // i.e., 45.80
  
  return true;
}

//*********************************************************************
//
// GetCDouble. virtual
//
//
double nuiFormater::GetCDouble (const nglString& text) const
{
  return text.GetCDouble ();
}
