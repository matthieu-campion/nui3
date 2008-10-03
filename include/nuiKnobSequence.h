/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiFrameSequence.h"



/// Basic slider with range support.
class NUI_API nuiKnobSequence : public nuiSimpleContainer
{
public:
  nuiKnobSequence(const nuiRange& rRange = nuiRange());
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiKnobSequence();
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;

  virtual nuiRange& GetRange(); ///< Return the Range used by this scroll bar. 

  void SetFrameSequence(nuiFrameSequence* pSeq);
  nuiFrameSequence* GetFrameSequence();

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

protected:

//  virtual bool Draw(nuiDrawContext* pContext);

  //Received Events:
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);
  
  bool HandlePageDown(const nuiEvent& rEvent);
  bool HandlePageUp(const nuiEvent& rEvent);
  
  virtual nuiRect CalcIdealSize();
  
  bool DoInvalidate(const nuiEvent& rEvent);
  bool HandleClicked(const nuiEvent& rEvent);
  bool HandleUnclicked(const nuiEvent& rEvent);

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
  
  nuiFrameSequence* mpFrameSequence;

  nuiEventSink<nuiKnobSequence> mKnobSink;
};


