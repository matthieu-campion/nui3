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
  bool mIsValid;
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
  
  void AddSheet(nuiCSSStyleSheet* pSheet); ///< Add pSheet to the list of active css style sheets.
  void RemoveSheets(uint32 count); ///< remove the last count sheets from the context.
  
  bool Select(nuiHTMLContext& rContext, nuiHTMLItem* pNode);
private:
  std::vector<nuiCSSStyleSheet*> mSheets;
  css_select_ctx* mpContext;
  
};

class nuiCSSStyle
{
public:
  nuiCSSStyle();
  ~nuiCSSStyle();
    
  css_computed_style* GetStyle(); 
private:
  friend class nuiCSSContext;
  css_computed_style* mpStyle;
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


