/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiContainer.h"
#include "nuiMainWindow.h"
#include "nuiWindow.h"
#include "nuiXML.h"
#include "nuiBuilder.h"
#include "nuiLabel.h"
#include "nuiIntrospector.h"
#include "nuiSoftwarePainter.h"

//#define STUPID
//#define STUPIDBASE
#ifdef STUPID
#define STUPIDBASE
#endif

using namespace std;

#define NUI_MULTISAMPLES 0

nuiContextInfo::nuiContextInfo(Type type)
{
  TargetAPI = (nglTargetAPI)nuiMainWindow::GetRenderer();

  StencilBits = 8;
  DepthBits = 0;
  FrameBitsA = 0;
  AABufferCnt = 0;
  AASampleCnt = 0;

  switch (type)
  {
  case StandardContext2D:
  case StandardContext3D:
    Offscreen = false;
    RenderToTexture = false;
    AASampleCnt = NUI_MULTISAMPLES;
    if (AASampleCnt)
      AABufferCnt = 1;
    CopyOnSwap = true;
    break;
  case OffscreenContext2D:
  case OffscreenContext3D:
    Offscreen = true;
    RenderToTexture = true;
    AASampleCnt = NUI_MULTISAMPLES;
    if (AASampleCnt)
      AABufferCnt = 1;
    CopyOnSwap = true;
    break;
  }

  if (type == StandardContext3D || type == OffscreenContext3D)
    DepthBits = 16;
}

nuiMainWindow::nuiMainWindow(uint Width, uint Height, bool Fullscreen, const nglPath& rResPath)
  : nuiTopLevel(rResPath),
    mMainWinSink(this),
    mQuitOnClose(true),
    mpDragSource(NULL)
{
  mFullFrameRedraw = 2;
  mpNGLWindow = new NGLWindow(this, Width, Height, Fullscreen);

  nuiRect rect(0.0f, 0.0f, (nuiSize)Width, (nuiSize)Height);
  //nuiSimpleContainer::SetRect(rect);
  SetObjectClass(_T("nuiMainWindow"));
  mMaxFPS = 0.0f;

  uint w,h;
  mpNGLWindow->GetSize(w,h);

  SetRect(nuiRect(0.0f, 0.0f, (nuiSize)w, (nuiSize)h));
  
  mpInvalidateTimer = 0;
  mLastRendering = 0;

  mDisplayMouseOverInfo = false;
  mDisplayMouseOverObject = false;
  mpInfoLabel = new nuiLabel(_T("No info"));
  AddChild(mpInfoLabel);
  mpInfoLabel->SetVisible(false);

  mDebugMode = false;
  mInvalidatePosted = false;
  mpInspectorWindow = NULL;
  
  mpWidgetCanDrop = NULL;
}

nuiMainWindow::nuiMainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglContext* pShared, const nglPath& rResPath)
  : nuiTopLevel(rResPath),
    mMainWinSink(this),
    mQuitOnClose(true),
    mpDragSource(NULL)
{
  mFullFrameRedraw = 2;
  mpNGLWindow = new NGLWindow(this, rContextInfo, rInfo, pShared);
  nuiRect rect(0.0f, 0.0f, (nuiSize)rInfo.Width, (nuiSize)rInfo.Height);
  //nuiSimpleContainer::SetRect(rect);
  SetObjectClass(_T("nuiMainWindow"));

  mMaxFPS = 0.0f;

  uint w,h;
  mpNGLWindow->GetSize(w,h);

  SetRect(nuiRect(0.0f, 0.0f, (nuiSize)w, (nuiSize)h));
  
  mpInvalidateTimer = 0;
  mLastRendering = 0;

  mDisplayMouseOverInfo = false;
  mDisplayMouseOverObject = false;
  mpInfoLabel = new nuiLabel(_T("No info"));
  AddChild(mpInfoLabel);
  mpInfoLabel->SetVisible(false);

  mDebugMode = false;
  mInvalidatePosted = false;
  mpInspectorWindow = NULL;
  mpWidgetCanDrop = NULL;
}

