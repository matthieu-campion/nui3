/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiAsyncIStream.h"
#include "nuiSingleton.h"

typedef struct css_stylesheet css_stylesheet;
typedef struct css_select_ctx css_select_ctx;
typedef struct css_computed_style css_computed_style;
typedef struct lwc_string_s lwc_string;

class nuiCSSStyleSheet;
class nuiHTMLItem;
class nuiHTMLNode;
class nuiHTMLContext;

typedef nuiSignal1<nuiCSSStyleSheet*>::Slot nuiStyleSheetDoneDelegate;

class nuiCSSStyleSheet
{
public:
  
  ~nuiCSSStyleSheet();
  nuiSignal1<nuiCSSStyleSheet*> Done;

  bool IsValid() const;
  
private:
  friend class nuiCSSEngine;
  friend class nuiCSSContext;
  std::vector<nuiCSSStyleSheet*> mpImports;
  nglIStream* mpStream;
  nuiSlotsSink mSlotSink;
  nglString mURL;
  bool mInline;
  css_stylesheet* mpSheet;
  
  nuiCSSStyleSheet(const nglString& rURL, const nglString& rText, bool Inline, const nuiStyleSheetDoneDelegate& rDelegate);
  nuiCSSStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset, const nuiStyleSheetDoneDelegate& rDelegate);
  nuiCSSStyleSheet(const nglString& rURL, const nuiStyleSheetDoneDelegate& rDelegate);
  void StreamDone(nuiAsyncIStream* pStream);
  
  void Init(nglIStream& rStream, const nglString& charset);
  void ImportDone(nuiCSSStyleSheet* pImport);
};


class nuiCSSContext
{
public:
  nuiCSSContext();
  ~nuiCSSContext();
  
  void AddSheet(const nuiCSSStyleSheet* pSheet); ///< Add pSheet to the list of active css style sheets.
  void RemoveSheets(uint32 count); ///< remove the last count sheets from the context.
  
  bool Select(nuiHTMLContext& rContext, nuiHTMLNode* pNode);
private:
  std::vector<const nuiCSSStyleSheet*> mSheets;
  css_select_ctx* mpContext;
  
};

class nuiCSSStyle
{
public:
  nuiCSSStyle(nuiHTMLNode* pNode);
  ~nuiCSSStyle();
    
  css_computed_style* GetStyle();
  
  
  nuiColor GetColor() const;
  nuiColor GetBgColor() const;
  bool HasBgColor() const;

  enum Position
  {
    CSS_POSITION_INHERIT			= 0x0,
    CSS_POSITION_STATIC			= 0x1,
    CSS_POSITION_RELATIVE			= 0x2,
    CSS_POSITION_ABSOLUTE			= 0x3,
    CSS_POSITION_FIXED			= 0x4
  };
  Position GetPosition() const;

  
  enum Unit
  {
    CSS_UNIT_PX                 = 0x0,
    CSS_UNIT_EX                 = 0x1,
    CSS_UNIT_EM                 = 0x2,
    CSS_UNIT_IN                 = 0x3,
    CSS_UNIT_CM                 = 0x4,
    CSS_UNIT_MM                 = 0x5,
    CSS_UNIT_PT                 = 0x6,
    CSS_UNIT_PC                 = 0x7,
    
    CSS_UNIT_PCT                = 0x8,	/* Percentage */
    
    CSS_UNIT_DEG                = 0x9,
    CSS_UNIT_GRAD               = 0xa,
    CSS_UNIT_RAD                = 0xb,
    
    CSS_UNIT_MS                 = 0xc,
    CSS_UNIT_S                  = 0xd,
    
    CSS_UNIT_HZ                 = 0xe,
    CSS_UNIT_KHZ                = 0xf
  };
  void GetTop(float& value, Unit& unit) const;
  void GetLeft(float& value, Unit& unit) const;
  void GetBottom(float& value, Unit& unit) const;
  void GetRight(float& value, Unit& unit) const;
  void GetWidth(float& value, Unit& unit) const;
  void GetMaxWidth(float& value, Unit& unit) const;
  
private:
  friend class nuiCSSContext;
  css_computed_style* mpStyle;
  nuiHTMLNode* mpNode;
};

class nuiCSSEngine
{
public:
  
  static nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, const nglString& rString, bool Inline, const nuiStyleSheetDoneDelegate& rDelegate = nuiStyleSheetDoneDelegate());
  static nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset, const nuiStyleSheetDoneDelegate& rDelegate = nuiStyleSheetDoneDelegate());
  static nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, const nuiStyleSheetDoneDelegate& rDelegate = nuiStyleSheetDoneDelegate());
  
  void Test();
  
private:
  nuiCSSEngine();
  ~nuiCSSEngine();
  
  friend class nuiSingletonHolder<nuiCSSEngine>;
  static nuiSingletonHolder<nuiCSSEngine> gCSSEngine;
};


