/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiAttributeAnimation.h"

//class nuiAttributeAnimation : public nuiAnimation
nuiAttributeAnimationBase::nuiAttributeAnimationBase()
: mCaptureStartOnPlay(false),
  mCaptureEndOnPlay(false),
  mpTarget(NULL)
{
  if (SetObjectClass(_T("nuiAttributeAnimationBase")))
  {
    // Init atributes
  }
}

nuiAttributeAnimationBase::~nuiAttributeAnimationBase()
{
}

nuiObjectPtr nuiAttributeAnimationBase::GetTargetObject() const
{
  return mpTarget;
}

void nuiAttributeAnimationBase::SetTargetObject(nuiObjectPtr pTarget)
{
  mpTarget = pTarget;
}

const nglString& nuiAttributeAnimationBase::GetTargetAttribute() const
{
  return mTarget;
}

void nuiAttributeAnimationBase::SetTargetAttribute(const nglString& rAttribute)
{
  mTarget = rAttribute;
}

void nuiAttributeAnimationBase::SetCaptureStartOnPlay(bool set)
{
  mCaptureStartOnPlay = set;
}

bool nuiAttributeAnimationBase::GetCaptureStartOnPlay() const
{
  return mCaptureStartOnPlay;
}

void nuiAttributeAnimationBase::SetCaptureEndOnPlay(bool set)
{
  mCaptureEndOnPlay = set;
}

bool nuiAttributeAnimationBase::GetCaptureEndOnPlay() const
{
  return mCaptureEndOnPlay;
}


//// nuiAttributeAnimation:
nuiAttributeAnimation::nuiAttributeAnimation()
: mStartValue(0),
  mEndValue(0)
{
  if (SetObjectClass(_T("nuiAttributeAnimation")))
  {
    // Init atributes
  }
}

nuiAttributeAnimation::~nuiAttributeAnimation()
{
  
}

void nuiAttributeAnimation::SetEndValue(double val)
{
  mEndValue = val;
}

double nuiAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiAttributeAnimation::SetStartValue(double val)
{
  mStartValue = val;
}

double nuiAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}

void nuiAttributeAnimation::Play(uint32 Count, nuiAnimLoop LoopMode)
{
  if (mCaptureStartOnPlay)
  {
    nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
    nglString str;
    if (attrib.ToString(str))
      mStartValue = str.GetCDouble();
  }
  if (mCaptureEndOnPlay)
  {
    nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
    nglString str;
    if (attrib.ToString(str))
      mEndValue = str.GetCDouble();
  }
  
  nuiAnimation::Play(Count, LoopMode);
}

void nuiAttributeAnimation::Stop()
{
  nuiAnimation::Stop();
}

#define SET_ATTRIB(X) \
if (nuiAttributeTypeTrait<X>::GetTypeId() == t) \
{ \
  nuiAttrib<X> attrib_##X(attrib); \
  NGL_ASSERT(attrib_##X.IsValid()); \
  attrib_##X.Set(pos); \
  return; \
}

void nuiAttributeAnimation::OnFrame()
{
  double pos = mStartValue + GetPosition() * (mEndValue - mStartValue);
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  if (!attrib.IsValid() || attrib.IsReadOnly())
    return;
  
  nuiAttributeType t = attrib.GetType();  
  SET_ATTRIB(float);
  SET_ATTRIB(double);
  SET_ATTRIB(int32);
  SET_ATTRIB(uint32);
  SET_ATTRIB(int64);
  SET_ATTRIB(uint64);
  SET_ATTRIB(int16);
  SET_ATTRIB(uint16);
  SET_ATTRIB(int8);
  SET_ATTRIB(uint8);               
  
  nglString str;
  str.SetCDouble(pos);
  attrib.FromString(str);
}

#undef SET_ATTRIB

/////////////////////////////////

// Color Attrib Animation:

/////////////////////////////////

//// nuiAttributeAnimation:
nuiColorAttributeAnimation::nuiColorAttributeAnimation()
{
  if (SetObjectClass(_T("nuiColorAttributeAnimation")))
  {
    // Init atributes
  }
}

nuiColorAttributeAnimation::~nuiColorAttributeAnimation()
{
}

void nuiColorAttributeAnimation::SetEndValue(const nuiColor& rVal)
{
  mEndValue = rVal;
}

const nuiColor& nuiColorAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiColorAttributeAnimation::SetStartValue(const nuiColor& rVal)
{
  mStartValue = rVal;
}

const nuiColor& nuiColorAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}



