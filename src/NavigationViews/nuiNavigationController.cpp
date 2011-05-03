/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nuiNavigationController.h"
#include "nuiViewController.h"
#include "nuiNavigationBar.h"


#define NOTIF_PENDING_OPERATION _T("nuiNavigationController_PendingOp")
#define NAVBAR_ANIM_ALPHA_DURATION 0.25f


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
  mAlphed = false;
  
  mPendingLayout = true;
  
 
}



nuiNavigationController::~nuiNavigationController()
{
  std::list<nuiAnimation*>::iterator it;
  for (it = mCurrentAnims.begin(); it != mCurrentAnims.end(); ++it)
    delete *it;
}

void nuiNavigationController::InitStatic()
{
  mEasings.resize(eTransitionNone);
  mDurations.resize(eTransitionNone);
  
  mEasings[eTransitionSlide] = nuiEasingSinusStartFast;
  mDurations[eTransitionSlide] = 0.25f;
  
  mEasings[eTransitionElastic] = nuiEasingElasticOut<300>;
  mDurations[eTransitionElastic] = 0.8f;

  mEasings[eTransitionTransparency] = nuiEasingSinusStartFast;
  mDurations[eTransitionTransparency] = 0.8f;
}


void nuiNavigationController::InitAttributes()
{
  mAnimPosition = 0.f;
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T(".AnimPosition")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiNavigationController::GetAnimPositon), 
                nuiMakeDelegate(this, &nuiNavigationController::SetAnimPosition)));  
}


const std::vector<nuiViewController*>& nuiNavigationController::GetViewControllers()
{
  return mViewControllers;
}


void nuiNavigationController::SetAnimPosition(nuiSize value)
{
  mAnimPosition = value;
  UpdateLayout();
}


nuiSize nuiNavigationController::GetAnimPositon() const
{
  return mAnimPosition;
}

void nuiNavigationController::PushViewController(nuiViewController* pViewController, bool animated, TransitionType transition, bool viewOverlay)
{
  // store a pending operation and call for a asynchronous process
  mPendingOperations.push_back(PendingOperation(pViewController, ePendingPush, animated, transition, viewOverlay));
  GetTopLevel()->PostNotification(new nuiNotification(NOTIF_PENDING_OPERATION));
}


