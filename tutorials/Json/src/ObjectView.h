/*
 *  ObjectView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ValueView.h"

class nuiVBox;

class ObjectView : public ValueView
{
public:
  ObjectView(nuiJson::Value& rValue);
  virtual ~ObjectView();
  
private:
  
  nuiVBox* mpVBox;
};