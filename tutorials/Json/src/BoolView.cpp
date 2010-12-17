/*
 *  BoolView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoolView.h"

BoolView::BoolView(nuiJson::Value& rValue)
: ValueView(rValue)
{
  if (SetObjectClass(_T("BoolView")))
  {
    AddAttribute(new nuiAttribute<bool>(_T("boolAttrib"), nuiUnitBoolean,
                                        nuiMakeDelegate(this, &BoolView::GetVal),
                                        nuiMakeDelegate(this, &BoolView::SetVal)));
  }
  
  nuiAttribBase attr  = GetAttribute(_T("boolAttrib"));
  nuiWidget* pEditor = attr.GetEditor();
  AddChild(pEditor);
}

BoolView::~BoolView()
{
  
}

void BoolView::SetVal(bool val)
{
  mrValue = val;
}

bool BoolView::GetVal()
{
  return mrValue.asBool();
}