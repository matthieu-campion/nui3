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
#include "nuiHTMLItem.h"

extern "C"
{
#include "libcss/libcss.h"
}



static void *nuiRealloc(void *ptr, size_t len, void *pw)
{
  UNUSED(pw);
  
  return realloc(ptr, len);
}

static lwc_string* MakeString(const nglString& rStr)
{
  std::string s(rStr.GetStdString());
  lwc_string* v;
  lwc_intern_string(s.c_str(), s.size(), &v);
  return v;
}


//class nuiCSSStyleSheet
static css_error ResolveUrl(void *pw, const char *base, lwc_string *rel, lwc_string **abs)
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

nuiCSSStyleSheet::nuiCSSStyleSheet(const nglString& rURL, const nglString& rText, bool Inline, const nuiStyleSheetDoneDelegate& rDelegate)
{
  NGL_OUT(_T("Create StyleSheet from text%ls\n"), Inline ? _T(" (inline)") : _T(""));
  mURL = rURL;
  mInline = Inline;
  mIsValid = false;
  char* pText = rText.Export();
  mpStream = new nglIMemory(pText, strlen(pText));
  if (rDelegate)
    mSlotSink.Connect(Done, rDelegate);
  Init(*mpStream, _T("UTF-8"));
  delete mpStream;
  mpStream = NULL;
}

nuiCSSStyleSheet::nuiCSSStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset, const nuiStyleSheetDoneDelegate& rDelegate)
{
  NGL_OUT(_T("Create StyleSheet from stream (base '%ls')%ls\n"), rURL.GetChars(), Inline ? _T(" (inline)") : _T(""));
  mURL = rURL;
  mInline = Inline;
  mIsValid = false;
  mpStream = &rStream;
  if (rDelegate)
    mSlotSink.Connect(Done, rDelegate);
  Init(rStream, rCharset);
}

nuiCSSStyleSheet::nuiCSSStyleSheet(const nglString& rURL, const nuiStyleSheetDoneDelegate& rDelegate)
{
  NGL_OUT(_T("Create StyleSheet from URL '%ls'\n"), rURL.GetChars());
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
}

void nuiCSSStyleSheet::Init(nglIStream& rStream, const nglString& charset)
{
  //NGL_OUT(_T("Init CSS '%ls'\n"), mURL.GetChars());
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
  else
  {
    Done(this);
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
    else
    {
      Done(this);
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

nuiCSSStyleSheet* nuiCSSEngine::CreateStyleSheet(const nglString& rURL, const nglString& rString, bool Inline, const nuiStyleSheetDoneDelegate& rDelegate)
{
  gCSSEngine.Instance();
  nuiCSSStyleSheet* pSheet = new nuiCSSStyleSheet(rURL, rString, Inline, rDelegate);
  if (!pSheet || !pSheet->IsValid())
  {
    delete pSheet;
    return NULL;
  }
  return pSheet;
}

nuiCSSStyleSheet* nuiCSSEngine::CreateStyleSheet(const nglString& rURL, nglIStream& rStream, bool Inline, const nglString& rCharset, const nuiStyleSheetDoneDelegate& rDelegate)
{
  gCSSEngine.Instance();
  nuiCSSStyleSheet* pSheet = new nuiCSSStyleSheet(rURL, rStream, Inline, rCharset, rDelegate);
  if (!pSheet || !pSheet->IsValid())
  {
    delete pSheet;
    return NULL;
  }
  return pSheet;
}

nuiCSSStyleSheet* nuiCSSEngine::CreateStyleSheet(const nglString& rURL, const nuiStyleSheetDoneDelegate& rDelegate)
{
  gCSSEngine.Instance();
  nuiCSSStyleSheet* pSheet = new nuiCSSStyleSheet(rURL, rDelegate);
  if (!pSheet || !pSheet->IsValid())
  {
    delete pSheet;
    return NULL;
  }
  return pSheet;
}

nuiSingletonHolder<nuiCSSEngine> nuiCSSEngine::gCSSEngine;


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

//////////
nuiCSSContext::nuiCSSContext()
{
}

nuiCSSContext::~nuiCSSContext()
{
}


void nuiCSSContext::AddSheet(nuiCSSStyleSheet* pSheet)
{
  mSheets.push_back(pSheet);
}

void nuiCSSContext::RemoveSheets(uint32 count)
{
  if (!count)
    return;
  NGL_ASSERT(count >= mSheets.size());
  mSheets.resize(mSheets.size() - count);
}

#if 0
css_error node_name(void *pw, void *n, lwc_string **name)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	
	*name = MakeString(node->GetNode()->GetName());
	
	return CSS_OK;
}

css_error node_classes(void *pw, void *n, lwc_string ***classes, uint32_t *n_classes)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	line_ctx *lc = pw;
  
	for (i = 0; i < node->n_attrs; i++)
  {
		bool amatch;
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, lc->attr_class, &amatch) == lwc_error_ok);
		if (amatch == true)
			break;
	}
  
	if (i != node->n_attrs)
  {
		*classes = realloc(NULL, sizeof(lwc_string **));
		if (*classes == NULL)
			return CSS_NOMEM;
    
		*(classes[0]) = lwc_string_ref(node->attrs[i].value);
		*n_classes = 1;
	}
  else
  {
		*classes = NULL;
		*n_classes = 0;
	}
  
	return CSS_OK;
  
}

