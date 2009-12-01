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

#include "nuiDialogSelectDirectory.h"
#include "nuiDialogSelectFile.h"
#include "nuiDialogSelectFile.h"

#include "nuiAnimView.h"
#include "nuiBackgroundPane.h"
//#include "nuiDialog.h"
#include "nuiFlowView.h"
#include "nuiImageSequence.h"
#include "nuiMatrixView.h"
#include "nuiModalContainer.h"
#include "nuiOffscreenView.h"
#include "nuiPopupView.h"
#include "nuiScrollBox.h"
#include "nuiShadeContainer.h"
#include "nuiTabView.h"
#include "nuiColorSelector.h"
#include "nuiComboBox.h"
#include "nuiComposite.h"
#include "nuiCoverFlow.h"
#include "nuiFileList.h"
#include "nuiFileSelector.h"
#include "nuiFileTree.h"
#include "nuiFontBrowser.h"
#include "nuiFrameView.h"
#include "nuiHotKeyEditor.h"
#include "nuiOffscreenView.h"
#include "nuiImageDropZone.h"
#include "nuiRectView.h"
#include "nuiShapeView.h"
#include "nuiZoomBar.h"
#include "nuiRSSView.h"
#include "nuiHugeImage.h"
#include "nuiHyperLink.h"
#include "nuiLabelAttribute.h"
#include "nuiLabelRenamer.h"
#include "nuiHTMLView.h"
#include "nuiScrollingLabel.h"
#include "nuiSpinnerLabel.h"


using namespace std;

//////////////////// Builder class:
nuiBuilder::nuiBuilder()
{
  Init();
}

nuiBuilder::~nuiBuilder()
{
  nuiWidgetCreatorMap::iterator it = mCreatorMap.begin();
  while (it != mCreatorMap.end())
  {
    delete it->second;
    ++it;
  }

}

void nuiBuilder::Init()
{
  NUI_ADD_WIDGET_CREATOR(nuiWidget, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiSimpleContainer, "Container");
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
  NUI_ADD_WIDGET_CREATOR(nuiFolderPane,"Container");
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
  NUI_ADD_WIDGET_CREATOR(nuiSVGView,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiTab,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiProgressBar,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiFormatedLabel,"Widget");
  
  //NUI_ADD_WIDGET_CREATOR(nuiMessageBox,"Dialog");
  //NUI_ADD_WIDGET_CREATOR(nuiDialogSelectDirectory,"Dialog");
  //NUI_ADD_WIDGET_CREATOR(nuiDialogSelectFile,"Dialog");
  
  NUI_ADD_WIDGET_CREATOR(nuiAnimView, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiBackgroundPane, "Container");
  //NUI_ADD_WIDGET_CREATOR(nuiDialog, "Dialog");
  //NUI_ADD_WIDGET_CREATOR(nuiFlowView, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiImageSequence, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiMatrixView, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiModalContainer, "Container");
  //NUI_ADD_WIDGET_CREATOR(nuiOffscreenView, "Container");
  //NUI_ADD_WIDGET_CREATOR(nuiPopupView, "Container");
  //NUI_ADD_WIDGET_CREATOR(nuiScrollBox, "Container");
  //NUI_ADD_WIDGET_CREATOR(nuiShadeContainer, "Container");
  //NUI_ADD_WIDGET_CREATOR(nuiTabView, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiTitledPane, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiColorSelector, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiComboBox, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiComposite, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiCoverFlow, "Container");
  NUI_ADD_WIDGET_CREATOR(nuiFileList, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiFileSelector, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiFileTree, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiFontBrowser, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiFrameView, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiHotKeyEditor, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiImageDropZone, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiRectView, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiShapeView, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiZoomBar, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiRSSView, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiHugeImage, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiHyperLink, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiLabelAttribute, "Widget");
  //NUI_ADD_WIDGET_CREATOR(nuiLabelRenamer, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiHTMLView, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiScrollingLabel, "Widget");
  NUI_ADD_WIDGET_CREATOR(nuiSpinnerLabel, "Widget");

#ifndef _OPENGL_ES_
  NUI_ADD_WIDGET_CREATOR(nuiMesh,"Widget");
  NUI_ADD_WIDGET_CREATOR(nuiUserArea,"Widget");
#endif
}

