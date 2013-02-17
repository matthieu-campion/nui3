/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"



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
  mSink.Connect(mpRedSlider->InteractiveValueChanged, &nuiColorSelector::RGBSliderChanged);
  mSink.Connect(mpRedSlider->ValueChanged, &nuiColorSelector::RGBSliderChanged);
  
  mpGreenSlider = new nuiSlider(nuiHorizontal, nuiRange(green, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 1, mpGreenSlider);
  mSink.Connect(mpGreenSlider->InteractiveValueChanged, &nuiColorSelector::RGBSliderChanged);
  mSink.Connect(mpGreenSlider->ValueChanged, &nuiColorSelector::RGBSliderChanged);
  
  mpBlueSlider = new nuiSlider(nuiHorizontal, nuiRange(blue, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 2, mpBlueSlider);
  mSink.Connect(mpBlueSlider->InteractiveValueChanged, &nuiColorSelector::RGBSliderChanged);
  mSink.Connect(mpBlueSlider->ValueChanged, &nuiColorSelector::RGBSliderChanged);
  
  mpRGBAlphaSlider = new nuiSlider(nuiHorizontal, nuiRange(alpha, 0.f, 1.f));
  mpRgbGrid->SetCell(1, 3, mpRGBAlphaSlider);
  mSink.Connect(mpRGBAlphaSlider->InteractiveValueChanged, &nuiColorSelector::RGBSliderChanged);
  mSink.Connect(mpRGBAlphaSlider->ValueChanged, &nuiColorSelector::RGBSliderChanged);
  
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
  
  mpRGBAlphaSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(alpha, mpRGBAlphaSliderLabel);
  mpRgbGrid->SetCell(2, 3, mpRGBAlphaSliderLabel, nuiRight);
  
  
  return mpRgbGrid;
}


void nuiColorSelector::FormatColor(nuiSize color, nuiLabel* pLabel, float maxval, bool integer)
{
  nglString str;
  str.Add(maxval * color, integer?0:1);
  pLabel->SetText(str);
}


nuiWidget* nuiColorSelector::Tab_HSV()
{
  float h, s, v;
  mCurrentColor.GetHSV(h, s, v);
  nuiSize alpha = mCurrentColor.Alpha();
  
  mpHsvGrid = new nuiGrid(3, 4);
  mpHsvGrid->SetBorder(0,0,10,10);
  
  nuiPane* pPane = new nuiPane(nuiColor(1.f, 1.f, 1.f, 1.f), nuiColor(0.f, 0.f, 0.f, 1.f), eStrokeShape);
  nuiLabel* pLabel = new nuiLabel(_T("HSV"));
  pLabel->SetBorder(2.f, 1.f);
  pPane->AddChild(pLabel);
  
  mpHsvGrid->SetCell(0, 0, new nuiLabel(_T("h:")));
  mpHsvGrid->SetCell(0, 1, new nuiLabel(_T("s:")));
  mpHsvGrid->SetCell(0, 2, new nuiLabel(_T("v:")));
  mpHsvGrid->SetCell(0, 3, new nuiLabel(_T("a:")));
  
  mpHueSlider = new nuiSlider(nuiHorizontal, nuiRange(h, 0.f, 1.f));
  mpHsvGrid->SetCell(1, 0, mpHueSlider);
  mSink.Connect(mpHueSlider->InteractiveValueChanged, &nuiColorSelector::HSVSliderChanged);
  mSink.Connect(mpHueSlider->ValueChanged, &nuiColorSelector::HSVSliderChanged);
  
  mpSaturationSlider = new nuiSlider(nuiHorizontal, nuiRange(s, 0.f, 1.f));
  mpHsvGrid->SetCell(1, 1, mpSaturationSlider);
  mSink.Connect(mpSaturationSlider->InteractiveValueChanged, &nuiColorSelector::HSVSliderChanged);
  mSink.Connect(mpSaturationSlider->ValueChanged, &nuiColorSelector::HSVSliderChanged);
  
  mpValueSlider = new nuiSlider(nuiHorizontal, nuiRange(v, 0.f, 1.f));
  mpHsvGrid->SetCell(1, 2, mpValueSlider);
  mSink.Connect(mpValueSlider->InteractiveValueChanged, &nuiColorSelector::HSVSliderChanged);
  mSink.Connect(mpValueSlider->ValueChanged, &nuiColorSelector::HSVSliderChanged);
  //
  mpHSVAlphaSlider = new nuiSlider(nuiHorizontal, nuiRange(alpha, 0.f, 1.f));
  mpHsvGrid->SetCell(1, 3, mpHSVAlphaSlider);
  mSink.Connect(mpHSVAlphaSlider->InteractiveValueChanged, &nuiColorSelector::HSVSliderChanged);
  mSink.Connect(mpHSVAlphaSlider->ValueChanged, &nuiColorSelector::HSVSliderChanged);
  
  mpHsvGrid->SetColumnPixels(2, 25);
  
  mpHueSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(h, mpHueSliderLabel, 100, false);
  mpHsvGrid->SetCell(2, 0, mpHueSliderLabel, nuiRight);
  
  mpSaturationSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(s, mpSaturationSliderLabel, 100, false);
  mpHsvGrid->SetCell(2, 1, mpSaturationSliderLabel, nuiRight);
  
  mpValueSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(v, mpValueSliderLabel, 100, false);
  mpHsvGrid->SetCell(2, 2, mpValueSliderLabel, nuiRight);
  
  mpHSVAlphaSliderLabel = new nuiLabel(_T(""), nuiFont::GetFont(8));
  FormatColor(alpha, mpHSVAlphaSliderLabel, 100, false);
  mpHsvGrid->SetCell(2, 3, mpHSVAlphaSliderLabel, nuiRight);
  
  
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
  FormatColor(alpha, mpRGBAlphaSliderLabel);
  
  float h, s, v;
  h = mpHueSlider->GetRange().GetValue();
  s = mpSaturationSlider->GetRange().GetValue();
  v = mpValueSlider->GetRange().GetValue();
  FormatColor(h, mpHueSliderLabel, 100, false);
  FormatColor(s, mpSaturationSliderLabel, 100, false);
  FormatColor(v, mpValueSliderLabel, 100, false);
  FormatColor(alpha, mpHSVAlphaSliderLabel, 100, false);
  
  mpColorPane->Invalidate();
  
  uint tabIndex = mpTabView->GetSelectedTab();
  if (tabIndex != 0)
    Tab_RGB_Update();
  if (tabIndex != 1)
    Tab_HSV_Update();
  
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

void nuiColorSelector::SwatchSelected(const nuiEvent& rEvent)
{
  nuiPane* pPane = (nuiPane*) rEvent.mpUser;
  SetCurrentColor(pPane->GetFillColor());
  
  // send event
  SwatchColorChanged(); 
  rEvent.Cancel();
}

void nuiColorSelector::OnTabSelected(const nuiEvent& rEvent)
{
  uint tabIndex = mpTabView->GetSelectedTab();
  switch (tabIndex)
  {
    case 0: //RGB
      Tab_RGB_Update();
      break;
    case 1: //HSV
      Tab_HSV_Update();
      break;
    case 2: //Swatches
      break;
    default:
      break;
  }
  rEvent.Cancel();
}

void nuiColorSelector::Tab_RGB_Update()
{
  mpRedSlider->ValueChanged.Disable();
  mpRedSlider->InteractiveValueChanged.Disable();
  mpRedSlider->GetRange().SetValue(mCurrentColor.Red());
  mpRedSlider->InteractiveValueChanged.Enable();
  mpRedSlider->ValueChanged.Enable();
  
  mpGreenSlider->ValueChanged.Disable();
  mpGreenSlider->InteractiveValueChanged.Disable();
  mpGreenSlider->GetRange().SetValue(mCurrentColor.Green());
  mpGreenSlider->InteractiveValueChanged.Enable();
  mpGreenSlider->ValueChanged.Enable();
  
  mpBlueSlider->ValueChanged.Disable();
  mpBlueSlider->InteractiveValueChanged.Disable();
  mpBlueSlider->GetRange().SetValue(mCurrentColor.Blue());
  mpBlueSlider->InteractiveValueChanged.Enable();
  mpBlueSlider->ValueChanged.Enable();
  
  mpRGBAlphaSlider->ValueChanged.Disable();
  mpRGBAlphaSlider->InteractiveValueChanged.Disable();
  mpRGBAlphaSlider->GetRange().SetValue(mCurrentColor.Alpha());
  mpRGBAlphaSlider->InteractiveValueChanged.Enable();
  mpRGBAlphaSlider->ValueChanged.Enable();

}

void nuiColorSelector::Tab_HSV_Update()
{
  float h, s ,v;
  mCurrentColor.GetHSV(h, s, v);
  mpHueSlider->ValueChanged.Disable();
  mpHueSlider->InteractiveValueChanged.Disable();
  mpHueSlider->GetRange().SetValue(h);
  mpHueSlider->InteractiveValueChanged.Enable();
  mpHueSlider->ValueChanged.Enable();
  
  mpSaturationSlider->ValueChanged.Disable();
  mpSaturationSlider->InteractiveValueChanged.Disable();
  mpSaturationSlider->GetRange().SetValue(s);
  mpSaturationSlider->InteractiveValueChanged.Enable();
  mpSaturationSlider->ValueChanged.Enable();
  
  mpSaturationSlider->ValueChanged.Disable();
  mpSaturationSlider->InteractiveValueChanged.Disable();
  mpSaturationSlider->GetRange().SetValue(v);
  mpSaturationSlider->InteractiveValueChanged.Enable();
  mpSaturationSlider->ValueChanged.Enable();
  
  mpHSVAlphaSlider->ValueChanged.Disable();
  mpHSVAlphaSlider->InteractiveValueChanged.Disable();
  mpHSVAlphaSlider->GetRange().SetValue(mCurrentColor.Alpha());
  mpHSVAlphaSlider->InteractiveValueChanged.Enable();
  mpHSVAlphaSlider->ValueChanged.Enable();
}


void nuiColorSelector::RGBSliderChanged(const nuiEvent& rEvent)
{
  nuiSize red = (float)mpRedSlider->GetRange().GetValue();
  nuiSize green = (float)mpGreenSlider->GetRange().GetValue();
  nuiSize blue = (float)mpBlueSlider->GetRange().GetValue();
  nuiSize alpha = (float)mpRGBAlphaSlider->GetRange().GetValue();
  
  SetCurrentColor(nuiColor(red, green, blue, alpha));
  
  // send event
  RGBColorChanged();
  
  rEvent.Cancel();
}

void nuiColorSelector::HSVSliderChanged(const nuiEvent& rEvent)
{
  nuiSize h = (float)mpHueSlider->GetRange().GetValue();
  nuiSize s = (float)mpSaturationSlider->GetRange().GetValue();
  nuiSize v = (float)mpValueSlider->GetRange().GetValue();
  nuiSize alpha = (float)mpHSVAlphaSlider->GetRange().GetValue();
  
  nuiColor c;
  c.SetHSV(h, s, v, alpha);
  SetCurrentColor(c);
  
  // send event
  HSVColorChanged();
  
  rEvent.Cancel();
}

