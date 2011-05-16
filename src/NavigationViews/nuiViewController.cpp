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
void nuiViewController::SwipeBegan(nuiPosition swipeDirection)
{
  //NGL_OUT(_T("SwipeBegan %ls\n"), nuiGetPosition(swipeDirection).GetChars());
  // to be overloaded
}


void nuiViewController::SwipeEnd(nuiPosition swipeDirection)
{
  //NGL_OUT(_T("SwipeEnd(nuiPosition swipeD %ls\n"), nuiGetPosition(swipeDirection).GetChars());
  // to be overloaded
}

nuiPosition nuiViewController::GetSwipeDirection() const
{
  return mSwipeDirection;
}








#define SWIPE_INITIATED_THRESHOLD 9
#define SWIPE_INITIATED_TIMEOUT 0.10f
#define SWIPE_ACTIVATED_THRESHOLD 70
#define SWIPE_ACTIVATED_TIMEOUT 0.15f


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
    
    bool initiatedOnX = (abs(diffx) >= SWIPE_INITIATED_THRESHOLD);
    bool initiatedOnY = (abs(diffy) >= SWIPE_INITIATED_THRESHOLD);

    if (initiatedOnX || initiatedOnY)
    {
      NGL_OUT(_T("initiated? %.2f\n"), (currentTime  - mSwipeTime));

      // yes, it's been initiated.
      if ((currentTime  - mSwipeTime) < SWIPE_INITIATED_TIMEOUT)
      {
        NGL_OUT(_T("YES! initiated!\n"));

        mSwipeInitiated = true;
        mSwipeInitiatedTime = currentTime;
        mSwipeDirection = GetGestureDirection(initiatedOnX, initiatedOnY, mSwipeStartX, X, mSwipeStartY, Y);
        
        // send synchronous event
        EventSwipeBegan();
        // call virtual cbk
        SwipeBegan(mSwipeDirection);
        
      }
      // no, it's not. set data for the next call
      else
      {
        NGL_OUT(_T("NO! let's start again\n\n"));

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
  bool activatedOnX = (abs(diffx) >= SWIPE_ACTIVATED_THRESHOLD);
  bool activatedOnY = (abs(diffy) >= SWIPE_ACTIVATED_THRESHOLD);
       
	// LBDEBUG 
  // if(abs(diffx / diffy) > 1 && abs(diffx) > SWIPE_ACTIVATED_THRESHOLD)
  if(activatedOnX || activatedOnY)
	{
    NGL_OUT(_T("activated? %.2f\n"), (currentTime  - mSwipeInitiatedTime));

    // the swipe gesture has been avorted. let's reset the data and start again
    if ((currentTime  - mSwipeInitiatedTime) > SWIPE_ACTIVATED_TIMEOUT)
    {
      NGL_OUT(_T("NO! let's start again\n\n"));
 
      mSwipeTime = nglTime();
      mSwipeInitiatedTime = 0;
      mSwipeStartX = X;
      mSwipeStartY = Y;
      mSwipeInitiated = false;
      mSwipeActivated = false;
      mSwipeDirection = nuiNoPosition;
      
      return false;
    }
    
    NGL_OUT(_T("YES! activated!\n"));

    // ok! the swipe gesture has been completed!
		mSwipeActivated = true;
    
    // let's assume it's no use to compute mSwipeDirection here, since it's been done when the swipe gesture has been initiated.
    // we'll see in the future if it's wrong to make that assumption.
    // mSwipeDirection = GetGestureDirection(activatedOnX, activatedOnY, mSwipeStartX, X, mSwipeStartY, Y);
    
    // send synchronous event
    EventSwipeEnd();
    // call virtual cbk
    SwipeEnd(mSwipeDirection);
	}


  return false;
}





nuiPosition nuiViewController::GetGestureDirection(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const
{
  // diagonal gesture?
  if (evalOnX && evalOnY)
  {
    if (x1 < x2) 
    {
      if (y1 < y2)
        return nuiBottomRight;
      else
        return nuiTopRight;
    }
    else 
    {
      if (y1 < y2)
        return nuiBottomLeft;
      else
        return nuiTopLeft;
    }
  }
  // horizontal gesture?
  else if (evalOnX)
  {
    if (x1 < x2) 
      return nuiRight;
    else
      return nuiLeft;
  }
  // vertical gesture?
  else if (evalOnY)
  {
    if (y1 < y2) 
      return nuiBottom;
    else
      return nuiTop;
  }
  
  return nuiNoPosition;

}