bool nuiMainWindow::Load(const nuiXMLNode* pNode)
{
  nuiTopLevel::Load(pNode);
  
  int W = nuiGetVal(pNode, _T("Width"), 320);
  int H = nuiGetVal(pNode, _T("Height"), 200);
  bool Fullscreen = nuiGetBool(pNode, _T("FullScreen"), false);
  
  mFullFrameRedraw = 2;
  mpNGLWindow = new NGLWindow(this, W, H, Fullscreen);

  nuiRect rect((nuiSize)0, (nuiSize)0, (nuiSize)W, (nuiSize)H);
  nuiSimpleContainer::SetRect(rect);
  SetObjectClass(_T("nuiMainWindow"));
  mMainWinSink.SetTarget(this);

  if (pNode->HasAttribute("Renderer"))
  {
    nglString renderer = pNode->GetAttribute("Renderer");
    renderer.ToLower();
    if (renderer == _T("opengl"))
      nuiTopLevel::SetRenderer(eOpenGL);
    if (renderer == _T("direct3d"))
      nuiTopLevel::SetRenderer(eDirect3D);
    else if (renderer == _T("software"))
      nuiTopLevel::SetRenderer(eSoftware);
  }

  uint w,h;
  mpNGLWindow->GetSize(w,h);

  GetDrawContext();

  SetRect(nuiRect(0.0f, 0.0f, (nuiSize)w, (nuiSize)h));

  mMaxFPS = 0.0f;
  mpInvalidateTimer = 0;
  mLastRendering = 0;

  // Once every thing is properly created on the root window, create the children:
  nglString name;
  name.CFormat(_T("%p"),this);
  SetProperty(nglString(_T("Name")),name);

  if (mpParent)
    mpParent->DelChild(this);
  SetParent(NULL);
  mHasFocus = false;
  SetVisible(true);

  if (pNode->HasAttribute(_T("Name")))
    SetProperty(nglString(_T("Name")),pNode->GetAttribute(nglString(_T("Name"))));

  if (pNode->HasAttribute(_T("Title")))
    mpNGLWindow->SetTitle(pNode->GetAttribute(nglString(_T("Title"))));

  // Retrieve the size of the widget from the xml description (ignored if not present):
  mRect.mLeft = pNode->GetAttribute(nglString(_T("X"))).GetCFloat();
  mRect.mRight = mRect.mLeft + pNode->GetAttribute(nglString(_T("Width"))).GetCFloat();
  mRect.mTop = pNode->GetAttribute(nglString(_T("Y"))).GetCFloat();
  mRect.mBottom = mRect.mTop + pNode->GetAttribute(nglString(_T("Height"))).GetCFloat();
  mIdealRect = mRect;

  SetVisible(nuiGetBool(pNode,nglString(_T("Visible")),true));

  nuiWidget::SetEnabled(nuiGetBool(pNode, _T("Enabled"), true));
  nuiWidget::SetSelected(nuiGetBool(pNode, _T("Selected"), true));


/* THIS SHOULD ALREADY BE IN nuiContainer::nuiContainer...
  uint i, count = pNode->GetChildrenCount();
  for (i = 0; i < count; i++)
  {
    nuiXMLNode* pChild = pNode->GetChild(i);
    if (!nuiCreateWidget(this, pChild))
    {
      // If the child is not a creatable element then may be a text property of the object.
      nuiXMLNode* pText = pChild->SearchForChild(nglString("##text"));
      if (pText)
        SetProperty(pChild->GetName(),pText->GetValue());
    }
  }
*/

  mDisplayMouseOverInfo = false;
  mDisplayMouseOverObject = false;
  mDebugMode = false;
  mInvalidatePosted = false;
  mpInspectorWindow = NULL;
  mpWidgetCanDrop = NULL;

  return true;
}

nuiMainWindow::~nuiMainWindow()
{
  delete mpInspectorWindow;
  nuiTopLevel::Exit();
  
  if (mpInvalidateTimer)
    delete mpInvalidateTimer;
  delete mpNGLWindow;
  mpNGLWindow = NULL;
  //OnDestruction();
}

nuiXMLNode* nuiMainWindow::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode, Recursive);
  if (!pNode) 
    return NULL;

  pNode->SetAttribute(_T("Title"), mpNGLWindow->GetTitle());

  return pNode;
}


void nuiMainWindow::OnPaint()
{
  mInvalidatePosted = true;
  Paint();
}

void nuiMainWindow::LazyPaint()
{
  if (mInvalidatePosted || mNeedLayout)
  {
    Paint();
  }
}

