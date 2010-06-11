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

static nglString MakeString(lwc_string* str)
{
  nglString v(lwc_string_data(str), lwc_string_length(str));
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
  mpContext = NULL;
  css_error err = css_select_ctx_create(nuiRealloc, this, &mpContext);
  NGL_ASSERT(err == CSS_OK);
}

nuiCSSContext::~nuiCSSContext()
{
  css_select_ctx_destroy(mpContext);
}


void nuiCSSContext::AddSheet(nuiCSSStyleSheet* pSheet)
{
  mSheets.push_back(pSheet);
  css_select_ctx_append_sheet(mpContext, pSheet->mpSheet, CSS_ORIGIN_AUTHOR, CSS_MEDIA_SCREEN);
}

void nuiCSSContext::RemoveSheets(uint32 count)
{
  if (!count)
    return;
  NGL_ASSERT(count >= mSheets.size());
  for (uint32 i = mSheets.size() - count; i < mSheets.size(); i++)
    css_select_ctx_remove_sheet(mpContext, mSheets[i]->mpSheet);
  mSheets.resize(mSheets.size() - count);
}

static css_error node_name(void *pw, void *node, lwc_string **name);
static css_error node_classes(void *pw, void *node, lwc_string ***classes, uint32_t *n_classes);
static css_error node_id(void *pw, void *node, lwc_string **id);
static css_error named_ancestor_node(void *pw, void *node, lwc_string *name, void **ancestor);
static css_error named_parent_node(void *pw, void *node, lwc_string *name, void **parent);
static css_error named_sibling_node(void *pw, void *node, lwc_string *name, void **sibling);
static css_error parent_node(void *pw, void *node, void **parent);
static css_error sibling_node(void *pw, void *node, void **sibling);
static css_error node_has_name(void *pw, void *node, lwc_string *name, bool *match);
static css_error node_has_class(void *pw, void *node, lwc_string *name, bool *match);
static css_error node_has_id(void *pw, void *node, lwc_string *name, bool *match);
static css_error node_has_attribute(void *pw, void *node, lwc_string *name, bool *match);
static css_error node_has_attribute_equal(void *pw, void *node, lwc_string *name, lwc_string *value, bool *match);
static css_error node_has_attribute_dashmatch(void *pw, void *node, lwc_string *name, lwc_string *value, bool *match);
static css_error node_has_attribute_includes(void *pw, void *node, lwc_string *name, lwc_string *value, bool *match);
static css_error node_is_first_child(void *pw, void *node, bool *match);
static css_error node_is_link(void *pw, void *node, bool *match);
static css_error node_is_visited(void *pw, void *node, bool *match);
static css_error node_is_hover(void *pw, void *node, bool *match);
static css_error node_is_active(void *pw, void *node, bool *match);
static css_error node_is_focus(void *pw, void *node, bool *match);
static css_error node_is_lang(void *pw, void *node, lwc_string *lang, bool *match);
static css_error node_presentational_hint(void *pw, void *node, uint32_t property, css_hint *hint);
static css_error ua_default_for_property(void *pw, uint32_t property, css_hint *hint);
static css_error compute_font_size(void *pw, const css_hint *parent, css_hint *size);

static css_select_handler selection_handler =
{
	node_name,
	node_classes,
	node_id,
	named_ancestor_node,
	named_parent_node,
	named_sibling_node,
	parent_node,
	sibling_node,
	node_has_name,
	node_has_class,
	node_has_id,
	node_has_attribute,
	node_has_attribute_equal,
	node_has_attribute_dashmatch,
	node_has_attribute_includes,
	node_is_first_child,
	node_is_link,
	node_is_visited,
	node_is_hover,
	node_is_active,
	node_is_focus,
	node_is_lang,
	node_presentational_hint,
	ua_default_for_property,
	compute_font_size
};

bool nuiCSSContext::Select(nuiHTMLContext& rContext, nuiHTMLItem* pNode)
{
  css_computed_style *style;
	css_error error;

  /*
   * \param ctx             Selection context to use
   * \param node            Node to select style for
   * \param pseudo_element  Pseudo element to select for, instead
   * \param media           Currently active media types
   * \param inline_style    Corresponding inline style for node, or NULL
   * \param result          Pointer to style to populate (assumed clean)
   * \param handler         Dispatch table of handler functions
   * \param pw              Client-specific private data for handler functions
   */
  error = css_select_style(mpContext, pNode, 0, CSS_MEDIA_SCREEN, pNode->GetInlineStyle()->mpSheet, pNode->GetStyle().GetStyle(), &selection_handler, this);
    
	return error == CSS_OK;
}

/// class nuiCSSStyle
nuiCSSStyle::nuiCSSStyle()
{
	css_error error;
  
	error = css_computed_style_create(nuiRealloc, this, &mpStyle);
	if (error != CSS_OK)
		return;
}

