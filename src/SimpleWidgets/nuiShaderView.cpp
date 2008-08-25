/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiShaderView.h"
#include "nuiShader.h"

nuiShaderView::nuiShaderView(nuiShader* pShader)
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiShaderView"));
  mpShader = pShader;
}

bool nuiShaderView::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  mpShader = NULL;
  
  return true;
}

nuiShaderView::~nuiShaderView()
{
  delete mpShader;
}

bool nuiShaderView::Draw(nuiDrawContext* pContext)
{
  bool res = false;

  //mpShader->Bind();
  pContext->SetShader(mpShader);
  res = DrawChildren(pContext);
  pContext->SetShader(NULL);
  //mpShader->UnBind();

  return res;
}

