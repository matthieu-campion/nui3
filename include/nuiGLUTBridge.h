/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

extern void* gmpNUI_GLUTBridge;

class nuiGLUTBridge : public nglContext, public nuiTopLevel
{
public:
  nuiGLUTBridge()
  : nuiTopLevel(_T(""))
  {
    *((nuiGLUTBridge**)&gmpNUI_GLUTBridge) = this;

    mWidth = glutGet(GLUT_WINDOW_WIDTH);
    mHeight = glutGet(GLUT_WINDOW_HEIGHT);
    mClearBackground = false;
    EnableRenderCache(true);
    EnablePartialRedraw(false);    
    SetRect(nuiRect(0.0f, 0.0f, (nuiSize)mWidth, (nuiSize)mHeight));
    
#ifdef WIN32
    BuildOpenGLFromExisting(NULL, NULL);
#elif defined __APPLE__
    BuildOpenGLFromExisting(NULL, aglGetCurrentContext());
#else
    BuildOpenGLFromExisting(glXGetCurrentContext());
#endif
  }
  
  virtual ~nuiGLUTBridge()
  {
    *((nuiGLUTBridge**)&gmpNUI_GLUTBridge) = this;
  }
  
  void Display()
  {
    Paint();
  }
  
  ///< GLUT call backs:
  //------------------------------------------------------------------------
  // Window resize function
  //------------------------------------------------------------------------
  static void glutResize(int width, int height)
  { 
    ((nuiGLUTBridge*)gmpNUI_GLUTBridge)->OnResize(width, height);
  }
  
  //------------------------------------------------------------------------
  // Function that handles keyboard inputs
  //------------------------------------------------------------------------
  static void glutKeyboard(unsigned char key, int x, int y)
  {
  }
  
  //------------------------------------------------------------------------
  // Moves the screen based on mouse pressed button
  //------------------------------------------------------------------------
  static void glutMotion(int x, int y)
  {
    nglMouseInfo Info;
    Info.Buttons = 0;
    Info.X = x;
    Info.Y = y;
    ((nuiGLUTBridge*)gmpNUI_GLUTBridge)->OnMouseMove(Info);
  }
  
  //------------------------------------------------------------------------
  // Function that handles mouse input
  //------------------------------------------------------------------------
  static void glutMouse(int button, int state, int x, int y)
  {
    nglMouseInfo Info;
    Info.Buttons = 0;
    switch (button)
    {
      case 0:
        Info.Buttons = nglMouseInfo::ButtonLeft;
        break;
      case 1:
        Info.Buttons = nglMouseInfo::ButtonRight;
        break;
      case 2:
        Info.Buttons = nglMouseInfo::ButtonMiddle;
        break;
    }
    Info.X = x;
    Info.Y = y;
    if (state)
      ((nuiGLUTBridge*)gmpNUI_GLUTBridge)->OnMouseUnclick(Info);
    else
      ((nuiGLUTBridge*)gmpNUI_GLUTBridge)->OnMouseClick(Info);
    
  }
  
protected:
  
  void Paint()
  {
    do 
    {
      FillTrash();
      
      GetIdealRect();
      SetLayout(nuiRect(0, 0, mWidth, mHeight));
      
      EmptyTrash();
    } while (IsTrashFull());
    
    
    FillTrash();
    
    nuiDrawContext* pContext = GetDrawContext();
    nuiPainter* pOldPainter = pContext->GetPainter();
    
    pContext->GetPainter()->ResetStats();
    
    BeginSession();
    
    pContext->StartRendering();
    pContext->GetPainter()->SetState(nuiRenderState(), true);
    
    //static int counter = 0;
    //NGL_OUT(_T("%d OnPaint %d - %d\n"), counter++, DrawFullFrame, RestorePartial);
    
    pContext->LoadIdentity();
    
    pContext->MultMatrix(GetMatrix());
    DrawWidget(pContext);
    
    pContext->StopRendering();
    EmptyTrash();
    
    //   pContext->EndSession();
    pContext->SetPainter(pOldPainter);
    
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_TEXTURE_2D);
  }
  
  void OnResize(uint Width, uint Height)
  {
    mWidth = Width;
    mHeight = Height;
    FillTrash();
    nuiRect Rect;
    Rect.mRight=(nuiSize)Width;
    Rect.mBottom=(nuiSize)Height;
    //SetLayout(Rect);
    
    nuiDrawContext* pCtx = GetDrawContext();
    pCtx->SetSize(Width,Height);
    
    //NGL_OUT(_T("(OnResize)nglWindow::Invalidate()\n"));;
    InvalidateLayout();
    EmptyTrash();
  }
  
  void OnCreation()
  {
    EmptyTrash();
  }
  
  void OnDestruction()
  {
    //NGL_OUT(_T("OnDestruction\n"));
    EmptyTrash();
  }
  
  void OnActivation()
  {
    //OUT("OnActivation\n");
    EmptyTrash();
    CancelGrab();
    mMouseInfo.Buttons = 0;
  }
  
  void OnDesactivation()
  {
    //OUT("OnDesactivation\n");
    EmptyTrash();
    CancelGrab();
    mMouseInfo.Buttons = 0;
  }
  
  void OnState (nglWindow::StateInfo State)
  {
    //OUT("OnState\n");
    EmptyTrash();
  }
  
  
  void BroadcastInvalidate(nuiWidgetPtr pSender)
  {
    nuiTopLevel::BroadcastInvalidate(pSender);
    
    glutPostRedisplay();
  }
  
  void BroadcastInvalidateRect(nuiWidgetPtr pSender, const nuiRect& rRect)
  {
    nuiTopLevel::BroadcastInvalidateRect(pSender, rRect);
    
    glutPostRedisplay();
  }
  
  void BroadcastInvalidateLayout(nuiWidgetPtr pSender, bool BroadCastOnly)
  {
    nuiTopLevel::BroadcastInvalidateLayout(pSender, BroadCastOnly);
    glutPostRedisplay();
  }
  
  bool IsKeyDown (nglKeyCode Key) const
  {
    //return mpNGLWindow->IsKeyDown(Key);
    return false;
  }
  
  bool SetMouseCursor(nuiMouseCursor Cursor)
  {
    //return mpNGLWindow->SetCursor(Cursor);
    return true;
  }
  
  bool OnMouseMove(nglMouseInfo& rInfo)
  {
    return CallMouseMove(rInfo);
  }
  
  bool OnMouseClick(nglMouseInfo& rInfo)
  {
    return CallMouseClick(rInfo);
  }
  
  bool OnMouseUnclick(nglMouseInfo& rInfo)
  {
    return CallMouseUnclick(rInfo);
  }
  
  bool OnKeyUp(const nglKeyEvent& rEvent)
  {
    return CallKeyUp(rEvent);
  }
  
  bool OnKeyDown(const nglKeyEvent& rEvent)
  {
    return CallKeyDown(rEvent);
  }
  
  virtual nglContext* GetNGLContext()
  {
    return this;
  }
  
  virtual void BeginSession()
  {
    
  }
  
  virtual bool MakeCurrent() const
  {
    return true;
  }
  
  virtual void EndSession()
  {
    
  }
  
  nglContext* GetNGLContext() const
  {
    return const_cast<nglContext*>((nglContext*)this);
  }
  
  
  
  
  
protected:
  uint32 mWidth, mHeight;
};

