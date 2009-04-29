/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

class nuiBorder
{
public:
  nuiBorder()
  {
    Set(0.0f, 0.0f, 0.0f, 0.0f);
  }

  explicit nuiBorder(nuiSize left, nuiSize top, nuiSize right, nuiSize bottom)
  {
    Set(left,top,right,bottom);
  }

  nuiBorder(int32 left, int32 top, int32 right, int32 bottom)
  {
    Set(left,top,right,bottom);
  }

  nuiBorder(const nuiBorder& rBorder)
  {
    mLeft = rBorder.mLeft;
    mTop = rBorder.mTop;
    mRight = rBorder.mRight;
    mBottom = rBorder.mBottom;
  }

  virtual ~nuiBorder() {}

  void Set(nuiSize left = 0.0f, nuiSize top = 0.0f, nuiSize right = 0.0f, nuiSize bottom = 0.0f)
  {
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;
  }

  void Set(long left = 0, long top = 0, long right = 0, long bottom = 0)
  {
    Set((nuiSize)left, (nuiSize)top, (nuiSize)right, (nuiSize)bottom);
  }

  void Set(int left = 0, int top = 0, int right = 0, int bottom = 0)
  {
    Set((nuiSize)left, (nuiSize)top, (nuiSize)right, (nuiSize)bottom);
  }

  void SetWidth(nuiSize w)
  {
    Set(w/2.f, mTop, w/2.f, mBottom);
  }
  
  void SetHeight(nuiSize h)
  {
    Set(mLeft, h/2.f, mRight, h/2.f);
  }
  
  nuiSize GetWidth() const
  {
    return mLeft+mRight;
  }

  nuiSize GetHeight() const
  {
    return mTop+mBottom;
  }

  nuiSize& Left()
  {
    return mLeft;
  }

  nuiSize& Right()
  {
    return mRight;
  }

  nuiSize& Top()
  {
    return mTop;
  }

  nuiSize& Bottom()
  {
    return mBottom;
  }

  const nuiSize& Left() const
  {
    return mLeft;
  }

  const nuiSize& Right() const
  {
    return mRight;
  }

  const nuiSize& Top() const
  {
    return mTop;
  }

  const nuiSize& Bottom() const
  {
    return mBottom;
  }


  nglString GetValue() const
  {
    nglString str;
    str.CFormat("{%f %f %f %f}",Left(), Top(), Right(), Bottom());
    return str;
  }

  bool SetValue(const nglString& Value)
  {
    float left=0,top=0,right=0,bottom=0;
    long res;
    nglString str=Value;
    if (!str.GetLength() || str[0]!='{' || str[str.GetLength()-1]!='}')
      return false;
    str.DeleteLeft(1);
    str.DeleteRight(1);
    std::vector<nglString> vec;
    res = str.Tokenize(vec);
    if (vec.size() == 4)
    {
      left = vec[0].GetCFloat();
      top = vec[1].GetCFloat();
      right = vec[2].GetCFloat();
      bottom = vec[3].GetCFloat();
      Set(left,top,right,bottom);
      return true;
    }
    return false;
  }

  nuiBorder& operator= (const nuiBorder& rBorder)
  {
    mLeft  = rBorder.mLeft;
    mRight = rBorder.mRight;
    mTop = rBorder.mTop;
    mBottom = rBorder.mBottom;
    return *this;
  }

  bool operator== (const nuiBorder& rBorder) const
  {
    return IsEqual(rBorder);
  }

  bool IsEqual(const nuiBorder& rBorder) const
  {
    return (mLeft == rBorder.mLeft && mRight == rBorder.mRight && mTop == rBorder.mTop && mBottom == rBorder.mBottom);
  }

  
public:
  nuiSize mLeft,
      mRight,
      mTop,
      mBottom;
};

