/*
NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiInit.h"
#include "nuiWin.h"
#include "nuiApplication.h"

#include "nglFile.h"
#include "nglIFile.h"
#include "nglPath.h"
#include "nglThread.h"

#include "nuiCSS.h"
#include "nuiButton.h"
#include "nuiToggleButton.h"
#include "nuiFixed.h"
#include "nuiText.h"
#include "nuiEditText.h"
#include "nuiLabel.h"
#include "nuiScrollBar.h"
#include "nuiSplitter.h"
#include "nuiList.h"
#include "nuiFileList.h"
#include "nuiEditLine.h"
#include "nuiHoverDummy.h"
#include "nuiStateDummy.h"
#include "nuiSVGView.h"
#include "nuiWindowManager.h"
#include "nuiAnimView.h"
#include "nuiScrollView.h"
#include "nuiTreeView.h"
#include "nuiGrid.h"
#ifdef USE_SWF
#include "nuiSWF.h"
#endif
#include "nuiShapeView.h"
#include "nuiModalContainer.h"
#include "nuiPane.h"
#include "nuiFileSelector.h"
#include "nuiMessageQueue.h"
#include "nuiGradientDecoration.h"
#include "nuiBackgroundPane.h"
#include "nuiComboBox.h"

#include "nuiGladeLoader.h"
#include "nuiBuilder.h"

#include "nuiTextureHelpers.h"
#include "nuiColumnTreeView.h"

#include "nuiSoftwarePainter.h"

#include "nuiTessellator.h"
#include "nuiFontManager.h"

#include "nuiFormater.h"
#include "nuiFormatedLabel.h"
#include "nuiSpinnerLabel.h"
#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiMatrixView.h"
#include "nuiTitledPane.h"
#include "nuiFolderPane.h"
#include "nuiToken.h"

#include "nuiFrame.h"
#include "nuiFrameView.h"

#include "nuiAudioDevice.h"
#include "nuiAudioFifo.h"

#include "nuiNativeResource.h"

#include "nuiHTTP.h"
#include "nuiHTML.h"

#include "nuiMessageBox.h"

#include "nuiHyperLink.h"

#include "nuiColorDecoration.h"

#include <iostream>

using namespace std;

#include "nuiMatrixView.h"
#include "tests/ThreadInspectorTest.h"

#include "nuiBindings.h"

#include "nuiCoverFlow.h"

#include "nuiUnicode.h"

/*

* Simple console based on nuiText
*/

class Console : public nglConsole, public nuiText
{
public:
  Console(const nglString& rText)
    : nglConsole(false),
    nuiText(rText)
  {
    SetFont(nuiTheme::Fixed);
  }

  virtual ~Console()
  {
    App->SetConsole(NULL);
  }

  virtual void OnOutput(const nglString& rText)
  {
    AddText(rText);
    nglConsole::OnOutput(rText);
  }
};

template <class Param0, class Param1, class Ret>
class nuiAction
{
private:
  typedef nuiFastDelegate2<Ret, Param0, Param1> ActionDelegate;
  ActionDelegate mDelegate;
  Param0 mParam0;
  Param1 mParam1;
public:
  nuiAction(const ActionDelegate& rDelegate, Param0 p0, Param1 p1)
  : mDelegate(rDelegate), mParam0(p0), mParam1(p1)
  {
  }

  Ret operator()() const
  {
    return mDelegate(mParam0, mParam1);
  }
};


template <class Param0, class Param1, class Ret>
class nuiAdaptedAction
{
private:
  typedef nuiFastDelegate2<Ret, Param0, Param1> ActionDelegate;
  ActionDelegate mDelegate;
  Param1 mParam1;
public:
  nuiAdaptedAction(const ActionDelegate& rDelegate, Param1 p1)
  : mDelegate(rDelegate), mParam1(p1)
  {
  }

  Ret operator()(Param0 p0) const
  {
    return mDelegate(p0, mParam1);
  }
};

int prouttest(int a, int b)
{
  printf("A = %d , B = %d\n", a, b);
  return 13;
}

void prout()
{
  nuiAction<int, int, int> action(&::prouttest, 1, 42);
  int res = action();
  printf("result: %d\n", res);

  nuiAdaptedAction<int, int, int> aaction(&::prouttest, 42);
  res = aaction(12);
  printf("result: %d\n", res);
}

/*
* nuiWin
*/

uint nuiWin::mRefs = 0;


nuiWin::nuiWin(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath((_T("rsrc:")))),
mWinSink(this)
{
  mRefs++;

  mShowFPS = ShowFPS;
  mFrameCnt = 0;
  nuiTopLevel::EnablePartialRedraw(true);

  SetDebugMode(true);

  nuiInitBindings();

  if (rContextInfo.TargetAPI == eTargetAPI_OpenGL)
  {
    NGL_OUT(_T("glExtensions: %s\n"), glGetString(GL_EXTENSIONS));
  }



}



nuiTexture* gpTexture = NULL;
nuiDrawContext* gpDrawCtx = NULL;

static int OffX = 0,
OffY = 0,
OffW = 256,
OffH = 256;