css_error node_id(void *pw, void *n,
                  lwc_string **id)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	line_ctx *lc = pw;
  
	for (i = 0; i < node->n_attrs; i++)
  {
		bool amatch;
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, lc->attr_id, &amatch) == lwc_error_ok);
		if (amatch == true)
			break;
	}
  
	if (i != node->n_attrs)
		*id = lwc_string_ref(node->attrs[i].value);
	else
		*id = NULL;
  
	return CSS_OK;
}

css_error named_ancestor_node(void *pw, void *n, lwc_string *name, void **ancestor)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  
	for (node = node->parent; node != NULL; node = node->parent)
  {
		bool match;
		NGL_ASSERT(lwc_string_caseless_isequal(name, node->name, &match) == lwc_error_ok);
		if (match == true)
			break;
	}
  
	*ancestor = (void *) node;
  
	return CSS_OK;
}

css_error named_parent_node(void *pw, void *n, lwc_string *name, void **parent)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  
	*parent = NULL;
	if (node->parent != NULL)
  {
		bool match;
		NGL_ASSERT(lwc_string_caseless_isequal(name, node->parent->name, &match) == lwc_error_ok);
		if (match == true)
			*parent = (void *) node->parent;
	}
  
	return CSS_OK;
}

css_error named_sibling_node(void *pw, void *n, lwc_string *name, void **sibling)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  
	*sibling = NULL;
	if (node->prev != NULL)
  {
		bool match;
		NGL_ASSERT(lwc_string_caseless_isequal(name, node->prev->name, &match) == lwc_error_ok);
		if (match == true)
			*sibling = (void *) node->prev;
	}
  
	return CSS_OK;
}

css_error parent_node(void *pw, void *n, void **parent)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
  
	*parent = (void *) node->parent;
  
	return CSS_OK;
}

css_error sibling_node(void *pw, void *n, void **sibling)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
  
	*sibling = (void *) node->prev;
  
	return CSS_OK;
}

css_error node_has_name(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  
	NGL_ASSERT(lwc_string_caseless_isequal(node->name, name, match) == lwc_error_ok);
  
	return CSS_OK;
}

css_error node_has_class(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	line_ctx *ctx = pw;
  
	for (i = 0; i < node->n_attrs; i++)
  {
		bool amatch;
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, ctx->attr_class, &amatch) == lwc_error_ok);
		if (amatch == true)
			break;
	}
  
	/* Classes are case-sensitive in HTML */
	if (i != node->n_attrs && name == node->attrs[i].value)
		*match = true;
	else
		*match = false;
  
	return CSS_OK;
}

