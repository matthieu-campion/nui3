/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiWebCSS.h"

#undef Bool
#include "tidy.h"
#include "buffio.h"


//class nuiHTMLAttrib
nuiHTMLAttrib::nuiHTMLAttrib(AttributeType type, const nglString& rName, const nglString& rValue)
: mType(type),
mName(rName),
mValue(rValue)
{
}

nuiHTMLAttrib::nuiHTMLAttrib(const void* _tattrib, nglTextEncoding encoding)
{
  TidyAttr tattr = (TidyAttr)_tattrib;
  mType = (AttributeType)tidyAttrGetId(tattr);
  mName.Import(tidyAttrName(tattr), encoding);
  mValue.Import(tidyAttrValue(tattr), encoding);
}

nuiHTMLAttrib::~nuiHTMLAttrib()
{
}

nuiHTMLAttrib::AttributeType nuiHTMLAttrib::GetType() const
{
  return mType;
}

const nglString& nuiHTMLAttrib::GetName() const
{
  return mName;
}

const nglString& nuiHTMLAttrib::GetValue() const
{
  return mValue;
}

/////////////////////////////////
// class nuiHTMLNode
nuiHTMLNode::nuiHTMLNode(const nglString& rName, nuiHTMLNode::NodeType Type, nuiHTMLNode::TagType _tagType, const nglString& rText, nuiHTMLNode* pParent, bool ComputeStyle)
: mpParent(pParent), mpStyle(NULL), mpInlineStyle(NULL)
{
  mName = rName;
  mType = Type;
  mTagType = _tagType;
  mText = rText;
  mpParent = NULL;
  mpInlineStyle = NULL;

  if (ComputeStyle)
  {
    mpStyle = new nuiCSSStyle(this);
    nuiHTMLAttrib* pStyle = GetAttribute(nuiHTMLAttrib::eAttrib_STYLE);
    if (pStyle)
    {
      AddStyleSheet(GetSourceURL(), pStyle->GetValue(), true);
    }
  }
}

nuiHTMLNode::nuiHTMLNode(const void* _tdoc, const void* _tnod, nglTextEncoding encoding, nuiHTMLNode* pParent, bool ComputeStyle)
: mpParent(pParent), mpStyle(NULL), mpInlineStyle(NULL)
{
  SetFromNode(_tdoc, _tnod, encoding, ComputeStyle);
}


nuiHTMLNode::~nuiHTMLNode()
{
  Clear();
}

void nuiHTMLNode::Clear()
{
  for (uint32 i = 0; i < mChildren.size(); i++)
  {
    delete mChildren[i];
  }
  mChildren.clear();
  
  for (uint32 j = 0; j < mAttributes.size(); j++)
  {
    delete mAttributes[j];
  }
  mAttributes.clear();

  // stylesheets:
  for (uint32 i = 0; i < mStyleSheets.size(); i++)
  {
    delete mStyleSheets[i];
  }
  
  delete mpInlineStyle;
  mpInlineStyle = NULL;
  delete mpStyle;
  mpStyle = NULL;
}

void nuiHTMLNode::SetFromNode(const void* _tdoc, const void* _tnod, nglTextEncoding encoding, bool ComputeStyle)
{
  Clear();
  
  TidyDoc tdoc = (TidyDoc)_tdoc;
  TidyNode tnod = (TidyNode)_tnod;

  mName = nglString(tidyNodeGetName(tnod), encoding);
  mType = (NodeType)tidyNodeGetType(tnod);
  mTagType = (TagType)tidyNodeGetId(tnod);
  
  nglString text;
  TidyBuffer buf;
  tidyBufInit(&buf);
  if (tidyNodeGetValue(tdoc, tnod, &buf))
  {
    mText.Import((const char*)buf.bp, (int32)buf.size, encoding);
    //NGL_OUT(_T("<%s> %s\n"), mName.GetChars(), mText.GetChars());
  }
  tidyBufFree(&buf);
  
  // Fill the attributes:
  TidyAttr tattr;
  for (tattr = tidyAttrFirst(tnod); tattr; tattr = tidyAttrNext(tattr))
  {
    nuiHTMLAttrib* pAttrib = new nuiHTMLAttrib(tattr, encoding);
    mAttributes.push_back(pAttrib);
  }
  
  if (ComputeStyle)
  {
    mpStyle = new nuiCSSStyle(this);
    nuiHTMLAttrib* pStyle = GetAttribute(nuiHTMLAttrib::eAttrib_STYLE);
    if (pStyle)
    {
      AddStyleSheet(GetSourceURL(), pStyle->GetValue(), true);
    }
  }
}