void nuiWin::OnCreation()
{
  //EnableClearBackground(false);

  //SetColor(eActiveWindowBg, nuiColor(240,240,255));

  nuiSplitter* pFixed = new nuiSplitter(nuiVertical);
  AddChild(pFixed);

  pFixed->SetProperty(_T("Name"), _T("Bench"));

  nuiTreeNode* pMainTree = new nuiTreeNode(nuiTR("Main Menu"), true, false);

  nuiTreeNode* pElement = NULL;

  // Console Tester:
  pElement = new nuiTreeNode(nuiTR("Console Test window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateConsoleWindow, NULL);
  pMainTree->AddChild(pElement);

  // nuiThreadInspector Tester:
  pElement = new nuiTreeNode(nuiTR("nuiThreadInspector Test window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateThreadInspectorTestWindow, NULL);
  pMainTree->AddChild(pElement);


  // CSS Parser Tester:
  pElement = new nuiTreeNode(nuiTR("CSS Parser Test window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateCSSParserTest, NULL);
  pMainTree->AddChild(pElement);

  // Blur Area:
  pElement = new nuiTreeNode(nuiTR("Composited Radial Blur"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateBlurArea, NULL);
  pMainTree->AddChild(pElement);

  // TreeView Tester:
  pElement = new nuiTreeNode(nuiTR("Tree View"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateTreeViewWindow, NULL);
  pMainTree->AddChild(pElement);

  // SVG Tiger Tester:
  pElement = new nuiTreeNode(nuiTR("SVG View (Tiger)"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateSVGTigerWindow, NULL);
  pMainTree->AddChild(pElement);

  // Anim Tester:
  pElement = new nuiTreeNode(nuiTR("Simple Anim"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateAnimWindow, NULL);
  pMainTree->AddChild(pElement);

  // Save XML Desc:
  pElement = new nuiTreeNode(nuiTR("Serialize to nuiTextOutput.xml"));
  mWinSink.Connect(pElement->Activated, &nuiWin::SaveXMLDescription);
  pMainTree->AddChild(pElement);

  // Messed Up window:
  pElement = new nuiTreeNode(nuiTR("Messed up window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateMessedUpWindow);
  pMainTree->AddChild(pElement);

  // Text window:
  pElement = new nuiTreeNode(nuiTR("Fixed Font Text window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateTextWindow);
  pMainTree->AddChild(pElement);

  // Text2 window:
  pElement = new nuiTreeNode(nuiTR("Complex Font Layout test windows"));
  pMainTree->AddChild(pElement);

  {
    nuiTreeNode* pSubElement = new nuiTreeNode(nuiTR("I can eat glass"));
    mWinSink.Connect(pSubElement->Activated, &nuiWin::CreateText2Window, (void*)_T("rsrc:/GLASS.txt"));
    pElement->AddChild(pSubElement);

  }

  {
    nuiTreeNode* pSubElement = new nuiTreeNode(nuiTR("Hello"));
    mWinSink.Connect(pSubElement->Activated, &nuiWin::CreateText2Window, (void*)_T("rsrc:/HELLO.txt"));
    pElement->AddChild(pSubElement);

  }

  {
    nuiTreeNode* pSubElement = new nuiTreeNode(nuiTR("Simple text"));
    mWinSink.Connect(pSubElement->Activated, &nuiWin::CreateText2Window, (void*)_T("rsrc:/sampletext2.txt"));
    pElement->AddChild(pSubElement);

  }

  {
    nuiTreeNode* pSubElement = new nuiTreeNode(nuiTR("All unicode glyphs (very slow!)"));
    mWinSink.Connect(pSubElement->Activated, &nuiWin::CreateText2Window, (void*)_T("rsrc:/all-unicode.txt"));
    pElement->AddChild(pSubElement);

  }

  {
    static const nglChar* tests[] =
    {
      _T("rsrc:/font_tests/test-japanese.txt"),
      _T("rsrc:/font_tests/test-japanese2.txt"),
      _T("rsrc:/font_tests/test-arabic.txt"),
      _T("rsrc:/font_tests/test-chinese.txt"),
      _T("rsrc:/font_tests/test-devanagari.txt"),
      _T("rsrc:/font_tests/test-gurmukhi.txt"),
      _T("rsrc:/font_tests/test-hebrew.txt"),
      _T("rsrc:/font_tests/test-ipa.txt"),
      _T("rsrc:/font_tests/test-lao.txt"),
      _T("rsrc:/font_tests/test-latin.txt"),
      _T("rsrc:/font_tests/test-long-paragraph.txt"),
      _T("rsrc:/font_tests/test-mixed.txt"),
      _T("rsrc:/font_tests/test-nko.txt"),
      _T("rsrc:/font_tests/test-syriac.txt"),
      _T("rsrc:/font_tests/test-tamil.txt"),
      _T("rsrc:/font_tests/test-thai.txt"),
      _T("rsrc:/font_tests/test-tibetan.txt"),
      _T("rsrc:/font_tests/test-hangul.txt"),
      NULL
    };

    for (int i = 0; tests[i]; i++)
    {
      nglString txt(nuiTR("Test file: "));
      nglPath p(tests[i]);
      txt += p.GetNodeName();

      nuiTreeNode* pSubElement = new nuiTreeNode(txt);
      mWinSink.Connect(pSubElement->Activated, &nuiWin::CreateText2Window, (void*)tests[i]);
      pElement->AddChild(pSubElement);
    }
  }

  // File List window:
  pElement = new nuiTreeNode(nuiTR("File List window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateFileListWindow);
  pMainTree->AddChild(pElement);

  // Column Tree View window:
  pElement = new nuiTreeNode(nuiTR("Column Tree View Window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateColumnTreeViewWindow);
  pMainTree->AddChild(pElement);

  // CreateScrolledImageWindow:
  pElement = new nuiTreeNode(nuiTR("Scrolled Image Window (1)"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateScrolledImageWindow);
  pMainTree->AddChild(pElement);

  // CreateScrolledImageWindow2:
  pElement = new nuiTreeNode(nuiTR("Scrolled Image Window (2)"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateScrolledImageWindow2);
  pMainTree->AddChild(pElement);

#ifdef USE_SWF
  // CreateSWFTest1Window:
  pElement = new nuiTreeNode(nuiTR("SWF widget Test 1 Window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateSWFTest1Window);
  pMainTree->AddChild(pElement);
#endif

  // CreateShapeWindow:
  pElement = new nuiTreeNode(nuiTR("Shapes Window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateShapeWindow);
  pMainTree->AddChild(pElement);

  // TogglePartialRedraw:
  pElement = new nuiTreeNode(nuiTR("Toggle Partial Redraw"));
  mWinSink.Connect(pElement->Activated, &nuiWin::TogglePartialRedraw);
  pMainTree->AddChild(pElement);

  // CreateWrappedLabelWindow:
  pElement = new nuiTreeNode(nuiTR("Wrapped Label"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateWrappedLabelWindow);
  pMainTree->AddChild(pElement);

  // CreateWhizzAnimWindow:
  pElement = new nuiTreeNode(nuiTR("Whizzy Anim"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateWhizzAnimWindow);
  pMainTree->AddChild(pElement);

  // CreateTextLayoutWindow:
  pElement = new nuiTreeNode(nuiTR("Text Layout"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateTextLayoutWindow);
  pMainTree->AddChild(pElement);


  // CreateFontSubstitutionWindow:
  pElement = new nuiTreeNode(nuiTR("Font Substitution window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateFontSubstitutionWindow);
  pMainTree->AddChild(pElement);

  // CreateSignalsAndSlotsTestWindow:
  pElement = new nuiTreeNode(nuiTR("Signals And Slots Tests Window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateSignalsAndSlotsTestWindow);
  pMainTree->AddChild(pElement);

  // SoftwarePainterWindow:
  pElement = new nuiTreeNode(nuiTR("Software Painter Window"));
  mWinSink.Connect(pElement->Activated, &nuiWin::SoftwarePainterWindow);
  pMainTree->AddChild(pElement);

  // $loic (2007/08/24)
  // SpinerLabelWindow
  pElement = new nuiTreeNode (nuiTR("Spiner Label and Formated Label"));
  mWinSink.Connect (pElement->Activated, &nuiWin::SpinerLabelWindow);
  pMainTree->AddChild (pElement);

  // VBoxAndHBoxWindow
  pElement = new nuiTreeNode (nuiTR("VBox and HBox"));
  mWinSink.Connect (pElement->Activated, &nuiWin::VBoxAndHBoxWindow);
  pMainTree->AddChild (pElement);

  // MatrixViewWindow
  pElement = new nuiTreeNode (nuiTR("MatrixView"));
  mWinSink.Connect (pElement->Activated, &nuiWin::MatrixViewWindow);
  pMainTree->AddChild (pElement);


  // DumpStats:
  pElement = new nuiTreeNode(nuiTR("Dump some stats to the console"));
  mWinSink.Connect(pElement->Activated, &nuiWin::DumpStats);
  pMainTree->AddChild(pElement);

  nuiTreeView* pTreeMenu = new nuiTreeView(pMainTree, false);
  nuiScrollView* pScrollView = new nuiScrollView();
  pScrollView->AddChild(pTreeMenu);
  pFixed->AddChild(pScrollView);

  nuiScrollView* pWMScrollView = new nuiScrollView();
  pFixed->AddChild(pWMScrollView);
  pWMScrollView->SetObjectName(_T("WMScrollView"));
  mpManager = new nuiWindowManager();
  pWMScrollView->AddChild(mpManager);
  mpManager->SetObjectName(_T("wm"));


  pElement = new nuiTreeNode(nuiTR("nuiTitledPane and nuiFolderPane"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateTitledPaneWindow);
  pMainTree->AddChild(pElement);


  pElement = new nuiTreeNode(nuiTR("Slider test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateSliderWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Save font database"));
  mWinSink.Connect(pElement->Activated, &nuiWin::SaveFontDB);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Load font database"));
  mWinSink.Connect(pElement->Activated, &nuiWin::LoadFontDB);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Frame View"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateFrameWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Frame View 2"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateFrameWindow2);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Frame View 3"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateFrameWindow3);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Drag Zone"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateDragZone);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Clipping Test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateClippingTest);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Surface Test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateSurfaceTest);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Enum audio hardware"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateAudioWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("nuiAudioFifo test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateAudioFifoWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("nuiHTTP test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateHTTPWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("nuiHTML test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateHTMLWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("nglImage Resize test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateImageResizeWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("CoverFlow test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateCoverFlowWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Unicode Text Analysis"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateUnicodeTextWindow);
  pMainTree->AddChild(pElement);

  pElement = new nuiTreeNode(nuiTR("Enum native resources"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateResourceWindow);
  pMainTree->AddChild(pElement);

  // Enable Widget Tester:
  pElement = new nuiTreeNode(nuiTR("Enable Widget"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateEnableTestWindow, NULL);
  pMainTree->AddChild(pElement);

  // Button Decorations:
  pElement = new nuiTreeNode(nuiTR("Button decorations"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateButtonDecorationWindow, NULL);
  pMainTree->AddChild(pElement);

  // Enumerate Volumes:
  pElement = new nuiTreeNode(nuiTR("Enumerate Volumes"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateVolumesTestWindow, NULL);
  pMainTree->AddChild(pElement);

  // Label Elleipsi Test:
  pElement = new nuiTreeNode(nuiTR("Label Ellipsis Test"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateEllispsisTestWindow, NULL);
  pMainTree->AddChild(pElement);

  // Message box:
  pElement = new nuiTreeNode(nuiTR("Message Box"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateMessageBox, NULL);
  pMainTree->AddChild(pElement);

  // Widget Texture:
  pElement = new nuiTreeNode(nuiTR("Widget Texture"));
  mWinSink.Connect(pElement->Activated, &nuiWin::CreateWidgetTexture, NULL);
  pMainTree->AddChild(pElement);

}



void nuiWin::CreateConsoleWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10,10,400,400));
  mpManager->AddChild(pWindow);
  nuiScrollView* pScrollView = new nuiScrollView();
  pWindow->AddChild(pScrollView);

  Console* pConsole = new Console(nuiTR("Welcome to the nui test application!\nThis window contains a text widget to which the ngl's console output have been redirected\n"));
  pScrollView->AddChild(pConsole);
  App->SetConsole(pConsole);
}


// a test for the nuiThreadInspector from the nuiIntrospector
void nuiWin::CreateThreadInspectorTestWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10,10,500,400));
  mpManager->AddChild(pWindow);

  pWindow->AddChild(new ThreadInspectorTest());
}


void nuiWin::CreateCSSParserTest(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10,10,400,400));
  mpManager->AddChild(pWindow);
  nuiScrollView* pScrollView = new nuiScrollView();
  pWindow->AddChild(pScrollView);

  nuiText* pText = new nuiText();
  pScrollView->AddChild(pText);

  pText->Print(_T("CSS Parser test:\n"));
  nglPath p(_T("rsrc:/test.css"));
  nglIStream* pF = p.OpenRead();
  if (!pF)
  {
    pText->Print(_T("Unable to open CSS source file '%ls'\n"), p.GetChars());
    return;
  }

  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF);

  delete pF;

  if (!res)
  {
    pText->Print(_T("Error parsing CSS File: %s\n"), pCSS->GetErrorString().GetChars());
  }

}


void nuiWin::CreateBlurArea(const nuiEvent& rEvent)
{
  nuiUserArea* pUserArea = new nuiUserArea();
  AddChild(pUserArea);
  nuiTexture* pTexture = nuiTexture::GetTexture(nglPath(_T("rsrc:/nui.png")));
  pTexture->SetPermanent(true);
  mWinSink.Connect(pUserArea->UserDraw, &nuiWin::DrawBlur, pTexture);
}

void nuiWin::CreateTreeViewWindow(const nuiEvent& rEvent)
{
  nuiWindow* pTreeWin = new nuiWindow(nuiRect(10, 10, 320, 300), nuiWindow::NoFlag, nuiTR("TreeView example"));
  mpManager->AddChild(pTreeWin);

  nuiTreeNode* pTreeNode = new nuiTreeNode(_T("Prout"), true, true);
  pTreeNode->AddChild(new nuiTreeNode(_T("Prout1"), true, true));
  pTreeNode->AddChild(new nuiTreeNode(_T("Prout2"), true, true));

  nuiTreeNode* pTreeNode2 = new nuiTreeNode(_T("Prout3"), true, true);
  pTreeNode->AddChild(pTreeNode2);

  pTreeNode2->AddChild(new nuiTreeNode(_T("X")));
  pTreeNode2->AddChild(new nuiTreeNode(_T("XX")));

  nuiTreeNode* pTreeNode3 = new nuiTreeNode(nuiTR("Image Container:"), true, true);
  pTreeNode2->AddChild(pTreeNode3);
  pTreeNode3->AddChild(new nuiTreeNode(new nuiImage(_T("rsrc:/toucan.png"))));
  pTreeNode3->AddChild(new nuiTreeNode(new nuiImage(_T("rsrc:/small_ngl.png"))));

  pTreeNode2->AddChild(new nuiTreeNode(nuiTR("XXXX")));
  pTreeNode2->Open(true);

  nuiScrollView* pScrollView = new nuiScrollView();
  pTreeWin->AddChild(pScrollView);
  pScrollView->EnableSmoothScrolling(true);

  nuiTreeView* pTreeView = new nuiTreeView(pTreeNode);
  pScrollView->AddChild(pTreeView);
  pScrollView->EnableSurface(true);
}




void nuiWin::CreateAnimWindow(const nuiEvent& rEvent)
{
  nuiWindow* pAnimWin = new nuiWindow(nuiRect(10, 10, 100, 200), nuiWindow::NoFlag, nuiTR("Anim example"));
  mpManager->AddChild(pAnimWin);
  nuiAnimView* pAnim = new nuiAnimView();
  nuiDecoration* pDeco = nuiDecoration::Get(_T("AnimBg"));
  if (!pDeco)
    pDeco = new nuiColorDecoration(_T("AnimBg"), nuiColor(0, 0, 0), 0, nuiColor(0, 0, 0), eStrokeAndFillShape);
  pAnimWin->SetDecoration(pDeco);
  pAnimWin->AddChild(pAnim);
  for (int k = 0; k < 31; k++)
  {
    nglString kk;
    kk.Format(_T("rsrc:/knob/knob%d.png"), k);
    nuiImage* pImg = new nuiImage(kk);
    pAnim->AddChild(pImg);
    pImg->SetPosition(nuiCenter);
    //pImg->SetPosition(nuiFill);
  }


  pAnim->SetDuration(2);
  pAnim->SetEasing(nuiEasingSinus);
  pAnim->SetTime(0);
  pAnim->Play(10000, eAnimLoopPingPong);
}

void nuiWin::CreateSVGTigerWindow(const nuiEvent& rEvent)
{

  nuiWindow* pSVGWin = new nuiWindow(nuiRect(10.0f,10.0f,250.0f,400.0f));
  mpManager->AddChild(pSVGWin);
  nuiSVGView* pSVGView;

  nglTime before;
  //  pSVGView = new nuiSVGView(pSVGWin, nglPath(_T("rsrc:/tvtower.svg")));
  pSVGView = new nuiSVGView(nglPath(_T("rsrc:/tiger.svg")));
  //  pSVGView = new nuiSVGView(pSVGWin,_T("rsrc:/lion.svg"));
  //  pSVGView = new nuiSVGView(pSVGWin, nglPath(_T("rsrc:/mapsym.svg")));
  //  pSVGView = new nuiSVGView(pSVGWin,_T("rsrc:/butterfly.svg"));
  //  pSVGView = new nuiSVGView(pSVGWin,_T("rsrc:/cubic02.svg"));
  pSVGView->EnableSurface(true);
  //pSVGView->SetDebug(1);

  pSVGWin->AddChild(pSVGView);
  NGL_OUT((_T("Loaded SVG file in %f seconds\n")), (double)(nglTime()-before));

  pSVGView->EnableAntialias(true);
}


void nuiWin::CreateMessedUpWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10.0f,10.0f,350.0f,250.0f));
  mpManager->AddChild(pWin);
  nuiVBox* pBox = new nuiVBox(0);
  pWin->AddChild(pBox);
  //  pBox->SetUserRect(nuiRect(0,0,350,250));

  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  pBox->AddCell(pSplitter);
  //LBDEBUG pSplitter->SetFixed(true);
  pSplitter->SetMasterChild(true);

  nuiText* pText = new nuiText(nuiTR("This window is a box of buttons.\nYou can select any one of them and their\nlayout is managed by their owner!.\n"));
  nuiScrollView* pSV = new nuiScrollView();
  pSplitter->AddChild(pSV);
  pSV->AddChild(pText);

  nuiHoverDummy* pDummy = new nuiHoverDummy(NULL, NULL);
  pSplitter->AddChild(pDummy);
  nuiImage* pDummyImg = new nuiImage(nglPath(_T("rsrc:/small_ngl.png")));
  pDummyImg->SetFillRule(nuiCenter);
  nuiLabel* pDummyLabel = new nuiLabel((nuiTR("This is the ngl logo!!!")));

  pDummy->SetDummy(false, pDummyImg);
  pDummy->SetDummy(true, pDummyLabel);

  nuiButton* pButton1 = new nuiButton(nuiTR("Create a new window"));
  pBox->AddCell(pButton1);
  //mWinSink.Connect(pButton1->ButtonPressed, &nuiWin::CreateNewWindow);

  nuiButton* pBtn = new nuiButton(nuiTR("add text"));
  pBox->AddCell(pBtn);
  mWinSink.Connect(pBtn->ButtonPressed, &nuiWin::OutputSomething, pBtn);

  pBtn = new nuiButton(nuiTR("Create a window from an NUI XML description"));
  pBox->AddCell(pBtn);
  mWinSink.Connect(pBtn->ButtonPressed, &nuiWin::CreateFromXML, pBtn);

  pBtn = new nuiButton(nuiTR("Do a file selection"));
  pBox->AddCell(pBtn);
  mWinSink.Connect(pBtn->ButtonPressed, &nuiWin::FileSelect, NULL);

  nuiEditLine* pEdt = new nuiEditLine(nglString(_T(""))  );
  pBox->AddCell(pEdt);
  mWinSink.Connect(pEdt->Selected, &nuiWin::LogText, pEdt);
}

void nuiWin::CreateTextWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin2 = new nuiWindow(nuiRect(10.0f,10.0f,500.0f,350.0f),0 ,(nuiTR("Fixed Font Text Window")));
  mpManager->AddChild(pWin2);

  nuiEditText* pText = new nuiEditText(nglString::Empty);
  nuiScrollView* pSV = new nuiScrollView();
  pWin2->AddChild(pSV);
  pSV->AddChild(pText);


  nglPath path(_T("rsrc:/sampletext.txt"));
  nglIStream* pFile = path.OpenRead();
  nglString text;

  if (pFile && pFile->ReadText(text))
  {
    pText->SetFollowModifications(false);
    pText->AddText(text);
    pText->SetFollowModifications(true);
  }

  //pText->SetFont(nuiTheme::Default);
  pText->SetFont(nuiTheme::Fixed);

  delete pFile;
}

void nuiWin::CreateText2Window(const nuiEvent& rEvent)
{
  nuiWindow* pWin2 = new nuiWindow(nuiRect(10.0f,10.0f,500.0f,350.0f),0 ,(nuiTR("Default Font Text Window")));
  mpManager->AddChild(pWin2);

  nuiGrid* pGrid = new nuiGrid(1, 2);
  pWin2->AddChild(pGrid);
  nuiGrid* pGrid2 = new nuiGrid(2, 1);
  pGrid->SetColumnExpand(0, nuiExpandShrinkAndGrow, 1.0f);
  pGrid->SetRowExpand(1, nuiExpandShrink, 1.0f);
  pGrid2->SetColumnExpand(1, nuiExpandShrinkAndGrow, 1.0f);

  nuiScrollView* pScrollView = new nuiScrollView(false, true);
  nuiLabel* pText = new nuiLabel(nglString::Empty);
  pScrollView->AddChild(pText);
  pText->SetPosition(nuiTopLeft);

  nuiToggleButton* pCheck = new nuiToggleButton(nuiTR("Wrapping"));
  pCheck->SetPressed(true);
  nuiSlider* pSlider = new nuiSlider(nuiHorizontal, nuiRange(15, 1, 128, 1.f, 5.f, 0));


  pGrid2->SetCell(0, 0, pCheck, nuiFill);
  pGrid2->SetCell(1, 0, pSlider, nuiFill);

  pGrid->SetCell(0, 0, pGrid2, nuiFill);
  pGrid->SetCell(0, 1, pScrollView, nuiFill);

  pText->SetWrapping(true);
  //pText->SetDebug(1);

  nglPath path((nglChar*)rEvent.mpUser);
  nglIStream* pFile = path.OpenRead();
  if (!pFile)
    return;

  pFile->SetTextEncoding(eUTF8);
  nglString text;

  pText->SetFont(nuiFont::GetFont(15));

  if (pFile->ReadText(text))
  {
    //printf("Read text:\n\n%s\n\n", text.GetChars());
    pText->SetText(text);
  }

  mWinSink.Connect(pSlider->GetRange().ValueChanged, &nuiWin::OnTextSizeSliderChanged, new std::pair<nuiLabel*, nuiRange*>(pText, &pSlider->GetRange()));
  mWinSink.Connect(pCheck->ButtonPressed, &nuiWin::OnTextWrapChanged, pText);
  mWinSink.Connect(pCheck->ButtonDePressed, &nuiWin::OnTextWrapChanged, pText);

  delete pFile;
}

void nuiWin::OnTextWrapChanged(const nuiEvent& rEvent)
{
  nuiLabel* pLabel = (nuiLabel*)rEvent.mpUser;
  pLabel->SetWrapping(!pLabel->IsWrapping());
}

void nuiWin::OnTextSizeSliderChanged(const nuiEvent& rEvent)
{
  std::pair<nuiLabel*, nuiRange*>* pPair = (std::pair<nuiLabel*, nuiRange*>*)rEvent.mpUser;

  nuiLabel* pLabel = pPair->first;
  nuiRange* pRange = pPair->second;

  pLabel->SetFont(nuiFont::GetFont(pRange->GetValue()));
}

void nuiWin::CreateSliderWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10.0f,10.0f, 300.f,400.f), nuiWindow::DecoratedBackground ,(nuiTR("Slider Window")));
  mpManager->AddChild(pWin);

  nuiVBox* pVBox = new nuiVBox(5);
  pVBox->SetAllCellsSpacing(5);
  pVBox->SetExpand(nuiExpandShrinkAndGrow);
  pVBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
  pVBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);
  pVBox->SetCellExpand(2, nuiExpandShrinkAndGrow, 1);

  pWin->AddChild(pVBox);

  //Default Drawing
  {
    nuiHBox* pDefaultBox = new nuiHBox(2);
    pDefaultBox->SetExpand(nuiExpandShrinkAndGrow);
    pDefaultBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
    pDefaultBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);

    nuiVBox* pLabelBox = new nuiVBox(2);
    pLabelBox->SetExpand(nuiExpandShrinkAndGrow);
    pLabelBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
    pLabelBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);
    pLabelBox->SetCell(0, new nuiLabel(nuiTR("Default drawing:")));
    pLabelBox->SetCell(1, pDefaultBox);

    pVBox->SetCell(0, pLabelBox);

    //Vertical Slider
    {
      nuiHBox* pBox = new nuiHBox(3);
      pDefaultBox->SetCell(0, pBox);
      nuiSlider* pSlider = new nuiSlider(nuiVertical, nuiRange(0, 0, 100, 1.f, 5.f, 0));
      nglString s;
      s.Format(_T("%.2f"), pSlider->GetRange().GetValue());
      nuiLabel* pText = new nuiLabel(s);
      pText->SetPosition(nuiCenter);
      nuiLabel* pText2 = new nuiLabel(s);
      pText2->SetPosition(nuiCenter);


      pBox->SetCell(0, pSlider, nuiCenter);
      pBox->SetCell(1, pText);
      pBox->SetCell(2, pText2);

      mWinSink.Connect(pSlider->InteractiveValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText));
      mWinSink.Connect(pSlider->ValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText2));
    }

    //Horizontal Slider
    {
      nuiHBox* pBox = new nuiHBox(3);
      pDefaultBox->SetCell(1, pBox);
      nuiSlider* pSlider = new nuiSlider(nuiHorizontal, nuiRange(0, 0, 100, 1.f, 5.f, 0));
      nglString s;
      s.Format(_T(".2f"), pSlider->GetRange().GetValue());
      nuiLabel* pText = new nuiLabel(s);
      pText->SetPosition(nuiCenter);
      nuiLabel* pText2 = new nuiLabel(s);
      pText2->SetPosition(nuiCenter);

      pBox->SetCell(0, pSlider, nuiCenter);
      pBox->SetCell(1, pText);
      pBox->SetCell(2, pText2);

      mWinSink.Connect(pSlider->InteractiveValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText));
      mWinSink.Connect(pSlider->ValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText2));
    }

  }

  //Discreet Step
  {
    nuiHBox* pDefaultBox = new nuiHBox(2);
    pDefaultBox->SetExpand(nuiExpandShrinkAndGrow);
    pDefaultBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
    pDefaultBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);

    nuiVBox* pLabelBox = new nuiVBox(2);
    pLabelBox->SetExpand(nuiExpandShrinkAndGrow);
    pLabelBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
    pLabelBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);
    pLabelBox->SetCell(0, new nuiLabel(nuiTR("Discreet Step:")));
    pLabelBox->SetCell(1, pDefaultBox);

    pVBox->SetCell(1, pLabelBox);

    //Vertical Slider
    {
      nuiHBox* pBox = new nuiHBox(3);
      pDefaultBox->SetCell(0, pBox);
      nuiSlider* pSlider = new nuiSlider(nuiVertical, nuiRange(0, 0, 100, 20.f, 50.f, 0));
      pSlider->GetRange().SetDiscreetStepSize(true);
      nglString s;
      s.Format(_T("%.2f"), pSlider->GetRange().GetValue());
      nuiLabel* pText = new nuiLabel(s);
      pText->SetPosition(nuiCenter);
      nuiLabel* pText2 = new nuiLabel(s);
      pText2->SetPosition(nuiCenter);


      pBox->SetCell(0, pSlider, nuiCenter);
      pBox->SetCell(1, pText);
      pBox->SetCell(2, pText2);

      mWinSink.Connect(pSlider->InteractiveValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText));
      mWinSink.Connect(pSlider->ValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText2));
    }

    //Horizontal Slider
    {
      nuiHBox* pBox = new nuiHBox(3);
      pDefaultBox->SetCell(1, pBox);
      nuiSlider* pSlider = new nuiSlider(nuiHorizontal, nuiRange(0, 0, 100, 20.f, 50.f, 0));
      pSlider->GetRange().SetDiscreetStepSize(true);
      nglString s;
      s.Format(_T(".2f"), pSlider->GetRange().GetValue());
      nuiLabel* pText = new nuiLabel(s);
      pText->SetPosition(nuiCenter);
      nuiLabel* pText2 = new nuiLabel(s);
      pText2->SetPosition(nuiCenter);

      pBox->SetCell(0, pSlider, nuiCenter);
      pBox->SetCell(1, pText);
      pBox->SetCell(2, pText2);

      mWinSink.Connect(pSlider->InteractiveValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText));
      mWinSink.Connect(pSlider->ValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText2));
    }

  }



  //Handle with Image + Background Deco
  {
    nuiHBox* pHandleBox = new nuiHBox(2);
    pHandleBox->SetExpand(nuiExpandShrinkAndGrow);
    pHandleBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
    pHandleBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);

    nuiVBox* pLabelBox = new nuiVBox(2);
    pLabelBox->SetExpand(nuiExpandShrinkAndGrow);
    pLabelBox->SetCellExpand(0, nuiExpandShrinkAndGrow, 1);
    pLabelBox->SetCellExpand(1, nuiExpandShrinkAndGrow, 1);
    pLabelBox->SetCell(0, new nuiLabel(nuiTR("Handle with Image + Background Deco:")));
    pLabelBox->SetCell(1, pHandleBox);

    pVBox->SetCell(4, pLabelBox);

    nuiFrame* pFrame = new nuiFrame(nuiTR("SliderBackground"), _T("rsrc:/slider/background.png"), nuiRect(0 ,0 , 201, 7));
    {
      //Vertical Slider with widgets
      nuiImage* pHandle = new nuiImage(_T("rsrc:/slider/handle.png"));

      nuiHBox* pBox = new nuiHBox(3);
      pHandleBox->SetCell(0, pBox);
      nuiSlider* pSlider = new nuiSlider(nuiVertical, nuiRange(0, 0, 100, 1.f, 5.f, 0));
      pSlider->SetDecoration(pFrame);
      nglString s;
      s.Format(_T(".2f"), pSlider->GetRange().GetValue());
      nuiLabel* pText = new nuiLabel(s);
      pText->SetPosition(nuiCenter);
      nuiLabel* pText2 = new nuiLabel(s);
      pText2->SetPosition(nuiCenter);

      pSlider->SetHandle(pHandle);

      pBox->SetCell(0, pSlider, nuiCenter);
      pBox->SetCell(1, pText);
      pBox->SetCell(2, pText2);

      pBox->SetExpand(nuiExpandShrinkAndGrow);

      mWinSink.Connect(pSlider->InteractiveValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText));
      mWinSink.Connect(pSlider->ValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText2));
    }

    {
      //Horizontal Slider with widgets
      nuiImage* pHandle = new nuiImage(_T("rsrc:/slider/handle.png"));

      nuiHBox* pBox = new nuiHBox(3);
      pHandleBox->SetCell(1, pBox);
      nuiSlider* pSlider = new nuiSlider(nuiHorizontal, nuiRange(0, 0, 100, 1.f, 5.f, 0));
      pSlider->SetDecoration(pFrame);
      nglString s;
      s.Format(_T(".2f"), pSlider->GetRange().GetValue());
      nuiLabel* pText = new nuiLabel(s);
      pText->SetPosition(nuiCenter);
      nuiLabel* pText2 = new nuiLabel(s);
      pText2->SetPosition(nuiCenter);

      pSlider->SetHandle(pHandle);

      pBox->SetCell(0, pSlider, nuiCenter);
      pBox->SetCell(1, pText);
      pBox->SetCell(2, pText2);

      pBox->SetExpand(nuiExpandShrinkAndGrow);

      mWinSink.Connect(pSlider->InteractiveValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText));
      mWinSink.Connect(pSlider->ValueChanged, &nuiWin::OnSliderChanged, new std::pair<nuiSlider*, nuiLabel*>(pSlider, pText2));
    }
  }
}

void nuiWin::OnSliderChanged(const nuiEvent& rEvent)
{
  std::pair<nuiSlider*, nuiLabel*>* pPair = (std::pair<nuiSlider*, nuiLabel*>*)rEvent.mpUser;
  nuiSlider* pSlider = pPair->first;
  nuiLabel* pText = pPair->second;

  nglString s;
  s.Format(_T("%.2f"), pSlider->GetRange().GetValue());
  pText->SetText(s);
}

void nuiWin::CreateEllispsisTestWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10.0f,10.0f,200.0f,50.0f),0 ,(nuiTR("Label Ellipsis Test Window")));
  mpManager->AddChild(pWin);

  //  nuiHBox* pHBox = new nuiHBox(2);

  nuiLabel* pLabel = new nuiLabel(_T("toto1_toto2_toto3_toto4_toto5_toto6_toto7_toto8_toto9_toto10"));
  pLabel->UseEllipsis(true);
  //
  //  nuiButton* pBtn = new nuiButton(_T("blabla"));
  //
  //  pHBox->SetCell(0, pLabel);
  //  pHBox->SetCell(1, pBtn);
  //
  //  pHBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  //  pHBox->SetCellExpand(1, nuiExpandFixed);
  //
  nuiVBox* pVBox = new nuiVBox();
  pVBox->AddCell(pLabel);
  pVBox->SetExpand(nuiExpandShrinkAndGrow);

  pWin->AddChild(pVBox);
}


