/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAnimation.h"
#include "nuiXML.h"
#include "nglMath.h"

nuiTimer* nuiAnimation::mpTimer = NULL;
uint32 nuiAnimation::mAnimCounter = 0;
double nuiAnimation::mFrameRate = 60; // 30 FPS by default

void nuiAnimation::SetFrameRate(double FPS)
{
  mFrameRate = FPS;
  if (mpTimer)
  {
    mpTimer->Stop();
    mpTimer->SetPeriod(1.0 / mFrameRate);
    mpTimer->Start(false, false);
  }
}

double nuiAnimation::GetFrameRate()
{
  return mFrameRate;
}

nuiAnimation::nuiAnimation()
: mAnimSink(this)
{
  mCount = 0;
  mDuration = 0.5;
  mCurrentTime = 0;
  mDirection = 1.0;
  mLoopMode = eAnimLoopForward;
  mUpdatingTime = false;
  mEnableCallbacks = true;

  mEasing = (nuiEasingMethod)(&::nuiEasingIdentity);
  
  if (!mAnimCounter)
  {
    mpTimer = new nuiTimer(1.0 / mFrameRate);
    mpTimer->Start(false, true);
  }

  mAnimCounter++;
  
  if (SetObjectClass(_T("nuiAnimation")))
  {
    AddAttribute(new nuiAttribute<double>
                 (nglString(_T("CurrentTime")), nuiUnitSeconds,
                  nuiMakeDelegate(this, &nuiAnimation::GetTimeFromStart),
                  nuiMakeDelegate(this, &nuiAnimation::SetTimeFromStart)));
    
    AddAttribute(new nuiAttribute<double>
                 (nglString(_T("SetTimeFromNow")), nuiUnitSeconds,
                  nuiMakeDelegate(this, &nuiAnimation::GetTimeFromNow),
                  nuiMakeDelegate(this, &nuiAnimation::SetTimeFromNow)));

    AddAttribute(new nuiAttribute<double>
                 (nglString(_T("CurrentTimeFromEnd")), nuiUnitSeconds,
                  nuiMakeDelegate(this, &nuiAnimation::GetTimeFromEnd),
                  nuiMakeDelegate(this, &nuiAnimation::SetTimeFromEnd)));
    
    AddAttribute(new nuiAttribute<double>
                 (nglString(_T("Position")), nuiUnitSize,
                  nuiMakeDelegate(this, &nuiAnimation::GetPosition)));
    
    AddAttribute(new nuiAttribute<bool>
                 (nglString(_T("IsPlaying")), nuiUnitBoolean,
                  nuiMakeDelegate(this, &nuiAnimation::IsPlaying)));
        
    AddAttribute(new nuiAttribute<double>
                 (nglString(_T("Duration")), nuiUnitSeconds,
                  nuiMakeDelegate(this, &nuiAnimation::GetDuration),
                  nuiMakeDelegate(this, &nuiAnimation::SetDuration)));
  }
}

void nuiAnimation::SetEasing(const nuiEasingMethod& rMethod)
{
  mEasing = rMethod;
}


void nuiAnimation::SetTimeFromStart(double Time)
{
  SetTime(Time, eAnimFromStart);
}

void nuiAnimation::SetTimeFromNow(double Time)
{
  if (Time >= 0)
    SetTime(Time, eAnimForward);
  else
    SetTime(-Time, eAnimRewind);
}

void nuiAnimation::SetTimeFromEnd(double Time)
{
  SetTime(Time, eAnimFromEnd);
}

double nuiAnimation::GetTimeFromStart() const
{
  return GetTime();
}

double nuiAnimation::GetTimeFromNow() const
{
  return 0;
}

double nuiAnimation::GetTimeFromEnd() const
{
  return mDuration - GetTime();
}


bool nuiAnimation::Load(const nuiXMLNode* pNode)
{
  mCount = 0;
  mDuration = 0.5;
  mCurrentTime = 0;
  mDirection = 1.0;
  mLoopMode = eAnimLoopForward;
  mUpdatingTime = false;
  mEnableCallbacks = true;

  return true;
}

nuiAnimation::~nuiAnimation()
{
  mAnimCounter--;
  if (!mAnimCounter)
  {
    delete mpTimer;
  }
}

nuiXMLNode* nuiAnimation::Serialize(nuiXMLNode* pParentNode, bool CreateNewNode) const
{
  nuiXMLNode* pNode = NULL;

  if (CreateNewNode)
  {
    if (pParentNode)
    {
      pNode = new nuiXMLNode("nuiAnimation",pParentNode);
    }
    else
    {
      pNode = new nuiXML("nuiAnimation");
    }
  }
  else
    pNode = pParentNode;

  if (!pNode)
    return NULL;

  pNode->SetAttribute(_T("CurrentTime"),mCurrentTime);
  return pNode;
}

