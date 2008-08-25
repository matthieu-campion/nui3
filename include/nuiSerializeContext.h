/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiObject.h"
#include "nuiXML.h"

enum nuiSerializeOperation
{
  eLoad,
  eSave,
  eSaveAs
};

class nuiSerializeContext : public nuiObject
{
public:
  nuiSerializeContext(nuiXMLNode* pNode, nuiSerializeOperation mode, bool recursive = true);
  nuiSerializeContext(const nuiSerializeContext& rContext);
  virtual ~nuiSerializeContext();
  
  nuiXMLNode* operator->() const;
  operator nuiXMLNode*() const;
  nuiSerializeContext& operator=(nuiXMLNode* pNode);
  
  bool IsRecursive() const;
  void SetRecursive(bool set);

  const nglPath& GetPath() const;
  const nglPath& GetResourcePath() const;

  void SetPath(const nglPath& rPath);
  void SetResourcePath(const nglPath& rPath);

  bool IsSaveAs() const;
  nuiSerializeOperation GetOperation() const;
  
  void AddErrorComment(const nglString& error);
  const nglString& GetErrorComments() const;
private:
  nuiXMLNode* mpNode;
  bool mRecursive;
  nglPath mPath;
  nglPath mResourcePath;
  nuiSerializeOperation mOperation;
  nglString mErrorComment;
};

