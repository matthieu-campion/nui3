/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiSVGView.h"
#include "nglTime.h"
#include "nglIFile.h"
#include "nglKernel.h"
#include "nuiTheme.h"

nuiSVGView::nuiSVGView(nuiSVGShape* pShape)
  : nuiWidget(),
    mCache(nuiRect())
{
  Init();
  mpShape = pShape;
  mAntialias = true;
}

nuiSVGView::nuiSVGView(const nglPath& rSource)
  : nuiWidget(),
    mCache(nuiRect())
{
  Init();
  nglIStream* pFile = rSource.OpenRead();
  if (pFile)
  {
    Load(*pFile);
    delete pFile;
  }
}

bool nuiSVGView::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  mCache = nuiRect();
  Init();

  if (pNode->HasAttribute("Source"))
  {
    nglIFile file(pNode->GetAttribute("Source"));
    Load(file);
  }
  else // Is the SVG Desc embedded in the nui XML desc? 
  {
    nuiXMLNode* pSVG = pNode->GetChild(_T("svg"));
    if (pSVG)
    {
      mpShape = new nuiSVGShape();
      mpShape->Load(pSVG);
    }
  }

  return true;
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

  nuiDrawContext* pContext = nuiDrawContext::CreateDrawContext(nuiRect(0, 0, 512, 512), eSoftware);
  nuiPainter* pPainter = pContext->GetPainter();
  mCache.Reset(NULL);
  nuiRect r(mpShape->GetBoundingRect());
  //mCache.SetSize(ToBelow(r.GetWidth()), ToBelow(r.GetHeight()));

  pContext->SetPainter(&mCache);
  mpShape->Draw(pContext, mAntialias);
  pContext->SetPainter(pPainter);

  delete pContext;
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
  nuiTheme* pTheme = GetTheme();
  pContext->SetFont(pTheme->GetFont(nuiTheme::Default), true);
//  mpShape->Draw(pContext, mAntialias);
  mCache.ReDraw(pContext);
  pContext->PopMatrix();
  pTheme->Release();
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
