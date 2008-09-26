/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiColorSelector.h"

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
  AddChild(mpMainBox);
  
  mpColorPane = new nuiPane(mCurrentColor, nuiColor(0.f, 0.f, 0.f, 1.f));
  mpColorPane->SetBorder(2.f, 2.f);
  mpColorPane->SetUserSize(20.f, 20.f);
  mpMainBox->AddCell(mpColorPane, nuiLeft);
  
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
  
  mpRgbGrid = new nuiGrid(2, 4);
    
  nuiPane* pPane = new nuiPane(nuiColor(1.f, 1.f, 1.f, 1.f), nuiColor(0.f, 0.f, 0.f, 1.f), eStrokeShape);
  nuiLabel* pLabel = new nuiLabel(_T("RGB"));
  pLabel->SetBorder(2.f, 1.f);
  pPane->AddChild(pLabel);
  
  mpRgbGrid->SetCell(0, 0, new nuiLabel(_T("Red:")));
  mpRgbGrid->SetCell(0, 1, new nuiLabel(_T("Green:")));
  mpRgbGrid->SetCell(0, 2, new nuiLabel(_T("Blue:")));
  mpRgbGrid->SetCell(0, 3, new nuiLabel(_T("Alpha:")));
  
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
  
  return mpRgbGrid;
}


nuiWidget* nuiColorSelector::Tab_HSV()
{
  mpHsvGrid = new nuiGrid(2, 4);
  
  nuiPane* pPane = new nuiPane(nuiColor(1.f, 1.f, 1.f, 1.f), nuiColor(0.f, 0.f, 0.f, 1.f), eStrokeShape);
  nuiLabel* pLabel = new nuiLabel(_T("HSV"));
  pLabel->SetBorder(2.f, 1.f);
  pPane->AddChild(pLabel);

  return mpHsvGrid;
}


nuiWidget* nuiColorSelector::Tab_Swatches()
{
  nuiPane* pPane = new nuiPane(nuiColor(1.f, 1.f, 1.f, 1.f), nuiColor(0.f, 0.f, 0.f, 1.f), eStrokeShape);
  nuiLabel* pLabel = new nuiLabel(_T("Swatches"));
  pLabel->SetBorder(2.f, 1.f);
  pPane->AddChild(pLabel);
  
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
  for (int j=0; j<5; j++)
  {
    mpSwatchGrid->SetColumnPixels(j, 16.f);
  }
  
  std::vector<nuiColor>::const_iterator end = mColorList.end();
  uint i = 0;
  for (std::vector<nuiColor>::const_iterator it = mColorList.begin(); it != end; ++it, i++)
  {
    mpSwatchGrid->AddRows(i, 1);
    mpSwatchGrid->SetRowPixels(i, 16.f);
    nuiPane* pSwatchPane;
    pSwatchPane = new nuiPane(*it, nuiColor(0.f, 0.f, 0.f, 1.f));
    nuiButton* pButton = new nuiButton();
    pButton->SetDrawSelf(false);
    pButton->SetBorder(1.f,1.f);
    pButton->AddChild(pSwatchPane);
    mSink.Connect(pButton->Clicked, &nuiColorSelector::SwatchSelected, pSwatchPane);
    mpSwatchGrid->SetCell(i%5, i/5, pButton);
  }
}

const nuiColor& nuiColorSelector::GetCurrentColor() const
{
  return mCurrentColor;
}

void nuiColorSelector::SetCurrentColor(const nuiColor& rColor)
{
  mCurrentColor = rColor;
  mpColorPane->SetFillColor(rColor);
  mpColorPane->Invalidate();
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
  SetCurrentColor(pPane->GetColor(eShapeFill));
  OnTabSelected(nuiEvent());
  
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
//  mpRedSlider->ValueChanged.Disable();
//  mpRedSlider->GetRange().SetValue(mCurrentColor.Red());
//  mpRedSlider->ValueChanged.Enable();
//  
//  mpGreenSlider->ValueChanged.Disable();
//  mpGreenSlider->GetRange().SetValue(mCurrentColor.Green());
//  mpGreenSlider->ValueChanged.Enable();
//  
//  mpBlueSlider->ValueChanged.Disable();
//  mpBlueSlider->GetRange().SetValue(mCurrentColor.Blue());
//  mpBlueSlider->ValueChanged.Enable();
//  
//  mpRgbAlphaSlider->ValueChanged.Disable();
//  mpRgbAlphaSlider->GetRange().SetValue(mCurrentColor.Alpha());
//  mpRgbAlphaSlider->ValueChanged.Enable();

}

bool nuiColorSelector::RgbSliderChanged(const nuiEvent& rEvent)
{
  SetCurrentColor(nuiColor((float)mpRedSlider->GetRange().GetValue(), (float)mpGreenSlider->GetRange().GetValue(), (float)mpBlueSlider->GetRange().GetValue(), (float)mpRgbAlphaSlider->GetRange().GetValue()));
  return true;
}

