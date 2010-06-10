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

class nuiCSSStyleSheet
{
public:
  
  ~nuiCSSStyleSheet();
  nuiSignal1<nuiCSSStyleSheet*> Done;
  
private:
  friend class nuiCSSEngine;
  std::vector<nuiCSSStyleSheet*> mpImports;
  nglIStream* mpStream;
  nuiSlotsSink mSlotSink;
  bool mIsValid;
  nglString mURL;
  bool mInline;
  css_stylesheet* mpSheet;
  
  bool IsValid() const;
  nuiCSSStyleSheet(const nglString& rURL, const nglString& rText, bool Inline = true);
  nuiCSSStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset);
  nuiCSSStyleSheet(const nglString& rURL, const nuiSignal1<nuiCSSStyleSheet*>::Slot& rDelegate = nuiSignal1<nuiCSSStyleSheet*>::Slot());
  void StreamDone(nuiAsyncIStream* pStream);
  
  void Init(nglIStream& rStream, const nglString& charset);
  void ImportDone(nuiCSSStyleSheet* pImport);
};

class nuiCSSEngine
{
public:
  
  static nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, const nglString& rString, bool Inline);
  static nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset);
  static nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, const nuiSignal1<nuiCSSStyleSheet*>::Slot& rDelegate = nuiSignal1<nuiCSSStyleSheet*>::Slot());
  
  void Test();
  
private:
  nuiCSSEngine();
  ~nuiCSSEngine();
  
  friend class nuiSingletonHolder<nuiCSSEngine>;
  static nuiSingletonHolder<nuiCSSEngine> gCSSEngine;
};