void nuiNavigationController::_PushViewController(nuiViewController* pViewController, bool animated, TransitionType transition, bool viewOverlay)
{
  // don't overlapp animations
  if (mPushed || mPoped || mAlphed)
  {
    mPendingOperations.push_back(PendingOperation(pViewController, ePendingPush, animated, transition, viewOverlay));
    return;
  }
  
  // store the push request if we're not connected to the top level yet
  if (mPendingLayout)
  {
    mPendingOperations.push_back(PendingOperation(pViewController, ePendingPush, animated, transition, viewOverlay));
    return;
  }

  pViewController->GetNavigationBar()->Acquire();
  pViewController->Acquire();
  mpIn = pViewController;
  mpOut  = NULL;
  
  mpIn->mAnimated = animated;
  //LBDEBUG
//  mpIn->SetAlpha(1);
//  mpIn->GetNavigationBar()->SetAlpha(1);
  
  if ((mViewControllers.size() >0) && !viewOverlay)
  {
    mpOut = mViewControllers.back();
    mpOut->mAnimated = animated;
    //LBDEBUG
//    mpOut->SetAlpha(1);
//    mpOut->GetNavigationBar()->SetAlpha(1);
  }

  // push the new view in the stack
  mViewControllers.push_back(mpIn);
  
  // virtual cbk
  if (mpOut)
    mpOut->ViewWillDisappear();
  mpIn->ViewWillAppear();

  nuiRect idealsize = GetRect().Size();

  //*****************************************************
  //
  // TRANSPARENCY FADING ANIMATION REQUESTED
  //
  if (animated && (transition == eTransitionTransparency))
  {
    mPushed = false;
    mPoped = false;
    mAlphed = true;
    
    mAnimPosition = 0;
    
    //LBDEBUG
//    mpIn->SetAlpha(0);
//    mpIn->GetNavigationBar()->SetAlpha(0);
    AddChild(mpIn);
    AddChild(mpIn->GetNavigationBar());
    
    // FADE IN animation
    TransitionAnimation_Alpha* pAnim = NULL;

    //LBDEBUG
//    pAnim = new TransitionAnimation_Alpha(mpIn, transition, 0, 1);
//    mEventSink.Connect(pAnim->AnimStop, &nuiNavigationController::OnViewPushStop, (void*)viewOverlay);
//    NGL_ASSERT(mCurrentAnims.size() == 0);
//    mCurrentAnims.push_back(pAnim);
//    pAnim->Play();

    // nav bar FADE IN animation
    if (mpIn->GetNavigationBar()->IsVisible())
    {
//LBDEBUG
      //      pAnim = new TransitionAnimation_Alpha(mpIn->GetNavigationBar(), 0, 1, nuiEasingSinusStartFast, NAVBAR_ANIM_ALPHA_DURATION);
//      mCurrentAnims.push_back(pAnim);
//      pAnim->Play();
    }
    
    // FADE OUT animation
    if (mpOut)
    {
      //LBDEBUG
//      pAnim = new TransitionAnimation_Alpha(mpOut, transition, 1, 0);
//      mCurrentAnims.push_back(pAnim);
//      pAnim->Play();    
    }
    
  }
  //*****************************************************
  //
  // OTHER TYPE OF ANIMATION REQUESTED
  //
  else if (animated && (transition != eTransitionNone))
  {
    mPushed = true;
    mPoped = false;
    mAlphed = false;
     
    mAnimPosition = idealsize.GetWidth();
    
    AddChild(mpIn);  
    AddChild(mpIn->GetNavigationBar());

    // container POSITION animation
    TransitionAnimation_Position* pAnim = new TransitionAnimation_Position(this, transition, idealsize.GetWidth(), 0);
    mEventSink.Connect(pAnim->AnimStop, &nuiNavigationController::OnViewPushStop, (void*)viewOverlay);
    NGL_ASSERT(mCurrentAnims.size() == 0);
    mCurrentAnims.push_back(pAnim);
    pAnim->Play();
    
    // nav bar FADE IN animation
    if (mpIn->GetNavigationBar()->IsVisible())
    {
      //LBDEBUG
//      TransitionAnimation_Alpha* pAnim = new TransitionAnimation_Alpha(mpIn->GetNavigationBar(), 0, 1, nuiEasingSinusStartFast, NAVBAR_ANIM_ALPHA_DURATION);
//      mCurrentAnims.push_back(pAnim);
//      pAnim->Play();
    }    
  }
  else 
  {
    AddChild(mpIn);  
    AddChild(mpIn->GetNavigationBar());
  }
  
  
  if (!animated && mpOut && !viewOverlay)
  {
    mpOut->ViewDidDisappear();
    DelChild(mpOut->GetNavigationBar());
    DelChild(mpOut);
    // mpIn->ViewDidAppear is made in nuiViewController::ConnectTopLevel
  }

  if (!animated)
    if (mPendingOperations.size() >0)
      PopPendingOperation();
}




void nuiNavigationController::PopViewControllerAnimated(bool animated, TransitionType transition, bool viewOverlay)
{
  // store a pending operation and call for a asynchronous process
  mPendingOperations.push_back(PendingOperation(NULL, ePendingPop, animated, transition, viewOverlay));
  GetTopLevel()->PostNotification(new nuiNotification(NOTIF_PENDING_OPERATION));
}


