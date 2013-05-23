/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglMatrix.h
\brief Homogeneous matrixes template classes
*/

#ifndef __nglMatrix_h__
#define __nglMatrix_h__

//#include "nui.h"
#include "nglMath.h"
#include "nglVector.h"


//! Homogenous 4x4 matrixes
/*!
This class implements the usual operations on 4x4 homogenous matrixes used
in OpenGL, as a client side counterpart of the OpenGL matrix manipulations
functions. Remember that this implementation can't benefit of any hardware
accelerated TnL, and that you should only perform minimal geometry computing
without OpenGL.

The internal representation is the same as the OpenGL, it is ordered as
column first. Illustration :

\code
  // Using the Array field, we have this layout :
  //
  //   Array[0]  Array[4]  Array[8]  Array[12]
  //   Array[1]  Array[5]  Array[9]  Array[13]
  //   Array[2]  Array[6]  Array[10] Array[14]
  //   Array[3]  Array[7]  Array[11] Array[15]

  // Using the named elements from Elt instead of the array, we have this layout :
  //
  //   Elt.M11  Elt.M12  Elt.M13  Elt.M14
  //   Elt.M21  Elt.M22  Elt.M23  Elt.M24
  //   Elt.M31  Elt.M32  Elt.M33  Elt.M34
  //   Elt.M41  Elt.M42  Elt.M43  Elt.M44
\endcode
*/
template <class T> class nglMatrix
{
public:
  typedef T ValueType;

  union
  {
    T Array[16];
    struct 
    {
      T M11,M21,M31,M41;
      T M12,M22,M32,M42;
      T M13,M23,M33,M43;
      T M14,M24,M34,M44;
    } Elt;
  };

  /// Create an identity matrix (see SetIdentity())
  nglMatrix()
  {
    SetIdentity();
  }

  /*! Create a matrix from an array
    \param pArray flat array (see nglMatrix for conventions), nullify all elements if NULL
  */
  nglMatrix(const T pArray[16])
  {
    if (pArray)
      memcpy (Array, pArray, 16*sizeof(T));
    else
      Nullify();
  }

  virtual ~nglMatrix()
  {
  }

  /// Set all elements to zero
  void Nullify()
  {
    uint i;
    for (i = 0; i < 16; i++)
      Array[i] = 0.f;
  }

  /// Set the matrix to identity
  void SetIdentity()
  {
    Nullify();
    Elt.M11 = 1.f;
    Elt.M22 = 1.f;
    Elt.M33 = 1.f;
    Elt.M44 = 1.f;
  }

  bool IsIdentity() const
  {
    if  (Elt.M11 == 1.0f
      && Elt.M22 == 1.0f
      && Elt.M33 == 1.0f
      && Elt.M44 == 1.0f

      && Elt.M12 == 0.0f
      && Elt.M13 == 0.0f
      && Elt.M14 == 0.0f

      && Elt.M21 == 0.0f
      && Elt.M23 == 0.0f
      && Elt.M24 == 0.0f

      && Elt.M31 == 0.0f
      && Elt.M32 == 0.0f
      && Elt.M34 == 0.0f

      && Elt.M41 == 0.0f
      && Elt.M42 == 0.0f
      && Elt.M43 == 0.0f)
    {
      return true;
    }

    return false;
  }

  /// Transpose the matrix
  void Transpose ()
  {
    T x;

    x = Array[1];  Array[1]  = Array[4];  Array[4]  = x;
    x = Array[2];  Array[2]  = Array[8];  Array[8]  = x;
    x = Array[3];  Array[3]  = Array[12]; Array[12] = x;
    x = Array[6];  Array[6]  = Array[9];  Array[9]  = x;
    x = Array[7];  Array[7]  = Array[13]; Array[13] = x;
    x = Array[11]; Array[11] = Array[14]; Array[14] = x;
  }

  /// Compute the determinant
  T Determinant () const
  {
    return
       (Elt.M11*Elt.M22 - Elt.M12*Elt.M21)*(Elt.M33*Elt.M44 - Elt.M34*Elt.M43)
      -(Elt.M11*Elt.M23 - Elt.M13*Elt.M21)*(Elt.M32*Elt.M44 - Elt.M34*Elt.M42)
      +(Elt.M11*Elt.M24 - Elt.M14*Elt.M21)*(Elt.M32*Elt.M43 - Elt.M33*Elt.M42)
      +(Elt.M12*Elt.M23 - Elt.M13*Elt.M22)*(Elt.M31*Elt.M44 - Elt.M34*Elt.M41)
      -(Elt.M12*Elt.M24 - Elt.M14*Elt.M22)*(Elt.M31*Elt.M43 - Elt.M33*Elt.M41)
      +(Elt.M13*Elt.M24 - Elt.M14*Elt.M23)*(Elt.M31*Elt.M42 - Elt.M32*Elt.M41);
  }

  /// Invert matrix (does not check orthogonality)
  void Invert ()
  {
    T det = Determinant();
    if (det == 0.f) return;

    nglMatrix<T> result;

    det = 1.f / det;
    result.Elt.M11 = det * (Elt.M22*(Elt.M33*Elt.M44 - Elt.M34*Elt.M43) + Elt.M23*(Elt.M34*Elt.M42 - Elt.M32*Elt.M44) + Elt.M24*(Elt.M32*Elt.M43 - Elt.M33*Elt.M42));
    result.Elt.M12 = det * (Elt.M32*(Elt.M13*Elt.M44 - Elt.M14*Elt.M43) + Elt.M33*(Elt.M14*Elt.M42 - Elt.M12*Elt.M44) + Elt.M34*(Elt.M12*Elt.M43 - Elt.M13*Elt.M42));
    result.Elt.M13 = det * (Elt.M42*(Elt.M13*Elt.M24 - Elt.M14*Elt.M23) + Elt.M43*(Elt.M14*Elt.M22 - Elt.M12*Elt.M24) + Elt.M44*(Elt.M12*Elt.M23 - Elt.M13*Elt.M22));
    result.Elt.M14 = det * (Elt.M12*(Elt.M24*Elt.M33 - Elt.M23*Elt.M34) + Elt.M13*(Elt.M22*Elt.M34 - Elt.M24*Elt.M32) + Elt.M14*(Elt.M23*Elt.M32 - Elt.M22*Elt.M33));
    result.Elt.M21 = det * (Elt.M23*(Elt.M31*Elt.M44 - Elt.M34*Elt.M41) + Elt.M24*(Elt.M33*Elt.M41 - Elt.M31*Elt.M43) + Elt.M21*(Elt.M34*Elt.M43 - Elt.M33*Elt.M44));
    result.Elt.M22 = det * (Elt.M33*(Elt.M11*Elt.M44 - Elt.M14*Elt.M41) + Elt.M34*(Elt.M13*Elt.M41 - Elt.M11*Elt.M43) + Elt.M31*(Elt.M14*Elt.M43 - Elt.M13*Elt.M44));
    result.Elt.M23 = det * (Elt.M43*(Elt.M11*Elt.M24 - Elt.M14*Elt.M21) + Elt.M44*(Elt.M13*Elt.M21 - Elt.M11*Elt.M23) + Elt.M41*(Elt.M14*Elt.M23 - Elt.M13*Elt.M24));
    result.Elt.M24 = det * (Elt.M13*(Elt.M24*Elt.M31 - Elt.M21*Elt.M34) + Elt.M14*(Elt.M21*Elt.M33 - Elt.M23*Elt.M31) + Elt.M11*(Elt.M23*Elt.M34 - Elt.M24*Elt.M33));
    result.Elt.M31 = det * (Elt.M24*(Elt.M31*Elt.M42 - Elt.M32*Elt.M41) + Elt.M21*(Elt.M32*Elt.M44 - Elt.M34*Elt.M42) + Elt.M22*(Elt.M34*Elt.M41 - Elt.M31*Elt.M44));
    result.Elt.M32 = det * (Elt.M34*(Elt.M11*Elt.M42 - Elt.M12*Elt.M41) + Elt.M31*(Elt.M12*Elt.M44 - Elt.M14*Elt.M42) + Elt.M32*(Elt.M14*Elt.M41 - Elt.M11*Elt.M44));
    result.Elt.M33 = det * (Elt.M44*(Elt.M11*Elt.M22 - Elt.M12*Elt.M21) + Elt.M41*(Elt.M12*Elt.M24 - Elt.M14*Elt.M22) + Elt.M42*(Elt.M14*Elt.M21 - Elt.M11*Elt.M24));
    result.Elt.M34 = det * (Elt.M14*(Elt.M22*Elt.M31 - Elt.M21*Elt.M32) + Elt.M11*(Elt.M24*Elt.M32 - Elt.M22*Elt.M34) + Elt.M12*(Elt.M21*Elt.M34 - Elt.M24*Elt.M31));
    result.Elt.M41 = det * (Elt.M21*(Elt.M33*Elt.M42 - Elt.M32*Elt.M43) + Elt.M22*(Elt.M31*Elt.M43 - Elt.M33*Elt.M41) + Elt.M23*(Elt.M32*Elt.M41 - Elt.M31*Elt.M42));
    result.Elt.M42 = det * (Elt.M31*(Elt.M13*Elt.M42 - Elt.M12*Elt.M43) + Elt.M32*(Elt.M11*Elt.M43 - Elt.M13*Elt.M41) + Elt.M33*(Elt.M12*Elt.M41 - Elt.M11*Elt.M42));
    result.Elt.M43 = det * (Elt.M41*(Elt.M13*Elt.M22 - Elt.M12*Elt.M23) + Elt.M42*(Elt.M11*Elt.M23 - Elt.M13*Elt.M21) + Elt.M43*(Elt.M12*Elt.M21 - Elt.M11*Elt.M22));
    result.Elt.M44 = det * (Elt.M11*(Elt.M22*Elt.M33 - Elt.M23*Elt.M32) + Elt.M12*(Elt.M23*Elt.M31 - Elt.M21*Elt.M33) + Elt.M13*(Elt.M21*Elt.M32 - Elt.M22*Elt.M31));

    *this = result;
  }

  /// Invert an homogenous transformation matrix (the matrix must only be composed of scaling, rotations and translations.
  void InvertHomogenous ()
  {
    T det = Determinant();
    if (det == 0.f) return;

    nglMatrix<T> result;

    det = 1.f / det;
    result.Elt.M11 = det * (Elt.M22 * Elt.M33 - Elt.M23 * Elt.M32);
    result.Elt.M12 = det * (Elt.M32 * Elt.M13 - Elt.M33 * Elt.M12);
    result.Elt.M13 = det * (Elt.M12 * Elt.M23 - Elt.M13 * Elt.M22);
    result.Elt.M14 = det * (Elt.M12 * (Elt.M24 * Elt.M33 - Elt.M23 * Elt.M34) + Elt.M13 * (Elt.M22 * Elt.M34 - Elt.M24 * Elt.M32) + Elt.M14 * (Elt.M23 * Elt.M32 - Elt.M22 * Elt.M33));

    result.Elt.M21 = det * (Elt.M23 * Elt.M31 - Elt.M21 * Elt.M33);
    result.Elt.M22 = det * (Elt.M33 * Elt.M11 - Elt.M31 * Elt.M13);
    result.Elt.M23 = det * (Elt.M13 * Elt.M21 - Elt.M11 * Elt.M23);
    result.Elt.M24 = det * (Elt.M13 * (Elt.M24 * Elt.M31 - Elt.M21 * Elt.M34) + Elt.M14 * (Elt.M21 * Elt.M33 - Elt.M23 * Elt.M31) + Elt.M11 * (Elt.M23 * Elt.M34 - Elt.M24 * Elt.M33));
    
    result.Elt.M31 = det * (Elt.M21 * Elt.M32 - Elt.M22 * Elt.M31);
    result.Elt.M32 = det * (Elt.M31 * Elt.M12 - Elt.M32 * Elt.M11);
    result.Elt.M33 = det * (Elt.M11 * Elt.M22 - Elt.M12 * Elt.M21);
    result.Elt.M34 = det * (Elt.M14 * (Elt.M22 * Elt.M31 - Elt.M21 * Elt.M32) + Elt.M11 * (Elt.M24 * Elt.M32 - Elt.M22 * Elt.M34) + Elt.M12 * (Elt.M21 * Elt.M34 - Elt.M24 * Elt.M31));
    
    result.Elt.M41 = 0;
    result.Elt.M42 = 0;
    result.Elt.M43 = 0;
    result.Elt.M44 = det * (Elt.M11 * (Elt.M22 * Elt.M33 - Elt.M23 * Elt.M32) + Elt.M12 * (Elt.M23 * Elt.M31 - Elt.M21 * Elt.M33) + Elt.M13 * (Elt.M21 * Elt.M32 - Elt.M22 * Elt.M31));

    *this = result;
  }

  nglVector3<T> GetCol(int col) const
  {
    return nglVector3<T>((*this)(0, col), (*this)(1, col), (*this)(2, col));
  }

  void SetCol(int col, const nglVector3<T>& rCol)
  {
    (*this)(0, col) = rCol[0];
    (*this)(1, col) = rCol[1];
    (*this)(2, col) = rCol[2];
  }

  nglVector3<T> GetRow3(int row) const
  {
    return nglVector3<T>((*this)(row, 0), (*this)(row, 1), (*this)(row, 2));
  }

  nglVector<T> GetRow(int row) const
  {
    return nglVector<T>((*this)(row, 0), (*this)(row, 1), (*this)(row, 2), (*this)(row, 3));
  }

  void SetRow(int row, const nglVector3<T>& rRow)
  {
    (*this)(row, 0) = rRow[0];
    (*this)(row, 1) = rRow[1];
    (*this)(row, 2) = rRow[2];
  }

  void SetRow(int row, const nglVector<T>& rRow)
  {
    (*this)(row, 0) = rRow[0];
    (*this)(row, 1) = rRow[1];
    (*this)(row, 2) = rRow[2];
    (*this)(row, 3) = rRow[3];
  }

  /// Setup a translation matrix
  void SetDirectTranslation (const nglVector<T>& rDelta)
  {
    SetDirectTranslation(rDelta[0], rDelta[1], rDelta[2]);
  }

  /// Setup a translation matrix
  void SetDirectTranslation (T X, T Y, T Z)
  {
    Elt.M14 = X;
    Elt.M24 = Y;
    Elt.M34 = Z;
  }

  /// Setup a translation matrix
  void SetTranslation (const nglVector<T>& rDelta)
  {
    SetTranslation(rDelta[0], rDelta[1], rDelta[2]);
  }

  /// Setup a translation matrix
  void SetTranslation (T X, T Y, T Z)
  {
    SetIdentity();
    SetDirectTranslation(X, Y, Z);
  }

  nglVector3<T> GetTranslation() const
  {
    return nglVector3<T>(Elt.M14, Elt.M24, Elt.M34);
  }

  /// Compose with a translation
  void Translate (const nglVector<T>& rDelta)
  {
    Translate(rDelta[0], rDelta[1], rDelta[2]);
  }

  /// Compose with a translation
  void Translate (T X, T Y, T Z)
  {
    nglMatrix<T> m;
    m.SetTranslation(X, Y, Z);
    (*this) *= m;
  }

  /// Setup a scaling transformation (homothetia)
  void SetScaling (const nglVector<T>& rVector)
  {
    Nullify();
    Elt.M11 = rVector[0];
    Elt.M22 = rVector[1];
    Elt.M33 = rVector[2];
    Elt.M44 = 1;
  }

  /// Setup a scaling transformation (homothetia)
  void SetScaling (T X, T Y, T Z)
  {
    Nullify();
    Elt.M11 = X;
    Elt.M22 = Y;
    Elt.M33 = Z;
    Elt.M44 = 1;
  }

  /// Apply a scaling transformation
  void Scale (T X, T Y, T Z)
  {
    nglMatrix<T> m;
    m.SetScaling(X, Y, Z);
    (*this) *= m;
  }

  /// Apply a scaling transformation
  void Scale (const nglVector<T>& rVector)
  {
    Scale(rVector[0], rVector[1], rVector[2]);
  }

  /// Setup a rotation matrix of \a Angle degrees around \a rAxis (using right hand convention)
  void SetRotation (T Angle, const nglVector<T>& rAxis)
  {
    nglVector<T> axis = rAxis;
    axis.Normalize();

    T a = (T) DEG2RAD(Angle);
    T s = (T) sin (a);
    T c = (T) cos (a);
    T t = (T) 1.0 - c;
    T x = axis[0];
    T y = axis[1];
    T z = axis[2];
    T xy = x * y;
    T xz = x * z;
    T yz = y * z;

    Elt.M11 = t * x * x + c;
    Elt.M12 = t * xy    - s * z;
    Elt.M13 = t * xz    + s * y;
    Elt.M14 = 0.f;

    Elt.M21 = t * xy    + s * z;
    Elt.M22 = t * y * y + c;
    Elt.M23 = t * yz    - s * x;
    Elt.M24 = 0.f;

    Elt.M31 = t * xz    - s * y;
    Elt.M32 = t * yz    + s * x;
    Elt.M33 = t * z * z + c;
    Elt.M34 = 0.f;

    Elt.M41 = 0.f;
    Elt.M42 = 0.f;
    Elt.M43 = 0.f;
    Elt.M44 = 1.f;
  }

  /// Setup a rotation matrix of \a Angle degrees around the axis which coordinates are \a X, \a Y and \a Z
  void SetRotation (T Angle, T X, T Y, T Z)
  {
    SetRotation(Angle, nglVector<T>(X, Y, Z));
  }

  /// Compose with a rotation of \a Angle degrees around \a rAxis
  void Rotate (T Angle, const nglVector<T>& rAxis)
  {
    nglMatrix<T> trans;
    trans.SetRotation(Angle, rAxis);
    (*this) *= trans;
  }

  /// Compose with a rotation of \a Angle degrees around the axis which coordinates are \a X, \a Y and \a Z
  void Rotate (T Angle, T X, T Y, T Z)
  {
    nglMatrix<T> trans;
    trans.SetRotation(Angle, X, Y, Z);
    (*this) *= trans;
  }

  /// Setup a camera matrix, given eye and target position and up direction
  void LookAt (T EyeX, T EyeY, T EyeZ, T TargetX, T TargetY, T TargetZ, T UpX, T UpY, T UpZ)
  {
    LookAt(nglVector<T>(EyeX, EyeY, EyeZ),
           nglVector<T>(TargetX, TargetY, TargetZ),
           nglVector<T>(UpX, UpY, UpZ));
  }

  /// Setup a camera matrix, given eye and target position and up direction
  void LookAt (const nglVector<T>& rEye, const nglVector<T>& rTarget, const nglVector<T>& rUp)
  {
    nglVector<T> forward = rTarget - rEye;
    forward.Normalize();

    /* Side = forward x up */
    nglVector<T> right = forward ^ rUp;
    right.Normalize();

    /* Recompute up as : up = right x forward */
    nglVector<T> up = right ^ forward;

    Elt.M11 = right[0];
    Elt.M12 = right[1];
    Elt.M13 = right[2];
    Elt.M14 = -rEye[0];

    Elt.M21 = up[0];
    Elt.M22 = up[1];
    Elt.M23 = up[2];
    Elt.M24 = -rEye[1];

    Elt.M31 = -forward[0];
    Elt.M32 = -forward[1];
    Elt.M33 = -forward[2];
    Elt.M34 = -rEye[2];

    Elt.M41 = 0.f;
    Elt.M42 = 0.f;
    Elt.M43 = 0.f;
    Elt.M44 = 1.f;
  }

  /*! Interpret the matrix as a rotation matrix with \a pAlpha, \a pBeta and \a pGama Euler angles
      Only return valid results if the matrix determinant is 1
  */
  void GetEulerAngles (T* pAlpha, T* pBeta, T* pGamma) const
  {
    if (Elt.M21 != 0.f)
    {
      if (pAlpha) *pAlpha = (T)asin(Elt.M32);
      if (pBeta)  *pBeta  = (T)atan2(-Elt.M31, Elt.M33);
      if (pGamma) *pGamma = (T)atan2(-Elt.M21, Elt.M22);
    }
    else
    {
      if (pAlpha) *pAlpha = (T)asin(Elt.M32);
      if (pBeta)  *pBeta  = 0.f;
      if (pGamma) *pGamma = (T)atan2(-Elt.M21,Elt.M11);
    }
  }

  /// Setup an orthogonal projection matrix given a clipping cube
  bool SetOrtho (T Left, T Right, T Bottom, T Top, T Near, T Far)
  {
    T deltax, deltay, deltaz;

    deltax = Right - Left;
    deltay = Top - Bottom;
    deltaz = Far - Near;
    if ((deltax == 0.) || (deltay == 0.f) || (deltaz == 0.f))
      return false;

    Nullify();
    Elt.M11 = (T)(2.f / deltax);
    Elt.M14 = (T)(-(Right + Left) / deltax);
    Elt.M22 = (T)(2.f / deltay);
    Elt.M24 = (T)(-(Top + Bottom) / deltay);
    Elt.M33 = (T)(-2.f / deltaz);
    Elt.M34 = (T)(-(Far + Near) / deltaz);
    Elt.M44 = 1.f;

    return true;
  }

  /// Setup an orthogonal projection matrix, with near and far Z planes set to -1 and 1 respectively
  bool SetOrtho (T Left, T Right, T Bottom, T Top)
  {
    return SetOrtho(Left, Right, Bottom, Top, -1.f, 1.f);
  }

  /*! Setup a perspective projection matrix
    \param FovY field of view in degree, in the vertical (Y) direction
    \param Aspect ratio, ie. width / height using viewport dimensions
    \param Near smaller Z value (positive)
    \param Far greater Z value (positive)
  */
  bool SetPerspective (T FovY, T Aspect, T Near, T Far)
  {
    T sine, cotangent, deltaZ;
    T angle = (T)DEG2RAD(FovY);

    deltaZ = Far - Near;
    sine = (T)sin(angle);
    if ((deltaZ == 0.f) || (sine == 0.f) || (Aspect == 0.f))
      return false;

    cotangent = (T)cos(angle) / sine;

    Nullify();
    Elt.M11 = (T)(cotangent / Aspect);
    Elt.M22 = (T)cotangent;
    Elt.M33 = (T)(-(Far + Near) / deltaZ);
    Elt.M43 = -1.f;
    Elt.M34 = (T)(-2.f * Near * Far / deltaZ);

    return true;
  }

  /*! Setup a perspective projection matrix
    \param Left smaller X value (left plane)
    \param Right greater X value (right plane)
    \param Bottom smaller Y value (bottom plane)
    \param Top greater Y value (top plane)
    \param Near smaller Z value (near plane)
    \param Far greater Z value (far plane)
  */
  bool SetFrustum (T Left, T Right, T Bottom, T Top, T Near, T Far)
  {
    T deltaX, deltaY, deltaZ;

    deltaX = Right - Left;
    deltaY = Top - Bottom;
    deltaZ = Far - Near;
    if ((Near <= 0.f) || (Far <= 0.f) ||
        (deltaX == 0.f) || (deltaY == 0.f) || (deltaZ == 0.f)) 
      return false;

    Nullify();
    Elt.M11 = (T)(Near * 2.f / deltaX);
    Elt.M22 = (T)(Near * 2.f / deltaY);
    Elt.M13 = (T)((Right + Left) / deltaX);
    Elt.M23 = (T)((Top + Bottom) / deltaY);
    Elt.M33 = (T)(-(Far + Near) / deltaZ);
    Elt.M43 = (T)(-1.f);
    Elt.M34 = (T)(((T) -2.f) * Near * Far / deltaZ);

    return true;
  }

  /*
   * Operators
   */

  /// Return the value of the element from the i-th line and the j-th column
  T operator () (uint i, uint j) const
  {
    NGL_ASSERT(i < 4 && j < 4);
    return Array[i + 4*j];
  }

  T& operator () (uint i, uint j)
  {
    NGL_ASSERT(i < 4 && j < 4);
    return Array[i + 4*j];
  }

  nglMatrix<T> operator = (const nglMatrix<T>& rMatrix)
  {
    memcpy(Array, rMatrix.Array, 16*sizeof(T));
    return (*this);
  }

  /*!< Right matrix multiplication. These two lines perform the same operation :
\code
mat1 *= mat2;
mat1 = mat1 * mat2;
\endcode
  */
  void operator *= (const nglMatrix<T>& rMatrix)
  {
    struct
    {
      T M11,M21,M31,M41;
      T M12,M22,M32,M42;
      T M13,M23,M33,M43;
      T M14,M24,M34,M44;
    } elt;

    elt.M11 =(Elt.M11 * rMatrix.Elt.M11 + Elt.M12 * rMatrix.Elt.M21 + Elt.M13 * rMatrix.Elt.M31 + Elt.M14 * rMatrix.Elt.M41);
    elt.M12 =(Elt.M11 * rMatrix.Elt.M12 + Elt.M12 * rMatrix.Elt.M22 + Elt.M13 * rMatrix.Elt.M32 + Elt.M14 * rMatrix.Elt.M42);
    elt.M13 =(Elt.M11 * rMatrix.Elt.M13 + Elt.M12 * rMatrix.Elt.M23 + Elt.M13 * rMatrix.Elt.M33 + Elt.M14 * rMatrix.Elt.M43);
    elt.M14 =(Elt.M11 * rMatrix.Elt.M14 + Elt.M12 * rMatrix.Elt.M24 + Elt.M13 * rMatrix.Elt.M34 + Elt.M14 * rMatrix.Elt.M44);

    elt.M21 =(Elt.M21 * rMatrix.Elt.M11 + Elt.M22 * rMatrix.Elt.M21 + Elt.M23 * rMatrix.Elt.M31 + Elt.M24 * rMatrix.Elt.M41);
    elt.M22 =(Elt.M21 * rMatrix.Elt.M12 + Elt.M22 * rMatrix.Elt.M22 + Elt.M23 * rMatrix.Elt.M32 + Elt.M24 * rMatrix.Elt.M42);
    elt.M23 =(Elt.M21 * rMatrix.Elt.M13 + Elt.M22 * rMatrix.Elt.M23 + Elt.M23 * rMatrix.Elt.M33 + Elt.M24 * rMatrix.Elt.M43);
    elt.M24 =(Elt.M21 * rMatrix.Elt.M14 + Elt.M22 * rMatrix.Elt.M24 + Elt.M23 * rMatrix.Elt.M34 + Elt.M24 * rMatrix.Elt.M44);

    elt.M31 =(Elt.M31 * rMatrix.Elt.M11 + Elt.M32 * rMatrix.Elt.M21 + Elt.M33 * rMatrix.Elt.M31 + Elt.M34 * rMatrix.Elt.M41);
    elt.M32 =(Elt.M31 * rMatrix.Elt.M12 + Elt.M32 * rMatrix.Elt.M22 + Elt.M33 * rMatrix.Elt.M32 + Elt.M34 * rMatrix.Elt.M42);
    elt.M33 =(Elt.M31 * rMatrix.Elt.M13 + Elt.M32 * rMatrix.Elt.M23 + Elt.M33 * rMatrix.Elt.M33 + Elt.M34 * rMatrix.Elt.M43);
    elt.M34 =(Elt.M31 * rMatrix.Elt.M14 + Elt.M32 * rMatrix.Elt.M24 + Elt.M33 * rMatrix.Elt.M34 + Elt.M34 * rMatrix.Elt.M44);
                                                                                                
    elt.M41 =(Elt.M41 * rMatrix.Elt.M11 + Elt.M42 * rMatrix.Elt.M21 + Elt.M43 * rMatrix.Elt.M31 + Elt.M44 * rMatrix.Elt.M41);
    elt.M42 =(Elt.M41 * rMatrix.Elt.M12 + Elt.M42 * rMatrix.Elt.M22 + Elt.M43 * rMatrix.Elt.M32 + Elt.M44 * rMatrix.Elt.M42);
    elt.M43 =(Elt.M41 * rMatrix.Elt.M13 + Elt.M42 * rMatrix.Elt.M23 + Elt.M43 * rMatrix.Elt.M33 + Elt.M44 * rMatrix.Elt.M43);
    elt.M44 =(Elt.M41 * rMatrix.Elt.M14 + Elt.M42 * rMatrix.Elt.M24 + Elt.M43 * rMatrix.Elt.M34 + Elt.M44 * rMatrix.Elt.M44);

    memcpy(&(this->Elt), &elt, sizeof(Elt));
  }

  bool operator == (const nglMatrix<T>& rMatrix) const
  {
    return memcmp(Array, rMatrix.Array, sizeof(Array)) == 0;
  }

  /// Matrix multiplication, result is rA * rB
  friend nglMatrix<T> operator * (const nglMatrix& rA, const nglMatrix& rB)
  {
    nglMatrix<T> result = rA;
    result *= rB;
    return result;
  }

  /// Left matrix multiplication with a vector, ie. result = matrix * vector
  nglVector<T> operator * (const nglVector<T>& rVector) const
  {
    nglVector<T> result;
    T v0, v1, v2, v3;
    uint i;

    v0 = rVector[0];
    v1 = rVector[1];
    v2 = rVector[2];
    v3 = rVector[3];

    for (i = 0; i < 4; i++)
      result[i] = v0*Array[i] + v1*Array[4+i] + v2*Array[8+i] + v3*Array[12+i];

    return result;
  }

  /// Scalar multiplication (each element is multiplied by a scalar)
  void operator *= (T Scalar)
  {
    uint i;
    for (i = 0; i < 16; i++)
      Array[i] *= Scalar;
  }

  /*
   * Debug
   */

  /// Display the matrix content in the log
  void Dump (uint Level = 0, const nglChar* pTitle = NULL) const
  {
    uint i;

    if (pTitle)
    {
      NGL_LOG(_T("math"), Level, _T("%s :"), pTitle);
    }
    for (i = 0; i < 4; i++)
    {
      NGL_LOG(_T("math"), Level, _T("[ %8.3f %8.3f %8.3f %8.3f ] "), (T)(*this)(i,0), (T)(*this)(i,1), (T)(*this)(i,2), (T)(*this)(i,3));
    }
  }

  bool GetValue(nglString& rDump) const
  {
    rDump.CFormat(_T("{ %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f }"),
                  (T)Array[ 0], (T)Array[ 1], (T)Array[ 2], (T)Array[ 3], 
                  (T)Array[ 4], (T)Array[ 5], (T)Array[ 6], (T)Array[ 7], 
                  (T)Array[ 8], (T)Array[ 9], (T)Array[10], (T)Array[11], 
                  (T)Array[12], (T)Array[13], (T)Array[14], (T)Array[15]
        );
    return true;
  }
  
  bool SetValue(const nglString& rValue)
  {
    nglString val = rValue;
    
    val.Trim();
    val.TrimLeft(_T('{'));
    val.TrimRight(_T('}'));
    
    std::vector<nglString> tokens;
    val.Tokenize(tokens, _T(' '));
    
    for (uint i = 0; i < MIN(16, tokens.size()); i++)
    {
      Array[i] = (T)tokens[i].GetCDouble();
    }
    
    return true;
  }
  
};



typedef nglMatrix<float> nglMatrixf;
typedef nglMatrix<double> nglMatrixd;

#endif // __nglMatrix_h__
