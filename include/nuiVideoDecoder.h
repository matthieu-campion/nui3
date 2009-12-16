/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"

class nuiVideoDecoder
{
public:
  nuiVideoDecoder(const nglPath& path);
  virtual ~nuiVideoDecoder();
  
  bool IsValid() const;
  double GetDuration() const;
  double GetPosition() const;
  void SetPosition(double TimePosition);
  
  double GetTimeTillNextFrame() const;
  
  bool GoToNextFrame();
  bool GoToPrevFrame();
  
  nglImage* GetCurrentImage();
  
  
  
private:
  bool Init();
  
  nglPath mPath;
  nglImage* mpImage;
  class nuiVideoDecoderPrivate* mpPrivate;
  
};

