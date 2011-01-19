/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"
#include "nglCriticalSection.h"

class nuiVideoDecoder
{
public:
  nuiVideoDecoder(const nglPath& path);
  virtual ~nuiVideoDecoder();
  
  bool IsValid() const;
  
  double GetDuration() const;
  uint32 GetWidth() const;
  uint32 GetHeight() const;
  float GetRate() const;
  
  double GetPosition() const;
  void SetPosition(double TimePosition);
  
  bool GoToNextFrame();
  
  nglImage* UpdateImage();
  nuiTexture* UpdateTexture();
  
private:
  bool Init();
  
  double mDuration;
  uint32 mWidth;
  uint32 mHeight;
  float mRate;
  
  nglPath mPath;
  nglImage* mpImage;
  nuiTexture* mpTexture;
  class nuiVideoDecoderPrivate* mpPrivate;
  
};

