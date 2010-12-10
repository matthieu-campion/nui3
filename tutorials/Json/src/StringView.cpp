/*
 *  StringView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "StringView.h"

StringView::StringView(nuiJson::Value& rValue)
: ValueView(rValue)
{
  if (SetObjectClass(_T("StringView")))
  {
    AddAttribute(new nuiAttribute<nglString>(_T("stringAttrib"), nuiUnitNone,
                                        nuiMakeDelegate(this, &StringView::GetVal),
                                        nuiMakeDelegate(this, &StringView::SetVal)));
  }
  
  nuiAttribBase attr  = GetAttribute(_T("stringAttrib"));
  nuiWidget* pEditor = attr.GetEditor();
  pEditor->SetEnabled(false); // read-only, mouse events forbidden, there is a bug with label renamer
  AddChild(pEditor);
}

StringView::~StringView()
{
  
}

nglString StringView::GetVal()
{
  return mrValue.asString();
}

void StringView::SetVal(nglString val)
{
  mrValue = val.GetStdString();
}