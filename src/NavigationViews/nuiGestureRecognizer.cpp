/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nuiGestureRecognizer.h"



//*******************************************************************************************************************
//*******************************************************************************************************************
//
// class nuiGestureRecognizer
//


nuiGestureRecognizer::nuiGestureRecognizer()
: nuiSimpleContainer()
{
  mState = eGestureRecognizerStatePossible;
}


nuiGestureRecognizer::~nuiGestureRecognizer()
{

}


nuiGestureRecognizerState nuiGestureRecognizer::GetState() const
{
  return mState;
}

void nuiGestureRecognizer::SetState(nuiGestureRecognizerState state)
{
  mState = state;
  
  // send synchronous signal
  SignalStateChanged(mState);
}

  

// virtual 
bool nuiGestureRecognizer::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return false;
}


// virtual 
bool nuiGestureRecognizer::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return false;
}


// virtual 
bool nuiGestureRecognizer::MouseMoved(nuiSize X, nuiSize Y)
{
  return false;
}
  









#define SWIPE_INITIATED_THRESHOLD 9
#define SWIPE_INITIATED_TIMEOUT 0.10f
#define SWIPE_ACTIVATED_THRESHOLD 70
#define SWIPE_ACTIVATED_TIMEOUT 0.15f


//************************************************************************************************************
//************************************************************************************************************
//
// class nuiSwipeGestureRecognizer
//


nuiSwipeGestureRecognizer::nuiSwipeGestureRecognizer(nuiGestureDirection direction)
: nuiGestureRecognizer()
{
  mClicked = false;
  mStartX = 0;
  mStartY = 0;
  
  mDirection = direction;
  mRecognizedDirection = nuiGestureDirectionNull;
}



nuiSwipeGestureRecognizer::~nuiSwipeGestureRecognizer()
{

}
  
void nuiSwipeGestureRecognizer::SetDirections(nuiGestureDirection direction)
{
  mDirection = direction;
}


// virtual 
bool nuiSwipeGestureRecognizer::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool res = nuiGestureRecognizer::MouseClicked(X, Y, Button);
 
  mRecognizedDirection = nuiGestureDirectionNull;
  
  mClicked = true;
  mTime = nglTime();
  mInitiatedTime = 0;
  mStartX = X;
  mStartY = Y;

  return false;
}


// virtual 
bool nuiSwipeGestureRecognizer::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool res = nuiGestureRecognizer::MouseUnclicked(X, Y, Button);
  
  mClicked = false;
  SetState(eGestureRecognizerStatePossible);

  mTime = nglTime();
  mInitiatedTime = 0;
  mStartX = 0;
  mStartY = 0;
  
  return false;
}


// virtual 
bool nuiSwipeGestureRecognizer::MouseMoved(nuiSize X, nuiSize Y)
{
  bool res = nuiGestureRecognizer::MouseMoved(X, Y);

  if (!mClicked)
    return false;
  
  if (GetState() == eGestureRecognizerStateEnded)
    return false;
  
	double diffx = mStartX - X + 0.1; // adding 0.1 to avoid division by zero
	double diffy = mStartY - Y + 0.1; // adding 0.1 to avoid division by zero
  double currentTime = nglTime();
  
  // has the swipe gesture been initiated?
  if (GetState() != eGestureRecognizerStateBegan)
  {
    bool initiatedOnX = (abs(diffx) >= SWIPE_INITIATED_THRESHOLD);
    bool initiatedOnY = (abs(diffy) >= SWIPE_INITIATED_THRESHOLD);
    
    // yes, it's been initiated.
    if (initiatedOnX || initiatedOnY)
    {
      mRecognizedDirection = GetGestureDirection(initiatedOnX, initiatedOnY, mStartX, X, mStartY, Y);

      // is this recognizer in charge of the detected gesture?
      if ((mRecognizedDirection & mDirection) 
          && ((currentTime  - mTime) < SWIPE_INITIATED_TIMEOUT))
      {
        // yes. consider that the gesture recognition has began
        mInitiatedTime = currentTime;
        SetState(eGestureRecognizerStateBegan);
      }
      // no, it's not. set data for the next call
      else
      {
        mStartX = X;
        mStartY = Y;
        mTime = currentTime;
      }
    }
  }
  
  // not yet. it's a simple "move" gesture.
  if (GetState() != eGestureRecognizerStateBegan)
    return false;
  
  // swipe has been initiated, is the gesture fully completed?
  bool activatedOnX = (abs(diffx) >= SWIPE_ACTIVATED_THRESHOLD);
  bool activatedOnY = (abs(diffy) >= SWIPE_ACTIVATED_THRESHOLD);
  
  if(activatedOnX || activatedOnY)
	{
    // the swipe gesture has been avorted. let's reset the data and start again
    if ((currentTime  - mInitiatedTime) > SWIPE_ACTIVATED_TIMEOUT)
    {
      mTime = nglTime();
      mInitiatedTime = 0;
      mStartX = X;
      mStartY = Y;
      SetState(eGestureRecognizerStateFailed);
      mRecognizedDirection = nuiGestureDirectionNull;
      
      return false;
    }
    
    // ok! the swipe gesture has been completed!
		SetState(eGestureRecognizerStateEnded);
    
    // let's assume it's no use to compute mDirection here, since it's been done when the swipe gesture has been initiated.
    // we'll see in the future if it's wrong to make that assumption.
    // mDirection = GetGesturePosition(activatedOnX, activatedOnY, mStartX, X, mStartY, Y);
    
    // send synchronous event
    SignalSwipe(mRecognizedDirection);
	}
  
  return false;
}


nuiGestureDirection nuiSwipeGestureRecognizer::GetRecognizedDirection() const
{
  return mRecognizedDirection;
}


nuiGestureDirection nuiSwipeGestureRecognizer::GetGestureDirection(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const
{
  // diagonal gesture?
  if (evalOnX && evalOnY)
  {
    if (x1 < x2) 
    {
      if (y1 < y2)
        return nuiGestureDirectionDownRight;
      else
        return nuiGestureDirectionUpRight;
    }
    else 
    {
      if (y1 < y2)
        return nuiGestureDirectionDownLeft;
      else
        return nuiGestureDirectionUpLeft;
    }
  }
  // horizontal gesture?
  else if (evalOnX)
  {
    if (x1 < x2) 
      return nuiGestureDirectionRight;
    else
      return nuiGestureDirectionLeft;
  }
  // vertical gesture?
  else if (evalOnY)
  {
    if (y1 < y2) 
      return nuiGestureDirectionDown;
    else
      return nuiGestureDirectionUp;
  }
  
  return nuiGestureDirectionNull;
}





  