nuiHTMLNode::NodeType nuiHTMLNode::GetType() const
{
  return mType;
}

nuiHTMLNode::TagType nuiHTMLNode::GetTagType() const
{
  return mTagType;
}

const nglString& nuiHTMLNode::GetName() const
{
  return mName;
}

const nglString& nuiHTMLNode::GetText() const
{
  return mText;
}

uint32 nuiHTMLNode::GetNbChildren() const
{
  return mChildren.size();
}

const nuiHTMLNode* nuiHTMLNode::GetChild(uint32 index) const
{
  return mChildren.at(index);
}

nuiHTMLNode* nuiHTMLNode::GetChild(uint32 index)
{
  return mChildren.at(index);
}

uint32 nuiHTMLNode::GetNbAttributes() const
{
  return mAttributes.size();
}

const nuiHTMLAttrib* nuiHTMLNode::GetAttribute(uint32 index) const
{
  return mAttributes.at(index);
}

const nuiHTMLAttrib* nuiHTMLNode::GetAttribute(const nglString& rAttribName) const
{
  for (uint32 i = 0; i < mAttributes.size(); i++)
  {
    if (mAttributes.at(i)->GetName() == rAttribName)
      return mAttributes.at(i);
  }
  return NULL;
}

nuiHTMLAttrib* nuiHTMLNode::GetAttribute(uint32 index)
{
  return mAttributes.at(index);
}

nuiHTMLAttrib* nuiHTMLNode::GetAttribute(const nglString& rAttribName)
{
  for (uint32 i = 0; i < mAttributes.size(); i++)
  {
    if (mAttributes.at(i)->GetName() == rAttribName)
      return mAttributes.at(i);
  }
  return NULL;
}

const nuiHTMLAttrib* nuiHTMLNode::GetAttribute(nuiHTMLAttrib::AttributeType attrib_type) const
{
  for (uint32 i = 0; i < mAttributes.size(); i++)
  {
    if (mAttributes.at(i)->GetType() == attrib_type)
      return mAttributes.at(i);
  }
  return NULL;
}

nuiHTMLAttrib* nuiHTMLNode::GetAttribute(nuiHTMLAttrib::AttributeType attrib_type)
{
  for (uint32 i = 0; i < mAttributes.size(); i++)
  {
    if (mAttributes.at(i)->GetType() == attrib_type)
      return mAttributes.at(i);
  }
  return NULL;
}

void nuiHTMLNode::BuildTree(const void* _tdoc, const void* _tnod, nglTextEncoding encoding, bool ComputeStyle)
{
  TidyDoc tdoc = (TidyDoc)_tdoc;
  TidyNode tnod = (TidyNode)_tnod;
  SetFromNode(_tdoc, _tnod, encoding, ComputeStyle);
  
  TidyNode child;
  
  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child))
  {
    nuiHTMLNode* pNode = new nuiHTMLNode(_tdoc, child, encoding, this, ComputeStyle);
    mChildren.push_back(pNode);
    pNode->BuildTree(tdoc, child, encoding, ComputeStyle);
  }
}

void nuiHTMLNode::GetSimpleText(nglString& rString) const
{
  if (!mText.IsNull())
  {
    rString += mText;
//    NGL_OUT(_T("text: %s\n"), mText.GetChars());
//    NGL_OUT(_T("tmp: %s\n"), rString.GetChars());
  }
  
  uint32 count = GetNbChildren();
  
  for (uint32 i = 0; i < count; i++)
  {
    const nuiHTMLNode* pNode = GetChild(i);
    pNode->GetSimpleText(rString);
  }
}