void nuiNavigationController::_PopViewControllerAnimated(bool animated, TransitionType transition, bool viewOverlay)
{
  if (mViewControllers.size() < 1)
  {
    NGL_OUT(_T("nuiNavigationController::popViewControllerAnimated : nothing to pop"));
    return;
  }

  // don't overlapp animations
  if (mPushed || mPoped || mAlphed)
  {
    mPendingOperations.push_back(PendingOperation(NULL, ePendingPop, animated, transition, viewOverlay));
    return;
  }
  
  mpOut = mViewControllers[mViewControllers.size() -1];
  mpIn = NULL;
  if ((mViewControllers.size() > 1) && !viewOverlay)
    mpIn = mViewControllers[mViewControllers.size() -2];
  
  nuiRect idealsize = GetRect().Size();
  
  //LBDEBUG
//  mpOut->SetAlpha(1);
//  mpOut->GetNavigationBar()->SetAlpha(1);
//  if (mpIn)
//  {
//    mpIn->SetAlpha(1);
//    mpIn->GetNavigationBar()->SetAlpha(1);
//  }
  
  // virtual cbk
  mpOut->ViewWillDisappear();
  if (mpIn)
    mpIn->ViewWillAppear();

  
  //*****************************************************
  //
  // TRANSPARENCY FADING ANIMATION REQUESTED
  //
  if (animated && (transition == eTransitionTransparency))
  {
    mPushed = false;
    mPoped = false; 
    mAlphed = true;
    mAnimPosition = 0;
    
    if (mpIn)
    {
      AddChild(mpIn);  
      AddChild(mpIn->GetNavigationBar());
      //LBDEBUG
//      mpIn->SetAlpha(0);
//      mpIn->GetNavigationBar()->SetAlpha(0);
    
      // FADE IN animation
      TransitionAnimation_Alpha* pAnim = NULL;
      //LBDEBUG
//      pAnim = new TransitionAnimation_Alpha(mpIn, transition, 0, 1);
//      mEventSink.Connect(pAnim->AnimStop, &nuiNavigationController::OnViewPopStop, (void*)viewOverlay);
//      NGL_ASSERT(mCurrentAnims.size() == 0);
//      mCurrentAnims.push_back(pAnim);
//      pAnim->Play();
    
      // nav bar FADE IN animation
      if (mpIn->GetNavigationBar()->IsVisible())
      {
        //LBDEBUG
//        pAnim = new TransitionAnimation_Alpha(mpIn->GetNavigationBar(), 0, 1, nuiEasingSinusStartFast, NAVBAR_ANIM_ALPHA_DURATION);
//        mCurrentAnims.push_back(pAnim);
//        pAnim->Play();
      }
    }
    
    
    // FADE OUT animation
    //LBDEBUG
//    TransitionAnimation_Alpha* pAnim = new TransitionAnimation_Alpha(mpOut, transition, 1, 0);
//    mCurrentAnims.push_back(pAnim);
//    pAnim->Play();
    
  }
  //*****************************************************
  //
  // OTHER TYPE OF ANIMATION REQUESTED
  //
  else if (animated && (transition != eTransitionNone))
  {
    mPushed = false;
    mPoped = true;
    mAlphed = false;
    
    mAnimPosition = -idealsize.GetWidth();
    
    if (mpIn)
    {
      AddChild(mpIn);  
      AddChild(mpIn->GetNavigationBar());
      
      // nav bar FADE IN animation
      if (mpIn->GetNavigationBar()->IsVisible())
      {
        //LBDEBUG
//        TransitionAnimation_Alpha* pAnim = new TransitionAnimation_Alpha(mpIn->GetNavigationBar(), 0, 1, nuiEasingSinusStartFast, NAVBAR_ANIM_ALPHA_DURATION);
//        mCurrentAnims.push_back(pAnim);
//        pAnim->Play();
      }      
    }
    
    // container POSITION animation
    TransitionAnimation_Position* pAnim = new TransitionAnimation_Position(this, transition, -idealsize.GetWidth(), 0);
    mEventSink.Connect(pAnim->AnimStop, &nuiNavigationController::OnViewPopStop, (void*)viewOverlay);
    mCurrentAnims.push_back(pAnim);
    pAnim->Play();
  }
  else
  {
    if (mpIn)
    {
      AddChild(mpIn);  
      AddChild(mpIn->GetNavigationBar());
    }
    mpOut->ViewDidDisappear();
    mpOut->GetNavigationBar()->Release();
    DelChild(mpOut->GetNavigationBar());
    mpOut->Release();
    DelChild(mpOut);
    
    // mpIn->ViewDidAppear() is made in nuiViewController::ConnectTopLevel
    
    mViewControllers.pop_back();
    
    if (mPendingOperations.size() >0)
      PopPendingOperation();
  }

}



