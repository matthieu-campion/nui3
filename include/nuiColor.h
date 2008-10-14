/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiColor_h__
#define __nuiColor_h__

//#include "nui.h"
#include "nglString.h"
#include <limits>
#include <algorithm>

/// This class implements a basic RGB color management class.
class NUI_API nuiColor
{
public:
  explicit nuiColor(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f)
  {
    Set(r,g,b,a);
  }

  explicit nuiColor(uint8 r, uint8 g, uint8 b, uint8 a = 255)
  {
    static const float ratio = 1.0f/255.0f;
    Set(r*ratio, g*ratio, b*ratio, a*ratio);
  }

  explicit nuiColor(int r, int g, int b, int a = 255)
  {
    static const float ratio = 1.0f/255.0f;
    Set(r*ratio, g*ratio, b*ratio, a*ratio);
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

  inline float& Red()   { return mRed;   }
  inline float& Green() { return mGreen; }
  inline float& Blue()  { return mBlue;  }
  inline float& Alpha() { return mAlpha; }
  
  inline float Red() const   { return mRed;   }
  inline float Green() const { return mGreen; }
  inline float Blue() const  { return mBlue;  }
  inline float Alpha() const { return mAlpha; }

  inline void Set(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f)
  {
    mRed   = r;
    mGreen = g;
    mBlue  = b;
    mAlpha = a;
  }

  void Crop();

  void Saturate(const nuiColor& rColor, bool WithAlpha = false)
  {
    mRed   += mRed   + rColor.Red  ();
    mGreen += mGreen + rColor.Green();
    mBlue  += mBlue  + rColor.Blue ();
    if (WithAlpha)  
      mAlpha += mAlpha + rColor.Alpha();

    Crop();
  }

  void Multiply(float Factor, bool WithAlpha = false)
  {
    mRed   *= Factor;
    mGreen *= Factor;
    mBlue  *= Factor;
    if (WithAlpha)  
      mAlpha *= Factor;

    Crop();
  }

  void Multiply(const nuiColor& rFactor, bool WithAlpha = false)
  {
    mRed   *= rFactor.mRed;
    mGreen *= rFactor.mGreen;
    mBlue  *= rFactor.mBlue;
    if (WithAlpha)  
      mAlpha *= rFactor.mAlpha;

    Crop();
  }

  void Add(float Value, bool WithAlpha = false)
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
  
  inline void Apply() const
  {
    glColor4f(mRed,mGreen,mBlue,mAlpha);
  }

  bool operator==(const nuiColor& rColor) const
  {
    return mRed == rColor.mRed && mGreen == rColor.mGreen && mBlue == rColor.mBlue && mAlpha == rColor.mAlpha;
  }

  uint32 GetRGBA() const;
  
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
