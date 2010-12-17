/*
 *  NullView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ValueView.h"

class NullView : public ValueView
{
public:
  NullView(nuiJson::Value& rValue);
  virtual ~NullView();
  
private:
  
};