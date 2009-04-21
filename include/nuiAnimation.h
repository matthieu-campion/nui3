/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiAnimation_h__
#define __nuiAnimation_h__

//#include "nui.h"
#include "nuiTimer.h"

#include "nuiEventRegistry.h"

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
inline double nuiEasingIdentity(double val)
{
  return val;
}

inline double nuiEasingSquare(double val)
{
  return val * val;
}


inline double nuiEasingCubic(double val)
{
  return val * val * val;
}

inline double nuiEasingQuartic(double val)
{
  return val * val * val * val;
}

inline double nuiEasingSlowStart(double val)
{
  return val * val * val * val * val * val * val * val;
}

inline double nuiEasingQuintic(double val)
{
  return val * val * val * val;
}

inline double nuiEasingSinusStartFast(double val)
{
  return sin(val * M_PI_2);
}

inline double nuiEasingSinusStartSlow(double val)
{
  return sin(val * M_PI_2 - M_PI_2) + 1;
}

inline double nuiEasingSinus(double val)
{
  return .5 * (sin(val * M_PI - M_PI_2) + 1);
}

template <class func> double nuiEasingReverse(double val, func f)
{
  return 1.0 - f(1.0 - val);
}

inline double nuiEasingSquareRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSquare);
}

inline double nuiEasingCubicRev(double val)
{
  return nuiEasingReverse(val, nuiEasingCubic);
}

inline double nuiEasingQuarticRev(double val)
{
  return nuiEasingReverse(val, nuiEasingQuartic);
}

inline double nuiEasingSlowStartRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSlowStart);
}

inline double nuiEasingQuinticRev(double val)
{
  return nuiEasingReverse(val, nuiEasingQuintic);
}

inline double nuiEasingSinusStartSlowRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSinusStartSlow);
}

inline double nuiEasingSinusStartFastRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSinusStartFast);
}

inline double nuiEasingSinusRev(double val)
{
  return nuiEasingReverse(val, nuiEasingSinus);
}


/// nuiAnimation is a base class that implements a basic animation framework.
class NUI_API nuiAnimation : public nuiObject
{
public:
  nuiAnimation();
  virtual ~nuiAnimation();

  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode) const;

  bool SetTime(double Time, nuiAnimWhence Whence = eAnimFromStart); ///< Set the time to display. Returns false if Time is out of range (but the current time will be capped on the nearest boundary).
  double GetTime() const; ///< Return the currently displayed time in the animation.
  double GetPosition() const;

  void SetDuration(double); ///< Set the total duration of the Animation in seconds.
  double GetDuration() const; ///< Return the total duration of the Animation in seconds.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  bool IsPlaying() const; ///< Return true if the animation is currently playing.

  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.

  nuiSimpleEventSource<nuiAnimationStart> AnimStart;
  nuiSimpleEventSource<nuiAnimationStop> AnimStop;
  nuiSimpleEventSource<nuiAnimationLoop> AnimLoop;

  bool Play(const nuiEvent& rEvent); ///< The animation will start playing as soon as this method is called. Use this method if you want to start playing an animation when an nuiEvent is fired.
  bool Stop(const nuiEvent& rEvent); ///< The animation will stop playing as soon as this method is called. Use this method if you want to stop playing an animation when an nuiEvent is fired.

  static void SetFrameRate(double FPS);
  static double GetFrameRate();
  
  void SetEasing(const nuiEasingMethod& rMethod);
  
  static nuiTimer* AcquireTimer(); ///< You must pair each call to AcquireTimer() with a call to ReleaseTimer().
  static nuiTimer* GetTimer(); ///< GetTimer doesn't allocate the timer and you must not pair it with ReleaseTimer(). It may return NULL if the timer was never created.
  static void ReleaseTimer();
  
protected:
  void CallOnFrame();
  double UpdateTime(); ///< This method returns the number time elapsed since the last call to UpdateTime.
  bool OnTick(const nuiEvent& rEvent);

  double mCurrentTime;
  double mCurrentPosition;
  double mDuration;
  uint32 mCount;

  double mDirection; ///< Either 1 or -1. This sets the current direction of play back.
  nuiAnimLoop mLoopMode;
  nuiEasingMethod mEasing;

  static nuiTimer* mpTimer;
  static uint32 mAnimCounter;
  static double mFrameRate;
  
  nglTime mLastTime;
  bool mUpdatingTime;

  bool mEnableCallbacks;

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
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiMetaAnimation();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode) const;

  virtual void Play(uint32 Count = 1.0f, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

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
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiAnimationSequence();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode) const;

  virtual void Play(uint32 Count = 1.0f, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual double GetDuration(); ///< Return the total duration of the Animation in seconds.

  void AddAnimation(nuiAnimation* pAnim);
  void Clear();

  virtual void OnFrame();
protected:
  std::list<nuiAnimation*> mpAnimations;

  bool OnAnimStopped(const nuiEvent& rEvent);
  nuiEventSink<nuiAnimationSequence> mAnimSequenceSink;
};

#endif // __nuiAnimation_h__
