/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiTitledPane.h"
#include "nuiDrawContext.h"


nuiTitledPane::nuiTitledPane(nuiWidget* pTitleWidget)
: nuiVBox(2)
{
  SetExpand(nuiExpandShrinkAndGrow);
  SetObjectClass(_T("nuiTitledPane"));
  
  SetCellExpand(1, nuiExpandShrinkAndGrow);

  
	if (!pTitleWidget)
    SetTitle(_T("unnamed titledpane"));
	else
    SetTitle(pTitleWidget);
}



nuiTitledPane::nuiTitledPane(const nglString& rText)
: nuiVBox(2)
{
  SetExpand(nuiExpandShrinkAndGrow);
  SetObjectClass(_T("nuiTitledPane"));
  SetTitle(rText);
}


nuiTitledPane::nuiTitledPane(nuiLabelAttribute* pLabel)
: nuiVBox(2)
{
  SetExpand(nuiExpandShrinkAndGrow);
  SetObjectClass(_T("nuiTitledPane"));
  SetTitle(pLabel);
}

// virtuals, overloaded to redirect the use. Please use SetContents instead 
bool nuiTitledPane::AddChild(nuiWidgetPtr pChild)
{
  SetContents(pChild);
  return false;
}


bool nuiTitledPane::Load(const nuiXMLNode* pNode)
{
  return nuiVBox::Load(pNode);
}




nuiXMLNode* nuiTitledPane::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
	return nuiVBox::Serialize(pParentNode, Recursive);
}



nuiTitledPane::~nuiTitledPane()
{
}

nuiWidget* nuiTitledPane::GetTitle()
{
	return GetCell(0);
}


void nuiTitledPane::SetTitle(const nglString& rTitle, nuiPosition position)
{
  nuiLabel* label = new nuiLabel(rTitle, nuiFont::GetFont (12));
  SetCell(0, label, position);
  InvalidateLayout();
}	

void nuiTitledPane::SetTitle(nuiWidget* pTitleWidget, nuiPosition position)
{
  SetCell(0, pTitleWidget, position);
  InvalidateLayout();
}


nuiWidget* nuiTitledPane::GetContents()
{
  return GetCell(1);
}

void nuiTitledPane::SetContents(nuiWidget* pContentsWidget, nuiPosition position)
{
  SetCell(1, pContentsWidget, position);
  SetCellExpand(1, nuiExpandShrinkAndGrow);
  InvalidateLayout();
}





