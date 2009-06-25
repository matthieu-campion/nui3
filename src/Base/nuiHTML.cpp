/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiHTML.h"
#include "nglIMemory.h"
#include "nglOMemory.h"
#include "nuiUnicode.h"

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
nuiHTMLNode::nuiHTMLNode(const nglString& rName, nuiHTMLNode::NodeType Type, nuiHTMLNode::TagType _tagType, const nglString& rText)
{
  mName = rName;
  mType = Type;
  mTagType = _tagType;
  mText = rText;
  mpParent = NULL;
}

nuiHTMLNode::nuiHTMLNode(const void* _tdoc, const void* _tnod, nglTextEncoding encoding)
{
  SetFromNode(_tdoc, _tnod, encoding);
  mpParent = NULL;
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
}

void nuiHTMLNode::SetFromNode(const void* _tdoc, const void* _tnod, nglTextEncoding encoding)
{
  Clear();
  
  TidyDoc tdoc = (TidyDoc)_tdoc;
  TidyNode tnod = (TidyNode)_tnod;
  
  nglString text;
  TidyBuffer buf;
  tidyBufInit(&buf);
  if (tidyNodeGetValue(tdoc, tnod, &buf))
  {
    mText.Import((const char*)buf.bp, (int32)buf.size, encoding);
    //NGL_OUT(_T("text: %ls\n"), mText.GetChars());
  }
  tidyBufFree(&buf);
  
  mName = nglString(tidyNodeGetName(tnod), encoding);
  mType = (NodeType)tidyNodeGetType(tnod);
  mTagType = (TagType)tidyNodeGetId(tnod);
  
  // Fill the attributes:
  TidyAttr tattr;
  for (tattr = tidyAttrFirst(tnod); tattr; tattr = tidyAttrNext(tattr))
  {
    nuiHTMLAttrib* pAttrib = new nuiHTMLAttrib(tattr, encoding);
    mAttributes.push_back(pAttrib);
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

void nuiHTMLNode::BuildTree(const void* _tdoc, const void* _tnod, nglTextEncoding encoding)
{
  TidyDoc tdoc = (TidyDoc)_tdoc;
  TidyNode tnod = (TidyNode)_tnod;
  SetFromNode(_tdoc, _tnod, encoding);
  
  TidyNode child;
  
  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child))
  {
    nuiHTMLNode* pNode = new nuiHTMLNode(_tdoc, child, encoding);
    pNode->SetParent(this);
    mChildren.push_back(pNode);
    pNode->BuildTree(tdoc, child, encoding);
  }
}

void nuiHTMLNode::GetSimpleText(nglString& rString) const
{
  if (!mText.IsNull())
  {
    rString += mText;
//    NGL_OUT(_T("text: %ls\n"), mText.GetChars());
//    NGL_OUT(_T("tmp: %ls\n"), rString.GetChars());
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

nuiHTMLNode* nuiHTMLNode::SetParent(nuiHTMLNode* pParent)
{
  mpParent = pParent;
}



//class nuiHTML : public nuiHTMLNode
nuiHTML::nuiHTML()
: nuiHTMLNode(nglString::Null, eNode_Root, eTag_UNKNOWN, nglString::Null)
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
      NGL_OUT(_T("content found in the tree: %ls"), encoding.GetChars());
      int32 col = encoding.Find(_T("charset="));
      encoding = encoding.Extract(col + 8);
      NGL_OUT(_T("encoding found in the tree: %ls"), encoding.GetChars());
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

bool nuiHTML::Load(nglIStream& rStream, nglTextEncoding OverrideContentsEncoding)
{
  int res = -1;
  nglTextEncoding encoding = eUTF8;
  TidyDoc tdoc = NULL;
  {
    HTMLStream strm(rStream);
    tdoc = tidyCreate();
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
  
  if (encoding != eUTF8)
  {
    // Release the doc to create a new one
    tidyRelease(tdoc);
    
    nglOMemory omem;
    rStream.SetPos(0, eStreamFromStart);
    rStream.PipeTo(omem);
    nglString decoded;
    decoded.Import(omem.GetBufferData(), omem.GetSize(), encoding);
    std::string str(decoded.GetStdString(), eUTF8);
    printf("result:\n\n");
    write(0, str.c_str(), str.size());
    nglIMemory imem(str.c_str(), str.size());
    
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
    
  BuildTree(tdoc, tidyGetRoot(tdoc), eUTF8);
  
  tidyRelease(tdoc);
  
  return res < 2;
}
 
const nglString& nuiHTML::GetSourceURL() const
{
  return mSourceURL;
  
}

void nuiHTML::SetSourceURL(const nglString& rURL)
{
  mSourceURL = rURL;
}

