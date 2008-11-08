/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

//
// have a look to guiOscillo.h
//

#include "Gui/guiOscillo.h"

guiOscillo::guiOscillo()
: nuiWidget()
{
  // Setting the object class is not mandatory here. 
  // We use it in the css stylesheet to apply a decoration to the guiOscillo object. 
  // We could set the object name instead (SetObjectName), it's nearly the same for that purpose 
  // (except that, in that case, in the css stylesheet, the "guiOscillo" reference should be put between quotes.
  // Have a look to Gui.css, you've got some examples.)
  SetObjectClass(_T("guiOscillo"));
}


guiOscillo::~guiOscillo()
{

}


// virtual, overloaded from nuiWidget, to be able to draw the oscillo manually
bool guiOscillo::Draw(nuiDrawContext* pContext)
{
  return true;
}
