/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglVector.h
\brief Vectors (2, 3 and 4 dimensions) template classes
*/

#ifndef __nglVector_h__
#define __nglVector_h__

//#include "nui.h"
#include "nglMath.h"
//#include "nglApplication.h"


/// Two dimensional vectors
template <class T> class nglVector2
{
public:
  T Elt[2];

  /// Default constructor, set both coordinates to zero
  nglVector2()
  {
    Elt[0] = 0.f;
    Elt[1] = 0.f;
  }
  /// Initialize with values
  nglVector2(T X, T Y)
  {
    Elt[0] = X;
    Elt[1] = Y;
  }
  /// Destructor
  virtual ~nglVector2() {}

  /// Copy operator
  void operator=(const nglVector2<T>& rVector)
  {
    Elt[0] = rVector[0];
    Elt[1] = rVector[1];
  }

  void Set(T X, T Y)
  {
    Elt[0] = X;
    Elt[1] = Y;
  }
  
  
  /// Read-only access to an element
  T operator[](uint i) const
  {
    NGL_ASSERT(i < 2);
    return Elt[i];
  }
  /// Read-write access to an element
  T& operator[](uint i)
  {
    NGL_ASSERT(i < 2);
    return Elt[i];
  }

  /// Unary minus
  nglVector2<T> operator-() const
  {
    nglVector2<T> result(-Elt[0], -Elt[1]);
    return result;
  }

  /// Increment operator
  void operator+=(const nglVector2<T>& rVector)
  {
    Elt[0] += rVector[0];
    Elt[1] += rVector[1];
  }
  /// Decrement operator
  void operator-=(const nglVector2<T>& rVector)
  {
    Elt[0] -= rVector[0];
    Elt[1] -= rVector[1];
  }

  /// Multiplication by a scalar (scaling)
  void operator*=(T F)
  {
    Elt[0] *= F;
    Elt[1] *= F;
  }

  nglVector2<T> operator*(T F) const
  {
    return nglVector2<T>(Elt[0] * F, Elt[1] * F);
  }
  
  /// Scalar product
  T operator*(const nglVector2<T>& rVector) const
  {
    return Elt[0]*rVector[0] + Elt[1]*rVector[1];
  }

  /// Squared length
  T SquaredLength() const
  {
    return Elt[0]*Elt[0] + Elt[1]*Elt[1];
  }
  /// Length
  T Length() const
  {
    return std::sqrt(Elt[0]*Elt[0] + Elt[1]*Elt[1]);
  }
  /// Normalize. Returns false if the length is zero
  bool Normalize()
  {
    T l = Length();
    if (l != .0f)
    {
      Elt[0] /= l;
      Elt[1] /= l;
      return true;
    }
    return false;
  }
};


