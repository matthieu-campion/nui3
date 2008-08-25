/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAnimation.h"
#include "nuiXML.h"
#include "nglMath.h"

nuiAnimation::nuiAnimation()
: mTimer(0.1),
  mAnimSink(this)
{
  mFrameTime = 0;
  mCurrentTime = 0;
  mCount = 0;
  mDirection = 1.0;
  mLoopMode = eAnimLoopForward;
  mUpdatingTime = false;
  mEnableCallbacks = true;

  mAnimSink.Connect(mTimer.Tick, &nuiAnimation::OnTick);

  SetFrameRate(30);

}

bool nuiAnimation::Load(const nuiXMLNode* pNode)
{
  mFrameTime = 0;
  mCurrentTime = 0;
  mCount = 0;
  mDirection = 1.0;
  mLoopMode = eAnimLoopForward;
  mUpdatingTime = false;
  mEnableCallbacks = true;
  
  SetFrameRate(30);

  mAnimSink.Connect(mTimer.Tick, &nuiAnimation::OnTick);
  
  return true;
}

nuiAnimation::~nuiAnimation()
{
  mTimer.Stop();
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
  pNode->SetAttribute(_T("FrameTime"),mFrameTime);
  return pNode;
}

void nuiAnimation::SetFrameTime(double FrameTime)
{
  mFrameTime = FrameTime;

  if (mCount != 0)
    mTimer.Stop();

  mTimer.SetPeriod(mFrameTime);

  if (mCount != 0)
   mTimer.Start();
}

void nuiAnimation::SetFrameRate(double FrameRate)
{
  mFrameTime = 1.0 / FrameRate;

  if (mCount != 0)
    mTimer.Stop();
  
  mTimer.SetPeriod(mFrameTime);

  if (mCount != 0)
    mTimer.Start();
}

double nuiAnimation::GetFrameRate()
{
  return 1.0 / mFrameTime;
}

double nuiAnimation::GetFrameTime()
{
  return mFrameTime;
}


bool nuiAnimation::SetFrame(double Frame, nuiAnimWhence Whence)
{
  switch (Whence)
  {
  case eAnimFromStart:
    mCurrentTime = FrameToTime(Frame);
    break;
  case eAnimForward:
    mCurrentTime += FrameToTime(Frame);
    break;
  case eAnimRewind:
    mCurrentTime -= FrameToTime(Frame);
    break;
  case eAnimFromEnd:
    mCurrentTime = GetDuration() - FrameToTime(Frame);
    break;
  }

  if (mCurrentTime < 0)
  {
    mCurrentTime = 0;
    OnFrame();
    return false;
  }

  if (mCurrentTime > GetDuration())
  {
    mCurrentTime = GetDuration();
    OnFrame();
    return false;
  }

  OnFrame();
  return true;
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
    OnFrame();
    return false;
  }

  if (mCurrentTime > GetDuration())
  {
    mCurrentTime = GetDuration();
    OnFrame();
    return false;
  }

  OnFrame();
  return true;
}

double nuiAnimation::GetFrame()
{
  return TimeToFrame(mCurrentTime);
}

double nuiAnimation::GetTime()
{
  return mCurrentTime;
}

double nuiAnimation::TimeToFrame(double Tm)
{
  if (mFrameTime == 0)
    return 0;
  return Tm / mFrameTime;
}

double nuiAnimation::FrameToTime(double Frame)
{
  return Frame * mFrameTime;
}

double nuiAnimation::GetFrameCount()
{
  return 0;
}

double nuiAnimation::GetDuration()
{
  return FrameToTime(GetFrameCount());
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
  mTimer.Start();
  AnimStart();
}

void nuiAnimation::Stop()
{
  if (!mUpdatingTime)
  {
    mCount = 1;
    OnFrame();
  }
  mCount = 0;
  //NGL_OUT(_T("nuiAnimation::Stop at %f\n"), GetDuration());
  mTimer.Stop();
  AnimStop();
}

bool nuiAnimation::IsPlaying()
{
  return mCount != 0;
}

void nuiAnimation::OnFrame()
{
  // By defulat update the animation:
  UpdateTime();
}

bool nuiAnimation::OnTick(const nuiEvent& rEvent)
{
  OnFrame();
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

#define META_FRAMES (20.0)

nuiMetaAnimation::nuiMetaAnimation ()
{
  SetFrameRate(META_FRAMES);
}

bool nuiMetaAnimation::Load(const nuiXMLNode* pNode)
{
  nuiAnimation::Load(pNode);
  SetFrameRate(META_FRAMES);
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

double nuiMetaAnimation::GetFrameCount()
{
  return GetDuration()*META_FRAMES;
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
#define SEQUENCE_FRAMES (20.0)

nuiAnimationSequence::nuiAnimationSequence ()
: mAnimSequenceSink(this)
{
  SetFrameRate(SEQUENCE_FRAMES);
}

bool nuiAnimationSequence::Load(const nuiXMLNode* pNode)
{
  bool res = nuiAnimation::Load(pNode);
  SetFrameRate(SEQUENCE_FRAMES);
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

double nuiAnimationSequence::GetFrameCount()
{
  return GetDuration()*SEQUENCE_FRAMES;
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