void nuiMainWindow::Paint()
{
  do 
  {
    FillTrash();
    
    GetIdealRect();
    SetLayout(nuiRect(0, 0, mpNGLWindow->GetWidth(), mpNGLWindow->GetHeight()));
    
    EmptyTrash();
  } while (IsTrashFull());

  if (!mInvalidatePosted)
  {
    return;
  }
  
  FillTrash();

  nuiDrawContext* pContext = GetDrawContext();
  
  pContext->GetPainter()->ResetStats();
  nuiSoftwarePainter* pCTX = dynamic_cast<nuiSoftwarePainter*>(pContext->GetPainter());

  mpNGLWindow->BeginSession();

  pContext->StartRendering();
  bool DrawFullFrame = !mInvalidatePosted || (mFullFrameRedraw > 0);
  bool RestorePartial = IsPartialRedrawEnabled();
  mInvalidatePosted = false;
  if (DrawFullFrame && RestorePartial)
    EnablePartialRedraw(false);

//  static int counter = 0;
  //NGL_OUT(_T("%d OnPaint %d - %d\n"), counter++, DrawFullFrame, RestorePartial);

  pContext->LoadIdentity();

  pContext->MultMatrix(GetMatrix());
  mLastRendering = nglTime();
  DrawWidget(pContext);

  if (mDisplayMouseOverObject)
    DBG_DisplayMouseOverObject();

  if (mDisplayMouseOverInfo)
    DBG_DisplayMouseOverInfo();

  if (DrawFullFrame && RestorePartial)
    EnablePartialRedraw(true);

  pContext->StopRendering();
  EmptyTrash();

  if (pCTX)
    pCTX->Display(GetNGLWindow());

  pContext->EndSession();
  mpNGLWindow->EndSession();

  if (mFullFrameRedraw)
    mFullFrameRedraw--;
}

void nuiMainWindow::OnResize(uint Width, uint Height)
{
  FillTrash();
  nuiRect Rect;
  Rect.mRight=(nuiSize)Width;
  Rect.mBottom=(nuiSize)Height;
  //SetLayout(Rect);

  GetDrawContext()->SetSize(Width,Height);

  //NGL_OUT(_T("(OnResize)nglWindow::Invalidate()\n"));;
  InvalidateLayout();

  mFullFrameRedraw++;
  
  EmptyTrash();
}

void nuiMainWindow::OnCreation()
{
  EmptyTrash();
}

void nuiMainWindow::OnDestruction()
{
  //NGL_OUT(_T("OnDestruction\n"));
  EmptyTrash();
}

void nuiMainWindow::OnActivation()
{
  //OUT("OnActivation\n");
  EmptyTrash();
  CancelGrab();
  mMouseInfo.Buttons = 0;
}

void nuiMainWindow::OnDesactivation()
{
  //OUT("OnDesactivation\n");
  EmptyTrash();
  CancelGrab();
  mMouseInfo.Buttons = 0;
}

void nuiMainWindow::OnClose()
{
  //OUT("OnClose\n");
  if (mQuitOnClose)
    App->Quit(0);
  else
    nuiWidget::Trash();
}

void nuiMainWindow::OnState (nglWindow::StateInfo State)
{
  //OUT("OnState\n");
  EmptyTrash();
}


void nuiMainWindow::BroadcastInvalidate(nuiWidgetPtr pSender)
{
  nuiTopLevel::BroadcastInvalidate(pSender);

  //NGL_OUT(_T("(Invalidate)InvalidatePosted(%ls)\n"), pSender->GetProperty("Class").GetChars());
  mInvalidatePosted = true;

  if (mMaxFPS == 0.0f)
  {
    mpNGLWindow->Invalidate();
  }

}

void nuiMainWindow::BroadcastInvalidateRect(nuiWidgetPtr pSender, const nuiRect& rRect)
{
  nuiTopLevel::BroadcastInvalidateRect(pSender, rRect);

  //NGL_OUT(_T("(InvalidateRect)InvalidatePosted(%ls)\n"), pSender->GetProperty("Class").GetChars());
  mInvalidatePosted = true;

  if (mMaxFPS == 0.0f) // Do we limit the FPS?
  {
    mpNGLWindow->Invalidate();
  }
}

