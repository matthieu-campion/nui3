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

#include <vector>

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
  bool Load(const nuiXMLNode* pNode);
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;
  
  void SetColorList(const std::vector<nuiColor>& rColorList);
  void SetNumColors(uint NumColors);
  
  const nuiColor& GetCurrentColor() const;
  void SetCurrentColor(const nuiColor& rColor);
  
  nuiSimpleEventSource<0> ColorChanged;
  
private:
  bool RefreshColors(const nuiEvent& rEvent);
  bool SwatchSelected(const nuiEvent& rEvent);
  bool RgbSliderChanged(const nuiEvent& rEvent);
  
  void MakeSwatchGrid();
  
  nuiVBox* mpMainBox;
  nuiPane* mpColorPane;
  nuiTabView* mpTabView;
  nuiGrid* mpRgbGrid;
  nuiGrid* mpHsvGrid;
  nuiGrid* mpSwatchGrid;
  
  // Control widgets
  nuiSlider *mpRedSlider, *mpGreenSlider, *mpBlueSlider, *mpRgbAlphaSlider;
  nuiSlider *mpHueSlider, *mpSaturationSlider, *mpValueSlider, *mpHsvAlphaSlider;
  
  nuiColor mCurrentColor;
  std::vector<nuiColor> mColorList;
  uint mNumColors;
  
  nuiEventSink<nuiColorSelector> mSink;
};

#endif