nuiCSSStyle::~nuiCSSStyle()
{
  if (mpStyle)
    css_computed_style_destroy(mpStyle);
}

css_computed_style* nuiCSSStyle::GetStyle()
{
  return mpStyle;
}

/**
 * Callback to retrieve a node's name.
 *
 * \param pw    HTML document
 * \param node  DOM node
 * \param name  Pointer to location to receive node name
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 */
css_error node_name(void *pw, void *n, lwc_string **name)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	
	*name = MakeString(node->GetNode()->GetName());
	
	return CSS_OK;
}

/**
 * Callback to retrieve a node's classes.
 *
 * \param pw         HTML document
 * \param node       DOM node
 * \param classes    Pointer to location to receive class name array
 * \param n_classes  Pointer to location to receive length of class name array
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 *
 * \note The returned array will be destroyed by libcss. Therefore, it must
 *       be allocated using the same allocator as used by libcss during style
 *       selection.
 */
css_error node_classes(void *pw, void *n, lwc_string ***classes, uint32_t *n_classes)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
  *classes = NULL;
  *n_classes = 0;

  nuiHTMLAttrib* pClass = node->GetNode()->GetAttribute(nuiHTMLAttrib::eAttrib_CLASS);
  if (!pClass)
    return CSS_OK;
  
  nglString cls(pClass->GetValue());
  std::vector<nglString> tokens;
  cls.Tokenize(tokens);
  uint32 count = tokens.size();
  
  *classes = (lwc_string**)realloc(NULL, sizeof(lwc_string **) * count);
  if (*classes == NULL)
    return CSS_NOMEM;
  
  for (uint32 i = 0; i < count; i++)
    *(classes[i]) = MakeString(tokens[i]);
  *n_classes = count;
  
	return CSS_OK;
}

/**
 * Callback to retrieve a node's ID.
 *
 * \param pw    HTML document
 * \param node  DOM node
 * \param id    Pointer to location to receive id value
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 */
css_error node_id(void *pw, void *n,
                  lwc_string **id)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	*id = NULL;

  nuiHTMLAttrib* pID = node->GetNode()->GetAttribute(nuiHTMLAttrib::eAttrib_ID);
  if (!pID)
    return CSS_OK;

  *id = MakeString(pID->GetValue());
  
	return CSS_OK;
}

/**
 * Callback to find a named ancestor node.
 *
 * \param pw        HTML document
 * \param node      DOM node
 * \param name      Node name to search for
 * \param ancestor  Pointer to location to receive ancestor
 * \return CSS_OK.
 *
 * \post \a ancestor will contain the result, or NULL if there is no match
 */
css_error named_ancestor_node(void *pw, void *n, lwc_string *name, void **ancestor)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);

  nglString Name(MakeString(name));

  node = node->GetParent();
  
  while (node && node->GetNode()->GetName().Compare(Name, false) != 0)
    node = node->GetParent();
  
	*ancestor = (void *)node;
  
	return CSS_OK;
}

/**
 * Callback to find a named parent node
 *
 * \param pw      HTML document
 * \param node    DOM node
 * \param name    Node name to search for
 * \param parent  Pointer to location to receive parent
 * \return CSS_OK.
 *
 * \post \a parent will contain the result, or NULL if there is no match
 */
css_error named_parent_node(void *pw, void *n, lwc_string *name, void **parent)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  
  nglString Name(MakeString(name));
  
  node = node->GetParent();
  
  *parent = NULL;
  if (node && node->GetNode()->GetName().Compare(Name, false) != 0)
    *parent = (void *)node;
  
	return CSS_OK;
}

/**
 * Callback to find a named sibling node.
 *
 * \param pw       HTML document
 * \param node     DOM node
 * \param name     Node name to search for
 * \param sibling  Pointer to location to receive sibling
 * \return CSS_OK.
 *
 * \post \a sibling will contain the result, or NULL if there is no match
 */
css_error named_sibling_node(void *pw, void *n, lwc_string *name, void **sibling)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  *sibling = NULL;
  
  nuiHTMLItem* pParent = node->GetParent();
  if (!pParent)
    return CSS_OK;
  
  nglString Name = MakeString(name);
  
  for (uint32 i = 0; i < pParent->GetChildrenCount(); i++)
  {
    nuiHTMLItem* pChild = pParent->GetChild(i);
    if (pChild == node)
    {
      if (i == 0)
        return CSS_OK;
      pChild = pParent->GetChild(i - 1);
      if (pChild->GetNode()->GetName().Compare(Name, false) == 0)
        *sibling = pChild;
      return CSS_OK;
    }
  }

  // Huh?!?
	return CSS_OK;
}

