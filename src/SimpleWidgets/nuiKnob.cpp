/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiKnob.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiTheme.h"

#define KNOB_IDEAL_WIDTH 12
#define KNOB_IDEAL_SIZE 100
#define HANDLE_SIZE KNOB_IDEAL_WIDTH

nuiSize nuiKnob::mDefaultSensitivity = 10;
nuiSize nuiKnob::mDefaultFineSensitivityRatio = 10;
nglKeyCode nuiKnob::mDefaultFineSensitivityKey = NK_LSHIFT;

nuiKnob::nuiKnob(const nuiRange& rRange)
  : nuiSimpleContainer(),
    mRange(rRange),
    mKnobSink(this)
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

bool nuiKnob::Draw(nuiDrawContext* pContext)
{
  nuiWidget* pItem = NULL;

  long count = mpChildren.size();
  if (!count)
  {
    nuiTheme* pTheme = GetTheme();
    NGL_ASSERT(pTheme);
    pTheme->DrawKnob(pContext, this);
    pTheme->Release();
    return true;
  }
  if (count == 1)
  {
    nuiSize value = mRange.ConvertToUnit(mRange.GetValue());
    count = ToBelow((nuiSize)count * value);

    nuiWidgetList::iterator it = mpChildren.begin();
    pItem = *it;

    if (pItem)
      DrawChild(pContext, pItem);
  }
  else
  {
    count--;

    nuiSize value = mRange.ConvertToUnit(mRange.GetValue());
    count = ToBelow((nuiSize)count * value);

    nuiWidgetList::iterator it = mpChildren.begin();
    nuiWidgetList::iterator end = mpChildren.end();
    for ( ; count && it != end; count--)
      ++it;

    if (it == end)
      return true;

    pItem = *it;

    if (pItem)
      DrawChild(pContext, pItem);
  }
  return true;
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

bool nuiKnob::DoInvalidate(const nuiEvent& rEvent)
{
  mInteractiveValueChanged = true;
  //ValueChanged();
  InteractiveValueChanged();
  Invalidate();
  return false;
}

nuiRect nuiKnob::CalcIdealSize()
{
  if (mpChildren.empty())
  {
    mIdealRect.Set(0,0, 20,20);
    return mIdealRect;
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