void nuiWin::CreateFileListWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin3 = new nuiWindow(nuiRect(10.0f,10.0f,250.0f,400.0f));
  mpManager->AddChild(pWin3);
  //  nuiList* pList = new nuiList(pWin3,Horizontal);
  nuiList* pList = new nuiFileList(nglPath(ePathCurrent));
  nuiScrollView* pSV = new nuiScrollView(pWin3);
  pSV->AddChild(pList);
  pWin3->AddChild(pSV);
  //  (new nuiImage(pList,nglPath(_T("rsrc:/toucan.png"))))->SetPosition(nuiCenter);
  //  new nuiImage(pList,nglPath(_T("rsrc:/small_ngl.png")));
}


class SamplePreview : public nuiColumnTreeViewPreview
{
public:
  SamplePreview(nuiColumnTreeView* pParent)
    : nuiColumnTreeViewPreview(pParent)
  {
  }

  bool UpdatePreview(nuiColumnTreeView* pTree, nuiTreeNodePtr pSelectedNode)
  {
    if (!pTree)
    {
      Clear();
      return false;
    }

    if (pSelectedNode && !pSelectedNode->IsOpened())
    {
      Clear();
      nuiVBox* pWidget = new nuiVBox(0);
      AddChild(pWidget);
      nglString str;
      nuiWidget* pItem = pSelectedNode->GetElement();

      str.Format(_T("TreeNode pointer: 0x%x"), pSelectedNode);
      pWidget->AddCell(new nuiLabel(str));
      str.Format(_T("Element pointer: 0x%x"), pItem);
      pWidget->AddCell(new nuiLabel(str));
      str.Format(_T("Item class: %ls"), pItem->GetObjectClass().GetChars());
      pWidget->AddCell(new nuiLabel(str));
      str.Format(_T("Item name: %ls"), pItem->GetObjectName().GetChars());
      pWidget->AddCell(new nuiLabel(str));

      str.Format(_T("Is open: %ls"), pSelectedNode->IsOpened()?_T("yes"):_T("no"));
      pWidget->AddCell(new nuiLabel(str));
      str.Format(_T("Size: %f"), pSelectedNode->GetSize());
      pWidget->AddCell(new nuiLabel(str));
      str.Format(_T("Position: %f"), pSelectedNode->GetPosition());
      pWidget->AddCell(new nuiLabel(str));

      return true;
    }

    return false;
  }
};

void nuiWin::CreateColumnTreeViewWindow(const nuiEvent& rEvent)
{
  nuiWindow* pColumnTreeViewWin = new nuiWindow(nuiRect(10, 10, 300, 200), nuiWindow::NoFlag, nuiTR("Column Tree View"));
  mpManager->AddChild(pColumnTreeViewWin);
  nuiTreeNode* pTree = new nuiTreeNode(nuiTR("Menu"));
  pTree->AddChild(new nuiTreeNode(nuiTR("test1")));
  pTree->AddChild(new nuiTreeNode(nuiTR("test2")));
  pTree->AddChild(new nuiTreeNode(nuiTR("test3")));
  nuiTreeNode* pSubTree = new nuiTreeNode(nuiTR("test4"));
  pSubTree->Open(true);
  pTree->AddChild(pSubTree);
  pSubTree->AddChild(new nuiTreeNode(nuiTR("sub1")));
  pSubTree->AddChild(new nuiTreeNode(nuiTR("sub2 with a quite long name...")));
  pSubTree->AddChild(new nuiTreeNode(nuiTR("sub3")));

  nuiTreeNode* pSubTree2 = new nuiTreeNode(nuiTR("Bleh"));
  pSubTree2->Open(true);
  pSubTree->AddChild(pSubTree2);
  pSubTree2->AddChild(new nuiTreeNode(nuiTR("first bleh")));
  pSubTree2->AddChild(new nuiTreeNode(nuiTR("middle bleh")));
  pSubTree2->AddChild(new nuiTreeNode(nuiTR("last bleh")));

  nuiTreeNode* pSubTree3 = new nuiTreeNode(nuiTR("Kazoo"));
  pSubTree3->Open(true);
  pSubTree2->AddChild(pSubTree3);
  pSubTree3->AddChild(new nuiTreeNode(nuiTR("Kazoo for Breakfast")));
  pSubTree3->AddChild(new nuiTreeNode(nuiTR("Kazoo for Lunch")));
  pSubTree3->AddChild(new nuiTreeNode(nuiTR("Kazoo for Diner")));

  for (int counter = 0; counter < 10; counter++)
  {
    nglString tmp;
    tmp.Format(_T("Dummy child %d"), counter);
    pTree->AddChild(new nuiTreeNode(tmp));
    pSubTree->AddChild(new nuiTreeNode(tmp));
    pSubTree2->AddChild(new nuiTreeNode(tmp));
    pSubTree3->AddChild(new nuiTreeNode(tmp));
  }

  nuiColumnTreeView* pView = new nuiColumnTreeView(pTree);
  nuiScrollView* pSV = new nuiScrollView(true, false);
  pColumnTreeViewWin->AddChild(pSV);
  pSV->AddChild(pView);

  new SamplePreview(pView);
}



bool nuiWin::Draw(nuiDrawContext* pContext)
{
  //pContext->PermitAntialiasing(false);

  bool res;


  /*
  {
  nuiGradient gradient;
  gradient.AddStop(nuiColor(0.9f, 0.9f, 1.0f, 1.0f), 0.0f);
  gradient.AddStop(nuiColor(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
  pContext->DrawGradient(gradient, GetRect().Size(), 0,0, 0, GetRect().GetHeight());
  }
  */

  res = nuiMainWindow::Draw(pContext);

  if (mShowFPS)
  {
    nglTime now;

    // Print FPS
    mFrameCnt++;
    if (mFrameCnt > 20)
    {
      nuiPainter* pPainter = pContext->GetPainter();
      mFPSText.Format(_T("%.1f (%d vertices in %d batches and %d operations)"),
        (double)mFrameCnt / (now - mLastFPSMeasure),
        pPainter->GetVertices(),
        pPainter->GetBatches(),
        pPainter->GetRenderOperations());
      mLastFPSMeasure = now;
      mFrameCnt = 0;
      //pPainter->ResetStats();
      //NGL_OUT((_T("FPS: %ls\n")), mFPSText.GetChars());
    }

    if (!mFPSText.IsEmpty())
    {
      pContext->SetFont(nuiFont::GetFont(12));
      pContext->SetTextColor(nuiColor(1,0,0));
      pContext->DrawText(4,(nuiSize)GetHeight(), mFPSText);
    }

  }

  return res;
}

void nuiWin::OnPaint()
{
  nuiMainWindow::OnPaint();
  if (mShowFPS)
    nuiTopLevel::Invalidate();
}

void nuiWin::CreateScrolledImageWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 300, 300), nglWindow::NoFlag, nuiTR("Scrolled Image..."));
  mpManager->AddChild(pWin);
  nuiScrollView* pScrollView = new nuiScrollView();
  pWin->AddChild(pScrollView);
  //nuiImage* pImage = new nuiImage(_T("rsrc:/nui.png"));
  //nuiImage* pImage = new nuiImage(_T("rsrc:/testalacon.png"));
  nuiImage* pImage = new nuiImage(_T("rsrc:/sampler_edit_sample_load_dwn.png"));
  pImage->GetTexture()->SetMinFilter(GL_NEAREST);
  pImage->GetTexture()->SetMagFilter(GL_NEAREST);
  pScrollView->AddChild(pImage);
  pImage->SetPosition(nuiTopLeft);
  pImage->SetFillRule(nuiTopLeft);
}

void nuiWin::CreateScrolledImageWindow2(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 200, 200), nglWindow::NoFlag, nuiTR("Scrolled Image (2)..."));
  mpManager->AddChild(pWin);
  nuiScrollView* pScrollView = new nuiScrollView();
  pWin->AddChild(pScrollView);
  nuiImage* pImage = new nuiImage(_T("rsrc:/jpeg.jpg"));
  pScrollView->AddChild(pImage);
  pImage->SetFillRule(nuiTopLeft);
}


#ifdef USE_SWF
void nuiWin::CreateSWFTest1Window(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("SWF Movie..."));
  mpManager->AddChild(pWin);
  nuiSimpleContainer* pContainer = new nuiPositioner();
  pWin->AddChild(pContainer);

  nuiSWF* pSWF = new nuiSWF();
  pContainer->AddChild(pSWF);
  //nglString movie(_T("rsrc:/3d grid spinner[3].swf"));
  //nglString movie(_T("rsrc:/girlelevenx.swf"));
  //nglString movie(_T("rsrc:/julos2.swf"));
  //nglString movie(_T("rsrc:/julos2.swf"));
  //nglString movie(_T("rsrc:/eye.swf"));
  //nglString movie(_T("rsrc:/header2.swf"));
  //nglString movie(_T("rsrc:/menu.swf"));
  //nglString movie(_T("rsrc:/enconstruction.swf"));
  nglString movie(_T("rsrc:/nui1.swf"));
  //nglString movie(_T("rsrc:/Tweens.swf"));
  //nglString movie(_T("rsrc:/Electric-Field.swf"));
  //nglString movie(_T("rsrc:/ninja1.swf"));
  //nglString movie(_T("rsrc:/clinger.swf"));

  if (pSWF->Load(movie))
  {
    nuiRect r = pSWF->GetIdealRect();
    pWin->SetUserSize(r.GetWidth(), r.GetHeight());
    pSWF->Play();
  }
  else
  {
    delete pSWF;
    nglString text;
    text.Format(_T("Unable to load flash movie '%ls'"), movie.GetChars());
    pContainer->AddChild(new nuiLabel(text));
  }
}
#endif


void nuiWin::CreateShapeWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("nuiShapes..."));
  mpManager->AddChild(pWin);

  nuiShape* pShape = new nuiShape();

  pShape->AddContour();
  pShape->LineTo(nuiPoint(10, 10));
  pShape->LineTo(nuiPoint(60, 60));
  pShape->LineTo(nuiPoint(90, 60));
  pShape->CloseContour();

  pShape->AddContour();
  pShape->LineTo(nuiPoint(100, 120));
  pShape->ArcTo(200, 200, 10, 10, 15, false, false);
  pShape->CloseContour();

  pShape->AddCircle(200, 30, 25);

  pShape->AddEllipse(200, 90, 10, 25);

  pShape->AddRect(nuiRect(10, 100, 75, 60));
  pShape->AddRoundRect(nuiRect(20, 110, 75, 60), 10, false);
  pShape->AddRoundRect(nuiRect(30, 120, 75, 60), 10, true);


  pShape->AddArc(200, 100, 50, 50, 225, -45, 0);

  nuiShapeView* pShapeView = new nuiShapeView(pShape);
  nuiScrollView* pSV = new nuiScrollView();
  pWin->AddChild(pSV);
  pSV->AddChild(pShapeView);

  pShapeView->SetLineWidth(4.0f);
  pShapeView->SetShapeMode(eStrokeAndFillShape);
  //  pShapeView->SetShapeMode(eFillShape);
  //  pShapeView->SetShapeMode(eStrokeShape);
  pShapeView->SetColor(eShapeStroke, nuiColor(_T("darkblue")));
  pShapeView->SetColor(eShapeFill, nuiColor(0.0f, 0.0f, 0.5f, 0.5f));
}

