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

const float MAX_PARTICLES = 800;

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
  StartAutoDraw();
}

MainWindow::~MainWindow()
{
}

bool MainWindow::Draw(nuiDrawContext* pContext)
{
  nuiMainWindow::Draw(pContext);
  pContext->SetClearColor(nuiColor(10,0, 40));
  pContext->Clear();
  
  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLES);
  pArray->Reserve(MAX_PARTICLES);
  pArray->EnableArray(nuiRenderArray::eTexCoord, true);
  pArray->EnableArray(nuiRenderArray::eColor, true);
  
  nglTime now;
  float diff = now - mLastTime;
  mLastTime = now;

  float xoffset = GetRect().GetWidth() / 2;
  float yoffset = GetRect().GetHeight() / 2;
  
  float halfsize = mParticleSize / 2;
  
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

    nuiColor col(1.0f, 1.0f, 1.0f, age);
    
    pArray->SetColor(col); 
    pArray->SetVertex(x - halfsize, y - halfsize); pArray->SetTexCoords(0, 0); pArray->PushVertex();
    pArray->SetVertex(x + halfsize, y - halfsize); pArray->SetTexCoords(1, 0); pArray->PushVertex();
    pArray->SetVertex(x - halfsize, y + halfsize); pArray->SetTexCoords(0, 1); pArray->PushVertex();
    
    pArray->SetVertex(x + halfsize, y - halfsize); pArray->SetTexCoords(1, 0); pArray->PushVertex();
    pArray->SetVertex(x + halfsize, y + halfsize); pArray->SetTexCoords(1, 1); pArray->PushVertex();
    pArray->SetVertex(x - halfsize, y + halfsize); pArray->SetTexCoords(0, 1); pArray->PushVertex();
    
    if (mParticles[i].mAge >= mMaxAge)
      mParticles[i].Recycle();
  }
  
  pContext->SetFillColor(nuiColor(255, 255, 255));
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTranspAdd);
  pContext->SetTexture(mpTexture);
  pContext->EnableTexturing(true);
  pContext->DrawArray(pArray);
  
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

void MainWindow::Particle::Recycle()
{
  mAge = 0;
  mX = 0;
  mY = 0;

  const float xdirection = 0.0;
  const float ydirection = -1.0;
  
  const float MAX_SPEED = 100.0f;
  const float rx = ((float)random()) / (float)0x7FFFFFFF;
  const float ry = ((float)random()) / (float)0x7FFFFFFF;
  const float rrx = (rx - 0.5f) * 2.0f + xdirection;
  const float rry = (ry - 0.5f) * 2.0f + ydirection;
  mVX = rrx * MAX_SPEED;
  mVY = rry * MAX_SPEED;
  //printf("%f\n", mVX * mVX + mVY * mVY);
}

void MainWindow::Particle::Init(float MaxAge)
{
  Recycle();
  mAge = MaxAge * ((float)random() / (float)0x7FFFFFFF);
}

