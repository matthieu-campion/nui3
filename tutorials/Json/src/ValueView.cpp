/*
 *  ValueView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ValueView.h"

ValueView::ValueView(nuiJson::Value& rValue)
: mrValue(rValue)
{
  SetObjectClass(_T("ValueView"));
}

ValueView::~ValueView()
{
  
}