const nglString& nuiHTMLNode::GetSourceURL() const
{
  if (mpParent)
    return mpParent->GetSourceURL();
  return nglString::Null;
}

nuiHTMLNode* nuiHTMLNode::GetParent() const
{
  return mpParent;
}

void nuiHTMLNode::SetParent(nuiHTMLNode* pParent)
{
  mpParent = pParent;
}



//class nuiHTML : public nuiHTMLNode
nuiHTML::nuiHTML(bool ComputeStyle)
: nuiHTMLNode(nglString::Null, eNode_Root, eTag_UNKNOWN, nglString::Null, NULL, ComputeStyle),
  mComputeStyle(ComputeStyle)
{
}

nuiHTML::~nuiHTML()
{
}

class HTMLStream
{
public:
  HTMLStream(nglIStream& rStream)
  : mrStream(rStream)
  {
    
  }
  
  uint8 GetByte()
  {
    uint8 v;
    if (!mStack.empty())
    {
      v = mStack.top();
      mStack.pop();
    }
    else
    {
      mrStream.Read(&v, 1, 1);
    }
    
    return v;
  }
  
  void Unget(uint8 v)
  {
    mStack.push(v);
  }
  
  bool IsEOF()
  {
    return mStack.empty() && mrStream.Available(1) == 0;
  }

  static int TIDY_CALL TidyGetByte(void* sourceData)
  {
    HTMLStream* pStream = (HTMLStream*)sourceData;
    return pStream->GetByte();
  }
  
  static void TIDY_CALL TidyUngetByte(void* sourceData, byte bt)
  {
    HTMLStream* pStream = (HTMLStream*)sourceData;
    pStream->Unget(bt);
  }

  static Bool TIDY_CALL TidyEOF(void* sourceData)
  {
    HTMLStream* pStream = (HTMLStream*)sourceData;
    return pStream->IsEOF() ? yes : no;
  }
  
private:
  nglIStream& mrStream;
  std::stack<uint8> mStack; 
};

static nglString GetEncodingString(TidyNode tnod)
{
  if (tidyNodeGetId(tnod) == TidyTag_META)
  {
    // Search for the encoding attribute
    TidyAttr attr_content = tidyAttrGetById(tnod, TidyAttr_CONTENT);
    TidyAttr attr_httpequiv = tidyAttrGetById(tnod, TidyAttr_HTTP_EQUIV);
    if (attr_content && attr_httpequiv)
    {
      nglString contenttype(tidyAttrValue(attr_content));
      if (contenttype.Compare(_T("content-type"), false) != 0)
      {
        // bleh...
      }
      nglString encoding(tidyAttrValue(attr_content));
      //NGL_OUT(_T("content found in the tree: %s"), encoding.GetChars());
      int32 col = encoding.Find(_T("charset="));
      encoding = encoding.Extract(col + 8);
      //NGL_OUT(_T("encoding found in the tree: %s"), encoding.GetChars());
      return encoding;
    }
  }
  
  TidyNode child;
  
  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child))
  {
    nglString str(GetEncodingString(child));
    if (!str.IsNull())
      return str;
  }
  
  
  return nglString::Null;
}