bool nuiAnimation::SetTime(double Time, nuiAnimWhence Whence)
{
  switch (Whence)
  {
  case eAnimFromStart:
    mCurrentTime = Time;
    break;
  case eAnimForward:
    mCurrentTime += Time;
    break;
  case eAnimRewind:
    mCurrentTime -= Time;
    break;
  case eAnimFromEnd:
    mCurrentTime = GetDuration() - Time;
    break;
  }

  if (mCurrentTime < 0)
  {
    mCurrentTime = 0;
    CallOnFrame();
    return false;
  }

  if (mCurrentTime > GetDuration())
  {
    mCurrentTime = GetDuration();
    CallOnFrame();
    return false;
  }

  CallOnFrame();
  return true;
}

double nuiAnimation::GetTime() const
{
  return mCurrentTime;
}

double nuiAnimation::GetDuration() const
{
  return mDuration;
}

void nuiAnimation::SetDuration(double duration)
{
  mDuration = duration;
}

double nuiAnimation::GetPosition() const
{
  return mCurrentPosition;
}

void nuiAnimation::Play(uint32 Count, nuiAnimLoop LoopMode)
{
  mCount = Count;
  mLastTime = nglTime();
  mLoopMode = LoopMode;
  if (mLoopMode == eAnimLoopReverse)
    mDirection = -1.0;
  else
    mDirection = 1.0;
  
  mAnimSink.Connect(mpTimer->Tick, &nuiAnimation::OnTick);
  
  AnimStart();
}

void nuiAnimation::Stop()
{
  if (!mUpdatingTime)
  {
    mCount = 1;
    CallOnFrame();
  }
  mCount = 0;
  //NGL_OUT(_T("nuiAnimation::Stop at %f\n"), GetDuration());
  mAnimSink.Disconnect(mpTimer->Tick, &nuiAnimation::OnTick);
  AnimStop();
}

bool nuiAnimation::IsPlaying() const
{
  return mCount != 0;
}

void nuiAnimation::OnFrame()
{
}

void nuiAnimation::CallOnFrame()
{
  UpdateTime();
  OnFrame();
}

bool nuiAnimation::OnTick(const nuiEvent& rEvent)
{
  CallOnFrame();
  return false;
}

double nuiAnimation::UpdateTime()
{
  mUpdatingTime = true;

  nglTime now;
  double t = now - mLastTime;
  double advance = mDirection * t;
  mCurrentTime += advance;
  mLastTime = now;

  double duration = GetDuration();

  switch (mLoopMode)
  {
  case eAnimLoopForward:
    if (mCurrentTime >= duration)
    {
      mCount--;
      mCurrentTime = 0;

      if (!mCount)
      {
        mCurrentTime = duration;
        //NGL_OUT(_T("Stop anim (time has come: %f > %f)\n"), mCurrentTime, duration);
        Stop();
      }
      else
      {
        AnimLoop();
      }
    }
    break;
  case eAnimLoopReverse:
    if (mCurrentTime <= 0.0)
    {
      mCount--;
      if (!mCount)
      {
        mCurrentTime = 0;
        Stop();
      }
      else
      {
        mCurrentTime = duration;
        AnimLoop();
      }
    }
    break;
  case eAnimLoopPingPong:
    if (mCurrentTime >= duration)
    {
      if (!mCount--)
      {
        mCurrentTime = duration;
        Stop();
      }
      else
      {
        mCurrentTime = duration - (mCurrentTime - duration);
        mDirection = -mDirection;
        AnimLoop();
      }
    }
    if (mCurrentTime <= 0.0)
    {
      if (!mCount--)
      {
        mCurrentTime = 0;
        Stop();
      }
      else
      {
        mCurrentTime = -mCurrentTime;
        mDirection = -mDirection;
        AnimLoop();
      }
    }
    break;
  }
  
  mCurrentPosition = mEasing(mCurrentTime / mDuration);
  
  mUpdatingTime = false;
  if (IsPlaying())
    return advance;
  else
    return 0;
}

bool nuiAnimation::Play(const nuiEvent& rEvent)
{
  if (mEnableCallbacks)
    Play();
  return false;
}

bool nuiAnimation::Stop(const nuiEvent& rEvent)
{
  if (mEnableCallbacks)
    Stop();
  return false;
}

void nuiAnimation::EnableCallbacks(bool enable)
{
  mEnableCallbacks = enable;
}


