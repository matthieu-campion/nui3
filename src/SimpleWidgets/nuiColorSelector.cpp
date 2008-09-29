/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiColorSelector.h"
#include "nuiMetaDecoration.h"
#include "nuiFrame.h"



nuiColorSelector::nuiColorSelector(const nuiColor& rInitialColor, const std::vector<nuiColor>& rInitialColorList, ColorSelectorMode mode /* = eModeRGB */)
: nuiSimpleContainer(), 
  mpSwatchGrid(NULL),
  mCurrentColor(rInitialColor), 
  mColorList(rInitialColorList),
  mNumColors(20),
  mSink(this)
{
  SetObjectClass(_T("nuiColorSelector"));
  
  mpMainBox = new nuiVBox(0);
  mpMainBox->SetObjectName(_T("nuiColorSelector::MainBox"));
  mpMainBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(mpMainBox);
  
  mpColorPane = new nuiSimpleContainer();
  mpColorPane->SetBorder(2.f, 2.f);
  mpColorPane->SetUserSize(20.f, 20.f);
  mpMainBox->AddCell(mpColorPane, nuiLeft);
  
  nuiMetaDecoration* pMetaDeco = new nuiMetaDecoration(_T("dnuiColorSelector::ColorPane"));
  mpColorPane->SetDecoration(pMetaDeco, eDecorationBorder);
  
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationCheckerboardSmall"));
  NGL_ASSERT(pFrame);
  pMetaDeco->AddDecoration(pFrame);

  mpColorPaneDeco = new nuiColorDecoration(_T("dnuiColorSelector::ColorPane::ColorDeco"), mCurrentColor, 1, nuiColor(0,0,0));
  pMetaDeco->AddDecoration(mpColorPaneDeco);

  
  mpTabView = new nuiTabView(nuiTop);
  mpTabView->SetObjectName(_T("nuiColorSelector::TabView"));
  mSink.Connect(mpTabView->TabSelect, &nuiColorSelector::OnTabSelected);
  mpMainBox->AddCell(mpTabView);
  
  mpTabView->AddTab(_T("RGB"), Tab_RGB());
  mpTabView->AddTab(_T("HSV"), Tab_HSV());
  mpTabView->AddTab(_T("Swatches"), Tab_Swatches());
  
  OnTabSelected(nuiEvent());
  
  switch (mode)
  {
    case eModeRGB:
      break;
    case eModeHSV:
      mpTabView->SelectTab(1);
      break;
    case eModeSwatch:
      mpTabView->SelectTab(2);
      break;
  }
}