bool nuiHTML::Load(nglIStream& rStream, nglTextEncoding OverrideContentsEncoding, const nglString& rSourceURL)
{
  if (!rSourceURL.IsEmpty())
    SetSourceURL(rSourceURL);
  
  int res = -1;
  nglTextEncoding encoding = eUTF8;
  TidyDoc tdoc = NULL;
  {
    HTMLStream strm(rStream);
    tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyShowMarkup, no);
    tidyOptSetBool(tdoc, TidyShowWarnings, no);
    tidyOptSetInt(tdoc, TidyShowErrors, 0);
    tidyOptSetBool(tdoc, TidyQuiet, yes);
    tidySetCharEncoding(tdoc, "utf8");
    
    TidyInputSource source;
    tidyInitSource( &source, &strm, &HTMLStream::TidyGetByte, &HTMLStream::TidyUngetByte, &HTMLStream::TidyEOF);
    res = tidyParseSource(tdoc, &source);
    
    if ( res >= 0 )
      res = tidyCleanAndRepair(tdoc);               // Tidy it up!
    if ( res >= 0 )
      res = tidyRunDiagnostics(tdoc);               // Kvetch
  
    if (OverrideContentsEncoding == eEncodingUnknown)
    {
      nglString encoding_string(GetEncodingString(tidyGetRoot(tdoc)));
      
      //ascii, latin1, raw, utf8, iso2022, mac, win1252, utf16le, utf16be, utf16, big5 shiftjis
      encoding = nuiGetTextEncodingFromString(encoding_string);
    }
    else
    {
      encoding = OverrideContentsEncoding;
    }
  }
  
  char* pStr = NULL;

  if (encoding != eUTF8)
  {
    // Release the doc to create a new one
    tidyRelease(tdoc);
    
    nglOMemory omem;
    rStream.SetPos(0, eStreamFromStart);
    rStream.PipeTo(omem);
    nglString decoded;
    decoded.Import(omem.GetBufferData(), omem.GetSize(), encoding);
    pStr = decoded.Export(eUTF8);
    nglIMemory imem(pStr, strlen(pStr));
    
    HTMLStream strm(imem);
    tdoc = tidyCreate();
    tidySetCharEncoding(tdoc, "utf8");

    TidyInputSource source;
    tidyInitSource( &source, &strm, &HTMLStream::TidyGetByte, &HTMLStream::TidyUngetByte, &HTMLStream::TidyEOF);
    res = tidyParseSource(tdoc, &source);
    if ( res >= 0 )
      res = tidyCleanAndRepair(tdoc);               // Tidy it up!
    if ( res >= 0 )
      res = tidyRunDiagnostics(tdoc);               // Kvetch
  }    
    
  BuildTree(tdoc, tidyGetRoot(tdoc), eUTF8, mComputeStyle);
  
  tidyRelease(tdoc);
  
  if (pStr)
    free(pStr);
  
  return res < 2;
}
 
const nglString& nuiHTML::GetSourceURL() const
{
  return mSourceURL;
  
}

void nuiHTML::SetSourceURL(const nglString& rURL)
{
  mSourceURL = rURL;
  printf("New source URL (0x%x): %s\n", this, mSourceURL.GetChars());
}

// Stolen from nglPath!
static int32 GetRootPart(const nglString& rStr)
{
	if (rStr[0] == _T('/'))
	{
		if (rStr[1] != _T('/'))
			return 1;
    
		// //host[/path] (network address)
		// or /volume[/path] (standard unix like path)
		int32 end = rStr.Find(_T('/'), 2);
		return ((end > 0) ? end : rStr.GetLength());
	}
  
  // Find the protocol name:
  int col = rStr.Find(_T("://"), 0, true);
  
  return MIN(col + 3, rStr.GetLength());
}


static bool Canonize(nglString& rStr)
{
  nglString canon;
  int32 len = rStr.GetLength();
  int32 root_part = GetRootPart(rStr);
  int32 last_slash = root_part;
  int32 slash = 0;
  
  canon = rStr.GetLeft(root_part);
  while (slash < len)
  {
    slash = rStr.Find(_T('/'), last_slash);
    if (slash == - 1)
      slash = len;
    
    if (((slash - last_slash) == 1) && (rStr.GetChar(last_slash) == _T('.')))
    {
      // Ignore '.'
    }
    else
      if (((slash - last_slash) == 2) && (!rStr.Compare(_T(".."), last_slash, 2)))
      {
        // Interpret '..'
        int32 prev_slash = canon.FindLast(_T('/'));
        if (prev_slash < root_part)
          prev_slash = root_part;
        
        if (!canon.IsEmpty() && canon.Compare(_T(".."), canon.GetLength() - 2, 2))
          canon.Delete(prev_slash);
        else
        {
          if (canon.GetLength() > root_part)
            canon += _T('/');
          canon += _T("..");
        }
      }
      else
      {
        // Simply append path node
        nglString node = rStr.Extract(last_slash, (slash - last_slash));
        if (canon.GetLength() > root_part)
          canon += _T('/');
        canon += node;
      }
    
    last_slash = slash + 1;
  }
  
  rStr = canon;
  return true;
}

