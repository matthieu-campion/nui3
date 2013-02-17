/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiCoverFlow::nuiCoverFlow()
: mSelectedImage(0),
  mBackground(0, 0, 0, 255),
  mReflectionStart(0.3),
  mReflectionEnd(0.0),
  mDrawBackground(true),
  mYOffset(0.0f),
  mSelectionYOffset(0.0f),
  mAngle(60),
  mSideShift(1.1),
  mSideGap(.19),
  mSideDepth(.7),
  mPos(0.0f),
  mLastTime(nglTime()),
  mTimer(1.0f / 30.0f),
  mFlowSink(this)
{
  if (SetObjectClass(_T("nuiCoverFlow")))
  {
    AddAttribute(new nuiAttribute<int32>
                 (nglString(_T("SelectedImage")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetSelectedIndex), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SelectImageIndex)));
    
    AddAttribute(new nuiAttribute<int32>
                 (nglString(_T("SelectedImageNow")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetSelectedIndex), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SelectImageIndexNow)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("ReflectionStart")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetReflectionStart), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetReflectionStart)));

    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("ReflectionEnd")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetReflectionEnd), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetReflectionEnd)));

    AddAttribute(new nuiAttribute<bool>
                 (nglString(_T("DrawBackground")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetDrawBackground), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetDrawBackground)));

    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("YOffset")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetYOffset), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetYOffset)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("SelectionYOffset")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetSelectionYOffset), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetSelectionYOffset)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("Angle")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetAngle), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetAngle)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("SideShift")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetSideShift), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetSideShift)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("SideGap")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetSideGap), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetSideGap)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("SideDepth")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetSideDepth), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetSideDepth)));
    
    AddAttribute(new nuiAttribute<const nuiColor&>
                 (nglString(_T("BackgroundColor")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiCoverFlow::GetBackgroundColor), 
                  nuiMakeDelegate(this, &nuiCoverFlow::SetBackgroundColor)));
  }
  
  mFlowSink.Connect(mTimer.Tick, &nuiCoverFlow::OnUpdateTime);
  SetWantKeyboardFocus(true);
}

nuiCoverFlow::~nuiCoverFlow()
{
  mTimer.Stop();
  for (int32 i = 0; i < mImages.size(); i++)
    mImages[i]->Release();
}

void nuiCoverFlow::DrawCard(nuiDrawContext* pContext, int32 index, float start, float end)
{
  NGL_ASSERT(index < mImages.size());
  nuiTexture* pTex = mImages[index];
  // Draw the selected image:
  float imgw = pTex->GetWidth();
  float imgh = pTex->GetHeight();
  float s = MAX(imgw, imgh);
  float ratiow = imgw / s;
  float ratioh = imgh / s;
  nuiRect imgrect(0.0f, 0.0f, ratiow, ratioh);
  nuiRect r(-.5, 0.0f, 1.0f, 1.0f);
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
  
  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLE_STRIP);
  pArray->EnableArray(nuiRenderArray::eColor, true);
  pArray->EnableArray(nuiRenderArray::eTexCoord, true);
  nuiColor c0(mBackground);
  c0.Mix(blank, 1.0-start);
  pArray->SetColor(c0);
  pArray->SetVertex(vx0, vy0);
  pArray->SetTexCoords(0, th);
  pArray->PushVertex();
  
  pArray->SetVertex(vx1, vy0);
  pArray->SetTexCoords(tw, th);
  pArray->PushVertex();
  
  nuiColor c1(mBackground);
  c1.Mix(blank, 1.0-end);
  pArray->SetColor(c1);
  pArray->SetVertex(vx0, vy1);
  pArray->SetTexCoords(0, 0);
  pArray->PushVertex();
  
  pArray->SetVertex(vx1, vy1);
  pArray->SetTexCoords(tw, 0);
  pArray->PushVertex();
  
  pContext->EnableTexturing(true);
  pContext->SetTexture(pTex);
  pContext->DrawArray(pArray);  
}