/// Three dimensional vectors
template <class T> class nglVector3
{
public:
  T Elt[3];

  /// Default constructor, set all coordinates to zero
  nglVector3()
  {
    Elt[0] = 0.f;
    Elt[1] = 0.f;
    Elt[2] = 0.f;
  }
  /// Initialize with values
  nglVector3(T X, T Y, T Z)
  {
    Elt[0] = X;
    Elt[1] = Y;
    Elt[2] = Z;
  }

  nglVector3(const nglVector3<T>& rVector)
  {
    Elt[0] = rVector.Elt[0];
    Elt[1] = rVector.Elt[1];
    Elt[2] = rVector.Elt[2];
  }

  /// Destructor
  virtual ~nglVector3() {}

  /// Copy operator
  void operator=(const nglVector3<T>& rVector)
  {
    Elt[0] = rVector[0];
    Elt[1] = rVector[1];
    Elt[2] = rVector[2];
  }

  void Set(T X, T Y, T Z)
  {
    Elt[0] = X;
    Elt[1] = Y;
    Elt[2] = Z;
  }
  
  
  /// Read-only access to an element
  T operator[](uint i) const
  {
    NGL_ASSERT(i < 3);
    return Elt[i];
  }
  /// Read-write access to an element
  T& operator[](uint i)
  {
    NGL_ASSERT(i < 3);
    return Elt[i];
  }

  /// Unary minus
  nglVector3<T> operator-() const
  {
    nglVector3<T> result(-Elt[0], -Elt[1], -Elt[2]);
    return result;
  }

  /// Increment operator
  void operator+=(const nglVector3<T>& rVector)
  {
    Elt[0] += rVector[0];
    Elt[1] += rVector[1];
    Elt[2] += rVector[2];
  }
  /// Decrement operator
  void operator-=(const nglVector3<T>& rVector)
  {
    Elt[0] -= rVector[0];
    Elt[1] -= rVector[1];
    Elt[2] -= rVector[2];
  }

  /// Vector addition
  nglVector3<T> operator+ (const nglVector3<T>& rVector) const
  {
    nglVector3<T> result = *this;
    result += rVector;
    return result;
  }
  /// Vector substraction
  nglVector3<T> operator- (const nglVector3<T>& rVector) const
  {
    nglVector3<T> result = *this;
    result -= rVector;
    return result;
  }
  
  /// Multiplication by a scalar (scaling)
  void operator*=(T F)
  {
    Elt[0] *= F;
    Elt[1] *= F;
    Elt[2] *= F;
  }
  
  nglVector3<T> operator*(T F) const
  {
    return nglVector3<T>(Elt[0] * F, Elt[1] * F, Elt[2] * F);
  }

  nglVector3<T> operator/(T F) const
  {
    F = 1.0f / F;
    return (*this) * F;
  }

  /// Scalar product
  T operator*(const nglVector3<T>& rVector) const
  {
    return Elt[0]*rVector[0] + Elt[1]*rVector[1] + Elt[2]*rVector[2];
  }
  /// Vector product
  void operator^=(const nglVector3<T>& rVector) 
  {
    T x, y, z;

    x = Elt[1] * rVector[2] - Elt[2] * rVector[1];
    y = Elt[2] * rVector[0] - Elt[0] * rVector[2];
    z = Elt[0] * rVector[1] - Elt[1] * rVector[0];

    Elt[0] = x;
    Elt[1] = y;
    Elt[2] = z;
  }
  /// Vector product
  nglVector3<T> operator^(const nglVector3<T>& rVector) const
  {
    nglVector3<T> result = *this;
    result ^= rVector;
    return result;
  }

  /// Squared length
  T SquaredLength() const
  {
    return Elt[0]*Elt[0] + Elt[1]*Elt[1] + Elt[2]*Elt[2];
  }
  /// Length
  T Length() const
  {
    return std::sqrt(Elt[0]*Elt[0] + Elt[1]*Elt[1] + Elt[2]*Elt[2]);
  }
  /// Normalize. Returns false if the length is zero
  bool Normalize()
  {
    T l = Length();
    if (l != .0f)
    {
      Elt[0] /= l;
      Elt[1] /= l;
      Elt[2] /= l;
      return true;
    }
    return false;
  }

  bool GetValue(nglString& rDump, int32 NbElements = 3) const
  {
    switch (NbElements)
    {
      case 1:
        rDump.CFormat(_T("{ %f }"), (T)Elt[0]);
        break;
      case 2:
        rDump.CFormat(_T("{ %f %f }"), (T)Elt[0], (T)Elt[1]);
        break;
      default:
        rDump.CFormat(_T("{ %f %f %f }"), (T)Elt[0], (T)Elt[1], (T)Elt[2]);
        break;
    }
    return true;
  }

  bool SetValue(const nglString& rValue)
  {
    //rDump.CFormat(_T("{ %8.3f %8.3f %8.3f }"), (T)Elt[0], (T)Elt[1], (T)Elt[2]);

    nglString val = rValue;

    val.Trim();
    val.TrimLeft(_T('{'));
    val.TrimRight(_T('}'));

    std::vector<nglString> tokens;
    val.Tokenize(tokens, _T(' '));

    for (uint i = 0; i < MIN(3, tokens.size()); i++)
    {
      Elt[i] = (T)tokens[i].GetCDouble();
    }

    return true;
  }
  

};