void nuiMainWindow::BroadcastInvalidateLayout(nuiWidgetPtr pSender, bool BroadCastOnly)
{
  nuiTopLevel::BroadcastInvalidateLayout(pSender, BroadCastOnly);

  //NGL_OUT(_T("(Invalidate)BroadcastInvalidateLayout(%ls)\n"), pSender->GetProperty("Class").GetChars());
  mInvalidatePosted = true;

  if (mMaxFPS == 0.0f) // Do we limit the FPS?
  {
    mpNGLWindow->Invalidate();
  }

}

void nuiMainWindow::DBG_DisplayMouseOverObject()
{
  nglMouseInfo mouse;
  mpNGLWindow->GetMouse(mouse);

  nuiWidgetPtr pWidget = GetChild((nuiSize)mouse.X,(nuiSize)mouse.Y);
  if (pWidget)
  {
    if (!nuiGetBool(pWidget->GetInheritedProperty(_T("DebugObject")),true))
      return;

    nuiRect rect = pWidget->GetRect().Size();
    pWidget->LocalToGlobal(rect);
    nuiDrawContext* pContext = GetDrawContext();
    pContext->SetFillColor(nuiColor(.5f, .5f, 1.0f, 0.1f));
    pContext->SetStrokeColor(nuiColor(0.f, 0.f, 0.f, 0.2f));
    pContext->EnableBlending(true);
    pContext->SetBlendFunc(nuiBlendTransp);
    pContext->DrawRect(rect,eStrokeAndFillShape);
    pContext->EnableBlending(false);
  }
}

void nuiMainWindow::DBG_SetMouseOverObject(bool set)
{
  if (mDisplayMouseOverObject == set)
    return;
  mDisplayMouseOverObject = set;
  nuiTopLevel::Invalidate();
}

bool nuiMainWindow::DBG_GetMouseOverObject()
{
  return mDisplayMouseOverObject;
}


void nuiMainWindow::DBG_DisplayMouseOverInfo()
{
  nglMouseInfo mouse;
  mpNGLWindow->GetMouse(mouse);
  nuiWidgetPtr pWidget = GetChild((nuiSize)mouse.X,(nuiSize)mouse.Y);
  if (pWidget)
  {
    if (!nuiGetBool(pWidget->GetInheritedProperty(_T("DebugInfo")),true))
      return;

    nglString text;
    text.CFormat
      (
        _T("Class: '%ls'\nName: '%ls'\n"),
        pWidget->GetProperty(_T("Class")).GetChars(),
        pWidget->GetProperty(_T("Name")).GetChars()
      );
    nuiXMLNode* pNode = pWidget->Serialize(NULL,false);
    if (!pNode) // We have no information
      return;

    nuiXMLNode* pChild;
    while ((pChild = pNode->GetChild(0)))
      pNode->DelChild(pChild);
    nglString xmltext = pNode->Dump(0);
    xmltext.Replace(' ','\n');

    mpInfoLabel->SetText(text+xmltext);
  }

  nuiRect rect;
  rect = mpInfoLabel->GetIdealRect();
  // Keep at reasonable distance from mouse pointer
  rect.MoveTo((nuiSize)mouse.X+16, (nuiSize)mouse.Y+16);
  // Try to keep the tooltip inside the nuiMainWindow
  if (rect.mRight > GetRect().mRight)
    rect.Move(GetRect().mRight - rect.mRight, 0);
  if (rect.mBottom > GetRect().mBottom)
    rect.Move(0,GetRect().mBottom - rect.mBottom);
  mpInfoLabel->SetLayout(rect);

  nuiDrawContext* pContext = GetDrawContext();
  pContext->PushMatrix();
  float x = (float)mpInfoLabel->GetRect().mLeft;
  float y = (float)mpInfoLabel->GetRect().mTop;
  pContext->Translate( x, y );
  rect = mpInfoLabel->GetRect().Size();
  rect.mLeft -= 3;
  rect.mRight += 3;

  pContext->SetFillColor(nuiColor(1.f,1.f,1.f,.8f));
  pContext->SetStrokeColor(nuiColor(0.f,0.f,0.f,.3f));
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->DrawRect(rect, eStrokeAndFillShape);
  DrawChild(pContext, mpInfoLabel);
  pContext->PopMatrix();
  pContext->EnableBlending(false);
}

void nuiMainWindow::DBG_SetMouseOverInfo(bool set)
{
  if (mDisplayMouseOverInfo == set)
    return;
  mDisplayMouseOverInfo = set;
  nuiTopLevel::Invalidate();
}

