/*
 *  BoolView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ValueView.h"

class BoolView : public ValueView
{
public:
  BoolView(nuiJson::Value& rValue);
  virtual ~BoolView();
  
  void SetVal(bool val);
  bool GetVal();
  
private:
  
};