/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRect_h__
#define __nuiRect_h__

//#include "nui.h"
#include "nglMatrix.h"
#include "nuiFlags.h"
#include "nuiColor.h"

typedef nglMatrix<nuiSize> nuiMatrix;
typedef nglVector<nuiSize> nuiVector;
typedef nglVector2<nuiSize> nuiVector2;
typedef nglVector3<nuiSize> nuiVector3;

/// This class implements a 2D rectangle with many very useful methods to manage them.
class NUI_API nuiRect
{
public:
  nuiRect()
  {
    Set(0.0f, 0.0f, 0.0f, 0.0f, false);
  }

  explicit nuiRect(nuiSize x1, nuiSize y1, nuiSize x2, nuiSize y2, bool SecondPairIsSize = true)
  {
    Set(x1,y1,x2,y2,SecondPairIsSize);
  }

  explicit nuiRect(nuiSize Width, nuiSize Height)
  {
    Set(0.0f, 0.0f, Width, Height, true);
  }

  nuiRect(int32 x1, int32 y1, int32 x2, int32 y2, bool SecondPairIsSize = true)
  {
    Set(x1,y1,x2,y2,SecondPairIsSize);
  }

  nuiRect(int32 Width, int32 Height)
  {
    Set(0, 0, Width, Height);
  }


/*
  nuiRect(int x1, int y1, int x2, int y2, bool SecondPairIsSize = true)
  {
    Set((long)x1,(long)y1,(long)x2,(long)y2,SecondPairIsSize);
  }
*/

  nuiRect(const nuiRect& rRect)
  {
    mLeft = rRect.mLeft;
    mTop = rRect.mTop;
    mRight = rRect.mRight;
    mBottom = rRect.mBottom;
  }

  virtual ~nuiRect() {}

  void Set(nuiSize x1 = 0.0f, nuiSize y1 = 0.0f, nuiSize x2 = 0.0f, nuiSize y2 = 0.0f, bool SecondPairIsSize = true)
  {
    if (SecondPairIsSize)
    {
      mLeft = x1;
      mTop = y1;
      mRight = x1 + x2;
      mBottom = y1 + y2;
    }
    else
    {
      mLeft = MIN(x1,x2);
      mTop = MIN(y1,y2);
      mRight = MAX(x1,x2);
      mBottom = MAX(y1,y2);
    }
  }

  void Set(long x1 = 0, long y1 = 0, long x2 = 0, long y2 = 0, bool SecondPairIsSize = true)
  {
    Set((nuiSize)x1, (nuiSize)y1, (nuiSize)x2, (nuiSize)y2,SecondPairIsSize);
  }

  void Set(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, bool SecondPairIsSize = true)
  {
    Set((nuiSize)x1, (nuiSize)y1, (nuiSize)x2, (nuiSize)y2,SecondPairIsSize);
  }

  void SetSize(long w, long h)
  {
    Set(mLeft, mTop, (nuiSize)w, (nuiSize)h ,true);
  }

  void SetSize(nuiSize w, nuiSize h)
  {
    Set(mLeft, mTop, w, h ,true);
  }

  void SetWidth(nuiSize w)
  {
    Set(mLeft, mTop, w, GetHeight() ,true);
  }

  void SetHeight(nuiSize h)
  {
    Set(mLeft, mTop, GetWidth(), h ,true);
  }

  nuiSize GetWidth() const
  {
    return mRight - mLeft;
  }

  nuiSize GetHeight() const
  {
    return mBottom - mTop;
  }

  void GetSize(nuiSize& rWidth, nuiSize& rHeight) const
  {
    rWidth  = GetWidth();
    rHeight = GetHeight();
  }

  void GetSize(long& rWidth, long& rHeight) const
  {
    rWidth  = (long)GetWidth();
    rHeight = (long)GetHeight();
  }

  nuiSize GetSurface() const
  {
    return GetWidth()*GetHeight();
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

  void Move(nuiSize DeltaX, nuiSize DeltaY)
  {
    mLeft += DeltaX;
    mRight += DeltaX;
    mTop += DeltaY;
    mBottom += DeltaY;

  }

  void MoveTo(nuiSize X, nuiSize Y)
  {
    nuiSize w,h;
    w = mRight - mLeft;
    h = mBottom - mTop;
    mLeft = X;
    mTop = Y;
    mRight = mLeft + w;
    mBottom = mTop + h;

  }

  void Transform(const nuiMatrix& rMatrix);
  bool Intersect(const nuiRect& rRect1, const nuiRect& rRect2);
  void Union (const nuiRect& rRect1, const nuiRect& rRect2);
  void Union (float X, float Y);
  void Grow(nuiSize x, nuiSize y);
  void Scale(nuiSize x, nuiSize y);
  nuiRect Size() const;
  bool IsInside(nuiSize x, nuiSize y) const;
  bool IsInsideExact(nuiSize x, nuiSize y) const;
  enum DrawMode
  {
    Lines,
    Fill
  };
  void SetPosition(nuiPosition Position, const nuiRect& Inside);

  void RoundToNearest();
  void RoundToBelow();
  void RoundToZero();
  void RoundToAbove();
  void RoundToBiggest();
  void RoundToSmallest();

  // nuiVRect:
  virtual nglString GetValue() const;
  virtual bool SetValue(const nglString& Value);
  nuiRect& operator= (const nuiRect& Rect);
  bool operator== (const nuiRect& Rect) const;
  bool IsEqual(const nuiRect& Rect) const;

  nuiPosition GetPosition(nuiSize X,nuiSize Y, nuiSize HotSpotIn = 1.5, nuiSize HotSpotOut = 1.5) const;


public:
  nuiSize mLeft,
      mRight,
      mTop,
      mBottom;
};

#endif // __nuiRect_h__
