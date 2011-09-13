/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once

#include "nui.h"
#include "nuiFolderPane.h"

class NUI_API nuiAttributeEditorPane : public nuiFolderPane
{
public:

  nuiAttributeEditorPane(const nglString& rFolderName, const std::vector<nuiAttribBase>& rAttributes)
  : nuiFolderPane(rFolderName)
  {
    SetObjectClass("nuiAttributeEditorPane");
    Init(rAttributes);
  }

  nuiAttributeEditorPane(const nglString& rFolderName, nuiObject* pObject, const std::vector<nglString>& rAttributeNames)
  : nuiFolderPane(rFolderName)
  {
    SetObjectClass("nuiAttributeEditorPane");
    std::vector<nuiAttribBase> attribs;
    for (uint32 i = 0 ; i < rAttributeNames.size(); i++)
      attribs.push_back(pObject->GetAttribute(rAttributeNames[i]));
    Init(attribs);
  }

  nuiAttributeEditorPane(const nglString& rFolderName, nuiObject* pObject, const char* pAttributeNames[])
  : nuiFolderPane(rFolderName)
  {
    SetObjectClass("nuiAttributeEditorPane");
    std::vector<nuiAttribBase> attribs;
    for (uint32 i = 0 ; pAttributeNames[i]; i++)
      attribs.push_back(pObject->GetAttribute(pAttributeNames[i]));
    Init(attribs);
  }

  virtual ~nuiAttributeEditorPane()
  {
    
  }

protected:
  void Init(const std::vector<nuiAttribBase>& rAttributes)
  {
    GetTitle()->SetDecoration("nuiAttributeEditorPaneTitle");
    nuiVBox* pBox = new nuiVBox();
    SetContents(pBox);
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
