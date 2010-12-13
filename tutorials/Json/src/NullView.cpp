/*
 *  NullView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "NullView.h"

NullView::NullView(nuiJson::Value& rValue)
: ValueView(rValue)
{
  SetObjectClass(_T("NullView"));
  nuiLabel* pLabel = new nuiLabel(_T("null value"));
  AddChild(pLabel);
}

NullView::~NullView()
{
  
}