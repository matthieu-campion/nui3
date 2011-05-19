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

  nuiSignal1<nuiGestureRecognizerState> SignalStateChanged;
  nuiGestureRecognizerState GetState() const;
  
  
protected:
  
  void SetState(nuiGestureRecognizerState state);
  
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

private:
  nuiGestureRecognizerState mState;

};





typedef uint32 nuiGestureDirection;

//
// directions the swipe gesture is applying on. can combine several directions
//
const uint32 nuiGestureDirectionNull  = 1 << 0;

const uint32 nuiGestureDirectionRight = 1 << 1;
const uint32 nuiGestureDirectionLeft  = 1 << 2;
const uint32 nuiGestureDirectionUp    = 1 << 3;
const uint32 nuiGestureDirectionDown  = 1 << 4;

const uint32 nuiGestureDirectionUpRight   = 1 << 5;
const uint32 nuiGestureDirectionDownRight = 1 << 6;
const uint32 nuiGestureDirectionDownLeft  = 1 << 7;
const uint32 nuiGestureDirectionUpLeft    = 1 << 8;

const uint32 nuiGestureDirectionHorizontal = nuiGestureDirectionRight | nuiGestureDirectionLeft;
const uint32 nuiGestureDirectionVertical = nuiGestureDirectionUp | nuiGestureDirectionDown;

const uint32 nuiGestureDirectionDiagonals = nuiGestureDirectionUpRight | nuiGestureDirectionDownRight | nuiGestureDirectionDownLeft | nuiGestureDirectionUpLeft;







//**********************************************************************
//
// class nuiSwipeGestureRecognizer
//
// implements a swipe gesture recognizer
//
class nuiSwipeGestureRecognizer: public nuiGestureRecognizer
{
public:
  
  nuiSwipeGestureRecognizer(nuiGestureDirection direction);
  ~nuiSwipeGestureRecognizer();
  
  void SetDirections(nuiGestureDirection direction);
  
  nuiSignal1<nuiGestureDirection> SignalSwipe;
  nuiGestureDirection GetRecognizedDirection() const;
  
  
protected:
  
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

private:
  
//  nuiPosition GetGesturePosition(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const;
  nuiGestureDirection GetGestureDirection(bool evalOnX, bool evalOnY, nuiSize x1, nuiSize x2, nuiSize y1, nuiSize y2) const;
  
//  static bool DoesPositionMatchesDirection(nuiPosition position, nuiSwipeGestureRecognizerDirection direction);
  
  bool mClicked;
  double mTime;
  double mInitiatedTime;
  nuiSize mStartX;
  nuiSize mStartY;
  
  nuiGestureDirection mDirection;
  nuiGestureDirection mRecognizedDirection;
};

