/*
 *  guiTheme.h
 *  MXEditor
 *
 *  Created by Loic Berthelot on 2008/11/15
 *  Copyright 2007 MXP4. All rights reserved.
 *
 */


 #pragma once


#include "nui.h"
#include "nuiTheme.h"

class guiTheme : public nuiTheme
{
public:
  
  guiTheme();
  virtual ~guiTheme();
  
protected:
  
  virtual void DrawSliderBackground(nuiDrawContext* pContext, nuiSlider* pScroll);
  virtual void DrawSliderForeground(nuiDrawContext* pContext, nuiSlider* pScroll);

};