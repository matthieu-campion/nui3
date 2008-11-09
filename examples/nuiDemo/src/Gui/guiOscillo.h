/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

//
// guiOscillo inherits from nuiWidget, to draw an oscilloscope, reacting to the sound the application plays
// inheriting from nuiWidget allows us to:
// - include the guiOscillo object in the nui widget tree of the application graphic interface
// - overload all the nuiWidget method we need. 
//
// Here, we only need to overload bool Draw(nuiDrawContext* pContext). It's the standard method called by the 
// rendering system to draw any widget. 
//

#pragma once

#include "nui.h"
#include "nuiVBox.h"

class guiOscillo : public nuiWidget
{
public:
  guiOscillo();
  virtual ~guiOscillo();

protected:
  
  // overloaded from nuiWidget, to be able to draw the oscillo manually
  virtual bool Draw(nuiDrawContext* pContext);
  
private:
  
//    const std::vector<std::vector<float> >& mrData;
};