void nuiWin::TogglePartialRedraw(const nuiEvent& rEvent)
{
  nuiTopLevel::EnablePartialRedraw(!nuiTopLevel::IsPartialRedrawEnabled());
  if (nuiTopLevel::IsPartialRedrawEnabled())
  {
    NGL_OUT(_T("Partial Redraw Enabled\n"));
  }
  else
  {
    NGL_OUT(_T("Partial Redraw Disabled\n"));
  }
}

void nuiWin::CreateWrappedLabelWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Wrapped Label"));
  mpManager->AddChild(pWindow);

  nuiVBox* pBox = new nuiVBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  pWindow->AddChild(pBox);
  nuiLabel* pNormalLabel = new nuiLabel(nuiTR("Normal Label that is just cut out at the end of the window"));
  pBox->AddCell(pNormalLabel);
  nuiLabel* pCutLabel = new nuiLabel(nuiTR("Label that is cut in the middle of the line if there is not enough space."));
  pBox->AddCell(pCutLabel);
  pCutLabel->UseEllipsis(true);

  nuiLabel* pLabel = new nuiLabel(nuiTR("This text should be long enough for me to test wrapping of the label. So let's add some stupid stuff as I really can't think of anything clever to declare right now."));
  if (0)
  {
    nuiScrollView* pSV = new nuiScrollView();
    pBox->AddCell(pSV);
    pSV->AddChild(pLabel);
  }
  else
  {
    pBox->AddCell(pLabel);
  }
  pLabel->SetWrapping(true);
  pLabel->SetPosition(nuiTopLeft);
}


#define CIRCLE_COUNT 6
class nuiWhizzAnim : public nuiWidget
{
public:
  nuiWhizzAnim()
    : nuiWidget()
  {
    for (uint i = 0; i < 3; i++)
    {
      mLevels[i] = 1;
      mSizes[i] = 10 / ((float)i+1);
    }
    mAmplitude = 0.3f;
    mPhase = 0;

    StartAutoDraw();
  }

  virtual ~nuiWhizzAnim()
  {
  }

  bool SetRect(const nuiRect& rRect)
  {
    for (uint i = 0; i < 3; i++)
    {
      mLevels[i] = 1;
      mSizes[i] = MIN(rRect.GetWidth(), rRect.GetHeight()) /(CIRCLE_COUNT * ((float)i+1));
    }
    return nuiWidget::SetRect(rRect);
  }

  void PrepareShape(nuiShape* pShape, float* pSizes, float* pLevels, float coef)
  {
    float centerX = mRect.GetWidth()/2;
    float centerY = mRect.GetHeight()/2;


    pShape->AddCircle(centerX, centerY, pSizes[0] * mLevels[0] - coef);

    const float cangle = (float)(M_PI * 2) / (float)CIRCLE_COUNT;

    const float m1 = (pSizes[0] + pSizes[1] * .5f) * mLevels[1];

    for (uint i = 0; i < CIRCLE_COUNT; i++)
    {
      float angle = cangle * (float)i;
      float X = centerX + m1 * sin(angle);
      float Y = centerY + m1 * cos(angle);

      pShape->AddCircle(X, Y, mSizes[1] * mLevels[1] - coef);

      const float m2 = (pSizes[0] + pSizes[1] + pSizes[2]) * mLevels[2];

      {
        angle += cangle / 2;
        float X = centerX + m2 * sin(angle);
        float Y = centerY + m2 * cos(angle);

        pShape->AddCircle(X, Y, pSizes[2] * mLevels[2] - coef);
      }

    }
  }

  bool Draw(nuiDrawContext* pContext)
  {

    mLevels[0] = 1.0f + mAmplitude * (1.0f + sin(mPhase));
    mLevels[1] = 1.0f + mAmplitude * (1.0f + cos(mPhase * 2));
    mLevels[2] = 1.0f + mAmplitude * (1.0f + sin(mPhase * 3 + (float)M_PI));

    mPhase += 0.1f;

    nuiShape* pShape1 = new nuiShape();
    nuiShape* pShape2 = new nuiShape();
    PrepareShape(pShape1, mSizes, mLevels, 0.0f);
    PrepareShape(pShape2, mSizes, mLevels, -3.0f);

    pShape1->SetWinding(nuiShape::eNonZero);

    pContext->SetBlendFunc(nuiBlendTransp);
    pContext->EnableBlending(true);

    pContext->SetFillColor(nuiColor(0.0f ,0.0f, 0.0f, .5f));
    pContext->DrawShape(pShape2, eFillShape);

    pContext->SetFillColor(nuiColor(1.0f ,1.0f, 1.0f, 1.0f));
    pContext->SetStrokeColor(nuiColor(1.0f ,1.0f, 1.0f, 1.0f));

    nuiTessellator tess(pShape1);
    tess.SetFill(false);
    tess.SetOutLine(true);
    nuiRenderObject* pObject = tess.Generate(0.2f);
    pContext->DrawObject(*pObject);

    delete pObject;
    delete pShape1;
    delete pShape2;

    return true;
  }

  nuiRect CalcIdealSize()
  {
    return nuiRect(0,0, 256, 256);
  }

private:
  float mLevels[3];
  float mSizes[3];
  float mAmplitude;
  float mPhase;
};

void nuiWin::CreateWhizzAnimWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Whizzy Anim"));
  mpManager->AddChild(pWindow);

  nuiWhizzAnim* pAnim = new nuiWhizzAnim();
  pWindow->AddChild(pAnim);
}

class TestLabel : public nuiWidget
{
public:
  TestLabel(const nglString& rString, const nglString& fontdesc)
    : nuiWidget()
  {
    mString = rString;
    if (fontdesc.IsEmpty())
    {
      mpFont = nuiFont::GetFont(50);

    }
    else
    {
      nglString font(fontdesc);
      nuiXML XML(_T("FontDesc"));
      nglIMemory memory(font.GetChars(),font.GetLength());
      XML.Load(memory);
      mpFont = nuiFont::GetFont(&XML);
    }

    nuiFontInfo fontinfo;
    mpFont->GetInfo(fontinfo);
    fontinfo.Dump();
  }

  virtual ~TestLabel()
  {
  }

  bool Draw(nuiDrawContext* pContext)
  {
    // Draw the background:
    nuiRect r(GetRect().Size());

    pContext->SetBlendFunc(nuiBlendTransp);
    pContext->EnableBlending(true);
    pContext->SetFillColor(nuiColor(.5f, .5f, .5f, .5f));
    pContext->DrawRect(r, eFillShape);

    // Draw the text layout construction lines:
    nuiFontInfo fontinfo;
    mpFont->GetInfo(fontinfo);

    nuiSize y = r.GetHeight()/2;
    nuiSize w = r.GetWidth();

    // Draw Base Line:
    pContext->SetStrokeColor(nuiColor(0,0,192,192));
    pContext->DrawLine(0, y, w, y);

    /*
    // Draw Ascender Line:
    pContext->SetStrokeColor(nuiColor(128,128,192,192));
    pContext->DrawLine(0, y - fontinfo.Ascender, w, y - fontinfo.Ascender);
    // Draw Descender Line:
    pContext->SetStrokeColor(nuiColor(128,192,128,192));
    pContext->DrawLine(0, y - fontinfo.Descender, w, y - fontinfo.Descender);
    */

    // Draw the text:
    nuiTextLayout layout(mpFont, nuiHorizontal);
    layout.Layout(mString);

    nuiRect lrect = layout.GetRect();
    lrect.Move(0,y - fontinfo.Ascender);
    pContext->SetFillColor(nuiColor(255, 255, 255, 128));
    pContext->DrawRect(lrect, eFillShape);


    pContext->SetTextColor(nuiColor(0,0,0));
    pContext->SetFont(mpFont);
    pContext->DrawText(0, y, layout);

    /*
    nuiSize w = info.AdvanceX;
    //    nuiSize h = finfo.AdvanceMaxH;
    nuiSize x = rGlyph.X + info.BearingX;
    nuiSize y = rGlyph.Y - finfo.Ascender;
    nuiSize h = finfo.Ascender - finfo.Descender;
    */
    /*
    nuiRect idealrect(mIdealRect);
    idealrect.Move(0, y - fontinfo.Ascender);
    pContext->SetStrokeColor(nuiColor(255,0,0,128));
    pContext->DrawRect(idealrect, eStrokeShape);

    nuiRect bbox(fontinfo.BBoxMinX, fontinfo.BBoxMinY, fontinfo.BBoxMaxX, fontinfo.BBoxMaxY, false);
    bbox.Move(0, y - (fontinfo.Ascender + fontinfo.Descender));
    pContext->SetStrokeColor(nuiColor(0,255,0,128));
    pContext->DrawRect(bbox, eStrokeShape);

    // Draw Advances Line:
    pContext->SetStrokeColor(nuiColor(255,0,0,192));
    pContext->DrawLine(fontinfo.AdvanceMaxW * 2, y - fontinfo.Ascender, fontinfo.AdvanceMaxW * 3, y - fontinfo.Ascender);
    pContext->DrawLine(fontinfo.AdvanceMaxW * 3, y - fontinfo.Ascender, fontinfo.AdvanceMaxW * 3, y - fontinfo.Ascender + fontinfo.AdvanceMaxH);
    */

    return true;
  }

  nuiRect CalcIdealSize()
  {
    nuiFontInfo fontinfo;
    mpFont->GetInfo(fontinfo);

    nuiTextLayout layout(mpFont, nuiHorizontal);
    layout.Layout(mString);

    nuiSize height = fontinfo.Ascender - fontinfo.Descender;
    nuiSize width = layout.GetRect().GetWidth();

    nuiRect rect(0.0f,0.0f, width, height);

    return rect;
  }

private:
  nglString mString;
  nuiFont* mpFont;
};

void nuiWin::CreateTextLayoutWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Text Layout"));
  mpManager->AddChild(pWindow);
  //new TestLabel(new nuiScrollView(pWindow, true, false), _T("Test text: yHwWmMfFgGi .;[]()_-|#*%$ 1234567890"));
  nuiScrollView* pScroller = new nuiScrollView(true, false);
  pWindow->AddChild(pScroller);
  nuiVBox* pBox = new nuiVBox(0);
  pScroller->AddChild(pBox);

#define TEST_TEXT nuiTR("['AZERTY_azertyig.DAT']")
  //#define TEST_TEXT nuiTR("Test text: yHwWmMfFgGi .;[]()_-|#*%$ 1234567890")

  pBox->AddCell(new TestLabel(TEST_TEXT, _T("<nuiFont Size=\"12\" Source=\"../Data/Vera.ttf\"/>")));
  pBox->AddCell(new TestLabel(TEST_TEXT, _T("<nuiFont Size=\"12\" Source=\"../Data/VeraMono.ttf\"/>")));
}


void nuiWin::CreateFontSubstitutionWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Fonts"));
  mpManager->AddChild(pWindow);
  //new TestLabel(new nuiScrollView(pWindow, true, false), nuiTR("Test text: yHwWmMfFgGi .;[]()_-|#*%$ 1234567890"));
  nuiScrollView* pScroller = new nuiScrollView(true, true);
  pWindow->AddChild(pScroller);
  nuiText* pText = new nuiText(_T(""));
  pScroller->AddChild(pText);
  pText->SetFont(nuiTheme::Default);

  nuiFontManager& rFontManager(nuiFontManager::GetManager());
  nglString txt(nuiTR("This string will contain a chinese glyph: '_'\n\n"));
  pText->Print(txt.GetChars());

  txt.Replace('_',(nglChar)0x4eba);
  pText->Print(_T("%ls"), txt.GetChars());

  pText->Print(_T("here we are!\n"));

  rEvent.Cancel();
}

void nuiWin::SaveFontDB(const nuiEvent& rEvent)
{
  nglPath p(_T("nuiFonts.db3"));
  nglOFile strm(p, eOFileCreate);
  nuiFontManager& rManager(nuiFontManager::GetManager());
  rManager.Save(strm);
}

void nuiWin::LoadFontDB(const nuiEvent& rEvent)
{
  nglPath p(_T("nuiFonts.db3"));
  nglIFile strm(p);
  nuiFontManager::LoadManager(strm);
}


bool SlotFunction1(nuiText* pText)
{
  pText->Print(_T("Message received on slot1!\n"));

  return true;
}

bool SlotFunction2(nuiText* pText, const nglChar* pStr)
{
  pText->Print(pStr);

  return true;
}

class DelegateTest : public nuiWidget
{
public:
  DelegateTest(const nglString& rName)
  {
    SetObjectName(rName);
  }

  virtual void PrintName(nuiText* pText)
  {
    nglString str;
    str.Add("## IN PARENT ## This: ").Add(this).Add(" / ").Add(GetObjectName()).AddNewLine();
    pText->AddText(str);
  }

};

class DelegateTestChild : public DelegateTest
{
public:
  DelegateTestChild(const nglString& rName)
    : DelegateTest(rName)
  {
  }

  virtual void PrintName(nuiText* pText)
  {
    nglString str;
    str.Add("## IN CHILD  ## This: ").Add(this).Add(" / ").Add(GetObjectName()).AddNewLine();
    pText->AddText(str);
  }

};

void nuiWin::CreateSignalsAndSlotsTestWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Signals and slots"));
  mpManager->AddChild(pWindow);
  nuiScrollView* pScroller = new nuiScrollView(true, true);
  pWindow->AddChild(pScroller);
  nuiText* pText = new nuiText(_T(""));
  pScroller->AddChild(pText);
  pText->SetFont(nuiTheme::Default);

  DelegateTest t1(nglString("test1"));
  DelegateTestChild t2(nglString("test2"));

  pText->AddText(nuiTR("Direct access:\n"));
  t1.PrintName(pText);
  t2.PrintName(pText);

  pText->AddText(nuiTR("Delegate access:\n"));
  nuiFastDelegate1<nuiText*> delegate1(&t1, &DelegateTest::PrintName);
  nuiFastDelegate1<nuiText*> delegate2(&t2, &DelegateTest::PrintName);

  delegate1(pText);
  delegate2(pText);

  pText->AddText(nuiTR("Modified delegate access:\n"));
  delegate1.SetThis(&t2);
  delegate2.SetThis(&t1);

  delegate1(pText);
  delegate2(pText);

  pText->AddText(nuiTR("Modified indirect delegate access (thru nuiObject*):\n"));
  nuiObject* pObject1 = &t1;
  nuiObject* pObject2 = &t2;
  delegate1.SetThis(pObject1);
  delegate2.SetThis(pObject2);

  delegate1(pText);
  delegate2(pText);

  pText->AddText(nuiTR("Modified indirect delegate access (thru nuiWidget*):\n"));
  nuiWidget* pWidget1 = &t1;
  nuiWidget* pWidget2 = &t2;
  delegate1.SetThis(pWidget2);
  delegate2.SetThis(pWidget1);

  delegate1(pText);
  delegate2(pText);
}


void nuiWin::CreateFrameWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Frame View test"));
  mpManager->AddChild(pWindow);

  nuiFrame* pFrame = new nuiFrame(_T("TestFrame1"), nuiTexture::GetTexture(nglPath(_T("rsrc:/frame1.png"))), nuiRect(7, 5, 46, 45));
  nuiFrameView* pFrameView = new nuiFrameView(pFrame);
  pWindow->AddChild(pFrameView);
  pFrameView->SetBorder(16, 16);
  pFrameView->AddChild(new nuiImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/toucan.png"))), true));

  nuiLabel* pText = new nuiLabel(nuiTR("This label should\nbe inside\na bitmaped frame"), nuiFont::GetFont(30.0f));
  pFrameView->AddChild(pText);
  pText->SetPosition(nuiCenter);
  pText->SetColor(eNormalTextFg, nuiColor(128, 128, 182));
}

void nuiWin::CreateFrameWindow2(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Frame View test 2"));
  mpManager->AddChild(pWindow);

  nuiFrame* pFrame = new nuiFrame(_T("TestFrame2"), nuiTexture::GetTexture(nglPath(_T("rsrc:/frame1.png"))), nuiRect(7, 5, 46, 45));
  nuiLabel* pText = new nuiLabel(nuiTR("Label in a frame"), nuiFont::GetFont(35.0f));
  pText->SetDecoration(pFrame);
  pText->SetPosition(nuiCenter);
  pText->SetColor(eNormalTextFg, nuiColor(0, 0, 0, 128));

  pText->SetPosition(nuiCenter);
  pWindow->AddChild(pText);
}

