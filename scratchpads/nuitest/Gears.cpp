/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*
 * 2002: added typecastings everywhere to make VC happy (meeloo)
 *       major clean up, using NGL code style (zerodeux)
 * 1999: ripped from xlockmore 4.x for NGL testing purposes :)
 *       ported from C to C++
 * 1996: "written" by Danny Sung <dannys@ucla.edu>
 *       Based on 3-D gear wheels by Brian Paul which is in the public domain.
 */

#include "nui.h"
#include <math.h>
#include <stdlib.h>
#include "Gears.h"


#define NRAND(x) ((float)(rand()%(x)))

Gears::Gears (nuiDrawContext* pContext, int Width, int Height, bool IsWire)
{
  static GLfloat red[4]   = { 0.8f, 0.1f, 0.0f, 1.0f };
  static GLfloat green[4] = { 0.0f, 0.8f, 0.2f, 1.0f };
  static GLfloat blue[4]  = { 0.2f, 0.2f, 1.0f, 1.0f };
  struct
  {
    GLfloat  R1, R2;
    GLfloat  W;
    GLint    T;
    GLfloat  D;
    GLfloat* C;
  } gears_def[] = {
    { 1.0f, 4.0f, 1.0f, 20, 0.7f, red },
    { 0.5f, 2.0f, 2.0f, 10, 0.7f, green },
    { 1.3f, 2.0f, 0.5f, 10, 0.7f, blue },
    { 0.0f, 0.0f, 0.0f, 0, 0.0f, NULL },
  };

  mAngleX = 0.0f;
  mAngleY = 0.0f;
  mAngleZ = 0.0f;
  mSelf = 0.0f;

  mAngleXSpeed = 45.0f;
  mAngleYSpeed = 95.0f;
  mAngleZSpeed = 65.0f;
  mSelfSpeed = 60.0f;

  mIsWire = IsWire;

  // Build the gears
  int i = 0;
  while (gears_def[i].T)
  {
    mGears[i] = glGenLists(1);
    glNewList(mGears[i], GL_COMPILE);
    MakeGear(gears_def[i].R1, gears_def[i].R2,
             gears_def[i].W,
             gears_def[i].T, gears_def[i].D,
             mIsWire);
    glEndList();
    i++;
  }
  mGearCnt = i;

  if (!mIsWire)
    glEnable(GL_NORMALIZE);
}


Gears::~Gears()
{
  int i;
  
  for (i = 0; i < mGearCnt; i++)
    if (glIsList(mGears[i])) glDeleteLists(mGears[i], 1);
}


void Gears::Draw(nuiDrawContext* pContext, nglTime TimeDelta, float Alpha)
{
  GLfloat pos[4] = {5.0f, 5.0f, 10.0f, 1.0f};
  GLfloat red[4]   = { 0.8f, 0.1f, 0.0f, Alpha };
  GLfloat green[4] = { 0.0f, 0.8f, 0.2f, Alpha };
  GLfloat blue[4]  = { 0.2f, 0.2f, 1.0f, Alpha };
  struct
  {
    GLfloat  R1, R2;
    GLfloat  W;
    GLint    T;
    GLfloat  D;
    GLfloat* C;
  } gears_def[] = {
    { 1.0f, 4.0f, 1.0f, 20, 0.7f, red },
    { 0.5f, 2.0f, 2.0f, 10, 0.7f, green },
    { 1.3f, 2.0f, 0.5f, 10, 0.7f, blue },
    { 0.0f, 0.0f, 0.0f, 0, 0.0f, NULL },
  };
//  glClearColor(0,0,0,0);
  //glDrawBuffer(GL_BACK);
/*
  if (!mIsWire)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  else
    glClear(GL_COLOR_BUFFER_BIT);
*/

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!mIsWire)
  {
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
  }

  glPushMatrix();

  glRotatef(mAngleX, 1.0, 0.0, 0.0);
  glRotatef(mAngleY, 0.0, 1.0, 0.0);
  glRotatef(mAngleZ, 0.0, 0.0, 1.0);
  
  glPushMatrix();
  glTranslatef(-3.0, -2.0, 0.0);
  glRotatef(mSelf, 0.0, 0.0, 1.0);
  if (mIsWire)
    glColor4fv(gears_def[0].C);
  else
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gears_def[0].C);
  glCallList(mGears[0]);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(3.1f, -2.0f, 0.0f);
  glRotatef(-2.0f * mSelf - 9.0f, 0.0f, 0.0f, 1.0f);
  if (mIsWire)
    glColor4fv(gears_def[1].C);
  else
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gears_def[1].C);
  glCallList(mGears[1]);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-3.1f, 4.2f, 0.0f);
  glRotatef(-2.0f * mSelf - 25.0f, 0.0f, 0.0f, 1.0f);
  if (mIsWire)
    glColor4fv(gears_def[2].C);
  else
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gears_def[2].C);
  glCallList(mGears[2]);
  glPopMatrix();
  
  glPopMatrix();
  
  /* let's do something so we don't get bored */
  mSelf += mSelfSpeed * (float)TimeDelta;
  mAngleX += mAngleXSpeed * (float)TimeDelta;
  mAngleY += mAngleYSpeed * (float)TimeDelta;
  mAngleZ += mAngleZSpeed * (float)TimeDelta;
  
