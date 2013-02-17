/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

void nuiRect::Transform(const nuiMatrix& rMatrix)
{
  nuiVector v1(mLeft,mTop,0),v2(mRight, mBottom,0);
  v1 = rMatrix * v1;
  v2 = rMatrix * v2;

  mLeft   = v1[0];
  mTop    = v1[1];
  mRight  = v2[0];
  mBottom = v2[1];

}

bool nuiRect::Intersect(const nuiRect& rRect1, const nuiRect& rRect2)
{
  nuiSize dest_x, dest_y;
  nuiSize dest_w, dest_h;
  bool return_val;

  return_val = false;

  dest_x = MAX(rRect1.mLeft,   rRect2.mLeft  );
  dest_y = MAX(rRect1.mTop,    rRect2.mTop   );
  dest_w = MIN(rRect1.mRight,  rRect2.mRight ) - dest_x;
  dest_h = MIN(rRect1.mBottom, rRect2.mBottom) - dest_y;

  if (dest_w > 0 && dest_h > 0)
  {
    mLeft = dest_x;
    mTop = dest_y;
    mRight = mLeft + dest_w;
    mBottom = mTop + dest_h;
    return_val = true;
  }
  else
  {
    mLeft = mRight = mTop = mBottom = 0;
  }

  return return_val;
}

void nuiRect::Union(const nuiRect& rRect1, const nuiRect& rRect2)
{
  mLeft   = MIN(rRect1.mLeft,   rRect2.mLeft);
  mTop    = MIN(rRect1.mTop,    rRect2.mTop);
  mRight  = MAX(rRect1.mRight,  rRect2.mRight);
  mBottom = MAX(rRect1.mBottom, rRect2.mBottom);

}

void nuiRect::Union(float X, float Y)
{
  mLeft   = MIN(mLeft,   X);
  mTop    = MIN(mTop,    Y);
  mRight  = MAX(mRight,  X);
  mBottom = MAX(mBottom, Y);
  
}

void nuiRect::Grow(nuiSize x, nuiSize y)
{
  mLeft     -= x;
  mTop      -= y;
  mRight    += x;
  mBottom   += y;
  
}

void nuiRect::Scale(nuiSize x, nuiSize y)
{
  nuiSize w = GetWidth();
  w = ((w * x) - w) * .5f;
  nuiSize h = GetHeight();
  h = ((h * y) - h) * .5f;
  mLeft     -= w;
  mTop      -= h;
  mRight    += w;
  mBottom   += h;
  
}

nuiRect nuiRect::Size() const
{
  return nuiRect(0.0f, 0.0f, GetWidth(), GetHeight());
}

bool nuiRect::IsInsideExact(nuiSize x, nuiSize y) const
{
  return (x >= mLeft && x <= mRight && y >= mTop && y <= mBottom);
}

bool nuiRect::IsInside(nuiSize x, nuiSize y) const
{
  return (x > mLeft && x < mRight && y >= mTop && y < mBottom);
}

void nuiRect::SetPosition(nuiPosition Position, const nuiRect& Inside)
{
  nuiSize x,y,w,h;
  nuiSize rx,ry,rw,rh;
  rx = Inside.mLeft;
  ry = Inside.mTop;
  rw = Inside.GetWidth();
  rh = Inside.GetHeight();
  w = GetWidth();
  h = GetHeight();
  x = (rw - w) / 2.f;
  y = (rh - h) / 2.f;

  switch (Position)
  {
  case nuiCenter:
    mLeft = x;
    mRight = x+w;
    mTop = y;
    mBottom = y+h;
    break;
  case nuiFill:
    mLeft=0;
    mRight=rw;
    mTop=0;
    mBottom=rh;
    break;
  case nuiFillHorizontal:
    mLeft=0;
    mRight=rw;
    mTop=y;
    mBottom=y+h;
    break;
  case nuiFillVertical:
    mLeft=x;
    mRight=x+w;
    mTop=0;
    mBottom=rh;
    break;
  case nuiFillLeft:
    mLeft=0;
    mRight=w;
    mTop=0;
    mBottom=rh;
    break;
  case nuiFillRight:
    mLeft = rw-w;
    mRight = rw;
    mTop = 0;
    mBottom = rh;
    break;
  case nuiFillTop:
    mLeft = 0;
    mRight = rw;
    mTop = 0;
    mBottom = h;
    break;
  case nuiFillBottom:
    mLeft = 0;
    mRight = rw;
    mTop = rh - h;
    mBottom = rh;
    break;
  case nuiLeft:
    mLeft=0;
    mTop =y; 
    mRight=w;
    mBottom=y+h;
    break;
  case nuiRight:
    mLeft=rw-w;
    mTop =y; 
    mRight=rw;
    mBottom=y+h;
    break;
  case nuiTop:
    mLeft=x;
    mTop =0; 
    mRight=x+w;
    mBottom=h;
    break;
  case nuiBottom:
    mLeft=x;
    mTop =rh-h; 
    mRight=x+w;
    mBottom=rh;
    break;
  case nuiTopLeft:
    mLeft=0;
    mRight=w;
    mTop=0;
    mBottom=h;
    break;
  case nuiTopRight:
    mLeft=rw-w;
    mRight=rw;
    mTop=0;
    mBottom=h;
    break;
  case nuiBottomLeft:
    mLeft=0;
    mRight=w;
    mTop=rh-h;
    mBottom=rh;
    break;
  case nuiBottomRight:
    mLeft=rw-w;
    mRight=rw;
    mTop=rh-h;
    mBottom=rh;
    break;
  case nuiTile:
    mLeft = 0;
    mRight= w;
    mTop = 0;
    mBottom =h;
    break;
  case nuiNoPosition:
	break;
  }

  Move(rx, ry);
}