void nuiWin::CreateFrameWindow3(const nuiEvent& rEvent)
{
  nuiFrame* pFrame0 = new nuiFrame(_T("TestFrame3"), nuiTexture::GetTexture(nglPath(_T("rsrc:/fondtrack.png"))), nuiRect(7, 7, 35, 53));
  nuiFrame* pFrame1 = new nuiFrame(_T("TestFrame3"), nuiTexture::GetTexture(nglPath(_T("rsrc:/fondtrack.png"))), nuiRect(7, 7, 35, 53));
  pFrame0->EnableBorder(false);
  pFrame1->EnableBorder(true);
  nuiFrame* pFrames[2] = { pFrame0, pFrame1 };

  nuiDecorationMode modes[3] = {eDecorationBorder, eDecorationClientOnly, eDecorationOverdraw};
  nglString modesStr[3] = {_T("Border"), _T("Client"), _T("OvrDrw")};
  nglString borderStr[2] = {_T("false"), _T("true")};
  nuiWindow* windows[6];
  nuiSimpleContainer* containers[6];

  for (int i = 0; i < 3; i++)
  {
    for (int e = 0; e < 2; e++)
    {
      windows[2 * i + e] = new nuiWindow(nuiRect(50 + 200 * e, 50 + 150 * i, 160, 120), nglWindow::NoFlag, nuiTR("Frame View test 3"));
      mpManager->AddChild(windows[2 * i + e]);
      nuiLabel* pLabel = new nuiLabel(_T("Decoration mode\n= ") + modesStr[i] + _T("\nEnable Border\n= ") + borderStr[e]);
      nuiSimpleContainer * pCont = new nuiSimpleContainer();
      pCont->AddChild(pLabel);
      //pCont->SetPosition(nuiCenter);
      pCont->SetDecoration(pFrames[e], modes[i]);
      pCont->SetBorder(4, 4);
      //pCont->SetDebug(1);

      windows[2 * i + e]->AddChild(pCont);
      pLabel->SetColor(eNormalTextFg, nuiColor(0, 0, 0));
      pLabel->SetPosition(nuiCenter);
      //pLabel->SetDecoration(pFrames[e], modes[i]);
      pLabel->SetBackground(true);
      pLabel->SetBackgroundColor(nuiColor(192, 192, 0, 128));
      //pLabel->SetBorder(2, 2);
    }
  }
}

class DragPane : public nuiWidget
{
public:
  DragPane()
    : nuiWidget(),
    mDnd(false)
  {
  }

  bool Draw(nuiDrawContext* pContext)
  {
    nuiShape shape;
    shape.AddRoundRect(mRect.Size(), 10.f);
    if (mDnd)
    {
      pContext->DrawShape(&shape, eFillShape);
    }
    else
    {
      pContext->DrawShape(&shape, eStrokeShape);
    }
    return true;
  }

  virtual nglDropEffect OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y)
  {
    mDnd = true;
    Invalidate();
    return eDropEffectCopy;
  }

  virtual void OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
  {
    mDnd = false;
    Invalidate();
  }

  virtual void OnDropLeave()
  {
    mDnd = false;
    Invalidate();
  }

  bool mDnd;
};

void nuiWin::CreateDragZone(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Drag Zone test"));
  mpManager->AddChild(pWindow);
  DragPane* pDragPane = new DragPane();
  pWindow->AddChild(pDragPane);
}


class ClippingTest : public nuiWidget
{
public:
  ClippingTest()
    : nuiWidget()
  {
    SetUserRect(nuiRect(0, 0 , 300, 300));
  }

  bool Draw(nuiDrawContext* pContext)
  {
    nuiRect r(GetRect().Size());
    int32 w = r.GetWidth();
    int32 h = r.GetHeight();

    pContext->SetFillColor(nuiColor(_T("red")));
    pContext->SetStrokeColor(nuiColor(_T("green")));

    //r.Move(.5, .5);
    pContext->DrawRect(r, eStrokeShape);

    //pContext->AddBreakPoint();

//    pContext->PushClipping();
//    pContext->Clip(nuiRect(0, 0, w/2, h/2));
//    pContext->DrawRect(r, eStrokeAndFillShape);
//    pContext->PopClipping();
//
//    pContext->PushClipping();
//    pContext->Clip(nuiRect(w/2, h/2, w/2, h/2));
//    pContext->DrawRect(r, eStrokeAndFillShape);
//    pContext->PopClipping();

    return true;
  }
};

void nuiWin::CreateClippingTest(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Clipping test"));
  mpManager->AddChild(pWindow);
  pWindow->AddChild(new ClippingTest());
}

// nuiSurface test
class SurfaceTest : public nuiWidget
{
public:
  SurfaceTest()
  {
    SetUserSize(320, 200);
    SetPosition(nuiCenter);

    //    nglString surfaceName;
    //    surfaceName.CFormat(_T("%p"), this);
    //    mpSurface = nuiSurface::CreateSurface(surfaceName, 256, 256);
    //    mpSurfaceTexture = nuiTexture::GetTexture(mpSurface, false);
    //

  }

  bool Draw(nuiDrawContext* pContext)
  {
    nuiRect r(GetRect().Size());
    int32 w = r.GetWidth();
    int32 h = r.GetHeight();

    pContext->SetFillColor(nuiColor(_T("red")));
    pContext->SetStrokeColor(nuiColor(_T("green")));

    //r.Move(.5, .5);
    pContext->DrawRect(r, eStrokeAndFillShape);

    //pContext->SetClearColor(nuiColor(1, 1, 1));
    //pContext->Clear();
//    pContext->SetFillColor(nuiColor(1, 1, 1));
//    pContext->SetStrokeColor(nuiColor(1, 0, 0));
//    pContext->DrawRect(GetRect().Size(), eStrokeAndFillShape);
    pContext->SetFont(nuiFont::GetFont(50));
    pContext->SetBlendFunc(nuiBlendTransp);
    pContext->SetTextColor(nuiColor(0.0f, 1.0f, 0.0f));
    pContext->DrawText(10, 50, _T("test string"));

    return true;
  }

  nuiRect CalcIdealSize()
  {
    return nuiRect(0, 0, 200, 200);
  }

  nuiSurface* mpSurface;
  nuiTexture* mpSurfaceTexture;
};


void nuiWin::CreateSurfaceTest(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Surface test"));
  mpManager->AddChild(pWindow);
  pWindow->AddChild(new SurfaceTest());
}




class MyAudioStreamHandler
{
public:
  MyAudioStreamHandler(double sr)
  {
    mPhase = 0.0;
    mIncr = 440.0 * (2 * M_PI / sr);
    mVolume = 1.0f;
    mVolumeMod = 0.99993f;
  }

  void Process(const std::vector<const float*>& rInputBuffers, const std::vector<float*>& rOutputBuffers, uint32 SampleFrames)
  {

    for (uint32 i = 0; i < SampleFrames; i++)
    {
      if (rOutputBuffers[0] && rOutputBuffers[1])
      {
        rOutputBuffers[0][i] = mVolume * ((float)(sin(mPhase)));
        rOutputBuffers[1][i] = mVolume * ((mPhase < M_PI) ? -.3f : .3f); // a square wave is always much louder than a sinus so let's lower its volume a bit...
        mPhase += mIncr;

        if (mPhase >= 2 * M_PI)
          mPhase -= 2 * M_PI;

        mVolume *= mVolumeMod;
      }
      else
      {
        mVolume = 0.0f;
      }
    }

  }

  float GetVolume()
  {
    return mVolume;
  }

  bool CanContinue()
  {
    //NGL_OUT(_T("volume %f\n"), mVolume);
    return mVolume > 0.0003;
  }

private:
  double mPhase;
  double mIncr;
  float mVolume;
  float mVolumeMod;
};



void nuiWin::CreateAudioWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10.0f,10.0f, 300.f,400.f), nuiWindow::DecoratedBackground ,(nuiTR("Audio Hardware")));
  mpManager->AddChild(pWindow);
  nuiVBox* pBox = new nuiVBox();
  pWindow->AddChild(pBox);
  nuiLabel* pTitle = new nuiLabel(nuiTR("Enum audio devices on this machine:\n"));
  pBox->AddCell(pTitle);
  nuiComboBox* pCombo = new nuiComboBox();
  pBox->AddCell(pCombo);
  nuiScrollView* pScroll = new nuiScrollView();
  nuiText* pText = new nuiText();
  pScroll->AddChild(pText);
  pBox->AddCell(pScroll);
  pCombo->SetToken(new nuiToken<nuiText*>(pText));

  mWinSink.Connect(pCombo->SelectionChanged, &nuiWin::AudioDeviceSelected, pCombo);
  mWinSink.Connect(pCombo->Trashed, &nuiWin::AudioComboTrashed, pCombo);

  nuiTreeNode* pTree = new nuiTreeNode(nuiTR("AudioDevices"));

  nuiAudioDeviceManager::Get().Update();
  uint32 devs = nuiAudioDeviceManager::Get().GetDeviceCount();
  for (uint32 i = 0; i < devs; i++)
  {
    nglString devname = nuiAudioDeviceManager::Get().GetDeviceName(i);
    nuiLabel* pLabel = new nuiLabel(devname + _T(" (") + _T(")"));
    pLabel->SetToken(new nuiToken<nuiAudioDevice*>(nuiAudioDeviceManager::Get().GetDevice(i)));
    pTree->AddChild(new nuiTreeNode(pLabel));
  }

  pCombo->SetTree(pTree);
}

void nuiWin::AudioComboTrashed(const nuiEvent& rEvent)
{
  nuiComboBox* pCombo = (nuiComboBox*) rEvent.mpUser;

  nuiAudioDevice* pDev;

  const nuiTreeNode* pTree = pCombo->GetChoices();
  for (uint i = 0; i < pTree->GetChildrenCount(); i++)
  {
    const nuiTreeNode* pChild = dynamic_cast<const nuiTreeNode*>(pTree->GetChild(i));
    bool res = nuiGetTokenValue<nuiAudioDevice*>(pChild->GetElement()->GetToken(), pDev);
    delete pDev;
  }
}

void nuiWin::AudioDeviceSelected(const nuiEvent& rEvent)
{
  nuiComboBox* pCombo = (nuiComboBox*) rEvent.mpUser;
  nuiText* pText;
  bool res = nuiGetTokenValue<nuiText*>(pCombo->GetToken(), pText);
  nuiAudioDevice* pDev;
  nuiWidget* pWidget = pCombo->GetSelectedWidget();
  if (!pWidget)
    return;
  res = nuiGetTokenValue<nuiAudioDevice*>(pWidget->GetToken(), pDev);
  if (!res)
    return;

  nglString str;

  str.Add(nuiTR("Name: ")).Add(pDev->GetName()).AddNewLine();
  str.Add(nuiTR("API: ")).Add(pDev->GetAPIName()).AddNewLine();
  str.Add(nuiTR("Manufacturer: ")).Add(pDev->GetManufacturer()).AddNewLine();

  str.Add(nuiTR("Sample rates:"));
  const std::vector<double>& SampleRates(pDev->GetSampleRates());
  for (uint32 sr = 0; sr < SampleRates.size(); sr++)
    str.Add(_T(" ")).Add(SampleRates[sr]);
  str.AddNewLine();

  str.Add(nuiTR("Buffer sizes:"));
  const std::vector<uint32>& BuffserSizes(pDev->GetBufferSizes());
  for (uint32 sr = 0; sr < BuffserSizes.size(); sr++)
    str.Add(_T(" ")).Add(BuffserSizes[sr]);
  str.AddNewLine();

  // Channels:
  uint32 inchcount = pDev->GetInputChannelCount();
  if (inchcount)
  {
    str.Add(nuiTR("\tInput channels:")).AddNewLine();
    for (uint32 c = 0; c < inchcount; c++)
      str.Add(_T("\t\t")).Add(c).Add(_T(" - ")).Add(pDev->GetInputChannelName(c)).AddNewLine();
  }

  uint32 outchcount = pDev->GetOutputChannelCount();
  if (outchcount)
  {
    str.Add(nuiTR("\tOutput channels:")).AddNewLine();
    for (uint32 c = 0; c < outchcount; c++)
      str.Add(_T("\t\t")).Add(c).Add(_T(" - ")).Add(pDev->GetOutputChannelName(c)).AddNewLine();
  }

  pText->SetText(str);
  NGL_OUT(_T("%ls"), str.GetChars());

  if (!pDev->IsPresent())
    return;

  std::vector<uint32> InChannels;
  std::vector<uint32> OutChannels;
  OutChannels.push_back(0);
  OutChannels.push_back(1);


  MyAudioStreamHandler handler(pDev->GetSampleRates().back());

  res = pDev->Open(InChannels, OutChannels, pDev->GetSampleRates().back(), pDev->GetBufferSizes().back(), nuiMakeDelegate(&handler, &MyAudioStreamHandler::Process));

  if (!res)
  {
    NGL_OUT(nuiTR("nuiAudioDevice Open failed!\n"));
    return;
  }


  while (handler.CanContinue())
  {
    nglThread::Sleep(1);
  }



  pDev->Close();
}







//****************************************************************************************************
//
// AudioFifo test
//
//

class MyAudioTrack : public nuiAudioTrack
{
public:

  MyAudioTrack(double sr)
  {
    mPhase = 0.0;
    mIncr = 440.0 * (2 * M_PI / sr);
    mVolume = 1.0f;
    mVolumeMod = 0.99993f;
  };

  virtual ~MyAudioTrack()
  {

  }

  uint32 ReadSamples(uint32 samplesFrames, std::vector<float*>& rBuffer)
  {

    for (uint32 i = 0; i < samplesFrames; i++)
    {
      if (rBuffer[0] && rBuffer[1])
      {
        rBuffer[0][i] = mVolume * ((float)(sin(mPhase)));
        rBuffer[1][i] = mVolume * ((mPhase < M_PI) ? -.3f : .3f); // a square wave is always much louder than a sinus so let's lower its volume a bit...
        mPhase += mIncr;

        if (mPhase >= 2 * M_PI)
          mPhase -= 2 * M_PI;

        mVolume *= mVolumeMod;
      }
      else
      {
        mVolume = 0.0f;
      }
    }

    return samplesFrames;

  }

  float GetVolume()
  {
    return mVolume;
  }

  bool CanContinue()
  {
    //NGL_OUT(_T("volume %f\n"), mVolume);
    return mVolume > 0.0003;
  }

private:
  double mPhase;
  double mIncr;
  float mVolume;
  float mVolumeMod;

};



void nuiWin::CreateAudioFifoWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("AudioFifo test"));
  mpManager->AddChild(pWindow);
  nuiText* pText = new nuiText();
  nuiScrollView* pScrollView = new nuiScrollView();
  pScrollView->AddChild(pText);
  pWindow->AddChild(pScrollView);

  pText->AddText(nuiTR("You should hear a test sound. Otherwise, check the Console for error messages.\n"));

  // take a loot at CreateAudioWindow above, to see how the device can be asked about the provided sample rates and buffer sizes.
  // here, the values are just hard-coded. We give a 4096 samples buffer size as a parameter : with the machine this program has been written on, Windows's DirectSound can't bear a lower value without messing up the sound
  nuiAudioFifo* myfifo = new nuiAudioFifo(128*1024, 4096, 44100.0f, 2, nglString::Empty, nglString::Empty);
  if (!myfifo->Start())
  {
    //NGL_OUT(_T("error. end of program.\n"));
    delete myfifo;
  }

  MyAudioTrack* mytrack = new MyAudioTrack(44100.0F);

  myfifo->RegisterTrack(mytrack, 44100.0f, 2, 1.0f, false);

  // Here, we stop the audio system once all samples have been given to the audio system.
  // But if bufferingEnabled parameter == true, you have to overload the nuiAudioTrack::ProcessedSamples virtual method, in order to stop the audio system at the right moment
  // (<=> when all samples have been processed from buffer to device output), otherwise, you will not hear the whole sound, it'll be cut before the end...

  while (mytrack->CanContinue())
  {
    nglThread::Sleep(1);
  }

  myfifo->UnregisterTrack(mytrack);

  delete mytrack;

  myfifo->Close();
  delete myfifo;
}

void nuiWin::CreateHTTPWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("HTTP Request"));
  mpManager->AddChild(pWindow);

  nuiVBox* pVBox = new nuiVBox(3);
  nuiHBox* pHBox = new nuiHBox(2);
  pHBox->SetCell(0, new nuiLabel(_T("URL")));
  mpURLText = new nuiEditText(_T("http://home.gna.org/ngl"));
  pHBox->SetCell(1, mpURLText);
  pVBox->SetCell(0, pHBox);

  nuiButton* pButton = new nuiButton(nuiTR("Send"));
  mWinSink.Connect(pButton->Activated, &nuiWin::SendHTTPRequest);
  pVBox->SetCell(1, pButton, nuiRight);

  mpHTTPResponseLabel = new nuiLabel();
  pVBox->SetCell(2, mpHTTPResponseLabel);

  pWindow->AddChild(pVBox);
}

