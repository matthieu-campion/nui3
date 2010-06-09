/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiWebCSS.h"
#include "nuiHTTP.h"
#include "nuiHTML.h"
#include "nuiUnicode.h"

static void *nuiRealloc(void *ptr, size_t len, void *pw)
{
  UNUSED(pw);
  
  return realloc(ptr, len);
}


//class nuiCSSStyleSheet
css_error nuiCSSStyleSheet::ResolveUrl(void *pw, const char *base, lwc_string *rel, lwc_string **abs)
{
  UNUSED(pw);
  UNUSED(base);
  
  nglString url(lwc_string_data(rel), lwc_string_length(rel));
  nglString b(base);
  nuiHTML::GetAbsoluteURL(b, url);

  // Translate url:
  std::string s(url.GetStdString());
  lwc_intern_string(s.c_str(), s.size(), abs);
  
  //NGL_OUT(_T("CSS Resolve URL:\n\tbase '%ls'\n"), b.GetChars());
  //NGL_OUT(_T("\t-> '%ls'\n"), url.GetChars());
  
  return CSS_OK;
}
  
bool nuiCSSStyleSheet::IsValid() const
{
  return mpSheet != NULL;
}

nuiCSSStyleSheet::nuiCSSStyleSheet(const nglString& rURL, nglString& rText, bool Inline)
{
  mURL = rURL;
  mInline = Inline;
  mIsValid = false;
  char* pText = rText.Export();
  mpStream = new nglIMemory(pText, strlen(pText));
  Init(*mpStream, _T("UTF-8"));
  delete mpStream;
  mpStream = NULL;
}

nuiCSSStyleSheet::nuiCSSStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset)
{
  mURL = rURL;
  mInline = Inline;
  mIsValid = false;
  mpStream = &rStream;
  Init(rStream, rCharset);
}

nuiCSSStyleSheet::nuiCSSStyleSheet(const nglString& rURL, const nuiSignal1<nuiCSSStyleSheet*>::Slot& rDelegate)
{
  mURL = rURL;
  mInline = false;
  mIsValid = false;
  nuiAsyncIStream* pStream = new nuiAsyncIStream(mURL, true);
  mpStream = pStream;
  mSlotSink.Connect(pStream->StreamReady, nuiMakeDelegate(this, &nuiCSSStyleSheet::StreamDone));
  if (rDelegate)
    mSlotSink.Connect(Done, rDelegate);
}

void nuiCSSStyleSheet::StreamDone(nuiAsyncIStream* pStream)
{
  //  App->GetLog().SetLevel(_T("StopWatch"), 50);
  //  nuiStopWatch watch(_T("nuiHTMLView::StreamDone"));
  mpStream = NULL;
  mpSheet = NULL;
  nglString enc = _T("utf-8");
  const nuiHTTPResponse* pResponse = pStream->GetHTTPResponse();
  
  nglTextEncoding encoding = eEncodingUnknown;
  
  if (pResponse)
  {
    //NGL_OUT(_T("\n\nHTTP Headers:\n%ls\n\n"), pResponse->GetHeadersRep().GetChars());
    const nuiHTTPHeaderMap& rHeaders(pResponse->GetHeaders());
    nuiHTTPHeaderMap::const_iterator it = rHeaders.find(_T("location"));
    if (it != rHeaders.end())
    {
      nglString newurl = it->second;
      if (newurl[0] == '/')
      {
        mURL.TrimRight('/');
        mURL += newurl;
      }
      else
      {
        mURL = newurl;
      }
      NGL_OUT(_T("\n\nNew CSS location: %ls\n\n"), mURL.GetChars());
      
      mpStream = new nuiAsyncIStream(mURL, true);
      mSlotSink.Connect(pStream->StreamReady, nuiMakeDelegate(this, &nuiCSSStyleSheet::StreamDone));
      return;
    }
    
    it = rHeaders.find(_T("content-type"));
    
    if (it != rHeaders.end())
    {  
      nglString contents(it->second);
      contents.ToUpper();
      int32 pos = contents.Find(_T("CHARSET="));
      if (pos >= 0)
      {
        enc = contents.Extract(pos + 8);
        enc.Trim();
        encoding = nuiGetTextEncodingFromString(enc);
        //NGL_OUT(_T("\n\nHTTP Encoding: %ls - %d\n\n"), enc.GetChars(), encoding);
        
      }
    }
    
  }  
  
  Init(*pStream, enc);
  mpStream = NULL;    
  
  Done(this);
}