/////// META ANIMATIONS:
int PGCD(int a, int b)
{
  if (a < b)
  {
    int t = a;
    a = b;
    b = t;
  }

  int r = a % b;
  while (r != 0)
  {
    a = b;
    b = r;
    r = a % b;
  }
  return b;
}

int PPCM(int a, int b)
{
  return (a * b) / PGCD(a, b);
}

nuiMetaAnimation::nuiMetaAnimation ()
{
}

bool nuiMetaAnimation::Load(const nuiXMLNode* pNode)
{
  nuiAnimation::Load(pNode);
  return true;
}

nuiMetaAnimation::~nuiMetaAnimation()
{
  Clear();
}

nuiXMLNode* nuiMetaAnimation::Serialize(nuiXMLNode* pNode, bool CreateNewNode) const
{
  return nuiAnimation::Serialize(pNode, CreateNewNode);
}

void nuiMetaAnimation::Play(uint32 Count, nuiAnimLoop LoopMode)
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  for (; it != end; ++it)
    (*it)->Play(Count, LoopMode);

  nuiAnimation::Play(Count, LoopMode);
}

void nuiMetaAnimation::Stop()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  for (; it != end; ++it)
  {
    nuiAnimation* pItem = *it;
    if (pItem->IsPlaying())
      pItem->Stop();
  }

  nuiAnimation::Stop();
}

double nuiMetaAnimation::GetDuration()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  double dur = 0;
  for (; it != end; ++it)
    dur = MAX(dur, (*it)->GetDuration());

  return dur;
}

void nuiMetaAnimation::AddAnimation(nuiAnimation* pAnim)
{
  mpAnimations.push_back(pAnim);
}

void nuiMetaAnimation::Clear()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  for (; it != end; ++it)
    delete *it;

  mpAnimations.clear();
}


/////// ANIMATION SEQUENCES:

nuiAnimationSequence::nuiAnimationSequence ()
: mAnimSequenceSink(this)
{
}

bool nuiAnimationSequence::Load(const nuiXMLNode* pNode)
{
  bool res = nuiAnimation::Load(pNode);
  return res;
}

nuiAnimationSequence::~nuiAnimationSequence()
{
  Clear();
}

nuiXMLNode* nuiAnimationSequence::Serialize(nuiXMLNode* pNode, bool CreateNewNode) const
{
  return nuiAnimation::Serialize(pNode, CreateNewNode);
}

void nuiAnimationSequence::Play(uint32 Count, nuiAnimLoop LoopMode)
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();
  
  if (it != end)
    (*it)->Play(1, LoopMode);

  nuiAnimation::Play(Count, LoopMode);
}

void nuiAnimationSequence::Stop()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  for (; it != end; ++it)
  {
    nuiAnimation* pItem = *it;
    if (pItem->IsPlaying())
      pItem->Stop();
  }

  nuiAnimation::Stop();
}

double nuiAnimationSequence::GetDuration()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  double dur = 0;
  for (; it != end; ++it)
    dur += (*it)->GetDuration();

//  NGL_OUT(_T("nuiAnimationSequence::GetDuration() = %f\n"), dur);
  return dur;
}

void nuiAnimationSequence::AddAnimation(nuiAnimation* pAnim)
{
  mpAnimations.push_back(pAnim);
  mAnimSequenceSink.Connect(pAnim->AnimStop, &nuiAnimationSequence::OnAnimStopped, pAnim);
}

void nuiAnimationSequence::Clear()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  for (; it != end; ++it)
    delete *it;

  mpAnimations.clear();
}

void nuiAnimationSequence::OnFrame()
{
  // Do nothing, not even update time.
}

bool nuiAnimationSequence::OnAnimStopped(const nuiEvent& rEvent)
{
  if (IsPlaying())
  {
    //NGL_OUT(_T("nuiAnimationSequence::OnAnimStopped\n"));
    nuiAnimation* pAnim = (nuiAnimation*)rEvent.mpUser;
    // Find the next anim in the sequence:
    std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
    std::list<nuiAnimation*>::iterator end = mpAnimations.end();

    while (it != end)
    {
      if (pAnim == *it)
      {
        ++it;
        break;
      }

      ++it;
    }

    if (it != end)
    {
      // Launch the next anim in the sequence:
      pAnim = *it;
      pAnim->Play();
      //NGL_OUT(_T("nuiAnimationSequence::OnAnimStopped Launch next anim\n"));
    }
    else
    {
      // stop the sequence if this was the last in the chain:
      //NGL_OUT(_T("nuiAnimationSequence::OnAnimStopped STOP\n"));
      mpAnimations.front()->Play();
    }
  }

  return false;
}