//  glFinish(); (Someone may need to draw other things...)
  if (!mIsWire)
  {
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
  }
  
  if (Alpha > 0)
  {
    glDisable(GL_BLEND);
  }
}


void Gears::Reshape (int Width, int Height)
{
  GLfloat ratio = (GLfloat) Height / (GLfloat) Width;
  
//  glViewport(0, 0, (GLint) Width, (GLint) Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -ratio, ratio, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);
  
  /* The depth buffer will be cleared, if needed, before the
   * next frame.  Right now we just want to black the screen.
   */
  //glClear(GL_COLOR_BUFFER_BIT);
}


/*-
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  InnerRadius - radius of hole at center
 *         OuterRadius - radius at center of ToothCnt
 *         Width - Width of gear
 *         ToothCnt - number of ToothCnt
 *         TeethDepth - depth of tooth
 *         IsWire - true for IsWireframe mode
 */
void Gears::MakeGear (GLfloat InnerRadius, GLfloat OuterRadius,
                      GLfloat Width,
                      GLint ToothCnt, GLfloat TeethDepth,
                      bool IsWire)
{
  GLint   i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = InnerRadius;
  r1 = OuterRadius - TeethDepth / 2.0f;
  r2 = OuterRadius + TeethDepth / 2.0f;

  da = 2.0f * (float)M_PI / ToothCnt / 4.0f;

  glShadeModel(GL_SMOOTH);

  /* This subroutine got kind of messy when I added all the checks
   * for IsWireframe mode.  A much cleaner solution that I sometimes
   * use is to have a variable hold the value GL_LINE_LOOP when
   * in IsWireframe mode, or hold the value GL_POLYGON otherwise.
   * Then I just call glBegin(that_variable), give my polygon
   * coordinates, and glEnd().  Pretty neat eh?  Too bad I couldn't
   * integrate that trick here.
   *                                  --Ed.
   */

  if (!IsWire)
    glNormal3f(0.0f, 0.0f, 1.0f);

  /* draw front face */
  if (!IsWire)
    glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= ToothCnt; i++)
  {
    if (IsWire)
      glBegin(GL_LINES);
    angle = i * 2.0f * (float)M_PI / ToothCnt;
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), Width * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), Width * 0.5f);
    if (!IsWire)
    {
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), Width * 0.5f);
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), Width * 0.5f);
    }
    else
    {
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), Width * 0.5f);
      glVertex3f(r1 * (float)cos(angle + 4 * da), r1 * (float)sin(angle + 4 * da), Width * 0.5f);
      glEnd();
    }
  }
  if (!IsWire)
    glEnd();

  /* draw front sides of ToothCnt */
  if (!IsWire)
    glBegin(GL_QUADS);
  da = 2.0f * (float)M_PI / ToothCnt / 4.0f;
  for (i = 0; i < ToothCnt; i++) {
    angle = i * 2.0f * (float)M_PI / ToothCnt;

    if (IsWire)
      glBegin(GL_LINE_LOOP);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), Width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), Width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), Width * 0.5f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), Width * 0.5f);
    if (IsWire)
      glEnd();
  }
  if (!IsWire)
    glEnd();


  if (!IsWire)
    glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  if (!IsWire)
    glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= ToothCnt; i++)
  {
    angle = i * 2.0f * (float)M_PI / ToothCnt;
    if (IsWire)
      glBegin(GL_LINES);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -Width * 0.5f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -Width * 0.5f);
    if (!IsWire)
    {
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -Width * 0.5f);
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -Width * 0.5f);
    }
    else
    {
      glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -Width * 0.5f);
      glVertex3f(r1 * (float)cos(angle + 4 * da), r1 * (float)sin(angle + 4 * da), -Width * 0.5f);
      glEnd();
    }
  }
  if (!IsWire)
    glEnd();

  /* draw back sides of ToothCnt */
  if (!IsWire)
    glBegin(GL_QUADS);
  da = 2.0f * (float)M_PI / ToothCnt / 4.0f;
  for (i = 0; i < ToothCnt; i++) {
    angle = i * 2.0f * (float)M_PI / ToothCnt;

    if (IsWire)
      glBegin(GL_LINE_LOOP);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -Width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -Width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -Width * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -Width * 0.5f);
    if (IsWire)
      glEnd();
  }
  if (!IsWire)
    glEnd();


  /* draw outward faces of ToothCnt */
  if (!IsWire)
    glBegin(GL_QUAD_STRIP);
  for (i = 0; i < ToothCnt; i++)
  {
    angle = i * 2.0f * (float)M_PI / ToothCnt;

    if (IsWire)
      glBegin(GL_LINES);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), Width * 0.5f);
    glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -Width * 0.5f);
    u = r2 * (float)cos(angle + da) - r1 * (float)cos(angle);
    v = r2 * (float)sin(angle + da) - r1 * (float)sin(angle);
    len = (float)sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), Width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -Width * 0.5f);
    glNormal3f((float)cos(angle), (float)sin(angle), 0.0f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), Width * 0.5f);
    glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -Width * 0.5f);
    u = r1 * (float)cos(angle + 3 * da) - r2 * (float)cos(angle + 2 * da);
    v = r1 * (float)sin(angle + 3 * da) - r2 * (float)sin(angle + 2 * da);
    glNormal3f(v, -u, 0.0f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), Width * 0.5f);
    glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -Width * 0.5f);
    glNormal3f((float)cos(angle), (float)sin(angle), 0.0);
    if (IsWire)
      glEnd();
  }

  if (!IsWire)
  {
    glVertex3f(r1 * (float)cos(0.0f), r1 * (float)sin(0.0f), Width * 0.5f);
    glVertex3f(r1 * (float)cos(0.0f), r1 * (float)sin(0.0f), -Width * 0.5f);
    glEnd();
  }
  if (!IsWire)
    glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  if (!IsWire)
    glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= ToothCnt; i++)
  {
    angle = i * 2.0f * (float)M_PI / ToothCnt;
    if (IsWire)
      glBegin(GL_LINES);
    glNormal3f(-(float)cos(angle), -(float)sin(angle), 0.0f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -Width * 0.5f);
    glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), Width * 0.5f);
    if (IsWire)
    {
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -Width * 0.5f);
      glVertex3f(r0 * (float)cos(angle + 4 * da), r0 * (float)sin(angle + 4 * da), -Width * 0.5f);
      glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), Width * 0.5f);
      glVertex3f(r0 * (float)cos(angle + 4 * da), r0 * (float)sin(angle + 4 * da), Width * 0.5f);
      glEnd();
    }
  }
  if (!IsWire)
    glEnd();
}
