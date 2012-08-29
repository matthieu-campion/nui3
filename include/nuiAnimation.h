/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiAnimation_h__
#define __nuiAnimation_h__

//#include "nui.h"
#include "nuiTimer.h"

class nuiTask;

enum nuiAnimWhence
{
  eAnimFromStart,  ///< Move forward, from the beginning of the file (offset 0).
  eAnimForward,    ///< Move forward from the current read/write position.
  eAnimRewind,     ///< Move backwards from the current read/write position.
  eAnimFromEnd     ///< Move backwards from the end of the file.
};

enum nuiAnimLoop
{
  eAnimLoopForward,
  eAnimLoopReverse,
  eAnimLoopPingPong,
};

// Easing delegate:
typedef nuiFastDelegate1<double, double> nuiEasingMethod;

// Standard easing:
double nuiEasingIdentity(double val);
double nuiEasingSquare(double val);
double nuiEasingCubic(double val);
double nuiEasingQuartic(double val);
double nuiEasingSlowStart(double val);
double nuiEasingQuintic(double val);
double nuiEasingSinusStartFast(double val);
double nuiEasingSinusStartSlow(double val);
double nuiEasingSinus(double val);

template <class func> double nuiEasingReverse(double val, func f)
{
  return 1.0 - f(1.0 - val);
}

double nuiEasingSquareRev(double val);
double nuiEasingCubicRev(double val);
double nuiEasingQuarticRev(double val);
double nuiEasingSlowStartRev(double val);
double nuiEasingQuinticRev(double val);
double nuiEasingSinusStartSlowRev(double val);
double nuiEasingSinusStartFastRev(double val);
double nuiEasingSinusRev(double val);

template <int32 percent> double nuiEasingElasticIn(double val) ///< percent = 100 is slow, 500 is a good value
{
  static const double period = 100.0 / (double)percent;
  static const double p = 1.0 / (double)period;
	if (val == 0.0 || val == 1.0)
		return val;

  double s = period / 4.0;
  val = val - 1;
  return -pow(2.0, 10.0 * val) * sin( (val - s) * M_PI * 2.0 * p);
}

template <int32 percent> double nuiEasingElasticOut(double val) ///< percent = 100 is slow, 500 is a good value
{
  return 1.0 - nuiEasingElasticIn<percent>(1.0 - val);
}


double nuiEasingBounceOut(double val);
double nuiEasingBounceIn(double val);
double nuiEasingBackIn(double val);
double nuiEasingBackOut(double val);

class nuiEasing : public nuiRefCount
{
public:
  nuiEasing()
  {
  }
  
  virtual ~nuiEasing()
  {
  }
  
  virtual double Map(double val) = 0;
};

class nuiEasingFunction : public nuiEasing
{
public:
  nuiEasingFunction(const nuiEasingMethod& rFunction)
  : mFunction(rFunction)
  {
  }
  virtual ~nuiEasingFunction()
  {
  }
  
  double Map(double val)
  {
    return mFunction(val);
  }
  
private:
  nuiEasingMethod mFunction;
};



/// nuiAnimation is a base class that implements a basic animation framework.
class NUI_API nuiAnimation : public nuiObject
{
public:
  nuiAnimation();
  virtual ~nuiAnimation();

  bool SetTime(double Time, nuiAnimWhence Whence = eAnimFromStart); ///< Set the time to display. Returns false if Time is out of range (but the current time will be capped on the nearest boundary).
  double GetTime() const; ///< Return the currently displayed time in the animation.
  double GetPosition() const;

  void SetDuration(double); ///< Set the total duration of the Animation in seconds.
  double GetDuration() const; ///< Return the total duration of the Animation in seconds.

