/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiFixedPoint.h"

#ifdef _WIN32_
#define NUI_RGBA_ENDIANSAFE_R 2
#define NUI_RGBA_ENDIANSAFE_G 1
#define NUI_RGBA_ENDIANSAFE_B 0
#define NUI_RGBA_ENDIANSAFE_A 3
#elif defined _CARBON_
#define NUI_RGBA_ENDIANSAFE_A 3
#define NUI_RGBA_ENDIANSAFE_R 2
#define NUI_RGBA_ENDIANSAFE_G 1
#define NUI_RGBA_ENDIANSAFE_B 0
#else
#define NUI_RGBA_ENDIANSAFE_R 0
#define NUI_RGBA_ENDIANSAFE_G 1
#define NUI_RGBA_ENDIANSAFE_B 2
#define NUI_RGBA_ENDIANSAFE_A 3
#endif

#if 0
inline uint32 NUI_RGBA(uint8 R, uint8 G, uint8 B, uint8 A)
{
  uint32 c;
  uint8* pC = (uint8*)&c;
  //  return  ( (((uint32)R) << 24) + (((uint32)G) << 16) + (((uint32)B) << 8) + ((uint32)A) );
  pC[NUI_RGBA_ENDIANSAFE_R] = R;
  pC[NUI_RGBA_ENDIANSAFE_G] = G;
  pC[NUI_RGBA_ENDIANSAFE_B] = B;
  pC[NUI_RGBA_ENDIANSAFE_A] = A;
  return c;
}
#else
inline uint32 NUI_RGBA(uint8 R, uint8 G, uint8 B, uint8 A)
{
#ifdef _WIN32_
  return  ( (((uint32)R) << 24) + (((uint32)G) << 16) + (((uint32)B) << 8) + ((uint32)A) );
#else
  return  ( (((uint32)A) << 24) + (((uint32)R) << 16) + (((uint32)G) << 8) + ((uint32)B) );
#endif
}
#endif

inline uint32 NUI_RGBA_F(float R, float G, float B, float A)
{
  const uint8 r = ToNearest(255.f * R);
  const uint8 g = ToNearest(255.f * G);
  const uint8 b = ToNearest(255.f * B);
  const uint8 a = ToNearest(255.f * A);
  return NUI_RGBA(r, g, b, a);
}

inline uint32 NUI_RGBA(const nuiColor& rColor)
{
  const uint8 r = ToNearest(255.f * rColor.Red());
  const uint8 g = ToNearest(255.f * rColor.Green());
  const uint8 b = ToNearest(255.f * rColor.Blue());
  const uint8 a = ToNearest(255.f * rColor.Alpha());
  return NUI_RGBA(r, g, b, a);
}


class nuiSolidColor
{
  uint32 mColor;
public:
  nuiSolidColor(uint8 r, uint8 g, uint8 b, uint8 a)
  {
    mColor = NUI_RGBA(r, g, b, a);
  }

  nuiSolidColor(const nuiColor& rColor)
  {
    mColor = NUI_RGBA(rColor);
  }
  
  nuiSolidColor(uint32 rgba = 0x000000ff)
  {
    mColor = rgba;
  }
  
  nuiSolidColor(const nuiSolidColor& rColor)
  {
    mColor = rColor.mColor;
  }
  
  nuiSolidColor& operator=(const nuiSolidColor& rColor)
  {
    mColor = rColor.mColor;
    
    return *this;
  }  
  
  uint32 GetColor() const
  {
    return mColor;
  }
  
  void Add(const nuiSolidColor& rColor)
  {
  }

  void Sub(const nuiSolidColor& rColor)
  {
  }
  
  void Mul(ifp32 divisor)
  {
  }
  
  void Div(ifp32 divisor)
  {
  }

  void Clear()
  {
  }
  
  bool IsStable()
  {
    return true;
  }
};

template <class Source0, class Source1>
class nuiModulatedColor
{
protected:
  Source0 mSrc0;
  Source1 mSrc1;
public:
  nuiModulatedColor()
  {
  }
  
  nuiModulatedColor(const Source0& rSrc0, const Source1& rSrc1)
  : mSrc0(rSrc0),
    mSrc1(rSrc1)
  {
  }
  
  nuiModulatedColor& operator=(const nuiModulatedColor& rColor)
  {
    mSrc0 = rColor.mSrc0;
    mSrc1 = rColor.mSrc1;
    
    return *this;
  }  
  
