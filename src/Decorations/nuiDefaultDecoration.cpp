/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDefaultDecoration.h"
#include "nuiColorDecoration.h"
#include "nuiGradientDecoration.h"
#include "nuiBorderDecoration.h"
#include "nuiMetaDecoration.h"
#include "nuiDefaultDecorationMaps.h"
#include "nuiFrame.h"

nuiDefaultDecoration::nuiDefaultDecoration()
{
  
}


nuiDefaultDecoration::~nuiDefaultDecoration()
{
  
} 

void nuiDefaultDecoration::Init()
{
  InitColors();
  InitSelection();
  InitMaps();
  InitIcons();
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiWindow")), &nuiDefaultDecoration::Window);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiLabel")), &nuiDefaultDecoration::Label);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiEditText")), &nuiDefaultDecoration::EditText);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiEditLine")), &nuiDefaultDecoration::EditLine);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiSplitterHandle")), &nuiDefaultDecoration::SplitterHandle);
  

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiTitledPane")), &nuiDefaultDecoration::TitledPane);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFolderPane")), &nuiDefaultDecoration::FolderPane);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiButton")), &nuiDefaultDecoration::Button);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiCloseButton")), &nuiDefaultDecoration::CloseButton);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiToggleButton")), &nuiDefaultDecoration::ToggleButton);
  
  
  // FileSelector
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::FolderView")), &nuiDefaultDecoration::FileSelector_FolderView);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::InfoView")), &nuiDefaultDecoration::FileSelector_InfoView);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::FolderLabel")), &nuiDefaultDecoration::FileSelector_FolderLabel);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::VolumeIcon")), &nuiDefaultDecoration::FileSelector_VolumeIcon);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::FolderIcon")), &nuiDefaultDecoration::FileSelector_FolderIcon);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeView")), &nuiDefaultDecoration::FileSelector_TreeView);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFolderLabel")), &nuiDefaultDecoration::FileSelector_TreeFolderLabel);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFileLabel")), &nuiDefaultDecoration::FileSelector_TreeFileLabel);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFolderIcon")), &nuiDefaultDecoration::FileSelector_TreeFolderIcon);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFileIcon")), &nuiDefaultDecoration::FileSelector_TreeFileIcon);

}



void nuiDefaultDecoration::InitColors()
{
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBkg1"), nuiColor(197,197,197));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBkg2"), nuiColor(150,150,150));

  nuiColor::SetColor(_T("nuiDefaultClrCaptionBorder"), nuiColor(150,150,150));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBorderLight"), nuiColor(238,238,238));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBorderDark"), nuiColor(64,64,64));

  nuiColor::SetColor(_T("nuiDefaultClrCaptionText"), nuiColor(16,16,16));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionTextLight"), nuiColor(215,215,215));
  
  nuiColor::SetColor(_T("nuiDefaultClrSelection"), nuiColor(56,117,215));
}


void nuiDefaultDecoration::InitSelection()
{
  nuiColor color;
  nuiColor::GetColor(_T("nuiDefaultClrSelection"), color);
  
  nuiColorDecoration* pDeco = new nuiColorDecoration(_T("nuiDefaultDecorationSelectionBackground"), 
                                                     nuiRect(3,3,0,0), color);
}


