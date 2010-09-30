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

const float MAX_PARTICLES = 1000;
const float PARTICLE_MAX_SIZE = 64;
const float PARTICLE_MIN_SIZE = 64;

class ParticleSystem : public nuiWidget
{
public:
  ParticleSystem()
  {
    srand(time(NULL));
    mParticles.resize(MAX_PARTICLES);
    mMaxAge = 3.0f;
    mLastTime = nglTime();
    for (uint32 i = 0; i < MAX_PARTICLES; i++)
      mParticles[i].Init(mMaxAge);
    
    mpTexture = nuiTexture::GetTexture(nglPath(_T("rsrc:/particle.png")));
    mpArray = new nuiRenderArray(GL_TRIANGLES, false, false); // Create the array as static and 3d
    mpArray->EnableArray(nuiRenderArray::eTexCoord, true);
    mpArray->EnableArray(nuiRenderArray::eColor, true);
    mpArray->Resize(MAX_PARTICLES * 4); /// 2 triangles per particle, 4 vertices per triangle
    
    for (uint32 i = 0; i < MAX_PARTICLES; i++)
    {
      const uint32 idx = i * 4;
      mpArray->SetTexCoords(idx + 0, 0, 0);// mpArray->PushVertex();
      mpArray->SetTexCoords(idx + 1, 1, 0);// mpArray->PushVertex();
      mpArray->SetTexCoords(idx + 2, 0, 1);// mpArray->PushVertex();
      mpArray->SetTexCoords(idx + 3, 1, 1);// mpArray->PushVertex();
    }
    
    mpArray->AddIndicesArray(GL_TRIANGLE_STRIP);
    for (uint32 i = 0; i < MAX_PARTICLES; i++)
    {
      const uint32 idx = i * 4;
      mpArray->PushIndex(idx);
      
      mpArray->PushIndex(idx);
      mpArray->PushIndex(idx + 1);
      mpArray->PushIndex(idx + 2);
      mpArray->PushIndex(idx + 3);

      mpArray->PushIndex(idx + 3);
    }
    
    StartAutoDraw();
  }

  ~ParticleSystem()
  {
    mpArray->Release();
  }
  
  
  bool Draw(nuiDrawContext* pContext)
  {
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
      age = nuiClamp(age, 0.0f, 1.0f);
      
      const float halfsize = (1.0 - age * .8) * mParticles[i].mSize; // Grow the particles as they age
      const nuiColor col(1.0f, 1.0f, 1.0f, age);
      
      const uint32 idx = i * 4;
      
      mpArray->SetVertex(idx + 0, x - halfsize, y - halfsize);
      mpArray->SetColor(idx + 0, col);
      
      mpArray->SetVertex(idx + 1, x + halfsize, y - halfsize);
      mpArray->SetColor(idx + 1, col);
      
      mpArray->SetVertex(idx + 2, x - halfsize, y + halfsize);
      mpArray->SetColor(idx + 2, col);
      
      mpArray->SetVertex(idx + 3, x + halfsize, y + halfsize);
      mpArray->SetColor(idx + 3, col);
      
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

  
private:
  double mLastTime;
  class Particle
  {
  public:
    Particle()
    {
      mX = 0;
      mY = 0;
      mVX = 0;
      mVY = 0;
      mAge = 0;
    }
    
    void Update(float ElapsedTime)
    {
      mX += mVX * ElapsedTime;
      mY += mVY * ElapsedTime;
      
      // Add some gravity:
      mVY += 150.0 * ElapsedTime;
      
      mAge += ElapsedTime;
    }
    
    void Recycle(float MaxAge)
    {
      mSize = ((float)rand() / (float)RAND_MAX);
      mSize = (float)PARTICLE_MIN_SIZE + (float)(PARTICLE_MAX_SIZE - PARTICLE_MAX_SIZE) * mSize;
      mAge = MaxAge * ((float)rand() / (float)RAND_MAX);
      mX = 0;
      mY = 0;
      
      const float xdirection = 0.0;
      const float ydirection = -1.0;
      
      const float MAX_SPEED = 100.0f;
      const float angle = 360.0 * ((float)rand()) / (float)RAND_MAX;
      const float speed = MAX_SPEED * ((float)rand()) / (float)RAND_MAX;
      const float rx = speed * sin(angle);
      const float ry = speed * cos(angle);
      mVX = rx + xdirection * MAX_SPEED;
      mVY = ry + ydirection * MAX_SPEED;
      //printf("%f\n", mVX * mVX + mVY * mVY);
    }
    
    void Init(float MaxAge)
    {
      Recycle(MaxAge);
      mAge = MaxAge * ((float)rand() / (float)RAND_MAX);
    }
    
    
    float mX, mY; // Current position
    float mVX, mVY; // Current vector
    float mAge;
    float mSize;
  };
  
  float mMaxAge;
  std::vector<Particle> mParticles;
  nuiTexture* mpTexture;
  nuiRenderArray* mpArray;
};


MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
{
  SetDebugMode(true);
  AddChild(new ParticleSystem);
}

MainWindow::~MainWindow()
{
}

