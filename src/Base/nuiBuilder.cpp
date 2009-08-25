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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class nuiWidgetCreatorOperation
{
public:
  enum Type
  {
    eAddChild,
    eAddCell,
    eSetCell1,
    eSetCell2,
    eSetProperty,
    eSetAttribute,
  };
  
  nuiWidgetCreatorOperation(Type type, nuiWidgetCreator* pCreator, uint32 index1, uint32 index2)
  {
    mType = type;
    mpCreator = pCreator;
    mIndex1 = index1;
    mIndex2 = index2;
  }
  
  nuiWidgetCreatorOperation(Type type, const nglString& rName, const nglString& rValue)
  {
    mType = type;
    mpCreator = NULL;
    mIndex1 = 0;
    mIndex2 = 0;
    mName = rName;
    mValue = rValue;
  }
  
  Type mType;
  nuiWidgetCreator* mpCreator;
  uint32 mIndex1;
  uint32 mIndex2;
  nglString mName;
  nglString mValue;
};

nuiWidgetCreator::nuiWidgetCreator(const nglString& rClassName, const nglString& rObjectName)
{
  
}

nuiWidgetCreator::~nuiWidgetCreator()
{
  
}

nuiWidget* nuiWidgetCreator::Create(const std::map<nglString, nglString>& rParamDictionnary, nuiBuilder* pBuilder) const
{
  if (!pBuilder)
    pBuilder = &nuiBuilder::Get();
  
  nuiWidget* pWidget = pBuilder->CreateWidget(mClassName);
  if (!pWidget)
  {
    NGL_OUT(_T("Error while creating a %ls named %ls"), mClassName.GetChars(), mObjectName.GetChars());
    return NULL;
  }
  
  pWidget->SetObjectName(mObjectName);
  
  nuiSimpleContainer* pContainer = dynamic_cast<nuiSimpleContainer*> (pWidget);
  nuiBox* pBox = dynamic_cast<nuiBox*> (pWidget);
  nuiGrid* pGrid = dynamic_cast<nuiGrid*> (pWidget);
  
  for (uint32 i = 0; i < mOperations.size(); i++)
  {
    nuiWidget* pChild = mOperations[i].mpCreator->Create(rParamDictionnary, pBuilder);
    if (pChild)
    {
      switch (mOperations[i].mType)
      {
        case nuiWidgetCreatorOperation::eAddChild:
          if (pContainer)
            pContainer->AddChild(pChild);
          break;
        case nuiWidgetCreatorOperation::eAddCell:
          if (pBox)
            pBox->AddCell(pChild);
          break;
        case nuiWidgetCreatorOperation::eSetCell1:
          if (pBox)
            pBox->SetCell(mOperations[i].mIndex1, pChild);
          break;
        case nuiWidgetCreatorOperation::eSetCell2:
          if (pGrid)
            pGrid->SetCell(mOperations[i].mIndex1, mOperations[i].mIndex2, pChild);
          break;
        case nuiWidgetCreatorOperation::eSetProperty:
          pWidget->SetProperty(mOperations[i].mName, mOperations[i].mValue);
          break;
        case nuiWidgetCreatorOperation::eSetAttribute:
          {
            nuiAttribBase attrib(pWidget->GetAttribute(mOperations[i].mName));
            if (attrib.IsValid())
              attrib.FromString(mOperations[i].mValue);
          }
          break;
      }
    }
  }
  return pWidget;
}

nuiWidget* nuiWidgetCreator::Create(nuiBuilder* pBuilder) const
{
  std::map<nglString, nglString> EmptyDictionnary;
  
  return Create(EmptyDictionnary, pBuilder);
}

// For any simple container widget:
void nuiWidgetCreator::AddChild(nuiWidgetCreator* pCreator)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eAddChild, pCreator, 0, 0));
}

// For box containers (nuiBox: nuiHBox, nuiVBox):
void nuiWidgetCreator::AddCell(nuiWidgetCreator* pCreator)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eAddCell, pCreator, 0, 0));
}

void nuiWidgetCreator::SetCell(uint32 cell, nuiWidgetCreator* pCreator)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eSetCell2, pCreator, cell, 0));
}

// For nuiGrid based containers
void nuiWidgetCreator::SetCell(uint32 column, uint32 row, nuiWidgetCreator* pCreator)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eSetCell2, pCreator, column, row));
}

void nuiWidgetCreator::SetProperty(const nglString& rName, const nglString& rValue)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eSetProperty, rName, rValue));
}

void nuiWidgetCreator::SetAttribute(const nglString& rName, const nglString& rValue)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eSetAttribute, rName, rValue));
}

