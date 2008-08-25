/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiGearWin_h__
#define __nuiGearWin_h__

#include "ngl.h"
#include "nglTime.h"

#include "nuiEvent.h"
#include "nuiMainWindow.h"
#include "nuiWindow.h"
#include "nuiLabel.h"
#include "nuiUserArea.h"
#include "nuiScrollBar.h"
#include "nuiSlider.h"
#include "nuiImage.h"

#include "Gears.h"


class nuiGearWin : public nuiWindow 
{
public:
  nuiGearWin(const nuiRect& rRect);
  ~nuiGearWin();

  bool ShowMenu(const nuiEvent& rEvent);
  bool ItemSelected(const nuiEvent& rEvent);
  bool UserDraw(const nuiEvent& rEvent);
  bool ToggleAnimation(const nuiEvent& rEvent);
  bool UpdateFPS(const nuiEvent& rEvent);

protected:
  nuiLabel*    mpFPSLabel;
  nuiUserArea* mpUserArea;
  nuiSlider* mpAlphaBar;

  Gears*  mpGears;
  nglTime mLastDraw;
  nglTime mLastFPSMeasure;
  int     mFrameCnt;
  bool    mAnimate;
  nuiEventSink<nuiGearWin> mGearWinSink;
  nuiImage* mpImage;
  double mFPS;
  nuiTimer* mpTimer;
};

#endif // __nuiGearWin_h__
