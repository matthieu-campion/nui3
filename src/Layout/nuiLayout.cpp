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

void nuiLayoutConstraint::SetMiddle(const nglString& rStart, const nglString& rStop)
{
  mType = eSetMiddle;
  mAnchor[0] = rStart;
  mAnchor[1] = rStop;
  mSize = 0;
}

void nuiLayoutConstraint::SetMiddleAndSize(const nglString& rMiddle, float size)
{
  mType = eSetMiddleAndSize;
  mAnchor[0] = rMiddle;
  mAnchor[1].Nullify();
  mSize = size;
}

void nuiLayoutConstraint::SetMiddleAndSize(const nglString& rStart, const nglString& rStop, float size)
{
  mType = eSetMiddleAndSize;
  mAnchor[0] = rStart;
  mAnchor[1] = rStop;
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

    nglString anchor1 = rDescription.Extract(pos, index - pos - 1);
    anchor1.Trim();

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

      nglString anchor2 = rDescription.Extract(pos, index - pos - 1);
      anchor2.Trim();

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

    nglString anchor1 = rDescription.Extract(pos, index - pos - 1);
    anchor1.Trim();

    if (c == ',')
    {
      // Fix stop or fix size
      int pos = index;
      int end = index;
      c = rDescription.GetNextUChar(index);

      while (c != ']' && c != '}' && c != ',')
      {
        end = index;
        c = rDescription.GetNextUChar(index);
      }

      nglString anchor2 = rDescription.Extract(pos, index - pos - 1);
      anchor2.Trim();

      if (c == ']')
      {
        // size and Stop:
        SetStopAndSize(anchor2, anchor1.GetCFloat());
        return true;
      }
      else if (c == '}')
      {
        // Midle and size:
        if (anchor2.IsFloat())
          SetMiddleAndSize(anchor1, anchor2.GetCFloat());
        else
          SetMiddle(anchor1, anchor2);
        return true;
      }
      else if (c == ',')
      {
        nglString anchor2 = rDescription.Extract(pos, index - pos - 1);
        anchor2.Trim();

        // Fix stop or fix size
        int pos = index;
        int end = index;
        c = rDescription.GetNextUChar(index);

        while (c != '}')
        {
          end = index;
          c = rDescription.GetNextUChar(index);
        }

        nglString anchor3 = rDescription.Extract(pos, index - pos - 1);
        anchor3.Trim();

        SetMiddleAndSize(anchor1, anchor2, anchor3.GetCFloat());
        return true;
      }

    }
    else if (c == '}')
    {
      // Fix middle
      if (anchor1.IsFloat())
        SetMiddleAndSize(nglString::Null, anchor1.GetCFloat());
      else
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

void nuiLayout::SetHorizontalAnchor(const nglString& rName, float position, nuiAnchorType Type)
{
  mAnchors[0][rName] = std::make_pair(position, Type);
}


void nuiLayout::SetVerticalAnchor(const nglString& rName, float position, nuiAnchorType Type)
{
  mAnchors[1][rName] = std::make_pair(position, Type);
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

void nuiLayout::SetVerticalAnchorType(const nglString& rName, nuiAnchorType Type)
{
  auto it = mAnchors[1].find(rName);
  if (it != mAnchors[1].end())
    it->second.second = Type;
  else
    mAnchors[1][rName] = std::make_pair(0, Type);
}

void nuiLayout::SetHorizontalAnchorType(const nglString& rName, nuiAnchorType Type)
{
  auto it = mAnchors[0].find(rName);
  if (it != mAnchors[0].end())
    it->second.second = Type;
  else
    mAnchors[0][rName] = std::make_pair(0, Type);
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

nuiAnchorType nuiLayout::GetHorizontalAnchorType(const nglString& rName) const
{
  auto it = mAnchors[0].find(rName);
  if (it == mAnchors[0].end())
    return eAnchorAbsolute;

  return it->second.second;
}

nuiAnchorType nuiLayout::GetVerticalAnchorType(const nglString& rName) const
{
  auto it = mAnchors[1].find(rName);
  if (it == mAnchors[1].end())
    return eAnchorAbsolute;

  return it->second.second;
}

bool nuiLayout::AddChild(nuiWidgetPtr pChild)
{
  if (nuiSimpleContainer::AddChild(pChild))
  {
    SetConstraint(pChild, pChild->GetProperty("Layout"));
    return true;
  }

  return false;
}


void nuiLayout::SetConstraint(nuiWidget* pWidget, const nglString& rDescription)
{
  nuiLayoutConstraint constraintH, constraintV;
  int pos = rDescription.Find('/');
  if (pos < 0)
  {
    nglString desc(rDescription);
    desc.Trim();
    constraintH.Set(desc);
  }
  else if (pos == 0)
  {
    nglString desc2 = rDescription.Extract(pos+1, rDescription.GetLength() - (pos + 1));
    desc2.Trim();
    constraintV.Set(desc2);
  }
  else
  {
    nglString desc1 = rDescription.Extract(0, pos);
    nglString desc2 = rDescription.Extract(pos+1, rDescription.GetLength() - (pos + 1));
    desc1.Trim();
    desc2.Trim();
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
  : nuiAttribute<float>(rName, nuiUnitNone, nuiMakeDelegate(this, &LayoutAnchorValue::_Get), nuiMakeDelegate(this, &LayoutAnchorValue::_Set), NUI_INVALID_RANGE),
    mAnchor(rName.Extract(9))
  {
    nuiAttributeBase::SetAsInstanceAttribute(true);

    if (rName.CompareLeft("HAnchors_", true) == 0)
    {
      mGetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::GetHorizontalAnchorPosition);
      mSetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::SetHorizontalAnchorPosition);
    }
    else if (rName.CompareLeft("VAnchors_", true) == 0)
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


template <>
class nuiAttribute<nuiAnchorType> : public nuiAttribute<int32>
{
public:
  nuiAttribute<nuiAnchorType>(const nglString& rName, nuiLayout* pLayout)
  : nuiAttribute<int32>(rName, nuiUnitNone, nuiMakeDelegate(this, &nuiAttribute<nuiAnchorType>::_Get), nuiMakeDelegate(this, &nuiAttribute<nuiAnchorType>::_Set), NUI_INVALID_RANGE),
    mAnchor(rName.Extract(13))
  {
    nuiAttributeBase::SetAsInstanceAttribute(true);

    if (rName.CompareLeft("HAnchorsType_", true) == 0)
    {
      mGetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::GetHorizontalAnchorType);
      mSetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::SetHorizontalAnchorType);
    }
    else if (rName.CompareLeft("VAnchorsType_", true) == 0)
    {
      mGetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::GetVerticalAnchorType);
      mSetAnchorDelegate = nuiMakeDelegate(pLayout, &nuiLayout::SetVerticalAnchorType);
    }
  }

  void FormatDefault(nuiAnchorType value, nglString& string) const
  {
    if (value == eAnchorAbsolute)
    {
      string = "Absolute";
    }
    else if (value == eAnchorRelative)
    {
      string = "Relative";
    }
    
    return true;
  }

  bool ToString(void* pTarget, int32 index0, int32 index1, nglString& rString) const
  //bool ToString(nuiAnchorType Value, nglString& rString) const
  {
    nuiAnchorType Value = mGetAnchorDelegate(mAnchor);
    if (Value == eAnchorAbsolute)
    {
      rString = "Absolute";
      return true;
    }
    else if (Value == eAnchorRelative)
    {
      rString = "Relative";
      return true;
    }
    return false;
  }

  bool FromString(void* pTarget, int32 index0, int32 index1, const nglString& rString) const
  //bool FromString(nuiAnchorType& Value, const nglString& rString) const
  {
    if (rString.Compare("Relative", false) == 0)
    {
      mSetAnchorDelegate(mAnchor, eAnchorRelative);
      return true;
    }
    else if (rString.Compare("Absolute", false) == 0)
    {
      mSetAnchorDelegate(mAnchor, eAnchorAbsolute);
      return true;
    }
    return false;
  }
  


protected:
  int32 _Get() const
  {
    return (int32)mGetAnchorDelegate(mAnchor);
  }

  void _Set(int32 value)
  {
    mSetAnchorDelegate(mAnchor, (nuiAnchorType)value);
  }

  nuiFastDelegate1<const nglString&, nuiAnchorType> mGetAnchorDelegate;
  nuiFastDelegate2<const nglString&, nuiAnchorType> mSetAnchorDelegate;
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

  if (rName.CompareLeft("VAnchors_", true) == 0 || rName.CompareLeft("HAnchors_", true) == 0)
  {
    // Create an attribute for this anchor, unless it exists already
    AddAttribute(new LayoutAnchorValue(rName, this));
    {
      nuiAttribBase attr(GetAttribute(rName));
      NGL_ASSERT(attr.IsValid());
      attr.FromString(rValue);
    }
  }
  else if (rName.CompareLeft("VAnchorsType_", true) == 0 || rName.CompareLeft("HAnchorsType_", true) == 0)
  {
    // Create an attribute for this anchor, unless it exists already
    AddAttribute(new nuiAttribute<nuiAnchorType>(rName, this));
    {
      nuiAttribBase attr(GetAttribute(rName));
      NGL_ASSERT(attr.IsValid());
      attr.FromString(rValue);
    }
  }
  else
  {
    nuiObject::SetProperty(rName, rValue);
  }

}

float nuiLayout::ComputeAnchorPosition(const nglString& rName, int32 AnchorIndex, float Start, float Stop) const
{
  float Size = Stop - Start;
  float pos = AnchorIndex == 0 ? GetHorizontalAnchorPosition(rName) : GetVerticalAnchorPosition(rName);
  nuiAnchorType type = AnchorIndex == 0 ? GetHorizontalAnchorType(rName) : GetVerticalAnchorType(rName);
  switch (type)
  {
    case eAnchorAbsolute:
      if (pos < 0)
      {
        return Stop + pos;
      }
      else
      {
        return pos;
      }
      break;
    case eAnchorRelative:
      if (pos < 0)
      {
        return Stop + (pos * Size);
      }
      else
      {
        return pos * Size;
      }
      break;
  }

  return 0;
}


void nuiLayout::ComputeConstraint(const nuiLayoutConstraint& rC, float& ActualStart, float& ActualStop, float Start, float Stop, float IdealSize, int32 AnchorIndex)
{
  switch (rC.GetType())
  {
    case eFreeLayout:
      break;
    case eSetStartAndStop:
      {
        ActualStart = ComputeAnchorPosition(rC.GetAnchor(0), AnchorIndex, Start, Stop);
        ActualStop = ComputeAnchorPosition(rC.GetAnchor(1), AnchorIndex, Start, Stop);
      }
      break;

    case eSetStart:
      ActualStart = ComputeAnchorPosition(rC.GetAnchor(0), AnchorIndex, Start, Stop);
      ActualStop = ActualStart + IdealSize;
      break;
    case eSetStartAndSize:
      ActualStart = ComputeAnchorPosition(rC.GetAnchor(0), AnchorIndex, Start, Stop);
      ActualStop = ActualStart + rC.GetSize();
      break;

    case eSetStop:
      ActualStop = ComputeAnchorPosition(rC.GetAnchor(0), AnchorIndex, Start, Stop);
      ActualStart = ActualStop - IdealSize;
      break;
    case eSetStopAndSize:
      ActualStop = ComputeAnchorPosition(rC.GetAnchor(0), AnchorIndex, Start, Stop);
      ActualStart = ActualStop - rC.GetSize();
      break;

    case eSetMiddle:
      {
        const nglString& a0(rC.GetAnchor(0));
        const nglString& a1(rC.GetAnchor(1));
        float s0 = Start, s1 = Stop;
        if (!a0.IsEmpty())
        {
          s0 = s1 = ComputeAnchorPosition(a0, AnchorIndex, Start, Stop);
          if (!a1.IsEmpty())
            s1 = ComputeAnchorPosition(a1, AnchorIndex, Start, Stop);
        }

        float middle = s0 + (s1 - s0) * 0.5;
        ActualStart = middle - IdealSize * 0.5;
        ActualStop = ActualStart + IdealSize;
      }
      break;
    case eSetMiddleAndSize:
      {
        const nglString& a0(rC.GetAnchor(0));
        const nglString& a1(rC.GetAnchor(1));
        float s0 = Start, s1 = Stop;
        if (!a0.IsEmpty())
        {
          s0 = s1 = ComputeAnchorPosition(a0, AnchorIndex, Start, Stop);
          if (!a1.IsEmpty())
            s1 = ComputeAnchorPosition(a1, AnchorIndex, Start, Stop);
        }

        float middle = s0 + (s1 - s0) * 0.5;
        ActualStart = middle - rC.GetSize() * 0.5;
        ActualStop = ActualStart + rC.GetSize();
      }
      break;

    case eSetSize:
      ActualStart = Start;
      ActualStop = Start + rC.GetSize();
      break;

    default:
      NGL_ASSERT(0);
      break;
  }
}

void nuiLayout::DoLayout(const nuiRect& rRect)
{
  nuiRect r(GetRect());
  float width = r.GetWidth();
  float height = r.GetHeight();

  SetHorizontalAnchor("left", 0, eAnchorAbsolute);
  SetHorizontalAnchor("right", width, eAnchorAbsolute);
  SetVerticalAnchor("top", 0, eAnchorAbsolute);
  SetVerticalAnchor("bottom", height, eAnchorAbsolute);
  auto it = mConstraints.begin();
  while (it != mConstraints.end())
  {
    nuiWidget* pWidget = it->first;
    float left = 0, right = width, top = 0, bottom = height;
    nuiRect ideal(pWidget->GetIdealRect());
    float l = ideal.Left(), r = ideal.Right(), t = ideal.Top(), b = ideal.Bottom();
    const nuiLayoutConstraint& rH(it->second.first);
    const nuiLayoutConstraint& rV(it->second.second);

    // Horizontal Layout:
    ComputeConstraint(rH, l, r, left, right, ideal.GetWidth(), 0);

    // Vertical Layout:
    ComputeConstraint(rV, t, b, top, bottom, ideal.GetHeight(), 1);

    pWidget->SetLayout(nuiRect(l, t, r, b, false));

    ++it;
  }
}

bool nuiLayout::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  //NGL_OUT("nuiLayout::SetRect(%s)\n", rRect.GetValue().GetChars());
  
  nuiRect r(rRect.Size());
  DoLayout(r);
  
  return true;
}


