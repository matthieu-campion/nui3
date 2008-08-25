/*
 *  nuiZoomBar.h
 *  nui3
 *
 *  Created by campion matthieu on 10/5/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "nuiScrollBar.h"


class nuiZoomBar : public nuiScrollBar
{
public:
  nuiZoomBar(nuiOrientation Orientation, const nuiRange& rRange = nuiRange(), float SideSize = 10.0, float MiddleSize = 10.0, nuiSize PageSizeMin = 0.0, nuiWidgetPtr pThumb = NULL);
  virtual ~nuiZoomBar();
  
  nuiSize GetPageSizeMin() const;
  void SetPageSizeMin(nuiSize min);
  
  bool IsUnZoomAfterBoundEnabled();
  void EnableUnzoomAfterBound(bool set);
  
  //Received Events:
  bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved(nuiSize X, nuiSize Y);
  
  void UpdateUpBound(nuiSize move);
  void UpdateDownBound(nuiSize move);
  
  void ChangeZoomCursor(nuiSize coord, nuiSize DownLimit, nuiSize UpLimit);
  
  bool Draw(nuiDrawContext* pContext);
    
private:
  nuiSize mPageSizeMin;
  bool mUnzoomAfterBound;
};
