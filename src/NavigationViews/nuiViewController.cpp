/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiViewController.h"
#include "nuiNavigationBar.h"


nuiViewController::nuiViewController()
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiViewController"));
  mAnimated = false;
  
  mClicked = false;
  mSwipeStartX = 0;
  mSwipeStartY = 0;
  mSwipeInitiated = false;
  mSwipeActivated = false;
  mSwipeDirection = nuiNoPosition;
  
  
  mpBar = new nuiNavigationBar();

}



nuiViewController::~nuiViewController()
{
}

// virtual 
void nuiViewController::ConnectTopLevel()
{
  ViewDidAppear();
}


// virtual 
void nuiViewController::ViewWillAppear()
{
  
}

// virtual 
void nuiViewController::ViewWillDisappear()
{

}


// virtual 
void nuiViewController::ViewDidAppear()
{
  
}


// virtual 
void nuiViewController::ViewDidDisappear()
{
  
}


void nuiViewController::SetTitle(const nglString& rTitle)
{
  mpBar->SetTitle(rTitle);
}

nuiNavigationBar* nuiViewController::GetNavigationBar()
{
  return mpBar;
} 














// virtual 
void nuiViewController::HandleSwipe(nuiPosition swipeDirection)
{
  NGL_OUT(_T("HandleSwipe %ls\n"), nuiGetPosition(swipeDirection).GetChars());
  // to be overloaded
}


nuiPosition nuiViewController::GetSwipeDirection() const
{
  return mSwipeDirection;
}








#define SWIPE_INITIATED_THRESHOLD 10
#define SWIPE_INITIATED_TIMEOUT 10
#define SWIPE_ACTIVATED_THRESHOLD 100
#define SWIPE_ACTIVATED_TIMEOUT 100


// virtual 
bool nuiViewController::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClicked = true;
  mSwipeTime = nglTime();
  mSwipeInitiatedTime = 0;
  mSwipeStartX = X;
  mSwipeStartY = Y;
  
  mSwipeInitiated = false;
  mSwipeActivated = false;
  mSwipeDirection = nuiNoPosition;
  
  return false;  
}



// virtual 
bool nuiViewController::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClicked = false;
  mSwipeInitiated = false;
  mSwipeActivated = false;
  mSwipeTime = nglTime();
  mSwipeInitiatedTime = 0;
  mSwipeStartX = 0;
  mSwipeStartY = 0;
  mSwipeDirection = nuiNoPosition;
  
  return false;
}