bool nuiCoverFlow::Draw(nuiDrawContext* pContext)
{
  int32 image = ToBelow(mPos);
  float fractional = mPos - image;
  float fract = 1.0 - fractional;
  
  //NGL_OUT(_T("image: %d\npos: %f\nfractionnal: %f\nfract: %f\n\n"), image, mPos, fractional, fract);
  
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
  //LocalToGlobal(rect);
  pContext->SetPerspectiveProjectionMatrix(rect, 25, aspectratio, .2, 20);
  pContext->PushMatrix();
  pContext->LoadIdentity();
  pContext->Translate(0, -.5, -2);
//  pContext->ResetClipRect();
  
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
  int32 maxcount = ToBelow(halfwidth / sidespace);
  int32 size = mImages.size();
  
  int32 maxcountleft  = MIN(maxcount, MAX(0, image));
  int32 maxcountright = MIN(maxcount, size - 2 - image);
  nuiRect r(left, top, w, h);

  float start = mReflectionStart;
  float end = mReflectionEnd;
  pContext->Scale(1.0f, -1.0f, 1.0f);
  const float SIDE_SHIFT = mSideShift;
  const float SIDE_GAP = mSideGap;
  const float DEPTH_SHIFT = mSideDepth;
  const float ANGLE = mAngle;
  for (int32 i = 0; i < 2; i++)
  {
    {
      int32 s = image + 1;
      for (int32 l = s + maxcountright, c = maxcountright; c >= 0; l--, c--)
      {
        pContext->PushMatrix();
        float shiftx = c * SIDE_GAP + SIDE_SHIFT - SIDE_GAP * fractional;
        float shifty = mYOffset;
        float shiftz = -DEPTH_SHIFT;
        float angle = -ANGLE;
        if (l == image + 1)
        {
          angle *= fract;
          shiftx *= fract;
          shifty = (mYOffset * fract + mSelectionYOffset * fractional);
          shiftz *= fract;
        }
       
        pContext->Translate(shiftx, shifty, shiftz);
        nuiMatrix m;
        m.SetRotation(angle, 0, 1, 0);
        pContext->MultMatrix(m);
        float shade = 1 - (shiftx / (maxcountright * SIDE_GAP + SIDE_SHIFT));
        float sstart = start * shade;
        float send = end * shade;
        DrawCard(pContext, l, sstart, send);
        pContext->PopMatrix();
      }
    }

    {
      int32 s = image - maxcountleft;
      for (int32 l = s, c = 0; l <= image; l++, c++)
      {
        pContext->PushMatrix();
        float shiftx = (c - maxcountleft + 1) * SIDE_GAP - SIDE_SHIFT - SIDE_GAP * fractional;
        float shifty = mYOffset;
        float shiftz = -DEPTH_SHIFT;
        float angle = ANGLE;
        if (l == image)
        {
          angle *= fractional;
          shiftx *= fractional;
          shifty = (mYOffset * fractional + mSelectionYOffset * fract);
          shiftz *= fractional;
        }
        
        pContext->Translate(shiftx, shifty, shiftz);
        nuiMatrix m;
        m.SetRotation(angle, 0, 1, 0);
        pContext->MultMatrix(m);
        float shade = 1 + (shiftx / (maxcountleft * SIDE_GAP + SIDE_SHIFT));
        float sstart = start * shade;
        float send = end * shade;
        DrawCard(pContext, l, sstart, send);
        pContext->PopMatrix();
      }
    }
    
    
//    if (fractional == 0)
//      DrawCard(pContext, image, start, end);
    start = 1.0f;
    end = 1.0f;
    pContext->Scale(1.0f, -1.0f, 1.0f);
  }

  pContext->PopMatrix();
  
  pContext->PopProjectionMatrix();

//  pContext->EnableTexturing(false);
//  pContext->SetStrokeColor(nuiColor(_T("red")));
//  pContext->DrawLine(mRect.GetWidth() / 2, 0, mRect.GetWidth() / 2, mRect.GetHeight());
//  pContext->SetStrokeColor(nuiColor(_T("blue")));
//  pContext->DrawLine(mRect.GetWidth() * .25, 0, mRect.GetWidth() * .25, mRect.GetHeight());
//  pContext->DrawLine(mRect.GetWidth() * .75, 0, mRect.GetWidth() * .75, mRect.GetHeight());
  
  return false;
}

bool nuiCoverFlow::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);  
  return true;
}

nuiRect nuiCoverFlow::CalcIdealSize()
{
  return nuiRect(800, 400);
}

