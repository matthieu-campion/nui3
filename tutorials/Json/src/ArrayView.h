/*
 *  ArrayView.h
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ValueView.h"

class ArrayView : public ValueView
{
public:
  ArrayView(nuiJson::Value& rValue);
  virtual ~ArrayView();
  
private:
  
};