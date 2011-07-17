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

#include "nuiSignalsSlots.h"
#include "nuiApplication.h"
#include "nuiRefCount.h"
#include "nuiNonCopyable.h"
#include "nuiEvent.h"
#include "nuiObject.h"
#include "nuiBuilder.h"
#include "nuiRange.h"
#include "nuiRect.h"
#include "nuiButton.h"
#include "nuiToggleButton.h"
#include "nuiRadioButton.h"
#include "nuiColor.h"
#include "nuiContainer.h"
#include "nuiDrawContext.h"
#include "nuiEditLine.h"
#include "nuiEditText.h"
#include "nuiEventRegistry.h"
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
#include "nuiTimer.h"
#include "nuiUserArea.h"
#include "nuiWidget.h"
#include "nuiWindow.h"
#include "nuiXML.h"
#include "nuiStateDummy.h"
#include "nuiHoverDummy.h"
#include "nuiShape.h"
#include "nuiPoint.h"
#include "nuiGradient.h"
#include "nuiFont.h"
#include "nuiFontBase.h"
#include "nuiTextLayout.h"
#include "nuiTopLevel.h"
#include "nuiTree.h"
#include "nuiTreeView.h"
#include "nuiWindowManager.h"
//#include "nuiAnimView.h"
#include "nuiScrollView.h"
#include "nuiZoomView.h"
#include "nuiTextureHelpers.h"
#include "nuiWidgetAnimation.h"
#include "nuiApplication.h"
#include "nuiAttribute.h"
#include "nuiAttributeEditor.h"
#include "nuiTranslator.h"
#include "nuiAudioDb.h"
#include "nuiAudioEngine.h"
#include "nuiSoundManager.h"
#include "nuiSound.h"
#include "nuiVoice.h"

#endif // __nui_all_h__
