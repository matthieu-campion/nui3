/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiPane.h"
#include "nuiVBox.h"
#include "ElementEditor/ElementEditor.h"
#include "Tools/ToolPane.h"



class ElementInspector : public nuiPane
{
public:
  ElementInspector();
  virtual ~ElementInspector();

	void AddToolpane(ToolPane* pToolPane, uint32 maxPixels=0);
	bool ReplaceToolpane(ToolPane* pOld, ToolPane* pNew);

protected:

  
private:

	nuiVBox* mpVBox;
	std::map<ToolPane*, uint> mToolPanes;

};


