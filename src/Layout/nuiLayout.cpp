/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

// nuiLayoutConstraint
nuiLayoutConstraint::nuiLayoutConstraint()
: mType(eFreeLayout), mSize(0)
{

}

nuiLayoutConstraint::~nuiLayoutConstraint()
{

}

nuiLayoutConstraintType nuiLayoutConstraint::GetType() const
{
  return mType;
}

const nglString& nuiLayoutConstraint::GetAnchor(int i) const
{
  NGL_ASSERT(i >= 0);
  NGL_ASSERT(i < 2);
  return mAnchor[i];
}

float nuiLayoutConstraint::GetSize() const
{
  return mSize;
}

void nuiLayoutConstraint::SetStartAndStop(const nglString& rStart, const nglString& rStop)
{
  mType = eSetStartAndStop;
  mAnchor[0] = rStart;
  mAnchor[1] = rStop;
  mSize = 0;
}

void nuiLayoutConstraint::SetStart(const nglString& rStart)
{
  mType = eSetStart;
  mAnchor[0] = rStart;
  mAnchor[1].Nullify();
  mSize = 0;
}

void nuiLayoutConstraint::SetStartAndSize(const nglString& rStart, float size)
{
  mType = eSetStartAndSize;
  mAnchor[0] = rStart;
  mAnchor[1].Nullify();
  mSize = size;
}

void nuiLayoutConstraint::SetStop(const nglString& rStop)
{
  mType = eSetStop;
  mAnchor[0] = rStop;
  mAnchor[1].Nullify();
  mSize = 0;
}

void nuiLayoutConstraint::SetStopAndSize(const nglString& rStop, float size)
{
  mType = eSetStopAndSize;
  mAnchor[0] = rStop;
  mAnchor[1].Nullify();
  mSize = size;
}

void nuiLayoutConstraint::SetMiddle(const nglString& rMiddle)
{
  mType = eSetMiddle;
  mAnchor[0] = rMiddle;
  mAnchor[1].Nullify();
  mSize = 0;
}

void nuiLayoutConstraint::SetMiddleAndSize(const nglString& rMiddle, float size)
{
  mType = eSetMiddleAndSize;
  mAnchor[0] = rMiddle;
  mAnchor[1].Nullify();
  mSize = size;
}

void nuiLayoutConstraint::SetSize(float size)
{
  mType = eSetSize;
  mAnchor[0].Nullify();
  mAnchor[1].Nullify();
  mSize = size;
}

void nuiLayoutConstraint::SetFree()
{
  mType = eFreeLayout;
  mAnchor[0].Nullify();
  mAnchor[1].Nullify();
  mSize = 0;
}

bool nuiLayoutConstraint::Set(const nglString& rDescription)
{
  int index = 0;
  nglUChar c = rDescription.GetNextUChar(index);

  if (c == '[')
  {
    // Fix start + ?
    int pos = index;
    int end = index;
    c = rDescription.GetNextUChar(index);
    while (c != ',' && c != '}')
    {
      end = index;
      c = rDescription.GetNextUChar(index);
    }

    nglString anchor1 = rDescription.Extract(pos, index - pos);

    if (c == ',')
    {
      // Fix stop or fix size
      int pos = index;
      int end = index;
      c = rDescription.GetNextUChar(index);
      while (c != ']' && c != '}')
      {
        end = index;
        c = rDescription.GetNextUChar(index);
      }

      nglString anchor2 = rDescription.Extract(pos, index - pos);
      if (c == ']')
      {
        // Start and Stop:
        SetStartAndStop(anchor1, anchor2);
        return true;
      }
      else if (c == '}')
      {
        // Start and size:
        SetStartAndSize(anchor1, anchor2.GetCFloat());
        return true;
      }

    }
    else if (c == '}')
    {
      // Fix start
      SetStart(anchor1);
      return true;
    }
    
  }
  else if (c == '{')
  {
    // Fix end + ?
    int pos = index;
    int end = index;
    c = rDescription.GetNextUChar(index);
    while (c != ',' && c != '}' && c != ']')
    {
      end = index;
      c = rDescription.GetNextUChar(index);
    }

    nglString anchor1 = rDescription.Extract(pos, index - pos);

    if (c == ',')
    {
      // Fix stop or fix size
      int pos = index;
      int end = index;
      c = rDescription.GetNextUChar(index);

      while (c != ']' && c != '}')
      {
        end = index;
        c = rDescription.GetNextUChar(index);
      }

      nglString anchor2 = rDescription.Extract(pos, index - pos);
      if (c == ']')
      {
        // size and Stop:
        SetStopAndSize(anchor2, anchor1.GetCFloat());
        return true;
      }
      else if (c == '}')
      {
        // Midle and size:
        SetMiddleAndSize(anchor1, anchor2.GetCFloat());
        return true;
      }

    }
    else if (c == '}')
    {
      // Fix middle
      SetMiddle(anchor1);
      return true;
    }
    else if (c == ']')
    {
      // Fix stop
      SetStop(anchor1);
      return true;
    }
  }
  else
  {
    SetSize(rDescription.GetCFloat());
    return true;
  }

  return false;
}


//////////////
// nuiLayout:
nuiLayout::nuiLayout()
{
  if (SetObjectClass("nuiLayout"))
  {
    //
  }
}

nuiLayout::~nuiLayout()
{
}

void nuiLayout::SetHorizontalAnchor(const nglString& rName, float position, nuiAnchorType mode)
{
  mAnchors[0][rName] = std::make_pair(position, mode);
}


void nuiLayout::SetVerticalAnchor(const nglString& rName, float position, nuiAnchorType mode)
{
  mAnchors[1][rName] = std::make_pair(position, mode);
}

