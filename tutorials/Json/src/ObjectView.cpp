/*
 *  ObjectView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ObjectView.h"
#include "nuiVBox.h"
#include "nuiHBox.h"



ObjectView::ObjectView(nuiJson::Value& rValue)
: ValueView(rValue)
{
  SetObjectClass(_T("ObjectView"));
  
  NGL_ASSERT(mrValue.isObject());
  
  mpVBox = new nuiVBox();
  AddChild(mpVBox);
  mpVBox->SetExpand(nuiExpandShrinkAndGrow);
  
  for (uint32 i = 0; i < mrValue.getMemberNames().size(); i++)
  {
    std::string name = mrValue.getMemberNames()[i];
    nuiJson::Value& rVal = mrValue[name];
    
    nuiHBox* pHBox = new nuiHBox();
    mpVBox->AddCell(pHBox);
    pHBox->SetExpand(nuiExpandShrinkAndGrow);
    
    nglString typeStr;
    if (rVal.isNull())
    {
      typeStr += _T("(null):");
    }
    else if (rVal.isBool())
    {
      typeStr += _T("(bool):");
    }
    else if (rVal.isInt())
    {
      typeStr += _T("(int):");
    }
    else if (rVal.isUInt())
    {
      typeStr += _T("(uint):");
    }
    else if (rVal.isDouble())
    {
      typeStr += _T("(double):");
    }
    else if (rVal.isString())
    {
      typeStr += _T("(string):");
    }
    else if (rVal.isArray())
    {
      typeStr += _T("(array):");
    }
    else if (rVal.isObject())
    {
      typeStr += _T("(object):");
    }
    
    nuiLabel* pLabel = new nuiLabel(name);
    pLabel->SetObjectName(_T("attributeNameLabel"));
    pHBox->AddCell(pLabel);
    
    nuiLabel* pTypeLabel = new nuiLabel(typeStr);
    pTypeLabel->SetObjectName(_T("attributeNameTypeLabel"));
    pHBox->AddCell(pTypeLabel);
    
    ValueView* pView = ValueView::CreateView(rVal);
    NGL_ASSERT(pView);
    pHBox->AddCell(pView);
  }
}

ObjectView::~ObjectView()
{
  
}