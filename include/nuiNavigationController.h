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
  eTransitionTransparency,
  eTransitionNone
};



/// parent container for any nuiViewController object. Handles views switching and transitions
class nuiNavigationController : public nuiSimpleContainer, public nuiNotificationObserver
{
  
public:
  nuiNavigationController();
  virtual ~nuiNavigationController();
  
  void PushViewController(nuiViewController* pViewController, bool animated=true, TransitionType transition = eTransitionSlide, bool viewOverlay = false);
  void PopViewControllerAnimated(bool animated=true, TransitionType type = eTransitionSlide, bool viewOverlay = false); 
  
  void PopToViewController(nuiViewController* pViewController, bool animated=true, TransitionType transition = eTransitionSlide); 
  void PopToRootViewControllerAnimated(bool animated=true, TransitionType transition = eTransitionSlide); 
  
  const std::vector<nuiViewController*>& GetViewControllers();
  
protected:
  
  void OnViewPushStop(const nuiEvent& rEvent);
  void OnViewPopStop(const nuiEvent& rEvent);
  
  virtual bool SetRect(const nuiRect& rRect);
  
private:
  
  void _PushViewController(nuiViewController* pViewController, bool animated, TransitionType transition, bool viewOverlay = false);
  void _PopViewControllerAnimated(bool animated, TransitionType type, bool viewOverlay = false); 
  
  void _PopToViewController(nuiViewController* pViewController, bool animated, TransitionType transition); 
  void _PopToRootViewControllerAnimated(bool animated, TransitionType transition); 

  
  virtual void ConnectTopLevel();
  virtual void OnNotification(const nuiNotification& rNotif);  
  
  void InitAttributes();
  void InitStatic();
  
  void SetAnimPosition(nuiSize value);
  nuiSize GetAnimPositon() const;
  
  
  nuiNavigationBar* mpBar;
  std::vector<nuiViewController*> mViewControllers;
  nuiViewController* mpIn;
  nuiViewController* mpOut;
  
  bool mPushed;
  bool mPoped;
  bool mAlphed;

  bool mPendingLayout;
  enum PendingOperationType {ePendingPush, ePendingPop, ePendingPopTo, ePendingPopToRoot};
  class PendingOperation
  {
  public:
    PendingOperation(nuiViewController* pView, PendingOperationType type, bool animated, TransitionType transition, bool viewOverlay){mpView = pView; mType = type; mAnimated = animated; mTransition = transition; mOverlay = viewOverlay;};
    nuiViewController* mpView;
    PendingOperationType mType;
    bool mAnimated;
    TransitionType mTransition;
    bool mOverlay;
  };
  std::list<PendingOperation> mPendingOperations;
  void PopPendingOperation();
  
  nuiSize mAnimPosition;
  nuiEventSink<nuiNavigationController> mEventSink;
  
  std::list<nuiAnimation*> mCurrentAnims;
  
  static std::vector<nuiEasingMethod> mEasings;
  static std::vector<float> mDurations;
  
  
};