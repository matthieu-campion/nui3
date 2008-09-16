/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDefaultDecoration.h"
#include "nuiGradientDecoration.h"
#include "nuiBorderDecoration.h"
#include "nuiMetaDecoration.h"
#include "nuiDefaultDecoration_nuiButton.h"
#include "nuiFrame.h"

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
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiSplitterHandle")), &nuiDefaultDecoration::SplitterHandle);
  

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

void nuiDefaultDecoration::SplitterHandle(nuiWidget* pWidget)
{
  nuiSplitterHandle* pHandle = (nuiSplitterHandle*)pWidget;
  nuiSplitter* pSplitter = pHandle->GetSplitter();
  NGL_ASSERT(pSplitter);
  
  if (pSplitter->GetOrientation() == nuiVertical)
  {
    pHandle->SetUserSize(6,0);

    nuiMetaDecoration* pDeco = (nuiMetaDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationSplitterHandleVertical"));
    if (!pDeco)
    {
      nuiGradientDecoration* pDeco1 = new nuiGradientDecoration(_T("nuiDefaultDecorationSplitterHandleVertical1"), 
                                                                nuiRect(0, 0, 0, 0), nuiColor(232,232,232), nuiColor(196,196,196), nuiHorizontal);

      nuiBorderDecoration* pDeco2 = new nuiBorderDecoration(_T("nuiDefaultDecorationSplitterHandleVertical2"));
      pDeco2->SetBorderType(_T("All"));
      pDeco2->SetStrokeSize(1);
      pDeco2->SetStrokeLeftColor(nuiColor(190,190,190));
      pDeco2->SetStrokeRightColor(nuiColor(170,170,170));
      pDeco2->SetStrokeTopColor(nuiColor(180,180,180));
      pDeco2->SetStrokeBottomColor(nuiColor(180,180,180));
      
      pDeco = new nuiMetaDecoration(_T("nuiDefaultDecorationSplitterHandleVertical"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleVertical1"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleVertical2"));
      
    }
    pHandle->SetDecoration(pDeco, eDecorationBorder);  
    
  }
  else
  {
    pHandle->SetUserSize(0,6);
    
    nuiMetaDecoration* pDeco = (nuiMetaDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationSplitterHandleHorizontal"));
    if (!pDeco)
    {
      nuiGradientDecoration* pDeco1 = new nuiGradientDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal1"), 
                                                                nuiRect(0, 0, 0, 0), nuiColor(232,232,232), nuiColor(196,196,196), nuiVertical);
      nuiBorderDecoration* pDeco2 = new nuiBorderDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal2"));
      pDeco2->SetBorderType(_T("All"));
      pDeco2->SetStrokeSize(1);
      pDeco2->SetStrokeLeftColor(nuiColor(180,180,180));
      pDeco2->SetStrokeRightColor(nuiColor(180,180,180));
      pDeco2->SetStrokeTopColor(nuiColor(190,190,190));
      pDeco2->SetStrokeBottomColor(nuiColor(170,170,170));
      
      pDeco = new nuiMetaDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal1"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal2"));
      
    }
   pHandle->SetDecoration(pDeco, eDecorationBorder);  
  }
  
  
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



//void nuiDefaultDecoration::ScrollView(nuiWidget* pWidget)
//{
//  
//  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationScrollView"));
//  if (!pDeco)
//  {
//    nuiColor cPane1,cPane2,cStroke;
//    cPane1.SetValue(_T("nuiDefaultColorPane1"));
//    cPane2.SetValue(_T("nuiDefaultColorPane2"));
//    cStroke.SetValue(_T("nuiDefaultColorStroke"));
//    
//    pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationScrollView"), nuiRect(5,5,0,0), cPane1, cPane2, nuiVertical, 1/*strokesize*/, cStroke, eStrokeAndFillShape);
//  }
//  
//  pWidget->SetDecoration(pDeco, eDecorationBorder);
//}


void nuiDefaultDecoration::TitledPane(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FolderPane(nuiWidget* pWidget)
{
  
}



void nuiDefaultDecoration::Button(nuiWidget* pWidget)
{

  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationButton"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMemUp = new nglIMemory(gpDefaultDecorationButtonUp, gpDefaultDecorationButtonUpSize);
  nuiTexture* pTexUp = nuiTexture::GetTexture(pIMemUp);
  NGL_ASSERT(pTexUp);
  nuiFrame* pFrameUp = new nuiFrame(_T("nuiDefaultDecorationButtonUp"), pTexUp, nuiRect(5,5, 1,19));
  delete pIMemUp;

  
  nglIMemory* pIMemHover = new nglIMemory(gpDefaultDecorationButtonHover, gpDefaultDecorationButtonHoverSize);
  nuiTexture* pTexHover = nuiTexture::GetTexture(pIMemHover);
  NGL_ASSERT(pTexHover);
  nuiFrame* pFrameHover = new nuiFrame(_T("nuiDefaultDecorationButtonHover"), pTexHover, nuiRect(5,5, 1,19));
  delete pIMemHover;
  
  
  nglIMemory* pIMemDown = new nglIMemory(gpDefaultDecorationButtonDown, gpDefaultDecorationButtonDownSize);
  nuiTexture* pTexDown = nuiTexture::GetTexture(pIMemDown);
  NGL_ASSERT(pTexDown);
  nuiFrame* pFrameDown = new nuiFrame(_T("nuiDefaultDecorationButtonDown"), pTexDown, nuiRect(5,5, 1,19));
  delete pIMemDown;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationButton"), 
                                                      _T("nuiDefaultDecorationButtonUp"),
                                                      _T("nuiDefaultDecorationButtonDown"),
                                                      _T("nuiDefaultDecorationButtonHover"));

  pWidget->SetDecoration(pState, eDecorationBorder);

}


void nuiDefaultDecoration::ToggleButton(nuiWidget* pWidget)
{
  
}

