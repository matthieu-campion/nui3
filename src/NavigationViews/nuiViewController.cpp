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








#define HORIZ_SWIPE_DRAG_MIN 100


// virtual 
bool nuiViewController::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClicked = true;
  mSwipeStartX = X;
  mSwipeStartY = Y;
  
  mSwipeActivated = false;
  mSwipeDirection = nuiNoPosition;
  
  return false;  
}



// virtual 
bool nuiViewController::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClicked = false;
  mSwipeActivated = false;
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
	
	if(abs(diffx / diffy) > 1 && abs(diffx) > HORIZ_SWIPE_DRAG_MIN)
	{
		mSwipeActivated = true;
    
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
    
    // send event
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

