/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#pragma once
#include "nui.h"
#include "nuiAttributeAnimation.h"


class nuiViewController;


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
  
  /// Push a view container to the top of the controller stack
  /// \p pViewController
  /// \p animated: true if the "push" action has to be animated. if false, it's immediate.
  /// \p transition: type of animated transition : sliding effect, elastic "boing" effect, transparency fading effect
  /// \p viewOverlay : the view doesn't replace the previous one, it goes on top of it. Useful if you want transparency effects between the two views.
  void PushViewController(nuiViewController* pViewController, bool animated=true, TransitionType transition = eTransitionSlide, bool viewOverlay = false);
  
  // PopViewControllerAnimated, PopToViewController and PopToRootViewControllerAnimated:
  // the views, which the Pop operation is applied to, are released during the process. 
  // It means they're deleted, unless you specifically acquired a reference to them.
  //
  // example:
  // pViewController->Acquire();
  // [Push the view, do something, Pop the view]
  // [I still want to do something with the view after it's been poped]
  // pViewController->Release(); // then the view is really deleted
  
  /// Go back to the last view container from the top of the controller stack. Same parameters definition.
  void PopViewControllerAnimated(bool animated=true, TransitionType type = eTransitionSlide, bool viewOverlay = false); 
  
  /// Go back to a specific view from the controller stack. The expected view is referenced with its memory pointer.
  void PopToViewController(nuiViewController* pViewController, bool animated=true, TransitionType transition = eTransitionSlide); 
  
  /// Go back to the first view. All the other ones are released.
  void PopToRootViewControllerAnimated(bool animated=true, TransitionType transition = eTransitionSlide); 
  
  /// returns the list of the views from the stack controller
  const std::vector<nuiViewController*>& GetViewControllers();
  
  /// Show/Hide the navigationBar
  void ShowNavigationBar(bool show);
  
  
  void PopPendingOperation();

protected:
  
  void OnViewPushStop(const nuiEvent& rEvent);
  void OnViewPopStop(const nuiEvent& rEvent);
  
  virtual bool Draw(nuiDrawContext* pContext);
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
  
  
  std::vector<nuiViewController*> mViewControllers;
  nuiViewController* mpIn;
  nuiViewController* mpOut;
  
  bool mPushed;
  bool mPoped;
  bool mAlphed;
  bool mShowNavigationBar;

  
  
  
  
  //********************************************
  // transition requests stack, to avoid process overlapping
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
  
  
  
  
  
  
  
  nuiSize mAnimPosition;
  
  
  //********************************************
  // 
  // animation class, only for coding convenience
  // 
  class TransitionAttributeAnimation: public nuiAttributeAnimation
  {
  public:
    TransitionAttributeAnimation(nuiWidget* pTarget, const nglString& rAttribute, float start, float end, const nuiEasingMethod& rEasing, float duration)
    : nuiAttributeAnimation()
    {
      SetTargetObject(pTarget);
      SetTargetAttribute(rAttribute);
      SetStartValue(start);
      SetEndValue(end);
      SetEasing(rEasing);
      SetDuration(duration);
      SetDeleteOnStop(true);
    };
    ~TransitionAttributeAnimation(){};
  };
  
  //********************************************
  // transparency fading animation
  class TransitionAnimation_Alpha: public TransitionAttributeAnimation
  {
  public:
    TransitionAnimation_Alpha(nuiWidget* pTarget, TransitionType transition, float start, float end)
    : TransitionAttributeAnimation(pTarget, _T("Alpha"), start, end, mEasings[transition], mDurations[transition])
    {};
    TransitionAnimation_Alpha(nuiWidget* pTarget, float start, float end, const nuiEasingMethod& rEasing, float duration)
    : TransitionAttributeAnimation(pTarget, _T("Alpha"), start, end, rEasing, duration)
    {};
    virtual ~TransitionAnimation_Alpha(){};
  };

  //********************************************
  // container position animation
  class TransitionAnimation_Position: public TransitionAttributeAnimation
  {
  public:
    TransitionAnimation_Position(nuiWidget* pTarget, TransitionType transition, float start, float end)
    : TransitionAttributeAnimation(pTarget, _T(".AnimPosition"), start, end, mEasings[transition], mDurations[transition])
    {};
    virtual ~TransitionAnimation_Position(){};
  };
  
  
  
  
  
  nuiEventSink<nuiNavigationController> mEventSink;
  
  std::list<nuiAnimation*> mCurrentAnims;
  
  static std::vector<nuiEasingMethod> mEasings;
  static std::vector<float> mDurations;
  
  
};
