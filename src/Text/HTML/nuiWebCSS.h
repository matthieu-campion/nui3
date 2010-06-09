/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiAsyncIStream.h"

extern "C"
{
  #include "libcss/libcss.h"
}


class nuiCSSStyleSheet
{
public:
  
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
  
  static css_error ResolveUrl(void *pw, const char *base, lwc_string *rel, lwc_string **abs);
  bool IsValid() const;
  nuiCSSStyleSheet(const nglString& rURL, nglString& rText, bool Inline = true);
  nuiCSSStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset);
  nuiCSSStyleSheet(const nglString& rURL, const nuiSignal1<nuiCSSStyleSheet*>::Slot& rDelegate = nuiSignal1<nuiCSSStyleSheet*>::Slot());
  void StreamDone(nuiAsyncIStream* pStream);
  
  void Init(nglIStream& rStream, const nglString& charset);
  void ImportDone(nuiCSSStyleSheet* pImport);
  ~nuiCSSStyleSheet();
};

class nuiCSSEngine
{
public:
  nuiCSSEngine();
  ~nuiCSSEngine();
  
  nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset) const;
  nuiCSSStyleSheet* CreateStyleSheet(const nglString& rURL, const nuiSignal1<nuiCSSStyleSheet*>::Slot& rDelegate = nuiSignal1<nuiCSSStyleSheet*>::Slot()) const;
  
  void Test();
  
private:
};