/// Four dimensional (homogeneous) vectors. See also nglMatrix.
template <class T> class nglVector
{
public:
  T Elt[4];

  /// Default constructor, set all coordinates to zero
  nglVector()
  { 
    Elt[0] = 0.f;
    Elt[1] = 0.f;
    Elt[2] = 0.f;
    Elt[3] = 0.f;
  }

  nglVector(const nglVector<T>& rVector)
  {
    Elt[0] = rVector.Elt[0];
    Elt[1] = rVector.Elt[1];
    Elt[2] = rVector.Elt[2];
    Elt[3] = rVector.Elt[3];
  }


  /// Initialize with values
  nglVector(T X, T Y, T Z, T W = 1.f)
  {
    Elt[0] = X;
    Elt[1] = Y;
    Elt[2] = Z;
    Elt[3] = W;
  }
  /// Initialize with a two dimensional vector. Third element is zero, fourth is one.
  nglVector<T>(const nglVector2<T>& rVector)
  { 
    *this = rVector; 
  }
  /// Initialize with a two dimensional vector. Fourth element is one.
  nglVector<T>(const nglVector3<T>& rVector)
  { 
    *this = rVector; 
  }
  /// Destructor
  virtual ~nglVector() ///< Destructor.
  {
  }

  void Set(T X, T Y, T Z, T W = 1.f)
  {
    Elt[0] = X;
    Elt[1] = Y;
    Elt[2] = Z;
    Elt[3] = W;
  }
  
  /// Copy operator. Third element is set to zero, fourth to one.
  void operator=(const nglVector2<T>& rVector)
  {
    Elt[0] = rVector[0];
    Elt[1] = rVector[1];
    Elt[2] = 0.f; 
    Elt[3] = 1.f;
  }
  /// Copy operator. Fourth element is set one.
  void operator=(const nglVector3<T>& rVector)
  {
    Elt[0] = rVector[0];
    Elt[1] = rVector[1];
    Elt[2] = rVector[2]; 
    Elt[3] = 1.f;
  }
  /// Copy operator
  void operator=(const nglVector<T>& rVector)
  {
    Elt[0] = rVector[0];
    Elt[1] = rVector[1];
    Elt[2] = rVector[2]; 
    Elt[3] = rVector[3];
  }

  /// Read-only access to an element
  T operator[](uint i) const
  {
    NGL_ASSERT(i < 4);
    return Elt[i]; 
  }
  /// Read-write access to an element
  T& operator[](uint i)
  {
    NGL_ASSERT(i < 4);
    return Elt[i]; 
  }

  T& x() { return Elt[0]; }
  const T& x() const { return Elt[0]; }
  T& y() { return Elt[1]; }
  const T& y() const { return Elt[1]; }
  T& z() { return Elt[2]; }
  const T& z() const { return Elt[2]; }
  T& w() { return Elt[3]; }
  const T& w() const { return Elt[3]; }


  /// Unary minus
  nglVector<T> operator-() const
  {
    nglVector<T> result(-Elt[0], -Elt[1], -Elt[2]);
    return result;
  }

  /// Increment operator
  void operator+=(const nglVector<T>& rVector)
  { 
    Elt[0] += rVector[0]; 
    Elt[1] += rVector[1]; 
    Elt[2] += rVector[2]; 
    Elt[3] += rVector[3]; 
  }
  /// Decrement operator
  void operator-=(const nglVector<T>& rVector)
  { 
    Elt[0] -= rVector[0]; 
    Elt[1] -= rVector[1]; 
    Elt[2] -= rVector[2];  
    Elt[3] -= rVector[3];
  }
  /// Vector addition
  nglVector<T> operator+ (const nglVector<T>& rVector) const
  {
    nglVector<T> result = *this;
    result += rVector;
    return result;
  }
  /// Vector substraction
  nglVector<T> operator- (const nglVector<T>& rVector) const
  {
    nglVector<T> result = *this;
    result -= rVector;
    return result;
  }

  /// Multiplication by a scalar (scaling)
  void operator*= (T F)
  {
    Elt[0] *= F;
    Elt[1] *= F;
    Elt[2] *= F;
  }
  /// Multiplication by a scalar (scaling)
  friend nglVector<T> operator* (const nglVector<T>& rVector, T F)
  {
    nglVector<T> result = rVector;
    result *= F;
    return result;
  }

  friend nglVector<T> operator* (T F, const nglVector<T>& rVector)
  {
    return rVector * F;
  }

  // Division by scalar:
  friend nglVector<T> operator/ (const nglVector<T>& rVector, T F)
  {
    return rVector * ((T)1.0 / F);
  }

  /// Scalar product
  T operator*(const nglVector<T>& rVector) const
  {
    T result;
    result  = Elt[0] * rVector[0];
    result += Elt[1] * rVector[1];
    result += Elt[2] * rVector[2];
    result += Elt[3] * rVector[3];
    return result;
  }
  /// Vector product
  void operator^=(const nglVector<T>& rVector)
  {
    T x, y, z, w;

    x = Elt[1] * rVector[2] - Elt[2] * rVector[1];
    y = Elt[2] * rVector[0] - Elt[0] * rVector[2];
    z = Elt[0] * rVector[1] - Elt[1] * rVector[0];
    w = Elt[3] * rVector[3];

    Elt[0] = x;
    Elt[1] = y;
    Elt[2] = z;
    Elt[3] = w;
  }
  /// Vector product
  nglVector<T> operator^(const nglVector<T>& rVector)
  {
    nglVector<T> result = *this;
    result ^= rVector;
    return result;
  }

  /// Squared length
  T SquaredLength() const
  {
    return Elt[0]*Elt[0] + Elt[1]*Elt[1] + Elt[2]*Elt[2]; 
  }
  /// Length
  T Length() const
  {
    return std::sqrt(Elt[0]*Elt[0] + Elt[1]*Elt[1] + Elt[2]*Elt[2]);
  }
  /// Normalize. Returns false if the length is zero
  bool Normalize()
  {
    T l = Length();
    if (l != 0.f)
    {
      Elt[0] /= l;
      Elt[1] /= l;
      Elt[2] /= l;
      return true;
    }
    return false;
  }

  /// Display the vector's scalar values in the log
  void Dump (uint Level = 0, const nglChar* pTitle = NULL) const
  {
    NGL_LOG(_T("math"), Level, _T("%s%s[ %8.3f %8.3f %8.3f %8.3f ]"),
      pTitle ? pTitle : _T(""),
      pTitle ? _T(" = ") : _T(""),
      (T)Elt[0], (T)Elt[1], (T)Elt[2], (T)Elt[3]);
  }

  bool operator==(const nglVector& rA) const
  {
    for (int i =0; i< 4; i++)
      if (Elt[i] != rA.Elt[i])
        return false;
    return true;
  }

  bool GetValue(nglString& rDump, int32 NbElements = 4) const
  {
    switch (NbElements)
    {
      case 1:
        rDump.CFormat(_T("{ %f }"), (T)Elt[0]);
        break;
      case 2:
        rDump.CFormat(_T("{ %f %f }"), (T)Elt[0], (T)Elt[1]);
        break;
      case 3:
        rDump.CFormat(_T("{ %f %f %f }"), (T)Elt[0], (T)Elt[1], (T)Elt[2]);
        break;
      default:
        rDump.CFormat(_T("{ %f %f %f %f }"), (T)Elt[0], (T)Elt[1], (T)Elt[2], (T)Elt[3]);
        break;
    }
    return true;
  }
  
  bool SetValue(const nglString& rValue)
  {
    //rDump.CFormat(_T("{ %8.3f %8.3f %8.3f %8.3f }"), (T)Elt[0], (T)Elt[1], (T)Elt[2], (T)Elt[3]);

    nglString val = rValue;
    
    val.Trim();
    val.TrimLeft(_T('{'));
    val.TrimRight(_T('}'));

    std::vector<nglString> tokens;
    val.Tokenize(tokens, _T(' '));
    
    for (uint i = 0; i < MIN(4, tokens.size()); i++)
    {
      Elt[i] = (T)tokens[i].GetCDouble();
    }
    
    return true;
  }

  operator nglVector3<T> () const
  {
    return nglVector3<T>(Elt[0], Elt[1], Elt[2]);
  }

  operator nglVector2<T> () const
  {
    return nglVector2<T>(Elt[0], Elt[1]);
  }

};


