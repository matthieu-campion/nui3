/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiKnobSequence.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiTheme.h"

#define KNOB_IDEAL_WIDTH 12
#define KNOB_IDEAL_SIZE 100
#define HANDLE_SIZE KNOB_IDEAL_WIDTH

nuiSize nuiKnobSequence::mDefaultSensitivity = 10;
nuiSize nuiKnobSequence::mDefaultFineSensitivityRatio = 10;
nglKeyCode nuiKnobSequence::mDefaultFineSensitivityKey = NK_LSHIFT;

nuiKnobSequence::nuiKnobSequence(const nuiRange& rRange)
  : nuiSimpleContainer(),
    mRange(rRange),
    mKnobSink(this),
  mpFrameSequence(NULL)
{
  SetObjectClass(_T("nuiKnobSequence"));
  
  mClicked = false;
  mRange.SetPageSize(0);

  mSensitivity = mDefaultSensitivity;
  mFineSensitivityRatio = mDefaultFineSensitivityRatio;
  mFineSensitivityKey = mDefaultFineSensitivityKey;

  mKnobSink.Connect(mRange.Changed, &nuiKnobSequence::DoInvalidate);
  //mKnobSink.Connect(mRange.ValueChanged, &nuiKnobSequence::DoInvalidate);
  NUI_ADD_EVENT(ValueChanged);
  NUI_ADD_EVENT(InteractiveValueChanged);
}

bool nuiKnobSequence::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);

  SetObjectClass(_T("nuiKnobSequence"));
  mKnobSink.SetTarget(this);
  mClicked = false;
  mInteractiveValueChanged = false;

  mSensitivity = mDefaultSensitivity;
  mFineSensitivityRatio = mDefaultFineSensitivityRatio;
  mFineSensitivityKey = mDefaultFineSensitivityKey;

  // FIXME: interpret other attributes...
  mKnobSink.Connect(mRange.Changed, &nuiKnobSequence::DoInvalidate);
  //mKnobSink.Connect(mRange.ValueChanged, &nuiKnobSequence::DoInvalidate);
  NUI_ADD_EVENT(ValueChanged);
  NUI_ADD_EVENT(InteractiveValueChanged);
  
  return true;
}

nuiXMLNode* nuiKnobSequence::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiWidget::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;

  return pNode;
}


nuiKnobSequence::~nuiKnobSequence()
{
}

void nuiKnobSequence::SetFrameSequence(nuiFrameSequence* pSeq)
{
  mpFrameSequence = pSeq;
  SetDecoration(mpFrameSequence);
}


nuiFrameSequence* nuiKnobSequence::GetFrameSequence()
{
  return mpFrameSequence;
}


//bool nuiKnobSequence::Draw(nuiDrawContext* pContext)
//{
//  nuiWidget* pItem = NULL;
//
//  long count = mpChildren.size();
//  if (!count)
//  {
//    nuiTheme* pTheme = GetTheme();
//    NGL_ASSERT(pTheme);
//
//    //    pTheme->DrawKnob(pContext, this);
//    pTheme->Release();
//    return true;
//  }
//  if (count == 1)
//  {
//    nuiSize value = mRange.ConvertToUnit(mRange.GetValue());
//    count = ToBelow((nuiSize)count * value);
//
//    nuiWidgetList::iterator it = mpChildren.begin();
//    pItem = *it;
//
//    if (pItem)
//      DrawChild(pContext, pItem);
//  }
//  else
//  {
//    count--;
//
//    nuiSize value = mRange.ConvertToUnit(mRange.GetValue());
//    count = ToBelow((nuiSize)count * value);
//
//    nuiWidgetList::iterator it = mpChildren.begin();
//    nuiWidgetList::iterator end = mpChildren.end();
//    for ( ; count && it != end; count--)
//      ++it;
//
//    if (it == end)
//      return true;
//
//    pItem = *it;
//
//    if (pItem)
//      DrawChild(pContext, pItem);
//  }
//  return true;
//}

// Received Mouse events:
bool nuiKnobSequence::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
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

bool nuiKnobSequence::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
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

bool nuiKnobSequence::MouseMoved  (nuiSize X, nuiSize Y)
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
    /*
    if (count == 1)
    {
      nuiSize value = mRange.ConvertToUnit(mRange.GetValue());
      count = ToBelow((nuiSize)count * value);

      nuiWidgetList::iterator it = mpChildren.begin();
      pItem = *it;

      if (pItem)
      {
        nuiMatrix mat1,mat2,mat3, mat;
        nuiRect rect = pItem->GetRect();
        mat1.Translate(rect.GetWidth()/2.0f,rect.GetHeight()/2.0f, 0);
        mat2.Rotate(300.0f * value, 0,0,1);
        mat3.Translate(-rect.GetWidth()/2.0f,-rect.GetHeight()/2.0f, 0);

        mat *= mat1;
        mat *= mat2;
        mat *= mat3;
        pItem->SetMatrix(mat); // this will automatically call Invalidate() on the child item.
      }
    }
    */
    return true;
  }

  return false;
}

// Data management:
bool nuiKnobSequence::HandlePageDown(const nuiEvent& rEvent)
{
  mRange.PageIncrement();
  return false;
}

bool nuiKnobSequence::HandlePageUp(const nuiEvent& rEvent)
{
  mRange.PageDecrement();
  return false;
}

nuiRange& nuiKnobSequence::GetRange()
{
  return mRange;
}

bool nuiKnobSequence::DoInvalidate(const nuiEvent& rEvent)
{
  mInteractiveValueChanged = true;
  InteractiveValueChanged();
  
  // update FrameSequence decoration
  if (mpFrameSequence)
  {
    uint32 index = (int)((mpFrameSequence->GetNbFrames()-1) * (mRange.GetValue() - mRange.GetMinimum())) / (mRange.GetMaximum() - mRange.GetMinimum());
    
    mpFrameSequence->SetFrameIndex(index);
  }
  
  Invalidate();
  return false;
}

nuiRect nuiKnobSequence::CalcIdealSize()
{
  if (mpChildren.empty())
  {
    mIdealRect.Set(0,0, 32,32);
    return mIdealRect;
  }
  else
  {
    return nuiSimpleContainer::CalcIdealSize();
  }
}

void nuiKnobSequence::SetDefaultSensitivity(nuiSize DefaultSensitivity) 
{ 
  mDefaultSensitivity = DefaultSensitivity;
}

void nuiKnobSequence::SetSensitivity(nuiSize Sensitivity) 
{ 
  mSensitivity = Sensitivity; 
} 

// the bigger the ratio is, the slower the knob will turn.
void nuiKnobSequence::SetDefaultFineSensitivityRatio(nuiSize DefaultFineSensitivityRatio) 
{ 
  mDefaultFineSensitivityRatio = DefaultFineSensitivityRatio;
}

void nuiKnobSequence::SetFineSensitivityRatio(nuiSize FineSensitivityRatio) 
{ 
  mFineSensitivityRatio = FineSensitivityRatio; 
} 

void nuiKnobSequence::SetDefaultFineSensitivityKey(nglKeyCode DefaultFineSensitivityKey)
{
  mDefaultFineSensitivityKey = DefaultFineSensitivityKey;
}

void nuiKnobSequence::SetFineSensitivityKey(nglKeyCode FineSensitivityKey)
{
  mFineSensitivityKey = FineSensitivityKey;
}

