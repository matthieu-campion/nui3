/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"



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
  

const char* nuiGetString(nuiGestureDirection dir)
{
#define F(X) case X: return #X; break;
  switch (dir)
  {
    F(nuiGestureDirectionNull);
    F(nuiGestureDirectionRight);
    F(nuiGestureDirectionLeft);
    F(nuiGestureDirectionUp);
    F(nuiGestureDirectionDown);
    F(nuiGestureDirectionUpRight);
    F(nuiGestureDirectionDownRight);
    F(nuiGestureDirectionDownLeft);
    F(nuiGestureDirectionUpLeft);
  }
#undef F
  return "unknown direction";
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

  Grab();
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
  Ungrab();
  
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
  
	double diffx = mStartX - X;
	double diffy = mStartY - Y;
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



#define PAD_INITIATED_THRESHOLD 9
#define PAD_INITIATED_TIMEOUT 0.10f
#define PAD_ACTIVATED_THRESHOLD 70
#define PAD_ACTIVATED_TIMEOUT 0.15f
#define PAD_FRICTION 0.9f
#define PAD_ATTENUATOR_THRESHOLD 4.0f
#define PAD_THRESHOLD 8.0f


//************************************************************************************************************
//************************************************************************************************************
//
// class nuiPadGestureRecognizer
//


nuiPadGestureRecognizer::nuiPadGestureRecognizer(nuiGestureDirection direction)
: nuiGestureRecognizer(), mSink(this)
{
  mClicked = false;
  mLastX = 0;
  mLastY = 0;
  mFriction = 0.1;
  
  SetDirections(direction);
  mRecognizedDirection = nuiGestureDirectionNull;
  
  nuiTimer* pTimer = nuiAnimation::AcquireTimer();
  mSink.Connect(pTimer->Tick, &nuiPadGestureRecognizer::UpdateForces);
}



nuiPadGestureRecognizer::~nuiPadGestureRecognizer()
{
  
}

void nuiPadGestureRecognizer::SetDirections(nuiGestureDirection direction)
{
  mDirection = direction;

#define F(X) (std::pair<float, float>(X - 22.5, X + 22.5))
  
  mDirections[nuiGestureDirectionRight]     = F(0);
  mDirections[nuiGestureDirectionUpRight]   = F(45);
  mDirections[nuiGestureDirectionUp]        = F(90);
  mDirections[nuiGestureDirectionUpLeft]    = F(135);
  mDirections[nuiGestureDirectionLeft]      = F(180);
  mDirections[nuiGestureDirectionDownLeft]  = F(225);
  mDirections[nuiGestureDirectionDown]      = F(270);
  mDirections[nuiGestureDirectionDownRight] = F(315);
  
#undef X
}

nuiGestureDirection nuiPadGestureRecognizer::GetDirectionFromAngle(float angle) const
{
  std::map<nuiGestureDirection, std::pair<float, float> >::const_iterator it = mDirections.begin();

  if (angle > 360 - 22.5)
    angle = angle - 360;
  
  while (it != mDirections.end())
  {
    if (it->second.first < angle && it->second.second >= angle)
      return it->first;
    
    ++it;
  }
  
  return nuiGestureDirectionNull;
}


// virtual 
bool nuiPadGestureRecognizer::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool res = nuiGestureRecognizer::MouseClicked(X, Y, Button);
  
  mRecognizedDirection = nuiGestureDirectionNull;
  
  mClicked = true;
  mLastX = X;
  mLastY = Y;
  
  Grab();
  return false;
}


// virtual 
bool nuiPadGestureRecognizer::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool res = nuiGestureRecognizer::MouseUnclicked(X, Y, Button);
  
  mClicked = false;
  SetState(eGestureRecognizerStatePossible);
  
  mLastX = 0;
  mLastY = 0;

  Ungrab();
  return false;
}

void nuiPadGestureRecognizer::UpdateForces(const nuiEvent& rEvent)
{
  mForce *= mFriction;
  //printf("UPD norm: %f - angle: %f\n", GetStrength(), GetDegrees());
  
  UpdateDirection();
}

void nuiPadGestureRecognizer::UpdateDirection()
{
  nuiGestureDirection olddir = mRecognizedDirection;
  if (GetStrength() >= PAD_THRESHOLD)
  {
    // See where we're going
    mRecognizedDirection = GetDirectionFromAngle(GetDegrees());
  }
  else
  {
    mRecognizedDirection = nuiGestureDirectionNull;
  }
  
  if (olddir != mRecognizedDirection)
  {
    //printf("New Pad Direction: %s\n", nuiGetString(mRecognizedDirection));
    SignalDirectionChanged(mRecognizedDirection);
    
  }
}

const nuiVector& nuiPadGestureRecognizer::GetForce() const
{
  return mForce;
}

float nuiPadGestureRecognizer::GetStrength() const
{
  return mForce.Length();
}

float nuiPadGestureRecognizer::GetDegrees() const
{
  return 360.0 * GetRadians() / (2 * M_PI);
}

float nuiPadGestureRecognizer::GetRadians() const
{
  nuiVector n = mForce;
  n.Normalize();
  // Get angle:
  double angle = acos(-n[0]);
  if (n[1] < 0)
    angle = 2 * M_PI - angle;
  
  return angle;
}

float nuiPadGestureRecognizer::GetFriction() const
{
  return mFriction;
}

void nuiPadGestureRecognizer::SetFriction(float set)
{
  mFriction = set;
}


// virtual 
bool nuiPadGestureRecognizer::MouseMoved(nuiSize X, nuiSize Y)
{
  bool res = nuiGestureRecognizer::MouseMoved(X, Y);
  
  if (!mClicked)
    return false;
  
	double diffx = mLastX - X;
	double diffy = mLastY - Y;
  double currentTime = nglTime();
  nuiVector v(diffx, diffy, 0);

  double l = v.Length();
  if (l > PAD_ATTENUATOR_THRESHOLD)
    mForce *= 1.0 / l;
  
  mForce += v;

  double length = mForce.Length();

  nuiVector n = mForce;
  //printf("NEW norm: %f - angle: %f\n", GetStrength(), GetDegrees());
 
  mLastX = X;
  mLastY = Y;
  

  UpdateDirection();
  
  return false;
}


nuiGestureDirection nuiPadGestureRecognizer::GetRecognizedDirection() const
{
  return mRecognizedDirection;
}