nuiWidget* nuiColorSelector::Tab_RGB()
{
  nuiSize red = mCurrentColor.Red();
  nuiSize green = mCurrentColor.Green();
  nuiSize blue = mCurrentColor.Blue();
  nuiSize alpha = mCurrentColor.Alpha();
  
  mpRgbGrid = new nuiGrid(3, 4);
  mpRgbGrid->SetBorder(0,0,10,10);
    
  nuiPane* pPane = new nuiPane(nuiColor(1.f, 1.f, 1.f, 1.f), nuiColor(0.f, 0.f, 0.f, 1.f), eStrokeShape);
  nuiLabel* pLabel = new nuiLabel(_T("RGB"));
  pLabel->SetBorder(2.f, 1.f);
  pPane->AddChild(pLabel);
  
  mpRgbGrid->SetCell(0, 0, new nuiLabel(_T("r:")));
  mpRgbGrid->SetCell(0, 1, new nuiLabel(_T("g:")));
  mpRgbGrid->SetCell(0, 2, new nuiLabel(_T("b:")));
  mpRgbGrid->SetCell(0, 3, new nuiLabel(_T("a:")));
  
  mpRedSlider = new nuiSlider(nuiHorizontal, nuiRange(red, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 0, mpRedSlider);
  mSink.Connect(mpRedSlider->InteractiveValueChanged, &nuiColorSelector::RgbSliderChanged);
  mSink.Connect(mpRedSlider->ValueChanged, &nuiColorSelector::RgbSliderChanged);
  
  mpGreenSlider = new nuiSlider(nuiHorizontal, nuiRange(green, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 1, mpGreenSlider);
  mSink.Connect(mpGreenSlider->InteractiveValueChanged, &nuiColorSelector::RgbSliderChanged);
  mSink.Connect(mpGreenSlider->ValueChanged, &nuiColorSelector::RgbSliderChanged);
  
  mpBlueSlider = new nuiSlider(nuiHorizontal, nuiRange(blue, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 2, mpBlueSlider);
  mSink.Connect(mpBlueSlider->InteractiveValueChanged, &nuiColorSelector::RgbSliderChanged);
  mSink.Connect(mpBlueSlider->ValueChanged, &nuiColorSelector::RgbSliderChanged);
  
  mpRgbAlphaSlider = new nuiSlider(nuiHorizontal, nuiRange(alpha, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 3, mpRgbAlphaSlider);
  mSink.Connect(mpRgbAlphaSlider->InteractiveValueChanged, &nuiColorSelector::RgbSliderChanged);
  mSink.Connect(mpRgbAlphaSlider->ValueChanged, &nuiColorSelector::RgbSliderChanged);
  
  mpRgbGrid->SetColumnPixels(2, 25);
  
  mpRedSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(red, mpRedSliderLabel);
  mpRgbGrid->SetCell(2, 0, mpRedSliderLabel, nuiRight);
  
  mpGreenSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(green, mpGreenSliderLabel);
  mpRgbGrid->SetCell(2, 1, mpGreenSliderLabel, nuiRight);
  
  mpBlueSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(blue, mpBlueSliderLabel);
  mpRgbGrid->SetCell(2, 2, mpBlueSliderLabel, nuiRight);
  
  mpAlphaSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(alpha, mpAlphaSliderLabel);
  mpRgbGrid->SetCell(2, 3, mpAlphaSliderLabel, nuiRight);
  
  
  return mpRgbGrid;
}


void nuiColorSelector::FormatColor(nuiSize color, nuiLabel* pLabel)
{
  nglString str;
  uint32 value = uint32(color * 255);
  str.Format(_T("%d"), value);
  pLabel->SetText(str);
}


nuiWidget* nuiColorSelector::Tab_HSV()
{
  mpHsvGrid = new nuiGrid(2, 4);
  
  return mpHsvGrid;
}


nuiWidget* nuiColorSelector::Tab_Swatches()
{
  MakeSwatchGrid();

  return mpSwatchGrid;
}



nuiColorSelector::~nuiColorSelector()
{
}

bool nuiColorSelector::Load(const nuiXMLNode* pNode)
{
  return nuiSimpleContainer::Load(pNode);
}

nuiXMLNode* nuiColorSelector::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  return nuiSimpleContainer::Serialize(pParentNode, Recursive);
}

void nuiColorSelector::MakeSwatchGrid()
{
  if (mpSwatchGrid)
    mpSwatchGrid->Trash();
  
  mpSwatchGrid = new nuiGrid(5, 0);
  mpSwatchGrid->SetBorder(0,0,10,10);
  
  std::vector<nuiColor>::const_iterator end = mColorList.end();
  uint i = 0;
  for (std::vector<nuiColor>::const_iterator it = mColorList.begin(); it != end; ++it, i++)
  {
    mpSwatchGrid->AddRows(i, 1);
    nuiPane* pSwatchPane;
    pSwatchPane = new nuiPane(*it, nuiColor(32,32,32));
    nuiButton* pButton = new nuiButton();
    pButton->SetUserSize(16,16);
    pButton->SetDrawSelf(false);
    pButton->SetBorder(2,2);
    pButton->AddChild(pSwatchPane);
    
    pButton->EnableDecoration(false);
    
    
    mSink.Connect(pButton->Clicked, &nuiColorSelector::SwatchSelected, pSwatchPane);
    mpSwatchGrid->SetCell(i%5, i/5, pButton, nuiCenter);
  }
}

const nuiColor& nuiColorSelector::GetCurrentColor() const
{
  return mCurrentColor;
}

void nuiColorSelector::SetCurrentColor(const nuiColor& rColor)
{
  mCurrentColor = rColor;
  mpColorPaneDeco->SetFillColor(rColor);
  
  nuiSize red = rColor.Red();
  nuiSize green = rColor.Green();
  nuiSize blue = rColor.Blue();
  nuiSize alpha = rColor.Alpha();
  FormatColor(red, mpRedSliderLabel);
  FormatColor(green, mpGreenSliderLabel);
  FormatColor(blue, mpBlueSliderLabel);
  FormatColor(alpha, mpAlphaSliderLabel);
  
  mpColorPane->Invalidate();
  
  // send event
  ColorChanged(); 
}

void nuiColorSelector::SetColorList(const std::vector<nuiColor>& rColorList)
{
  mColorList = rColorList;
  MakeSwatchGrid();
  Invalidate();
}

void nuiColorSelector::SetNumColors(uint NumColors)
{
  mNumColors = NumColors;
  MakeSwatchGrid();
  Invalidate();
}

bool nuiColorSelector::SwatchSelected(const nuiEvent& rEvent)
{
  nuiPane* pPane = (nuiPane*) rEvent.mpUser;
  SetCurrentColor(pPane->GetFillColor());
  OnTabSelected(nuiEvent());
  
  // send event
  SwatchColorChanged(); 
  return true;
}

bool nuiColorSelector::OnTabSelected(const nuiEvent& rEvent)
{
  uint tabIndex = mpTabView->GetSelectedTab();
  switch (tabIndex)
  {
    case 0: //RGB
      Tab_RGB_Update();
      break;
    case 1: //HSV
      break;
    case 2: //Swatches
      break;
    default:
      break;
  }
  return true;
}

void nuiColorSelector::Tab_RGB_Update()
{
  mpRedSlider->ValueChanged.Disable();
  mpRedSlider->GetRange().SetValue(mCurrentColor.Red());
  mpRedSlider->ValueChanged.Enable();
  
  mpGreenSlider->ValueChanged.Disable();
  mpGreenSlider->GetRange().SetValue(mCurrentColor.Green());
  mpGreenSlider->ValueChanged.Enable();
  
  mpBlueSlider->ValueChanged.Disable();
  mpBlueSlider->GetRange().SetValue(mCurrentColor.Blue());
  mpBlueSlider->ValueChanged.Enable();
  
  mpRgbAlphaSlider->ValueChanged.Disable();
  mpRgbAlphaSlider->GetRange().SetValue(mCurrentColor.Alpha());
  mpRgbAlphaSlider->ValueChanged.Enable();

}

bool nuiColorSelector::RgbSliderChanged(const nuiEvent& rEvent)
{
  nuiSize red = (float)mpRedSlider->GetRange().GetValue();
  nuiSize green = (float)mpGreenSlider->GetRange().GetValue();
  nuiSize blue = (float)mpBlueSlider->GetRange().GetValue();
  nuiSize alpha = (float)mpRgbAlphaSlider->GetRange().GetValue();

  SetCurrentColor(nuiColor(red, green, blue, alpha));
  
  // send event
  RGBColorChanged();

  return true;
}

