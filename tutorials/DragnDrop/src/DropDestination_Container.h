/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
 

#include "nui.h"
#include "nglDragAndDropObjects.h"

class DropContainer : public nuiSimpleContainer
{
public:
  
  DropContainer();
  virtual ~DropContainer();
  
  virtual nglDropEffect OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y);
  virtual void OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  
  void Log(const nglString& rMsg);

  bool Draw(nuiDrawContext* pContext);
private:
  
  nglPath mDnDPath;
  bool mDnDing;
  bool mDnDValid;
  
  nuiLabel* mpText;
  nglDragAndDrop* mpLastDND;
  
  nuiSize mLastX;
  nuiSize mLastY;
};