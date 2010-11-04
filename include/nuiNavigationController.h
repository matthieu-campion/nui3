/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#pragma once
#include "nui.h"

class nuiViewController;
class nuiNavigationBar;


enum TransitionType
{
  eTransitionSlide=0,
  eTransitionElastic,
  eTransitionNone
};



/// parent container for any nuiViewController object. Handles views switching and transitions
class nuiNavigationController : public nuiSimpleContainer
{
  
public:
  nuiNavigationController();
  virtual ~nuiNavigationController();
  
  void PushnuiViewController(nuiViewController* pnuiViewController, bool animated=true, TransitionType type = eTransitionSlide);
  nuiViewController* PopnuiViewControllerAnimated(bool animated=true, TransitionType type = eTransitionSlide); 
  
  std::vector<nuiViewController*> PopTonuiViewController(nuiViewController* pnuiViewController, bool animated=true, TransitionType type = eTransitionSlide); 
  std::vector<nuiViewController*> PopToRootnuiViewControllerAnimated(bool animated=true, TransitionType type = eTransitionSlide); 
  
protected:
  
  void OnViewPushStop(const nuiEvent& rEvent);
  void OnViewPopStop(const nuiEvent& rEvent);
  
  virtual bool SetRect(const nuiRect& rRect);
  
private:
  
  void InitAttributes();
  void InitStatic();
  
  void SetAnimPosition(nuiSize value);
  nuiSize GetAnimPositon() const;
  
  
  nuiNavigationBar* mpBar;
  std::vector<nuiViewController*> mnuiViewControllers;
  nuiViewController* mpIn;
  nuiViewController* mpOut;
  
  bool mPushed;
  bool mPoped;
  
  nuiSize mAnimPosition;
  nuiEventSink<nuiNavigationController> mEventSink;
  
  static std::vector<nuiEasingMethod> mEasings;
  static std::vector<float> mDurations;
};