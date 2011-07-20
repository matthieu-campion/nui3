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

  void CreateConsoleWindow(const nuiEvent& rEvent);
  void CreateThreadInspectorTestWindow(const nuiEvent& rEvent);
  void CreateCSSParserTest(const nuiEvent& rEvent);
  void CreateBlurArea(const nuiEvent& rEvent);
  void CreateTreeViewWindow(const nuiEvent& rEvent);
  void CreateSVGTigerWindow(const nuiEvent& rEvent);
  void CreateAnimWindow(const nuiEvent& rEvent);
  void CreateMessedUpWindow(const nuiEvent& rEvent);
  void CreateTextWindow(const nuiEvent& rEvent);
  void CreateText2Window(const nuiEvent& rEvent);
  void CreateFileListWindow(const nuiEvent& rEvent);
  void CreateOffscreenArea(const nuiEvent& rEvent);
  void CreateColumnTreeViewWindow(const nuiEvent& rEvent);
  void CreateScrolledImageWindow(const nuiEvent& rEvent);
  void CreateScrolledImageWindow2(const nuiEvent& rEvent);
#ifdef USE_SWF
  void CreateSWFTest1Window(const nuiEvent& rEvent);
#endif
  void CreateShapeWindow(const nuiEvent& rEvent);
  void TogglePartialRedraw(const nuiEvent& rEvent);
  void CreateWrappedLabelWindow(const nuiEvent& rEvent);
  void CreateWhizzAnimWindow(const nuiEvent& rEvent);
  void CreateTextLayoutWindow(const nuiEvent& rEvent);
  void CreateFontSubstitutionWindow(const nuiEvent& rEvent);
  void CreateSignalsAndSlotsTestWindow(const nuiEvent& rEvent);
  void CreateClippingTest(const nuiEvent& rEvent);
  void CreateSurfaceTest(const nuiEvent& rEvent);
  void CreateDragZone(const nuiEvent& rEvent);
  void CreateEnableTestWindow(const nuiEvent& rEvent);
  void CreateButtonDecorationWindow(const nuiEvent& rEvent);
  void CreateVolumesTestWindow(const nuiEvent& rEvent);
  void CreateHTTPWindow(const nuiEvent& rEvent);
  void SendHTTPRequest(const nuiEvent& rEvent);
  void CreateHTMLWindow(const nuiEvent& rEvent);
  void CreateImageResizeWindow(const nuiEvent& rEvent);
  void CreateCoverFlowWindow(const nuiEvent& rEvent);
  void CreateUnicodeTextWindow(const nuiEvent& rEvent);

  void ShowMenu(const nuiEvent& rEvent);
  void ItemSelected(const nuiEvent& rEvent);
  void UserDraw(const nuiEvent& rEvent);
  void ToggleAnimation(const nuiEvent& rEvent);

  void SaveXMLDescription(const nuiEvent& rEvent);
  void OutputSomething(const nuiEvent& rEvent);
  void CreateFromGladeXML(const nuiEvent& rEvent);
  void CreateFromXML(const nuiEvent& rEvent);
  void HideText(const nuiEvent& rEvent);
  void LogText(const nuiEvent& rEvent);
  void ClearTable(const nuiEvent& rEvent);
  void FileSelect(const nuiEvent& rEvent);
  void FileSelectDone(const nuiEvent& rEvent);

  void UpdateInspector(const nuiEvent& rEvent);
  void ParentInspector(const nuiEvent& rEvent);
  void SelectInspector(const nuiEvent& rEvent);

  void DrawBlur(const nuiEvent& rEvent);

  void SoftwarePainterWindow(const nuiEvent& rEvent);

  void SpinerLabelWindow(const nuiEvent& rEvent);
  void VBoxAndHBoxWindow(const nuiEvent& rEvent);
  void MatrixViewWindow(const nuiEvent& rEvent);
  void MatrixViewTest(const nuiEvent& rEvent);
  void GetMatrixViewItemDesc(nuiMatrixViewItemDesc& rDesc);

  void DumpStats(const nuiEvent& rEvent);
  void OnTextWrapChanged(const nuiEvent& rEvent);
  void OnTextSizeSliderChanged(const nuiEvent& rEvent);
  void CreateTitledPaneWindow(const nuiEvent& rEvent);

  void CreateSliderWindow(const nuiEvent& rEvent);
  void OnSliderChanged(const nuiEvent& rEvent);

  void SaveFontDB(const nuiEvent& rEvent);
  void LoadFontDB(const nuiEvent& rEvent);

  void CreateFrameWindow(const nuiEvent& rEvent);
  void CreateFrameWindow2(const nuiEvent& rEvent);
  void CreateFrameWindow3(const nuiEvent& rEvent);

  void CreateAudioWindow(const nuiEvent& rEvent);

  void CreateAudioFifoWindow(const nuiEvent& rEvent);

  void CreateResourceWindow(const nuiEvent& rEvent);

  void CreateEllispsisTestWindow(const nuiEvent& rEvent);

  void CreateMessageBox(const nuiEvent& rEvent);
  void CreateWidgetTexture(const nuiEvent& rEvent);

  void AudioDeviceSelected(const nuiEvent& rEvent);
  void AudioComboTrashed(const nuiEvent& rEvent);

  // The window manager:
  nuiWindowManager* mpManager;

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
