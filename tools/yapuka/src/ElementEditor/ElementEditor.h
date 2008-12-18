/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"
#include "ElementDesc.h"


class ElementInspector;
class FrameEditor;

class ElementEditor : public nuiSimpleContainer
{
public:
  ElementEditor(ElementDesc* pDesc);

  virtual ~ElementEditor();
  
  ElementDesc* GetDesc() const;
	
  
private:

	FrameEditor* mpFramedit;
	
  ElementDesc* mpDesc;
	ElementInspector* mpInspector;



};


