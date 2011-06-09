/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiTextLayout.h"


nuiTextLine::nuiTextLine(const nuiTextLayout& rLayout, float X, float Y)
: mLayout(rLayout), mX(X), mY(Y)
{
}

nuiTextLine::~nuiTextLine()
{
  for (uint32 i = 0; i < mpRuns.size(); i++)
    mpRuns[i]->Release();
}

/** @name Drawing the Line */
//@{
void nuiTextLine::Draw(nuiDrawContext* pContext)
{
}
//@}

const std::vector<nuiTextRun*>& nuiTextLine::GetGlyphRuns() const
{
  return mpRuns;
}

float nuiTextLine::GetX() const
{
  return mX;
}

float nuiTextLine::GetY() const
{
  return mY;
}

void nuiTextLine::SetPosition(float X, float Y)
{
  mX = X;
  mY = Y;
}

void nuiTextLine::AddRun(nuiTextRun* pRun)
{
  pRun->Acquire();
  mpRuns.push_back(pRun);
}

int32 nuiTextLine::GetRunCount() const
{
  return mpRuns.size();
}

nuiTextRun* nuiTextLine::GetRun(int32 index) const
{
  return mpRuns[index];
}

float nuiTextLine::GetAdvanceY() const
{
  float y = 0;
  for (uint32 i = 0; i < mpRuns.size(); i++)
  {
    y = MAX(y, mpRuns[i]->GetAdvanceY());
  }
  
  return y;
}

float nuiTextLine::GetAdvanceX() const
{
  float x = 0;
  for (uint32 i = 0; i < mpRuns.size(); i++)
  {
    x += mpRuns[i]->GetAdvanceX();
  }
  
  return x;
}

float nuiTextLine::GetAscender() const
{
  float v = 0;
  for (uint32 i = 0; i < mpRuns.size(); i++)
  {
    v = MAX(v, mpRuns[i]->GetAscender());
  }
  
  return v;
}

float nuiTextLine::GetDescender() const
{
  float v = 0;
  for (uint32 i = 0; i < mpRuns.size(); i++)
  {
    v = MIN(v, mpRuns[i]->GetAscender());
  }
  
  return v;
}