void nuiHTML::GetAbsoluteURL(const nglString& rBaseURL, nglString& url)
{
  int32 colon = url.Find(':');
  if (colon > 0)
  {
    // complete url link
  }
  else if (url[0] == '/')
  {
    // Site absolute
    int32 col = rBaseURL.Find(_T("://"));
    if (col > 0)
    {
      int32 end = rBaseURL.Find('/', col + 3);
      if (end)
        url = rBaseURL.Extract(0, end) + url;
    }
  }
  else
  {
    // Site relative
    int32 end = rBaseURL.FindLast(_T('/'));
    if (end >= 0)
    {
      url = rBaseURL.Extract(0, end + 1) + url;
    }
    else
    {
      url = rBaseURL + _T("/") + url;
    }
  }
  
  Canonize(url);
}


//////// Style Sheets:
void nuiHTMLNode::AddStyleSheet(const nglString& rBaseURL, const nglString& rText, bool Inline)
{
  nuiCSSStyleSheet* pCSS = nuiCSSEngine::CreateStyleSheet(rBaseURL, rText, Inline, nuiMakeDelegate(this, &nuiHTMLNode::StyleSheetDone));
  if (pCSS)
  {
    if (Inline)
    {
      delete mpInlineStyle;
      mpInlineStyle = pCSS;
    }
    else
      mStyleSheets.push_back(pCSS);
  }
}

void nuiHTMLNode::AddStyleSheet(const nglString& rURL)
{
  nuiCSSStyleSheet* pCSS = nuiCSSEngine::CreateStyleSheet(rURL, nuiMakeDelegate(this, &nuiHTMLNode::StyleSheetDone));
  if (pCSS)
  {
    mStyleSheets.push_back(pCSS);
  }
}

const std::vector<nuiCSSStyleSheet*>& nuiHTMLNode::GetStyleSheets() const
{
  return mStyleSheets;
}

const nuiCSSStyleSheet* nuiHTMLNode::GetInlineStyle() const
{
  return mpInlineStyle;
}

nuiCSSStyle& nuiHTMLNode::GetStyle()
{
  NGL_ASSERT(mpStyle);
  return *mpStyle;
}

#include "nuiHTMLContext.h"

void nuiHTMLNode::UpdateStyle(nuiHTMLContext& rContext, bool Force)
{
  nuiHTMLContext ct(rContext);
  if (mpStyle)
  {
    for (uint32 i = 0; i < mStyleSheets.size(); i++)
      ct.mpStyleSheets.push_back(mStyleSheets[i]);
    
    if (GetInlineStyle())
      ct.mpStyleSheets.push_back(GetInlineStyle());
    
    nuiCSSContext ctx;
    ctx.Select(ct, this);
  }

  for (uint32 i = 0; i < mChildren.size(); i++)
    mChildren[i]->UpdateStyle(ct);
  
}

void nuiHTMLNode::StyleSheetDone(nuiCSSStyleSheet* pCSS)
{
  NGL_ASSERT(pCSS);
  if (!pCSS->IsValid())
  {
    delete pCSS;
    for (uint32 i = 0; i < mStyleSheets.size(); i++)
    {
      if (mStyleSheets[i] == pCSS)
      {
        mStyleSheets.erase(mStyleSheets.begin() + i);
        return;
      }
    }
  }
  
  LayoutInvalidated();
}


