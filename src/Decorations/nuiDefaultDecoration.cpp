/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDefaultDecoration.h"
#include "nuiGradientDecoration.h"


nuiDefaultDecoration::nuiDefaultDecoration()
{
  
}


nuiDefaultDecoration::~nuiDefaultDecoration()
{
  
}

void nuiDefaultDecoration::Init()
{
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiScrollView")), &nuiDefaultDecoration::nuiScrollView);
}


void nuiDefaultDecoration::nuiScrollView(nuiWidget* pWidget)
{
  nuiColor cPane1,cPane2,cStroke;
  
  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationScrollView"));
  if (!pDeco)
  {
    cPane1.SetValue(_T("nuiDefaultColorPane1"));
    cPane2.SetValue(_T("nuiDefaultColorPane2"));
    cStroke.SetValue(_T("nuiDefaultColorStroke"));

    
    pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationScrollView"), nuiRect(5,5,0,0), cPane1, cPane2, nuiVertical, 1/*strokesize*/, cStroke, eStrokeAndFillShape);
  }
  
  pWidget->SetDecoration(pDeco, eDecorationBorder);
}
