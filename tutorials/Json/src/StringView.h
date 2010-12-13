/*
 *  StringView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ValueView.h"

class StringView : public ValueView
{
public:
  StringView(nuiJson::Value& rValue);
  virtual ~StringView();
  
  nglString GetVal();
  void SetVal(nglString val);
  
private:
  
};