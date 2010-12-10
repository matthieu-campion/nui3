/*
 *  ValueView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ValueView.h"

#include "NullView.h"
#include "NumberView.h"
#include "StringView.h"
#include "BoolView.h"
#include "ArrayView.h"
#include "ObjectView.h"


ValueView* ValueView::CreateView(nuiJson::Value& rValue)
{
  ValueView* pView = NULL;
  if (rValue.isNull())
  {    
    pView = new NullView(rValue);
  }
  else if (rValue.isBool())
  { 
    pView = new BoolView(rValue);
  }
  else if (rValue.isInt())
  { 
    pView = new NumberView(rValue);
  }
  else if (rValue.isUInt())
  { 
    pView = new NumberView(rValue);
  }
  else if (rValue.isDouble())
  { 
    pView = new NumberView(rValue);
  }
  else if (rValue.isString())
  { 
    pView = new StringView(rValue);
  }
  else if (rValue.isArray())
  {
    pView = new ArrayView(rValue);
  }
  else if (rValue.isObject())
  {
    pView = new ObjectView(rValue);
  }
}


ValueView::ValueView(nuiJson::Value& rValue)
: mrValue(rValue)
{
  SetObjectClass(_T("ValueView"));
}

ValueView::~ValueView()
{
  
}