/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWin_h__
#define __nuiWin_h__

#include "ngl.h"
#include "nuiEvent.h"
#include "nuiMainWindow.h"
#include "nuiFileSelector.h"
#include "nuiMatrixView.h"


class Gears;
class nuiEvent;
class nuiWindowManager;

class nuiWin : public nuiMainWindow
{
public:
  nuiWin(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~nuiWin();

  void OnCreation();

  virtual bool OnKeyDown(const nglKeyEvent& rEvent);

  bool Draw(nuiDrawContext* pContext);
  
  void OnClose();
  void OnPaint();
protected:

  bool CreateConsoleWindow(const nuiEvent& rEvent);
  bool CreateThreadInspectorTestWindow(const nuiEvent& rEvent);
  bool CreateCSSParserTest(const nuiEvent& rEvent);
  bool CreateBlurArea(const nuiEvent& rEvent);
  bool CreateSplineWindow(const nuiEvent& rEvent);
  bool CreateTreeViewWindow(const nuiEvent& rEvent);
  bool CreateSVGTigerWindow(const nuiEvent& rEvent);
  bool CreateAnimWindow(const nuiEvent& rEvent);
  bool CreateMessedUpWindow(const nuiEvent& rEvent);
  bool CreateTextWindow(const nuiEvent& rEvent);
  bool CreateText2Window(const nuiEvent& rEvent);
  bool CreateFileListWindow(const nuiEvent& rEvent);
  bool CreateOffscreenArea(const nuiEvent& rEvent);
  bool CreateColumnTreeViewWindow(const nuiEvent& rEvent);
  bool CreateScrolledImageWindow(const nuiEvent& rEvent);
  bool CreateScrolledImageWindow2(const nuiEvent& rEvent);
#ifdef USE_SWF
  bool CreateSWFTest1Window(const nuiEvent& rEvent);
#endif
  bool CreateShapeWindow(const nuiEvent& rEvent);
  bool TogglePartialRedraw(const nuiEvent& rEvent);
  bool CreateWrappedLabelWindow(const nuiEvent& rEvent);
  bool CreateWhizzAnimWindow(const nuiEvent& rEvent);
  bool CreateTextLayoutWindow(const nuiEvent& rEvent);
  bool CreateFontSubstitutionWindow(const nuiEvent& rEvent);
  bool CreateSignalsAndSlotsTestWindow(const nuiEvent& rEvent);
  bool CreateClippingTest(const nuiEvent& rEvent);
  bool CreateSurfaceTest(const nuiEvent& rEvent);
  bool CreateDragZone(const nuiEvent& rEvent);
  bool CreateEnableTestWindow(const nuiEvent& rEvent);
  bool CreateButtonDecorationWindow(const nuiEvent& rEvent);
  bool CreateVolumesTestWindow(const nuiEvent& rEvent);
  bool CreateHTTPWindow(const nuiEvent& rEvent);
  bool SendHTTPRequest(const nuiEvent& rEvent);
  bool CreateHTMLWindow(const nuiEvent& rEvent);
  bool CreateImageResizeWindow(const nuiEvent& rEvent);
  bool CreateCoverFlowWindow(const nuiEvent& rEvent);
  bool CreateUnicodeTextWindow(const nuiEvent& rEvent);

  bool CreateNewWindow(const nuiEvent& rEvent);
  bool ShowMenu(const nuiEvent& rEvent);
  bool ItemSelected(const nuiEvent& rEvent);
  bool UserDraw(const nuiEvent& rEvent);
  bool ToggleAnimation(const nuiEvent& rEvent);

  bool SaveXMLDescription(const nuiEvent& rEvent);
  bool OutputSomething(const nuiEvent& rEvent);
  bool CreateFromGladeXML(const nuiEvent& rEvent);
  bool CreateFromXML(const nuiEvent& rEvent);
  bool HideText(const nuiEvent& rEvent);
  bool LogText(const nuiEvent& rEvent);
  bool ClearTable(const nuiEvent& rEvent);
  bool FileSelect(const nuiEvent& rEvent);
  bool FileSelectDone(const nuiEvent& rEvent);

  bool UpdateInspector(const nuiEvent& rEvent);
  bool ParentInspector(const nuiEvent& rEvent);
  bool SelectInspector(const nuiEvent& rEvent);

  bool DrawBlur(const nuiEvent& rEvent);

  bool SoftwarePainterWindow(const nuiEvent& rEvent);
  
  bool SpinerLabelWindow(const nuiEvent& rEvent);
  bool VBoxAndHBoxWindow(const nuiEvent& rEvent);
  bool MatrixViewWindow(const nuiEvent& rEvent);
  bool MatrixViewTest(const nuiEvent& rEvent);
  void GetMatrixViewItemDesc(nuiMatrixViewItemDesc& rDesc);
  
  bool DumpStats(const nuiEvent& rEvent);
  bool OnTextWrapChanged(const nuiEvent& rEvent);
  bool OnTextSizeSliderChanged(const nuiEvent& rEvent);
  bool CreateTitledPaneWindow(const nuiEvent& rEvent);

  bool CreateSliderWindow(const nuiEvent& rEvent);
  bool OnSliderChanged(const nuiEvent& rEvent);
    
  bool SaveFontDB(const nuiEvent& rEvent);
  bool LoadFontDB(const nuiEvent& rEvent);
  
  bool CreateFrameWindow(const nuiEvent& rEvent);
  bool CreateFrameWindow2(const nuiEvent& rEvent);
  bool CreateFrameWindow3(const nuiEvent& rEvent);

  bool CreateAudioWindow(const nuiEvent& rEvent);

  bool CreateAudioFifoWindow(const nuiEvent& rEvent);

  bool CreateResourceWindow(const nuiEvent& rEvent);

  bool CreateEllispsisTestWindow(const nuiEvent& rEvent);
  
  bool CreateMessageBox(const nuiEvent& rEvent);
  bool CreateWidgetTexture(const nuiEvent& rEvent);
  
  bool AudioDeviceSelected(const nuiEvent& rEvent);
  bool AudioComboTrashed(const nuiEvent& rEvent);

  // The window manager:
  nuiWindowManager* mpManager;

  Gears* mpGears;
  bool mAnimate;
  nuiEventSink<nuiWin> mWinSink;
  nuiSlotsSink mSink;
  uint mFrameCnt;
  nglTime mLastFPSMeasure;
  nglString mFPSText;
  bool mShowFPS;
  
  nuiEditText* mpURLText;
  nuiLabel* mpHTTPResponseLabel;

  static uint mRefs;
  
};

#endif
