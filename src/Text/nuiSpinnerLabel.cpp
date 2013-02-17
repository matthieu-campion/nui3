/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

/*
#define KNOB_IDEAL_WIDTH 12
#define KNOB_IDEAL_SIZE 100
#define HANDLE_SIZE KNOB_IDEAL_WIDTH
*/


nuiSize nuiSpinnerLabel::mDefaultSensitivity = 10;
nuiSize nuiSpinnerLabel::mDefaultFineSensitivityRatio = 10;
nglKeyCode nuiSpinnerLabel::mDefaultFineSensitivityKey = NK_LSHIFT;



//*********************************************************************
//
// Constr
//
//
nuiSpinnerLabel::nuiSpinnerLabel(const nuiFormater& rFormater, const nuiRange& rRange, nuiTheme::FontStyle FontStyle)
: nuiFormatedLabel (rFormater, rRange.GetValue(), FontStyle), 
mRange(rRange),
mSpinnerLabelSink(this)
{
  InitSpinnerLabel ();
}


//*********************************************************************
//
// Constr
//
//
nuiSpinnerLabel::nuiSpinnerLabel(const nuiFormater& rFormater, const nuiRange& rRange, nuiFont* pFont, bool AlreadyAcquired)
: nuiFormatedLabel (rFormater, rRange.GetValue(), pFont, AlreadyAcquired),
mRange(rRange),
mSpinnerLabelSink(this)
{

  //#FIXME waiting for me to know if there is a default font pointer...
  NGL_ASSERT (pFont != NULL);

  InitSpinnerLabel ();
}



//*********************************************************************
//
// InitSpinnerLabel. Constr
//
//
void nuiSpinnerLabel::InitSpinnerLabel ()
{
  SetObjectClass(_T("nuiSpinnerLabel"));
  mClicked = false;
  mRange.SetPageSize(0);

  mSensitivity = mDefaultSensitivity;
  mFineSensitivityRatio = mDefaultFineSensitivityRatio;
  mFineSensitivityKey = mDefaultFineSensitivityKey;

  mSpinnerLabelSink.Connect(mRange.Changed, &nuiSpinnerLabel::OnValueChanging);
  NUI_ADD_EVENT(ValueChanged);
}



//*********************************************************************
//
// Destr. virtual.
//
//
nuiSpinnerLabel::~nuiSpinnerLabel ()
{

}



//*********************************************************************
//
// MouseClicked
// Received Mouse event
//
//
bool nuiSpinnerLabel::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClickX = X;
  mClickY = Y;
  nglString res;

  //
  // LEFT BUTTON
  //
  if (Button & nglMouseInfo::ButtonLeft)
  {    
    if (Button & nglMouseInfo::ButtonDoubleClick)
    {
      nuiLabelRenamer* renamer = new nuiLabelRenamer (this, false);
      mSpinnerLabelSink.Connect (renamer->Renamed, &nuiSpinnerLabel::Renamed, (void*)renamer);
    }
    else
    {
      Grab();
      mClicked = true;
      mMoved = false;

      mClickValue = mRange.GetValue();
    }

    return true;
  }

  //
  // WHEEL UP
  //
  else if (Button & nglMouseInfo::ButtonWheelUp)
  {

    if (IsKeyDown(mFineSensitivityKey))
    {
      mRange.SetValue(mRange.GetValue() + mRange.GetIncrement() / mFineSensitivityRatio);
    }
    else
    {
      mRange.Increment();
    }

    mFormater.Format (mRange.GetValue(), mValueString);
    SetText (mValueString);

    nuiEvent rEvent;
    mMoved=true;
    DoInvalidate(rEvent);

    ActivateToolTip(this, true);

    return true;
  }

  //
  // WHEEL DOWN
  //

  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    if (IsKeyDown(mFineSensitivityKey))
    {
      mRange.SetValue(mRange.GetValue() - mRange.GetIncrement() / mFineSensitivityRatio);
    }
    else
    {
      mRange.Decrement();
    }

    mFormater.Format (mRange.GetValue(), mValueString);
    SetText (mValueString);

    nuiEvent rEvent;
    mMoved=true;
    DoInvalidate(rEvent);

    ActivateToolTip(this, true);

    return true;
  }
  return false;
}            



