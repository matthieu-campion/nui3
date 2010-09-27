/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"

class nuiColumnTreeView;
class nuiFontDesc;

class nuiFontBrowser : public nuiSimpleContainer
{
public:
  nuiFontBrowser();
  virtual ~nuiFontBrowser();

  nuiFontDesc* GetCurrentFontDesc() const;
  uint32 GetFontSize() const;
  void SetFont(const nglString& rFontName, const nglString& rFontStyle, uint32 FontSize);
  
  nuiSimpleEventSource<0> FontChanged;
private:
  nuiEventSink<nuiFontBrowser> mSink;
  void OnFontChanged(const nuiEvent& rEvent);
  void OnStyleChanged(const nuiEvent& rEvent);

  nuiList* mpFontList;
  nuiList* mpStyleList;
  nuiLabel* mpPreviewLabel;
  nuiSlider* mpSizeSlider;
  nuiLabel* mpSizeLabel;
  
  typedef std::map<nglString, nuiFontDesc*> FontStyleMap;
  typedef std::map<nglString, FontStyleMap> FontMap;
  
  FontMap mFonts;
  uint32 mFontSize;
  
  nuiFontDesc* mpFontDesc;
};