/**
 * Callback to retrieve the parent of a node.
 *
 * \param pw      HTML document
 * \param node    DOM node
 * \param parent  Pointer to location to receive parent
 * \return CSS_OK.
 *
 * \post \a parent will contain the result, or NULL if there is no match
 */
css_error parent_node(void *pw, void *n, void **parent)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
  
	*parent = node->GetParent();
  
	return CSS_OK;
}

/**
 * Callback to retrieve the preceding sibling of a node.
 *
 * \param pw       HTML document
 * \param node     DOM node
 * \param sibling  Pointer to location to receive sibling
 * \return CSS_OK.
 *
 * \post \a sibling will contain the result, or NULL if there is no match
 */
css_error sibling_node(void *pw, void *n, void **sibling)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  *sibling = NULL;
  
  nuiHTMLItem* pParent = node->GetParent();
  if (!pParent)
    return CSS_OK;
  
  for (uint32 i = 0; i < pParent->GetChildrenCount(); i++)
  {
    nuiHTMLItem* pChild = pParent->GetChild(i);
    if (pChild == node)
    {
      if (i == 0)
        return CSS_OK;
      pChild = pParent->GetChild(i - 1);
      *sibling = pChild;
      return CSS_OK;
    }
  }
  
  // Huh?!?
	return CSS_OK;
}

/**
 * Callback to determine if a node has the given name.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_name(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	UNUSED(pw);
  
  *match = node->GetNode()->GetName().Compare(MakeString(name), false) == 0;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node has the given class.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_class(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;

  *match = false;
  
  nuiHTMLAttrib* pClass = node->GetNode()->GetAttribute(nuiHTMLAttrib::eAttrib_CLASS);
  if (!pClass)
    return CSS_OK;

  nglString Class(MakeString(name));
  nglString cls(pClass->GetValue());
  std::vector<nglString> tokens;
  cls.Tokenize(tokens);
  uint32 count = tokens.size();
  
  for (uint32 i = 0; i < count; i++)
  { 
    if (tokens[i].Compare(Class, false) == 0)
    {
      *match = true;
      return CSS_OK;
    }
  }
  
	return CSS_OK;
}

/**
 * Callback to determine if a node has the given id.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_id(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
  *match = false;
  
  nuiHTMLAttrib* pID = node->GetNode()->GetAttribute(nuiHTMLAttrib::eAttrib_ID);
  if (!pID)
    return CSS_OK;
  
  nglString ID(MakeString(name));
  nglString id(pID->GetValue());
  
  if (ID.Compare(id, false) == 0)
    *match = true;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node has an attribute with the given name.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_attribute(void *pw, void *n, lwc_string *name, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	UNUSED(pw);
	
	*match = false;
  nuiHTMLAttrib* pAttr = node->GetNode()->GetAttribute(MakeString(name));
  if (pAttr)
    *match = true;

	return CSS_OK;
}

/**
 * Callback to determine if a node has an attribute with given name and value.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param value  Value to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_attribute_equal(void *pw, void *n, lwc_string *name, lwc_string *value, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	UNUSED(pw);
	
	*match = false;
  nuiHTMLAttrib* pAttr = node->GetNode()->GetAttribute(MakeString(name));
  if (!pAttr)
    return CSS_OK;

  if (pAttr->GetValue().Compare(MakeString(value), false) == 0)
    *match = true;
	return CSS_OK;
}

/**
 * Callback to determine if a node has an attribute with the given name whose
 * value includes that given.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param value  Value to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_attribute_includes(void *pw, void *n, lwc_string *name, lwc_string *value, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	UNUSED(pw);
	
	*match = false;
  nuiHTMLAttrib* pAttr = node->GetNode()->GetAttribute(MakeString(name));
  if (!pAttr)
    return CSS_OK;
  
  if (pAttr->GetValue().Find(MakeString(value), 0, false) >= 0)
    *match = true;
	return CSS_OK;
}

/**
 * Callback to determine if a node has an attribute with the given name whose
 * value dashmatches that given.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param name   Name to match
 * \param value  Value to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_has_attribute_dashmatch(void *pw, void *n, lwc_string *name, lwc_string *value, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
	uint32_t i;
	size_t vlen = lwc_string_length(value);
	UNUSED(pw);
  
	*match = false;
  nuiHTMLAttrib* pAttr = node->GetNode()->GetAttribute(MakeString(name));
  if (!pAttr)
    return CSS_OK;

	nglString val(MakeString(value));
  
  const nglChar* p;
  const nglChar* start = val.GetChars();
  const nglChar* end = start + val.GetLength();
  
  for (p = start; p < end; p++)
  {
    if (*p == _T('-'))
    {
      if (((size_t) (p - start) == vlen) && (val.Compare(start, 0, vlen, false) == 0))
      {
        *match = true; 
        break;
      }
      
      start = p + 1;
    }
  }
  
	return CSS_OK;
}

/**
 * Callback to determine if a node is the first child of its parent.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_is_first_child(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
  
	*match = (node->GetParent() != NULL && node->GetParent()->GetChild(0) == node);
  
	return CSS_OK;
}

/**
 * Callback to determine if a node is a linking element.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_is_link(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
  
	*match = node->GetNode()->GetTagType() == nuiHTMLNode::eTag_A ;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node is a linking element whose target has been
 * visited.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_is_visited(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node is currently being hovered over.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_is_hover(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node is currently activated.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_is_active(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node has the input focus.
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
css_error node_is_focus(void *pw, void *n, bool *match)
{
	nuiHTMLItem* node = (nuiHTMLItem*)n;
  
	UNUSED(pw);
	UNUSED(node);
  
	*match = false;
  
	return CSS_OK;
}

/**
 * Callback to determine if a node has the given language
 *
 * \param pw     HTML document
 * \param node   DOM node
 * \param lang   Language specifier to match
 * \param match  Pointer to location to receive result
 * \return CSS_OK.
 *
 * \post \a match will contain true if the node matches and false otherwise.
 */
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