void nuiLayout::SetVerticalAnchorPosition(const nglString& rName, float position)
{
  auto it = mAnchors[1].find(rName);
  if (it != mAnchors[1].end())
    it->second.first = position;
  else
    mAnchors[1][rName] = std::make_pair(position, eAnchorAbsolute);
}

void nuiLayout::SetHorizontalAnchorPosition(const nglString& rName, float position)
{
  auto it = mAnchors[0].find(rName);
  if (it != mAnchors[0].end())
    it->second.first = position;
  else
    mAnchors[0][rName] = std::make_pair(position, eAnchorAbsolute);
}

void nuiLayout::SetVerticalAnchorMode(const nglString& rName, nuiAnchorType mode)
{
  auto it = mAnchors[1].find(rName);
  if (it != mAnchors[1].end())
    it->second.second = mode;
  else
    mAnchors[1][rName] = std::make_pair(0, mode);
}

void nuiLayout::SetHorizontalAnchorMode(const nglString& rName, nuiAnchorType mode)
{
  auto it = mAnchors[0].find(rName);
  if (it != mAnchors[0].end())
    it->second.second = mode;
  else
    mAnchors[0][rName] = std::make_pair(0, mode);
}



float nuiLayout::GetHorizontalAnchorPosition(const nglString& rName) const
{
  auto it = mAnchors[0].find(rName);
  if (it == mAnchors[0].end())
    return 0;

  return it->second.first;
}

float nuiLayout::GetVerticalAnchorPosition(const nglString& rName) const
{
  auto it = mAnchors[1].find(rName);
  if (it == mAnchors[1].end())
    return 0;

  return it->second.first;
}

nuiAnchorType nuiLayout::GetHorizontalAnchorMode(const nglString& rName) const
{
  auto it = mAnchors[0].find(rName);
  if (it == mAnchors[0].end())
    return eAnchorAbsolute;

  return it->second.second;
}

nuiAnchorType nuiLayout::GetVerticalAnchorMode(const nglString& rName) const
{
  auto it = mAnchors[1].find(rName);
  if (it == mAnchors[1].end())
    return eAnchorAbsolute;

  return it->second.second;
}


void nuiLayout::SetConstraint(nuiWidget* pWidget, const nglString& rDescription)
{
  nuiLayoutConstraint constraintH, constraintV;
  int pos = rDescription.Find('/');
  if (pos < 0)
  {
    constraintH.Set(rDescription);
  }
  else if (pos == 0)
  {
    nglString desc2 = rDescription.Extract(pos+1, rDescription.GetLength() - (pos + 1));
    constraintV.Set(desc2);
  }
  else
  {
    nglString desc1 = rDescription.Extract(0, pos);
    nglString desc2 = rDescription.Extract(pos+1, rDescription.GetLength() - (pos + 1));
    constraintH.Set(desc1);
    constraintV.Set(desc2);
  }

  SetConstraint(pWidget, constraintH, constraintV);
}

void nuiLayout::SetConstraint(nuiWidget* pWidget, const nuiLayoutConstraint& rHorizontal, const nuiLayoutConstraint& rVertical)
{
  mConstraints[pWidget] = std::make_pair(rHorizontal, rVertical);
}

void nuiLayout::GetConstraints(nuiWidget* pWidget, nuiLayoutConstraint& rHorizontal, nuiLayoutConstraint& rVertical) const
{
  auto it = mConstraints.find(pWidget);
  if (it != mConstraints.end())
  {
    rHorizontal = it->second.first;
    rVertical = it->second.second;
  }
  else
  {
    rHorizontal = rVertical = nuiLayoutConstraint();
  }
}

void nuiLayout::GetConstraintString(nuiWidget* pWidget, nglString& rString) const
{

}


class LayoutAnchorValue : public nuiAttribute<float>
{
public:
  LayoutAnchorValue(const nglString& rName, nuiLayout* pLayout)
  : nuiAttribute<float>(rName, nuiUnitNone, nuiMakeDelegate(this, &LayoutAnchorValue::_Get), nuiMakeDelegate(this, &LayoutAnchorValue::_Set), NUI_INVALID_RANGE)
  {
    nuiAttributeBase::SetAsInstanceAttribute(true);

    if (rName.CompareLeft("HAnchors.", true) == true)
    {
      mGetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::GetHorizontalAnchorPosition);
      mSetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::SetHorizontalAnchorPosition);
    }
    else if (rName.CompareLeft("VAnchors.", true) == true)
    {
      mGetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::GetVerticalAnchorPosition);
      mSetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::SetVerticalAnchorPosition);
    }
  }

protected:
  float _Get() const
  {
    return mGetAnchorDelegate(mAnchor);
  }

  void _Set(float value)
  {
    mSetAnchorDelegate(mAnchor, value);
  }

  nuiFastDelegate1<const nglString&, float> mGetAnchorDelegate;
  nuiFastDelegate2<const nglString&, float> mSetAnchorDelegate;
  nglString mAnchor;
};


void nuiLayout::SetProperty(const nglString& rName, const nglString& rValue)
{
  nuiAttribBase attr(GetAttribute(rName));
  if (attr.IsValid())
  {
    attr.FromString(rValue);
    return;
  }

  if (rName.CompareLeft("VAnchors.", true) == true || rName.CompareLeft("HAnchors.", true) == true)
  {
    // Create an attribute for this anchor, unless it exists already
    AddAttribute(new LayoutAnchorValue(rName, this));
  }
}


void nuiLayout::DoLayout(const nuiRect& rRect)
{
  
}

bool nuiLayout::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  //NGL_OUT("nuiLayout::SetRect(%s)\n", rRect.GetValue().GetChars());
  
  nuiRect r(rRect.Size());
  DoLayout(r);
  
  return true;
}