css_error node_has_id(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	line_ctx *ctx = pw;
  
	for (i = 0; i < node->n_attrs; i++)
  {
		bool amatch;
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, ctx->attr_id, &amatch) == lwc_error_ok);
		if (amatch == true)
			break;
	}
  
	/* IDs are case-sensitive in HTML */
	if (i != node->n_attrs && name == node->attrs[i].value)
		*match = true;
	else
		*match = false;
  
	return CSS_OK;
}

css_error node_has_attribute(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	UNUSED(pw);
	
	*match = false;
	for (i = 0; i < node->n_attrs; i++)
  {
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, name, match) == lwc_error_ok);
		if (*match == true)
			break;
	}
  
	return CSS_OK;
}

css_error node_has_attribute_equal(void *pw, void *n, lwc_string *name, lwc_string *value, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	UNUSED(pw);
  
	*match = false;
	
	for (i = 0; i < node->n_attrs; i++)
  {
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, name, match) == lwc_error_ok);
		if (*match == true)
			break;
	}
	
	if (*match == true)
  {
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, value, match) == lwc_error_ok);
	}
	
	return CSS_OK;
}

css_error node_has_attribute_includes(void *pw, void *n, lwc_string *name, lwc_string *value, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	size_t vlen = lwc_string_length(value);
	UNUSED(pw);
  
	*match = false;
	
	for (i = 0; i < node->n_attrs; i++)
  {
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, name, match) == lwc_error_ok);
		if (*match == true)
			break;
	}
  
	if (*match == true)
  {
		const char *p;
		const char *start = lwc_string_data(node->attrs[i].value);
		const char *end = start + 
    lwc_string_length(node->attrs[i].value);
		
		*match = false;
		
		for (p = start; p < end; p++)
    {
			if (*p == ' ')
      {
				if ((size_t) (p - start) == vlen && strncasecmp(start, lwc_string_data(value), vlen) == 0)
        {
          *match = true;
          break;
        }
        
				start = p + 1;
			}
		}
	}
  
	return CSS_OK;
}

css_error node_has_attribute_dashmatch(void *pw, void *n, lwc_string *name, lwc_string *value, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	size_t vlen = lwc_string_length(value);
	UNUSED(pw);
  
	*match = false;
	
	for (i = 0; i < node->n_attrs; i++)
  {
		NGL_ASSERT(lwc_string_caseless_isequal(node->attrs[i].name, name, match) == lwc_error_ok);
		if (*match == true)
			break;
	}
  
	if (*match == true)
  {
		const char *p;
		const char *start = lwc_string_data(node->attrs[i].value);
		const char *end = start + 
    lwc_string_length(node->attrs[i].value);
		
		*match = false;
		
		for (p = start; p < end; p++)
    {
			if (*p == '-')
      {
				if ((size_t) (p - start) == vlen && strncasecmp(start, lwc_string_data(value), vlen) == 0)
        {
          *match = true; 
          break;
        }
        
				start = p + 1;
			}
		}
	}
  
	return CSS_OK;
}

css_error node_is_first_child(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
  
	*match = (node->parent != NULL && node->parent->children == node);
  
	return CSS_OK;
}

