/*
 *  ArrayView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ArrayView.h"

#include "nuiHBox.h"

ArrayView::ArrayView(nuiJson::Value& rValue)
: ValueView(rValue)
{
  SetObjectClass(_T("ArrayView"));
  
  nuiHBox* pHBox = new nuiHBox();
  AddChild(pHBox);
  
  for (uint32 i = 0; i < mrValue.size(); i++)
  {
    nuiJson::Value& rVal = mrValue[i];
    ValueView* pView = ValueView::CreateView(rVal);
    pHBox->AddCell(pView);
  }
}

ArrayView::~ArrayView()
{
  
}