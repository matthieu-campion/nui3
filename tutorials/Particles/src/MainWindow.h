/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"


class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  bool Draw(nuiDrawContext* pContext);
  
private:
  double mLastTime;
  class Particle
  {
  public:
    Particle();
    
    void Update(float ElapsedTime);
    void Init(float MaxAge);
    void Recycle();
    
    float mX, mY; // Current position
    float mVX, mVY; // Current vector
    float mAge;
  };
  
  float mMaxAge;
  float mParticleSize;
  std::vector<Particle> mParticles;
  nuiTexture* mpTexture;
};

