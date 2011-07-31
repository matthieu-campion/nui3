/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiColorSelector_h__
#define __nuiColorSelector_h__

#include "nuiContainer.h"
#include "nuiTabView.h"
#include "nuiGrid.h"
#include "nuiVBox.h"
#include "nuiPane.h"
#include "nuiColorDecoration.h"

enum ColorSelectorMode
{
  eModeRGB = 0,
  eModeHSV,
  eModeSwatch
};

// can apply decoration on "nuiColorSelector::SwatchGrid"

class nuiColorSelector : public nuiSimpleContainer
{
public:
  nuiColorSelector(const nuiColor& rInitialColor = nuiColor(1.f, 1.f, 1.f, 1.f), const std::vector<nuiColor>& rInitialColorList = std::vector<nuiColor>(), ColorSelectorMode mode = eModeRGB);
  virtual ~nuiColorSelector();

  void SetColorList(const std::vector<nuiColor>& rColorList);
  void SetNumColors(uint NumColors);
  
  const nuiColor& GetCurrentColor() const;
  void SetCurrentColor(const nuiColor& rColor);


  nuiSimpleEventSource<0> ColorChanged; // sent in all cases
  
  nuiSimpleEventSource<0> RGBColorChanged; // sent if the color has been changed with the RGB sliders
  nuiSimpleEventSource<0> HSVColorChanged; // sent if the color has been changed with the RGB sliders
  nuiSimpleEventSource<0> SwatchColorChanged; // sent if the color has been changed through the Swatches selector
  
  
private:
  
  nuiWidget* Tab_RGB();
  nuiWidget* Tab_HSV();
  nuiWidget* Tab_Swatches();
  
  void OnTabSelected(const nuiEvent& rEvent);
  
  void Tab_RGB_Update();
  void Tab_HSV_Update();
  void FormatColor(nuiSize color, nuiLabel* pLabel, float maxval = 255.0f, bool integer = true);
  
  
  void SwatchSelected(const nuiEvent& rEvent);
  void RGBSliderChanged(const nuiEvent& rEvent);
  void HSVSliderChanged(const nuiEvent& rEvent);
  
  void MakeSwatchGrid();
  
  nuiVBox* mpMainBox;
  nuiSimpleContainer* mpColorPane;
  nuiColorDecoration* mpColorPaneDeco;
  
  nuiTabView* mpTabView;
  nuiGrid* mpRgbGrid;
  nuiGrid* mpHsvGrid;
  nuiGrid* mpSwatchGrid;
  
  // Control widgets
  nuiSlider *mpRedSlider, *mpGreenSlider, *mpBlueSlider, *mpRGBAlphaSlider;
  nuiSlider *mpHueSlider, *mpSaturationSlider, *mpValueSlider, *mpHSVAlphaSlider;
  nuiLabel *mpRedSliderLabel, *mpGreenSliderLabel, *mpBlueSliderLabel, *mpRGBAlphaSliderLabel;
  nuiLabel *mpHueSliderLabel, *mpSaturationSliderLabel, *mpValueSliderLabel, *mpHSVAlphaSliderLabel;
  
  nuiColor mCurrentColor;
  std::vector<nuiColor> mColorList;
  uint mNumColors;
  
  nuiEventSink<nuiColorSelector> mSink;
};

#endif