/**
 * Callback to retrieve presentational hints for a node
 *
 * \param pw        HTML document
 * \param node      DOM node
 * \param property  CSS property to retrieve
 * \param hint      Pointer to hint object to populate
 * \return CSS_OK               on success,
 *         CSS_PROPERTY_NOT_SET if there is no hint for the requested property,
 *         CSS_NOMEM            on memory exhaustion.
 */
css_error node_presentational_hint(void *pw, void *node,
                                   uint32_t property, css_hint *hint)
{
	UNUSED(pw);
	UNUSED(node);
	UNUSED(property);
	UNUSED(hint);
  
#if 0
	struct content *html = pw;
	xmlNode *n = node;
  
	if (property == CSS_PROP_BACKGROUND_IMAGE) {
		char *url;
		url_func_result res;
		xmlChar *bg = xmlGetProp(n, (const xmlChar *) "background");
    
		if (bg == NULL)
			return CSS_PROPERTY_NOT_SET;
    
    
		res = url_join((const char *) bg,
                   html->data.html.base_url, &url);
    
		xmlFree(bg);
    
		if (res == URL_FUNC_NOMEM) {
			return CSS_NOMEM;
		} else if (res == URL_FUNC_OK) {
			lwc_string *iurl;
			lwc_error lerror;
      
			lerror = lwc_intern_string(url, strlen(url), &iurl);
      
			free(url);
      
			if (lerror == lwc_error_oom) {
				return CSS_NOMEM;
			} else if (lerror == lwc_error_ok) {
				hint->data.string = iurl;
				hint->status = CSS_BACKGROUND_IMAGE_IMAGE;
				return CSS_OK;
			}
		}
	} else if (property == CSS_PROP_BACKGROUND_COLOR) {
		xmlChar *bgcol = xmlGetProp(n, (const xmlChar *) "bgcolor");
		if (bgcol == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (nscss_parse_colour((const char *) bgcol,
                           &hint->data.color)) {
			hint->status = CSS_BACKGROUND_COLOR_COLOR;
		} else {
			xmlFree(bgcol);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(bgcol);
    
		return CSS_OK;
	} else if (property == CSS_PROP_CAPTION_SIDE) {
		xmlChar *align = NULL;
    
		if (strcmp((const char *) n->name, "caption") == 0)
			align = xmlGetProp(n, (const xmlChar *) "align");
    
		if (align == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (strcasecmp((const char *) align, "bottom") == 0) {
			hint->status = CSS_CAPTION_SIDE_BOTTOM;
		} else {
			xmlFree(align);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(align);
    
		return CSS_OK;
	} else if (property == CSS_PROP_COLOR) {
		xmlChar *col;
		css_error error;
		bool is_link, is_visited;
    
		error = node_is_link(html, n, &is_link);
		if (error != CSS_OK)
			return error;
    
		if (is_link) {
			xmlNode *body;
			for (body = n; body != NULL && body->parent != NULL &&
           body->parent->parent != NULL;
           body = body->parent) {
				if (body->parent->parent->parent == NULL)
					break;
			}
      
			error = node_is_visited(html, n, &is_visited);
			if (error != CSS_OK)
				return error;
      
			if (is_visited)
				col = xmlGetProp(body,
                         (const xmlChar *) "vlink");
			else
				col = xmlGetProp(body,
                         (const xmlChar *) "link");
		} else if (strcmp((const char *) n->name, "body") == 0) {
			col = xmlGetProp(n, (const xmlChar *) "text");
		} else {
			col = xmlGetProp(n, (const xmlChar *) "color");
		}
    
		if (col == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (nscss_parse_colour((const char *) col, &hint->data.color)) {
			hint->status = CSS_COLOR_COLOR;
		} else {
			xmlFree(col);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(col);
    
		return CSS_OK;
	} else if (property == CSS_PROP_FLOAT) {
		xmlChar *align = NULL;
    
		/** \todo input[type=image][align=*] - $11.3.3 */
		if (strcmp((const char *) n->name, "table") == 0 ||
				strcmp((const char *) n->name, "applet") == 0 ||
				strcmp((const char *) n->name, "embed") == 0 ||
				strcmp((const char *) n->name, "iframe") == 0 ||
				strcmp((const char *) n->name, "img") == 0 ||
				strcmp((const char *) n->name, "object") == 0)
			align = xmlGetProp(n, (const xmlChar *) "align");
    
		if (align == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (strcasecmp((const char *) align, "left") == 0) {
			hint->status = CSS_FLOAT_LEFT;
		} else if (strcasecmp((const char *) align, "right") == 0) {
			hint->status = CSS_FLOAT_RIGHT;
		} else {
			xmlFree(align);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(align);
    
		return CSS_OK;
	} else if (property == CSS_PROP_HEIGHT) {
		xmlChar *height;
    
		if (strcmp((const char *) n->name, "iframe") == 0 ||
				strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0 ||
				strcmp((const char *) n->name, "tr") == 0 ||
				strcmp((const char *) n->name, "img") == 0 ||
				strcmp((const char *) n->name, "object") == 0 ||
				strcmp((const char *) n->name, "applet") == 0)
			height = xmlGetProp(n, (const xmlChar *) "height");
		else if (strcmp((const char *) n->name, "textarea") == 0)
			height = xmlGetProp(n, (const xmlChar *) "rows");
		else
			height = NULL;
    
		if (height == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (parse_dimension((const char *) height, false,
                        &hint->data.length.value,
                        &hint->data.length.unit)) {
			hint->status = CSS_HEIGHT_SET;
		} else {
			xmlFree(height);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(height);
    
		if (strcmp((const char *) n->name, "textarea") == 0)
			hint->data.length.unit = CSS_UNIT_EM;
    
		return CSS_OK;
	} else if (property == CSS_PROP_WIDTH) {
		xmlChar *width;
    
		if (strcmp((const char *) n->name, "hr") == 0 ||
				strcmp((const char *) n->name, "iframe") == 0 ||
				strcmp((const char *) n->name, "img") == 0 ||
				strcmp((const char *) n->name, "object") == 0 ||
				strcmp((const char *) n->name, "table") == 0 ||
				strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0 ||
				strcmp((const char *) n->name, "applet") == 0)
			width = xmlGetProp(n, (const xmlChar *) "width");
		else if (strcmp((const char *) n->name, "textarea") == 0)
			width = xmlGetProp(n, (const xmlChar *) "cols");
		else if (strcmp((const char *) n->name, "input") == 0) {
			width = xmlGetProp(n, (const xmlChar *) "size");
		} else
			width = NULL;
    
		if (width == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (parse_dimension((const char *) width, false,
                        &hint->data.length.value,
                        &hint->data.length.unit)) {
			hint->status = CSS_WIDTH_SET;
		} else {
			xmlFree(width);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(width);
    
		if (strcmp((const char *) n->name, "textarea") == 0)
			hint->data.length.unit = CSS_UNIT_EX;
		else if (strcmp((const char *) n->name, "input") == 0) {
			xmlChar *type = xmlGetProp(n, (const xmlChar *) "type");
      
			if (type == NULL || strcasecmp((const char *) type,
                                     "text") == 0 ||
					strcasecmp((const char *) type,
                     "password") == 0)
				hint->data.length.unit = CSS_UNIT_EX;
			else {
				xmlFree(type);
				return CSS_PROPERTY_NOT_SET;
			}
      
			if (type != NULL)
				xmlFree(type);
		}
    
		return CSS_OK;
	} else if (property == CSS_PROP_BORDER_SPACING) {
		xmlChar *cellspacing;
    
		if (strcmp((const char *) n->name, "table") != 0)
			return CSS_PROPERTY_NOT_SET;
    
		cellspacing = xmlGetProp(n, (const xmlChar *) "cellspacing");
		if (cellspacing == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (parse_dimension((const char *) cellspacing, false,
                        &hint->data.position.h.value,
                        &hint->data.position.h.unit)) {
			hint->data.position.v = hint->data.position.h;
			hint->status = CSS_BORDER_SPACING_SET;
		} else {
			xmlFree(cellspacing);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(cellspacing);
    
		return CSS_OK;
	} else if (property == CSS_PROP_BORDER_TOP_COLOR ||
             property == CSS_PROP_BORDER_RIGHT_COLOR ||
             property == CSS_PROP_BORDER_BOTTOM_COLOR ||
             property == CSS_PROP_BORDER_LEFT_COLOR) {
		xmlChar *col;
    
		if (strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0) {
			/* Find table */
			for (n = n->parent; n != NULL &&
           n->type == XML_ELEMENT_NODE;
           n = n->parent) {
				if (strcmp((const char *) n->name, "table") ==
						0)
					break;
			}
      
			if (n == NULL)
				return CSS_PROPERTY_NOT_SET;
		}
    
		if (strcmp((const char *) n->name, "table") == 0)
			col = xmlGetProp(n, (const xmlChar *) "bordercolor");
		else
			col = NULL;
    
		if (col == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (nscss_parse_colour((const char *) col, &hint->data.color)) {
			hint->status = CSS_BORDER_COLOR_COLOR;
		} else {
			xmlFree(col);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(col);
    
		return CSS_OK;
	} else if (property == CSS_PROP_BORDER_TOP_STYLE ||
             property == CSS_PROP_BORDER_RIGHT_STYLE ||
             property == CSS_PROP_BORDER_BOTTOM_STYLE ||
             property == CSS_PROP_BORDER_LEFT_STYLE) {
		bool is_table_cell = false;
    
		if (strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0) {
			is_table_cell = true;
			/* Find table */
			for (n = n->parent; n != NULL &&
           n->type == XML_ELEMENT_NODE;
           n = n->parent) {
				if (strcmp((const char *) n->name, "table") ==
						0)
					break;
			}
      
			if (n == NULL)
				return CSS_PROPERTY_NOT_SET;
		}
    
		if (strcmp((const char *) n->name, "table") == 0 &&
				xmlHasProp(n,
                   (const xmlChar *) "border") != NULL) {
          if (is_table_cell)
            hint->status = CSS_BORDER_STYLE_INSET;
          else
            hint->status = CSS_BORDER_STYLE_OUTSET;
          return CSS_OK;
        }
	} else if (property == CSS_PROP_BORDER_TOP_WIDTH ||
             property == CSS_PROP_BORDER_RIGHT_WIDTH ||
             property == CSS_PROP_BORDER_BOTTOM_WIDTH ||
             property == CSS_PROP_BORDER_LEFT_WIDTH) {
		xmlChar *width;
		bool is_table_cell = false;
    
		if (strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0) {
			is_table_cell = true;
			/* Find table */
			for (n = n->parent; n != NULL &&
           n->type == XML_ELEMENT_NODE;
           n = n->parent) {
				if (strcmp((const char *) n->name, "table") ==
						0)
					break;
			}
      
			if (n == NULL)
				return CSS_PROPERTY_NOT_SET;
		}
    
		if (strcmp((const char *) n->name, "table") == 0)
			width = xmlGetProp(n, (const xmlChar *) "border");
		else
			width = NULL;
    
		if (width == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (parse_dimension((const char *) width, false,
                        &hint->data.length.value,
                        &hint->data.length.unit)) {
			if (is_table_cell &&
					INTTOFIX(0) !=
					hint->data.length.value) {
				hint->data.length.value = INTTOFIX(1);
				hint->data.length.unit = CSS_UNIT_PX;
			}
			hint->status = CSS_BORDER_WIDTH_WIDTH;
		} else {
			xmlFree(width);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(width);
    
		return CSS_OK;
	} else if (property == CSS_PROP_MARGIN_TOP ||
             property == CSS_PROP_MARGIN_BOTTOM) {
		xmlChar *vspace;
    
		if (strcmp((const char *) n->name, "img") == 0 ||
				strcmp((const char *) n->name, "applet") == 0)
			vspace = xmlGetProp(n, (const xmlChar *) "vspace");
		else
			vspace = NULL;
    
		if (vspace == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (parse_dimension((const char *) vspace, false,
                        &hint->data.length.value,
                        &hint->data.length.unit)) {
			hint->status = CSS_MARGIN_SET;
		} else {
			xmlFree(vspace);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(vspace);
    
		return CSS_OK;
	} else if (property == CSS_PROP_MARGIN_RIGHT ||
             property == CSS_PROP_MARGIN_LEFT) {
		xmlChar *hspace = NULL;
		xmlChar *align = NULL;
    
		if (strcmp((const char *) n->name, "img") == 0 ||
				strcmp((const char *) n->name, "applet") == 0) {
			hspace = xmlGetProp(n, (const xmlChar *) "hspace");
      
			if (hspace == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (parse_dimension((const char *) hspace, false,
                          &hint->data.length.value,
                          &hint->data.length.unit)) {
				hint->status = CSS_MARGIN_SET;
			} else {
				xmlFree(hspace);
				return CSS_PROPERTY_NOT_SET;
			}
      
			xmlFree(hspace);
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "table") == 0) {
			align = xmlGetProp(n, (const xmlChar *) "align");
      
			if (align == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (strcasecmp((const char *) align, "center") == 0 ||
					strcasecmp((const char *) align,
                     "abscenter") == 0 ||
					strcasecmp((const char *) align,
                     "middle") == 0 ||
					strcasecmp((const char *) align,
                     "absmiddle") == 0) {
            hint->status = CSS_MARGIN_AUTO;
          } else {
            xmlFree(align);
            return CSS_PROPERTY_NOT_SET;
          }
      
			xmlFree(align);
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "hr") == 0) {
			align = xmlGetProp(n, (const xmlChar *) "align");
      
			if (align == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (strcasecmp((const char *) align, "left") == 0) {
				if (property == CSS_PROP_MARGIN_LEFT) {
					hint->data.length.value = 0;
					hint->data.length.unit = CSS_UNIT_PX;
					hint->status = CSS_MARGIN_SET;
				} else {
					hint->status = CSS_MARGIN_AUTO;
				}
			} else if (strcasecmp((const char *) align,
                            "center") == 0) {
				hint->status = CSS_MARGIN_AUTO;
			} else if (strcasecmp((const char *) align,
                            "right") == 0) {
				if (property == CSS_PROP_MARGIN_RIGHT) {
					hint->data.length.value = 0;
					hint->data.length.unit = CSS_UNIT_PX;
					hint->status = CSS_MARGIN_SET;
				} else {
					hint->status = CSS_MARGIN_AUTO;
				}
			} else {
				xmlFree(align);
				return CSS_PROPERTY_NOT_SET;
			}
      
			xmlFree(align);
      
			return CSS_OK;
		}
	} else if (property == CSS_PROP_PADDING_TOP ||
             property == CSS_PROP_PADDING_RIGHT ||
             property == CSS_PROP_PADDING_BOTTOM ||
             property == CSS_PROP_PADDING_LEFT) {
		xmlChar *cellpadding = NULL;
    
		if (strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0) {
			/* Find table */
			for (n = n->parent; n != NULL &&
           n->type == XML_ELEMENT_NODE;
           n = n->parent) {
				if (strcmp((const char *) n->name, "table") ==
						0)
					break;
			}
      
			if (n != NULL)
				cellpadding = xmlGetProp(n,
                                 (const xmlChar *) "cellpadding");
		}
    
		if (cellpadding == NULL)
			return CSS_PROPERTY_NOT_SET;
    
		if (parse_dimension((const char *) cellpadding, false,
                        &hint->data.length.value,
                        &hint->data.length.unit)) {
			hint->status = CSS_PADDING_SET;
		} else {
			xmlFree(cellpadding);
			return CSS_PROPERTY_NOT_SET;
		}
    
		xmlFree(cellpadding);
    
		return CSS_OK;
	} else if (property == CSS_PROP_TEXT_ALIGN) {
		xmlChar *align = NULL;
    
		if (strcmp((const char *) n->name, "p") == 0 ||
				strcmp((const char *) n->name, "h1") == 0 ||
				strcmp((const char *) n->name, "h2") == 0 ||
				strcmp((const char *) n->name, "h3") == 0 ||
				strcmp((const char *) n->name, "h4") == 0 ||
				strcmp((const char *) n->name, "h5") == 0 ||
				strcmp((const char *) n->name, "h6") == 0) {
			align = xmlGetProp(n, (const xmlChar *) "align");
      
			if (align == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (strcasecmp((const char *) align, "left") == 0) {
				hint->status = CSS_TEXT_ALIGN_LEFT;
			} else if (strcasecmp((const char *) align,
                            "center") == 0) {
				hint->status = CSS_TEXT_ALIGN_CENTER;
			} else if (strcasecmp((const char *) align,
                            "right") == 0) {
				hint->status = CSS_TEXT_ALIGN_RIGHT;
			} else if (strcasecmp((const char *) align,
                            "justify") == 0) {
				hint->status = CSS_TEXT_ALIGN_JUSTIFY;
			} else {
				xmlFree(align);
				return CSS_PROPERTY_NOT_SET;
			}
      
			xmlFree(align);
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "center") == 0) {
			hint->status = CSS_TEXT_ALIGN_LIBCSS_CENTER;
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "caption") == 0) {
			align = xmlGetProp(n, (const xmlChar *) "align");
      
			if (align == NULL || strcasecmp((const char *) align,
                                      "center") == 0) {
				hint->status = CSS_TEXT_ALIGN_LIBCSS_CENTER;
			} else if (strcasecmp((const char *) align,
                            "left") == 0) {
				hint->status = CSS_TEXT_ALIGN_LIBCSS_LEFT;
			} else if (strcasecmp((const char *) align,
                            "right") == 0) {
				hint->status = CSS_TEXT_ALIGN_LIBCSS_RIGHT;
			} else if (strcasecmp((const char *) align,
                            "justify") == 0) {
				hint->status = CSS_TEXT_ALIGN_JUSTIFY;
			} else {
				xmlFree(align);
				return CSS_PROPERTY_NOT_SET;
			}
      
			if (align != NULL)
				xmlFree(align);
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "div") == 0 ||
               strcmp((const char *) n->name, "thead") == 0 ||
               strcmp((const char *) n->name, "tbody") == 0 ||
               strcmp((const char *) n->name, "tfoot") == 0 ||
               strcmp((const char *) n->name, "tr") == 0 ||
               strcmp((const char *) n->name, "td") == 0 ||
               strcmp((const char *) n->name, "th") == 0) {
			align = xmlGetProp(n, (const xmlChar *) "align");
      
			if (align == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (strcasecmp((const char *) align, "center") == 0) {
				hint->status = CSS_TEXT_ALIGN_LIBCSS_CENTER;
			} else if (strcasecmp((const char *) align,
                            "left") == 0) {
				hint->status = CSS_TEXT_ALIGN_LIBCSS_LEFT;
			} else if (strcasecmp((const char *) align,
                            "right") == 0) {
				hint->status = CSS_TEXT_ALIGN_LIBCSS_RIGHT;
			} else if (strcasecmp((const char *) align,
                            "justify") == 0) {
				hint->status = CSS_TEXT_ALIGN_JUSTIFY;
			} else {
				xmlFree(align);
				return CSS_PROPERTY_NOT_SET;
			}
      
			xmlFree(align);
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "table") == 0) {
			/* Tables usually reset alignment */
			hint->status = CSS_TEXT_ALIGN_INHERIT_IF_NON_MAGIC;
      
			return CSS_OK;
		} else {
			return CSS_PROPERTY_NOT_SET;
		}
	} else if (property == CSS_PROP_VERTICAL_ALIGN) {
		xmlChar *valign = NULL;
    
		if (strcmp((const char *) n->name, "col") == 0 ||
				strcmp((const char *) n->name, "thead") == 0 ||
				strcmp((const char *) n->name, "tbody") == 0 ||
				strcmp((const char *) n->name, "tfoot") == 0 ||
				strcmp((const char *) n->name, "tr") == 0 ||
				strcmp((const char *) n->name, "td") == 0 ||
				strcmp((const char *) n->name, "th") == 0) {
			valign = xmlGetProp(n, (const xmlChar *) "valign");
      
			if (valign == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (strcasecmp((const char *) valign, "top") == 0) {
				hint->status = CSS_VERTICAL_ALIGN_TOP;
			} else if (strcasecmp((const char *) valign,
                            "middle") == 0) {
				hint->status = CSS_VERTICAL_ALIGN_MIDDLE;
			} else if (strcasecmp((const char *) valign,
                            "bottom") == 0) {
				hint->status = CSS_VERTICAL_ALIGN_BOTTOM;
			} else if (strcasecmp((const char *) valign,
                            "baseline") == 0) {
				hint->status = CSS_VERTICAL_ALIGN_BASELINE;
			} else {
				xmlFree(valign);
				return CSS_PROPERTY_NOT_SET;
			}
      
			xmlFree(valign);
      
			return CSS_OK;
		} else if (strcmp((const char *) n->name, "applet") == 0 ||
               strcmp((const char *) n->name, "embed") == 0 ||
               strcmp((const char *) n->name, "iframe") == 0 ||
               strcmp((const char *) n->name, "img") == 0 ||
               strcmp((const char *) n->name, "object") == 0) {
			/** \todo input[type=image][align=*] - $11.3.3 */
			valign = xmlGetProp(n, (const xmlChar *) "align");
      
			if (valign == NULL)
				return CSS_PROPERTY_NOT_SET;
      
			if (strcasecmp((const char *) valign, "top") == 0) {
				hint->status = CSS_VERTICAL_ALIGN_TOP;
			} else if (strcasecmp((const char *) valign,
                            "bottom") == 0 ||
                 strcasecmp((const char *) valign,
                            "baseline") == 0) {
                   hint->status = CSS_VERTICAL_ALIGN_BASELINE;
                 } else if (strcasecmp((const char *) valign,
                                       "texttop") == 0) {
                   hint->status = CSS_VERTICAL_ALIGN_TEXT_TOP;
                 } else if (strcasecmp((const char *) valign,
                                       "absmiddle") == 0 ||
                            strcasecmp((const char *) valign,
                                       "abscenter") == 0) {
                              hint->status = CSS_VERTICAL_ALIGN_MIDDLE;
                            } else {
                              xmlFree(valign);
                              return CSS_PROPERTY_NOT_SET;
                            }
      
			xmlFree(valign);
      
			return CSS_OK;
		}
	}
  
	return CSS_PROPERTY_NOT_SET;
#endif
  
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


