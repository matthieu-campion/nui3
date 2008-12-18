/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#include "Tools/ElementInspector.h"
#include "ElementEditor/FrameEditor.h"
#include "ElementEditor/WidgetEditor.h"
#include "nuiAttributeEditor.h"

ElementInspector::ElementInspector()
	: nuiPane()
{
	// build 
	mpVBox = new nuiVBox(0);
	mpVBox->SetExpand(nuiExpandShrinkAndGrow);
 	
	AddChild(mpVBox);
}

ElementInspector::~ElementInspector()
{
  
}





void ElementInspector::AddToolpane(ToolPane* pToolPane, uint32 maxPixels)
{
	mpVBox->AddCell(pToolPane);
	uint index = mpVBox->GetNbCells() - 1;
	mToolPanes[pToolPane] = index;
	
	if (maxPixels)
	mpVBox->SetCellMaxPixels(index, maxPixels);	
}


bool ElementInspector::ReplaceToolpane(ToolPane* pOld, ToolPane* pNew)
{
	std::map<ToolPane*, uint>::iterator it = mToolPanes.find(pOld);
	if (it == mToolPanes.end())
		return false;
		
	uint index = it->second;
	
	// set the new toolpane in position
	mpVBox->SetCell(index, pNew);
	
	// update map
	mToolPanes.erase(it);
	mToolPanes[pNew] = index;
		
	return true;
}




//ToolPane* ElementInspector::AddToolpane(nuiWidget* pObject)
//{
//	ToolPane* toolpane = new ToolPane(pObject);
//
//	if (!mpTargetToolPane)
//	{
//		mpVBox->AddCell(toolpane);
//		mTargetIndex = mpVBox->GetNbCells()-1;
//		mpTargetToolPane = toolpane;
//		return;
//	}
//	
//	mpVBox->SetCell(mTargetIndex, toolpane);
//	mpTargetToolPane = toolpane;
//}
//
//
//ToolPane* ElementInspector::AddToolpane(const nglString& rTitle, const std::map<nglString,nuiAttributeBase*>& rAttributes)
//{
//	ToolPane* toolpane = new ToolPane(rTitle, rAttributes);
//	mpVBox->AddCell(toolpane);
//}
//