//*********************************************************************
//
// Renamed
// labelRenamer event
//
//
void nuiSpinnerLabel::Renamed (const nuiEvent& rEvent)
{
  NGL_ASSERT (rEvent.mpUser != NULL);

  nuiLabelRenamer* renamer = (nuiLabelRenamer*)rEvent.mpUser;
  nglString text = renamer->GetText();

  double value = mFormater.GetCDouble (text);

  mRange.SetValue (value);

  mFormater.Format (mRange.GetValue(), text);
  SetText (text);

  DoInvalidate(rEvent);

  rEvent.Cancel();
}




//*********************************************************************
//
// MouseUnclicked
// Received Mouse event
//
//
bool nuiSpinnerLabel::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mClicked)
  {
    mClicked = false;

    Ungrab();

    nuiEvent rEvent;
    if (mMoved)
      DoInvalidate(rEvent);

    return true;
  }
  return false;
}





void nuiSpinnerLabel::StoreCurrentValue ()
{
  mClickValue = mRange.GetValue ();
}


//*********************************************************************
//
// MouseMoved
// Received Mouse event
//
//
bool nuiSpinnerLabel::MouseMoved  (nuiSize X, nuiSize Y)
{
  if (!mClicked) return false;

  mMoved = true;
  nuiSize x,y;
  x = X-mClickX;
  y = mClickY - Y;

  return UpdateValue (x, y);

}


//*********************************************************************
//
// UpdateValue
//
//
bool nuiSpinnerLabel::UpdateValue(nuiSize movX, nuiSize movY)
{
  nuiSize sensitivity = mSensitivity;
  if (IsKeyDown(mFineSensitivityKey))
  {
    sensitivity *= mFineSensitivityRatio;
  }

  double start = mClickValue;
  double movement = ( movX + movY ) / sensitivity;
  start+= movement;

  return SetValue(start);
}


//*********************************************************************
//
// SetValue
//
//
bool nuiSpinnerLabel::SetValue  (const double& value)
{
  mRange.SetValue(value);
  return true;
}






//*********************************************************************
//
// Increment
//
//
bool nuiSpinnerLabel::Increment()
{
  mRange.Increment();
  return true;
}


//*********************************************************************
//
// Decrement
//
//
bool nuiSpinnerLabel::Decrement()
{
  mRange.Decrement();
  return true;
}




//*********************************************************************
//
// GetRange
//
//
nuiRange& nuiSpinnerLabel::GetRange()
{
  return mRange;
}



//*********************************************************************
//
// DoInvalidate
//
//
void nuiSpinnerLabel::DoInvalidate(const nuiEvent& rEvent)
{
  ValueChanged();
  Invalidate();
}



//*********************************************************************
//
// ValueInChange
//
//
void nuiSpinnerLabel::OnValueChanging(const nuiEvent& rEvent)
{
  ValueChanging();
  mFormater.Format (mRange.GetValue(), mValueString);
  SetText (mValueString);
}




//*********************************************************************
//
// SetDefaultSensitivity
//
//
void nuiSpinnerLabel::SetDefaultSensitivity(nuiSize DefaultSensitivity) 
{ 
  mDefaultSensitivity = DefaultSensitivity;
}




//*********************************************************************
//
// MouseMoved
//
//
void nuiSpinnerLabel::SetSensitivity(nuiSize Sensitivity) 
{ 
  mSensitivity = Sensitivity; 
} 



//*********************************************************************
//
// SetDefaultFineSensitivityRatio
// the bigger the ratio is, the slower the knob will turn.
//
//
void nuiSpinnerLabel::SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio) 
{ 
  mDefaultFineSensitivityRatio = DefaultFineSensitivityRatio;
}




//*********************************************************************
//
// SetFineSensitivityRatio
//
//
void nuiSpinnerLabel::SetFineSensitivityRatio(nuiSize FineSensitivityRatio) 
{ 
  mFineSensitivityRatio = FineSensitivityRatio; 
} 



//*********************************************************************
//
// SetDefaultFineSensitivityKey
//
//
void nuiSpinnerLabel::SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey)
{
  mDefaultFineSensitivityKey = DefaultFineSensitivityKey;
}



//*********************************************************************
//
// SetFineSensitivityKey
//
//
void nuiSpinnerLabel::SetFineSensitivityKey(nglKeyCode FineSensitivityKey)
{
  mFineSensitivityKey = FineSensitivityKey;
}





