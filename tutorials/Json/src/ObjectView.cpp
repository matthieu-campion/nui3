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

#include "NullView.h"
#include "NumberView.h"
#include "StringView.h"
#include "BoolView.h"

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
    
    ValueView* pView = NULL;
    
    nglString typeStr;
    if (rVal.isNull())
    {
      typeStr += _T("(null):");
      
      pView = new NullView(rVal);
    }
    else if (rVal.isBool())
    {
      typeStr += _T("(bool):");
      
      pView = new BoolView(rVal);
    }
    else if (rVal.isInt())
    {
      typeStr += _T("(int):");
      
      pView = new NumberView(rVal);
    }
    else if (rVal.isUInt())
    {
      typeStr += _T("(uint):");
      
      pView = new NumberView(rVal);
    }
    else if (rVal.isDouble())
    {
      typeStr += _T("(double):");
      
      pView = new NumberView(rVal);
    }
    else if (rVal.isString())
    {
      typeStr += _T("(string):");
      
      pView = new StringView(rVal);
    }
    else if (rVal.isArray())
    {
      typeStr += _T("(array):");
    }
    else if (rVal.isObject())
    {
      typeStr += _T("(object):");
      
      pView = new ObjectView(rVal);
    }
    
    nuiLabel* pLabel = new nuiLabel(name);
    pLabel->SetObjectName(_T("attributeNameLabel"));
    pHBox->AddCell(pLabel);
    
    nuiLabel* pTypeLabel = new nuiLabel(typeStr);
    pTypeLabel->SetObjectName(_T("attributeNameTypeLabel"));
    pHBox->AddCell(pTypeLabel);
    
    NGL_ASSERT(pView);
    pHBox->AddCell(pView);
  }
}

ObjectView::~ObjectView()
{
  
}