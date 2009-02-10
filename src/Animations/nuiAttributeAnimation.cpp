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
  const float invpos = 1.0 - pos;

  const float r = r0 + invpos * rd;
  const float g = g0 + invpos * gd;
  const float b = b0 + invpos * bd;
  const float a = a0 + invpos * ad;
  
  nuiColor col(r, g, b, a);
  
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiColor> color_attrib(attrib);
  nuiAttrib<const nuiColor&> const_color_attrib(attrib);
  
  if (color_attrib)
    color_attrib.Set(col);
  else if (const_color_attrib)
    const_color_attrib.Set(col);
}

