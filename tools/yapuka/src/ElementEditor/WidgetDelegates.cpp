/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "WidgetDelegates.h"
#include "nuiTitledPane.h"
#include "nuiFolderPane.h"

void WidgetDelegateLabel(nuiWidget* pWidget)
{
	nuiLabel* pLabel = (nuiLabel*) pWidget;
	
	pLabel->SetObjectName(_T("unnamed label"));
	pLabel->SetText(_T("empty label"));
}


void WidgetDelegateTitledPane(nuiWidget* pWidget)
{
	nuiTitledPane* pPane = (nuiTitledPane*) pWidget;
	
	pPane->SetObjectName(_T("unnamed titledpane"));
	pPane->SetTitle(_T("unnamed titledpane"));
}

void WidgetDelegateFolderPane(nuiWidget* pWidget)
{
	nuiFolderPane* pFolder = (nuiFolderPane*) pWidget;
	
	pFolder->SetObjectName(_T("unnamed folderpane"));
	pFolder->SetTitle(_T("unnamed folderpane"));
}