void nuiBuilder::SetHandler(const nglString& ClassName, const nglString& ClassGroup, nuiCreateWidgetFn pHandler)
{
  //wprintf(_T("Adding Widget handler: %ls\n"), ClassName.GetChars());
  NGL_ASSERT(mBuilderMap.find(ClassName) == mBuilderMap.end());
  NGL_ASSERT(mCreatorMap.find(ClassName) == mCreatorMap.end());
  nuiWidgetDesc desc(ClassName, ClassGroup, pHandler);
  mBuilderMap[ClassName] = desc;
}

void nuiBuilder::SetHandler(const nglString& rClassName, nuiWidgetCreator* pCreator)
{
  NGL_ASSERT(mBuilderMap.find(rClassName) == mBuilderMap.end());
  NGL_ASSERT(mCreatorMap.find(rClassName) == mCreatorMap.end());
  mCreatorMap[rClassName] = pCreator;
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
  std::map<nglString, nglString> ParamDictionary;
  return CreateWidget(rClassName, ParamDictionary);
}

nuiWidget* nuiBuilder::CreateWidget(const nglString& rClassName, const std::map<nglString, nglString>& rParamDictionary) const
{
  nuiCreateWidgetFn pFn = GetHandler(rClassName);
  if (pFn)
    return pFn();
  
  map<nglString, nuiWidgetCreator*, nglString::LessFunctor>::const_iterator it = mCreatorMap.find(rClassName);
  if (it == mCreatorMap.end())
    return NULL;
  
  return (it->second)->Create(rParamDictionary, this);
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
  
  nuiWidgetCreatorOperation(Type type, const nglString& rName, const nglString& rValue, uint32 index1 = -1, uint32 index2 = -1)
  {
    mType = type;
    mpCreator = NULL;
    mIndex1 = index1;
    mIndex2 = index2;
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
  mClassName = rClassName;
  mObjectName = rObjectName;
}

nuiWidgetCreator::~nuiWidgetCreator()
{
  for (uint32 i = 0; i < mOperations.size(); i++)
  {
    delete mOperations[i].mpCreator;
  }
}

const nglString& nuiWidgetCreator::LookUp(const std::map<nglString, nglString>& rParamDictionary, const nglString& rString) const
{
  const std::map<nglString, nglString>::const_iterator it = rParamDictionary.find(rString);
  if (it != rParamDictionary.end())
    return it->second;

  return rString;
}

nuiWidget* nuiWidgetCreator::Create(const std::map<nglString, nglString>& rParamDictionary, const nuiBuilder* pBuilder) const
{
  if (!pBuilder)
    pBuilder = &nuiBuilder::Get();

  // Merge the dictionaries:
  std::map<nglString, nglString> ParamDictionary(mDefaultDictionary);
  ParamDictionary.insert(rParamDictionary.begin(), rParamDictionary.end());
  
//  NGL_LOG(_T("nuiWidgetCreator"), NGL_LOG_DEBUG, _T("\nBuild %ls / %ls\n"), mClassName.GetChars(), mObjectName.GetChars());
//  std::map<nglString, nglString>::iterator it = ParamDictionary.begin();
//  std::map<nglString, nglString>::iterator end = ParamDictionary.end();
//  while (it != end)
//  {
//    NGL_LOG(_T("nuiWidgetCreator"), NGL_LOG_DEBUG, _T("\t%ls - %ls\n"), it->first.GetChars(), it->second.GetChars());
//    ++it;
//  }
  
  
  nglString classname(LookUp(ParamDictionary, mClassName));
  nglString objectname(LookUp(ParamDictionary,mObjectName));
  nuiWidget* pWidget = pBuilder->CreateWidget(classname, ParamDictionary);
  if (!pWidget)
  {
    NGL_LOG(_T("nuiWidgetCreator"), NGL_LOG_ERROR, _T("Error while creating a %ls named %ls (translated to %ls - %ls"), 
            mClassName.GetChars(), mObjectName.GetChars(),
            classname.GetChars(), objectname.GetChars());
    return NULL;
  }
  
  if (!objectname.IsEmpty())
    pWidget->SetObjectName(objectname);
  
  nuiSimpleContainer* pContainer = dynamic_cast<nuiSimpleContainer*> (pWidget);
  nuiBox* pBox = dynamic_cast<nuiBox*> (pWidget);
  nuiGrid* pGrid = dynamic_cast<nuiGrid*> (pWidget);
  
  for (uint32 i = 0; i < mOperations.size(); i++)
  {
    nuiWidget* pChild = NULL;
    if (mOperations[i].mpCreator)
      pChild = mOperations[i].mpCreator->Create(ParamDictionary, pBuilder);
    
    if (pChild)
    {
      switch (mOperations[i].mType)
      {
        case nuiWidgetCreatorOperation::eAddChild:
          if (pBox)
            pBox->AddCell(pChild);
          else if (pContainer)
            pContainer->AddChild(pChild);
          break;
          
        case nuiWidgetCreatorOperation::eSetCell1:
          if (pBox)
            pBox->SetCell(mOperations[i].mIndex1, pChild);
          break;
          
        case nuiWidgetCreatorOperation::eSetCell2:
          if (pGrid)
            pGrid->SetCell(mOperations[i].mIndex1, mOperations[i].mIndex2, pChild);
          break;

        default:
          NGL_ASSERT(0);
          break;
      }
    }
    else
    {
      switch (mOperations[i].mType)
      {
        case nuiWidgetCreatorOperation::eSetProperty:
          pWidget->SetProperty(LookUp(ParamDictionary, mOperations[i].mName),
                               LookUp(ParamDictionary, mOperations[i].mValue));
          break;
          
        case nuiWidgetCreatorOperation::eSetAttribute:
          {
            nuiAttribBase attrib(pWidget->GetAttribute(LookUp(ParamDictionary, mOperations[i].mName)));
            if (attrib.IsValid())
            {
              int32 i0 = mOperations[i].mIndex1;
              int32 i1 = mOperations[i].mIndex2;
              
              if (i0 < 0)
                attrib.FromString(LookUp(ParamDictionary, mOperations[i].mValue));
              else if (i1 < 0)
                attrib.FromString(i0, LookUp(ParamDictionary, mOperations[i].mValue));
              else
                attrib.FromString(i0, i1, LookUp(ParamDictionary, mOperations[i].mValue));
              
            }
          }
          break;
          
        default:
          NGL_ASSERT(0);
          break;
      }
      
    }
  }
  return pWidget;
}

nuiWidget* nuiWidgetCreator::Create(const nuiBuilder* pBuilder) const
{
  std::map<nglString, nglString> ParamDictionary;
  return Create(ParamDictionary, pBuilder);
}

// For any simple container widget:
void nuiWidgetCreator::AddChild(nuiWidgetCreator* pCreator)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eAddChild, pCreator, 0, 0));
}

// For box containers (nuiBox: nuiHBox, nuiVBox):
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

void nuiWidgetCreator::SetAttribute(const nglString& rName, const nglString& rValue, int32 index0, int32 index1)
{
  mOperations.push_back(nuiWidgetCreatorOperation(nuiWidgetCreatorOperation::eSetAttribute, rName, rValue, index0, index1));
}

const nglString& nuiWidgetCreator::GetObjectClass() const
{
  return mClassName;
}

const nglString& nuiWidgetCreator::GetObjectName() const
{
  return mObjectName;
}

void nuiWidgetCreator::SetDefaultDictionary(const std::map<nglString, nglString>& rParamDictionary)
{
  mDefaultDictionary = rParamDictionary;
}

const std::map<nglString, nglString>& nuiWidgetCreator::GetDefaultDictionary() const
{
  return mDefaultDictionary;
}

std::map<nglString, nglString>& nuiWidgetCreator::GetDefaultDictionary()
{
  return mDefaultDictionary;
}

