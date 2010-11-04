/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nuiNavigationController.h"
#include "nuiViewController.h"
#include "nuiAttributeAnimation.h"



// static inits
//
std::vector<nuiEasingMethod> nuiNavigationController::mEasings;
std::vector<float> nuiNavigationController::mDurations;
//



nuiNavigationController::nuiNavigationController()
: nuiSimpleContainer(), mEventSink(this)
{
  if (SetObjectClass(_T("nuiNavigationController")))
    InitAttributes();
  
  if (mEasings.size() == 0)
    InitStatic();
  
  mpIn = NULL;
  mpOut = NULL;
  mPushed = false;
  mPoped = false;
  
  mPending = true;
  mpPendingViewController = NULL;
  mPendingAnimated = false;
  mPendingType = eTransitionNone;
  
}



nuiNavigationController::~nuiNavigationController()
{
  
}

void nuiNavigationController::InitStatic()
{
  mEasings.resize(eTransitionNone);
  mDurations.resize(eTransitionNone);
  
  mEasings[eTransitionSlide] = nuiEasingSinusStartFast;
  mDurations[eTransitionSlide] = 0.25f;
  
  mEasings[eTransitionElastic] = nuiEasingElasticOut<300>;
  mDurations[eTransitionElastic] = 0.8f;
}


void nuiNavigationController::InitAttributes()
{
  mAnimPosition = 0.f;
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T(".AnimPosition")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiNavigationController::GetAnimPositon), 
                nuiMakeDelegate(this, &nuiNavigationController::SetAnimPosition)));  
}


void nuiNavigationController::SetAnimPosition(nuiSize value)
{
  //LBDEBUG
  NGL_OUT(_T("SetAnimPosition %.2f\n"));
  
  mAnimPosition = value;
  UpdateLayout();
}


nuiSize nuiNavigationController::GetAnimPositon() const
{
  return mAnimPosition;
}


void nuiNavigationController::PushViewController(nuiViewController* pViewController, bool animated, TransitionType type)
{
  // don't overlapp animations
  if (mPushed || mPoped)
    return;
  
  // store the push request if we're not connected to the top level yet
  if (mPending)
  {
    mpPendingViewController = pViewController;
    mPendingAnimated = animated;
    mPendingType = type;
    return;
  }
  
  mpIn = pViewController;
  mpOut  = NULL;
  
  mpIn->mAnimated = animated;
  if (mpOut)
    mpOut->mAnimated = animated;
  
  if (mnuiViewControllers.size() >0)
    mpOut = mnuiViewControllers.back();
  
  // push the new view in the stack
  mnuiViewControllers.push_back(mpIn);
  
  nuiRect idealsize = GetRect().Size();
  
  
  // animation was requested. launch animation and connect the event
  if (animated && (type != eTransitionNone))
  {
    mPushed = true;
    mPoped = false;
    
    mAnimPosition = idealsize.GetWidth();
    
    AddChild(mpIn);  
    
    nuiAttributeAnimation* pAnim = new nuiAttributeAnimation();
    pAnim->SetTargetObject(this);
    pAnim->SetTargetAttribute(_T(".AnimPosition"));
    pAnim->SetStartValue(idealsize.GetWidth());
    pAnim->SetEndValue(0);
    pAnim->SetEasing(mEasings[type]);
    pAnim->SetDuration(mDurations[type]);
    mEventSink.Connect(pAnim->AnimStop, &nuiNavigationController::OnViewPushStop, (void*)pAnim);
    pAnim->Play();
  }
  else 
  {
    AddChild(mpIn);  
  }
  
  
  if (!animated && mpOut)
  {
    mpOut->ViewDidDisappear();
    DelChild(mpOut);
  }
  
}




