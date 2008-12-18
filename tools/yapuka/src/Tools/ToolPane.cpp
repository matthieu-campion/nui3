/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "Tools/ToolPane.h"
#include "Main/Yapuka.h"


ToolPane::ToolPane(const nglString& rTitle, nuiWidget* pWidget)
	: nuiFolderPane(rTitle, true/*opened*/)
{
  nglString title = rTitle;
  nuiLabel* pLabel = new nuiLabel(title.ToUpper(), nuiFont::GetFont(11));
  pLabel->SetObjectName(_T("Toolpane::Title"));
  pLabel->SetColor(eNormalTextFg, nuiColor(255,255,255));
  SetTitle(pLabel);
	if (pWidget)
		SetContents(pWidget);
}


ToolPane::ToolPane(nuiLabelAttribute* rLabel, nuiWidget* pWidget)
	: nuiFolderPane(rLabel, true/*opened*/)
{
  GetTitle()->SetObjectName(_T("Toolpane::Title"));
  GetTitle()->SetColor(eNormalTextFg, nuiColor(255,255,255));
	if (pWidget)
		SetContents(pWidget);
}


// virtual from nuiTitledPane(through nuiFolderPane)
void ToolPane::SetContents(nuiWidget* pWidget, nuiPosition position)
{
  pWidget->SetObjectName(_T("Toolpane::Contents"));
	nuiFolderPane::SetContents(pWidget);
}


ToolPane::~ToolPane()
{

}




