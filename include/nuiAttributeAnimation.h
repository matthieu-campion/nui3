/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once

class nuiAttributeAnimation : public nuiAnimation
{
public:
  nuiAttributeAnimation();
  virtual ~nuiAttributeAnimation();
  
  nuiObjectPtr GetTargetObject() const;
  void SetTargetObject(nuiObjectPtr pTarget);
  
  const nglString& GetTargetAttribute() const;
  void SetTargetAttribute(const nglString& rAttribute);
  
  void SetCaptureStartOnPlay(bool set);
  bool GetCaptureStartOnPlay() const;

  void SetCaptureEndOnPlay(bool set);
  bool GetCaptureEndOnPlay() const;
  
  void SetStartValue(double val);
  double GetStartValue() const;

  void SetEndValue(double val);
  double GetEndValue() const;

  // Inherited:
  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.
  
  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.
  
private:
  double mStartValue;
  double mEndValue;
  bool mCaptureStartOnPlay;
  bool mCaptureEndOnPlay;
  nuiObjectPtr mpTarget;
  nglString mTarget;
};

