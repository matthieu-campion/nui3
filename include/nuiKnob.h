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
// nuiKnob::Background
//


/// Basic slider with range support.
class NUI_API nuiKnob : public nuiSimpleContainer
{
public:
  nuiKnob(const nuiRange& rRange = nuiRange(), bool showDefaultBackground = true);
  nuiKnob(const nuiRange& rRange, nuiImageSequence* pImageSequence, bool showDefaultBackground);
  void Init(const nuiRange& rRange, bool showDefaultBackground);
  void InitAttributes();

  
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiKnob();
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;
  
  void SetImageSequence(nuiImageSequence* pImageSequence);
  nuiImageSequence* GetImageSequence();
  bool GetShowDefaultBackground();
  
  // attributes
  const nglPath& GetSequencePath();
  void SetSequencePath(const nglPath& rPath);
  nglString GetOrientation();
  void SetOrientation(nglString orientation);
  uint32 GetNbFrames();
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

protected:

  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);

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
  
  nuiImageSequence* mpImageSequence;
  uint32 mFrameIndex;

  nuiEventSink<nuiKnob> mKnobSink;
  
  bool mShowDefaultBackground;
  
  nglPath mSequencePath;
  uint32 mSequenceNbFrames;
  nglString mSequenceOrientation;
  bool mSequenceNeedRefresh;
};


