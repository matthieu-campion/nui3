/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/* Based on 3-D gear wheels by Brian Paul which is in the public domain
 */

#ifndef __Gears_h__
#define __Gears_h__

#include "ngl.h"
#include "nglTime.h"
#include "nuiDrawContext.h"

#define GEARS_MAX 8


class Gears
{
public:
  Gears(nuiDrawContext* pContext, int Width, int Height, bool IsWire);
  ~Gears();

  void MakeGear (GLfloat InnerRadius, GLfloat OuterRadius,
                 GLfloat Width,
                 GLint TeethCount, GLfloat TeethDepth,
                 bool IsWire);
  void Reshape (int Width, int Height);
  void Draw    (nuiDrawContext* pContext, nglTime TimeDelta, float Alpha);

private:
  GLfloat mAngleX,
          mAngleY,
          mAngleZ,
          mSelf;
  GLfloat mAngleXSpeed,
          mAngleYSpeed,
          mAngleZSpeed,
          mSelfSpeed;
  GLuint  mGears[GEARS_MAX];
  int     mGearCnt;
  bool    mIsWire;
};


#endif
