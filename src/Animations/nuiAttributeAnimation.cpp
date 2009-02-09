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

void nuiAttributeAnimation::SetStartValue(double val)
{
  mStartValue = val;
}

double nuiAttributeAnimation::GetStartValue() const
{
  return mStartValue;
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