int32 nuiCoverFlow::AddImage(nuiTexture* pTexture, nuiTexture* pBefore)
{
  pTexture->Acquire();
  Invalidate();
  if (!pBefore)
  {
    mImages.push_back(pTexture);
    return mImages.size() - 1;
  }

  for (int32 i = 0; i < mImages.size(); i++)
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

int32 nuiCoverFlow::AddImage(nuiTexture* pTexture, int32 index)
{
  pTexture->Acquire();
  Invalidate();
  mImages.insert(mImages.begin() + index, pTexture);
  return index;
}

void nuiCoverFlow::DelImage(nuiTexture* pTexture)
{
  for (int32 i = 0; i < mImages.size(); i++)
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

void nuiCoverFlow::DelImage(int32 index)
{
  mImages[index]->Release();
  mImages.erase(mImages.begin() + index);
  SelectImage(mSelectedImage);
  Invalidate();
}

void nuiCoverFlow::SelectImage(nuiTexture* pTexture)
{
  for (int32 i = 0; i < mImages.size(); i++)
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
  if (index == mSelectedImage)
    return;
  
  if (index >= (int32)mImages.size())
    index = mImages.size() - 1;

  if (index < 0)
    index = 0;

  mSelectedImage = index;
  SelectionChanged();
  
  mLastTime = nglTime();
  if (!mTimer.IsRunning())
  {
    mTimer.Start();
  }
}

void nuiCoverFlow::SelectImageNow(nuiTexture* pTexture)
{
  SelectImage(pTexture);
  mPos = mSelectedImage;
  mTimer.Stop();
  Invalidate();
}

void nuiCoverFlow::SelectImageNow(int32 index)
{
  SelectImage(index);
  mPos = mSelectedImage;
  mTimer.Stop();
  Invalidate();
}



const std::vector<nuiTexture*>& nuiCoverFlow::GetImages() const
{
  return mImages;
}

void nuiCoverFlow::SetBackgroundColor(const nuiColor& rColor)
{
  mBackground = rColor;
  Invalidate();
}

const nuiColor& nuiCoverFlow::GetBackgroundColor() const
{
  return mBackground;
}

void nuiCoverFlow::OnUpdateTime(const nuiEvent& rEvent)
{
  float diff = ((float)mSelectedImage) - mPos;
  double _t = nglTime();
  double t = _t - mLastTime;
  mLastTime = _t;
  const double TIME_RATIO = 8.0f;
  
  if (fabs(diff) < 0.001)
  {
    mPos = mSelectedImage;
    mTimer.Stop();
  }
  else
  {
    mPos += diff * t * TIME_RATIO;
  }
  
  Invalidate();
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

bool nuiCoverFlow::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonWheelUp || Button & nglMouseInfo::ButtonWheelLeft)
  {    
    SelectImage(mSelectedImage - 1);
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelDown || Button & nglMouseInfo::ButtonWheelRight)
  {    
    SelectImage(mSelectedImage + 1);
    return true;
  }
  
  return true;
}

bool nuiCoverFlow::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonWheelDown
    || Button & nglMouseInfo::ButtonWheelLeft
    || Button & nglMouseInfo::ButtonWheelDown
    || Button & nglMouseInfo::ButtonWheelRight)
    return true;
  
  return true;
}

int32 nuiCoverFlow::GetSelectedIndex() const
{
  return mSelectedImage;
}

nuiTexture* nuiCoverFlow::GetSelectedTexture() const
{
  if (mSelectedImage >= 0 && mSelectedImage < mImages.size())
    return mImages[mSelectedImage];
  
  return NULL;
}

void nuiCoverFlow::SetReflectionStart(float set)
{
  mReflectionStart = set;
  Invalidate();
}

float nuiCoverFlow::GetReflectionStart() const
{
  return mReflectionStart;
}

void nuiCoverFlow::SetReflectionEnd(float set)
{
  mReflectionEnd = set;
  Invalidate();
}

float nuiCoverFlow::GetReflectionEnd() const
{
  return mReflectionEnd;
}

void nuiCoverFlow::SetDrawBackground(bool set)
{
  mDrawBackground = set;
  Invalidate();
}

bool nuiCoverFlow::GetDrawBackground() const
{
  return mDrawBackground;
}

void nuiCoverFlow::SetYOffset(float set)
{
  mYOffset = set;
  Invalidate();
}

float nuiCoverFlow::GetYOffset() const
{
  return mYOffset;
}

void nuiCoverFlow::SetSelectionYOffset(float set)
{
  mSelectionYOffset = set;
  Invalidate();
}

float nuiCoverFlow::GetSelectionYOffset() const
{
  return mSelectionYOffset;
}

void nuiCoverFlow::SetAngle(float set)
{
  mAngle = set;
  Invalidate();
}

float nuiCoverFlow::GetAngle() const
{
  return mAngle;
}

void nuiCoverFlow::SetSideShift(float set)
{
  mSideShift = set;
  Invalidate();
}

float nuiCoverFlow::GetSideShift() const
{
  return mSideShift;
}

void nuiCoverFlow::SetSideGap(float set)
{
  mSideGap = set;
  Invalidate();
}

float nuiCoverFlow::GetSideGap() const
{
  return mSideGap;
}

void nuiCoverFlow::SetSideDepth(float set)
{
  mSideDepth = set;
  Invalidate();
}

float nuiCoverFlow::GetSideDepth() const
{
  return mSideDepth;
}

void nuiCoverFlow::SelectImageIndex(int32 index)
{
  SelectImage(index);
}

void nuiCoverFlow::SelectImageIndexNow(int32 index)
{
  SelectImageNow(index);
}
