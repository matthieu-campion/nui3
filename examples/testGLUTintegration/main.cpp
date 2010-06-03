/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*

   What you need to get this example code working:

   MacOSX: nothing special, just build it in Xcode and it works...

   Windows: You need to download the GLUT precompiled libraries from http://www.opengl.org/resources/libraries/glut/glut_downloads.php#windows
            The glut.h header goes in either your global GL/ include directory or the local one in this directory.
            The libs should go to the local GL/ folder or to your system libraries folder.
            The dlls should either be in the windows/System32 folder or next to the executable.

*/
#include "nui.h"

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

#include "nuiInit.h"
#include "nuiGrid.h"

#include "nuiGLUTBridge.h"

double rx = 0., rz = 0.;
double translateX = 0.f, translateY = 0.f, translateZ = -10.f;
double scaleFactor = .5;
int mode = 0;

nuiGLUTBridge* gpBridge = NULL;


void glutDisplay(void)
{
  // Draw some OpenGL contents:
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* modify this line to change perspective values */
  gluPerspective(45.0, (float)glutGet(GLUT_WINDOW_WIDTH)/(float)glutGet(GLUT_WINDOW_HEIGHT), 1.0, 5000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_TRUE);
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

  // Draw NUI's Widget Tree:
  gpBridge->Display();
  
  // Swap the OpenGL buffer:
  glFlush();
  glutSwapBuffers();
}

void InitializeOGL()
{
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  
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
  // Simple and standard GLUT init:
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_RGBA | GLUT_MULTISAMPLE );
  glutInit( &argc, argv );
  glutCreateWindow("NUI over GLUT");
  InitializeOGL();

  // Init the basic NUI services (strings, streams, unicode, etc...)
  nuiInit(NULL);

  // Create the NUI bridge which also serves as the main window/widget tree:
  gpBridge = new nuiGLUTBridge();

  // Register the NUI bridge in GLUT:
  glutReshapeFunc(&nuiGLUTBridge::glutResize);       // called every time  the screen is resized
  glutDisplayFunc(glutDisplay);      // called when window needs to be redisplayed
  glutKeyboardFunc(&nuiGLUTBridge::glutKeyboard);    // called when the application receives a input from the keyboard
  glutMouseFunc(&nuiGLUTBridge::glutMouse);          // called when the application receives a input from the mouse
  glutMotionFunc(&nuiGLUTBridge::glutMotion);        // called when the mouse moves over the screen with one of this button pressed
  glutPassiveMotionFunc(&nuiGLUTBridge::glutMotion);        // called when the mouse moves over the screen with one of this button pressed
  glutIdleFunc(glutIdle);         // called whenever the application is idle

  // Create some stupid widgets:
  nuiGrid* pGrid = new nuiGrid(4, 4);
  for (uint i = 0; i < 4; i++)
  {
    pGrid->SetRowExpand(i, nuiExpandShrinkAndGrow);
    pGrid->SetColumnExpand(i, nuiExpandShrinkAndGrow);
    for (uint j = 0; j < 4; j++)
    {
      nglString str;
      str.CFormat(_T("btn(%d,%d)"), i, j);
      nuiButton* pButton = new nuiButton(str);
      pButton->SetPosition(nuiCenter);
      pGrid->SetCell(i, j, pButton);
    }
  }
  gpBridge->AddChild(pGrid);
  //gpBridge->OnResize(300, 300);

  // Execute the GLUT main loop:
  glutMainLoop();
  
  // Exit the application
  // First destroy the NUI bridge / widget tree:
  delete gpBridge;
  
  // Shutdown the basic NUI services:
  nuiUninit();
  
  return 0;
}