bool nuiMainWindow::DBG_GetMouseOverInfo()
{
  return mDisplayMouseOverInfo;
}

void  nuiMainWindow::SetFrameRateLimit(float fps)
{
  mMaxFPS = fps;
  if (mMaxFPS < 0.0f)
    mMaxFPS = 0.0f;

  if (mpInvalidateTimer)
    delete mpInvalidateTimer;

  if (mMaxFPS > 0.0f)
  {
    mpInvalidateTimer = new nuiTimer(1.0f/fps);
    mpInvalidateTimer->Start();
    mMainWinSink.Connect(mpInvalidateTimer->Tick, &nuiMainWindow::InvalidateTimer);
  }
}

float nuiMainWindow::GetFrameRateLimit()
{
  return mMaxFPS;
}

bool nuiMainWindow::InvalidateTimer(const nuiEvent& rEvent)
{
  LazyPaint();
  return false;
}


bool nuiMainWindow::IsKeyDown (nglKeyCode Key) const
{
  return mpNGLWindow->IsKeyDown(Key);
}

bool nuiMainWindow::SetMouseCursor(nuiMouseCursor Cursor)
{
  return mpNGLWindow->SetCursor(Cursor);
}

bool nuiMainWindow::OnMouseMove(nglMouseInfo& rInfo)
{
  return CallMouseMove(rInfo);
}

bool nuiMainWindow::OnMouseClick(nglMouseInfo& rInfo)
{
  return CallMouseClick(rInfo);
}

bool nuiMainWindow::OnMouseUnclick(nglMouseInfo& rInfo)
{
  return CallMouseUnclick(rInfo);
}

bool nuiMainWindow::OnTextInput(const nglString& rUnicodeText)
{
  return CallTextInput(rUnicodeText);
}

bool nuiMainWindow::OnKeyUp(const nglKeyEvent& rEvent)
{
  return CallKeyUp(rEvent);
}

bool nuiMainWindow::OnKeyDown(const nglKeyEvent& rEvent)
{
  if (mDebugMode)
  {
    if (rEvent.mKey == NK_D && 
        (IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL)) && 
        (IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT))
       )
    {
      ShowWidgetInspector();
    }
    else if (rEvent.mKey == NK_T && 
        (IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL)) && 
        (IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT))
        )
    {
      nuiDrawContext* pCtx = GetDrawContext();
      nuiPainter* pPainter = pCtx->GetPainter();
      pPainter->DEBUG_EnableDrawArray(!pPainter->DEBUG_GetEnableDrawArray());
      
      InvalidateLayout();
    }
  }
  return CallKeyDown(rEvent);
}

bool nuiMainWindow::ShowWidgetInspector()
{
  if (mpInspectorWindow)
  {
    delete mpInspectorWindow;
    mpInspectorWindow = NULL;
  }
  else
  {
    nglWindowInfo Info;

    nglPath ResPath = nglPath(ePathCurrent);

    Info.Pos = nglWindowInfo::ePosAuto;

    Info.Width  = 800;
    Info.Height = 600;

    mpInspectorWindow = new nuiMainWindow(nuiContextInfo(nuiContextInfo::StandardContext2D), Info, GetNGLContext(), ResPath);
    mpInspectorWindow->SetQuitOnClose(false);
    mpInspectorWindow->AddChild(new nuiIntrospector(this));
    mpInspectorWindow->SetState(nglWindow::eShow);
    mMainWinSink.Connect(mpInspectorWindow->Destroyed, &nuiMainWindow::OnInspectorDeath);
  }

  return false;
}

bool nuiMainWindow::OnInspectorDeath(const nuiEvent& rEvent)
{
  mpInspectorWindow = NULL;
  return false;
}

void nuiMainWindow::SetDebugMode(bool Set)
{
  mDebugMode = Set;
}

void nuiMainWindow::BeginSession()
{
  mpNGLWindow->BeginSession();
}

nglContext* nuiMainWindow::GetNGLContext()
{
  return mpNGLWindow;
}

void nuiMainWindow::SetState(nglWindow::StateChange State)
{
  mpNGLWindow->SetState(State);
}


uint32 nuiMainWindow::GetWidth()
{
  return mpNGLWindow->GetWidth();
}

