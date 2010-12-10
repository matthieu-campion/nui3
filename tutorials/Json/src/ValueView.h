/*
 *  ValueView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "nui.h"
#include "nuiJson.h"

class ValueView : public nuiSimpleContainer 
{
public:
  ValueView(nuiJson::Value& rValue);
  virtual ~ValueView();
  
protected:
  nuiJson::Value& mrValue;
};

