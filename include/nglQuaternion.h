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

  nglQuaternion(T x, T y, T z, T w)
  {
    Elt.x = x;
    Elt.y = y;
    Elt.z = z;
    Elt.w = w;
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

  void SetRotation(const nglVector<T>& from, const nglVector<T>& to)
  {
    nglVector<T> H = from + to;
    H.Normalize();

    Elt.w = from * H;
    Elt.x = from.Elt.y * H.Elt.z - from.Elt.z * H.Elt.y;
    Elt.y = from.Elt.z * H.Elt.x - from.Elt.x * H.Elt.z;
    Elt.z = from.Elt.x * H.Elt.y - from.Elt.y * H.Elt.x;
  }

	void AimZAxis( const nglVector<T>& from, const nglVector<T>& to )
	{
		nglVector<T> vAim = to - from;
		vAim.Normalize();

		Elt.x = vAim.Elt.y;
		Elt.y = -vAim.Elt.x;
		Elt.z = 0.0f;
		Elt.w = 1.0f + vAim.Elt.z;

		if ( Elt.x == 0.0f && Elt.y == 0.0f && Elt.z == 0.0f && Elt.w == 0.0f )
    {
      // If we can't normalize it, just set it
			Elt.x = 0;
      Elt.y = 1;
      Elt.z = 0;
      Elt.w = 0;
		}
    else
    {
			Normalize();
		}
	}

  void LookAt(nglVector<T>& lookAt, nglVector<T>& upDirection)
  {
    nglMatrix<T> m;
    m.LookAt(nglVector<T>(0, 0, 0, 1), lookAt, upDirection);
    *this = m;
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

  void GetEulerAngles(T& x, T& y, T& z) const
  {
    T test = Elt.x * Elt.y + Elt.z * Elt.w;

    if (test > 0.499)
    { // singularity at north pole
      x = RAD2DEG(2.0 * atan2(Elt.x, Elt.w));
      y = RAD2DEG(M_PI / 2.0);
      z = 0;
      return;
    }

    if (test < -0.499)
    { // singularity at south pole
      x = RAD2DEG(-2 * atan2(Elt.x,Elt.w));
      y = RAD2DEG(- M_PI/2);
      z = 0;
      return;
    }
    T sqx = Elt.x * Elt.x;
    T sqy = Elt.y * Elt.y;
    T sqz = Elt.z * Elt.z;
    x = RAD2DEG(atan2(2 * Elt.y * Elt.w - 2 * Elt.x * Elt.z , 1 - 2 * sqy - 2 * sqz));
    y = RAD2DEG(asin(2 * test));
    z = RAD2DEG(atan2(2 * Elt.x * Elt.w - 2 * Elt.y * Elt.z , 1 - 2 * sqx - 2 * sqz));
  }

  
  void SetSphericalRotation(T latitude, T longitude, T angle = 0)
  {
    latitude = DEG2RAD(latitude);
    longitude = DEG2RAD(longitude);

    T sin_a    = sin( angle / 2 );
    T cos_a    = cos( angle / 2 );

    T sin_lat  = sin( latitude );
    T cos_lat  = cos( latitude );

    T sin_long = sin( longitude );
    T cos_long = cos( longitude );

    Elt.x       = sin_a * cos_lat * sin_long;
    Elt.y       = sin_a * sin_lat;
    Elt.z       = sin_a * sin_lat * cos_long;
    Elt.w       = cos_a;
  }

  void GetShericalRotation(T& latitude, T& longitude)
  {
    T cos_angle  = Elt.w;
    T sin_angle  = sqrt( 1.0 - cos_angle * cos_angle );
    T angle      = RAD2DEG(acos( cos_angle ) * 2);

    if ( fabs( sin_angle ) < 0.0005 )
      sin_angle = 1;

    T tx = Elt.x / sin_angle;
    T ty = Elt.y / sin_angle;
    T tz = Elt.z / sin_angle;

    latitude = -asin( ty );

    if ( tx * tx + tz * tz < 0.0005 )
      longitude   = 0;
    else
      longitude  = atan2( tx, tz );

    latitude = RAD2DEG(latitude);
    longitude = RAD2DEG(longitude);

    while (longitude < 0)
      longitude += 360.0;
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
  nglQuaternion<T>& operator =(const nglMatrix<T>& a)
  {
    float trace = a(0,0) + a(1,1) + a(2,2); // I removed + 1.0f; see discussion with Ethan
    if( trace > 0 )
    {
      T s = 0.5 / sqrt(trace + 1.0);
      Elt.w = 0.25f / s;
      Elt.x = ( a(2,1) - a(1,2) ) * s;
      Elt.y = ( a(0,2) - a(2,0) ) * s;
      Elt.z = ( a(1,0) - a(0,1) ) * s;
    }
    else
    {
      if ( a(0,0) > a(1,1) && a(0,0) > a(2,2) )
      {
        T s = 2.0f * sqrt( 1.0f + a(0,0) - a(1,1) - a(2,2));
        Elt.w = (a(2,1) - a(1,2) ) / s;
        Elt.x = 0.25f * s;
        Elt.y = (a(0,1) + a(1,0) ) / s;
        Elt.z = (a(0,2) + a(2,0) ) / s;
      }
      else if (a(1,1) > a(2,2))
      {
        T s = 2.0f * sqrt( 1.0f + a(1,1) - a(0,0) - a(2,2));
        Elt.w = (a(0,2) - a(2,0) ) / s;
        Elt.x = (a(0,1) + a(1,0) ) / s;
        Elt.y = 0.25f * s;
        Elt.z = (a(1,2) + a(2,1) ) / s;
      }
      else
      {
        T s = 2.0f * sqrt( 1.0f + a(2,2) - a(0,0) - a(1,1) );
        Elt.w = (a(1,0) - a(0,1) ) / s;
        Elt.x = (a(0,2) + a(2,0) ) / s;
        Elt.y = (a(1,2) + a(2,1) ) / s;
        Elt.z = 0.25f * s;
      }
    }
  }

  /// Conversion to matrix
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

  nglQuaternion<T> NlerpTo(const nglQuaternion<T>& f, T blend)
  {
    const nglQuaternion& i(*this);
    nglQuaternion<T> result;
    T dot = i.Elt.w * f.Elt.w + i.Elt.x * f.Elt.x + i.Elt.y * f.Elt.y + i.Elt.z * f.Elt.z;
    T blendI = 1.0f - blend;

    if (dot < 0.0f)
    {
      nglQuaternion<T> tmpF;
      tmpF.Elt.w = -f.Elt.w;
      tmpF.Elt.x = -f.Elt.x;
      tmpF.Elt.y = -f.Elt.y;
      tmpF.Elt.z = -f.Elt.z;

      result.Elt.w = blendI * i.Elt.w + blend * tmpF.Elt.w;
      result.Elt.x = blendI * i.Elt.x + blend * tmpF.Elt.x;
      result.Elt.y = blendI * i.Elt.y + blend * tmpF.Elt.y;
      result.Elt.z = blendI * i.Elt.z + blend * tmpF.Elt.z;
    }
    else
    {
      result.Elt.w = blendI * i.Elt.w + blend * f.Elt.w;
      result.Elt.x = blendI * i.Elt.x + blend * f.Elt.x;
      result.Elt.y = blendI * i.Elt.y + blend * f.Elt.y;
      result.Elt.z = blendI * i.Elt.z + blend * f.Elt.z;
    }

    result.Normalize();
    return result;
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

  void Transform(nglVector<T> &v) const
  {
    nglQuaternion<T> qv;
    qv.Elt.w = 0;
    qv.Elt.x = v[0];
    qv.Elt.y = v[1];
    qv.Elt.z = v[2];

    nglQuaternion<T> conj = *this;
    conj.Invert();
    nglQuaternion<T> result = *this * qv * conj;

    v[0] = result.Elt.x;
    v[1] = result.Elt.y;
    v[2] = result.Elt.z;
  }
  
  void RotateAroundX(float angle)
  {
    RotateAround(angle, nglVector3f(1, 0, 0));
  }

  void RotateAroundY(float angle)
  {
    RotateAround(angle, nglVector3f(0, 1, 0));
  }

  void RotateAroundZ(float angle)
  {
    RotateAround(angle, nglVector3f(0, 0, 1));
  }

  void RotateAround(float angle, const nglVector3f& rAxis)
  {
    nglQuaternion<T> rotation;
    nglVector3f v(rAxis);
    v.Normalize();
    rotation.SetRotation(angle, v);

    *this = *this * rotation;
  }
  

};


/*
 * Specializations
 */

typedef nglQuaternion<float> nglQuaternionf;
typedef nglQuaternion<double> nglQuaterniond;

#endif // __nglQuaternion_h__
