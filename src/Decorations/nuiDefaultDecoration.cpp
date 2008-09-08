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
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiLabel")), &nuiDefaultDecoration::Label);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiEditText")), &nuiDefaultDecoration::EditText);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiEditLine")), &nuiDefaultDecoration::EditLine);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiScrollView")), &nuiDefaultDecoration::ScrollView);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiTitledPane")), &nuiDefaultDecoration::TitledPane);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFolderPane")), &nuiDefaultDecoration::FolderPane);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiButton")), &nuiDefaultDecoration::Button);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiToggleButton")), &nuiDefaultDecoration::ToggleButton);

}



void nuiDefaultDecoration::Label(nuiWidget* pWidget)
{
  nuiColor cText;
  cText.SetValue(_T("nuiDefaultColorLabelText"));
  pWidget->SetColor(eNormalTextFg, cText);
}

void nuiDefaultDecoration::EditText(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::EditLine(nuiWidget* pWidget)
{
  
}



void nuiDefaultDecoration::ScrollBar(nuiWidget* pWidget)
{
  
  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationScrollBar"));
  if (!pDeco)
  {
    nuiColor c1,c2,c3,cStroke;
    nuiScrollBar* pBar = (nuiScrollBar*)pWidget;
    nuiOrientation orient = pBar->GetOrientation();

    c1.SetValue(_T("nuiDefaultColorScrollBar1"));
    c2.SetValue(_T("nuiDefaultColorScrollBar2"));
    c3.SetValue(_T("nuiDefaultColorScrollBar3"));
    cStroke.SetValue(_T("nuiDefaultColorScrollBarStroke"));
    
    pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationScrollBar"), nuiRect(5,5,0,0), c1, c2, c3, c3, orient, 1/*strokesize*/, cStroke, eStrokeAndFillShape);
    pDeco->SetOffset1(0.f);
    pDeco->SetOffset2(0.1f);
    pDeco->SetOffset3(1.f);
    pDeco->SetOffset4(1.f);
  }
  
  pWidget->SetDecoration(pDeco, eDecorationBorder);
}



void nuiDefaultDecoration::ScrollView(nuiWidget* pWidget)
{
  
  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationScrollView"));
  if (!pDeco)
  {
    nuiColor cPane1,cPane2,cStroke;
    cPane1.SetValue(_T("nuiDefaultColorPane1"));
    cPane2.SetValue(_T("nuiDefaultColorPane2"));
    cStroke.SetValue(_T("nuiDefaultColorStroke"));
    
    pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationScrollView"), nuiRect(5,5,0,0), cPane1, cPane2, nuiVertical, 1/*strokesize*/, cStroke, eStrokeAndFillShape);
  }
  
  pWidget->SetDecoration(pDeco, eDecorationBorder);
}


void nuiDefaultDecoration::TitledPane(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FolderPane(nuiWidget* pWidget)
{
  
}



void nuiDefaultDecoration::Button(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::ToggleButton(nuiWidget* pWidget)
{
  
}