void nuiColorAttributeAnimation::Play(uint32 Count, nuiAnimLoop LoopMode)
{
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiColor> color_attrib(attrib);
  nuiAttrib<const nuiColor&> const_color_attrib(attrib);

  if (mCaptureStartOnPlay)
  {
    if (color_attrib)
      mStartValue = color_attrib.Get();
    else if (const_color_attrib)
      mStartValue = const_color_attrib.Get();
  }
  if (mCaptureEndOnPlay)
  {
    if (color_attrib)
      mEndValue = color_attrib.Get();
    else if (const_color_attrib)
      mEndValue = const_color_attrib.Get();
  }
  
  nuiAnimation::Play(Count, LoopMode);
}

void nuiColorAttributeAnimation::Stop()
{
  nuiAnimation::Stop();
}

void nuiColorAttributeAnimation::OnFrame()
{
  const float r0 = mStartValue.Red();
  const float g0 = mStartValue.Green();
  const float b0 = mStartValue.Blue();
  const float a0 = mStartValue.Alpha();

  const float r1 = mEndValue.Red();
  const float g1 = mEndValue.Green();
  const float b1 = mEndValue.Blue();
  const float a1 = mEndValue.Alpha();
  
  const float rd = r1 - r0;
  const float gd = g1 - g0;
  const float bd = b1 - b0;
  const float ad = a1 - a0;
  
  const float pos = GetPosition();

  const float r = r0 + pos * rd;
  const float g = g0 + pos * gd;
  const float b = b0 + pos * bd;
  const float a = a0 + pos * ad;
  
  nuiColor col(r, g, b, a);
  
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiColor> color_attrib(attrib);
  nuiAttrib<const nuiColor&> const_color_attrib(attrib);
  
  if (color_attrib)
    color_attrib.Set(col);
  else if (const_color_attrib)
    const_color_attrib.Set(col);
}














/////////////////////////////////

// Rect Attrib Animation:

/////////////////////////////////

//// nuiAttributeAnimation:
nuiRectAttributeAnimation::nuiRectAttributeAnimation()
{
  if (SetObjectClass(_T("nuiRectAttributeAnimation")))
  {
    // Init atributes
  }
}

nuiRectAttributeAnimation::~nuiRectAttributeAnimation()
{
}

void nuiRectAttributeAnimation::SetEndValue(const nuiRect& rVal)
{
  mEndValue = rVal;
}

const nuiRect& nuiRectAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiRectAttributeAnimation::SetStartValue(const nuiRect& rVal)
{
  mStartValue = rVal;
}

const nuiRect& nuiRectAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}



void nuiRectAttributeAnimation::Play(uint32 Count, nuiAnimLoop LoopMode)
{
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiRect> rect_attrib(attrib);
  nuiAttrib<const nuiRect&> const_rect_attrib(attrib);
  
  if (mCaptureStartOnPlay)
  {
    if (rect_attrib.IsValid())
      mStartValue = rect_attrib.Get();
    else if (const_rect_attrib)
      mStartValue = const_rect_attrib.Get();
  }
  if (mCaptureEndOnPlay)
  {
    if (rect_attrib)
      mEndValue = rect_attrib.Get();
    else if (const_rect_attrib)
      mEndValue = const_rect_attrib.Get();
  }
  
  nuiAnimation::Play(Count, LoopMode);
}

void nuiRectAttributeAnimation::Stop()
{
  nuiAnimation::Stop();
}

void nuiRectAttributeAnimation::OnFrame()
{
  const float x0 = mStartValue.Left();
  const float y0 = mStartValue.Top();
  const float w0 = mStartValue.GetWidth();
  const float h0 = mStartValue.GetHeight();
  
  const float x1 = mEndValue.Left();
  const float y1 = mEndValue.Top();
  const float w1 = mEndValue.GetWidth();
  const float h1 = mEndValue.GetHeight();
  
  const float xd = x1 - x0;
  const float yd = y1 - y0;
  const float wd = w1 - w0;
  const float hd = h1 - h0;
  
  const float pos = GetPosition();
  
  const float x = x0 + pos * xd;
  const float y = y0 + pos * yd;
  const float w = w0 + pos * wd;
  const float h = h0 + pos * hd;
  
  nuiRect rect(x, y, w, h);
  //NGL_OUT(_T("rect anim: %ls\n"), rect.GetValue().GetChars());
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiRect> rect_attrib(attrib);
  nuiAttrib<const nuiRect&> const_rect_attrib(attrib);
  
  if (rect_attrib)
    rect_attrib.Set(rect);
  else if (const_rect_attrib)
    const_rect_attrib.Set(rect);
}