  virtual void Play(int32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  bool PlayOnNextTick(int32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< The animation will start playing as soon as the next animation tick is reached. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation and reset the play position so tht the next play will restart from the begining.
  virtual void Pause(); ///< Stop Playing the animation but don't reset the play position, the next play will restart from the last position.

  bool IsPlaying() const; ///< Return true if the animation is currently playing.

  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.

  nuiSimpleEventSource<0> AnimStart;
  nuiSimpleEventSource<0> AnimStop;
  nuiSimpleEventSource<0> AnimPause;
  nuiSimpleEventSource<0> AnimLoop;

  void Play(const nuiEvent& rEvent); ///< The animation will start playing as soon as this method is called. Use this method if you want to start playing an animation when an nuiEvent is fired.
  void Stop(const nuiEvent& rEvent); ///< The animation will stop playing as soon as this method is called. Use this method if you want to stop playing an animation when an nuiEvent is fired.
  void Pause(const nuiEvent& rEvent); ///< The animation will pause as soon as this method is called. Use this method if you want to pause an animation when an nuiEvent is fired.

  static void SetFrameRate(double FPS);
  static double GetFrameRate();
  
  void SetEasing(const nuiEasingMethod& rMethod);
  void SetEasing(nuiEasing* pEasing);
  
  static nuiTimer* AcquireTimer(); ///< You must pair each call to AcquireTimer() with a call to ReleaseTimer().
  static nuiTimer* GetTimer(); ///< GetTimer doesn't allocate the timer and you must not pair it with ReleaseTimer(). It may return NULL if the timer was never created.
  static void ReleaseTimer();
  static void RunOnAnimationTick(nuiTask* pTask, int32 WaitNTickCount = 0);
  
  void SetDeleteOnStop(bool set); ///< If DeleteOnStop is set, the animation will be deleted as soon as it is stopped by calling Stop or by reaching the end of the loop counter.
  bool GetDeleteOnStop() const; ///< If DeleteOnStop is set, the animation will be deleted as soon as it is stopped by calling Stop or by reaching the end of the loop counter.
  
protected:
  void CallOnFrame();
  bool UpdateTime(); ///< This method returns the number time elapsed since the last call to UpdateTime.
  void OnTick(const nuiEvent& rEvent);
  void InternalStop();
  void InternalPause();

  bool mFirstFrameSincePlay;
  double mCurrentTime;
  double mCurrentPosition;
  double mDuration;
  int32 mCount;

  double mDirection; ///< Either 1 or -1. This sets the current direction of play back.
  nuiAnimLoop mLoopMode;
  nuiEasing* mpEasing;

  static nuiTimer* mpTimer;
  static int32 mAnimCounter;
  static double mFrameRate;
  static std::list<std::pair<int32, nuiTask*> > mOnNextTick;
  static void StartTasks(const nuiEvent& rEvent);
  
  nglTime mLastTime;
  bool mUpdatingTime;

  bool mEnableCallbacks;
  
  bool mDeleteOnStop;

  void EnableCallbacks(bool enable);

  void SetTimeFromStart(double Time);
  void SetTimeFromNow(double Time);
  void SetTimeFromEnd(double Time);
  
  double GetTimeFromStart() const;
  double GetTimeFromNow() const;
  double GetTimeFromEnd() const;
  
  nuiEventSink<nuiAnimation> mAnimSink;

};

class NUI_API nuiWaitAnimation : public nuiAnimation
{
public:
  nuiWaitAnimation(double duration)
  {
    mUserDuration = duration;
  }

  virtual ~nuiWaitAnimation()
  {
  }

  double GetDuration()
  {
    return mUserDuration;
  }

protected:
  double mUserDuration;
};

class NUI_API nuiMetaAnimation : public nuiAnimation
{
public:
  nuiMetaAnimation ();
  virtual ~nuiMetaAnimation();

  virtual void Play(int32 Count = 1.0f, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.
  virtual void Pause(); ///< Pause Playing the animation.

  virtual double GetDuration(); ///< Return the total duration of the Animation in seconds.

  void AddAnimation(nuiAnimation* pAnim);
  void Clear();
protected:
  std::list<nuiAnimation*> mpAnimations;
};

class NUI_API nuiAnimationSequence : public nuiAnimation
{
public:
  nuiAnimationSequence ();
  virtual ~nuiAnimationSequence();

  virtual void Play(int32 Count = 1.0f, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.
  virtual void Pause(); ///< Pause Playing the animation.

  virtual double GetDuration(); ///< Return the total duration of the Animation in seconds.

  void AddAnimation(nuiAnimation* pAnim);
  void Clear();

  virtual void OnFrame();
protected:
  std::list<nuiAnimation*> mpAnimations;

  void OnAnimStopped(const nuiEvent& rEvent);
  nuiEventSink<nuiAnimationSequence> mAnimSequenceSink;
};

#endif // __nuiAnimation_h__
