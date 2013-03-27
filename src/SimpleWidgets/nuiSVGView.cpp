/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiSVGView::nuiSVGView(nuiSVGShape* pShape)
  : nuiWidget(),
    mCache()
{
  nuiSVGView::Init();
  mpShape = pShape;
  mAntialias = true;
}

nuiSVGView::nuiSVGView(const nglPath& rSource)
  : nuiWidget(),
    mCache()
{
  nuiSVGView::Init();
  nglIStream* pFile = rSource.OpenRead();
  if (pFile)
  {
    Load(*pFile);
    delete pFile;
  }
}

nuiSVGView::~nuiSVGView()
{
  delete mpShape;
}

void nuiSVGView::Init()
{
  SetObjectClass(_T("nuiSVGView"));
  mpShape = NULL;
  mAntialias = true;
}


bool nuiSVGView::Load (nglIStream& rSource)
{
  bool result = false;
  if (mpShape)
    delete mpShape;

  nuiXML XML("SVG File");
  mpShape = new nuiSVGShape();
  InvalidateLayout();
  nglTime t;
  if (!XML.Load(rSource))
    return false;
  nglTime xmlt;
  result = mpShape->Load(&XML);
  nglTime loadt;

  NGL_OUT(_T("Xml load time: %f\nSvg load time: %f\n"), (double)xmlt - (double)t, (double)loadt - (double)xmlt);

  return result;
}

bool nuiSVGView::Draw(nuiDrawContext* pContext)
{
  if (!mpShape || !IsVisible())
    return true;
  pContext->ResetState();
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableBlending(true);
  pContext->PushMatrix();
  float x,y;
  x = mRect.GetWidth() / mIdealRect.GetWidth();
  y = mRect.GetHeight() / mIdealRect.GetHeight();
  pContext->Scale(x,y);
  pContext->SetFont(nuiFont::GetFont(12));
  mpShape->Draw(pContext, mAntialias);
  pContext->PopMatrix();
  return true;
}

nuiRect nuiSVGView::CalcIdealSize()
{
  if (!mpShape)
  {
    mIdealRect = nuiRect();
    return mIdealRect;
  }
  mIdealRect = mpShape->GetBoundingRect();
  // Add 2% to the total size in order to balance not taking splines into account while calculating the bounding box.
  mIdealRect.SetSize(mIdealRect.GetWidth()*1.02f, mIdealRect.GetHeight()*1.02f);  
  return mIdealRect;
}

void nuiSVGView::EnableAntialias(bool Antialias)
{
  mAntialias = Antialias;
  Invalidate();
}

bool nuiSVGView::IsAntialiasEnabled()
{
  return mAntialias;
}