nuiViewController* nuiNavigationController::PopViewControllerAnimated(bool animated, TransitionType type)
{
  // don't overlapp animations
  if (mPushed || mPoped)
    return NULL;
  
  if (mnuiViewControllers.size() < 1)
  {
    NGL_OUT(_T("nuiNavigationController::popViewControllerAnimated : nothing to pop"));
    return NULL;
  }
  
  mpOut = mnuiViewControllers[mnuiViewControllers.size() -1];
  mpIn = NULL;
  if (mnuiViewControllers.size() > 1)
    mpIn = mnuiViewControllers[mnuiViewControllers.size() -2];
  
  nuiRect idealsize = GetRect().Size();
  
  // animation was requested. launch animation and connect the event
  if (animated && (type != eTransitionNone))
  {
    mPushed = false;
    mPoped = true;
    
    mAnimPosition = -idealsize.GetWidth();
    
    AddChild(mpIn);  
    
    nuiAttributeAnimation* pAnim = new nuiAttributeAnimation();
    pAnim->SetTargetObject(this);
    pAnim->SetTargetAttribute(_T(".AnimPosition"));
    pAnim->SetStartValue(-idealsize.GetWidth());
    pAnim->SetEndValue(0);
    pAnim->SetEasing(mEasings[type]);
    pAnim->SetDuration(mDurations[type]);
    mEventSink.Connect(pAnim->AnimStop, &nuiNavigationController::OnViewPopStop, (void*)pAnim);
    pAnim->Play();
  }
  else
  {
    AddChild(mpIn);  
    mpOut->ViewDidDisappear();
    DelChild(mpOut);
    mnuiViewControllers.pop_back();
  }
  
  return mpOut;
}





// virtual 
bool nuiNavigationController::SetRect(const nuiRect& rRect)
{
  if (!mPushed && !mPoped)
    nuiSimpleContainer::SetRect(rRect);
  else
    nuiWidget::SetRect(rRect);
 
  // pending operation, if any...
  if (mPending)
  {
    mPending = false;
    if (mpPendingViewController)
    {
      PushViewController(mpPendingViewController, mPendingAnimated, mPendingType);
      mpPendingViewController = NULL;
      mPendingAnimated = false;
      mPendingType = eTransitionNone;
    }
  }

  // nothing to layout right now...
  if (!mPushed && !mPoped)
    return true;

  
  nuiRect rect;
  rect.Set(mAnimPosition, rRect.Top(), rRect.GetWidth(), rRect.GetHeight());
  mpIn->SetLayout(rect);
  
  if (mpOut)
  {
    if (mPushed)
      rect.Set(mAnimPosition - rRect.GetWidth(), rRect.Top(), rRect.GetWidth(), rRect.GetHeight());
    else
      rect.Set(mAnimPosition + rRect.GetWidth(), rRect.Top(), rRect.GetWidth(), rRect.GetHeight());
    
    mpOut->SetLayout(rect);
  }
  
  return true;
}




std::vector<nuiViewController*> nuiNavigationController::PopToViewController(nuiViewController* pViewController, bool animated, TransitionType type)
{
  // don't overlapp animations
  if (mPushed || mPoped)
    return mnuiViewControllers;
  
}


std::vector<nuiViewController*> nuiNavigationController::PopToRootViewControllerAnimated(bool animated, TransitionType type)
{
  // don't overlapp animations
  if (mPushed || mPoped)
    return mnuiViewControllers;
  
  
}



void nuiNavigationController::OnViewPushStop(const nuiEvent& rEvent)
{
  nuiAttributeAnimation* pAnim = (nuiAttributeAnimation*)rEvent.mpUser;
  delete pAnim;
  
  mPushed = false;
  mPoped = false;
  
  if (mnuiViewControllers.size() >1) 
  {
    mpOut = mnuiViewControllers[mnuiViewControllers.size()-2];  
    mpOut->ViewDidDisappear();
    if (mpOut->GetParent())
      DelChild(mpOut);
  }
  
}



void nuiNavigationController::OnViewPopStop(const nuiEvent& rEvent)
{
  nuiAttributeAnimation* pAnim = (nuiAttributeAnimation*)rEvent.mpUser;
  delete pAnim;
  
  mPushed = false;
  mPoped = false;
  
  mpOut->ViewDidDisappear();
  if (mpOut->GetParent())
  {
    DelChild(mpOut);
    mnuiViewControllers.pop_back();  
  }
  
}