void nuiWin::SendHTTPRequest(const nuiEvent& rEvent)
{
  nglString url = mpURLText->GetText();
  nuiHTTPRequest req(url);
  nuiHTTPResponse* pResponse = req.SendRequest();
  if (pResponse)
  {
    mpHTTPResponseLabel->SetText(pResponse->GetBodyStr());
  }
  else
  {
    mpHTTPResponseLabel->SetText(nuiTR("Unable to connect to ") + url);
  }
  rEvent.Cancel();
}

void nuiWin::CreateHTMLWindow(const nuiEvent& rEvent)
{
  //  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, _T("HTML Test"));
  //  mpManager->AddChild(pWindow);

  nglPath p(_T("rsrc:/test.html"));
  nglIStream* pStream = p.OpenRead();

  nuiHTML html(false);
  html.Load(*pStream);

  delete pStream;
}

void nuiWin::CreateImageResizeWindow(const nuiEvent& rEvent)
{
//  nglPath p(_T("rsrc:/nui.png"));
  nglPath p(_T("rsrc:/jpeg.jpg"));
//  nglPath p(_T("rsrc:/gnu.tga"));
  nglIStream* pStream = p.OpenRead();
  nglImage* pImage = new nglImage(pStream);
  nglImage* pNewImage = new nglImage(*pImage, 320, 320);

  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 320, 320));
  nuiScrollView* pScroll = new nuiScrollView();
  pWin->AddChild(pScroll);
  nuiBox* pBox = new nuiVBox();
  pScroll->AddChild(pBox);
  pBox->AddCell(new nuiImage(pNewImage, true), nuiTopLeft);
  pBox->AddCell(new nuiImage(pImage, true), nuiTopLeft);
  mpManager->AddChild(pWin);
}

void nuiWin::CreateCoverFlowWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 320, 320));
  nuiCoverFlow* pFlow = new nuiCoverFlow();
  pWin->AddChild(pFlow);
  pFlow->AddImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/toucan.png"))));
  pFlow->AddImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/nui.png"))));
  pFlow->AddImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/ngl.png"))));
  pFlow->AddImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/white-xifish.png"))));
  pFlow->AddImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/gnu.png"))));
  pFlow->AddImage(nuiTexture::GetTexture(nglPath(_T("rsrc:/jpeg.jpg"))));
  //  pFlow->SetBackground(nuiColor(255, 255, 255, 0));
  pFlow->SelectImage(2);
  pFlow->SetBackgroundColor(nuiColor(0, 0, 0, 255));
  mpManager->AddChild(pWin);
}

void nuiWin::CreateUnicodeTextWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 320, 320));
  nuiScrollView* pView = new nuiScrollView();
  pWin->AddChild(pView);
  nuiText* pText = new nuiText();
  pView->AddChild(pText);
  mpManager->AddChild(pWin);

  //nglPath path(_T("rsrc:/font_tests/test-japanese.txt"));
  nglPath path(_T("rsrc:/GLASS.txt"));
  nglIStream* pFile = path.OpenRead();
  if (!pFile)
    return;

  pFile->SetTextEncoding(eUTF8);
  nglString text;

  bool r = pFile->ReadText(text);
  delete pFile;

  if (!r)
    return;

  nuiTextRangeList Ranges;
  bool res = nuiSplitText(text, Ranges, nuiST_ScriptChange);
  if (!res)
  {
    NGL_OUT(_T("Error while splitting the text\n"));
    return;
  }

  nuiTextRangeList::iterator it = Ranges.begin();
  nuiTextRangeList::iterator end = Ranges.end();

  uint32 i = 0;
  uint32 current = 0;
  uint32 len = 0;
  while (it != end)
  {
    const nuiTextRange& rRange(*it);

    len = rRange.mLength;

    nglString str;
    str.CFormat(_T("Text range %4d\t[%4d to %4d - %3d glyphs]\t%ls - %ls - %ls'\n"),
            i, current, current + len, len,
            rRange.mBlank?_T("blank"):_T("     "),
            rRange.mDirection?_T("RtL"):_T("LtR"),
            nuiGetUnicodeScriptName(rRange.mScript).GetChars()
    );

    NGL_OUT(str.GetChars());
    pText->AddText(str);

    current += len;

    i++;
    ++it;
  }

  //pText->AddText(text);
}


#include "nuiMemoryPool.h"

struct Test
{
  Test() { NGL_OUT(_T("Test()")); }
  ~Test() { NGL_OUT(_T("~Test()")); }
};


void nuiWin::CreateResourceWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Native Resources"));
  mpManager->AddChild(pWindow);
  nuiText* pText = new nuiText();
  nuiScrollView* pScrollView = new nuiScrollView();
  pScrollView->AddChild(pText);
  pWindow->AddChild(pScrollView);

  pText->AddText(nuiTR("Enum native resources on this executable:\n"));

  std::list<nglPath> resources;
  nglPath p(_T("rsrc:/"));
  p.GetChildren(&resources);

  std::list<nglPath>::iterator it = resources.begin();
  std::list<nglPath>::iterator end = resources.end();
  while (it != end)
  {
    pText->AddText((*it).GetPathName());
    pText->AddText(_T("\n"));

    ++it;
  }

  nglString str;
  str.CFormat(_T("Found %d resources\n"), resources.size());
  pText->AddText(str);
}


#include "nuiVertex.h"
#include "nuiPixelBlender.h"
#include "nuiRasterizer.h"

#define __w 256
#define __h 256

class SoftwareWindow : public nuiWindow
{
public:
  SoftwareWindow(const nuiRect& rRect, nuiWindowFlags Flags=NoFlag, nglString Title=nglString((_T("nuiWindow"))))
    : nuiWindow(rRect, Flags, Title)
  {
    mpRasterizer = new nuiRasterizer(__w, __h);

    nglImageInfo info(false);
    info.mBufferFormat = eImageFormatRaw;  ///< Buffer data format can be raw (user accessible) or proprietary (opaque to user, such as S3TC/DXTC)
    info.mPixelFormat = eImagePixelRGBA;   ///< Pixel components and respective components bit resolution
    info.mWidth = __w;                        ///< Image width in pixels (0 if \a mpBuffer is NULL)
    info.mHeight = __h;                       ///< Image height in pixels (0 if \a mpBuffer is NULL)
    info.mBitDepth = 32;                     ///< Pixel bit depth (sum of components bit resolution, 0 if \a mpBuffer is NULL)
    info.mBytesPerPixel = 4;                ///< Pixel allocation size in bytes (>= pixel bit depth, 0 if \a mpBuffer is NULL)
    info.mBytesPerLine = __w * info.mBytesPerPixel;                 ///< Pixel row allocation size in bytes (>= pixel size * image width, 0 if \a mpBuffer is NULL)
    info.mpBuffer = (char*)mpRasterizer->GetBuffer();                      ///< Buffer data

    nuiScrollView* pSV = new nuiScrollView();
    AddChild(pSV);

    nuiVBox* pBox = new nuiVBox();
    pSV->AddChild(pBox);

    TestShape(pBox, info);
    TestRandomTriangles(pBox, info);
    Test1(pBox, info);
    Test2(pBox, info);
    Test3(pBox, info);
  }

  virtual ~SoftwareWindow()
  {
    delete mpRasterizer;
    if (mpTex)
      mpTex->Release();
  }

private:

  void Test1(nuiBox* pBox, nglImageInfo& info)
  {
    memset((char*)mpRasterizer->GetBuffer(), 0x7f, __h * __w * 4);
    mpRasterizer->SetClipRect(32, 32, 256 - 32, 256 - 32);
    mpRasterizer->ClearColor(NUI_RGBA(0, 0, 128, 255));

    mpRasterizer->SetClipRect(0, 0, __w, __h);

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(10, 10, col);
      nuiVertex_Solid v1(100, 10, col);
      nuiVertex_Solid v2(10, 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v1, v2);
    }

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(__w - 100, 10, col);
      nuiVertex_Solid v1(__w - 10, 10, col);
      nuiVertex_Solid v2(__w - 10, 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(v0, v1, v2);
    }

    {
      nuiVertex_Gouraud v0(10, __h - 10, nuiColor(255, 0, 0, 128));
      nuiVertex_Gouraud v1(100, __h - 10, nuiColor(0, 255, 0, 128));
      nuiVertex_Gouraud v2(10, __h - 100, nuiColor(0, 0, 255, 128));
      mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(v0, v1, v2);
    }

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(__w - 100, __h - 10, col);
      nuiVertex_Solid v1(__w - 10, __h - 10, col);
      nuiVertex_Solid v2(__w - 10, __h - 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Add32>(v0, v1, v2);
    }

    {
      // Draw a solid rectangle:
      uint32 col = NUI_RGBA(255, 255, 0, 128);
      nuiVertex_Solid v0(50, 50, col);
      nuiVertex_Solid v1(__w - 50, 50, col);
      nuiVertex_Solid v2(__w - 50, __h - 50, col);
      nuiVertex_Solid v3(50, __h - 50, col);
      mpRasterizer->DrawRectangle<nuiPixelBlender_TranspAdd>(v0, v1, v2, v3);
    }

    {
      // Draw a rectangle:
      nuiVertex_Gouraud v0(100, 10, nuiColor(255, 0, 0, 128));
      nuiVertex_Gouraud v1(__w - 100, 10, nuiColor(0, 255, 0, 128));
      nuiVertex_Gouraud v2(__w - 100, __h - 10, nuiColor(0, 0, 255, 128));
      nuiVertex_Gouraud v3(100, __h - 10, nuiColor(255, 255, 255, 64));
      mpRasterizer->DrawRectangle<nuiPixelBlender_Transp>(v0, v1, v2, v3);
    }

    {
      // Draw a rectangle as to triangles:
      nuiVertex_Gouraud v0(10, 100, nuiColor(255, 0, 0, 128));
      nuiVertex_Gouraud v1(__w - 10, 100, nuiColor(0, 255, 0, 128));
      nuiVertex_Gouraud v2(__w - 10, __h - 100, nuiColor(0, 0, 255, 128));
      nuiVertex_Gouraud v3(10, __h - 100, nuiColor(255, 255, 255, 64));
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v1, v2);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v2, v3);
    }

    pBox->AddCell(new nuiLabel(nuiTR("General test")));
    mpRasterizer->ReverseBufferABGR();
    mpTex = nuiTexture::GetTexture(info);
    mpTex->SetSource(_T("soft_image3"));
    nuiImage* pImage = new nuiImage(mpTex->GetSource());
    pBox->AddCell(pImage);
  }

  void Test2(nuiBox* pBox, nglImageInfo& info)
  {
    mpRasterizer->SetClipRect(0, 0, __w, __h);
    mpRasterizer->ClearColor(NUI_RGBA(0,0,0,255));

    int32 ww = __w / 8;
    int32 hh = __h / 8;
    //    mpRasterizer->SetClipRect(ww, hh, __w - ww, __h - hh);
    //    mpRasterizer->ClearColor(NUI_RGBA(0,0,128,255));

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(__w - 100, 10, col);
      nuiVertex_Solid v1(__w - 10, 10, col);
      nuiVertex_Solid v2(__w - 10, 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(v0, v1, v2);
    }

    {
      uint32 col = NUI_RGBA(255, 0, 255, 128);
      nuiVertex_Solid v0(0, 0, col);
      nuiVertex_Solid v1(100, 1, col);
      nuiVertex_Solid v2(45, 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v1, v2);
    }

    mpRasterizer->SetClipRect(0, 0, __w, __h);

    { // Horizontal line
      uint32 col = NUI_RGBA(0, 0, 255, 128);
      nuiVertex_Solid v0(50, 50, col);
      nuiVertex_Solid v1(100, 50, col);
      mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v0, v1);
    }

    { // Oblique line
      uint32 col = NUI_RGBA(255, 0, 255, 128);
      nuiVertex_Solid v0(50, 50, col);
      nuiVertex_Solid v1(100, 100, col);
      mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v0, v1);
    }

    { // Oblique line 2
      uint32 col = NUI_RGBA(255, 0, 255, 128);
      nuiVertex_Solid v0(50, 50, col);
      nuiVertex_Solid v1(150, 100, col);
      mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v0, v1);
    }

    { // Vertical Line
      uint32 col = NUI_RGBA(0, 0, 255, 128);
      nuiVertex_Solid v0(50, 50, col);
      nuiVertex_Solid v1(50, 100, col);
      mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v0, v1);
    }

    { // Clipping limits test
      int32 x0 = ww;
      int32 x1 = __w - ww;
      int32 y0 = hh;
      int32 y1 = __h - hh;

      //mpRasterizer->SetClipRect(x0, y0, x1, y1);

      uint32 col = NUI_RGBA(128, 128, 128, 128);

      { // Top
        nuiVertex_Solid v0(x0+1, y0, col);
        nuiVertex_Solid v1(x1-1, y0, col);
        mpRasterizer->DrawLine<nuiPixelBlender_Copy>(v0, v1);
      }

      { // Right
        nuiVertex_Solid v0(x1, y0+1, col);
        nuiVertex_Solid v1(x1, y1-1, col);
        mpRasterizer->DrawLine<nuiPixelBlender_Copy>(v0, v1);
      }

      { // Bottom
        nuiVertex_Solid v0(x1-1, y1, col);
        nuiVertex_Solid v1(x0+1, y1, col);
        mpRasterizer->DrawLine<nuiPixelBlender_Copy>(v0, v1);
      }

      { // Left
        nuiVertex_Solid v0(x0, y1-1, col);
        nuiVertex_Solid v1(x0, y0+1, col);
        mpRasterizer->DrawLine<nuiPixelBlender_Copy>(v0, v1);
      }

      col = NUI_RGBA(255, 0, 0, 128);
      { // Oblique 1
        nuiVertex_Solid v0(x0, y0, col);
        nuiVertex_Solid v1(x1, y1, col);
        mpRasterizer->DrawLine<nuiPixelBlender_Copy>(v0, v1);
      }

      { // Oblique 2
        nuiVertex_Solid v0(x1, y0, col);
        nuiVertex_Solid v1(x0, y1, col);
        mpRasterizer->DrawLine<nuiPixelBlender_Copy>(v0, v1);
      }
    }


    {
      pBox->AddCell(new nuiLabel(nuiTR("clipping test")));
      //mpRasterizer->ReverseBufferABGR();
      mpTex = nuiTexture::GetTexture(info);
      mpTex->SetSource(_T("soft_image2"));
      nuiImage* pImage = new nuiImage(mpTex->GetSource());
      pBox->AddCell(pImage);
    }
  }

  void Test3(nuiBox* pBox, nglImageInfo& info)
  {
    memset((char*)mpRasterizer->GetBuffer(), 0x7f, __h * __w * 4);
    mpRasterizer->SetClipRect(32, 32, 256 - 32, 256 - 32);
    mpRasterizer->ClearColor(NUI_RGBA(0, 0, 128, 255));

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(10, 10, col);
      nuiVertex_Solid v1(100, 10, col);
      nuiVertex_Solid v2(10, 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v1, v2);
    }

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(__w - 100, 10, col);
      nuiVertex_Solid v1(__w - 10, 10, col);
      nuiVertex_Solid v2(__w - 10, 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(v0, v1, v2);
    }

    {
      nuiVertex_Gouraud v0(10, __h - 10, nuiColor(255, 0, 0, 128));
      nuiVertex_Gouraud v1(100, __h - 10, nuiColor(0, 255, 0, 128));
      nuiVertex_Gouraud v2(10, __h - 100, nuiColor(0, 0, 255, 128));
      mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(v0, v1, v2);
    }

    {
      uint32 col = NUI_RGBA(255, 0, 0, 128);
      nuiVertex_Solid v0(__w - 100, __h - 10, col);
      nuiVertex_Solid v1(__w - 10, __h - 10, col);
      nuiVertex_Solid v2(__w - 10, __h - 100, col);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Add32>(v0, v1, v2);
    }

    {
      // Draw a solid rectangle:
      uint32 col = NUI_RGBA(255, 255, 0, 128);
      nuiVertex_Solid v0(50, 50, col);
      nuiVertex_Solid v1(__w - 50, 50, col);
      nuiVertex_Solid v2(__w - 50, __h - 50, col);
      nuiVertex_Solid v3(50, __h - 50, col);
      mpRasterizer->DrawRectangle<nuiPixelBlender_TranspAdd>(v0, v1, v2, v3);
    }

    {
      // Draw a rectangle:
      nuiVertex_Gouraud v0(100, 10, nuiColor(255, 0, 0, 128));
      nuiVertex_Gouraud v1(__w - 100, 10, nuiColor(0, 255, 0, 128));
      nuiVertex_Gouraud v2(__w - 100, __h - 10, nuiColor(0, 0, 255, 128));
      nuiVertex_Gouraud v3(100, __h - 10, nuiColor(255, 255, 255, 64));
      mpRasterizer->DrawRectangle<nuiPixelBlender_Transp>(v0, v1, v2, v3);
    }

    {
      // Draw a rectangle as to triangles:
      nuiVertex_Gouraud v0(10, 100, nuiColor(255, 0, 0, 128));
      nuiVertex_Gouraud v1(__w - 10, 100, nuiColor(0, 255, 0, 128));
      nuiVertex_Gouraud v2(__w - 10, __h - 100, nuiColor(0, 0, 255, 128));
      nuiVertex_Gouraud v3(10, __h - 100, nuiColor(255, 255, 255, 64));
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v1, v2);
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v2, v3);
    }

    pBox->AddCell(new nuiLabel(nuiTR("General test")));
    mpRasterizer->ReverseBufferABGR();
    mpTex = nuiTexture::GetTexture(info);
    mpTex->SetSource(_T("soft_image0"));
    nuiImage* pImage = new nuiImage(mpTex->GetSource());
    pBox->AddCell(pImage);
  }

  int32 random()
  {
#undef max
    double f = rand() & 0x3ff;
    return ToBelow(f);
  }

  void TestRandomTriangles(nuiBox* pBox, nglImageInfo& info)
  {
    srand( (unsigned)time( NULL ) );

    mpRasterizer->SetClipRect(0, 0, __w, __h);
    mpRasterizer->ClearColor(NUI_RGBA(0,0,0,255));

    {
      nuiVertex_Solid v0(1, 64, rand());
      nuiVertex_Solid v1(-1, 64, rand());
      nuiVertex_Solid v2(1, 64, rand());
      mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(v0, v1, v2);
    }


    int32 ww = __w / 8;
    int32 hh = __h / 8;
    mpRasterizer->SetClipRect(ww, hh, __w - ww, __h - hh);
    mpRasterizer->ClearColor(NUI_RGBA(0,0,128,255));

    for (uint32 i = 0; i < 10000; i++)
    {
      nuiVertex_Solid v0(random(), random(), rand());
      nuiVertex_Solid v1(random(), random(), rand());
      nuiVertex_Solid v2(random(), random(), rand());
      mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(v0, v1, v2);
    }


    {
      pBox->AddCell(new nuiLabel(nuiTR("clipping test")));
      mpRasterizer->ReverseBufferABGR();
      mpTex = nuiTexture::GetTexture(info);
      mpTex->SetSource(_T("soft_image1"));
      nuiImage* pImage = new nuiImage(mpTex->GetSource());
      pBox->AddCell(pImage);
    }
  }

  void TestShape(nuiBox* pBox, nglImageInfo& info)
  {
    srand( (unsigned)time( NULL ) );

    mpRasterizer->SetClipRect(0, 0, __w, __h);
    mpRasterizer->ClearColor(NUI_RGBA(0,0,0,255));

    nuiShape shp;
    shp.AddRoundRect(nuiRect(5, 5, __w - 10, __h - 10), 15);

    nuiTessellator* pTess = new nuiTessellator(&shp);
    pTess->SetFill(true);
    nuiRenderObject* pObject = pTess->Generate(0.1f);

    //DrawObject(*pObject);
    for (uint32 k = 0; k < pObject->GetSize(); k++)
    {
      nuiRenderArray* pArray = pObject->GetArray(k);

      NGL_ASSERT(pArray->GetMode() == GL_TRIANGLES);

      const std::vector<nuiRenderArray::Vertex>& rVertices(pArray->GetVertices());

      uint32 c0 = NUI_RGBA(128,0,0,128);
      uint32 c1 = NUI_RGBA(0,128,0,128);
      uint32 c2 = NUI_RGBA(0,0,128,128);

      //      uint32 Min = 0;
      uint32 Min = 4;
      uint32 Max = Min+1; //(pArray->GetSize() / 3)
      //      uint32 Max = (pArray->GetSize() / 3);

      for (uint32 i = Min; i < Max; i++)
      {
        uint32 ii = i * 3;
        nuiVertex_Solid v0(rVertices[ii+0].mX, rVertices[ii+0].mY, c0);
        nuiVertex_Solid v1(rVertices[ii+1].mX, rVertices[ii+1].mY, c1);
        nuiVertex_Solid v2(rVertices[ii+2].mX, rVertices[ii+2].mY, c2);

        for (uint32 j = ii; j < ii+3; j++)
        {
          printf("%3d: %10f / %10f\n", j, rVertices[j].mX, rVertices[j].mY);
        }
        printf("[0x%x / 0x%x]\n", v0.X(), v0.Y());
        printf("[0x%x / 0x%x]\n", v1.X(), v1.Y());
        printf("[0x%x / 0x%x]\n", v2.X(), v2.Y());
        printf("\n");

        mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v0, v0);
        mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v1, v1);
        mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v2, v2);

        //        mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v0, v1);
        //        mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v1, v2);
        //        mpRasterizer->DrawLine<nuiPixelBlender_Transp>(v2, v0);
        mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(v0, v1, v2);
        //        mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(v1, v2, v0);

      }

    }

    delete pObject;
    delete pTess;

    {
      pBox->AddCell(new nuiLabel(nuiTR("Shape rendering")));
      //mpRasterizer->ReverseBufferABGR();
      mpTex = nuiTexture::GetTexture(info);
      mpTex->SetSource(_T("soft_image4"));
      nuiImage* pImage = new nuiImage(mpTex->GetSource());
      pBox->AddCell(pImage);
    }
  }


  nuiRasterizer* mpRasterizer;
  nuiTexture* mpTex;
};