/*
 * Specializations
 */

typedef nglVector2<float>  nglVector2f;
typedef nglVector3<float>  nglVector3f;
typedef nglVector<float>   nglVectorf;
typedef nglVector2<double> nglVector2d;
typedef nglVector3<double> nglVector3d;
typedef nglVector<double>  nglVectord;


template <class T>
void nuiDistanceFromLineAndSegment(T px, T py, T x0, T y0 , T x1, T y1, T &rDistanceFromLine, T &rDistanceFromSegment)
{
  T r_numerator = (px - x0) * (x1 - x0) + (py - y0) * (y1 - y0);
  T r_denominator = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
  T r = r_numerator / r_denominator;
  //
  //    
  T s =  ((y0 - py) * (x1 - x0) - (x0 - px) * (y1 - y0) ) / r_denominator;
  
  rDistanceFromLine = fabs(s) * std::sqrt(r_denominator);
  
  //
  // (xx,yy) is the point on the lineSegment closest to (px,py)
  //
  
  if ( (r >= 0.0) && (r <= 1.0) )
  {
    rDistanceFromSegment = rDistanceFromLine;
  }
  else
  {
    
    T dist1 = (px - x0) * (px - x0) + (py - y0) * (py - y0);
    T dist2 = (px - x1) * (px - x1) + (py - y1) * (py - y1);
    if (dist1 < dist2)
      rDistanceFromSegment = std::sqrt(dist1);
    else
      rDistanceFromSegment = std::sqrt(dist2);
  }

  return;
}

