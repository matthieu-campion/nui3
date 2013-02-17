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
#include "nuiToken.h"
#include "nuiObject.h"
#include "nuiTimer.h"
#include "nuiXML.h"
#include "nuiTree.h"
#include "nuiAttribute.h"
#include "nuiAnimation.h"
#include "nuiTranslator.h"
#include "nuiAudioDb.h"
#include "nuiAudioEngine.h"
#include "nuiAudioFifo.h"
#include "nuiSoundManager.h"
#include "nuiSound.h"
#include "nuiVoice.h"
#include "nuiMemorySound.h"
#include "nuiMemoryVoice.h"
#include "nuiSynthSound.h"
#include "nuiSynthVoice.h"
#include "nuiFileVoice.h"
#include "nuiFileSound.h"
#include "nuiWaveReader.h"
#include "nuiAiffReader.h"
#include "nuiAudioDecoder.h"
#include "nuiWaveWriter.h"
#include "nuiSampleWriter.h"
#include "nuiSampleReader.h"
#include "nuiSampleInfo.h"
#include "nuiSample.h"
#include "nuiChunkSampleReader.h"
#include "nuiAudioConvert.h"
#include "nuiAiffWriter.h"

#include "nuiStringTemplate.h"
#include "nuiFormater.h"

#include "nuiCommand.h"
#include "nuiCommandContainer.h"
#include "nuiCommandManager.h"

#include "nuiRect.h"
#include "nuiColor.h"


#include "nglMath.h"
#include "nuiTask.h"
#include "nuiAttributeAnimation.h"
#include "nuiLocale.h"
#include "nuiMessageQueue.h"
#include "nuiNotification.h"

#include "nuiMemoryPool.h"

#include "nuiSerializeContext.h"
#include "nuiArchive.h"

#include "nuiURL.h"
#include "nuiNetworkHost.h"
#include "nuiSocket.h"
#include "nuiTCPClient.h"
#include "nuiTCPServer.h"
#include "nuiMimeMultiPart.h"
#include "nuiHTTP.h"
#include "nuiHTTPServer.h"

#include "nuiStopWatch.h"

