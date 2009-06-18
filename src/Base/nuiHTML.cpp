/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiHTML.h"

#include "tidy.h"
#include "buffio.h"

//class nuiHTMLAttrib
nuiHTMLAttrib::nuiHTMLAttrib(AttributeType type, const nglString& rName, const nglString& rValue)
: mType(type),
mName(rName),
mValue(rValue)
{
}

nuiHTMLAttrib::nuiHTMLAttrib(const void* _tattrib)
{
  TidyAttr tattr = (TidyAttr)_tattrib;
  mType = (AttributeType)tidyAttrGetId(tattr);
  mName = tidyAttrName(tattr);
  mValue = tidyAttrValue(tattr);
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
nuiHTMLNode::nuiHTMLNode(const nglString& rName, nuiHTMLNode::NodeType Type, nuiHTMLNode::TagType TagType, const nglString& rText)
{
  mName = rName;
  mType = Type;
  mTagType = TagType;
  mText = rText;
}

nuiHTMLNode::nuiHTMLNode(const void* _tdoc, const void* _tnod)
{
  SetFromNode(_tdoc, _tnod);
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

void nuiHTMLNode::SetFromNode(const void* _tdoc, const void* _tnod)
{
  Clear();
  
  TidyDoc tdoc = (TidyDoc)_tdoc;
  TidyNode tnod = (TidyNode)_tnod;
  
  nglString text;
  TidyBuffer buf;
  tidyBufInit(&buf);
  if (tidyNodeGetValue(tdoc, tnod, &buf))
  {
    mText.Import((const char*)buf.bp, (int32)buf.size, eUTF8);
    //NGL_OUT(_T("text: %ls\n"), mText.GetChars());
  }
  tidyBufFree(&buf);
  
  mName = nglString(tidyNodeGetName(tnod));
  mType = (NodeType)tidyNodeGetType(tnod);
  
  // Fill the attributes:
  TidyAttr tattr;
  for (tattr = tidyAttrFirst(tnod); tattr; tattr = tidyAttrNext(tattr))
  {
    nuiHTMLAttrib* pAttrib = new nuiHTMLAttrib(tattr);
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

void nuiHTMLNode::BuildTree(const void* _tdoc, const void* _tnod)
{
  TidyDoc tdoc = (TidyDoc)_tdoc;
  TidyNode tnod = (TidyNode)_tnod;
  SetFromNode(_tdoc, _tnod);
  
  TidyNode child;
  
  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child))
  {
    nuiHTMLNode* pNode = new nuiHTMLNode(_tdoc, child);
    mChildren.push_back(pNode);
    pNode->BuildTree(tdoc, child);
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

bool nuiHTML::Load(nglIStream& rStream)
{
  HTMLStream strm(rStream);
  TidyDoc tdoc = tidyCreate();

  TidyInputSource source;
  tidyInitSource( &source, &strm, &HTMLStream::TidyGetByte, &HTMLStream::TidyUngetByte, &HTMLStream::TidyEOF);
  
  int res = tidyParseSource(tdoc, &source);
  
  BuildTree(tdoc, tidyGetRoot(tdoc));
  
  tidyRelease(tdoc);
  
  return res < 2;
}
 
