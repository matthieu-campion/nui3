/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#include "nui.h"


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
  
  label->SetObjectName(_T("nuiTitledPane::Title"));
  
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





