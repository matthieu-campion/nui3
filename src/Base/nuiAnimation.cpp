/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiTimer* nuiAnimation::mpTimer = NULL;
int32 nuiAnimation::mAnimCounter = 0;
double nuiAnimation::mFrameRate = 120; // 30 FPS by default

// Standard easing:
double nuiEasingIdentity(double val)
{
  return val;
}

double nuiEasingSquare(double val)
{
  return val * val;
}


double nuiEasingCubic(double val)
{
  return val * val * val;
}

double nuiEasingQuartic(double val)
{
  return val * val * val * val;
}

double nuiEasingSlowStart(double val)
{
  return val * val * val * val * val * val * val * val;
}

double nuiEasingQuintic(double val)
{
  return val * val * val * val * val;
}

double nuiEasingSinusStartFast(double val)
{
  return sin(val * M_PI_2);
}

double nuiEasingSinusStartSlow(double val)
{
  return sin(val * M_PI_2 - M_PI_2) + 1;
}

double nuiEasingSinus(double val)
{
  return .5 * (sin(val * M_PI - M_PI_2) + 1);
}

double nuiEasingSquareRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSquare);
}

double nuiEasingCubicRev(double val)
{
  return nuiEasingReverse(val, nuiEasingCubic);
}

double nuiEasingQuarticRev(double val)
{
  return nuiEasingReverse(val, nuiEasingQuartic);
}

double nuiEasingSlowStartRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSlowStart);
}

double nuiEasingQuinticRev(double val)
{
  return nuiEasingReverse(val, nuiEasingQuintic);
}

double nuiEasingSinusStartSlowRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSinusStartSlow);
}

double nuiEasingSinusStartFastRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSinusStartFast);
}

double nuiEasingSinusRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSinus);
}

double nuiEasingBounceOut(double val)
{
	if (val < 1 / 2.75)
  {
		return 7.5625f * val * val;
	}
	else if (val < 2 / 2.75)
  {
		val -= 1.5f / 2.75f;
		return 7.5625f * val * val + 0.75f;
	}
	else if (val < 2.5 / 2.75)
  {
		val -= 2.25f / 2.75f;
		return 7.5625f * val * val + 0.9375f;
	}
  
	val -= 2.625f / 2.75f;
	return 7.5625f * val * val + 0.984375f;
}

double nuiEasingBounceIn(double val)
{
  return nuiEasingReverse(val, nuiEasingBounceOut);
}

double nuiEasingBackIn(double val)
{
	double overshoot = 1.70158f;
	return val * val * ((overshoot + 1) * val - overshoot);
}

double nuiEasingBackOut(double val)
{
  return nuiEasingReverse(val, nuiEasingBackIn);
}


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

static nuiEventSink<nuiAnimation> AnimSink(NULL);

nuiTimer* nuiAnimation::AcquireTimer()
{
  mAnimCounter++;
  if (!mpTimer)
  {
    mpTimer = new nuiTimer(1.0 / mFrameRate);
    AnimSink.Connect(mpTimer->Tick, (void (*)(const nuiEvent&))&nuiAnimation::StartTasks);
    mpTimer->Start(false, false);
  }
  return mpTimer;
}

nuiTimer* nuiAnimation::GetTimer()
{
  return mpTimer;
}

void nuiAnimation::ReleaseTimer()
{
  mAnimCounter--;
  if (!mAnimCounter)
  {
    delete mpTimer;
    AnimSink.DisconnectAll();
    // also kill all tasks:
    std::list<std::pair<int32, nuiTask*> >::iterator it = mOnNextTick.begin();
    std::list<std::pair<int32, nuiTask*> >::iterator end = mOnNextTick.end();
    while (it != end)
    {
      nuiTask* pTask = it->second;
      NGL_ASSERT(pTask->GetRefCount() == 1); // We should be the last ones to hold on to nuiTasks! If you get there then you have done something wrong like forgetting to release your tasks.
      NGL_ASSERT(pTask->IsCanceled()); // If you get there then you have done something wrong like forgetting to cancel your tasks.
      pTask->Release();
      ++it;
    }
    mOnNextTick.clear();
    mpTimer = NULL;
  }
}

