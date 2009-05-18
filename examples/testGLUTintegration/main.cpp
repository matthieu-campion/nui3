/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <iostream>
#include <cmath>
#include <string>

#include "nui.h"
#include "nuiInit.h"
#include "MainWindow.h"
#include "nuiGLPainter.h"
#include "nuiSoftwarePainter.h"
#include "nuiGrid.h"

class nuiStandaloneDisplay : public nuiTopLevel, public nglContext
{
public:
  nuiStandaloneDisplay(uint32 width, uint32 height)
  : nuiTopLevel(_T(""))
  {
    mWidth = width;
    mHeight = height;
    mClearBackground = false;
    EnableRenderCache(true);
    EnablePartialRedraw(false);    
    SetRect(nuiRect(0.0f, 0.0f, (nuiSize)width, (nuiSize)height));

    //BuildOpenGLFromExisting(NULL, aglGetCurrentContext());
  }
  
  virtual ~nuiStandaloneDisplay()
  {
    
  }

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
    nuiSoftwarePainter* pCTX = dynamic_cast<nuiSoftwarePainter*>(pContext->GetPainter());
    
    BeginSession();
    
    pContext->StartRendering();
    
    static int counter = 0;
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

double rx = 0., rz = 0.;
double translateX = 0.f, translateY = 0.f, translateZ = -10.f;
double scaleFactor = .5;
int mode = 0;
int gWidth, gHeight;

nuiStandaloneDisplay* pDisplay = NULL;


//------------------------------------------------------------------------
// Window resize function
//------------------------------------------------------------------------
void glutResize(int width, int height)
{ 
//  glViewport(0, 0, gWidth, gHeight);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  glDisable(GL_STENCIL_TEST);
//  /* modify this line to change perspective values */
//  gluPerspective(45.0, (float)gWidth/(float)gHeight, 1.0, 5000.0);
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  gWidth = width;
  gHeight = height;
  
  pDisplay->OnResize(width, height);
}

//------------------------------------------------------------------------
// Function that handles keyboard inputs
//------------------------------------------------------------------------
void glutKeyboard(unsigned char key, int x, int y)
{
}

//------------------------------------------------------------------------
// Moves the screen based on mouse pressed button
//------------------------------------------------------------------------
void glutMotion(int x, int y)
{
  nglMouseInfo Info;
  Info.Buttons = 0;
  Info.X = x;
  Info.Y = y;
  pDisplay->OnMouseMove(Info);
}

//------------------------------------------------------------------------
// Function that handles mouse input
//------------------------------------------------------------------------
void glutMouse(int button, int state, int x, int y)
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
    pDisplay->OnMouseUnclick(Info);
  else
    pDisplay->OnMouseClick(Info);
  
}


void glutDisplay(void)
{
  // Display the NUI stuff:
  pDisplay->Paint();
  
  glViewport(0, 0, gWidth, gHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* modify this line to change perspective values */
  gluPerspective(45.0, (float)gWidth/(float)gHeight, 1.0, 5000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  glLoadIdentity();
  
  glColor3f(1.f,0.f,0.f);
  glTranslatef(translateX, translateY, translateZ);
  glScalef(2, 2, 2);
  glRotatef(rx, 0, 1, 0);
  glRotatef(rz, 1, 0, 0);
  
  double vectA[4] = {-1,1,0, 1};
  double vectB[4] = {1,1,0, 1};
  double vectC[4] = {1,-1,0, 1};
  double vectD[4] = {-1,-1,0, 1};
  
  double dx1 = vectB[0] - vectA[0];
  double dy1 = vectB[1] - vectA[1];
  double dx2 = vectC[0] - vectD[0];
  double dy2 = vectC[1] - vectD[1];
  for (int i=0;i<=20;++i) 
  {
    double x1=vectA[0]+dx1*i/20.0;
    double y1=vectA[1]+dy1*i/20.0;
    double x2=vectD[0]+dx2*i/20.0;
    double y2=vectD[1]+dy2*i/20.0;
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
  }
  dx1=vectD[0]-vectA[0];
  dy1=vectD[1]-vectA[1];
  dx2=vectC[0]-vectB[0];
  dy2=vectC[1]-vectB[1];
  for (int i=0;i<=20;++i) 
  {
    double x1=vectA[0]+dx1*i/20.0;
    double y1=vectA[1]+dy1*i/20.0;
    double x2=vectB[0]+dx2*i/20.0;
    double y2=vectB[1]+dy2*i/20.0;
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
  }
  
  glFlush();
  glutSwapBuffers();
}

void InitializeOGL()
{
//  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
//  glShadeModel(GL_SMOOTH);
//  glEnable(GL_DEPTH_TEST);
  
  return;
}

void glutIdle()
{
  rx += 0.5f;
  rz += 0.2f;
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_RGBA | GLUT_MULTISAMPLE );
  
  glutInit( &argc, argv );
  
  glutCreateWindow("NUI2 over GLUT");
  
  glutReshapeFunc(glutResize);       // called every time  the screen is resized
  glutDisplayFunc(glutDisplay);      // called when window needs to be redisplayed
  glutIdleFunc(glutIdle);         // called whenever the application is idle
  glutKeyboardFunc(glutKeyboard);    // called when the application receives a input from the keyboard
  glutMouseFunc(glutMouse);          // called when the application receives a input from the mouse
  glutMotionFunc(glutMotion);        // called when the mouse moves over the screen with one of this button pressed
  glutPassiveMotionFunc(glutMotion);        // called when the mouse moves over the screen with one of this button pressed
  InitializeOGL();
  
  nuiInit(NULL);
  pDisplay = new nuiStandaloneDisplay(256, 256);
  
  nuiGrid* pGrid = new nuiGrid(10, 10);
  for (uint i = 0; i < 5; i++)
  {
    pGrid->SetRowExpand(i, nuiExpandShrinkAndGrow);
    pGrid->SetColumnExpand(i, nuiExpandShrinkAndGrow);
    for (uint j = 0; j < 5; j++)
    {
      nglString str;
      str.CFormat(_T("btn(%d,%d)"), i, j);
      nuiButton* pButton = new nuiButton(str);
      pButton->SetPosition(nuiCenter);
      pGrid->SetCell(i, j, pButton);
    }
  }
  pDisplay->AddChild(pGrid);
  pDisplay->OnResize(300, 300);

  glutMainLoop();
  
  return 0;
}