  uint32 GetColor() const
  {
    const uint32 col0 = mSrc0.GetColor();
    const uint32 col1 = mSrc1.GetColor();
    uint32 col = 0;
    
    const uint8* pCol0 = (const uint8*)&col0;
    const uint8* pCol1 = (const uint8*)&col1;
    uint8* pCol = (uint8*)&col;
    
    pCol[0] = (pCol0[0] * pCol1[0]) >> 8;
    pCol[1] = (pCol0[1] * pCol1[1]) >> 8;
    pCol[2] = (pCol0[2] * pCol1[2]) >> 8;
    pCol[3] = (pCol0[3] * pCol1[3]) >> 8;
    
    return col;
  }
  
  void Add(const nuiModulatedColor& rColor)
  {
    mSrc0.Add(rColor.mSrc0);
    mSrc1.Add(rColor.mSrc1);
  }
  
  void Sub(const nuiModulatedColor& rColor)
  {
    mSrc0.Sub(rColor.mSrc0);
    mSrc1.Sub(rColor.mSrc1);
  }
  
  void Mul(ifp32 multiplicator)
  {
    mSrc0.Mul(multiplicator);
    mSrc1.Mul(multiplicator);
  }
  
  void Div(ifp32 divisor)
  {
    mSrc0.Div(divisor);
    mSrc1.Div(divisor);
  }
  
  void Clear()
  {
    mSrc0.Clear();
    mSrc1.Clear();
  }
  
  bool IsStable()
  {
    return mSrc0.IsStable() && mSrc1.IsStable();
  }
};



class nuiGouraudColor
{
  int32 mR, mG, mB, mA;
public:
  nuiGouraudColor()
  {
    Clear();
  }
  
  nuiGouraudColor(uint8 r, uint8 g, uint8 b, uint8 a)
  {
    mR = ((uint32)r) << NUI_FP_SHIFT;
    mG = ((uint32)g) << NUI_FP_SHIFT;
    mB = ((uint32)b) << NUI_FP_SHIFT;
    mA = ((uint32)a) << NUI_FP_SHIFT;
  }
  
  nuiGouraudColor(const nuiGouraudColor& rColor)
  {
    mR = rColor.mR;
    mG = rColor.mG;
    mB = rColor.mB;
    mA = rColor.mA;
  }
  
  nuiGouraudColor(const nuiColor& rColor)
  {
    mR = ToNearest(rColor.Red() * 255.f) << NUI_FP_SHIFT;
    mG = ToNearest(rColor.Green() * 255.f) << NUI_FP_SHIFT;
    mB = ToNearest(rColor.Blue() * 255.f) << NUI_FP_SHIFT;
    mA = ToNearest(rColor.Alpha() * 255.f) << NUI_FP_SHIFT;
  }
  
  nuiGouraudColor& operator=(const nuiGouraudColor& rColor)
  {
    mR = rColor.mR;
    mG = rColor.mG;
    mB = rColor.mB;
    mA = rColor.mA;
    
    return *this;
  }  
  
  inline uint32 GetColor() const
  {
    return NUI_RGBA(ToNearest(mR), ToNearest(mG), ToNearest(mB), ToNearest(mA));
//    return NUI_RGBA(64, 64, 128, 128);
  }
  
  void Add(const nuiGouraudColor& rColor)
  {
    mR += rColor.mR;
    mG += rColor.mG;
    mB += rColor.mB;
    mA += rColor.mA;
  }
  
  void Sub(const nuiGouraudColor& rColor)
  {
    mR -= rColor.mR;
    mG -= rColor.mG;
    mB -= rColor.mB;
    mA -= rColor.mA;
  }
  
  void Mul(ifp32 multiplicator)
  {
    mR = nuiFPMul(mR, multiplicator);
    mG = nuiFPMul(mG, multiplicator);
    mB = nuiFPMul(mB, multiplicator);
    mA = nuiFPMul(mA, multiplicator);
  }
  
  void Div(ifp32 divisor)
  {
    mR = nuiFPDiv(mR, divisor);
    mG = nuiFPDiv(mG, divisor);
    mB = nuiFPDiv(mB, divisor);
    mA = nuiFPDiv(mA, divisor);
  }

  void Clear()
  {
    mR = 0;
    mG = 0;
    mB = 0;
    mA = 0;
  }
  
  bool IsStable()
  {
    return (mR == 0) && (mG == 0) && (mB == 0) && (mA == 0);
  }
};

class nuiTexelAccessor_Lum
{
public:
  inline static uint32 GetTexelColor(nuiTexture* mpTexture, uint8* pBuffer, int32 width, int32 height, ifp32 U, ifp32 V)
  {
    U = ToAbove(U);
    V = ToAbove(V);
    const int32 index = (U + width * V);
    const uint8 lum = pBuffer[index];
    return NUI_RGBA(lum, lum, lum, 255);
  }
};

class nuiTexelAccessor_Alpha
{
public:
  static uint32 GetTexelColor(nuiTexture* mpTexture, uint8* pBuffer, int32 width, int32 height, ifp32 U, ifp32 V)
  {
    U = ToAbove(U);
    V = ToAbove(V);
    const int32 index = (U + width * V);
    const uint8 alpha = pBuffer[index];
    return NUI_RGBA(255, 255, 255, alpha);
  }
};

