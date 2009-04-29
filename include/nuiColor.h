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

  float& Red()   { return mRed;   }
  float& Green() { return mGreen; }
  float& Blue()  { return mBlue;  }
  float& Alpha() { return mAlpha; }
  
  float Red() const   { return mRed;   }
  float Green() const { return mGreen; }
  float Blue() const  { return mBlue;  }
  float Alpha() const { return mAlpha; }

  void Set(float r=0.0f,float g=0.0f, float b=0.0f, float a=1.0f)
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

  void Mix(const nuiColor& rColor, float ratio)
  {
    mRed   *= ratio;
    mGreen *= ratio;
    mBlue  *= ratio;
    mAlpha *= ratio;
    
    ratio = 1.0f - ratio;
    
    mRed += rColor.mRed * ratio;
    mGreen += rColor.mGreen * ratio;
    mBlue += rColor.mBlue * ratio;
    mAlpha += rColor.mAlpha * ratio;
    
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
  
  void Apply() const
  {
    glColor4f(mRed,mGreen,mBlue,mAlpha);
  }

  bool operator==(const nuiColor& rColor) const
  {
    return mRed == rColor.mRed && mGreen == rColor.mGreen && mBlue == rColor.mBlue && mAlpha == rColor.mAlpha;
  }

  uint32 GetRGBA() const;
  void SetHSV(float h, float s, float v, float a = 1.0f); ///< all parameters are [0, 1]
  void GetHSV(float& h, float& s, float& v) const;        ///< all parameters are [0, 1] except when h i undefined (r = g = b = 0), in this case h = -1
  void SetHSL(float h, float s, float v, float a = 1.0f); ///< all parameters are [0, 1]
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
