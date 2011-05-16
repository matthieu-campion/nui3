/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#pragma once
#include "nui.h"


class nuiImageSequence;


// a simple widget class to handle an image sequence as a widget
class nuiImageAnimation : public nuiWidget
{
public:
  nuiImageAnimation(uint32 nbFrames, const nglPath& rTexturePath, bool framesInSingleFile = true, nuiOrientation orientation = nuiVertical); // all frames in a single file, or stored in separated png files
  virtual ~nuiImageAnimation();
  
  uint32 GetFrameIndex() const;
  void SetFrameIndex(uint32 index);

protected:
  
  virtual nuiRect CalcIdealSize();
  virtual bool Draw(nuiDrawContext* pContext);
  
private:
  
  nuiImageSequence* mpSequence;
  
};