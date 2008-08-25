/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiOffscreenView.h"
#include "nuiMainWindow.h"
#include "nuiTopLevel.h"
#include "nuiGLPainter.h"
#include "nuiTexture.h"

#define OFFSCREEN_WIDTH 512
#define OFFSCREEN_HEIGHT 512

//class nuiOffscreenView: public nuiSimpleContainer
nuiOffscreenView::nuiOffscreenView(nuiContainerPtr pParent)
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiOffscreenView"));

  NGL_OUT(_T("Ext: %s"), glGetString(GL_EXTENSIONS));
  nglContextInfo Info;
  Info.FrameBitsA = 8;
  Info.StencilBits = 0;
  Info.RenderToTexture = true;
  Info.Offscreen = true;
  mRedrawChildren = true;
}

nuiOffscreenView::~nuiOffscreenView()
{
  if (mpContext)
    delete mpContext;
}

nuiXMLNode* nuiOffscreenView::Serialize(nuiXMLNode* pParentNode) const
{
  return nuiSimpleContainer::Serialize(pParentNode);
}

bool nuiOffscreenView::Draw(nuiDrawContext* pContext)
{
  bool needredraw = mRedrawChildren;

  bool toobig = mRect.GetWidth() > OFFSCREEN_WIDTH || mRect.GetHeight() > OFFSCREEN_HEIGHT;

  if (toobig)
  {
    return DrawChildren(pContext);
  }

  nuiWidgetList::iterator it = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  while (!needredraw && it != end)
  {
    needredraw |= (*it)->GetNeedRender();
    ++it;
  }

  if (needredraw)
  {
    mRedrawChildren = false;
    
    nuiRenderState State = pContext->GetState();

    nuiSize OffX,OffY;
    OffX = OffY = 0;
    LocalToGlobal(OffX, OffY);
/*  FIXME: disabled during the transition to the new rendering system
    mpContext->StartRendering(OffX, OffY);
    mpContext->SetState(State);
    mpContext->ResetClipRect(true);
    mpContext->EnableClipping(false);

    mpContext->Clear();
    mpContext->LoadIdentity();

    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawChildren(mpContext, false);

    mpContext->StopRendering();
    mpOffscreen->EndSession();
    */
  }

  dynamic_cast<nuiMainWindow*>(GetTopLevel())->BeginSession();

  nuiRect rectDest(0.0f, 0.0f, mRect.GetWidth(), mRect.GetHeight());
//  nuiRect rectDest(0.0f, 0.0f, 512.f, 512.f);
  nuiRect rectSrc(rectDest);

  mpTexture->SetEnvMode(GL_MODULATE);
  pContext->SetTexture(mpTexture);

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(true);
//  if (needredraw)
//    pContext->SetFillColor(nuiColor(1,0,0,1));
//  else
    pContext->SetFillColor(nuiColor(1.0f, 1.0f, 1.0f, 1.0f));
    pContext->ResetClipRect();
    pContext->DrawImage(rectDest, rectSrc);

  return true;
}

bool nuiOffscreenView::SetRect(const nuiRect& rRect)
{
  if (!(rRect == GetRect()))
    mRedrawChildren = true;
  return nuiSimpleContainer::SetRect(rRect);
}

