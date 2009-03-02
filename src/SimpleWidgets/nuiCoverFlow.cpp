/*
 *  nuiCoverFlow.cpp
 *  nui3
 *
 *  Created by meeloo on 2/25/09.
 *  Copyright 2009 mxp4. All rights reserved.
 *
 */

#include "nuiCoverFlow.h"

nuiCoverFlow::nuiCoverFlow()
: mSelectedImage(0),
  mBackground(0, 0, 0, 255),
  mReflectionStart(0.3),
  mReflectionEnd(0.0),
  mDrawBackground(true),
  mYOffset(0.0f),
  mPos(0.0f),
  mLastTime(nglTime()),
  mTimer(1.0f / 30.0f),
  mFlowSink(this)
{
  mFlowSink.Connect(mTimer.Tick, &nuiCoverFlow::OnUpdateTime);
}

nuiCoverFlow::~nuiCoverFlow()
{
  mTimer.Stop();
  for (uint32 i = 0; i < mImages.size(); i++)
    mImages[i]->Release();
}

void nuiCoverFlow::DrawCard(nuiDrawContext* pContext, uint32 index, float start, float end)
{
  nuiTexture* pTex = mImages[index];
  // Draw the selected image:
  float imgw = pTex->GetWidth();
  float imgh = pTex->GetHeight();
  float s = MAX(imgw, imgh);
  float ratiow = imgw / s;
  float ratioh = imgh / s;
  nuiRect imgrect(0.0f, 0.0f, ratiow, ratioh);
  nuiRect r(-.5, mYOffset, 1.0f, 1.0f);
  imgrect.SetPosition(nuiTop, r);
  float tw = imgw;
  float th = imgh;
  pTex->ImageToTextureCoord(tw, th);
  
  float vx0 = imgrect.Left();
  float vy0 = imgrect.Top();
  float vx1 = imgrect.Right();
  float vy1 = imgrect.Bottom();
  float vw = imgrect.GetWidth();
  float vh = imgrect.GetHeight();
  
  nuiColor blank(1.0f, 1.0f, 1.0f, 1.0f);
  
  nuiRenderArray array(GL_TRIANGLE_STRIP);
  array.EnableArray(nuiRenderArray::eColor, true);
  array.EnableArray(nuiRenderArray::eTexCoord, true);
  nuiColor c0(mBackground);
  c0.Mix(blank, 1.0-start);
  array.SetColor(c0);
  array.SetVertex(vx0, vy0);
  array.SetTexCoords(0, th);
  array.PushVertex();
  
  array.SetVertex(vx1, vy0);
  array.SetTexCoords(tw, th);
  array.PushVertex();
  
  nuiColor c1(mBackground);
  c1.Mix(blank, 1.0-end);
  array.SetColor(c1);
  array.SetVertex(vx0, vy1);
  array.SetTexCoords(0, 0);
  array.PushVertex();
  
  array.SetVertex(vx1, vy1);
  array.SetTexCoords(tw, 0);
  array.PushVertex();
  
  pContext->EnableTexturing(true);
  pContext->SetTexture(pTex);
  pContext->DrawArray(array);  
}

bool nuiCoverFlow::Draw(nuiDrawContext* pContext)
{
  uint32 image = ToNearest(mPos);
  float fractional = mPos - image;
  float fract = 1.0 - fractional;
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);

  nuiRect rect(GetRect().Size());
  if (mDrawBackground)
  {
    // Draw a rectangle with the background color:
    pContext->SetFillColor(mBackground);
    pContext->DrawRect(rect, eFillShape);          
  }
  
  if (mImages.empty())
    return true;

  float aspectratio = rect.GetWidth() / rect.GetHeight();
  pContext->PushProjectionMatrix();
  LocalToGlobal(rect);
  pContext->SetPerspectiveProjectionMatrix(rect, 30, aspectratio, .2, 20);
  pContext->Translate(0, -.5, -2);
  pContext->ResetClipRect();
  glBegin(GL_POINTS);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glEnd();
  
  float height = mRect.GetHeight();
  float width = mRect.GetWidth();
  float top = height * .05;
  float bot = height - 100;
  float h = bot - top;
  float w = h;
  float left = (width - w) * .5;
  float right = width - left;
  float halfwidth = left * .8;
  float sidewidth = w * .1;
  float sidespace = sidewidth * .5;
  uint32 maxcount = ToBelow(halfwidth / sidespace);

  int32 maxcountleft  = MIN(maxcount, MAX(0, image));
  int32 maxcountright = MIN(maxcount, MIN(mImages.size(), image + 1));
  nuiRect r(left, top, w, h);

  pContext->PushMatrix();
  float start = mReflectionStart;
  float end = mReflectionEnd;
  pContext->Scale(1.0f, -1.0f, 1.0f);
  const float SIDE_SHIFT = .6;
  const float SIDE_GAP = .22;
  for (uint32 i = 0; i < 2; i++)
  {
    {
      int32 s = image - maxcountleft;
      for (int32 l = s, c = 0; l < image; l++, c++)
      {
        pContext->PushMatrix();
        float shiftx = (c - maxcountleft) * SIDE_GAP - SIDE_SHIFT - SIDE_GAP * fractional;
        float shiftz = -.4;
        float angle = 90;
        if (l == image - 1)
        {
          angle *= 1.0 - fractional;
          shiftx *= fract;
          shiftz *= fract;
        }
        pContext->Translate(shiftx, 0, shiftz);
        nuiMatrix m;
        m.SetRotation(angle, 0, 1, 0);
        pContext->MultMatrix(m);
        DrawCard(pContext, l, start, end);
        pContext->PopMatrix();
      }
    }
    
    {
      int32 s = image + 1;
      for (int32 l = s + maxcountright - 1, c = maxcountright - 1; c >= 0; l--, c--)
      {
        pContext->PushMatrix();
        float shiftx = c * SIDE_GAP + SIDE_SHIFT - SIDE_GAP * fractional;
        float shiftz = -.4;
        float angle = -90;
        if (l == image + 1)
        {
          angle *= fract;
          shiftx *= fract;
          shiftz *= fract;
        }
        pContext->Translate(shiftx, 0, shiftz);
        nuiMatrix m;
        m.SetRotation(angle, 0, 1, 0);
        pContext->MultMatrix(m);
        DrawCard(pContext, l, start, end);
        pContext->PopMatrix();
      }
    }
    
//    if (fractional == 0)
      DrawCard(pContext, image, start, end);
    start = 1.0f;
    end = 1.0f;
    pContext->Scale(1.0f, -1.0f, 1.0f);
  }
  pContext->PopMatrix();
  
  pContext->PopProjectionMatrix();
  return false;
}

