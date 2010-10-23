/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSpriteView.h"
#include "nuiTexture.h"

// class nuiSpriteFrame
nuiSpriteFrame::nuiSpriteFrame()
: mpTexture(NULL),
  mX(0),
  mY(0)
{
}

nuiSpriteFrame::~nuiSpriteFrame()
{
  if (mpTexture)
    mpTexture->Release();
}

bool nuiSpriteFrame::SetTexture(nuiTexture* pTexture, const nuiRect& rRect)
{
  if (mpTexture)
    mpTexture->Release();
  mpTexture = pTexture;
  mpTexture->Acquire();
  mRect = rRect;
  return true;
}

bool nuiSpriteFrame::SetTexture(const nglPath& rPath, const nuiRect& rRect)
{
  if (mpTexture)
    mpTexture->Release();
  mpTexture = nuiTexture::GetTexture(rPath);
  mRect = rRect;
  return mpTexture != NULL;
}

bool nuiSpriteFrame::SetTexture(const nglPath& rPath)
{
  if (!SetTexture(rPath, nuiRect()))
    return false;
  
  mRect = nuiRect((float)mpTexture->GetWidth(), (float)mpTexture->GetHeight());

}

void nuiSpriteFrame::SetRect(const nuiRect& rRect)
{
  mRect = rRect;
}

void nuiSpriteFrame::SetHandle(float x, float y)
{
  mX = x;
  mY = y;
}

nuiTexture* nuiSpriteFrame::GetTexture() const
{
  return mpTexture;
}

const nuiRect& nuiSpriteFrame::GetRect()
{
  return mRect;
}

float nuiSpriteFrame::GetHandleX() const
{
  return mX;
}

float nuiSpriteFrame::GetHandleY() const
{
  return mY;
}

//////////////////////////////////////////////////////
// class nuiSpriteAnimation
nuiSpriteAnimation::nuiSpriteAnimation()
: mFPS(10)
{

}

nuiSpriteAnimation::~nuiSpriteAnimation()
{  
  for (int32 i = 0; i < mpFrames.size(); i++)
    delete mpFrames[i];
}

int32 nuiSpriteAnimation::GetFrameCount() const
{
  return mpFrames.size();
}

void nuiSpriteAnimation::AddFrame(nuiSpriteFrame* pFrame)
{
  mpFrames.push_back(pFrame);
}

const nuiSpriteFrame* nuiSpriteAnimation::GetFrame(int32 index) const
{
  return mpFrames[index];
}

void nuiSpriteAnimation::SetDuration(float seconds)
{
  mFPS = seconds / (float)GetFrameCount();
}

void nuiSpriteAnimation::SetFPS(float FPS)
{
  mFPS = FPS;
}

float nuiSpriteAnimation::GetDuration() const
{
  return mFPS * GetFrameCount();
}

float nuiSpriteAnimation::GetFPS() const
{
  return mFPS;
}
               
void nuiSpriteAnimation::SetName(const nglString& rName)
{
  mName = rName;
}

const nglString& nuiSpriteAnimation::GetName() const
{
  return mName;
}
               