nuiAnimation::nuiAnimation()
: mAnimSink(this)
{
  mCount = 0;
  mCurrentPosition = 0;
  mDuration = 0.5;
  mCurrentTime = 0;
  mDirection = 1.0;
  mLoopMode = eAnimLoopForward;
  mUpdatingTime = false;
  mEnableCallbacks = true;
  mDeleteOnStop = false;

  mpEasing = NULL;
  //(nuiEasingMethod)(&::nuiEasingIdentity);
  
  AcquireTimer();
  
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

    AddAttribute(new nuiAttribute<bool>
                 (nglString(_T("DeleteOnStop")), nuiUnitBoolean,
                  nuiMakeDelegate(this, &nuiAnimation::GetDeleteOnStop),
                  nuiMakeDelegate(this, &nuiAnimation::SetDeleteOnStop)));
    
  }
}

void nuiAnimation::SetDeleteOnStop(bool set)
{
  mDeleteOnStop = set;
}

bool nuiAnimation::GetDeleteOnStop() const
{
  return mDeleteOnStop;
}


void nuiAnimation::SetEasing(const nuiEasingMethod& rMethod)
{
  SetEasing(new nuiEasingFunction(rMethod));
}

void nuiAnimation::SetEasing(nuiEasing* pEasing)
{
  pEasing->Acquire();
  if (mpEasing)
    mpEasing->Release();
  mpEasing = pEasing;
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
  return GetDuration() - GetTime();
}