// virtual 
bool nuiViewController::MouseMoved(nuiSize X, nuiSize Y)
{
  if (!mClicked)
    return false;
  
  if (mSwipeActivated)
    return false;
  
	double diffx = mSwipeStartX - X + 0.1; // adding 0.1 to avoid division by zero
	double diffy = mSwipeStartY - Y + 0.1; // adding 0.1 to avoid division by zero
  double currentTime = nglTime();

  // has the swipe gesture been initiated?
  if (!mSwipeInitiated)
  {
    //LBDEBUG
    NGL_OUT(_T("\n"));
    NGL_OUT(_T("diffX %.2f   diffY %.2f\n"), diffx, diffy);
    
    //LBDEBUG
//    if(abs(diffx / diffy) > 1 && ((abs(diffx) > SWIPE_INITIATED_THRESHOLD) || (abs(diffy) > SWIPE_INITIATED_THRESHOLD))
    if ((abs(diffx) > SWIPE_INITIATED_THRESHOLD) || (abs(diffy) > SWIPE_INITIATED_THRESHOLD))
    {
      // yes, it's been initiated.
      if ((currentTime  - mSwipeTime) > SWIPE_INITIATED_TIMEOUT)
      {
        mSwipeInitiated = true;
        mSwipeInitiatedTime = currentTime;
      }
      // no, it's not. set data for the next call
      else
      {
        mSwipeStartX = X;
        mSwipeStartY = Y;
        mSwipeTime = currentTime;
      }
    }
  }

  // not yet. it's a simple "move" gesture.
	if (!mSwipeInitiated)
    return false;

  // swipe has been initiated, is the gesture fully completed?
  bool activatedOnX = (abs(diffx) > SWIPE_ACTIVATED_THRESHOLD);
  bool activatedOnY = (abs(diffy) > SWIPE_ACTIVATED_THRESHOLD);
       
	// LBDEBUG 
  // if(abs(diffx / diffy) > 1 && abs(diffx) > SWIPE_ACTIVATED_THRESHOLD)
  if(activatedOnX || activatedOnY)
	{
    // the swipe gesture has been avorted. let's reset the data and start again
    if ((currentTime  - mSwipeInitiatedTime) < SWIPE_INITIATED_TIMEOUT)
    {
      mSwipeTime = nglTime();
      mSwipeInitiatedTime = 0;
      mSwipeStartX = X;
      mSwipeStartY = Y;
      mSwipeInitiated = false;
      mSwipeActivated = false;
      mSwipeDirection = nuiNoPosition;
      
      return false;
    }
    
    // ok! the swipe gesture has been completed!
		mSwipeActivated = true;
    
    // diagonal gesture?
    if (activatedOnX && activatedOnY)
    {
      if (mSwipeStartX < X) 
      {
        if (mSwipeStartY < Y)
          mSwipeDirection = nuiBottomRight;
        else
          mSwipeDirection = nuiTopRight;
      }
      else 
      {
        if (mSwipeStartY < Y)
          mSwipeDirection = nuiBottomLeft;
        else
          mSwipeDirection = nuiTopLeft;
      }
    }
    // horizontal gesture?
    else if (activatedOnX)
    {
      if (mSwipeStartX < X) 
        mSwipeDirection = nuiRight;
      else
        mSwipeDirection = nuiLeft;
    }
    // vertical gesture?
    else if (activatedOnY)
    {
      if (mSwipeStartY < Y) 
        mSwipeDirection = nuiBottom;
      else
        mSwipeDirection = nuiTop;
    }
    
    // send synchronous event
    EventSwipe();
    // call virtual cbk
    HandleSwipe(mSwipeDirection);
	}


  return false;
}



//- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
//{
//	UITouch *touch = [touches anyObject];
//	CGPoint newTouchPosition = [touch locationInView:self.view];
//	if(mystartTouchPosition.x != newTouchPosition.x || mystartTouchPosition.y != newTouchPosition.y) {
//		isProcessingListMove = NO;
//	}
//	mystartTouchPosition = [touch locationInView:self.view];
//	[super touchesBegan:touches withEvent:event];
//}

//-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
//{
//	UITouch *touch = touches.anyObject;
//	CGPoint currentTouchPosition = [touch locationInView:self.view];
//	
//	// If the swipe tracks correctly.
//	double diffx = mystartTouchPosition.x - currentTouchPosition.x + 0.1; // adding 0.1 to avoid division by zero
//	double diffy = mystartTouchPosition.y - currentTouchPosition.y + 0.1; // adding 0.1 to avoid division by zero
//	
//	if(abs(diffx / diffy) > 1 && abs(diffx) > HORIZ_SWIPE_DRAG_MIN)
//	{
//		// It appears to be a swipe.
//		if(isProcessingListMove) {
//			// ignore move, we're currently processing the swipe
//			return;
//		}
//		
//		if (mystartTouchPosition.x < currentTouchPosition.x) {
//			isProcessingListMove = YES;
//			[self moveToPreviousItem];
//			return;
//		}
//		else {
//			isProcessingListMove = YES;
//			[self moveToNextItem];
//			return;
//		}
//	}
//	else if(abs(diffy / diffx) > 1)
//	{
//		isProcessingListMove = YES;
//		[super touchesMoved:touches	withEvent:event];
//	}
//}

