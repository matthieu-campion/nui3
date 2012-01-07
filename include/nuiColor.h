/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiColor_h__
#define __nuiColor_h__

//#include "nui.h"
#include "nglString.h"

/// This class implements a basic RGB color management class.
class NUI_API nuiColor
{
public:
  explicit nuiColor(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f, bool Premultiplied = false)
  {
    if (Premultiplied)
      SetP(r, g, b, a);
    else
      SetU(r,g,b,a);
  }

  explicit nuiColor(uint8 r, uint8 g, uint8 b, uint8 a = 255, bool Premultiplied = false)
  {
    static const float ratio = 1.0f/255.0f;
    if (Premultiplied)
      SetP(r*ratio, g*ratio, b*ratio, a*ratio);
    else
      SetU(r*ratio, g*ratio, b*ratio, a*ratio);
  }

  explicit nuiColor(int r, int g, int b, int a = 255, bool Premultiplied = false)
  {
    static const float ratio = 1.0f/255.0f;
    if (Premultiplied)
      SetP(r*ratio, g*ratio, b*ratio, a*ratio);
    else
      SetU(r*ratio, g*ratio, b*ratio, a*ratio);
  }

  nuiColor(const nuiColor& rColor)
  {
    mRed   = rColor.mRed;
    mGreen = rColor.mGreen;
    mBlue  = rColor.mBlue;
    mAlpha = rColor.mAlpha;
  }

  nuiColor(const nglString& rString)
  {
    SetValue(rString);
  }

  nuiColor(const char* pString)
  {
    SetValue(nglString(pString));
  }

  ~nuiColor()
  {
  }

  void SetRed(float v)    { mRed = v;   }
  void SetGreen(float v)  { mGreen = v; }
  void SetBlue(float v)   { mBlue = v;  }
  void SetAlpha(float v)  { mAlpha = v; }
  
  float Red() const   { return mRed;   }
  float Green() const { return mGreen; }
  float Blue() const  { return mBlue;  }
  float Alpha() const { return mAlpha; }

  void SetOpacity(float v);
  
  void Set(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f, bool Premultiplied = false)
  {
    if (Premultiplied)
      SetP(r, g, b, a);
    else
      SetU(r, g, b, a);
  }

  void SetU(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f) ///< Set Un premultiplied alpha (the resulting argument color components will be premultiplied)
  {
    mRed   = r * a;
    mGreen = g * a;
    mBlue  = b * a;
    mAlpha = a;
  }

  void SetP(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f) ///< Set Premultiplied alpha. The argument color components will not be changed.
  {
    mRed   = r;
    mGreen = g;
    mBlue  = b;
    mAlpha = a;
  }
  
  void Premultiply()
  {
    mRed    *= mAlpha;
    mGreen  *= mAlpha;
    mBlue   *= mAlpha;
  }
  
  void UnPremultiply()
  {
    if (mAlpha > 0.0f)
    {
      const float ratio = 1.0f / mAlpha;
      mRed    *= ratio;
      mGreen  *= ratio;
      mBlue   *= ratio;
    }
    else
    {
      mRed    = 0;
      mGreen  = 0;
      mBlue   = 0;
    }
    
  }
  
  void Crop();

  void Saturate(const nuiColor& rColor, bool WithAlpha = true)
  {
    mRed   += mRed   + rColor.Red  ();
    mGreen += mGreen + rColor.Green();
    mBlue  += mBlue  + rColor.Blue ();
    if (WithAlpha)  
      mAlpha += mAlpha + rColor.Alpha();

    Crop();
  }

  void Multiply(float Factor, bool WithAlpha = true)
  {
    mRed   *= Factor;
    mGreen *= Factor;
    mBlue  *= Factor;
    if (WithAlpha)  
      mAlpha *= Factor;

    Crop();
  }

  void Multiply(const nuiColor& rFactor, bool WithAlpha = true)
  {
    mRed   *= rFactor.mRed;
    mGreen *= rFactor.mGreen;
    mBlue  *= rFactor.mBlue;
    if (WithAlpha)  
      mAlpha *= rFactor.mAlpha;

    Crop();
  }

  void Mix(const nuiColor& rColor, float ratio)
  {
    mRed   *= ratio;
    mGreen *= ratio;
    mBlue  *= ratio;
    mAlpha *= ratio;
    
    ratio = 1.0f - ratio;
    
    mRed    += rColor.mRed * ratio;
    mGreen  += rColor.mGreen * ratio;
    mBlue   += rColor.mBlue * ratio;
    mAlpha  += rColor.mAlpha * ratio;
    
    Crop();
  }
  
  
  void Add(float Value, bool WithAlpha = true)
  {
    mRed   += Value;
    mGreen += Value;
    mBlue  += Value;
    if (WithAlpha)  
      mAlpha += Value;

    Crop();
  }

  bool SetValue(const nglString& rString);
  void Get(nglString& rString) const;
  nglString GetValue() const;
  
  bool operator==(const nuiColor& rColor) const
  {
    return mRed == rColor.mRed && mGreen == rColor.mGreen && mBlue == rColor.mBlue && mAlpha == rColor.mAlpha;
  }

  uint32 GetRGBA() const;
  void SetHSV(float h,  float s,  float v, float a = 1.0f); ///< all parameters are [0, 1]
  void GetHSV(float& h, float& s, float& v) const;        ///< all parameters are [0, 1] except when h i undefined (r = g = b = 0), in this case h = -1
  void SetHSL(float h,  float s,  float v, float a = 1.0f); ///< all parameters are [0, 1]
  void GetHSL(float& h, float& s, float& v) const;        ///< all parameters are [0, 1]
  
  static void EnumStandardColorNames(std::vector<nglString>& rStandardColorNames);
  static void EnumCustomizedColorNames(std::vector<nglString>& rUserColorNames);
  
  static void SetColor(const nglString& rName, const nuiColor& rColor);
  static bool GetColor(const nglString& rName, nuiColor& rColor);
protected:
  float mRed, 
        mGreen, 
        mBlue,
        mAlpha;
  
  typedef std::map<nglString, nuiColor, nglString::LessFunctor> ColorMap;
  static ColorMap mCustomColors;
};

#endif // __nuiColor_h__
