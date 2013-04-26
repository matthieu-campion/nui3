/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglQuaternion.h
\brief Quaternion template class
*/

#ifndef __nglQuaternion_h__
#define __nglQuaternion_h__

//#include "nui.h"
#include "nglVector.h"
#include "nglMatrix.h"


//! Quaternion
/*!
This template class provides an implementation of (four dimensional) quaternion
calculus. The most useful application in the computing and 3D area relies on
the SLERP interpolation and the from/to homogeneous matrixes.

See http://www.gamedev.net/reference/articles/article1095.asp for a practical
approach.
*/
template<class T> class nglQuaternion
{
public:
  union 
  {
    T Array[4]; 
    struct
    {
      T x, y, z, w;
    } Elt;
  };

  /// Create a null quaternion (all elements set to zero)
  nglQuaternion()
  {
    Elt.x = 0.f;
    Elt.y = 0.f;
    Elt.z = 0.f;
    Elt.w = 1.f;
  }

  nglQuaternion(const nglQuaternion<T>& rQuat)
  {
    Elt.x = rQuat.Elt.x;
    Elt.y = rQuat.Elt.y;
    Elt.z = rQuat.Elt.z;
    Elt.w = rQuat.Elt.w;
  }

  virtual ~nglQuaternion()
  {
  }

  /// Set to zero (all elements to zero)
  void Nullify()
  {
    //printf("quat::Nullify\n");
    Elt.x = 0.f;
    Elt.y = 0.f;
    Elt.z = 0.f;
    Elt.w = 0.f;
  }

  /// Set to (multiplication) identity
  void SetIdentity()
  {
    //printf("quat::SetId\n");
    Elt.x = 0.f;
    Elt.y = 0.f;
    Elt.z = 0.f;
    Elt.w = 1.f;
  }

  /// Invert the quaternion (= get the conjugate). This method only works if this quaternion is normalized (otherwise use InvertAny())
  void Invert()
  {
    Elt.x = - Elt.x;
    Elt.y = - Elt.y;
    Elt.z = - Elt.z;
    //Elt.w =   Elt.w;
  }

  T SquaredLength() const
  {
    return (Elt.w * Elt.w) + (Elt.x * Elt.x) + (Elt.y * Elt.y) + (Elt.z * Elt.z);
  }

  T Length() const
  {
    return std::sqrt(SquaredLength());
  }

  T Normalize()
  {
    T length = Length();
    if (length < 1e-4f)
      return 0.f;
    T rcpLength = 1.f / length;
    Elt.x *= rcpLength;
    Elt.y *= rcpLength;
    Elt.z *= rcpLength;
    Elt.w *= rcpLength;
    return length;
  }

  /*! Set to a rotation representation
    \param Angle rotation angle, in degrees
    \param X axis X coordinate
    \param Y axis Y coordinate
    \param Z axis Z coordinate
  */
  void SetRotation (const T Angle, const T X, const T Y, const T Z)
  {
    //printf("quat::SetRot %f {%f, %f %f}\n", Angle, X, Y, Z);
    T angle_rad = (T) (DEG2RAD(Angle) * .5f);

    T s = (T) sin ( angle_rad );

    Elt.w = (T) cos ( angle_rad );
    Elt.x = s * X;
    Elt.y = s * Y;
    Elt.z = s * Z;
  }

  /*! Set to a rotation representation
    \param Angle rotation angle, in degrees
    \param rAxis rotation axis
  */
  void SetRotation (const T Angle, const nglVector<T>& rAxis)
  {
    SetRotation (Angle, rAxis[0], rAxis[1], rAxis[2]);
  }

  /*! Get rotation representation
    \param rAxis returns the rotation axis
    \return rotation angle
  */
  T GetRotation (nglVector<T>& rAxis) const
  {
    T a = (T) acos ( Elt.w );
    T s = (T) sin  ( a );
    T angle = 0.f;

    angle = (T) RAD2DEG(a) * 2.f;

    if ( s == 0.f )
    {
      rAxis[0] = 0.f;
      rAxis[1] = 0.f;
      rAxis[2] = 1.f;
    }
    else
    {
      T m = 1.f / s;
      rAxis[0] = Elt.x * m;
      rAxis[1] = Elt.y * m;
      rAxis[2] = Elt.z * m;
    }

    return angle;
  }

  /// Create a quaternion from Euler angles in degrees
  void SetEulerAngles(T x, T y, T z)
  {
    x = DEG2RAD(x);
    y = DEG2RAD(y);
    z = DEG2RAD(z);

    T sh = (T) -sin ( x );
    T ch = (T)  cos ( x );
    T sp = (T) -sin ( y );
    T cp = (T)  cos ( y );
    T sr = (T) -sin ( z );
    T cr = (T)  cos ( z );

    T cpch = cp * ch;
    T spsh = sp * sh;

    Elt.w = cr * cpch + sr * spsh;
    Elt.x = sr * cpch - cr * spsh;
    Elt.y = cr * sp * ch + sr * cp * sh;
    Elt.z = cr * cp * sh - sr * sp * ch;
  }
  


  /// Multiplication
  nglQuaternion<T> operator* (const nglQuaternion<T> &rq) const
  {
    nglQuaternion<T> result;
    const float x = Elt.x;
    const float y = Elt.y;
    const float z = Elt.z;
    const float w = Elt.w;
    const float nx = rq.Elt.x;
    const float ny = rq.Elt.y;
    const float nz = rq.Elt.z;
    const float nw = rq.Elt.w;

    result.Elt.x = w * nx + x * nw + y * nz - z * ny;
    result.Elt.y = w * ny + y * nw + z * nx - x * nz;
    result.Elt.z = w * nz + z * nw + x * ny - y * nx;
    result.Elt.w = w * nw - x * nx - y * ny - z * nz;

    return result;
  }

  /// Conversion from an homogeneous matrix
  nglQuaternion<T>& operator = (const nglMatrix<T>& rMatrix)
  {
    //printf("quat::=\n");
    T trace = rMatrix(0,0) + rMatrix(1,1) + rMatrix(2,2);

    if (trace > 0.f)
    {
      T s = (T) sqrt (trace + 1.f);

      Elt.w = (T) (s / 2.f);
      s = (T) (0.5f / s);
      Elt.x = (rMatrix(1,2) - rMatrix(2,1)) * s;
      Elt.y = (rMatrix(2,0) - rMatrix(0,2)) * s;
      Elt.z = (rMatrix(0,1) - rMatrix(1,0)) * s;
    }
    else 
    {
      uint i, j, k;
      uint next[3] = {1, 2, 0};

      i = 0;
      if (rMatrix(1,1) > rMatrix(0,0)) i = 1;
      if (rMatrix(2,2) > rMatrix(i,i)) i = 2;
      j = next[i];
      k = next[j];

      T s = (T) (sqrt ((rMatrix(i,i) - (rMatrix(j,j) + rMatrix(k,k))) + 1.f));

      Array[i] = (T) (s * 0.5f);
      if (s != 0.f) s = (T) (0.5f / s);
      Array[j] = (rMatrix(i,j) + rMatrix(j,i)) * s;
      Array[k] = (rMatrix(i,k) + rMatrix(k,i)) * s;
      Array[3] = (rMatrix(j,k) - rMatrix(k,j)) * s;
    }

    return *this;
  }

  /// Conversion to matrix
//  operator nglMatrix<T> () const
//  {
//    nglMatrix<T> result;
//
//    T two_xx = Elt.x * (Elt.x + Elt.x);
//    T two_xy = Elt.x * (Elt.y + Elt.y);
//    T two_xz = Elt.x * (Elt.z + Elt.z);
//
//    T two_wx = Elt.w * (Elt.x + Elt.x);
//    T two_wy = Elt.w * (Elt.y + Elt.y);
//    T two_wz = Elt.w * (Elt.z + Elt.z);
//
//    T two_yy = Elt.y * (Elt.y + Elt.y);
//    T two_yz = Elt.y * (Elt.z + Elt.z);
//
//    T two_zz = Elt.z * (Elt.z + Elt.z);
//
//    result.Array[0]= 1.f - (two_yy+two_zz);
//    result.Array[1]= two_xy - two_wz;
//    result.Array[2]= two_xz + two_wy;
//    result.Array[3]= 0.f;
//
//    result.Array[4]= two_xy + two_wz;
//    result.Array[5]= 1.f - (two_xx + two_zz);
//    result.Array[6]= two_yz - two_wx;
//    result.Array[7]= 0.f;
//
//    result.Array[8]= two_xz - two_wy;
//    result.Array[9]= two_yz + two_wx;
//    result.Array[10]= 1.f - (two_xx + two_yy);
//    result.Array[11]= 0.f;
//
//    result.Array[12]= 0.f;
//    result.Array[13]= 0.f;
//    result.Array[14]= 0.f;
//    result.Array[15]= 1.f;
//
//    return result;
//  }

  operator nglMatrix<T> () const
  {
    const T x = Elt.x;
    const T y = Elt.y;
    const T z = Elt.z;
    const T w = Elt.w;

    const T x2 = x * x;
    const T y2 = y * y;
    const T z2 = z * z;
    const T xy = x * y;
    const T xz = x * z;
    const T yz = y * z;
    const T wx = w * x;
    const T wy = w * y;
    const T wz = w * z;

    // This calculation would be a lot more complicated for non-unit length quaternions
    // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
    //   OpenGL
    T a[] = {
      1.0f - 2.0f * (y2 + z2),
      2.0f * (xy - wz),
      2.0f * (xz + wy),
      0.0f,

      2.0f * (xy + wz),
      1.0f - 2.0f * (x2 + z2),
      2.0f * (yz - wx),
      0.0f,

      2.0f * (xz - wy),
      2.0f * (yz + wx),
      1.0f - 2.0f * (x2 + y2),
      0.0f,

      0.0f,
      0.0f,
      0.0f,
      1.0f
    };

    return nglMatrix<T>(a);
  }

  /// Conversion to vector (x, y and z quaternion components are pasted, w is zero)
  operator nglVector<T> () const
  {
    return ::nglVector<T>(Elt.x, Elt.y, Elt.z, 0.f);
  }

  /// Inversion
  void InvertAny()
  {
    //printf("quat::Invert\n");
    T n;

    n = Elt.x * Elt.x + Elt.y * Elt.y + Elt.z * Elt.z + Elt.w * Elt.w;

    Elt.w =  n * Elt.w;
    Elt.x = -n * Elt.x;
    Elt.y = -n * Elt.y;
    Elt.z = -n * Elt.z;
  }

  // SLERP interpolation from this instance to \a rTarget at position \a Pos
  nglQuaternion<T> SlerpTo( const nglQuaternion& rTarget, T Pos) const
  {
    //printf("quat::SlerpTo\n");
    T sign, co, scale0, scale1;
    nglQuaternion<T> quat;

    co = Elt.x * rTarget.Elt.x + Elt.y * rTarget.Elt.y + Elt.z * rTarget.Elt.z + Elt.w * rTarget.Elt.w;

    if ( co < (T) 0.f )
    {
      co = -co;
      sign = (T) -1.;
    }
    else
      sign = (T) 1.;

    if( co < (T) 1. )
    {
      double o = acos( co );
      double so = sin( o );

      scale0 = (T)( sin( ((T)1. - Pos) * o ) / so );
      scale1 = (T)( sin( Pos * o ) / so );
    }
    else
    {
      scale0 = (T) (1.f - Pos);
      scale1 = (T) Pos;
    }

    quat.Elt.x = scale0 * Elt.x + scale1 * ((sign >= (T) 0.) ? rTarget.Elt.x : -rTarget.Elt.x);
    quat.Elt.y = scale0 * Elt.y + scale1 * ((sign >= (T) 0.) ? rTarget.Elt.y : -rTarget.Elt.y);
    quat.Elt.z = scale0 * Elt.z + scale1 * ((sign >= (T) 0.) ? rTarget.Elt.z : -rTarget.Elt.z);
    quat.Elt.w = scale0 * Elt.w + scale1 * ((sign >= (T) 0.) ? rTarget.Elt.w : -rTarget.Elt.w);

    return quat;
  }


  bool GetValue(nglString& rDump) const
  {
    rDump.CFormat(_T("{ %f %f %f %f }"), (T)Array[0], (T)Array[1], (T)Array[2], (T)Array[3]);
    return true;
  }

  bool SetValue(const nglString& rValue)
  {
    //printf("quat::SetValue(%s)\n", rValue.GetChars());
    nglString val = rValue;

    val.Trim();
    val.TrimLeft(_T('{'));
    val.TrimRight(_T('}'));

    std::vector<nglString> tokens;
    val.Tokenize(tokens, _T(' '));

    for (uint i = 0; i < 4; i++)
    {
      Array[i] = (T)tokens[i].GetCDouble();
    }

    return tokens.size() == 4;
  }

  // Next 3 method stolen from http://nic-gamedev.blogspot.fr/2011/11/quaternion-math-getting-local-axis.html
  nglVector3<T> GetForwardVector() const
  {
    const float w = Elt.w;
    const float x = Elt.x;
    const float y = Elt.y;
    const float z = Elt.z;
    return nglVector3<T>(  2 * (x * z + w * y),
                           2 * (y * x - w * x),
                           1 - 2 * (x * x + y * y));
  }

  nglVector3<T> GetUpVector() const
  {
    const float w = Elt.w;
    const float x = Elt.x;
    const float y = Elt.y;
    const float z = Elt.z;
    return nglVector3<T>(  2 * (x * y - w * z),
                           1 - 2 * (x * x + z * z),
                           2 * (y * z + w * x));
  }

  nglVector3<T> GetRightVector() const
  {
    const float w = Elt.w;
    const float x = Elt.x;
    const float y = Elt.y;
    const float z = Elt.z;
    return nglVector3<T>(  1 - 2 * (y * y + z * z),
                           2 * (x * y + w * z),
                           2 * (x * z - w * y));
  }

};


/*
 * Specializations
 */

typedef nglQuaternion<float> nglQuaternionf;
typedef nglQuaternion<double> nglQuaterniond;

#endif // __nglQuaternion_h__