uint32 nuiMainWindow::GetHeight()
{
  return mpNGLWindow->GetHeight();
}

bool nuiMainWindow::SetSize (uint Width, uint Height)
{
  return mpNGLWindow->SetSize(Width, Height);
}

uint nuiMainWindow::GetError() const
{
  return mpNGLWindow->GetError();
}

const nglChar* nuiMainWindow::GetErrorStr() const
{
  return mpNGLWindow->GetErrorStr();
}

const nglChar* nuiMainWindow::GetErrorStr(uint Error) const
{
  return mpNGLWindow->GetErrorStr(Error);
}

nglWindow* nuiMainWindow::GetNGLWindow()
{
  return mpNGLWindow;
}

void nuiMainWindow::OnDragEnter()
{
  //NGL_OUT(_T("nuiMainWindow::OnDragEnter\n"));
}

void nuiMainWindow::OnDragLeave()
{
  //NGL_OUT(_T("nuiMainWindow::OnDragLeave\n"));
  if (mpWidgetCanDrop)
  {
    mpWidgetCanDrop->OnDropLeave();
    mpWidgetCanDrop = NULL;
  }
}


bool nuiMainWindow::OnCanDrop (nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button)
{
  //NGL_OUT(_T("nuiMainWindow::OnCanDrop\n"));
  
  nuiSize x = (nuiSize)X;
  nuiSize y = (nuiSize)Y;

  WidgetCanDrop Functor(pDragObject, x, y);
  nuiWidget* pWidget = GetChildIf(x,y, &Functor);
  if (!pWidget)
  {
    if (mpWidgetCanDrop)
    {
      mpWidgetCanDrop->OnDropLeave();
      mpWidgetCanDrop = NULL;
    }
    return false;
  }

  pWidget->GlobalToLocal(x, y);
  
  NGL_ASSERT(pWidget->OnCanDrop(pDragObject, x, y));
  
  if (mpWidgetCanDrop && pWidget != mpWidgetCanDrop)
    mpWidgetCanDrop->OnDropLeave();
  
  mpWidgetCanDrop = pWidget;
  return true;
}

void nuiMainWindow::OnDropped (nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button)
{
  nuiSize x = (nuiSize)X;
  nuiSize y = (nuiSize)Y;

  WidgetCanDrop Functor(pDragObject, x, y);
  nuiWidget* pWidget = GetChildIf(x,y, &Functor);
  NGL_ASSERT(pWidget);

  NGL_ASSERT(mpWidgetCanDrop == pWidget);

  pWidget->GlobalToLocal(x,y);
  pWidget->OnDropped(pDragObject, x, y, Button);
  mpWidgetCanDrop = NULL;
}

bool nuiMainWindow::Drag(nuiWidget* pDragSource, nglDragAndDrop* pDragObject) 
{ 
  //NGL_OUT(_T("nuiMainWindow::Drag\n"));
  
  mpDragSource = pDragSource;
  return mpNGLWindow->Drag(pDragObject); 
}

void nuiMainWindow::OnDragFeedback(nglDropEffect eDropEffect)
{
  //NGL_OUT(_T("nuiMainWindow::OnDragFeedback\n"));
}

void nuiMainWindow::OnDragged (nglDragAndDrop* pDragObject)
{
  //NGL_OUT(_T("nuiMainWindow::OnDragged\n"));
  NGL_ASSERT(mpDragSource);
  if (mpDragSource != this)
    mpDragSource->OnDragged(pDragObject); ///< fill data for supported types just before drop occurs
}

void nuiMainWindow::OnStopDragging()
{
  if (mpDragSource && mpDragSource != this)
    mpDragSource->OnStopDragging(); ///< advise drag source
}

///////////////////////////
nuiMainWindow::NGLWindow::NGLWindow(nuiMainWindow* pMainWindow, uint Width, uint Height, bool FullScreen)
: nglWindow(Width, Height, FullScreen),
  mpMainWindow(pMainWindow)
{
}

nuiMainWindow::NGLWindow::NGLWindow(nuiMainWindow* pMainWindow, const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglContext* pShared)
: nglWindow(rContextInfo, rInfo, pShared),
  mpMainWindow(pMainWindow)
{
}

nuiMainWindow::NGLWindow::~NGLWindow()
{
}

void nuiMainWindow::NGLWindow::OnPaint()
{
  mpMainWindow->OnPaint();
}

