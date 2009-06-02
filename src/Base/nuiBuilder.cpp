/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiBuilder.h"
#include "nuiXML.h"

#include "nuiObject.h"

#include "nuiMainWindow.h"
#include "nuiWindow.h"
#include "nuiText.h"
#include "nuiEditText.h"
#include "nuiEditLine.h"
#include "nuiButton.h"
#include "nuiToggleButton.h"
#include "nuiRadioButton.h"
#include "nuiSplitter.h"
#include "nuiPopupMenu.h"
#include "nuiList.h"
#include "nuiLabel.h"
#include "nuiScrollBar.h"
#include "nuiFixed.h"
#include "nuiPositioner.h"
#include "nuiUserArea.h"
#include "nuiImage.h"
#include "nuiSplineEdit.h"
#include "nuiStateDummy.h"
#include "nuiHoverDummy.h"
#include "nuiMesh.h"
#include "nuiSVGView.h"
#include "nuiScrollView.h"
#include "nuiZoomView.h"
#include "nuiTabBar.h"
#include "nuiTreeView.h"
#include "nuiColumnTreeView.h"
#include "nuiSlider.h"
#include "nuiPane.h"
#include "nuiProgressBar.h"
#include "nuiGrid.h"
#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiFolderPane.h"
#include "nuiTitledPane.h"
#include "nuiFormatedLabel.h"
#include "nuiKnob.h"


using namespace std;

//////////////////// Builder class:
nuiBuilder::nuiBuilder()
{
  Init();
}

nuiBuilder::~nuiBuilder()
{
}

void nuiBuilder::Init()
{
  NUI_ADD_WIDGET_CREATOR(nuiWindow,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiSplitter,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiPopupMenu,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiList,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiScrollBar,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiFixed,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiPositioner,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiMainWindow,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiStateDummy,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiHoverDummy,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiScrollView,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiZoomView,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiTabBar,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiTreeView,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiColumnTreeView,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiSlider,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiPane,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiGrid,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiHBox,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiVBox,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiTitledPane,"Container");
  NUI_ADD_WIDGET_CREATOR(nuiFolderPane,"Container");
#ifndef _OPENGL_ES_
  NUI_ADD_WIDGET_CREATOR(nuiUserArea,"Widget");
#endif
  NUI_ADD_WIDGET_CREATOR(nuiLabel,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiText,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiEditText,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiEditLine,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiButton,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiKnob,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiToggleButton,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiRadioButton,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiImage,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiSplineEdit,"Widget");
#ifndef _OPENGL_ES_
  NUI_ADD_WIDGET_CREATOR(nuiMesh,"Widget");
#endif
  NUI_ADD_WIDGET_CREATOR(nuiSVGView,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiTab,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiProgressBar,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiFormatedLabel,"Widget");
}

void nuiBuilder::SetHandler(const nglString& ClassName, const nglString& ClassGroup, nuiCreateWidgetFn pHandler)
{
  NGL_ASSERT(mBuilderMap.find(ClassName) == mBuilderMap.end());
  nuiWidgetDesc desc(ClassName, ClassGroup, pHandler);
  mBuilderMap[ClassName] = desc;
}

nuiCreateWidgetFn nuiBuilder::GetHandler(const nglString& ClassName) const
{
  map<nglString, nuiWidgetDesc, nglString::LessFunctor>::const_iterator it = mBuilderMap.find(ClassName);
  if (it == mBuilderMap.end())
    return NULL;
  
  const nuiWidgetDesc& desc = it->second;
  return desc.GetHandler();
}

bool nuiBuilder::GetClassList(list<nuiWidgetDesc>& rClassNames) const
{
  map<nglString, nuiWidgetDesc, nglString::LessFunctor>::const_iterator it;
  map<nglString, nuiWidgetDesc, nglString::LessFunctor>::const_iterator end = mBuilderMap.end();

  for (it = mBuilderMap.begin(); it != end; ++it)
    rClassNames.push_back((*it).second);

  return true;
}

nuiWidget* nuiBuilder::CreateWidget(const nglString& rClassName) const
{
  nuiCreateWidgetFn pFn = GetHandler(rClassName);
  if (!pFn)
    return NULL;
  
  return pFn();
}

nuiWidget* nuiCreateWidget(const nglString& rClassName)
{
  return nuiBuilder::Get().CreateWidget(rClassName);
}

nuiWidget* nuiCreateWidget(const nuiXMLNode* pNode)
{
  nuiWidgetPtr pWidget = nuiBuilder::Get().CreateWidget(pNode->GetName());
  if (!pWidget)
		return NULL;
		pWidget->Load(pNode);
	return pWidget;
}


nuiBuilder nuiBuilder::mBuilder;