void nuiDefaultDecoration::InitMaps()
{
  // vertical scrollbar background
  nglIMemory* pIMem = new nglIMemory(gpDefaultDecorationScrollbarVerticalBkg, gpDefaultDecorationScrollbarVerticalBkgSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarVerticalBkg"), pTex, nuiRect(6,8,0,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;

  // vertical scrollbar handle
  pIMem = new nglIMemory(gpDefaultDecorationScrollbarVerticalHdl, gpDefaultDecorationScrollbarVerticalHdlSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarVerticalHdl"), pTex, nuiRect(2,6,6,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // Horizontal scrollbar background
  pIMem = new nglIMemory(gpDefaultDecorationScrollbarHorizontalBkg, gpDefaultDecorationScrollbarHorizontalBkgSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarHorizontalBkg"), pTex, nuiRect(8,6,0,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // Horizontal scrollbar handle
  pIMem = new nglIMemory(gpDefaultDecorationScrollbarHorizontalHdl, gpDefaultDecorationScrollbarHorizontalHdlSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarHorizontalHdl"), pTex, nuiRect(6,2,0,6));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  



  // arrow handle close
  pIMem = new nglIMemory(gpDefaultDecorationArrowClose, gpDefaultDecorationArrowCloseSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationArrowClose"), pTex, nuiRect(0,0,7,6));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  

  // arrow handle open
  pIMem = new nglIMemory(gpDefaultDecorationArrowOpen, gpDefaultDecorationArrowOpenSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationArrowOpen"), pTex, nuiRect(0,0,6,7));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  
}



void nuiDefaultDecoration::InitIcons()
{
  // volume icon
  nglIMemory* pIMem = new nglIMemory(gpDefaultDecorationIconVolume, gpDefaultDecorationIconVolumeSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationIconVolume"), pTex, nuiRect(0,0,18,19));
  pFrame->UseWidgetAlpha(false);
  delete pIMem; 

  // folder icon
  pIMem = new nglIMemory(gpDefaultDecorationIconFolder, gpDefaultDecorationIconFolderSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationIconFolder"), pTex, nuiRect(0,0,18,16));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  
  
  // file icon
  pIMem = new nglIMemory(gpDefaultDecorationIconFile, gpDefaultDecorationIconFileSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationIconFile"), pTex, nuiRect(0,0,16,20));
  pFrame->UseWidgetAlpha(false);
  delete pIMem; 
}


void nuiDefaultDecoration::Window(nuiWidget* pWidget)
{
  nuiWindow* pWindow = (nuiWindow*)pWidget;
  if (pWindow->GetFlags() & nuiWindow::DecoratedBackground)
  {
    nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationWindow"));
    if (!pDeco)
    {
      pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationWindow"), 
                                        nuiRect(0,0, 0,0), nuiColor(245,245,245), nuiColor(235,235,235), nuiVertical, 1, nuiColor(175,175,175), eStrokeAndFillShape);
    }
    pWindow->SetDecoration(pDeco);
  }
  else
  {
    pWindow->SetColor(eActiveWindowBg, nuiColor(255, 255, 255));
    pWindow->SetColor(eInactiveWindowBg, nuiColor(200, 200, 200));
  }

  // see nuiTheme::DrawActiveWindow for the rest
  
}





void nuiDefaultDecoration::Label(nuiWidget* pWidget)
{
//  nuiColor cText;
//  cText.SetValue(_T("nuiDefaultColorLabelText"));
//  pWidget->SetColor(eNormalTextFg, cText);
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
  nuiFrame* pFrameUp = new nuiFrame(_T("nuiDefaultDecorationButtonUp"), pTexUp, nuiRect(4,5,2,7));
  delete pIMemUp;

  
  nglIMemory* pIMemHover = new nglIMemory(gpDefaultDecorationButtonHover, gpDefaultDecorationButtonHoverSize);
  nuiTexture* pTexHover = nuiTexture::GetTexture(pIMemHover);
  NGL_ASSERT(pTexHover);
  nuiFrame* pFrameHover = new nuiFrame(_T("nuiDefaultDecorationButtonHover"), pTexHover, nuiRect(4,5,2,7));
  delete pIMemHover;
  
  
  nglIMemory* pIMemDown = new nglIMemory(gpDefaultDecorationButtonDown, gpDefaultDecorationButtonDownSize);
  nuiTexture* pTexDown = nuiTexture::GetTexture(pIMemDown);
  NGL_ASSERT(pTexDown);
  nuiFrame* pFrameDown = new nuiFrame(_T("nuiDefaultDecorationButtonDown"), pTexDown, nuiRect(4,5,2,7));
  delete pIMemDown;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationButton"), 
                                                      _T("nuiDefaultDecorationButtonUp"),
                                                      _T("nuiDefaultDecorationButtonDown"),
                                                      _T("nuiDefaultDecorationButtonHover"));
  pState->SetSourceClientRect(nuiRect(4,4,2,9));

  pWidget->SetDecoration(pState, eDecorationBorder);

}



void nuiDefaultDecoration::CloseButton(nuiWidget* pWidget)
{
  
  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationCloseButton"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMemUp = new nglIMemory(gpDefaultDecorationCloseButtonUp, gpDefaultDecorationCloseButtonUpSize);
  nuiTexture* pTexUp = nuiTexture::GetTexture(pIMemUp);
  NGL_ASSERT(pTexUp);
  nuiFrame* pFrameUp = new nuiFrame(_T("nuiDefaultDecorationCloseButtonUp"), pTexUp, nuiRect(0,0,12,15));
  delete pIMemUp;
  
  
  nglIMemory* pIMemHover = new nglIMemory(gpDefaultDecorationCloseButtonHover, gpDefaultDecorationCloseButtonHoverSize);
  nuiTexture* pTexHover = nuiTexture::GetTexture(pIMemHover);
  NGL_ASSERT(pTexHover);
  nuiFrame* pFrameHover = new nuiFrame(_T("nuiDefaultDecorationCloseButtonHover"), pTexHover, nuiRect(0,0,12,15));
  delete pIMemHover;
  
  
  nglIMemory* pIMemDown = new nglIMemory(gpDefaultDecorationCloseButtonDown, gpDefaultDecorationCloseButtonDownSize);
  nuiTexture* pTexDown = nuiTexture::GetTexture(pIMemDown);
  NGL_ASSERT(pTexDown);
  nuiFrame* pFrameDown = new nuiFrame(_T("nuiDefaultDecorationCloseButtonDown"), pTexDown, nuiRect(0,0,12,15));
  delete pIMemDown;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationCloseButton"), 
                                                      _T("nuiDefaultDecorationCloseButtonUp"),
                                                      _T("nuiDefaultDecorationCloseButtonDown"),
                                                      _T("nuiDefaultDecorationCloseButtonHover"));
  pState->SetSourceClientRect(nuiRect(0,0,12,15));
  
  pWidget->SetDecoration(pState, eDecorationBorder);
  
}


void nuiDefaultDecoration::ToggleButton(nuiWidget* pWidget)
{
  
}







//***********************************************************************************************************************************
//
// FileSelector
//
//***********************************************************************************************************************************

void nuiDefaultDecoration::FileSelector_FolderView(nuiWidget* pWidget)
{
  nuiMetaDecoration* pDeco = (nuiMetaDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationFileSelector_FolderView"));
  if (!pDeco)
  {
    nuiColorDecoration* pDeco1 = new nuiColorDecoration(_T("nuiDefaultDecorationFileSelector_FolderView_1"), nuiRect(5,5,0,0), nuiColor(214,221,229));
    nuiBorderDecoration* pDeco2 = new nuiBorderDecoration(_T("nuiDefaultDecorationFileSelector_FolderView_2"));
    pDeco2->SetStrokeColor(nuiColor(139,139,139));
    pDeco2->SetStrokeSize(1);
    pDeco2->SetBorderType(_T("Right"));

    pDeco = new nuiMetaDecoration(_T("nuiDefaultDecorationFileSelector_FolderView"));
    pDeco->AddDecoration(pDeco1);
    pDeco->AddDecoration(pDeco2);
  }
  pWidget->SetDecoration(pDeco);
}

void nuiDefaultDecoration::FileSelector_InfoView(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_FolderLabel(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_VolumeIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconVolume"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);
  pWidget->SetUserSize(18,19);
}


void nuiDefaultDecoration::FileSelector_FolderIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconFolder"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);  
  pWidget->SetUserSize(18,16);
}


void nuiDefaultDecoration::FileSelector_TreeView(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_TreeFolderLabel(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_TreeFileLabel(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_TreeFolderIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconFolder"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);    
  pWidget->SetUserSize(18,16);
}


void nuiDefaultDecoration::FileSelector_TreeFileIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconFile"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);    
  pWidget->SetUserSize(16,20);
}




//***********************************************************************************************************************************
//
// special cases
//
//***********************************************************************************************************************************

void nuiDefaultDecoration::MainWindow(nuiMainWindow* pWindow)
{
  nuiWidget* pCont = new nuiWidget();
  pWindow->AddChild(pCont);
  nuiGradientDecoration* pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationMainWindow"), 
                                                           nuiRect(0,0, 0,0), nuiColor(245,245,245), nuiColor(225,225,225), nuiVertical, 0, nuiColor(0,0,0), eFillShape);
  pCont->SetDecoration(pDeco);  
}