void nuiMainWindow::NGLWindow::OnResize(uint Width, uint Height)
{
  mpMainWindow->OnResize(Width, Height);
}

void nuiMainWindow::NGLWindow::OnCreation()
{
  mpMainWindow->OnCreation();
}

void nuiMainWindow::NGLWindow::OnDestruction() 
{ 
  if (mpMainWindow) 
    mpMainWindow->OnDestruction();
}

void nuiMainWindow::NGLWindow::OnActivation()
{
  mpMainWindow->OnActivation();
}

void nuiMainWindow::NGLWindow::OnDesactivation()
{
  mpMainWindow->OnDesactivation();
}

void nuiMainWindow::NGLWindow::OnClose()
{
  mpMainWindow->OnClose();
}

void nuiMainWindow::NGLWindow::OnState(StateInfo State)
{
  mpMainWindow->OnState(State);
}

bool nuiMainWindow::NGLWindow::OnTextInput(const nglString& rUnicodeText)
{
  return mpMainWindow->OnTextInput(rUnicodeText);
}

bool nuiMainWindow::NGLWindow::OnKeyDown(const nglKeyEvent& rEvent)
{
  return mpMainWindow->OnKeyDown(rEvent);
}

bool nuiMainWindow::NGLWindow::OnKeyUp(const nglKeyEvent& rEvent)
{
  return mpMainWindow->OnKeyUp(rEvent);
}

bool nuiMainWindow::NGLWindow::OnMouseClick(nglMouseInfo& rInfo)
{
  return mpMainWindow->OnMouseClick(rInfo);
}

bool nuiMainWindow::NGLWindow::OnMouseUnclick(nglMouseInfo& rInfo)
{
  return mpMainWindow->OnMouseUnclick(rInfo);
}

bool nuiMainWindow::NGLWindow::OnMouseMove(nglMouseInfo& rInfo)
{
  return mpMainWindow->OnMouseMove(rInfo);
}

// Dnd receive
void nuiMainWindow::NGLWindow::OnDragEnter()
{
  nglWindow::OnDragEnter(); 
  mpMainWindow->OnDragEnter();
}

void nuiMainWindow::NGLWindow::OnDragLeave()
{
  nglWindow::OnDragLeave(); mpMainWindow->OnDragLeave();
}

bool nuiMainWindow::NGLWindow::OnCanDrop(nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button)
{
  return mpMainWindow->OnCanDrop(pDragObject, X, Y, Button);
}

void nuiMainWindow::NGLWindow::OnDropped(nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button)
{
  mpMainWindow->OnDropped(pDragObject, X, Y, Button);
}

// Dnd send

bool nuiMainWindow::NGLWindow::Drag(nglDragAndDrop* pDragObject)
{
  return nglWindow::Drag(pDragObject);
}

void nuiMainWindow::NGLWindow::OnDragFeedback(nglDropEffect eDropEffect)
{
  nglWindow::OnDragFeedback(eDropEffect);
  mpMainWindow->OnDragFeedback(eDropEffect);
}

void nuiMainWindow::NGLWindow::OnDragged (nglDragAndDrop* pDragObject)
{
  nglWindow::OnDragged(pDragObject);
  mpMainWindow->OnDragged(pDragObject);
}

void nuiMainWindow::NGLWindow::OnStopDragging()
{
  nglWindow::OnStopDragging();
  mpMainWindow->OnStopDragging();
}

void nuiMainWindow::SetQuitOnClose(bool Set)
{
  mQuitOnClose = Set;
}

bool nuiMainWindow::GetQuitOnClose() const
{
  return mQuitOnClose;
}

void nuiMainWindow::EnterModalState()
{
  EmptyTrash();
  CancelGrab();
  mMouseInfo.Buttons = 0;
  mpNGLWindow->EnterModalState();
  EmptyTrash();
  CancelGrab();
  mMouseInfo.Buttons = 0;
}

void nuiMainWindow::ExitModalState()
{
  mpNGLWindow->ExitModalState();
}

void nuiMainWindow::SetMainMenu(nuiMainMenu* pMainMenu)
{
  mpNGLWindow->SetMainMenu(pMainMenu);
}

nuiMainMenu* nuiMainWindow::GetMainMenu()
{
  return mpNGLWindow->GetMainMenu();
}

