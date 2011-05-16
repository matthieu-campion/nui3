/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once
#include "nui.h"


class nuiGestureRecognizer;
class nuiSwipeGestureRecognizer;



//**********************************************************************
//
// enum nuiGestureRecognizerState
//
// defines current state of a gesture recognizer
//
enum nuiGestureRecognizerState
{
  eGestureRecognizerStatePossible,   // default state
  eGestureRecognizerStateBegan,      // touches recognized as the gesture.
  eGestureRecognizerStateEnded,      // the end of the gesture. 
  eGestureRecognizerStateFailed      // a touch sequence that can not be recognized as the gesture.

  // maybe later...
  // eGestureRecognizerStateCancelled,  // cancellation of the gesture. 
  //  eGestureRecognizerStateChanged,    // the recognizer has received touches recognized as a change to the gesture. the action method will be called at the next turn of the run loop
  //  eGestureRecognizerStateRecognized = UIGestureRecognizerStateEnded // the recognizer has received touches recognized as the gesture. the action method will be called at the next turn of the run loop and the recognizer will be reset to UIGestureRecognizerStatePossible

};




//**********************************************************************
//
// class nuiGestureRecognizer
//
// mother class to handle a gesture recognizer
//
class nuiGestureRecognizer : public nuiSimpleContainer
{
public:
  nuiGestureRecognizer();
  virtual ~nuiGestureRecognizer();

  nuiSimpleEventSource<0> EventGesture;
  
  nuiSimpleEventSource<0> EventStateChanged;
  nuiGestureRecognizerState GetState() const;

protected:
  
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

  nuiGestureRecognizerState mState;

};




//**********************************************************************
//
// enum nuiSwipeGestureRecognizerDirection
//
// directions the swipe gesture is applying on. can combine several directions
//
typedef enum 
{
  eSwipeGestureRecognizerDirectionRight = 1 << 0,
  eSwipeGestureRecognizerDirectionLeft  = 1 << 1,
  eSwipeGestureRecognizerDirectionUp    = 1 << 2,
  eSwipeGestureRecognizerDirectionDown  = 1 << 3,

  eSwipeGestureRecognizerDirectionUpRight   = eSwipeGestureRecognizerDirectionRight | eSwipeGestureRecognizerDirectionUp,
  eSwipeGestureRecognizerDirectionDownRight = eSwipeGestureRecognizerDirectionRight | eSwipeGestureRecognizerDirectionDown,
  eSwipeGestureRecognizerDirectionDownLeft  = eSwipeGestureRecognizerDirectionLeft | eSwipeGestureRecognizerDirectionDown,
  eSwipeGestureRecognizerDirectionUpLeft    = eSwipeGestureRecognizerDirectionLeft | eSwipeGestureRecognizerDirectionUp


} nuiSwipeGestureRecognizerDirection;





//**********************************************************************
//
// class nuiSwipeGestureRecognizer
//
// implements a swipe gesture recognizer
//
class nuiSwipeGestureRecognizer: public nuiGestureRecognizer
{
public:
  
  nuiSwipeGestureRecognizer();
  ~nuiSwipeGestureRecognizer();
  
  void SetDirection(nuiSwipeGestureRecognizerDirection direction);
  
protected:
  
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

private:
  
//  nuiPosition GetGesturePosition(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const;
  nuiSwipeGestureRecognizerDirection GetGestureDirection(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const;
  
  static bool DoesPositionMatchesDirection(nuiPosition position, nuiSwipeGestureRecognizerDirection direction);
  
  bool mClicked;
  double mTime;
  double mInitiatedTime;
  nuiSize mStartX;
  nuiSize mStartY;
  
  nuiSwipeGestureRecognizerDirection mDirection;
};

