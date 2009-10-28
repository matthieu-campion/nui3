/*
 *  nuiVideoDecoder.h
 *  nui3
 *
 *  Created by mat on 10/27/09.
 *  Copyright 2009 libNUI. All rights reserved.
 *
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

