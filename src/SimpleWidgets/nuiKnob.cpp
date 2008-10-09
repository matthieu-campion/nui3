/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiKnob.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiTheme.h"
#include "nuiMetaDecoration.h"

#define KNOB_IDEAL_WIDTH 12
#define KNOB_IDEAL_SIZE 100
#define HANDLE_SIZE KNOB_IDEAL_WIDTH

nuiSize nuiKnob::mDefaultSensitivity = 10;
nuiSize nuiKnob::mDefaultFineSensitivityRatio = 10;
nglKeyCode nuiKnob::mDefaultFineSensitivityKey = NK_LSHIFT;

nuiKnob::nuiKnob(const nuiRange& rRange, bool showDefaultBackground)
  : nuiSimpleContainer(),
    mRange(rRange),
    mKnobSink(this),
    mpFrameSequence(NULL),
    mShowDefaultBackground(showDefaultBackground)
{
  SetObjectClass(_T("nuiKnob"));
    
  mClicked = false;
  mRange.SetPageSize(0);

  mSensitivity = mDefaultSensitivity;
  mFineSensitivityRatio = mDefaultFineSensitivityRatio;
  mFineSensitivityKey = mDefaultFineSensitivityKey;

  mKnobSink.Connect(mRange.Changed, &nuiKnob::DoInvalidate);
  //mKnobSink.Connect(mRange.ValueChanged, &nuiKnob::DoInvalidate);
  NUI_ADD_EVENT(ValueChanged);
  NUI_ADD_EVENT(InteractiveValueChanged);  
}

bool nuiKnob::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);

  SetObjectClass(_T("nuiKnob"));
  mKnobSink.SetTarget(this);
  mClicked = false;
  mInteractiveValueChanged = false;

  mSensitivity = mDefaultSensitivity;
  mFineSensitivityRatio = mDefaultFineSensitivityRatio;
  mFineSensitivityKey = mDefaultFineSensitivityKey;

  // FIXME: interpret other attributes...
  mKnobSink.Connect(mRange.Changed, &nuiKnob::DoInvalidate);
  //mKnobSink.Connect(mRange.ValueChanged, &nuiKnob::DoInvalidate);
  NUI_ADD_EVENT(ValueChanged);
  NUI_ADD_EVENT(InteractiveValueChanged);
  
  return true;
}

nuiXMLNode* nuiKnob::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiWidget::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;

  return pNode;
}


nuiKnob::~nuiKnob()
{
}

void nuiKnob::SetKnobDecoration(nuiFrameSequence* pFrameSeq, nuiDecoration* pBkgDeco, nuiDecorationMode mode)
{
  mpFrameSequence = pFrameSeq;
  
  if (!pBkgDeco)
  {
    nuiWidget::SetDecoration(mpFrameSequence, mode);
    return;
  }
  
  nuiMetaDecoration* pDeco = new nuiMetaDecoration(_T("nuiKnobDecoration"));
  pDeco->AddDecoration(pBkgDeco);
  pDeco->AddDecoration(mpFrameSequence);
  
  nuiWidget::SetDecoration(pDeco, mode);

  // init knob first position regarding current range value
  nuiEvent event;
  DoInvalidate(event);
}

// virtual from nuiWidget
void nuiKnob::SetDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode)
{
  nuiFrameSequence* pFrameSeq = dynamic_cast<nuiFrameSequence*>(pDecoration);
  if (pFrameSeq)
    mpFrameSequence = pFrameSeq;
  
  nuiWidget::SetDecoration(pDecoration, Mode);
  
  // init knob first position regarding current range value
  nuiEvent event;
  DoInvalidate(event);
  
}



nuiFrameSequence* nuiKnob::GetFrameSequence()
{
  return mpFrameSequence;
}





// Received Mouse events:
bool nuiKnob::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mClickX = X;
  mClickY = Y;

  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    Grab();
    Invalidate();
    mClickValue = mRange.GetValue();

    return true;
  }
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
    ActivateToolTip(this, true);
    return true;
  }
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
    ActivateToolTip(this, true);
    return true;
  }
  return false;
}            

bool nuiKnob::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mInteractiveValueChanged)
    ValueChanged();
  mInteractiveValueChanged = false;
  
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = false;

    Ungrab();
   
    Invalidate();
    return true;
  }
  return false;
}

bool nuiKnob::MouseMoved  (nuiSize X, nuiSize Y)
{
  if (mClicked)
  {
    nuiSize sensitivity = mSensitivity;
    if (IsKeyDown(mFineSensitivityKey))
    {
      sensitivity *= mFineSensitivityRatio;
    }

    nuiSize x,y;
    nuiSize range = sqrtf(GetRect().GetWidth() * GetRect().GetHeight());
    x = X-mClickX;
    y = mClickY - Y;

    //nuiSize length = range;
    nuiSize start= mClickValue;
    nuiSize movement = ( x + y ) / sensitivity;

    start += (mRange.GetRange() - mRange.GetPageSize()) * (movement/range);

    mRange.SetValue(start);

    // Rotate the contained widget if there is only one:
    /* unused: nuiWidget* pItem = NULL; */

    long count = mpChildren.size();
    if (!count)
      return true;

    return true;
  }

  return false;
}

// Data management:
bool nuiKnob::HandlePageDown(const nuiEvent& rEvent)
{
  mRange.PageIncrement();
  return false;
}

bool nuiKnob::HandlePageUp(const nuiEvent& rEvent)
{
  mRange.PageDecrement();
  return false;
}

nuiRange& nuiKnob::GetRange()
{
  return mRange;
}

bool nuiKnob::GetShowDefaultBackground() const
{
  return mShowDefaultBackground;
}


bool nuiKnob::DoInvalidate(const nuiEvent& rEvent)
{
  mInteractiveValueChanged = true;
  InteractiveValueChanged();
  
  // update FrameSequence decoration
  if (mpFrameSequence)
  {
    uint32 index = (int)((mpFrameSequence->GetNbFrames()-1) * (mRange.GetValue() - mRange.GetMinimum())) / (mRange.GetMaximum() - mRange.GetMinimum());
    mpFrameSequence->SetFrameIndex(this, index);
  }
  
  Invalidate();
  return false;
}

nuiRect nuiKnob::CalcIdealSize()
{
  if (mpChildren.empty())
  {
    nuiDecoration* pDeco = GetDecoration();
    if (!pDeco)
      return nuiRect(0,0,0,0);
    return pDeco->GetIdealClientRect();
  }
  else
  {
    return nuiSimpleContainer::CalcIdealSize();
  }
}

void nuiKnob::SetDefaultSensitivity(nuiSize DefaultSensitivity) 
{ 
  mDefaultSensitivity = DefaultSensitivity;
}

void nuiKnob::SetSensitivity(nuiSize Sensitivity) 
{ 
  mSensitivity = Sensitivity; 
} 

// the bigger the ratio is, the slower the knob will turn.
void nuiKnob::SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio) 
{ 
  mDefaultFineSensitivityRatio = DefaultFineSensitivityRatio;
}

void nuiKnob::SetFineSensitivityRatio(nuiSize FineSensitivityRatio) 
{ 
  mFineSensitivityRatio = FineSensitivityRatio; 
} 

void nuiKnob::SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey)
{
  mDefaultFineSensitivityKey = DefaultFineSensitivityKey;
}

void nuiKnob::SetFineSensitivityKey(nglKeyCode FineSensitivityKey)
{
  mFineSensitivityKey = FineSensitivityKey;
}