void nuiCSSStyleSheet::Init(nglIStream& rStream, const nglString& charset)
{
  NGL_OUT(_T("Init CSS '%ls'\n"), mURL.GetChars());
  mpSheet = NULL;
  
  size_t len, origlen;
#define CHUNK_SIZE (4096)
  uint8_t buf[CHUNK_SIZE];
  css_error error;
  int count;
  
  /* Initialise library */
  lwc_initialise(nuiRealloc, NULL, 0);
  
  nglString cs(charset);
  cs.ToUpper();
  error = css_stylesheet_create(CSS_LEVEL_21, cs.GetStdString().c_str(), mURL.GetStdString().c_str(), NULL, false, mInline, nuiRealloc, this, ResolveUrl, this, &mpSheet);
  
  NGL_ASSERT(error == CSS_OK);
  
  origlen = len = rStream.Available();
  
  while (len >= CHUNK_SIZE)
  {
    size_t read = rStream.Read(buf, CHUNK_SIZE, 1);
    NGL_ASSERT(read == CHUNK_SIZE);
    
    error = css_stylesheet_append_data(mpSheet, buf, CHUNK_SIZE);
    NGL_ASSERT(error == CSS_OK || error == CSS_NEEDDATA);
    
    len -= CHUNK_SIZE;
  }
  
  if (len > 0)
  {
    size_t read = rStream.Read(buf, len, 1);
    NGL_ASSERT(read == len);
    
    error = css_stylesheet_append_data(mpSheet, buf, len);
    NGL_ASSERT(error == CSS_OK || error == CSS_NEEDDATA);
    
    len = 0;
  }
  
  error = css_stylesheet_data_done(mpSheet);
  NGL_ASSERT(error == CSS_OK || error == CSS_IMPORTS_PENDING);
  
  if (error == CSS_IMPORTS_PENDING)
  {
    lwc_string *url;
    uint64_t media;
    
    error = css_stylesheet_next_pending_import(mpSheet, &url, &media);
    NGL_ASSERT(error == CSS_OK || error == CSS_INVALID);
    
    if (error == CSS_OK)
    {
      nglString urlstr (lwc_string_data(url), lwc_string_length(url));
      NGL_OUT(_T("CSS Import Request '%ls'\n"), urlstr.GetChars());
      nuiCSSStyleSheet* pImport = new nuiCSSStyleSheet(urlstr, nuiMakeDelegate(this, &nuiCSSStyleSheet::ImportDone));
      mpImports.push_back(pImport);
      
      error = CSS_IMPORTS_PENDING;
    }
  }
}

void nuiCSSStyleSheet::ImportDone(nuiCSSStyleSheet* pImport)
{
  if (pImport->IsValid())
  {
    css_stylesheet_register_import(mpSheet, pImport->mpSheet);
    NGL_OUT(_T("CSS Import done '%ls'\n"), mURL.GetChars());

    lwc_string *url;
    uint64_t media;
    
    css_error error = css_stylesheet_next_pending_import(mpSheet, &url, &media);
    NGL_ASSERT(error == CSS_OK || error == CSS_INVALID);
    
    if (error == CSS_OK)
    {
      nglString urlstr (lwc_string_data(url), lwc_string_length(url));
      NGL_OUT(_T("CSS Import Request '%ls'\n"), urlstr.GetChars());
      nuiCSSStyleSheet* pImport = new nuiCSSStyleSheet(urlstr, nuiMakeDelegate(this, &nuiCSSStyleSheet::ImportDone));
      mpImports.push_back(pImport);
      
      error = CSS_IMPORTS_PENDING;
    }
  }
  else
  {
    NGL_OUT(_T("CSS Import failed '%ls'\n"), mURL.GetChars());
  }
  
}

nuiCSSStyleSheet::~nuiCSSStyleSheet()
{
  delete mpStream;
  for (uint32 i = 0; i < mpImports.size(); i++)
    delete mpImports[i];
  css_stylesheet_destroy(mpSheet);
}
  

// class nuiCSSEngine
nuiCSSEngine::nuiCSSEngine()
{
  int res = css_initialise("", nuiRealloc, this);
  NGL_ASSERT(res == CSS_OK);
}

nuiCSSEngine::~nuiCSSEngine()
{
  int res = css_finalise(nuiRealloc, this);
  NGL_ASSERT(res == CSS_OK);
}

nuiCSSStyleSheet* nuiCSSEngine::CreateStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset) const
{
  nuiCSSStyleSheet* pSheet = new nuiCSSStyleSheet(rURL, rStream, Inline, rCharset);
}

nuiCSSStyleSheet* nuiCSSEngine::CreateStyleSheet(const nglString& rURL, const nuiSignal1<nuiCSSStyleSheet*>::Slot& rDelegate) const
{
  nuiCSSStyleSheet* pSheet = new nuiCSSStyleSheet(rURL, rDelegate);
}

void nuiCSSEngine::Test()
{
#if 0
  const char* css = 
  "@import \"mystyle1.css\";"\
  "@import url(\"mystyle2.css\");"\
  "body"\
  "{"\
  "background-color:#d0e4fe;"\
  "}"\
  "h1"\
  "{"\
  "color:orange;"\
  "text-align:center;"\
  "}"\
  "p"\
  "{"\
  "font-family:\"Times New Roman\";"\
  "font-size:20px;"\
  "}";
  
  nglIMemory mem(css, strlen(css));
  nuiCSSStyleSheet* pSheet = CreateStyleSheet(_T("http://prout.com/bleh.css"), mem, false);
  delete pSheet;
#endif
  
  nuiCSSStyleSheet* pSheet = CreateStyleSheet(_T("http://redmine.libnui.net/themes/mariposa/stylesheets/application.css?1251500354"));
  //nuiCSSStyleSheet* pSheet = CreateStyleSheet(_T("http://redmine.libnui.net/stylesheets/application.css"));
  
}