void nuiWin::SoftwarePainterWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new SoftwareWindow(nuiRect(10, 10, 300, 600), nglWindow::NoFlag, nuiTR("Software Rasterizer"));
  mpManager->AddChild(pWin);
}




//
// SpinerLabelWindow
//
//

class MillimeterFormater: public nuiFormater
{
public:

  MillimeterFormater () : nuiFormater ()
  {
  };


  ~MillimeterFormater ()
  {
  };

  virtual bool Format (double dValue, nglString& rResult) const
  {
    rResult.CFormat ("%.0fmm", dValue);
    return true;
  };
};


static MillimeterFormater mmFormater;
static nuiRange mmRange(0,-100,100);

void nuiWin::SpinerLabelWindow (const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("Spiner Label, Formated Label and Formater"));
  mpManager->AddChild(pWindow);


  nuiVBox* box1 = new nuiVBox(2);
  nuiButton* pBtn = new nuiButton(nuiTR("prout"));
  box1->SetCell(0, pBtn);

  //  nuiScrollView* pScroller = new nuiScrollView(true, true);
  //  pWindow->AddChild(pScroller);
  //
  //  //
  //  // available nuiSpinerLabel constr.
  //  //
  //  //nuiSpinerLabel(nuiContainerPtr pParent, const nuiFormater& rFormater = &nuiFormatedLabel::DefaultFormater, const nuiRange& rRange = nuiRange(), nuiTheme::FontStyle FontStyle=nuiTheme::Default);
  //  //nuiSpinerLabel(nuiContainerPtr pParent, const nuiFormater& rFormater, const nuiRange& rRange, nuiFont* pFont, bool AlreadyAcquired = false);
  //  //
  //
  //  nuiFixed* pFixed = new nuiFixed();
  //  pScroller->AddChild(pFixed);
  //
  //  nuiPane* pPane = new nuiPane(nuiColor(0.7f, 0.7f, 0.7f, 0.8f), nuiColor(0.5f, 0.5f, 0.5f, 1.0f));
  //  pFixed->AddChild(pPane);
  //  pPane->SetUserPos (20,20);
  //
  //  nuiSpinnerLabel* pSLabel = new nuiSpinnerLabel(mmFormater, mmRange);
  //  pPane->AddChild(pSLabel);
}



//
// VBoxAndHBoxWindow
//

void nuiWin::VBoxAndHBoxWindow (const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 400, 300), nglWindow::NoFlag, nuiTR("nuiVBox and nuiHBox"));
  mpManager->AddChild(pWindow);

  nuiVBox* boxv = new nuiVBox(2);
  nuiHBox* boxh = new nuiHBox(2);
  boxv->DisplayGridBorder(true);

  nuiLabel* pLabel;

  pLabel = new nuiLabel(_T("r0 long enough"));
  boxv->SetCell(0, pLabel);

  pLabel = new nuiLabel(_T("r1"));
  pLabel->SetTextPosition(nuiCenter);
  boxv->SetCell(1, pLabel);

  pLabel = new nuiLabel(_T("r2"));
  pLabel->SetTextPosition(nuiBottom);
  boxv->AddCell(pLabel);

  pLabel = new nuiLabel(_T("r3"));
  pLabel->SetPosition(nuiCenter);
  boxv->AddCell(pLabel);

  pLabel = new nuiLabel(_T("r4"));
  pLabel->SetPosition(nuiBottom);
  boxv->AddCell(pLabel);


  boxh->SetCell(0, new nuiLabel(_T("c0")));
  boxh->SetCell(1, new nuiLabel(_T("c1")));
  boxh->AddCell(new nuiLabel(_T("c2")));
  boxh->AddCell(new nuiLabel(_T("c3")));


  pWindow->AddChild(boxv);
  boxv->AddCell(boxh);
}







//********************************************************************************************
//
// MatrixViewWindow
//

#define ROW_HEIGHT 17

bool MatrixViewFlag;

void nuiWin::MatrixViewTest(const nuiEvent& rEvent)
{
  nuiMatrixView* mat = (nuiMatrixView*)rEvent.mpUser;

  if (MatrixViewFlag)
  {
    MatrixViewFlag = false;
    mat->SetRowVisible(3, true);
  }
  else
  {
    MatrixViewFlag = true;
    mat->SetRowVisible(3, false);
  }

  mat->InvalidateLayout();
  rEvent.Cancel();
}

void nuiWin::MatrixViewWindow (const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 500, 300), nglWindow::NoFlag, _T("nuiMatrixView"));
  mpManager->AddChild(pWindow);

  MatrixViewFlag = false;

  uint length = 6;

  nuiVBox* vbox = new nuiVBox(2);
  pWindow->AddChild(vbox);


  nuiMatrixView* mat = new nuiMatrixView (length, length, true, true, nuiMatrixView::mDefaultFormater, nuiRange(0.0f,0.0f,100.0f,0.0f,10.0f), 2, 2);
  vbox->SetCell(0, mat);

  mSink.Connect(mat->GetItemDesc, nuiMakeDelegate(this, &nuiWin::GetMatrixViewItemDesc));


  mat->Init();

  for (uint i=0; i < length; i++)
  {
    mat->SetRowMaxPixels(i, ROW_HEIGHT);
    mat->SetRowMinPixels(i, ROW_HEIGHT);
  }



  nuiButton* btn = new nuiButton(nuiTR("test"));
  vbox->SetCell(1, btn);
  mWinSink.Connect(btn->Activated, &nuiWin::MatrixViewTest, (void*)mat);


  // css decoration example
  //  nuiMatrixViewItem
  //  {
  //    TextColor: white;
  //  }



  /*

  the nuiMatrixView cells are initialized with default values.
  But you're then able to set the cells using the following methods :


  mat->SetColumnHeader (0, new nuiLabel (NULL, _T("MatrixView")));
  mat->SetColumnHeader (1, new nuiLabel (NULL, _T("Column 1")));
  mat->SetColumnHeader (2, new nuiLabel (NULL, _T("Column 2")));

  mat->SetColumnHeader (3, new nuiLabel (NULL, _T("Column 3")));
  mat->SetColumnHeader (4, new nuiLabel (NULL, _T("Column 4")));
  mat->SetColumnHeader (5, new nuiLabel (NULL, _T("Column 5")));
  //mat->SetColumnHeader (6, new nuiLabel (NULL, _T("Column 6")));
  mat->SetRowSpacing (0, 10);


  mat->SetRowHeader (1,new  nuiLabel (NULL, _T("Row 1")));
  mat->SetRowHeader (2, new nuiLabel (NULL, _T("Row 2")));

  mat->SetRowHeader (3, new nuiLabel (NULL, _T("Row 3")));
  mat->SetRowHeader (4, new nuiLabel (NULL, _T("Row 4")));
  mat->SetRowHeader (5, new nuiLabel (NULL, _T("Row 5")));
  //mat->SetRowHeader (6, new nuiLabel (NULL, _T("Row 6")));
  mat->SetColumnSpacing (0, 5);



  */
}




void nuiWin::GetMatrixViewItemDesc(nuiMatrixViewItemDesc& rDesc)
{
  rDesc.SetFillColor(nuiColor(0, 116, 180));
  rDesc.SetStrokeColor(nuiColor(0,0,200));
  rDesc.SetSelectionColor(nuiColor(250,0,0));
  rDesc.SetSelectionSize(2);
}



//********************************************************************************************
//
// CreateTitledPaneWindow (nuiTitledPane and nuiFolderPane)
//
void nuiWin::CreateTitledPaneWindow(const nuiEvent& rEvent)
{
  nuiWindow* pWindow = new nuiWindow(nuiRect(10, 10, 500, 300), nglWindow::NoFlag, nuiTR("nuiTitledPane and nuiFolderPane"));
  mpManager->AddChild(pWindow);

  nuiVBox* vbox = new nuiVBox(2);
  pWindow->AddChild(vbox);
  vbox->SetExpand(nuiExpandShrinkAndGrow);
  vbox->SetDefaultSpacing(10);

  nuiPane* pPane = new nuiPane();
  nuiLabel* pLabel = new nuiLabel(nuiTR("Title"), nuiFont::GetFont(12));
  pPane->AddChild(pLabel);

  nuiTitledPane* pTPane = new nuiTitledPane(pPane);
  vbox->SetCell (0, pTPane);
  vbox->SetCellSpacing (0, 10);

  pTPane->AddChild(new nuiLabel(nuiTR("bleh!")));

  nuiPane* pPane2 = new nuiPane();
  nuiLabel* pLabel2 = new nuiLabel(_T("FolderTitle"), nuiFont::GetFont(12));
  pPane2->AddChild(pLabel2);

  nuiFolderPane* pFPane = new nuiFolderPane(nuiTR("My Folder Pane"));
  vbox->SetCell (1, pFPane);
  vbox->SetCellSpacing (1, 10);

  pFPane->AddChild(new nuiLabel(nuiTR("bleh!")));
}



// ====================================================================================================================
//  Defines
// ====================================================================================================================
#ifndef _xs_DEFAULT_CONVERSION
#define _xs_DEFAULT_CONVERSION      0
#endif //_xs_DEFAULT_CONVERSION


#if _xs_BigEndian_
#define _xs_iexp_                        0
#define _xs_iman_                        1
#else
#define _xs_iexp_                        1       //intel is little endian
#define _xs_iman_                        0
#endif //BigEndian_


#define _xs_doublecopysgn(a,b)      ((int32*)&a)[_xs_iexp_]&=~(((int32*)&b)[_xs_iexp_]&0x80000000)
#define _xs_doubleisnegative(a)     ((((int32*)&a)[_xs_iexp_])|0x80000000)

typedef double real64;
typedef float real32;

/// ====================================================================================================================
//  Constants
// ====================================================================================================================
const real64 _xs_doublemagic             = real64 (6755399441055744.0);      //2^52 * 1.5,  uses limited precisicion to floor
const real64 _xs_doublemagicdelta        = (1.5e-8);                         //almost .5f = .5f + 1e^(number of exp bit)
const real64 _xs_doublemagicroundeps     = (.5-_xs_doublemagicdelta);       //almost .5f = .5f - 1e^(number of exp bit)


// ====================================================================================================================
//  Prototypes
// ====================================================================================================================
int32 xs_CRoundToInt      (real64 val, real64 dmr =  _xs_doublemagic);
int32 xs_ToInt            (real64 val, real64 dme = -_xs_doublemagicroundeps);
int32 xs_FloorToInt       (real64 val, real64 dme =  _xs_doublemagicroundeps);
int32 xs_CeilToInt        (real64 val, real64 dme =  _xs_doublemagicroundeps);
int32 xs_RoundToInt       (real64 val);


//int32 versions	 	- just to make macros and templates easier
int32 xs_CRoundToInt      (int32 val)   {return val;}
int32 xs_ToInt            (int32 val)   {return val;}

// ====================================================================================================================
//  Fix Class
// ====================================================================================================================
template <int32 N> class xs_Fix
{
public:
  typedef int32 Fix;
  // ====================================================================================================================
  //  Basic Conversion from Numbers
  // ====================================================================================================================
  inline static Fix       ToFix       (int32 val)    {return val<<N;}
  inline static Fix       ToFix       (real64 val)   {return xs_ConvertToFixed(val);}

  // ====================================================================================================================
  //  Basic Conversion to Numbers
  // ====================================================================================================================
  inline static real64    ToReal      (Fix f)        {return real64(f)/real64(1<<N);}
  inline static int32     ToInt       (Fix f)        {return f>>N;}

protected:
  // ====================================================================================================================
  // Helper function - mainly to preserve _xs_DEFAULT_CONVERSION
  // ====================================================================================================================
  inline static int32 xs_ConvertToFixed (real64 val)
  {
#if _xs_DEFAULT_CONVERSION==0
    return xs_CRoundToInt(val, _xs_doublemagic/(1<<N));
#else
    return (long)((val)*(1<<N));
#endif
  }
};

