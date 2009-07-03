/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiParser;

class nuiWebCSS
{
  nuiWebCSS();
  ~nuiWebCSS();
  
  bool Load(nglIStream* pStream, const nglString& rSourceURL);
  bool Load(const nglString& rString, const nglString& rSourceURL);
  
  enum SelectorElementType
  {
    eTag,
    eClass,
    eProperty
  };
  
  class SelectorElement
  {
  public:
    SelectorElement(SelectorElementType tpe, const nglString& rSymbol, const nglString& rValue = nglString::Null);
    
    SelectorElementType mType;
  };
  
  enum ValueType
  {
    eEMs,
    eEXs,
    ePixels,
    eCentimeters,
    eMilimeters,
    eInches,
    ePoints,
    ePicas,
    eDegrees,
    eRadiants,
    eGradiants,
    eMiliseconds,
    eSeconds,
    eHertz,
    eKHertz,
    ePercents,
    eNumber,
    eURL,
    eColor,
    eString
  };
  
  class Action
  {
  public:
    Action(const nglString& rProperty, const nglString& rValue, ValueType tpe);
    
    nglString mProperty;
    nglString mValue;
    ValueType mType;
  };
  
protected:
  nuiParser* mpParser;
  
  bool ParseBody();
  bool ParseComment();
  bool ParseAction();
  bool ParseActions();
  bool ParseSelector();
  bool ParseSelectors();
  bool ParseRule();
  bool ParseCharset();
  bool ParseProperty();
  bool ParseValue();
  bool ParseAtCommand();
};