// virtual 
bool nuiNavigationController::Draw(nuiDrawContext* pContext)
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && (pItem->GetObjectClass() != _T("nuiNavigationBar")))
      DrawChild(pContext, pItem);
  }

  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && (pItem->GetObjectClass() == _T("nuiNavigationBar")))
      DrawChild(pContext, pItem);
  }
  
  delete pIt;
  
  return true;
}


// virtual 
bool nuiNavigationController::SetRect(const nuiRect& rRect)
{
  if (!mPushed && !mPoped)
    nuiSimpleContainer::SetRect(rRect);
  else
    nuiWidget::SetRect(rRect);
 
  // pending operation, if any...
  if (mPendingLayout)
  {
    mPendingLayout = false;
    if (mPendingOperations.size() >0)
      PopPendingOperation();
  }

  // nothing to layout right now...
  if (!mPushed && !mPoped)
    return true;

  nuiRect rect;
  rect.Set(mAnimPosition, rRect.Top(), rRect.GetWidth(), rRect.GetHeight());
  if (mpIn)
  {
    //ICI
    mpIn->SetLayout(rect);
  }
  
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


void nuiNavigationController::PopPendingOperation()
{
  if (mPendingOperations.size() == 0)
    return;

  PendingOperation op = mPendingOperations.front();
  mPendingOperations.pop_front();
 
  switch (op.mType)
  {
  case ePendingPush:
    _PushViewController(op.mpView, op.mAnimated, op.mTransition, op.mOverlay);
    break;
  case ePendingPop:
    _PopViewControllerAnimated(op.mAnimated, op.mTransition, op.mOverlay);
    break;
  case ePendingPopTo:
    _PopToViewController(op.mpView, op.mAnimated, op.mTransition);
    break;
  case ePendingPopToRoot:
    _PopToRootViewControllerAnimated(op.mAnimated, op.mTransition);
    break;
  }    

  UpdateLayout();
}



void nuiNavigationController::PopToViewController(nuiViewController* pViewController, bool animated, TransitionType transition)
{
  // store a pending operation and call for a asynchronous process
  mPendingOperations.push_back(PendingOperation(pViewController, ePendingPopTo, animated, transition, false));
  GetTopLevel()->PostNotification(new nuiNotification(NOTIF_PENDING_OPERATION));
}


void nuiNavigationController::_PopToViewController(nuiViewController* pViewController, bool animated, TransitionType transition)
{
 if (mViewControllers.size() < 2)
  {
    NGL_OUT(_T("nuiNavigationController::PopToViewController : nothing to pop to..."));
    return;
  }

  // don't overlapp animations
  if (mPushed || mPoped || mAlphed)
  {
    mPendingOperations.push_back(PendingOperation(pViewController, ePendingPopTo, animated, transition, false));
    return;
  }
  
  bool done = false;
  std::vector<nuiViewController*>::iterator it;
  while (!done)
  {
    if (mViewControllers.size() < 2)
      break;
    
    it = mViewControllers.end(); --it; --it;
    
    if (*it == pViewController)
    {
      done = true;
      break;
    }
    
    nuiViewController* pView = *it;

    // objects have already been detached from the tree (using DelChild, in ::OnViewPushStop (or in ::_PushViewController if no transition)).
    // but their references had not been released to keep the objects alive (in case of specific pop operation, you want those objects come back to the screen)
    // therefore, to delete them here, don't call DelChild, simply call Release for the objects reference to be decreased (and let the nui references system manage the memory).
    pView->GetNavigationBar()->Release();
    pView->Release();
    
    mViewControllers.erase(it);
  }
  
  if (!done)
  {
    NGL_OUT(_T("nuiNavigationController::PopToViewController error : could not find any ViewController %p\n"));
    return;  
  }
  
  // now, the main pop operation
  _PopViewControllerAnimated(animated, transition);
  
}




void nuiNavigationController::PopToRootViewControllerAnimated(bool animated, TransitionType transition)
{
  // store a pending operation and call for a asynchronous process
  mPendingOperations.push_back(PendingOperation(NULL, ePendingPopToRoot, animated, transition, false));
  GetTopLevel()->PostNotification(new nuiNotification(NOTIF_PENDING_OPERATION));
}

void nuiNavigationController::_PopToRootViewControllerAnimated(bool animated, TransitionType transition)
{
 if (mViewControllers.size() < 1)
  {
    NGL_OUT(_T("nuiNavigationController::PopToRootViewControllerAnimated : nothing to pop"));
    return;
  }

  // don't overlapp animations
  if (mPushed || mPoped || mAlphed)
  {
    mPendingOperations.push_back(PendingOperation(NULL, ePendingPopToRoot, animated, transition, false));
    return;
  }
  
  nuiViewController* pRoot = mViewControllers.front();
  bool done = false;
  std::vector<nuiViewController*>::iterator it;
  while (!done)
  {
    if (mViewControllers.size() < 2)
      break;
    
    it = mViewControllers.end(); --it; --it;
    
    if (*it == pRoot)
    {
      done = true;
      break;
    }
    
    nuiViewController* pView = *it;
    
    // objects have already been detached from the tree (using DelChild, in ::OnViewPushStop (or in ::_PushViewController if no transition)).
    // but their references had not been released to keep the objects alive (in case of specific pop operation, you want those objects come back to the screen)
    // therefore, to delete them here, don't call DelChild, simply call Release for the objects reference to be decreased (and let the nui references system manage the memory).
    pView->GetNavigationBar()->Release();
    pView->Release();
    
    mViewControllers.erase(it);
  }
  
  if (!done)
  {
    NGL_OUT(_T("nuiNavigationController::PopToRootViewControllerAnimated error : could not complete operation\n"));
    return;  
  }
  
  // now, the main pop operation : it will be in charge of deleting the last child before getting to the root
  _PopViewControllerAnimated(animated, transition);
}



void nuiNavigationController::OnViewPushStop(const nuiEvent& rEvent)
{
  mCurrentAnims.clear();
  mPushed = false;
  mPoped = false;
  mAlphed = false;
  bool viewOverlay = (bool)rEvent.mpUser;
  
  if (mViewControllers.size() >1) 
  {
    if (!viewOverlay)
    {
      mpOut = mViewControllers[mViewControllers.size()-2];  
      mpOut->ViewDidDisappear();
      // mpIn->ViewDidAppear is made in nuiViewController::ConnectTopLevel
      if (mpOut->GetParent())
      {
        DelChild(mpOut->GetNavigationBar());
        DelChild(mpOut);
      }
    }

    if (mPendingOperations.size() >0)
      PopPendingOperation();
  }
  
}



void nuiNavigationController::OnViewPopStop(const nuiEvent& rEvent)
{
  mCurrentAnims.clear();
  mPushed = false;
  mPoped = false;
  mAlphed = false;
  bool viewOverlay = (bool)rEvent.mpUser;

  mpOut->ViewDidDisappear();
  // mpIn->ViewDidAppear is made in nuiViewController::ConnectTopLevel

  if (mpOut->GetParent())
  {
    mpOut->GetNavigationBar()->Release();
    DelChild(mpOut->GetNavigationBar());

    mpOut->Release();
    DelChild(mpOut);
    
    mViewControllers.pop_back(); 

    if (mPendingOperations.size() >0)
      PopPendingOperation();
  }
  
}




// virtual
void nuiNavigationController::ConnectTopLevel()
{
  GetTopLevel()->RegisterObserver(NOTIF_PENDING_OPERATION, this); 
}



//virtual 
void nuiNavigationController::OnNotification(const nuiNotification& rNotif)
{
  if (rNotif.GetName() == NOTIF_PENDING_OPERATION)
  {
    if (mPendingOperations.size() > 0)
      PopPendingOperation();
    return;
  }
}