// ====================================================================================================================
// ====================================================================================================================
//  Inline implementation
// ====================================================================================================================
// ====================================================================================================================
inline int32 xs_CRoundToInt(real64 val, real64 dmr)
{
#if _xs_DEFAULT_CONVERSION==0
  val = val + dmr;
  return ((int32*)&val)[_xs_iman_];
#else
  return int32(floor(val+.5));
#endif
}

// ====================================================================================================================
inline int32 xs_ToInt(real64 val, real64 dme)
{
  /* unused - something else I tried...
  _xs_doublecopysgn(dme,val);
  return xs_CRoundToInt(val+dme);
  */

#if _xs_DEFAULT_CONVERSION==0
  //  return (val<0) ?   xs_CRoundToInt(val-dme) : xs_CRoundToInt(val+dme);
  _xs_doublecopysgn(dme,val);
  return xs_CRoundToInt(val+dme);
#else
  return int32(val);
#endif
}

// ====================================================================================================================
inline int32 xs_FloorToInt(real64 val, real64 dme)
{
#if _xs_DEFAULT_CONVERSION==0
  return xs_CRoundToInt (val - dme);
#else
  return floor(val);
#endif
}

// ====================================================================================================================
inline int32 xs_CeilToInt(real64 val, real64 dme)
{
#if _xs_DEFAULT_CONVERSION==0
  return xs_CRoundToInt (val + dme);
#else
  return ceil(val);
#endif
}

//=================================================================================================================
inline int32 xs_RoundToInt(real64 val)
{
#if _xs_DEFAULT_CONVERSION==0
  return xs_CRoundToInt (val + _xs_doublemagicdelta);
#else
  return floor(val+.5);
#endif
}

// ====================================================================================================================
// ====================================================================================================================

template <class Class> inline int32 tozero(Class x)
{
  //  return x >= 0 ? xs_CRoundToInt(x) : -xs_CRoundToInt(-x);
  return xs_ToInt(x);
}

//inline int32 tozero(float x)
//{
//  return x >= 0 ? xs_CRoundToInt(x) : -xs_CRoundToInt(-x);
//}

void nuiWin::DumpStats(const nuiEvent& rEvent)
{
  NGL_OUT((_T("sizeof(nuiObject): %d\n")), sizeof(nuiObject));
  NGL_OUT((_T("sizeof(nuiWidget): %d\n")), sizeof(nuiWidget));
  NGL_OUT((_T("sizeof(nuiContainer): %d\n")), sizeof(nuiContainer));
  NGL_OUT((_T("sizeof(nglString): %d\n")), sizeof(nglString));
  NGL_OUT((_T("\tsizeof(wstring): %d\n")), sizeof(wstring));

  NGL_OUT(nuiTR("floating point tests:\n"));
  {
    float f = -28.5;
    int32 below = ToBelow(f);
    int32 above = ToAbove(f);
    int32 zero = ToZero(f);
    int32 nearest = ToNearest(f);

    NGL_OUT(_T("float below %d\n"), below);
    NGL_OUT(_T("float above %d\n"), above);
    NGL_OUT(_T("float zero %d\n"), zero);
    NGL_OUT(_T("float nearest %d\n"), nearest);
  }
  {
    double f = -28.5;
    int32 below = ToBelow(f);
    int32 above = ToAbove(f);
    int32 zero = ToZero(f);
    int32 nearest = ToNearest(f);

    NGL_OUT(_T("double below %d\n"), below);
    NGL_OUT(_T("double above %d\n"), above);
    NGL_OUT(_T("double zero %d\n"), zero);
    NGL_OUT(_T("double nearest %d\n"), nearest);
  }

  {
    float f1 = -10000;
    float f2 = 10000;
    float incr = 0.1f;

    while (f1 < f2)
    {
      int32 i1 = tozero(f1);
      int32 i2 = (int32)f1;

      if (i1 != i2)
      {
        NGL_OUT(_T("Float Rounding error: %f (ToBelow = %d / cast = %d)\n"), f1, i1, i2);
      }
      f1 += incr;
    }
  }

  {
    double f1 = -10000;
    double f2 = 10000;
    double incr = 0.1f;

    while (f1 < f2)
    {
      int32 i1 = tozero(f1);
      int32 i2 = (int32)f1;

      if (i1 != i2)
      {
        NGL_OUT(_T("Double Rounding error: %f (ToBelow = %d / cast = %d)\n"), f1, i1, i2);
      }
      f1 += incr;
    }
  }
}

bool nuiWin::OnKeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ESC)
  {
    App->Quit (0);
    return true;
  }

  return nuiMainWindow::OnKeyDown(rEvent);
}

nuiWin::~nuiWin()
{
  mRefs--;
}

void nuiWin::OnClose()
{
  if (!mRefs != 1)
    App->Quit();
  else
    delete this;
}

void nuiWin::OutputSomething( const nuiEvent& rEvent)
{
  static int value = 0;
  NGL_OUT((_T("Position: %d\n")),value++);
}

void nuiWin::CreateFromXML( const nuiEvent& rEvent)
{
  /*
  nuiXML XML(nglString((_T("Test"))));
  nglIFile file(nglPath(_T("test.xml")));
  if (!XML.Load(file))
  {
  NGL_OUT((_T("\nERROR: unable to load glade XML description :-(.\n")));
  return false;
  }

  nuiCreateWidget(NULL,&XML);
  //  nuiCreateWidget(GetRoot(),&XML);
  */

  /* Create the nglWindow (and thus a GL context, don't even try to
  *   instantiate the gui (or nglFont) before the nuiWin !)
  */
  nuiContextInfo ContextInfo;
  nglWindowInfo Info;

  Info.Flags = 0;
  Info.Width = 640;
  Info.Height = 480;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = nuiTR("nui test");
  Info.XPos = 0;
  Info.YPos = 0;

  nuiWin* win = new nuiWin(ContextInfo,Info, false, GetNGLContext());
  if ((!win) || (win->GetError()))
  {
    if (win)
      NGL_OUT(_T("Error: cannot create window (%ls)\n"), win->GetErrorStr());
    App->Quit (1);
    return;
  }
  win->DBG_SetMouseOverInfo(false);
  win->DBG_SetMouseOverObject(false);
  win->SetState(nglWindow::eShow);
}

void nuiWin::HideText( const nuiEvent& rEvent)
{
  if (!rEvent.mpUser)
    return;
  ((nuiWidgetPtr)rEvent.mpUser)->SetVisible(!((nuiWidgetPtr)rEvent.mpUser)->IsVisible());
  return;
}

void nuiWin::SaveXMLDescription(const nuiEvent& rEvent)
{
//  nuiXML* pXML = NULL;
//  pXML = (nuiXML*) Serialize(NULL,true);
//  nglOFile file(nglPath((_T("nuiTestOutput.xml"))),eOFileCreate);
//  pXML->Save(file);
}

void nuiWin::LogText(const nuiEvent& rEvent)
{
  nuiEditLine* pLine = (nuiEditLine*)rEvent.mpUser;
  NGL_OUT(_T("From Edit Line: %ls\n"),pLine->GetText().GetChars());
  pLine->SetText((_T("")));
}

void nuiWin::FileSelect(const nuiEvent& rEvent)
{
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 200, 200), nuiWindow::NoFlag, nuiTR("Select a file..."));
  mpManager->AddChild(pWin);
  nuiFileSelector* pFileSelector = new nuiFileSelector(nglPath(_T("/")));
  pWin->AddChild(pFileSelector);
  mWinSink.Connect(pFileSelector->OK, &nuiWin::FileSelectDone, pFileSelector);
  mWinSink.Connect(pFileSelector->Cancel, &nuiWin::FileSelectDone, pFileSelector);
  pFileSelector->SetPath(nglPath(ePathCurrent));
  rEvent.Cancel();
}

void nuiWin::FileSelectDone(const nuiEvent& rEvent)
{
  nuiFileSelector* pFileSelector = (nuiFileSelector*)rEvent.mpUser;

  nglPath path = pFileSelector->GetPath();
  NGL_OUT(_T("File selection: '%ls'\n"), path.GetChars());

  pFileSelector->GetParent()->Trash();
  rEvent.Cancel();
}

void nuiWin::DrawBlur(const nuiEvent& rEvent)
{
  nuiDrawEvent* pDrawEvent = (nuiDrawEvent*)&rEvent;
  nuiTexture* pTexture = (nuiTexture*) pDrawEvent->mpUser;
  nuiDrawContext* pContext= pDrawEvent->mpContext;

  static nglTime t;
  nglTime now;

  double val = (double)now - (double)t;
  t = now;
  static float v = 0;
  v += (float)val;
  if (v > 360)
    v = 0;
  float zoom = (2.2f+sin(v*2.2f));
  nuiSize w = (float)GetWidth()/8.0f;
  nuiSize h = (float)GetHeight()/8.0f;
  float x = w * sin(v) * .2f;
  float y = h * cos(v/2) * .2f;
  nuiRadialBlur(pContext, pTexture, nuiRect(w/2,h/2,w, h), zoom, nuiVector2(x,y), 20, .8f, .5f);
  nuiTopLevel::Invalidate();
  rEvent.Cancel();
}

void nuiWin::CreateEnableTestWindow(const nuiEvent& rEvent)
{
  nuiWindow* pEnableWin = new nuiWindow(nuiRect(10, 10, 100, 100), nuiWindow::NoFlag, nuiTR("Enable Widget Test"));

  nuiLabel* pLabel = new nuiLabel(nuiTR("Disabled Label"));
  pLabel->SetEnabled(false);

  nuiSpinnerLabel* pSpinner = new nuiSpinnerLabel();
  pSpinner->SetEnabled(false);

  nuiHBox* pBox = new nuiHBox(2);
  pBox->SetCell(0, pLabel);
  pBox->SetCell(1, pSpinner);

  nuiFolderPane* pFolder = new nuiFolderPane(nuiTR("EnableWidget Folder"));
  pFolder->AddChild(pBox);

  pEnableWin->AddChild(pFolder);
  mpManager->AddChild(pEnableWin);
}

void nuiWin::CreateButtonDecorationWindow(const nuiEvent& rEvent)
{
  nuiWindow* pEnableWin = new nuiWindow(nuiRect(10, 10, 100, 100), nuiWindow::NoFlag, nuiTR("Button Decoration sample"));

  pEnableWin->SetColor(eActiveWindowBg, nuiColor(0,0,0));
  pEnableWin->SetColor(eInactiveWindowBg, nuiColor(0,0,0));

  nuiButton* pButton = new nuiButton(new nuiStateDecoration(_T("myRollOver"), _T("rsrc:/stateBt_on.png"), _T("rsrc:/stateBt_off.png"), _T("rsrc:/stateBt_roll.png")));
  pEnableWin->AddChild(pButton);
  pButton->SetPosition(nuiCenter);
  mpManager->AddChild(pEnableWin);
}

void nuiWin::CreateVolumesTestWindow(const nuiEvent& rEvent)
{
  nuiWindow* pEnableWin = new nuiWindow(nuiRect(10, 10, 300, 300), nuiWindow::NoFlag, nuiTR("Enumerate Volumes Test"));
  nuiText* pText = new nuiText();

  std::list<nglPathVolume> Volumes;

  std::pair<uint64, nglString> flags[5];
  flags[0].first = 0xFFFFFFFF;
  flags[0].second = nuiTR("All");
  flags[1].first = 1;
  flags[1].second = nuiTR("ReadOnly");
  flags[2].first = 2;
  flags[2].second = nuiTR("Removable");
  flags[3].first = 4;
  flags[3].second = nuiTR("Offline");
  flags[4].first = 8;
  flags[4].second = nuiTR("System");

  for (int i =0; i < 5; i++)
  {
    nglVolume::GetVolumes(Volumes, flags[i].first);
    pText->AddText(nuiTR("\nFlags :  ") + flags[i].second + nuiTR("\nGetVolumes   number of volumes : ") + nglString((uint32)Volumes.size()) + _T("\n"));

    std::list<nglPathVolume>::iterator it;
    for (it = Volumes.begin(); it != Volumes.end(); ++it, i++)
    {
      nglPathVolume volume = *it;
      pText->AddText(nuiTR("VOLUME ") + nglString((uint32)i) + nuiTR(":\n  Path:  ") + volume.mPath.GetPathName() + nuiTR("\n  Comment: ") + volume.mComment + _T("\n\n"));
    }
    Volumes.clear();
  }


  pEnableWin->AddChild(pText);
  mpManager->AddChild(pEnableWin);
}

void nuiWin::CreateMessageBox(const nuiEvent& rEvent)
{
  nuiMessageBox::Button res;

  nuiMessageBox* pBox = new nuiMessageBox(this, nuiTR("Simple message box (OK)"), nuiTR("This simple message box should be displayed.\nYou normaly can't interact with the rest of the GUI\nbefore you click the OK button"), eMB_OK);
  res = pBox->QueryUser();

  res = nuiMessageBox::Do(this, nuiTR("Simple message box (Cancel)"), nuiTR("This is a 'Cancel' message box"), eMB_Cancel);
  res = nuiMessageBox::Do(this, nuiTR("Simple message box (OK - Cancel)"), nuiTR("This is a 'OK/Cancel' message box"), eMB_OKCancel);
  res = nuiMessageBox::Do(this, nuiTR("Simple message box (Retry - OK - Cancel)"), nuiTR("This is a 'Retry/OK/Cancel' message box"), eMB_RetryOKCancel);
  res = nuiMessageBox::Do(this, nuiTR("Simple message box (Yes - No) [Last MsgBox Test]"), nuiTR("This is a 'Yes/No' message box,\nyou will now be able to go back to what you were doing..."), eMB_YesNo);
}

void nuiWin::CreateWidgetTexture(const nuiEvent& rEvent)
{
  nuiTopLevel* pTop = GetTopLevel();
  nuiContainer::Iterator* it = mpManager->GetFirstChild();
  if (!it->IsValid())
  {
    delete it;
    return;
  }

  nuiWidgetPtr pSource = it->GetWidget();
  delete it;
  nuiRect r = pSource->GetRect();
  nuiDrawContext* pContext = nuiDrawContext::CreateDrawContext(r.Size(), eSoftware, NULL);
  bool old = IsPartialRedrawEnabled();
  EnablePartialRedraw(false);

  pSource->LocalToGlobal(r);
  pContext->StartRendering();
  pContext->LoadIdentity();
  pContext->Translate(r.Left(), r.Top());
  pContext->SetClearColor(nuiColor(255,255,255,255));
  pContext->Clear();
  pContext->Clip(r.Size());
  pContext->EnableClipping(true);

  pSource->DrawWidget(pContext);
  pContext->StopRendering();
  nuiSoftwarePainter* pPainter = (nuiSoftwarePainter*)pContext->GetPainter();
  nuiRasterizer* pRasterizer = pPainter->GetRasterizer();
  pRasterizer->ReverseBufferBGRA();

  nglImageInfo info;
  info.mBufferFormat = eImageFormatRaw;  ///< Buffer data format can be raw (user accessible) or proprietary (opaque to user, such as S3TC/DXTC)
  info.mPixelFormat = eImagePixelRGBA;   ///< Pixel components and respective components bit resolution
  info.mWidth = pContext->GetWidth();                        ///< Image width in pixels (0 if \a mpBuffer is NULL)
  info.mHeight = pContext->GetHeight();                       ///< Image height in pixels (0 if \a mpBuffer is NULL)
  info.mBitDepth = 32;                     ///< Pixel bit depth (sum of components bit resolution, 0 if \a mpBuffer is NULL)
  info.mBytesPerPixel = (info.mBitDepth / 8);                ///< Pixel allocation size in bytes (>= pixel bit depth, 0 if \a mpBuffer is NULL)
  info.mBytesPerLine = info.mBytesPerPixel * info.mWidth;                 ///< Pixel row allocation size in bytes (>= pixel size * image width, 0 if \a mpBuffer is NULL)
  info.mpBuffer = (char*)pRasterizer->GetBuffer();                      ///< Buffer data

  nuiTexture* pTexture = nuiTexture::GetTexture(info, true);

  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 300, 300), nuiWindow::NoFlag, nuiTR("Widget Texture"));
  mpManager->AddChild(pWin);
  nuiScrollView* pScroll = new nuiScrollView();
  pWin->AddChild(pScroll);
  nuiImage* pImage = new nuiImage(pTexture, true);
  pScroll->AddChild(pImage);

  delete pContext;
  EnablePartialRedraw(old);
}