// nuiVRect:
nglString nuiRect::GetValue() const
{
  nglString str;
  str.CFormat("{%f %f %f %f}",Left(), Top(), GetWidth(), GetHeight());
  return str;
}

bool nuiRect::SetValue(const nglString& Value)
{
  float x=0,y=0,w=0,h=0;
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
    x = vec[0].GetCFloat();
    y = vec[1].GetCFloat();
    w = vec[2].GetCFloat();
    h = vec[3].GetCFloat();
    Set(x,y,w,h);
    //NGL_OUT(_T("%s\n"), GetValue().GetChars());

    return true;
  }
  return false;
}

nuiRect& nuiRect::operator= (const nuiRect& rRect)
{
  mLeft  = rRect.mLeft;
  mRight = rRect.mRight;
  mTop = rRect.mTop;
  mBottom = rRect.mBottom;
  return *this;
}

bool nuiRect::operator== (const nuiRect& rRect) const
{
  return IsEqual(rRect);
}

bool nuiRect::IsEqual(const nuiRect& rRect) const
{
  return (mLeft == rRect.mLeft && mRight == rRect.mRight && mTop == rRect.mTop && mBottom == rRect.mBottom);
}

void nuiRect::RoundToNearest()
{
  mLeft   = (nuiSize)ToNearest(mLeft  );
  mRight  = (nuiSize)ToNearest(mRight );
  mTop    = (nuiSize)ToNearest(mTop   );
  mBottom = (nuiSize)ToNearest(mBottom);
}

void nuiRect::RoundToBelow()
{
  mLeft   = (nuiSize)ToBelow(mLeft  );
  mRight  = (nuiSize)ToBelow(mRight );
  mTop    = (nuiSize)ToBelow(mTop   );
  mBottom = (nuiSize)ToBelow(mBottom);
}

void nuiRect::RoundToZero()
{
  mLeft   = (nuiSize)ToZero(mLeft  );
  mRight  = (nuiSize)ToZero(mRight );
  mTop    = (nuiSize)ToZero(mTop   );
  mBottom = (nuiSize)ToZero(mBottom);
}

void nuiRect::RoundToAbove()
{
  mLeft   = (nuiSize)ToAbove(mLeft  );
  mRight  = (nuiSize)ToAbove(mRight );
  mTop    = (nuiSize)ToAbove(mTop   );
  mBottom = (nuiSize)ToAbove(mBottom);
}

void nuiRect::RoundToBiggest()
{
  mLeft   = (nuiSize)ToBelow(mLeft  );
  mRight  = (nuiSize)ToAbove(mRight );
  mTop    = (nuiSize)ToBelow(mTop   );
  mBottom = (nuiSize)ToAbove(mBottom);
}

void nuiRect::RoundToSmallest()
{
  mLeft   = (nuiSize)ToAbove(mLeft  );
  mRight  = (nuiSize)ToBelow(mRight );
  mTop    = (nuiSize)ToAbove(mTop   );
  mBottom = (nuiSize)ToBelow(mBottom);
}

nuiPosition nuiRect::GetPosition(nuiSize X,nuiSize Y, nuiSize HotSpotIn, nuiSize HotSpotOut) const
{
  nuiPosition ClickPos = nuiNoPosition;
  
  nuiRect rect(*this);
  
  HotSpotIn = MIN(GetWidth() / 2, HotSpotIn); 
  HotSpotIn = MIN(GetHeight() / 2, HotSpotIn); 

  nuiRect out(*this);
  nuiRect in(*this);

  out.Grow(HotSpotOut, HotSpotOut);
  in.Grow(-HotSpotIn, -HotSpotIn);
 
  if (!out.IsInside(X, Y))
    return nuiNoPosition;
  
  if (in.IsInside(X, Y))
    return nuiCenter;
  
  if (X <= in.mLeft) // On the Left
  {
    if (Y <= in.mTop) // Top Left
      ClickPos = nuiTopLeft;
    else if (Y >= in.mBottom) // Bottom Left
      ClickPos = nuiBottomLeft;
    else // Left
      ClickPos = nuiLeft;
  }
  else if (X >= in.mRight) // On the Right
  {
    if (Y <= in.mTop) // Top Right
      ClickPos = nuiTopRight;
    else if (Y >= in.mBottom) // Bottom right
      ClickPos = nuiBottomRight;
    else // Right
      ClickPos = nuiRight;
  }
  else if (Y <= in.mTop) // Top
    ClickPos = nuiTop;
  else if (Y >= rect.mBottom) // Bottom
    ClickPos = nuiBottom;

  return ClickPos;
}
