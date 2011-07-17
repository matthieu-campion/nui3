/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPoint_h__
#define __nuiPoint_h__

//#include "nui.h"
#include "nglMath.h"

enum nuiPointType 
{
  nuiPointTypeNormal,
  nuiPointTypeStop
};

class NUI_API nuiPoint: public nuiVector
{
public:
  nuiPoint() : nuiVector(), mType(nuiPointTypeNormal)
  {
  }

  /// Initialize with values
  explicit nuiPoint(float X, float Y, float Z = 0.0f, float W = 1.f) : nuiVector(X,Y,Z,W), mType(nuiPointTypeNormal)
  {
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a two dimensional vector. Third element is zero, fourth is one.
  explicit nuiPoint(const nuiVector2& rVector) : nuiVector(rVector), mType(nuiPointTypeNormal)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a two dimensional vector. Fourth element is one.
  explicit nuiPoint(const nuiVector3& rVector)  : nuiVector(rVector), mType(nuiPointTypeNormal)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a four dimensional vector.
  explicit nuiPoint(const nuiVector& rVector)  : nuiVector(rVector), mType(nuiPointTypeNormal)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a special point type.
  explicit nuiPoint(nuiPointType type)  : nuiVector(), mType(type)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Copy constructor
  nuiPoint(const nuiPoint& rPoint)  : nuiVector(rPoint), mType(rPoint.mType)
  {
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  nuiPoint& operator=(const nuiPoint& rPoint)
  {
    *((nuiVector*)this) = (nuiVector)rPoint;
    SetType(rPoint.mType);
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
    return *this;
  }

  bool operator==(const nuiPoint& rPoint) const
  {
    return (nuiVector::operator==(rPoint)) && mType == rPoint.mType;
  }

  /// Destructor
  virtual ~nuiPoint() ///< Destructor.
  {
  }

  void SetType(nuiPointType type)
  {
    mType = type;
    if (mType == nuiPointTypeStop)
    {
      Elt[0] = Elt[1] = Elt[2] = Elt[3] = 0.0f;
    }
  }

  nuiPointType GetType() const
  {
    return mType;
  }

  static const nuiPoint Zero;
  static const nuiPoint One;

  bool IsValid() const
  {
    bool f0 = finite(Elt[0]) != 0;
    bool f1 = finite(Elt[1]) != 0;
    bool f2 = finite(Elt[2]) != 0;
    bool f3 = finite(Elt[3]) != 0;

    bool res = f0 && f1 && f2 && f3;

/*
    if (!res)
    {
      NGL_OUT(_T("Invalid point: {%f, %f, %f, %f}\n"), Elt[0], Elt[1], Elt[2], Elt[3]);
      _asm
      {
        int 3;
      }
    }
*/
    return res;
  }
protected:
  nuiPointType mType;
};

#endif // __nuiPoint_h__