template <class T>
T nuiDistanceFromLine(T px, T py, T x0, T y0 , T x1, T y1)
{
  T r_numerator = (px - x0) * (x1 - x0) + (py - y0) * (y1 - y0);
  T r_denominator = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);

  T s =  ((y0 - py) * (x1 - x0) - (x0 - px) * (y1 - y0) ) / r_denominator;
  
  return fabs(s) * std::sqrt(r_denominator);
}

template <class T>
T nuiDistanceFromSegment(T px, T py, T x0, T y0 , T x1, T y1)
{
  T l, s;
  nuiDistanceFromLineAndSegment(px, py, x0, y0, x1, y1, l, s);
  return s;
}


template <class T>
void nuiDistanceFromLineAndSegment(const nglVector2<T>& p, const nglVector2<T>& p0 , const nglVector2<T>& p1, T &rDistanceFromLine, T &rDistanceFromSegment)
{
  nuiDistanceFromLineAndSegment(p[0], p[1], p0[0], p0[1], p1[0], p1[1], rDistanceFromLine, rDistanceFromSegment);
}

template <class T>
T nuiDistanceFromLine(const nglVector2<T>& p, const nglVector2<T>& p0 , const nglVector2<T>& p1)
{
  return nuiDistanceFromLine(p[0], p[1], p0[0], p0[1], p1[0], p1[1]);
}

template <class T>
T nuiDistanceFromSegment(const nglVector2<T>& p, const nglVector2<T>& p0 , const nglVector2<T>& p1)
{
  return nuiDistanceFromSegment(p[0], p[1], p0[0], p0[1], p1[0], p1[1]);
}

#endif // __nglVector_h__
