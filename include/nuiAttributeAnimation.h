/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */


#pragma once

class nuiAttributeAnimationBase : public nuiAnimation
{
public:
  nuiAttributeAnimationBase();
  virtual ~nuiAttributeAnimationBase();

  nuiObjectPtr GetTargetObject() const;
  void SetTargetObject(nuiObjectPtr pTarget);

  const nglString& GetTargetAttribute() const;
  void SetTargetAttribute(const nglString& rAttribute);

  void SetCaptureStartOnPlay(bool set);
  bool GetCaptureStartOnPlay() const;

  void SetCaptureEndOnPlay(bool set);
  bool GetCaptureEndOnPlay() const;

protected:
  bool mCaptureStartOnPlay;
  bool mCaptureEndOnPlay;
  nuiObjectPtr mpTarget;
  nglString mTarget;
};


// Simple class that treats all simple type by converting them to and from strings (ints, floats...)
class nuiAttributeAnimation : public nuiAttributeAnimationBase
{
public:
  nuiAttributeAnimation();
  virtual ~nuiAttributeAnimation();

  void SetStartValue(double val);
  double GetStartValue() const;

  void SetEndValue(double val);
  double GetEndValue() const;

  // Inherited:
  virtual void Play(int32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.
  
private:
  double mStartValue;
  double mEndValue;
};

template <class T>
class nuiAttributeAnim : public nuiAttributeAnimationBase
{
public:
  nuiAttributeAnim()
  : mStartValue(0),
  mEndValue(0)
  {
    if (SetObjectClass(_T("nuiAttributeAnim")))
    {
      // Init atributes
    }
  }
  
  nuiAttributeAnim(T start, T end)
  : mStartValue(start),
  mEndValue(end)
  {
    if (SetObjectClass(_T("nuiAttributeAnim")))
    {
      // Init atributes
    }
  }
  
  ~nuiAttributeAnim()
  {
    
  }
  
  void SetEndValue(T val)
  {
    mEndValue = val;
  }
  
  T GetEndValue() const
  {
    return mEndValue;
  }
  
  void SetStartValue(T val)
  {
    mStartValue = val;
  }
  
  T GetStartValue() const
  {
    return mStartValue;
  }
  
  void Play(int32 Count, nuiAnimLoop LoopMode)
  {
    if (mCaptureStartOnPlay)
    {
      nuiAttrib<T> attrib(mpTarget->GetAttribute(mTarget));
      NGL_ASSERT(attrib.IsValid());
      mStartValue = attrib.Get();
    }
    if (mCaptureEndOnPlay)
    {
      nuiAttrib<T> attrib(mpTarget->GetAttribute(mTarget));
      NGL_ASSERT(attrib.IsValid());
      mEndValue = attrib.Get();
    }
    
    nuiAnimation::Play(Count, LoopMode);
  }
    
  void OnFrame()
  {
    T pos = mStartValue + GetPosition() * (mEndValue - mStartValue);
    nuiAttrib<T> attrib(mpTarget->GetAttribute(mTarget));
    if (!attrib.IsValid() || attrib.IsReadOnly())
      return;
    
    attrib.Set(pos);
  }
  
private:
  T mStartValue;
  T mEndValue;
};


class nuiColorAttributeAnimation : public nuiAttributeAnimationBase
{
public:
  nuiColorAttributeAnimation();
  virtual ~nuiColorAttributeAnimation();

  void SetStartValue(const nuiColor& rColor);
  const nuiColor& GetStartValue() const;

  void SetEndValue(const nuiColor& rColor);
  const nuiColor& GetEndValue() const;

  // Inherited:
  virtual void Play(int32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.

private:
  nuiColor mStartValue;
  nuiColor mEndValue;


};


class nuiRectAttributeAnimation : public nuiAttributeAnimationBase
{
public:
  nuiRectAttributeAnimation();
  virtual ~nuiRectAttributeAnimation();

  void SetStartValue(const nuiRect& rRect);
  const nuiRect& GetStartValue() const;

  void SetEndValue(const nuiRect& rRect);
  const nuiRect& GetEndValue() const;

  // Inherited:
  virtual void Play(int32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.

  void SetLeftAnim(bool set);   ///< If set to true (default) the left   part of the rectangle will be animated. If RightAnim is false, LeftAnim has to be true and vice versa.
  void SetRightAnim(bool set);  ///< If set to true (default) the right  part of the rectangle will be animated. If RightAnim is false, LeftAnim has to be true and vice versa.
  void SetTopAnim(bool set);    ///< If set to true (default) the top    part of the rectangle will be animated. If BottomAnim is false, TopAnim has to be true and vice versa.
  void SetBottomAnim(bool set); ///< If set to true (default) the bottom part of the rectangle will be animated. If BottomAnim is false, TopAnim has to be true and vice versa.
  void SetWidthAnim(bool set);  ///< If set to true (default) the width  of the rectangle will be animated.
  void SetHeightAnim(bool Set); ///< If set to true (default) the height of the rectangle will be animated.
  
  bool GetLeftAnim() const;
  bool GetRightAnim() const;
  bool GetTopAnim() const;
  bool GetBottomAnim() const;
  bool GetWidthAnim() const;
  bool GetHeightAnim() const;
  
  void SetAutoRound(bool set);
  bool GetAutoRound() const;
private:
  nuiRect mStartValue;
  nuiRect mEndValue;

  bool mAnimLeft;
  bool mAnimRight;
  bool mAnimTop;
  bool mAnimBottom;
  bool mAnimWidth;
  bool mAnimHeight;
  
  bool mAutoRound;
};


