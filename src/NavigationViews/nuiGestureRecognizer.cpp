/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nuiGestureRecognizer.h"




////**********************************************************************
////
//// enum nuiGestureRecognizerState
////
//// defines current state of a gesture recognizer
////
//typedef enum
//{
//  eGestureRecognizerStatePossible,   // default state
//  eGestureRecognizerStateBegan,      // touches recognized as the gesture.
//  eGestureRecognizerStateEnded,      // the end of the gesture. 
//  eGestureRecognizerStateCancelled,  // cancellation of the gesture. 
//  eGestureRecognizerStateFailed      // a touch sequence that can not be recognized as the gesture.
//  
//  // maybe later...
//  //  eGestureRecognizerStateChanged,    // the recognizer has received touches recognized as a change to the gesture. the action method will be called at the next turn of the run loop
//  //  eGestureRecognizerStateRecognized = UIGestureRecognizerStateEnded // the recognizer has received touches recognized as the gesture. the action method will be called at the next turn of the run loop and the recognizer will be reset to UIGestureRecognizerStatePossible
//  
//} nuiGestureRecognizerState;
//

//nuiSimpleEventSource<0> EventGesture;
//nuiSimpleEventSource<0> EventStateChanged;



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
  






////**********************************************************************
////
//// enum nuiSwipeGestureRecognizerDirection
////
//// directions the swipe gesture is applying on. can combine several directions
////
//typedef enum
//{
//  eSwipeGestureRecognizerDirectionRight = 1 << 0,
//  eSwipeGestureRecognizerDirectionLeft  = 1 << 1,
//  eSwipeGestureRecognizerDirectionUp    = 1 << 2,
//  eSwipeGestureRecognizerDirectionDown  = 1 << 3
//  
//} nuiSwipeGestureRecognizerDirection;
//




#define SWIPE_INITIATED_THRESHOLD 9
#define SWIPE_INITIATED_TIMEOUT 0.10f
#define SWIPE_ACTIVATED_THRESHOLD 70
#define SWIPE_ACTIVATED_TIMEOUT 0.15f


//************************************************************************************************************
//************************************************************************************************************
//
// class nuiSwipeGestureRecognizer
//


nuiSwipeGestureRecognizer::nuiSwipeGestureRecognizer()
: nuiGestureRecognizer()
{
  mClicked = false;
  mStartX = 0;
  mStartY = 0;
  
  mDirection = eSwipeGestureRecognizerDirectionRight;  
}



nuiSwipeGestureRecognizer::~nuiSwipeGestureRecognizer()
{

}
  
void nuiSwipeGestureRecognizer::SetDirection(nuiSwipeGestureRecognizerDirection direction)
{
  mDirection = direction;
}


// virtual 
bool nuiSwipeGestureRecognizer::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool res = nuiGestureRecognizer::MouseClicked(X, Y, Button);
 
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
  mState = eGestureRecognizerStatePossible;

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
  
  if (mState == eGestureRecognizerStateEnded)
    return false;
  
	double diffx = mStartX - X + 0.1; // adding 0.1 to avoid division by zero
	double diffy = mStartY - Y + 0.1; // adding 0.1 to avoid division by zero
  double currentTime = nglTime();
  
  // has the swipe gesture been initiated?
  if (mState != eGestureRecognizerStateBegan)
  {
    bool initiatedOnX = (abs(diffx) >= SWIPE_INITIATED_THRESHOLD);
    bool initiatedOnY = (abs(diffy) >= SWIPE_INITIATED_THRESHOLD);
    
    // yes, it's been initiated.
    if (initiatedOnX || initiatedOnY)
    {
      nuiSwipeGestureRecognizerDirection direction = GetGestureDirection(initiatedOnX, initiatedOnY, mStartX, X, mStartY, Y);

      // is this recognizer in charge of the detected gesture?
      if ((direction & mDirection) 
          && ((currentTime  - mTime) < SWIPE_INITIATED_TIMEOUT))
      {
        // yes. consider that the gesture recognition has began
        mState = eGestureRecognizerStateBegan;
        mInitiatedTime = currentTime;
        
        // send synchronous event
        EventStateChanged();
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
  if (mState != eGestureRecognizerStateBegan)
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
      mState = eGestureRecognizerStateFailed;
      
      return false;
    }
    
    // ok! the swipe gesture has been completed!
		mState = eGestureRecognizerStateEnded;
    
    // let's assume it's no use to compute mDirection here, since it's been done when the swipe gesture has been initiated.
    // we'll see in the future if it's wrong to make that assumption.
    // mDirection = GetGesturePosition(activatedOnX, activatedOnY, mStartX, X, mStartY, Y);
    
    // send synchronous event
    EventStateChanged();
    EventGesture();
	}
  
  return false;
}


//nuiPosition nuiSwipeGestureRecognizer::GetGesturePosition(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const
//{
//  // diagonal gesture?
//  if (evalOnX && evalOnY)
//  {
//    if (x1 < x2) 
//    {
//      if (y1 < y2)
//        return nuiBottomRight;
//      else
//        return nuiTopRight;
//    }
//    else 
//    {
//      if (y1 < y2)
//        return nuiBottomLeft;
//      else
//        return nuiTopLeft;
//    }
//  }
//  // horizontal gesture?
//  else if (evalOnX)
//  {
//    if (x1 < x2) 
//      return nuiRight;
//    else
//      return nuiLeft;
//  }
//  // vertical gesture?
//  else if (evalOnY)
//  {
//    if (y1 < y2) 
//      return nuiBottom;
//    else
//      return nuiTop;
//  }
//  
//  return nuiNoPosition;
//}


nuiSwipeGestureRecognizerDirection nuiSwipeGestureRecognizer::GetGestureDirection(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const
{
  // diagonal gesture?
  if (evalOnX && evalOnY)
  {
    if (x1 < x2) 
    {
      if (y1 < y2)
        return (eSwipeGestureRecognizerDirectionDownRight | eSwipeGestureRecognizerDirectionRight);
      else
        return (eSwipeGestureRecognizerDirectionUp | eSwipeGestureRecognizerDirectionRight);
    }
    else 
    {
      if (y1 < y2)
        return (eSwipeGestureRecognizerDirectionDown | eSwipeGestureRecognizerDirectionLeft);
      else
        return (eSwipeGestureRecognizerDirectionUp | eSwipeGestureRecognizerDirectionLeft);
    }
  }
  // horizontal gesture?
  else if (evalOnX)
  {
    if (x1 < x2) 
      return eSwipeGestureRecognizerDirectionRight;
    else
      return eSwipeGestureRecognizerDirectionLeft;
  }
  // vertical gesture?
  else if (evalOnY)
  {
    if (y1 < y2) 
      return eSwipeGestureRecognizerDirectionDown;
    else
      return eSwipeGestureRecognizerDirectionUp;
  }
  
  return -1;
}



// static 
bool nuiSwipeGestureRecognizer::DoesPositionMatchesDirection(nuiPosition position, nuiSwipeGestureRecognizerDirection direction)
{
  if (
}



  
