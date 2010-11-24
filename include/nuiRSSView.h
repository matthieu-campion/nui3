/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHyperLink.h"
#include "nuiSimpleContainer.h"
#include "nuiRSS.h"

class nuiVBox;

class nuiRSSView : public nuiSimpleContainer
{
public:
  nuiRSSView(const nglString& rURL = nglString::Empty, int32 SecondsBetweenUpdates = (30*60), nglIStream* pOriginalStream = NULL, bool ForceNoHTML = false);
  virtual ~nuiRSSView();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  
  void SetURL(const nglString& rURL);
  const nglString&  GetURL() const;
  
  void SetItemWidget(const nglString& rWidget);
  const nglString&  GetItemWidget() const;
  
  void SetMaxItems(int32 maxshown);
  int32  GetMaxItems() const;
  
  void ForceUpdate();
  
  void _SetFont(const nglString& rFontSymbol);
  const nglString& _GetFont() const;
  const nuiColor& GetTextColor() const;
  void SetTextColor(const nuiColor& Color);
  
  void SetExpand(nuiExpandMode expand);
  nuiExpandMode GetExpand() const;
  
  void SetRefreshRate(int32 SecondsBetweenUpdates);
  int32 GetRefreshRate() const;
protected:
  
  void Update(const nuiEvent& rEvent);
  
  nuiRSS* mpRSS;
  nuiVBox* mpBox;
  bool mForceNoHTML;
  
  nglString mFont;
  nuiColor mTextColor;

  nglString mItemWidget;
  
  int32 mMaxItems;
  
  nuiExpandMode mExpand;
private:
  
  void InitAttributes();
  nuiEventSink<nuiRSSView> mViewSink;

};