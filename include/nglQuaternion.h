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
  nglQuaternion ()
  {
    Nullify();
  }

  /// Create a quaternion from Euler angles in degrees (vector input)
  nglQuaternion (const nglVector<T>& rVector)
  {
    T x, y, z;

    x = (T) ( DEG2RAD(rVector[0]) / 2.f );
    y = (T) ( DEG2RAD(rVector[1]) / 2.f );
    z = (T) ( DEG2RAD(rVector[2]) / 2.f );

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

  nglQuaternion (const nglQuaternion<T>& rQuat)
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
    Elt.x = 0.f;
    Elt.y = 0.f;
    Elt.z = 0.f;
    Elt.w = 0.f;
  }

  /// Set to (multiplication) identity
  void SetIdentity()
  {
    Elt.x = 0.f;
    Elt.y = 0.f;
    Elt.z = 0.f;
    Elt.w = 1.f;
  }

  /*! Set to a rotation representation
    \param Angle rotation angle, in degrees
    \param X axis X coordinate
    \param Y axis Y coordinate
    \param Z axis Z coordinate
  */
  void SetRotation (const T Angle, const T X, const T Y, const T Z)
  {
    T angle_rad = (T) (DEG2RAD(Angle) / 2.f);

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
      rAxis[0] = -Elt.z * m;
      rAxis[1] = -Elt.y * m;
      rAxis[2] = -Elt.x * m;
    }

    return angle;
  }

  /// Multiplication
  nglQuaternion<T> operator * (const nglQuaternion<T>& rQuat) const
  {
    nglQuaternion<T> result;
    T t[8];

    t[0] = (Elt.w + Elt.x) * (rQuat.Elt.w + rQuat.Elt.x);
    t[1] = (Elt.z - Elt.y) * (rQuat.Elt.y - rQuat.Elt.z);
    t[2] = (Elt.x - Elt.w) * (rQuat.Elt.y - rQuat.Elt.z);
    t[3] = (Elt.y + Elt.z) * (rQuat.Elt.x - rQuat.Elt.w);
    t[4] = (Elt.x + Elt.z) * (rQuat.Elt.x + rQuat.Elt.y);
    t[5] = (Elt.x - Elt.z) * (rQuat.Elt.x - rQuat.Elt.y);
    t[6] = (Elt.w + Elt.y) * (rQuat.Elt.w - rQuat.Elt.z);
    t[7] = (Elt.w - Elt.y) * (rQuat.Elt.w + rQuat.Elt.z);

    result.Elt.w =  t[1] + ((-t[4] - t[5] + t[6] + t[7]) * (T)0.5);
    result.Elt.x =  t[0] - (( t[4] + t[5] + t[6] + t[7]) * (T)0.5);
    result.Elt.y = -t[2] + (( t[4] - t[5] + t[6] - t[7]) * (T)0.5);
    result.Elt.z = -t[3] + (( t[4] - t[5] - t[6] + t[7]) * (T)0.5);

    return result;
  }

  /// Conversion from an homogeneous matrix
  nglQuaternion<T>& operator = (const nglMatrix<T>& rMatrix)
  {
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
  operator nglMatrix<T> () const
  {
    nglMatrix<T> result;

    T two_xx = Elt.x * (Elt.x + Elt.x);
    T two_xy = Elt.x * (Elt.y + Elt.y);
    T two_xz = Elt.x * (Elt.z + Elt.z);

    T two_wx = Elt.w * (Elt.x + Elt.x);
    T two_wy = Elt.w * (Elt.y + Elt.y);
    T two_wz = Elt.w * (Elt.z + Elt.z);

    T two_yy = Elt.y * (Elt.y + Elt.y);
    T two_yz = Elt.y * (Elt.z + Elt.z);

    T two_zz = Elt.z * (Elt.z + Elt.z);

    result.Array[0]= 1.f - (two_yy+two_zz);
    result.Array[1]= two_xy - two_wz;
    result.Array[2]= two_xz + two_wy;
    result.Array[3]= 0.f;

    result.Array[4]= two_xy + two_wz;
    result.Array[5]= 1.f - (two_xx + two_zz);
    result.Array[6]= two_yz - two_wx;
    result.Array[7]= 0.f;

    result.Array[8]= two_xz - two_wy;
    result.Array[9]= two_yz + two_wx;
    result.Array[10]= 1.f - (two_xx + two_yy);
    result.Array[11]= 0.f;

    result.Array[12]= 0.f;
    result.Array[13]= 0.f;
    result.Array[14]= 0.f;
    result.Array[15]= 1.f;

    return result;
  }

  /// Conversion to vector (x, y and z quaternion components are pasted, w is zero)
  operator nglVector<T> () const
  {
    return ::nglVector<T>(Elt.x, Elt.y, Elt.z, 0.f);
  }

  /// Inversion
  void Invert()
  {
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
  

};


/*
 * Specializations
 */

typedef nglQuaternion<float> nglQuaternionf;
typedef nglQuaternion<double> nglQuaterniond;

#endif // __nglQuaternion_h__
