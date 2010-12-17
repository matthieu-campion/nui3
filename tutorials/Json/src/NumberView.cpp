/*
 *  NumberView.cpp
 *  Json
 *
 *  Created by  mat on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "NumberView.h"

NumberView::NumberView(nuiJson::Value& rValue)
: ValueView(rValue)
{ 
  if (SetObjectClass(_T("NumberView")))
  {
    AddAttribute(new nuiAttribute<int>(_T("intAttrib"), nuiUnitNone,
                                             nuiMakeDelegate(this, &NumberView::GetInt),
                                             nuiMakeDelegate(this, &NumberView::SetInt)));
    
    AddAttribute(new nuiAttribute<uint>(_T("uintAttrib"), nuiUnitNone,
                                       nuiMakeDelegate(this, &NumberView::GetUInt),
                                       nuiMakeDelegate(this, &NumberView::SetUInt)));
    
    AddAttribute(new nuiAttribute<double>(_T("doubleAttrib"), nuiUnitNone,
                                       nuiMakeDelegate(this, &NumberView::GetDouble),
                                       nuiMakeDelegate(this, &NumberView::SetDouble)));
  }
  
  nuiAttribBase attr;  
  if (mrValue.isInt())
  {
    attr = GetAttribute(_T("intAttrib"));
  }
  else if (mrValue.isUInt())
  {
    attr = GetAttribute(_T("uintAttrib"));
  }
  else if (mrValue.isDouble())
  {
    attr = GetAttribute(_T("doubleAttrib"));
  }
  
  nuiWidget* pEditor = attr.GetEditor();
  AddChild(pEditor);
}

NumberView::~NumberView()
{
  
}

void NumberView::SetInt(int val)
{
  if (!mrValue.isInt())
    return;
  
  mrValue = val;
}

int NumberView::GetInt()
{
  if (!mrValue.isInt())
    return 0;
  
  return mrValue.asInt();
}

void NumberView::SetUInt(uint val)
{
  if (!mrValue.isUInt())
    return;
  
  mrValue = val;
}

uint NumberView::GetUInt()
{
  if (!mrValue.isUInt())
    return 0;
  
  return mrValue.asUInt();
}

void NumberView::SetDouble(double val)
{
  if (!mrValue.isDouble())
    return;
  
  mrValue = val;
}

double NumberView::GetDouble()
{
  if (!mrValue.isDouble())
    return 0;
  
  return mrValue.asDouble();
}