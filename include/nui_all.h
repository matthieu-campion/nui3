/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


/*!
\file  nui_all.h
\brief All nui includes in one header file.

This file includes all nui headers to ease the creation of precompiled headers outside of the ngl project in VC. Just include it in your
precompiled header central file and everything should be rolling. (lookup precompiled headers in the VC doc to learn how to use them).
*/

#ifndef __nui_all_h__
#define __nui_all_h__

//#define NUI_USE_FLOATS
//#define NUI_USE_DOUBLES

#if !defined NUI_USE_DOUBLES && !defined NUI_USE_FLOATS
  #define NUI_USE_FLOATS
#endif

#ifdef NUI_USE_FLOATS
typedef float nuiSize;
typedef double nuiAltSize;
#else
typedef float nuiAltSize;
typedef double nuiSize;
#endif



#include "nglLog.h"
#include "nuiSignalsSlots.h"
#include "nuiRefCount.h"
#include "nuiNonCopyable.h"
#include "nuiEvent.h"
#include "nuiEventRegistry.h"
#include "nuiObject.h"
#include "nuiTimer.h"
#include "nuiXML.h"
#include "nuiTree.h"
#include "nuiAttribute.h"
#include "nuiAnimation.h"
#include "nuiTranslator.h"
#include "nuiAudioDb.h"
#include "nuiAudioEngine.h"
#include "nuiSoundManager.h"
#include "nuiSound.h"
#include "nuiVoice.h"
#include "nuiRect.h"
#include "nuiColor.h"


#include "nuiNotification.h"

#ifndef _MINUI3_
  #include "nuiApplication.h"

  #include "nuiBuilder.h"
  #include "nuiRange.h"
  #include "nuiButton.h"
  #include "nuiToggleButton.h"
  #include "nuiRadioButton.h"
  #include "nuiContainer.h"
  #include "nuiDrawContext.h"
  #include "nuiEditLine.h"
  #include "nuiEditText.h"
  #include "nuiFileList.h"
  #include "nuiFixed.h"
  #include "nuiFlags.h"
  #include "nuiGladeLoader.h"
  #include "nuiImage.h"
  #include "nuiLabel.h"
  #include "nuiList.h"
  #include "nuiMainWindow.h"
  #include "nuiPopupMenu.h"
  #include "nuiMouseEvent.h"
  #include "nuiBorder.h"
  #include "nuiScrollBar.h"
  #include "nuiSlider.h"
  #include "nuiSpline.h"
  #include "nuiSplitter.h"
  #include "nuiText.h"
  #include "nuiTexture.h"
  #include "nuiTheme.h"
  #include "nuiUserArea.h"
  #include "nuiWidget.h"
  #include "nuiWindow.h"

  #include "nuiStateDummy.h"
  #include "nuiHoverDummy.h"
  #include "nuiShape.h"
  #include "nuiPoint.h"
  #include "nuiGradient.h"
  #include "nuiFont.h"
  #include "nuiFontBase.h"
  #include "nuiTextLayout.h"
  #include "nuiTopLevel.h"

  #include "nuiTreeView.h"
  #include "nuiWindowManager.h"
  //#include "nuiAnimView.h"
  #include "nuiScrollView.h"
  #include "nuiZoomView.h"
  #include "nuiTextureHelpers.h"
  #include "nuiWidgetAnimation.h"
  #include "nuiApplication.h"

  #include "nuiNavigationBar.h"
  #include "nuiNavigationController.h"
  #include "nuiNavigationButton.h"
  #include "nuiViewController.h"
  #include "nuiAttributeEditor.h"
#endif


#endif // __nui_all_h__
