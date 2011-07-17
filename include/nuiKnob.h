/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiImageSequence.h"

//
// decoration classes
//
// nuiKnob
//


/// Basic slider with range support.
class NUI_API nuiKnob : public nuiSimpleContainer
{
public:
  nuiKnob(const nuiRange& rRange = nuiRange());
  nuiKnob(const nuiRange& rRange, nuiImageSequence* pImageSequence);
  void Init(const nuiRange& rRange);
  void InitAttributes();

  
  virtual ~nuiKnob();
  
  void SetImageSequence(nuiImageSequence* pImageSequence);
  nuiImageSequence* GetImageSequence() const;
  
  // attributes
  const nglPath& GetSequencePath() const;
  void SetSequencePath(const nglPath& rPath);
  nuiOrientation GetOrientation() const;
  void SetOrientation(nuiOrientation orientation);
  uint32 GetNbFrames() const;
  void SetNbFrames(uint32 nbFrames);

  virtual nuiRange& GetRange(); ///< Return the Range used by this scroll bar. 

  // Sent events:
  nuiSimpleEventSource<nuiValueChanged> ValueChanged; ///< This event is sent whenever the Knob's thumb position is changed (by the user or by the program).
  nuiSimpleEventSource<nuiValueChanged> InteractiveValueChanged; ///< This event is sent whenever the Knob's thumb position is changed and the mouse is unclicked.

  // the bigger the Sensitivity is, the slower the knob will turn.
  static void SetDefaultSensitivity(nuiSize DefaultSensitivity);
  void SetSensitivity(nuiSize Sensitivity);

  // the bigger the ratio is, the slower the knob will turn.
  static void SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio);
  void SetFineSensitivityRatio(nuiSize FineSensitivityRatio);

  static void SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey);
  void SetFineSensitivityKey(nglKeyCode FineSensitivityKey);

  // Keyboard events:
  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
  
protected:

  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);

  //Received Events:
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);
  
  void HandlePageDown(const nuiEvent& rEvent);
  void HandlePageUp(const nuiEvent& rEvent);
  
  virtual nuiRect CalcIdealSize();
  
  void DoInvalidate(const nuiEvent& rEvent);
  void HandleClicked(const nuiEvent& rEvent);
  void HandleUnclicked(const nuiEvent& rEvent);

  nuiSize mClickValue;

  bool mClicked;

  nuiSize mClickX;
  nuiSize mClickY;

  bool mInteractiveValueChanged;
  
  nuiRange mRange;

  static nuiSize mDefaultSensitivity;
  nuiSize mSensitivity;

  static nuiSize mDefaultFineSensitivityRatio;
  nuiSize mFineSensitivityRatio;

  static nglKeyCode mDefaultFineSensitivityKey;
  nglKeyCode mFineSensitivityKey;
  
  nuiImageSequence* mpImageSequence;
  uint32 mFrameIndex;

  nuiEventSink<nuiKnob> mKnobSink;
  
  nglPath mSequencePath;
  uint32 mSequenceNbFrames;
  nuiOrientation mSequenceOrientation;
  bool mSequenceNeedRefresh;
};


