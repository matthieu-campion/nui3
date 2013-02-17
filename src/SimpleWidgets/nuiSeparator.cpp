/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiSeparator::nuiSeparator(nuiOrientation orientation)
: nuiWidget()
{
  SetObjectClass(_T("nuiSeparator"));
  mOrientation = orientation;
}

nuiSeparator::~nuiSeparator()
{
}


nuiRect nuiSeparator::CalcIdealSize()
{
  nuiContainer* pParent = GetParent();
  nuiRect rect(0,0,32, 32);
  if (pParent)
    rect = pParent->GetRect();
  nuiRect res;
  switch (mOrientation)
  {
  case nuiHorizontal:
    res.Set(0.0f, 0.0f, rect.GetWidth(), 3.0f);
    break;
  case nuiVertical:
    res.Set(0.0f, 0.0f, 3.0f, rect.GetHeight());
    break;
  }
  
  return res;
}


bool nuiSeparator::Draw(nuiDrawContext *pContext)
{
  nuiRect r(GetRect());

  // TODO : vertical draw 
  
  pContext->SetStrokeColor(nuiColor(255, 255, 255));
  pContext->DrawLine(r.Left(), 0, r.Right(), 0);

  pContext->SetStrokeColor(nuiColor(0, 0, 0, 56));
  pContext->DrawLine(r.Left(), 1.5, r.Right(), 1.5);
  
  return true;
}