class nuiTexelAccessor_LumA
{
public:
  inline static uint32 GetTexelColor(nuiTexture* mpTexture, uint8* pBuffer, int32 width, int32 height, ifp32 U, ifp32 V)
  {
    U = ToAbove(U);
    V = ToAbove(V);
    const int32 index = 2 * (U + width * V);
    const uint8 lum = pBuffer[index];
    const uint8 alpha = pBuffer[index + 1];
    return NUI_RGBA(lum, lum, lum, alpha);
  }
};

class nuiTexelAccessor_RGB24
{
public:
  inline static uint32 GetTexelColor(nuiTexture* mpTexture, uint8* pBuffer, int32 width, int32 height, ifp32 U, ifp32 V)
  {
    U = ToAbove(U);
    V = ToAbove(V);
    const int32 index = 3 * (U + width * V);
    const uint8 r = pBuffer[index];
    const uint8 g = pBuffer[index + 1];
    const uint8 b = pBuffer[index + 2];
    return NUI_RGBA(r, g, b, 255);
  }
};

class nuiTexelAccessor_RGBA32
{
public:
  inline static uint32 GetTexelColor(nuiTexture* mpTexture, uint8* pBuffer, int32 width, int32 height, ifp32 U, ifp32 V)
  {
    U = ToAbove(U);
    V = ToAbove(V);
    if (U < 0)
      U = 0;
    if (V < 0)
      V = 0;
    if (U >= width)
      U = width - 1;
    if (V >= height)
      V = height - 1;
    const int32 index = 4 * (U + width * V);
    const uint8 r = pBuffer[index];
    const uint8 g = pBuffer[index + 1];
    const uint8 b = pBuffer[index + 2];
    const uint8 a = pBuffer[index + 3];
    return NUI_RGBA(r, g, b, a);
  }
};


template <class TexelAccessor>
class nuiTexelColor
{
  nuiTexture* mpTexture;
  int32 mU, mV;
  uint8* mpBuffer;
  int32 mWidth;
  int32 mHeight;
public:
  nuiTexelColor()
  {
    Clear();
  }
  
  nuiTexelColor(nuiTexture* pTexture, float u, float v)
  {
    mpTexture = pTexture;
    nglImage* pImage = mpTexture->GetImage();
    mpBuffer = (uint8*)pImage->GetBuffer();
    mWidth = pImage->GetWidth();
    mHeight = pImage->GetHeight();
    
    mU = (ToNearest(u)) << NUI_FP_SHIFT;
    mV = (ToNearest(v)) << NUI_FP_SHIFT;
  }
    
  nuiTexelColor& operator=(const nuiTexelColor& rTexel)
  {
    mpTexture = rTexel.mpTexture;
    mU = rTexel.mU;
    mV = rTexel.mV;
    return *this;
  }  
  
  inline uint32 GetColor() const
  {
    //return NUI_RGBA(ToNearest(mR), ToNearest(mG), ToNearest(mB), ToNearest(mA));
    return TexelAccessor::GetTexelColor(mpTexture, mpBuffer, mWidth, mHeight, mU, mV);
  }
  
  void Add(const nuiTexelColor& rTexel)
  {
    mU += rTexel.mU;
    mV += rTexel.mV;
  }
  
  void Sub(const nuiTexelColor& rTexel)
  {
    mU -= rTexel.mU;
    mV -= rTexel.mV;
  }
  
  void Mul(ifp32 multiplicator)
  {
    mU = nuiFPMul(mU, multiplicator);
    mV = nuiFPMul(mV, multiplicator);
  }
  
  void Div(ifp32 divisor)
  {
    mU = nuiFPDiv(mU, divisor);
    mV = nuiFPDiv(mV, divisor);
  }
  
  void Clear()
  {
    mU = 0;
    mV = 0;
  }
  
  bool IsStable()
  {
    return (mU == 0) && (mV == 0);
  }
};



template <class InterpolatedType>
class nuiVertex
{
private:
  InterpolatedType mValue;
public:
  nuiVertex()
  {
  }
  
  nuiVertex(ifp32 x, ifp32 y, const InterpolatedType& rValue)
  : mValue(rValue)
  {
    mX = x << NUI_FP_SHIFT;
    mY = y << NUI_FP_SHIFT;
  }
  
  nuiVertex(nuiSize x, nuiSize y, const InterpolatedType& rValue)
  : mValue(rValue)
  {
    mX = ToNearest(x * (nuiSize)NUI_FP_ONE);
    mY = ToNearest(y * (nuiSize)NUI_FP_ONE);
  }
  