css_error node_is_link(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

css_error node_is_visited(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

css_error node_is_hover(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

css_error node_is_active(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

css_error node_is_focus(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

css_error node_is_lang(void *pw, void *n,
                       lwc_string *lang,
                       bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
	UNUSED(lang);
  
	*match = false;
  
	return CSS_OK;
}

css_error node_presentational_hint(void *pw, void *node,
                                   uint32_t property, css_hint *hint)
{
	UNUSED(pw);
	UNUSED(node);
	UNUSED(property);
	UNUSED(hint);
  
	return CSS_PROPERTY_NOT_SET;
}

css_error ua_default_for_property(void *pw, uint32_t property, css_hint *hint)
{
	UNUSED(pw);
  
	if (property == CSS_PROP_COLOR)
  {
		hint->data.color = 0x00000000;
		hint->status = CSS_COLOR_COLOR;
	}
  else if (property == CSS_PROP_FONT_FAMILY)
  {
		hint->data.strings = NULL;
		hint->status = CSS_FONT_FAMILY_SANS_SERIF;
	}
  else if (property == CSS_PROP_QUOTES)
  {
		/* Not exactly useful :) */
		hint->data.strings = NULL;
		hint->status = CSS_QUOTES_NONE;
	}
  else if (property == CSS_PROP_VOICE_FAMILY)
  {
		/** \todo Fix this when we have voice-family done */
		hint->data.strings = NULL;
		hint->status = 0;
	}
  else
  {
		return CSS_INVALID;
	}
  
	return CSS_OK;
}

css_error compute_font_size(void *pw, const css_hint *parent, css_hint *size)
{
	static css_hint_length sizes[] = {
		{ FLTTOFIX(6.75), CSS_UNIT_PT },
		{ FLTTOFIX(7.50), CSS_UNIT_PT },
		{ FLTTOFIX(9.75), CSS_UNIT_PT },
		{ FLTTOFIX(12.0), CSS_UNIT_PT },
		{ FLTTOFIX(13.5), CSS_UNIT_PT },
		{ FLTTOFIX(18.0), CSS_UNIT_PT },
		{ FLTTOFIX(24.0), CSS_UNIT_PT }
	};
	const css_hint_length *parent_size;
  
	UNUSED(pw);
  
	/* Grab parent size, defaulting to medium if none */
	if (parent == NULL)
  {
		parent_size = &sizes[CSS_FONT_SIZE_MEDIUM - 1];
	}
  else
  {
		NGL_ASSERT(parent->status == CSS_FONT_SIZE_DIMENSION);
		NGL_ASSERT(parent->data.length.unit != CSS_UNIT_EM);
		NGL_ASSERT(parent->data.length.unit != CSS_UNIT_EX);
		parent_size = &parent->data.length;
	}
  
	NGL_ASSERT(size->status != CSS_FONT_SIZE_INHERIT);
  
	if (size->status < CSS_FONT_SIZE_LARGER)
  {
		/* Keyword -- simple */
		size->data.length = sizes[size->status - 1];
	}
  else if (size->status == CSS_FONT_SIZE_LARGER)
  {
		/** \todo Step within table, if appropriate */
		size->data.length.value = 
    FMUL(parent_size->value, FLTTOFIX(1.2));
		size->data.length.unit = parent_size->unit;
	}
  else if (size->status == CSS_FONT_SIZE_SMALLER)
  {
		/** \todo Step within table, if appropriate */
		size->data.length.value = 
    FMUL(parent_size->value, FLTTOFIX(1.2));
		size->data.length.unit = parent_size->unit;
	}
  else if (size->data.length.unit == CSS_UNIT_EM || size->data.length.unit == CSS_UNIT_EX)
  {
		size->data.length.value = 
    FMUL(size->data.length.value, parent_size->value);
    
		if (size->data.length.unit == CSS_UNIT_EX)
    {
			size->data.length.value = FMUL(size->data.length.value, FLTTOFIX(0.6));
		}
    
		size->data.length.unit = parent_size->unit;
	}
  else if (size->data.length.unit == CSS_UNIT_PCT)
  {
		size->data.length.value = FDIV(FMUL(size->data.length.value, parent_size->value), FLTTOFIX(100));
		size->data.length.unit = parent_size->unit;
	}
  
	size->status = CSS_FONT_SIZE_DIMENSION;
  
	return CSS_OK;
}

#endif

