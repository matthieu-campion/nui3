/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"

//class nuiShapeView : public nuiWidget
nuiShapeView::nuiShapeView(nuiShape* pShape)
: nuiWidget()
{
  SetObjectClass(_T("nuiShapeView"));
  mpShape = pShape;
  mShapeMode = eStrokeShape;
  mAntialiasing = true;
  mLineWidth = 1.0f;
}

nuiShapeView::~nuiShapeView()
{
  delete mpShape;
}

nuiRect nuiShapeView::CalcIdealSize()
{
	if (!mpShape) 
		return nuiRect(0,0,0,0);
  return mpShape->GetRect().Size();
}

bool nuiShapeView::Draw(nuiDrawContext* pContext)
{
	if (!mpShape)
		return true;
  nuiRect rect = mpShape->GetRect();
  nuiSize x = 0,y = 0;
  if (rect.Top() < 0)
    y = -rect.Top();
  if (rect.Left() < 0)
    x = -rect.Left();
  pContext->SetStrokeColor(GetColor(eShapeStroke));
  pContext->SetFillColor(GetColor(eShapeFill));

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableAntialiasing(mAntialiasing);
  pContext->SetLineWidth(mLineWidth);
  pContext->PushMatrix();
  pContext->Translate(x, y);
  pContext->DrawShape(mpShape, mShapeMode);
  pContext->PopMatrix();

  return true;
}

void nuiShapeView::SetShapeMode(nuiShapeMode ShapeMode)
{
  mShapeMode = ShapeMode;
  Invalidate();
}

nuiShapeMode nuiShapeView::GetShapeMode() const
{
  return mShapeMode;
}

void nuiShapeView::EnableAntialiasing(bool Set)
{
  mAntialiasing = Set;
  Invalidate();
}

bool nuiShapeView::ISAntialiasingEnabled() const
{
  return mAntialiasing;
}


void nuiShapeView::SetLineWidth(nuiSize Set)
{
  mLineWidth = Set;
  Invalidate();
}

nuiSize nuiShapeView::GetLineWidth() const
{
  return mLineWidth;
}

