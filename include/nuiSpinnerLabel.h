/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSpinnerLabel_h__
#define __nuiSpinnerLabel_h__

//#include "nui.h"
#include "nuiFormatedLabel.h"
#include "nuiLabelRenamer.h"






/// This widget implements a text label with mouse controls. It can be used as a basic building block for buttons, list headers, list elements, etc...
class NUI_API nuiSpinnerLabel : public nuiFormatedLabel
{
public:

  nuiSpinnerLabel(const nuiFormater& rFormater = nuiFormatedLabel::DefaultFormater, const nuiRange& rRange = nuiRange(), nuiTheme::FontStyle FontStyle=nuiTheme::Default);
  nuiSpinnerLabel(const nuiFormater& rFormater, const nuiRange& rRange, nuiFont* pFont, bool AlreadyAcquired = false);
  virtual ~nuiSpinnerLabel();
  

  virtual nuiRange& GetRange(); ///< Return the Range used by this scroll bar. 
  
  void StoreCurrentValue ();

  //Received Events:
  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);
  
  bool UpdateValue  (nuiSize movX, nuiSize movY); ///< update the value if function of the mouse position. it's called by the MouseMoved event, and also by the MatrixViewer mouse events.
  virtual bool SetValue  (nuiSize value);
  bool Increment ();
  bool Decrement ();


  // Sent events:
  nuiSimpleEventSource<nuiValueChanged> ValueChanged; ///< This event is sent whenever the Knob's thumb position is changed (by the user or by the program).
  nuiSimpleEventSource<nuiValueChanged> ValueChanging; ///< the value is currently changing but is not necessarly fixed (on mouse wheel up for instance...)




  // the bigger the Sensitivity is, the slower the knob will turn.
  static void SetDefaultSensitivity(nuiSize DefaultSensitivity);
  void SetSensitivity(nuiSize Sensitivity);

  // the bigger the ratio is, the slower the knob will turn.
  static void SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio);
  void SetFineSensitivityRatio(nuiSize FineSensitivityRatio);

  static void SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey);
  void SetFineSensitivityKey(nglKeyCode FineSensitivityKey);


  virtual bool Renamed (const nuiEvent& event);

protected:

  void InitSpinnerLabel ();
  



  bool DoInvalidate(const nuiEvent& rEvent);
  bool OnValueChanging(const nuiEvent& rEvent);
  bool HandleClicked(const nuiEvent& rEvent);
  bool HandleUnclicked(const nuiEvent& rEvent);

  nuiSize mClickValue;
  nglString mValueString;

  bool mClicked;
	bool mMoved;

  nuiSize mClickX;
  nuiSize mClickY;

  nuiRange mRange;

  static nuiSize mDefaultSensitivity;
  nuiSize mSensitivity;

  static nuiSize mDefaultFineSensitivityRatio;
  nuiSize mFineSensitivityRatio;

  static nglKeyCode mDefaultFineSensitivityKey;
  nglKeyCode mFineSensitivityKey;


  nuiEventSink<nuiSpinnerLabel> mSpinnerLabelSink;

private : 
/*

  class nuiSpinnerLabelRenamer : public nuiLabelRenamer
  {
  public : 
    nuiSpinnerLabelRenamer (nuiSpinnerLabel* pSLabel);
    ~nuiSpinnerLabelRenamer ();
    
    virtual bool OnEditLineSelected(const nuiEvent& rEvent);

  };
*/

};

#endif // __nuiSpinnerLabel_h__
