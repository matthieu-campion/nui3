/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"


// TODO / Suggestions
// add an autostart configuration option ?
// vertically centered when scrolling an horizontal line ?
// pausing timing especially for vertical scrolling (add setters for this? add in constructor ?)
// fine tune ping pong scroll to avoid scrolling because we're missing 2 pixels (see margins) and on the other hand scroll when there are only 2 pixels left.
// add eScrollLoopRandom, eScrollLoopHRandom, eScrollLoopVRandom

// speed control
// Get Rid of mPPF if we're always setting it at 1 ppf, and increasing speed using the fps value ?


#define DEFAULT_VERTICAL_FPS 20.0
#define DEFAULT_HORIZONTAL_FPS 60.0

enum nuiScrollLoop
{
  eScrollLoopHRightToLeft,
  eScrollLoopHLeftToRight,
  eScrollLoopHPingPong,
  eScrollLoopVBottomToTop,
  eScrollLoopVTopToBottom,
  eScrollLoopVBounceUp,
  eScrollLoopVBounceDown,
};




class nuiScrollingLabel : public nuiSimpleContainer
{
public:
  nuiScrollingLabel(const nglString& rText = nglString::Empty, nuiTheme::FontStyle FontStyle=nuiTheme::Default, nuiScrollLoop ScrollLoop = eScrollLoopHRightToLeft, bool smartScroll = true, bool centered = true);
  nuiScrollingLabel(const nglString& rText, nuiFont* pFont, bool AlreadyAcquired = false, nuiScrollLoop ScrollLoop = eScrollLoopHRightToLeft, bool smartScroll = true, bool centered = true);
  nuiScrollingLabel(nuiLabel* pLabel, nuiScrollLoop ScrollLoop = eScrollLoopHRightToLeft, bool smartScroll = true, bool centered = true);
  virtual ~nuiScrollingLabel();
  
  
  // Scrolling management
  bool SetRect(const nuiRect& rRect);
  void RestartScrolling();
  void StopScrolling();
  void UnpauseScrolling();
  void PauseScrolling();  
  bool IsScrolling() const;
  virtual bool OnTimerChange(const nuiEvent& rEvent);
  
  // nuiLabel management
  nuiLabel* GetLabel();
  void SetLabel(nuiLabel* pLabel);
  void SetText(const nglString& rText);
  void CreateSubStrings(nuiSize width);
  
  // Getters & Setters
  void SetHFPS(double hfps);
  void SetVFPS(double vfps);
  double GetFPS() const;
  double GetHFPS() const;
  double GetVFPS() const;
  void SetPPF(double ppf);
  double GetPPF() const;
  void SetCentered(bool centered);
  bool IsCentered() const;
  void SetScrollLoop(nuiScrollLoop scrollLoop);
  nuiScrollLoop GetScrollLoop();
  void SetSmartScroll(bool smartScroll);
  bool GetSmartScroll() const;
  void SetHPause(int hPause);
  void SetHWait(int hWait);
  void SetVPause(int vPause);
  void SetVWait(int vWait);
  int GetHPause() const;
  int GetHWait() const;
  int GetVPause() const;
  int GetVWait() const;
  
  
private:
  int mTickCount;
  nuiOrientation mOrientation;
  bool mCentered;
  nuiScrollLoop mScrollLoop;
  bool mSmartScroll;
  // bool mAutoStart;
  
  // Speed management : actual scrolling speed is (mFPS * mPPF) pixels per second
  double mFPS; // current frames per second value used to calculate the timer's period and step animation
  void SetFPS(double fps); // private because only used internally
  double mHFPS; // desired fps value for horizontal scrolling
  double mVFPS; // desired fps value for vertical scrolling
  double mPPF; // pixels per frame (default: 1.0)
  
  // Pauses
  int mHPause; // time (in seconds) paused between each ping and pong : default is 2 seconds
  int mHWait; // time (in seconds) waited before another ping pong : default is 2 seconds
  int mVPause; // time (in seconds) paused with the label displayed : default is 2 seconds
  int mVWait; // time (in seconds) waited before scrolling another line : default is 0 seconds
  
  // sub strings management (nuiVertical scrolling)
  bool mNeedSubStrings;
  bool mTextChanged;
  nglString mCompleteString;
  nuiSize mCompleteLabelWidth;
  nuiSize mCompleteLabelHeight;
  std::map<int, nglString> mSubStrings;
  int mNbSubStrings;
  nuiRect mOldRect;
  
  nuiLabel* mpLabel;
    
  nuiTimer* mpTimer;
  nuiEventSink<nuiScrollingLabel> mScrollingLabelSink;
};