/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define CELL_CONTENTS 1
#define CELL_CLOSINGCONTENTS 2


nuiFolderPane::nuiFolderPane(nuiWidget* pTitleWidget, bool opened)
: nuiTitledPane()
{
  SetObjectClass(_T("nuiFolderPane"));
  SetTitleWithHandle(pTitleWidget, nuiLeft);
  Init(opened);
}



nuiFolderPane::nuiFolderPane(const nglString& rText, bool opened)
: nuiTitledPane()
{
  SetObjectClass(_T("nuiFolderPane"));
  SetTitleWithHandle(rText, nuiLeft);
  Init(opened);
  
}


nuiFolderPane::nuiFolderPane(nuiLabelAttribute* pLabel, bool opened)
: nuiTitledPane()
{
  SetObjectClass(_T("nuiFolderPane"));
  SetTitleWithHandle(pLabel, nuiLeft);
  Init(opened);
  
}

void nuiFolderPane::Init (bool opened)
{
  mClicked = false;
  mIsOpened = opened;
  mInteractiveCloseEnabled = true;
  nuiWidget* pTitle = GetTitle();
  
  if (mIsOpened)
    pTitle->SetSelected(true);
    
  pTitle->SetColor(eSelectedTextFg, pTitle->GetColor(eNormalTextFg));
  
  AddCell(NULL); // a third cell to the titledpane for the WidgetOnClosing
  
  SetLayoutAnimationDuration(0.1f);
  SetLayoutAnimationEasing(nuiEasingSinus);
  nuiRectAttributeAnimation* pAnim = GetLayoutAnimation(false);
  pAnim->SetWidthAnim(false);
  pAnim->SetAutoRound(true);
}


nuiFolderPane::~nuiFolderPane()
{

}


bool nuiFolderPane::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mClicked && Button & nglMouseInfo::ButtonLeft)
  {
    Ungrab();
    nuiRect rect = GetTitle()->GetRect();
    rect.SetSize (GetRect().GetWidth(), rect.GetHeight());
    rect.MoveTo (0, rect.Top());
    
    if (rect.IsInside (X, Y))
    {
      if (!mInteractiveCloseEnabled)
        return false;
      
      if (mIsOpened)
        Close();
      else
        Open();
    }
    return true;
  }
  return false;
}

bool nuiFolderPane::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    nuiRect rect = GetTitle()->GetRect();
    rect.SetSize (GetRect().GetWidth(), rect.GetHeight());
    rect.MoveTo (0, rect.Top());
    
    if (rect.IsInside (X, Y))
    {
      mClicked = true;
      Grab();
    }
    return true;
  }
  return false;
}






void nuiFolderPane::SetContentsOnClosing(nuiWidget* pWidget, nuiPosition position)
{
  SetCell(CELL_CLOSINGCONTENTS, pWidget, position);
}

nuiWidget* nuiFolderPane::GetContentsOnClosing()
{
  return GetCell(CELL_CLOSINGCONTENTS);
}


bool nuiFolderPane::IsOpened()
{
  return mIsOpened;
}

void nuiFolderPane::EnableInteractiveClose(bool set)
{
  mInteractiveCloseEnabled = set;
}

// virtual, to be optionally overloaded
void nuiFolderPane::Open()
{
  if (mIsOpened)
    return;
  
  SetCellPixels(CELL_CLOSINGCONTENTS, 0.0f);      
  ResetCellPixels(CELL_CONTENTS);

  GetTitle()->SetSelected(true);
  mIsOpened = true;
  
  Opened();
  
  SetHotRect(GetIdealRect().Size());
}

// virtual, to be optionally overloaded
void nuiFolderPane::Close()
{
  if (!mIsOpened)
    return;
    
  SetCellPixels(CELL_CONTENTS, 0.0f);      
  ResetCellPixels(CELL_CLOSINGCONTENTS);

  GetTitle()->SetSelected(false);
  mIsOpened = false;
  
  Closed();
}

//virtual
void nuiFolderPane::SetTitle(const nglString& rTitle, nuiPosition position)
{
  nuiLabel* label = new nuiLabel(rTitle, nuiFont::GetFont (12));
  SetCell(0, label, position);
  
  if (mIsOpened)
    label->SetSelected(true);
  
  label->SetObjectName(_T("nuiFolderPane::Title"));  
      
  InvalidateLayout();
}	

//virtual
void nuiFolderPane::SetTitle(nuiWidget* pTitleWidget, nuiPosition position)
{
  SetCell(0, pTitleWidget, position);
  
  if (mIsOpened)
    pTitleWidget->SetSelected(true);
      
  InvalidateLayout();
}

void nuiFolderPane::SetTitleWithHandle(const nglString& rTitle, nuiPosition position)
{
  nuiLabel* pLabel = new nuiLabel(rTitle);
  pLabel->SetObjectName(_T("nuiFolderPane::TitleLabel"));
  SetTitleWithHandle(pLabel, position);
}


void nuiFolderPane::SetTitleWithHandle(nuiWidget* pTitleWidget, nuiPosition position)
{
  nuiHBox* pTitleBox = new nuiHBox(0);
  nuiSimpleContainer* pPaneHandle = new nuiSimpleContainer();
  
  nuiTreeHandleDecoration* pHandleDeco = (nuiTreeHandleDecoration*)nuiDecoration::Get(_T("nuiFolderPaneTreeHandleDecoration"));
  if (!pHandleDeco)
    pHandleDeco = new nuiTreeHandleDecoration(_T("nuiFolderPaneTreeHandleDecoration"), nuiColor(140,140,140), 8, nuiBorder(3,0,5,0));

  pPaneHandle->SetDecoration(pHandleDeco, eDecorationBorder);
  pTitleBox->AddCell(pPaneHandle);
  
  pTitleBox->AddCell(pTitleWidget);
  
  pTitleBox->SetObjectName(_T("nuiFolderPane::Title"));  
  
  SetTitle(pTitleBox);
}






