/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "ToolPane.h"
#include "Yapuka.h"


ToolPane::ToolPane(const nglString& rTitle, nuiWidget* pWidget)
	: nuiFolderPane(rTitle, true/*opened*/)
{
  nglString title = rTitle;
  nuiLabel* pLabel = new nuiLabel(title.ToUpper(), nuiFont::GetFont(11));
  pLabel->SetColor(eNormalTextFg, nuiColor(255,255,255));
  pLabel->SetDecoration(GetDecorationManager()->Get(DECO_PANE_TITLE), eDecorationBorder);
  SetTitle(pLabel);
	if (pWidget)
		SetContents(pWidget);
}


ToolPane::ToolPane(nuiLabelAttribute* rLabel, nuiWidget* pWidget)
	: nuiFolderPane(rLabel, true/*opened*/)
{
  GetTitle()->SetDecoration(GetDecorationManager()->Get(DECO_PANE_TITLE), eDecorationBorder);
  GetTitle()->SetColor(eNormalTextFg, nuiColor(255,255,255));
	if (pWidget)
		SetContents(pWidget);
}


// virtual from nuiTitledPane(through nuiFolderPane)
void ToolPane::SetContents(nuiWidget* pWidget, nuiPosition position)
{
  pWidget->SetDecoration(GetDecorationManager()->Get(DECO_PANE_CONTENTS), eDecorationBorder);
	nuiFolderPane::SetContents(pWidget);
}


ToolPane::~ToolPane()
{

}