nuiAnimation::~nuiAnimation()
{
  if (mpEasing)
    mpEasing->Release();
  ReleaseTimer();
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

void nuiAnimation::Play(int32 Count, nuiAnimLoop LoopMode)
{
  mCount = Count;
  mLastTime = nglTime();
  mLoopMode = LoopMode;
  if (mLoopMode == eAnimLoopReverse)
    mDirection = -1.0;
  else
    mDirection = 1.0;
  
  mAnimSink.Disconnect(GetTimer()->Tick, &nuiAnimation::OnTick);
  mAnimSink.Connect(GetTimer()->Tick, &nuiAnimation::OnTick);
  mFirstFrameSincePlay = true;

  AnimStart();
}

bool nuiAnimation::PlayOnNextTick(int32 Count, nuiAnimLoop LoopMode)
{
  nuiTask* pTask = nuiMakeTask(this, &nuiAnimation::Play, Count, LoopMode);
  RunOnAnimationTick(pTask);
  return true;
}

void nuiAnimation::Stop()
{
  if (!mUpdatingTime && IsPlaying())
  {
    mCount = 1;
    CallOnFrame();
  }
  
  InternalStop();
}

void nuiAnimation::Pause()
{
  if (!mUpdatingTime && IsPlaying())
  {
    mCount = 1;
    CallOnFrame();
  }
  
  InternalPause();
}

void nuiAnimation::InternalStop()
{
  mCount = 0;
  //NGL_LOG(_T("nuiAnimation), NGL_LOG_DEBUG, _T("Stop at %f\n"), GetDuration());
  mAnimSink.Disconnect(GetTimer()->Tick, &nuiAnimation::OnTick);
  AnimStop();
  mCurrentPosition = 0;

  if (mDeleteOnStop)
    delete this;
}

void nuiAnimation::InternalPause()
{
  mCount = 0;
  //NGL_LOG(_T("nuiAnimation), NGL_LOG_DEBUG, _T("Stop at %f\n"), GetDuration());
  mAnimSink.Disconnect(GetTimer()->Tick, &nuiAnimation::OnTick);
  AnimPause();
}


bool nuiAnimation::IsPlaying() const
{
  return mCount > 0;
}

void nuiAnimation::OnFrame()
{
}

void nuiAnimation::CallOnFrame()
{
  bool ShouldStop = UpdateTime();
  OnFrame();
  if (ShouldStop)
  {
    mUpdatingTime = true;
    bool deleteonstop = GetDeleteOnStop();
    Stop();
    if (!deleteonstop)
      mUpdatingTime = false;
  }
}

void nuiAnimation::OnTick(const nuiEvent& rEvent)
{
  CallOnFrame();
}

bool nuiAnimation::UpdateTime()
{
  bool ShouldStop = false;
  mUpdatingTime = true;
  
  nglTime now;
  double t = now - mLastTime;
  if (!IsPlaying())
    t = 0;

  if (!mFirstFrameSincePlay)
    mCurrentTime += t;
  else
    mFirstFrameSincePlay = false;
  
  mLastTime = now;
  
  double duration = GetDuration();  
  if (mCurrentTime >= duration) 
  {
    mCount--;
    mCurrentTime = 0;
    if (mCount <= 0)
    {
      mCurrentTime = duration;
      ShouldStop = true;
    }
    else 
    {
      AnimLoop();
      if (mLoopMode == eAnimLoopPingPong)
        mDirection = -mDirection;
    }

  }
  
  if (GetDuration() != 0)
  {    
    switch (mLoopMode) 
    {
      case eAnimLoopForward:
        mCurrentPosition = mCurrentTime / GetDuration();
        break;
        
      case eAnimLoopReverse:
        mCurrentPosition = 1.f - (mCurrentTime / GetDuration());
        break;
        
      case eAnimLoopPingPong:
        if (mDirection > 0)
          mCurrentPosition = mCurrentTime / GetDuration();
        else
          mCurrentPosition = 1.f - (mCurrentTime / GetDuration());
        break;
        
      default:
        break;
    }
    
    
    if (mpEasing)
      mCurrentPosition = mpEasing->Map(mCurrentPosition);
  }
  else
  {
    mCurrentPosition = 1.0f;
  }
  
  mUpdatingTime = false;
  return ShouldStop;
}

void nuiAnimation::Play(const nuiEvent& rEvent)
{
  if (mEnableCallbacks)
    Play();
}

void nuiAnimation::Stop(const nuiEvent& rEvent)
{
  if (mEnableCallbacks)
    Stop();
}

void nuiAnimation::Pause(const nuiEvent& rEvent)
{
  if (mEnableCallbacks)
    Pause();
}

void nuiAnimation::EnableCallbacks(bool enable)
{
  mEnableCallbacks = enable;
}


std::list<std::pair<int32, nuiTask*> > nuiAnimation::mOnNextTick;

void nuiAnimation::StartTasks(const nuiEvent& rEvent)
{
  std::list<std::pair<int32, nuiTask*> >::iterator it = mOnNextTick.begin();
  std::list<std::pair<int32, nuiTask*> >::iterator end = mOnNextTick.end();
  while (it != end)
  {
    if (it->first)
    {
      it->first--;
      ++it;
    }
    else
    {
      static int count = 0;
      nuiTask* pTask = it->second;
      pTask->Run();
      pTask->Release();
      mOnNextTick.erase(it++);
      count++;
    }
  }
}

void nuiAnimation::RunOnAnimationTick(nuiTask* pTask, int32 TickCount)
{
  pTask->Acquire();
  mOnNextTick.push_back(std::pair<int32, nuiTask*>(TickCount, pTask));
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

nuiMetaAnimation::~nuiMetaAnimation()
{
  Clear();
}

void nuiMetaAnimation::Play(int32 Count, nuiAnimLoop LoopMode)
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

void nuiMetaAnimation::Pause()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();
  
  for (; it != end; ++it)
  {
    nuiAnimation* pItem = *it;
    if (pItem->IsPlaying())
      pItem->Pause();
  }
  
  nuiAnimation::Pause();
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

nuiAnimationSequence::~nuiAnimationSequence()
{
  Clear();
}

void nuiAnimationSequence::Play(int32 Count, nuiAnimLoop LoopMode)
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

void nuiAnimationSequence::Pause()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();
  
  for (; it != end; ++it)
  {
    nuiAnimation* pItem = *it;
    if (pItem->IsPlaying())
      pItem->Pause();
  }
  
  nuiAnimation::Pause();
}

double nuiAnimationSequence::GetDuration()
{
  std::list<nuiAnimation*>::iterator it = mpAnimations.begin();
  std::list<nuiAnimation*>::iterator end = mpAnimations.end();

  double dur = 0;
  for (; it != end; ++it)
    dur += (*it)->GetDuration();

//  NGL_LOG(_T("nuiAnimationSequence), NGL_LOG_DEBUG, _T("::GetDuration() = %f\n"), dur);
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

void nuiAnimationSequence::OnAnimStopped(const nuiEvent& rEvent)
{
  if (IsPlaying())
  {
    //NGL_LOG(_T("nuiAnimationSequence), NGL_LOG_DEBUG, _T("::OnAnimStopped\n"));
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
      //NGL_LOG(_T("nuiAnimationSequence), NGL_LOG_DEBUG, _T("OnAnimStopped Launch next anim\n"));
    }
    else
    {
      // stop the sequence if this was the last in the chain:
      //NGL_LOG(_T("nuiAnimationSequence), NGL_LOG_DEBUG, _T("OnAnimStopped STOP\n"));
      mpAnimations.front()->Play();
    }
  }
}
