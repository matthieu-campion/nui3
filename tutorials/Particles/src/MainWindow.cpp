/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"


/*
 * MainWindow
 */

const float MAX_PARTICLES = 200;

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
{
  SetDebugMode(true);
  
  srand(time(NULL));
  mParticles.resize(MAX_PARTICLES);
  mMaxAge = 3.0f;
  mLastTime = nglTime();
  for (uint32 i = 0; i < MAX_PARTICLES; i++)
    mParticles[i].Init(mMaxAge);
   
  mParticleSize = 32;
  
  mpTexture = nuiTexture::GetTexture(nglPath(_T("rsrc:/particle.png")));
  mpArray = new nuiRenderArray(GL_TRIANGLES, false, false); // Create the array as static and 3d
  mpArray->EnableArray(nuiRenderArray::eTexCoord, true);
  mpArray->EnableArray(nuiRenderArray::eColor, true);
  mpArray->Resize(MAX_PARTICLES * 2 * 3); /// 2 triangles per particle, 3 vertices per triangle
  
  for (uint32 i = 0; i < MAX_PARTICLES; i++)
  {
    uint32 idx = i * 6;
    mpArray->SetTexCoords(idx + 0, 0, 0);// mpArray->PushVertex();
    mpArray->SetTexCoords(idx + 1, 1, 0);// mpArray->PushVertex();
    mpArray->SetTexCoords(idx + 2, 0, 1);// mpArray->PushVertex();
    
    mpArray->SetTexCoords(idx + 3, 1, 0);// mpArray->PushVertex();
    mpArray->SetTexCoords(idx + 4, 1, 1);// mpArray->PushVertex();
    mpArray->SetTexCoords(idx + 5, 0, 1);// mpArray->PushVertex();
    
  }
    
  StartAutoDraw();
}

MainWindow::~MainWindow()
{
  mpArray->Release();
}

bool MainWindow::Draw(nuiDrawContext* pContext)
{
  nuiMainWindow::Draw(pContext);
  pContext->SetClearColor(nuiColor(10,0, 40));
  pContext->Clear();


  nglTime now;
  float diff = now - mLastTime;
  mLastTime = now;

  float xoffset = GetRect().GetWidth() / 2;
  float yoffset = GetRect().GetHeight() / 2;

  for (uint32 i = 0; i < MAX_PARTICLES; i++)
  {
    mParticles[i].Update(diff);
    const float x = mParticles[i].mX + xoffset;
    const float y = mParticles[i].mY + yoffset;
    float age = 1.0 - (mParticles[i].mAge / mMaxAge);
    if (age > 1.0f)
      age = 1.0f;
    else if (age < 0.0f)
      age = 0.0f;

    float halfsize = (1.0 - age * .8) * mParticleSize; // Grow the particles as they age
    nuiColor col(1.0f, 1.0f, 1.0f, age);

    uint32 idx = i * 6;

    mpArray->SetColor(idx + 0, col);
    mpArray->SetColor(idx + 1, col);
    mpArray->SetColor(idx + 2, col);
    mpArray->SetColor(idx + 3, col);
    mpArray->SetColor(idx + 4, col);
    mpArray->SetColor(idx + 5, col);

    mpArray->SetVertex(idx + 0, x - halfsize, y - halfsize); mpArray->SetTexCoords(idx + 0, 0, 0);
    mpArray->SetVertex(idx + 1, x + halfsize, y - halfsize); mpArray->SetTexCoords(idx + 1, 1, 0);
    mpArray->SetVertex(idx + 2, x - halfsize, y + halfsize); mpArray->SetTexCoords(idx + 2, 0, 1);

    mpArray->SetVertex(idx + 3, x + halfsize, y - halfsize); mpArray->SetTexCoords(idx + 3, 1, 0);
    mpArray->SetVertex(idx + 4, x + halfsize, y + halfsize); mpArray->SetTexCoords(idx + 4, 1, 1);
    mpArray->SetVertex(idx + 5, x - halfsize, y + halfsize); mpArray->SetTexCoords(idx + 5, 0, 1);

    if (mParticles[i].mAge >= mMaxAge)
      mParticles[i].Recycle(mMaxAge);
  }

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTranspAdd);
  pContext->SetTexture(mpTexture);
  pContext->EnableTexturing(true);
  mpArray->Acquire();
  pContext->DrawArray(mpArray);
  
  return true;
}


MainWindow::Particle::Particle()
{
  mX = 0;
  mY = 0;
  mVX = 0;
  mVY = 0;
  mAge = 0;
}

void MainWindow::Particle::Update(float ElapsedTime)
{
  mX += mVX * ElapsedTime;
  mY += mVY * ElapsedTime;
  
  // Add some gravity:
  mVY += 150.0 * ElapsedTime;
  
  mAge += ElapsedTime;
}

void MainWindow::Particle::Recycle(float MaxAge)
{
  mAge = MaxAge * ((float)random() / (float)0x7FFFFFFF);
  mX = 0;
  mY = 0;

  const float xdirection = 0.0;
  const float ydirection = -1.0;
  
  const float MAX_SPEED = 100.0f;
  const float angle = 360.0 * ((float)random()) / (float)0x7FFFFFFF;
  const float speed = MAX_SPEED * ((float)random()) / (float)0x7FFFFFFF;
  const float rx = speed * sin(angle);
  const float ry = speed * cos(angle);
  mVX = rx + xdirection * MAX_SPEED;
  mVY = ry + ydirection * MAX_SPEED;
  //printf("%f\n", mVX * mVX + mVY * mVY);
}

void MainWindow::Particle::Init(float MaxAge)
{
  Recycle(MaxAge);
  mAge = MaxAge * ((float)random() / (float)0x7FFFFFFF);
}