  nuiVertex(const nuiVertex<InterpolatedType>& rVertex)
  : mValue(rVertex.mValue)
  {
    mX = rVertex.mX;
    mY = rVertex.mY;
  }
  
  nuiVertex<InterpolatedType>& operator=(const nuiVertex<InterpolatedType>& rVertex)
  {
    mX = rVertex.mX;
    mY = rVertex.mY;
    
    mValue = rVertex.mValue;
    
    return *this;
  }
  
  ifp32 X() const
  {
    return mX;
  }
  
  ifp32 Y() const
  {
    return mY;
  }
  
  void SetX(ifp32 x)
  {
    mX = x;
  }
  
  void SetY(ifp32 y)
  {
    mY = y;
  }
  
  ifp32 CeiledY() const
  {
    return ToAbove(mY);
  }
  
  void Add(const nuiVertex<InterpolatedType>& rVertex)
  {
    mX += rVertex.mX;
    mY += rVertex.mY;
    
    mValue.Add(rVertex.mValue);
  }
  
  void Sub(const nuiVertex<InterpolatedType>& rVertex)
  {
    mX -= rVertex.mX;
    mY -= rVertex.mY;
    
    mValue.Sub(rVertex.mValue);
  }
  
  void Mul(ifp32 i)
  {
    mX = nuiFPMul(mX, i);
    mY = nuiFPMul(mY, i);
    
    mValue.Mul(i);
  }
  
  void Div(ifp32 i)
  {
    mX = nuiFPDiv(mX, i);
    mY = nuiFPDiv(mY, i);
    
    mValue.Div(i);
  }
  
  void Clear()
  {
    mX = 0;
    mY = 0;

    mValue.Clear();
  }

  uint32 GetColor()
  {
    return mValue.GetColor();
  }
  
  void PrestepY(const nuiVertex<InterpolatedType>& rIncr)
  {
    const int32 f = Frac(mY);
    const int32 ff = NUI_FP_MASK - f;
    
    //if (f)
    {
      mY = Trunc(mY);
      mX += nuiFPMul(ff, rIncr.mX);

      InterpolatedType t = rIncr.mValue;
      t.Mul(ff);
      mValue.Add(t);
    }
  }
  
  void PrestepX(const nuiVertex<InterpolatedType>& rIncr)
  {
    const int32 f = Frac(rIncr.mX);
    const int32 ff = NUI_FP_MASK - f;
    
    mX += ff;
    
    InterpolatedType t(rIncr.mValue);
    t.Mul(ff);
    mValue.Add(t);
  }
  
  
  ifp32 GetIncrToX(const nuiVertex<InterpolatedType>& rVertex, nuiVertex<InterpolatedType>& rResultVertex) const
  {
    const ifp32 _count = rVertex.mX - mX;
    
    if (_count > 0)
    {
      rResultVertex.mX = NUI_FP_ONE;
      rResultVertex.mY = 0;
      
      rResultVertex.mValue = rVertex.mValue;
      rResultVertex.mValue.Sub(mValue);
      rResultVertex.mValue.Div(_count);
    }
    else
    {
      rResultVertex.mX = NUI_FP_ONE;
      rResultVertex.mY = 0;

      rResultVertex.mValue.Clear();
    }
    
    return ToAbove(rVertex.mX) - ToAbove(mX);
  }
  
  ifp32 GetIncrToY(const nuiVertex<InterpolatedType>& rVertex, nuiVertex<InterpolatedType>& rResultVertex) const
  {
    const ifp32 _count = rVertex.mY - mY;
    
    if (_count > 0)
    {
      rResultVertex.mX = nuiFPDiv((rVertex.mX - mX), _count);
      rResultVertex.mY = NUI_FP_ONE;

      rResultVertex.mValue = rVertex.mValue;
      rResultVertex.mValue.Sub(mValue);
      rResultVertex.mValue.Div(_count);
    }
    else
    {
      rResultVertex.mX = 0;
      rResultVertex.mY = NUI_FP_ONE;

      rResultVertex.mValue.Clear();
    }
    
    return ToAbove(rVertex.mY) - ToAbove(mY);
  }
  
  bool IsStable()
  {
    return mValue.IsStable();
  }
  
  
private:
  ifp32 mX;
  ifp32 mY;
};



////////////////////

typedef nuiVertex<nuiSolidColor> nuiVertex_Solid;
typedef nuiVertex<nuiGouraudColor> nuiVertex_Gouraud;


template <class VertexType>
bool nuiSortVertexPointers(const VertexType& rV0, const VertexType& rV1)
{
  const int32 y0 = rV0.Y();
  const int32 y1 = rV1.Y();

  if (y0 < y1)
    return true;
  if (y0 > y1)
    return false;
  
  return rV0.X() < rV1.X();
}

