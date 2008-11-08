/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Gui/guiOscillo.h"

guiOscillo::guiOscillo()
: nuiWidget()
{
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
