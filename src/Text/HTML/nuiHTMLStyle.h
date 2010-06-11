/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTML.h"

class nuiHTMLStyleValueBase : public nuiObject
{
public:
  nuiHTMLStyleValueBase(bool use = false);
  virtual ~nuiHTMLStyleValueBase();
  bool Use() const;
  void Use(bool set);
  
protected:
  bool mUse;
};


template <class ValueType> class nuiHTMLStyleValue : public nuiHTMLStyleValueBase
{
public:
  nuiHTMLStyleValue()
  {
    Init();
  }
  
  nuiHTMLStyleValue(const ValueType& rValue)
  : nuiHTMLStyleValueBase(true), mValue(rValue)
  {
    Init();
  }
  
  nuiHTMLStyleValue(const nuiHTMLStyleValue& rStyleValue)
  : nuiHTMLStyleValueBase(rStyleValue.Use()), mValue(rStyleValue.mValue)
  {
    Init();
  }
  
  void Init()
  {
    nglString Class(typeid(ValueType).name());
    
    if (SetObjectClass(_T("nuiHTMLStyleValue<") + Class + (_T(">"))))
    {
      
    }
  }
  
  const ValueType& GetValue() const
  {
    NGL_ASSERT(Use());
    return mValue;
  }
  
  void SetValue(const ValueType& rValue)
  {
    mValue = rValue;
    Use(true);
  }
  
protected:
  ValueType mValue;
};

class nuiHTMLStyle : public nuiObject
{
public:
  nuiHTMLStyle();
  nuiHTMLStyle(const nuiHTMLStyle& rStyle);
  virtual ~nuiHTMLStyle();
  
  void CopyFrom(const nuiHTMLStyle& rStyle);
  nuiHTMLStyle& operator=(const nuiHTMLStyle& rStyle);
  
  nuiHTMLStyleValue<nuiColor> mBackgroundColor;
  nuiHTMLStyleValue<nuiColor> mBorderColor;
  nuiHTMLStyleValue<nuiColor> mColor;

  nuiHTMLStyleValue<float> mBorderSize;

  nuiHTMLStyleValue<bool> mFontBold;
  nuiHTMLStyleValue<bool> mFontItalic;
  nuiHTMLStyleValue<bool> mFontUnderline;
  nuiHTMLStyleValue<bool> mFontStrike;
  nuiHTMLStyleValue<bool> mMonospace;
  nuiHTMLStyleValue<float> mFontSize;
  nuiHTMLStyleValue<nglString> mFontFamilyName;
  nuiHTMLStyleValue<nglString> mFontGenericName;
  nuiHTMLStyleValue<nglString> mFontStyle;
  
private:
  void Init();
};

