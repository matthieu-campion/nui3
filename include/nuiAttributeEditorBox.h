/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once

#include "nui.h"
#include "nuiVBox.h"
#include "nuiHBox.h"

class NUI_API nuiAttributeEditorBox : public nuiSimpleContainer
{
public:

  nuiAttributeEditorBox(const nglString& rFolderName, const std::vector<nuiAttribBase>& rAttributes, nuiOrientation orientation)
  : nuiSimpleContainer()
  {
    SetObjectClass("nuiAttributeEditorBox");
    Init(rFolderName, rAttributes, orientation);
  }

  nuiAttributeEditorBox(const nglString& rFolderName, nuiObject* pObject, const std::vector<nglString>& rAttributeNames, nuiOrientation orientation)
  : nuiSimpleContainer()
  {
    SetObjectClass("nuiAttributeEditorBox");
    std::vector<nuiAttribBase> attribs;
    for (uint32 i = 0 ; i < rAttributeNames.size(); i++)
      attribs.push_back(pObject->GetAttribute(rAttributeNames[i]));
    Init(rFolderName, attribs, orientation);
  }

  nuiAttributeEditorBox(const nglString& rFolderName, nuiObject* pObject, const char* pAttributeNames[], nuiOrientation orientation)
  : nuiSimpleContainer()
  {
    SetObjectClass("nuiAttributeEditorBox");
    std::vector<nuiAttribBase> attribs;
    for (uint32 i = 0 ; pAttributeNames[i]; i++)
      attribs.push_back(pObject->GetAttribute(pAttributeNames[i]));
    Init(rFolderName, attribs, orientation);
  }

  virtual ~nuiAttributeEditorBox()
  {
    
  }

protected:
  void Init(const nglString& rFolderName, const std::vector<nuiAttribBase>& rAttributes, nuiOrientation orientation)
  {
    nuiVBox* pPane = new nuiVBox();
    pPane->AddCell(new nuiLabel(rFolderName));
    
    
    nuiBox* pBox = NULL;
    switch (orientation)
    {
      case nuiHorizontal:
        pBox = new nuiHBox();
        break;
      case nuiVertical:
        pBox = new nuiVBox();
        break;
    }
    pPane->AddCell(pBox);
    AddChild(pPane);
    const char* params[] = { "Type", "Frequency", "Q", NULL };
    for (uint32 i = 0 ; i < rAttributes.size(); i++)
    {
      if (rAttributes[i].IsValid())
      {
        nuiWidget* pWidget = rAttributes[i].GetEditor();
        pBox->AddCell(pWidget, nuiFill);
      }
    }
  }
};
