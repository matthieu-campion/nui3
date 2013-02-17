/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

//class nuiFontBrowser : public nuiSimpleContainer
nuiFontBrowser::nuiFontBrowser()
: mSink(this)
{
  SetObjectClass(_T("nuiFontBrowser"));
  
  mFontSize = 14;
  mpFontDesc = NULL;
  
  SetPosition(nuiFill);
  
  nuiSplitter* pBox = new nuiSplitter(nuiHorizontal);
  AddChild(pBox);
  pBox->SetObjectName(_T("nuiFontBrowser_MainBox"));
  //pBox->SetExpand(nuiExpandShrinkAndGrow);

  nuiHBox* pPreview = new nuiHBox();
  pBox->AddChild(pPreview);
  
  nuiScrollView* pScroll = new nuiScrollView();
  pScroll->SetObjectName(_T("nuiFontBrowser_PreviewScroller"));
  mpPreviewLabel = new nuiLabel(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n01234567890!@#$%^&*(){}[]\\|/?<>,.:"));
  mpPreviewLabel->SetFont(nuiFont::GetFont(mFontSize));
  mpPreviewLabel->SetPosition(nuiCenter);
  pScroll->AddChild(mpPreviewLabel);
  pPreview->AddCell(pScroll);
  pPreview->SetExpand(nuiExpandShrinkAndGrow);

  nuiVBox* pSizeBox = new nuiVBox();
  pSizeBox->SetExpand(nuiExpandShrinkAndGrow);
  mpSizeSlider = new nuiSlider(nuiVertical, nuiRange(mFontSize, 1, 100));
  pSizeBox->AddCell(mpSizeSlider);
  nglString str;
  str.SetCUInt(mFontSize);
  mpSizeLabel = new nuiLabel(str);
  pSizeBox->AddCell(mpSizeLabel);
  pSizeBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  pSizeBox->SetCellExpand(1, nuiExpandFixed);
  mpSizeLabel->SetPosition(nuiFillHorizontal);
  mpSizeSlider->SetPosition(nuiFill);
  
  pPreview->AddCell(pSizeBox);
  pPreview->SetCellExpand(0, nuiExpandShrinkAndGrow);
  pPreview->SetCellExpand(1, nuiExpandFixed);
  uint32 width = 25;
  pPreview->SetCellMinPixels(1, width);
  pPreview->SetCellMaxPixels(1, width);
  
  nuiFontManager& rManager = nuiFontManager::GetManager();
  
  std::vector<nuiFontDesc*> FontList;
  rManager.GetFonts(FontList);

  { /// Build the font desc maps
    std::vector<nuiFontDesc*>::iterator it = FontList.begin();
    std::vector<nuiFontDesc*>::iterator end = FontList.end();
    
    while (it != end)
    {
      nuiFontDesc* pDesc = *it;
      mFonts[pDesc->GetName()][pDesc->GetStyle()] = pDesc;;
      
      ++it;
    }
  }

  nuiHBox* pHBox = new nuiHBox();
  nuiScrollView* pLeftScroll = new nuiScrollView();
  nuiScrollView* pRightScroll = new nuiScrollView();
  pHBox->AddCell(pLeftScroll);
  pHBox->AddCell(pRightScroll);
  mpFontList = new nuiList();
  mpStyleList = new nuiList();
  pLeftScroll->AddChild(mpFontList);
  pRightScroll->AddChild(mpStyleList);

  pBox->AddChild(pHBox);
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  
  {
    FontMap::const_iterator it = mFonts.begin();
    FontMap::const_iterator end = mFonts.end();
    
    while (it != end)
    {
      nuiLabel* pLabel = new nuiLabel(it->first);
      pLabel->SetProperty(_T("FontName"), it->first);
      mpFontList->AddChild(pLabel);
      
      ++it;
    }
  }
  
  mSink.Connect(mpFontList->SelectionChanged, &nuiFontBrowser::OnFontChanged);
  mSink.Connect(mpStyleList->SelectionChanged, &nuiFontBrowser::OnStyleChanged);
  mSink.Connect(mpSizeSlider->ValueChanged, &nuiFontBrowser::OnStyleChanged);
  mSink.Connect(mpSizeSlider->InteractiveValueChanged, &nuiFontBrowser::OnStyleChanged);
  
  mpFontList->SelectItem((uint)0);
}

nuiFontBrowser::~nuiFontBrowser()
{
}

void nuiFontBrowser::OnFontChanged(const nuiEvent& rEvent)
{
  nuiWidget* pLabel = mpFontList->GetSelected();
  if (pLabel)
  {
    FontMap::const_iterator it = mFonts.find(pLabel->GetProperty(_T("FontName")));
    const FontStyleMap& rMap(it->second);
    FontStyleMap::const_iterator sit = rMap.begin();
    FontStyleMap::const_iterator send = rMap.end();
    
    mpStyleList->Clear();
    while (sit != send)
    {
      nuiLabel* pFontStyle = new nuiLabel(sit->first);
      pFontStyle->SetProperty(_T("FontStyle"), sit->first);
      mpStyleList->AddChild(pFontStyle);

      ++sit;
    }
    
    mpStyleList->SelectItem((uint)0);
  }
}
    
    
void nuiFontBrowser::OnStyleChanged(const nuiEvent& rEvent)
{
  mFontSize = mpSizeSlider->GetRange().GetValue();
  nglString str;
  str.SetCUInt(mFontSize);
  mpSizeLabel->SetText(str);
  
  nuiWidget* pFontLabel = mpFontList->GetSelected();
  nuiWidget* pStyleLabel = mpStyleList->GetSelected();
  if (pFontLabel && pStyleLabel)
  {
    const nglString& rFontName(pFontLabel->GetProperty(_T("FontName")));
    const nglString& rFontStyle(pStyleLabel->GetProperty(_T("FontStyle")));
    SetFont(rFontName, rFontStyle, mFontSize);
    FontChanged();
  }
}

nuiFontDesc* nuiFontBrowser::GetCurrentFontDesc() const
{
  return mpFontDesc;
}

uint32 nuiFontBrowser::GetFontSize() const
{
  return mFontSize;
}

void nuiFontBrowser::SetFont(const nglString& rFontName, const nglString& rFontStyle, uint32 FontSize)
{
  mpFontDesc = mFonts[rFontName][rFontStyle];
  mFontSize = FontSize;
  nglString str;
  str.SetCUInt(mFontSize);
  mpSizeLabel->SetText(str);
  mpSizeSlider->GetRange().SetValue(mFontSize);
  mpPreviewLabel->SetFont(nuiFont::GetFont(mpFontDesc->GetPath(), mFontSize, mpFontDesc->GetFace()));
}