#ifndef _MINUI3_
  #include "nuiApplication.h"

  #include "nuiContour.h"
  #include "nuiArc.h"
  #include "nuiRectPath.h"
  #include "nuiSpline.h"
  #include "nuiPolyLine.h"
  #include "nuiOutliner.h"
  #include "nuiRenderArray.h"
  #include "nuiPath.h"
  #include "nuiPathGenerator.h"
  #include "nuiPathOptimizer.h"
  #include "nuiPoint.h"
  #include "nuiPolyLine.h"
  #include "nuiShape.h"
  #include "nuiTessellator.h"
  #include "nuiSpline.h"


  #include "nuiRenderState.h"
  #include "nuiDrawContext.h"
  #include "nuiSurface.h"
  #include "nuiShader.h"
  #include "nuiPainter.h"
  #include "nuiGLPainter.h"
  #include "nuiGL2Painter.h"
  #include "nuiMetaPainter.h"
  #include "nuiSoftwarePainter.h"




  #include "nuiMatrixNode.h"
  #include "nuiWidget.h"
  #include "nuiContainer.h"
  #include "nuiSimpleContainer.h"
  #include "nuiTopLevel.h"
  #include "nuiGestureRecognizer.h"
  #include "nuiLayout.h"
  #include "nuiImageAnimation.h"
  #include "nuiImageSequence.h"

  #include "nuiBox.h"
  #include "nuiHBox.h"
  #include "nuiVBox.h"
  #include "nuiBuilder.h"
  #include "nuiRange.h"

  #include "nuiSpriteView.h"

  #include "nuiButton.h"
  #include "nuiToggleButton.h"
  #include "nuiRadioButton.h"
  #include "nuiRadioButtonGroup.h"

  #include "nuiComboBox.h"
  #include "nuiEditLine.h"
  #include "nuiEditText.h"
  #include "nuiFileList.h"
  #include "nuiFileSelector.h"
  #include "nuiFixed.h"
  #include "nuiFlags.h"
  #include "nuiGladeLoader.h"
  #include "nuiImage.h"
  #include "nuiHugeImage.h"
  #include "nuiLabel.h"
  #include "nuiList.h"
  #include "nuiMainWindow.h"
  #include "nuiMainMenu.h"
  #include "nuiPopupMenu.h"
  #include "nuiMouseEvent.h"
  #include "nuiBorder.h"
  #include "nuiScrollBar.h"
  #include "nuiSlider.h"
  #include "nuiSplitter.h"
  #include "nuiText.h"
  #include "nuiTexture.h"
  #include "nuiTheme.h"
  #include "nuiUserArea.h"
  #include "nuiWindow.h"
  #include "nuiPane.h"

  #include "nuiStateDummy.h"
  #include "nuiHoverDummy.h"
  #include "nuiShape.h"
  #include "nuiPoint.h"
  #include "nuiGradient.h"
  #include "nuiFont.h"
  #include "nuiFontBase.h"
  #include "nuiFontInspector.h"
  #include "nuiTextLayout.h"
  #include "nuiFontManager.h"
  #include "nuiPanose.h"

  #include "nuiImageDropZone.h"
  #include "nuiShapeView.h"


  #include "nuiTreeView.h"
  #include "nuiTabView.h"
  #include "nuiKnob.h"
  #include "nuiWindowManager.h"
  #include "nuiAnimView.h"
  #include "nuiScrollView.h"
  #include "nuiFileTree.h"
  #include "nuiZoomView.h"
  #include "nuiScrollBox.h"
  #include "nuiTextureHelpers.h"
  #include "nuiWidgetAnimation.h"
  #include "nuiApplication.h"
  #include "nuiZoomBar.h"

  #include "nuiNavigationBar.h"
  #include "nuiNavigationController.h"
  #include "nuiNavigationButton.h"
  #include "nuiViewController.h"
  #include "nuiAttributeEditor.h"

  #include "nuiPercentAttributeEditor.h"
  #include "nuiPointAttributeEditor.h"
  #include "nuiColorAttributeEditor.h"
  #include "nuiRectAttributeEditor.h"
  #include "nuiStringAttributeEditor.h"
  #include "nuiBooleanAttributeEditor.h"
  #include "nuiSizeAttributeEditor.h"
  #include "nuiBorderAttributeEditor.h"
  #include "nuiRangeAttributeEditor.h"
  #include "nuiRangeKnobAttributeEditor.h"
  #include "nuiGenericAttributeEditor.h"
  #include "nuiClampedValueAttributeEditor.h"
  #include "nuiPopupValueAttributeEditor.h"
  #include "nuiComboAttributeEditor.h"
  #include "nuiPathAttributeEditor.h"
  #include "nuiLabelRenamer.h"

  #include "nuiPopupView.h"

  #include "nuiCSS.h"
  #include "nuiBindingManager.h"
  #include "nuiTypeTraits.h"

  #include "nuiTabBar.h"
  #include "nuiColumnTreeView.h"
  #include "nuiFolderPane.h"
  #include "nuiSVGView.h"
  #include "nuiSVGShape.h"
  #include "nuiProgressBar.h"
  #include "nuiBackgroundPane.h"
  #include "nuiFlowView.h"
  #include "nuiMatrixView.h"
  #include "nuiTitledPane.h"
  #include "nuiCoverFlow.h"
  #include "nuiFontBrowser.h"
  #include "nuiFrameView.h"
  #include "nuiHotKey.h"
  #include "nuiHotKeyEditor.h"
  #include "nuiRectView.h"
  #include "nuiRSS.h"
  #include "nuiRSSView.h"
  #include "nuiHyperLink.h"
  #include "nuiHTML.h"
  #include "nuiHTMLView.h"
  #include "nuiScrollingLabel.h"
  #include "nuiSpinnerLabel.h"


  #include "nuiScriptEngine.h"
  #include "nuiSpiderMonkey.h"

  #include "nuiDialog.h"
  #include "nuiMessageBox.h"
  #include "nuiDialogSelectDirectory.h"
  #include "nuiDialogSelectFile.h"


  #include "nuiDecoration.h"
  #include "nuiBorderDecoration.h"
  #include "nuiCheckerboardDecoration.h"
  #include "nuiNavigationViewDecoration.h"
  #include "nuiColorDecoration.h"
  #include "nuiDefaultDecoration.h"
  #include "nuiGradientDecoration.h"
  #include "nuiMetaDecoration.h"
  #include "nuiImageDecoration.h"
  #include "nuiStateDecoration.h"
  #include "nuiTreeHandleDecoration.h"
  #include "nuiFrame.h"

  #include "nuiIntrospector.h"
  #include "nuiSeparator.h"
  #include "nuiWidgetInspector.h"
  #include "nuiThreadInspector.h"
  #include "nuiFontInspector.h"
  #include "nuiDecorationInspector.h"
  #include "nuiTextureInspector.h"
  #include "nuiObjectInspector.h"

  #include "nuiMidi.h"
  #include "nuiVideoDecoder.h"
  #include "nuiAVIwriter.h"

#endif


#endif // __nui_all_h__