bool nuiCoverFlow::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);  
  return true;
}

uint32 nuiCoverFlow::AddImage(nuiTexture* pTexture, nuiTexture* pBefore)
{
  pTexture->Acquire();
  Invalidate();
  if (!pBefore)
  {
    mImages.push_back(pTexture);
    return mImages.size() - 1;
  }

  for (uint32 i = 0; i < mImages.size(); i++)
  {
    if (mImages[i] == pBefore)
    {
      mImages.insert(mImages.begin() + i, pTexture);
      return i;
    }
  }

  // pBefore not found? Add to back
  mImages.push_back(pTexture);
  return mImages.size() - 1;
}

uint32 nuiCoverFlow::AddImage(nuiTexture* pTexture, uint32 index)
{
  pTexture->Acquire();
  Invalidate();
  mImages.insert(mImages.begin() + index, pTexture);
  return index;
}

void nuiCoverFlow::DelImage(nuiTexture* pTexture)
{
  for (uint32 i = 0; i < mImages.size(); i++)
  {
    if (mImages[i] == pTexture)
    {
      mImages[i]->Release();
      mImages.erase(mImages.begin() + i);
      Invalidate();
      SelectImage(mSelectedImage);
      return;
    }
  }
}

void nuiCoverFlow::DelImage(uint32 index)
{
  mImages[index]->Release();
  mImages.erase(mImages.begin() + index);
  SelectImage(mSelectedImage);
  Invalidate();
}

void nuiCoverFlow::SelectImage(nuiTexture* pTexture)
{
  for (uint32 i = 0; i < mImages.size(); i++)
  {
    if (mImages[i] == pTexture)
    {
      SelectImage(i);
      return;
    }
  }
}

void nuiCoverFlow::SelectImage(int32 index)
{
  if (index >= mImages.size())
    index = mImages.size() - 1;
  else if (index < 0)
    index = 0;
  mSelectedImage = index;
  
  mLastTime = nglTime();
  mTimer.Start();
  Invalidate();
}


const std::vector<nuiTexture*>& nuiCoverFlow::GetImages() const
{
  return mImages;
}

void nuiCoverFlow::SetBackground(const nuiColor& rColor)
{
  mBackground = rColor;
  Invalidate();
}

const nuiColor& nuiCoverFlow::GetBackground() const
{
  return mBackground;
}

bool nuiCoverFlow::OnUpdateTime(const nuiEvent& rEvent)
{
  float diff = (float)mSelectedImage - mPos;
  double t = nglTime() - mLastTime;
  const double TIME_RATIO = 0.1f;
  
  if (diff < 0.001)
  {
    mTimer.Stop();
  }
  else
  {
    mPos += diff * t * TIME_RATIO;
  }
  
  Invalidate();
  return false;
}

bool nuiCoverFlow::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_LEFT)
  {
    SelectImage(mSelectedImage - 1);
    return true;
  }
  else if (rEvent.mKey == NK_RIGHT)
  {
    SelectImage(mSelectedImage + 1);
    return true;
  }
  return false;
}

bool nuiCoverFlow::KeyUp(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_LEFT)
  {
    return true;
  }
  else if (rEvent.mKey == NK_RIGHT)
  {
    return true;
  }
  return false;
}
