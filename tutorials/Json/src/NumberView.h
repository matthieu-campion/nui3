/*
 *  NumberView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ValueView.h"

class NumberView : public ValueView
{
public:
  NumberView(nuiJson::Value& rValue);
  virtual ~NumberView();
  
  void SetInt(int val);
  int GetInt();
  
  void SetUInt(uint val);
  uint GetUInt();
  
  void SetDouble(double val);
  double GetDouble();
  
private:

};