/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
  
#include "ElementEditor.h"
#include "ElementInspector.h"
#include "FrameEditor.h"
#include "WidgetEditor.h"
#include "nuiHBox.h"
#include "Yapuka.h"
#include "MainWindow.h"

ElementEditor::ElementEditor(ElementDesc* pDesc)
{
	nuiHBox* hbox = new nuiHBox(2);
	hbox->SetExpand(nuiExpandShrinkAndGrow);
	AddChild(hbox);

	mpInspector = new ElementInspector();


  mpDesc = pDesc;
  switch (pDesc->GetType())
  {
  case eElementFrame:
	{
		mpFramedit = new FrameEditor(pDesc, mpInspector);
    hbox->SetCell(0, mpFramedit);
		hbox->SetCellExpand(0, nuiExpandShrinkAndGrow);


	}
    break;
		
  case eElementWidget:
    hbox->SetCell(0, new WidgetEditor(pDesc, mpInspector));
			hbox->SetCellExpand(0, nuiExpandShrinkAndGrow);

    break;
  }


	
	
	hbox->SetCell(1, mpInspector);
	hbox->SetCellExpand(1, nuiExpandFixed);
	hbox->SetCellMinPixels(1, 200);
	
	
}




ElementEditor::~ElementEditor()
{
  
}

ElementDesc* ElementEditor::GetDesc() const
{
  return mpDesc;
}


