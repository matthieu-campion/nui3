/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiXML.h"
#include "nuiFlags.h"
#define XML_STATIC
#include "expat.h"

using namespace std;

// Helper functions:
void XMLizeString(nglString& str, const nglString& src)
{
  str = src;
  str.EncodeToXML();
}

void DeXMLizeString(nglString& str)
{
  str.DecodeFromXML();
}

class xmlLexer
  {
  public:
    xmlLexer(nglIStream* pStream);
    virtual ~xmlLexer();
    
    bool GetChar();
    
    bool SkipBlank();
    bool GetToken(nglString& rToken);
    
    bool AddEntity(const nglString& rSymbol, const nglString& rValue);
  protected:
    bool InternalGetToken(nglString& rToken);
    nglSize ReadLine (nglString& rLine, nglTextFormat* pFormat = NULL);
    
    nglIStream* mpStream;
    nglString mTempLine;
    int mCharPos;
    nglChar mChar;
    
    nuiXMLAttributeList mEntities;
  };

xmlLexer::xmlLexer(nglIStream* pStream)
{
  mpStream = pStream;
  mCharPos = 0;
  mChar = ' ';
}

xmlLexer::~xmlLexer()
{
}

#define BUFFER_SIZE 1024

nglSize xmlLexer::ReadLine (nglString& rLine, nglTextFormat* pFormat)
{
  nglSize size = mpStream->ReadLine(rLine, pFormat);
  //wprintf(_T("Read line from XML: %d chars '%s'\n"), size, rLine.GetChars());
  return size;
}

bool xmlLexer::GetChar()
{
  if (mCharPos >= mTempLine.GetLength())
  {
    mCharPos = 0;
    if (!ReadLine(mTempLine))
      return false;
  }
  
  mChar = mTempLine.GetChar(mCharPos++);
  return true;
}

bool xmlLexer::SkipBlank()
{
  bool res=true;
  while ((mChar==' ' || mChar==0xa || mChar==0xd || mChar=='\t') && res) 
    res = GetChar();
  return res;
}

bool xmlLexer::GetToken(nglString& rToken)
{
  bool res = InternalGetToken(rToken);
  
  nuiXMLAttributeList::iterator it = mEntities.begin();
  nuiXMLAttributeList::iterator end = mEntities.end();
  
  int pos = 0;
  for ( ; it != end; ++it)
  {
    pos = 0;
    nglString tok = ((*it).first);
    nglString val = ((*it).second);
    int toklen = tok.GetLength();
    int vallen = val.GetLength();
    
    pos = rToken.Find(tok, pos);
    while (pos != -1)
    {
      rToken.Replace(pos,toklen,val);
      pos += vallen;
      pos = rToken.Find(tok, pos);
    }
  }
  
  return res;
}

bool xmlLexer::InternalGetToken(nglString& rToken)
{
  int TagToSkip = 0;
  rToken.Wipe();
  
  if (!SkipBlank())
    return false;
  
  if (mChar=='<')
  {
    rToken+=mChar;
    while (GetChar())
    {
      rToken+=mChar;
      if (mChar == '<')
      {
        TagToSkip++;
      }
      
      if (mChar == '>')
      {
        if (!TagToSkip)
        {
          GetChar();
          return true;
        }
        else
          TagToSkip--;
      }
    }
    // Test the last char of the file:
    if (mChar == '>')
    {
      rToken+=mChar;
      return true;
    }
  }
  else
  {
    rToken+=mChar;
    while (GetChar())
    {
      if (mChar == '<')
      {
        rToken.TrimRight();
        return true;
      }
      rToken+=mChar;
    }
  }
  return false;
}

bool xmlLexer::AddEntity(const nglString& rName, const nglString& rValue)
{
  mEntities[rName] = rValue;
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

nuiXMLParser::nuiXMLParser()
{
  mParser = XML_ParserCreate(NULL);
  XML_SetUserData(mParser, this);
  XML_SetStartElementHandler(mParser, &nuiXMLParser::StartElement);
  XML_SetEndElementHandler(mParser, &nuiXMLParser::EndElement);
  XML_SetCharacterDataHandler(mParser, &nuiXMLParser::Characters);
  XML_SetProcessingInstructionHandler(mParser, &nuiXMLParser::ProcessingInstruction);
  XML_SetCommentHandler(mParser, &nuiXMLParser::Comment);
}

nuiXMLParser::~nuiXMLParser()
{
  XML_ParserFree(mParser);
}

bool nuiXMLParser::Parse(nglIStream* pStream)
{
  mpStream = pStream;
  
  bool done = false;
  do
  {
    char pBuf[BufferSize];
    nglSize size = mpStream->Read(pBuf, BufferSize, 1);
    if (size == 0 && mpStream->GetState() != eStreamWait)
      return false;
    
    XML_Status xmlStatus = XML_Parse(mParser, pBuf, size, mpStream->GetState() == eStreamEnd);
    if (xmlStatus == XML_STATUS_ERROR)
      return false;
    
    XML_ParsingStatus parsingStatus;
    XML_GetParsingStatus(mParser, &parsingStatus);
    if (parsingStatus.parsing == XML_FINISHED)
      return true;
  }
  while(!done);
  return true;
}

void nuiXMLParser::Stop()
{
  XML_StopParser(mParser, false);
}

void nuiXMLParser::StartElement(void* pThis, const nuiXML_Char* name, const nuiXML_Char** atts)
{
  ((nuiXMLParser*)pThis)->StartElement(name, atts);
}

void nuiXMLParser::EndElement(void* pThis, const nuiXML_Char* name)
{
  ((nuiXMLParser*)pThis)->EndElement(name);
}

void nuiXMLParser::Characters(void* pThis, const nuiXML_Char* s, int len)
{
  ((nuiXMLParser*)pThis)->Characters(s, len);
}

void nuiXMLParser::ProcessingInstruction(void* pThis, const nuiXML_Char* target, const nuiXML_Char* data)
{
  ((nuiXMLParser*)pThis)->ProcessingInstruction(target, data);
}

void nuiXMLParser::Comment(void* pThis, const nuiXML_Char* data)
{
  ((nuiXMLParser*)pThis)->Comment(data);
}

void nuiXMLParser::StartElement(const nuiXML_Char* name, const nuiXML_Char** atts)
{
}

void nuiXMLParser::EndElement(const nuiXML_Char* name)
{
}

void nuiXMLParser::Characters(const nuiXML_Char* s, int len)
{
}

void nuiXMLParser::ProcessingInstruction(const nuiXML_Char* target, const nuiXML_Char* data)
{
}

void nuiXMLParser::Comment(const nuiXML_Char* data)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class nuiXMLBuilder : public nuiXMLParser
{
public:
  nuiXMLBuilder();
  virtual ~nuiXMLBuilder();
  
  virtual bool Parse(nglIStream* pStream, nuiXML* pRoot);
  
  virtual void StartElement(const nuiXML_Char* name, const nuiXML_Char** atts);
  virtual void EndElement(const nuiXML_Char* name);
  virtual void Characters(const nuiXML_Char* s, int len);
  virtual void ProcessingInstruction(const nuiXML_Char* target, const nuiXML_Char* data);
  virtual void Comment(const nuiXML_Char* data);
protected:
  
  nglIStream* mpStream;
  
  //    static const uint32 BufferSize = 1024;
  static const uint32 BufferSize = 4096;
  
  nuiXML* mpRootNode;
  nuiXMLNode* mpCurrentNode;
  bool mIsRootNode;
  bool mIsTextNode;
  
  XML_Parser mParser;
};

nuiXMLBuilder::nuiXMLBuilder()
{
}

nuiXMLBuilder::~nuiXMLBuilder()
{
}

bool nuiXMLBuilder::Parse(nglIStream* pStream, nuiXML* pRoot)
{
  mpRootNode = pRoot;
  mpCurrentNode = pRoot;
  mIsRootNode = true;
  mIsTextNode = false;
  
  return nuiXMLParser::Parse(pStream);
}

void nuiXMLBuilder::StartElement(const XML_Char* name, const XML_Char** atts)
{
  if (!mIsRootNode)
  {
    if (mIsTextNode)
    {
      mpCurrentNode = mpCurrentNode->GetParent();
      mIsTextNode = false;
    }
    mpCurrentNode = new nuiXMLNode(nglString(name, eUTF8), mpCurrentNode);
  }
  else
  {
    mpCurrentNode->SetName(nglString(name, eUTF8));
  }
  
  for (uint i = 0; atts[i]; i += 2)
  {
    mpCurrentNode->SetAttribute(nglString(atts[i], eUTF8), nglString(atts[i+1], eUTF8));
  }
  
  mIsRootNode = false;
}

void nuiXMLBuilder::EndElement(const XML_Char* name)
{
  if (mpCurrentNode != mpRootNode)
  {
    if (mIsTextNode)
    {
      mpCurrentNode = mpCurrentNode->GetParent();
      mIsTextNode = false;
    }
    mpCurrentNode = mpCurrentNode->GetParent();
  }
}

void nuiXMLBuilder::Characters(const XML_Char* s, int len)
{
  bool isWhiteSpace = true;
  for (int i = 0; i<len && isWhiteSpace; i++)
  {
    if (s[i] != ' '  &&
        s[i] != '\n' &&
        s[i] != '\t')
    {
      isWhiteSpace = false;
    }
  }
  
  if (!isWhiteSpace)
  {
    if (!mIsTextNode)
    {
      mpCurrentNode = new nuiXMLNode(_T("##text"), mpCurrentNode);
      mIsTextNode = true;
      mIsRootNode = false;
    }
    mpCurrentNode->SetValue(mpCurrentNode->GetValue() + nglString(s, len, eUTF8));
  }
}

void nuiXMLBuilder::ProcessingInstruction(const XML_Char* target, const XML_Char* data)
{
  nuiXMLNode* pNode = new nuiXMLNode(_T("##comment"), mpCurrentNode);
  pNode->SetValue(nglString(target, eUTF8) + _T(" ") + nglString(data, eUTF8));
}

void nuiXMLBuilder::Comment(const XML_Char* data)
{
  nuiXMLNode* pNode = new nuiXMLNode(_T("##comment"), mpCurrentNode);
  pNode->SetValue(nglString(data, eUTF8));
}

///////////////////
// nuiXMLNode
nuiXMLNode::nuiXMLNode(const nglString& rName, nuiXMLNode* pParent)
{
  mName = rName;
  mpParent = pParent;
  mpTag = NULL;
  if (pParent)
    pParent->AddChild(this);
  
}

nuiXMLNode::nuiXMLNode(const char* pName, nuiXMLNode* pParent)
{
  mName = nglString(pName);
  mpParent = pParent;
  mpTag = NULL;
  if (pParent)
    pParent->AddChild(this);
  
}

nuiXMLNode::nuiXMLNode(const nuiXMLNode& rOriginal)
{
  // Private method: no copy constructor allowed!
}

nuiXMLNode::~nuiXMLNode()
{
  list<nuiXMLNode*>::iterator it;
  list<nuiXMLNode*>::iterator end = mpChildren.end();
  
  for (it = mpChildren.begin(); it!=end; ++it)
    delete (*it);
}

nuiXMLNode* nuiXMLNode::Clone(nuiXMLNode* pParent) const
{
  nuiXMLNode* pNode = new nuiXMLNode(mName,pParent);
  
  pNode->mValue = mValue;
  pNode->mAttributes = mAttributes;
  
  list<nuiXMLNode*>::const_iterator it;
  list<nuiXMLNode*>::const_iterator end = mpChildren.end();
  for (it = mpChildren.begin(); it!=end; ++it)
    (*it)->Clone(pNode);
  
  return pNode;
}

// Node name management:
const nglString& nuiXMLNode::GetName() const
{
  return mName;
}

void nuiXMLNode::SetName(const nglString& rName)
{
  mName = rName;
}

void nuiXMLNode::SetName(const char* pName)
{
  mName = nglString(pName);
}

const nglString& nuiXMLNode::GetValue() const
{
  return mValue;
}

void nuiXMLNode::SetValue(const nglString& rValue)
{
  mValue = rValue;
  if (mName.GetLeft(2) != _T("##"))
    mName = _T("##") + mName;
}

void nuiXMLNode::SetValue(const char* pValue)
{
  mValue = nglString(pValue);
  if (mName.GetLeft(2) != _T("##"))
    mName = _T("##") + mName;
}

// Node attributes management:
const nglString& nuiXMLNode::GetAttribute (const nglString& rName) const
{
  nuiXMLAttributeList::const_iterator it = mAttributes.find(rName);
  if (it == mAttributes.end())
    return nglString::Null;
  return (*it).second;
}

void nuiXMLNode::SetAttribute (const nglString& rName, const nglString& rValue)
{
  mAttributes[rName] = rValue;
}

void nuiXMLNode::SetAttribute (const nglString& rName, const nglChar* pValue)
{
  if (pValue)
    mAttributes[rName] = pValue;
  else
    DelAttribute(rName);
}


void nuiXMLNode::SetAttribute(const nglString& rName, uint8 value)
{
  nglString val;
  val.SetCUInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, int8 value)
{
  nglString val;
  val.SetCInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, uint16 value)
{
  nglString val;
  val.SetCUInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, int16 value)
{
  nglString val;
  val.SetCInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, uint32 value)
{
  nglString val;
  val.SetCUInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, int32 value)
{
  nglString val;
  val.SetCInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, uint64 value)
{
  nglString val;
  val.SetCUInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, int64 value)
{
  nglString val;
  val.SetCInt(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, bool value)
{
  nglString val = value?_T("true"):_T("false");
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, float value)
{
  nglString val;
  val.SetCFloat(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, double value)
{
  nglString val;
  val.SetCDouble(value);
  SetAttribute(rName,val);
}

void nuiXMLNode::SetAttribute(const nglString& rName, nuiPosition value)
{
  SetAttribute(rName,nuiGetPosition(value));
}

void nuiXMLNode::SetAttribute(const nglString& rName, nuiDirection value)
{
  SetAttribute(rName,nuiGetDirection(value));
}

void nuiXMLNode::SetAttribute(const nglString& rName, nuiOrientation value)
{
  SetAttribute(rName,nuiGetOrientation(value));
}


bool nuiXMLNode::DelAttribute(const nglString& rName)
{    
  bool res = HasAttribute(rName);
  mAttributes.erase(rName);
  return res;
}

bool nuiXMLNode::HasAttribute(const nglString& rName) const
{
  return mAttributes.find(rName) != mAttributes.end();
}
///////////////////////////
// char* versions
const nglString& nuiXMLNode::GetAttribute(const char* pName) const
{
  nglString name(pName);
  nuiXMLAttributeList::const_iterator it = mAttributes.find(name);
  if (it == mAttributes.end())
    return nglString::Null;
  return (*it).second;
}

void nuiXMLNode::SetAttribute(const char* pName, const nglString& rValue)
{
  nglString name(pName);
  mAttributes[name] = rValue;
}

void nuiXMLNode::SetAttribute (const char* pName, const char* pValue)
{
  nglString name(pName);
  if (pValue)
  {
    nglString value(pValue);
    mAttributes[name] = value;
  }
  else
    DelAttribute(name);
}

void nuiXMLNode::SetAttribute(const char* pName, uint8 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, int8 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, uint16 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, int16 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, uint32 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, int32 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, uint64 value)
{
  nglString name(pName);
  nglString val;
  val.SetCUInt(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, int64 value)
{
  nglString name(pName);
  nglString val;
  val.SetCInt(value);
  SetAttribute(pName,val);
}

void nuiXMLNode::SetAttribute(const char* pName, bool value)
{
  nglString name(pName);
  nglString val = value?_T("true"):_T("false");
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, float value)
{
  nglString name(pName);
  nglString val;
  val.SetCFloat(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, double value)
{
  nglString name(pName);
  nglString val;
  val.SetCDouble(value);
  SetAttribute(name,val);
}

void nuiXMLNode::SetAttribute(const char* pName, nuiPosition value)
{
  nglString name(pName);
  SetAttribute(name,nuiGetPosition(value));
}

void nuiXMLNode::SetAttribute(const char* pName, nuiDirection value)
{
  nglString name(pName);
  SetAttribute(name,nuiGetDirection(value));
}

void nuiXMLNode::SetAttribute(const char* pName, nuiOrientation value)
{
  nglString name(pName);
  SetAttribute(name,nuiGetOrientation(value));
}


bool nuiXMLNode::DelAttribute (const char* pName)
{    
  nglString name(pName);
  bool res = HasAttribute(name);
  mAttributes.erase(name);
  return res;
}

bool nuiXMLNode::HasAttribute (const char* pName) const
{
  return mAttributes.find(nglString(pName)) != mAttributes.end();
}


///////////////////////////
uint nuiXMLNode::GetAttributeCount () const
{
  return (uint)mAttributes.size();
}

const nglString& nuiXMLNode::GetAttributeName (uint index) const
{
  uint i;
  nuiXMLAttributeList::const_iterator it;
  nuiXMLAttributeList::const_iterator end = mAttributes.end();
  
  for (it = mAttributes.begin(), i=0 ; it!=end && i<index; ++it)
    i++;
  
  if (i == index && it != end)
  {
    return (*it).first;
  }
  
  return nglString::Null;
}

const nglString& nuiXMLNode::GetAttributeValue (uint index) const
{
  uint i;
  nuiXMLAttributeList::const_iterator it;
  nuiXMLAttributeList::const_iterator end = mAttributes.end();
  
  for (it = mAttributes.begin(), i=0 ; it!=end && i<index; ++it)
    i++;
  
  if (i == index && it != end)
  {
    return (*it).second;
  }
  
  return nglString::Null;
}


// Node tree management:
const list<nuiXMLNode*>& nuiXMLNode::GetChildren() const
{
  return mpChildren;
}

bool nuiXMLNode::AddChild (nuiXMLNode* pNode)
{
  mpChildren.push_back(pNode);
  pNode->SetParent(this);
  return true;
}

bool nuiXMLNode::DelChild (nuiXMLNode* pNode)
{
  mpChildren.remove(pNode);
  pNode->SetParent(NULL);
  return true;
}

uint nuiXMLNode::GetChildrenCount () const
{
  return (uint)mpChildren.size();
}

nuiXMLNode* nuiXMLNode::GetChild (uint index) const
{
  uint i;
  list<nuiXMLNode*>::const_iterator it;
  list<nuiXMLNode*>::const_iterator end = mpChildren.end();
  
  for (it = mpChildren.begin(), i=0 ; it!=end && i<index; ++it)
    i++;
  
  if (i == index && it != end)
    return (*it);
  
  return NULL;
}

nuiXMLNode* nuiXMLNode::GetChild (const nglString& rName) const
{
  list<nuiXMLNode*>::const_iterator it;
  list<nuiXMLNode*>::const_iterator end = mpChildren.end();
  
  for (it = mpChildren.begin(); it!=end; ++it)
  {
    if ((*it)->GetName() == rName)
      return (*it);
  }
  return NULL;
}

nuiXMLNode* nuiXMLNode::Find (const nglString& rName) const
{
  int slash = rName.Find('/');
  
  if (slash >= 0)
  {
    nglString path = rName.GetLeft(slash);
    nglString rest = rName.Extract(slash + 1);
    nuiXMLNode* node = GetChild(path);
    
    return node ? node->Find(rest) : NULL;
  }
  else
    return GetChild(rName);
}

#define DUMP(x) { nglString tempdump; tempdump.CFormat x; res+=tempdump;}
#define DUMPT(x) { res+=tab; DUMP(x)}

//#define DUMPT(x) { OUT(tab.GetChars()); OUT x;}
//#define DUMP(x) { OUT x;}

int64 nuiXMLNode::Write(nglOStream& rStream, uint level) const
{
  nglString tab;
  int64 res = 0;
  nglString scratch1;
  nglString scratch2;
  nglString name;
  tab.Fill(_T("  "),level);
  
  if (mName.GetLeft(2) != _T("##"))
  {
    XMLizeString(name, mName);
    //#FIXME
    //DUMPT(("<%s",name.GetChars()));
    res += rStream.WriteText(tab);
    res += rStream.WriteText(nglString(_T(" <")));
    res += rStream.WriteText(name);
    //res.Add(tab).Add(_T(" <")).Add(name);
    
    //    uint i;
    
    nuiXMLAttributeList::const_iterator attrib = mAttributes.begin();
    nuiXMLAttributeList::const_iterator attrib_end = mAttributes.end();
    
    while (attrib != attrib_end)
    {
      
      XMLizeString(scratch1, attrib->first.GetChars());
      XMLizeString(scratch2, attrib->second.GetChars());
      
      
      
      //      DUMP((" %s=\"%s\"",
      //        scratch1.GetChars(),
      //        scratch2.GetChars()
      //        ));
      
      
      //#FIXME
      //res.Add(_T(' ')).Add(scratch1).Add(_T("=\"")).Add(scratch2).Add(_T("\""));
      res += rStream.WriteText(nglString(_T(" ")));
      res += rStream.WriteText(scratch1);
      res += rStream.WriteText(nglString(_T("=\"")));
      res += rStream.WriteText(scratch2);
      res += rStream.WriteText(nglString(_T("\"")));
      
      ++attrib;
    }
    
    if (!mpChildren.empty())
    {
      //#FIXME
      //DUMP((">\n"));
      //res.Add(_T(">\n"));
      res += rStream.WriteText(nglString(_T(">\n")));
      
      list<nuiXMLNode*>::const_iterator it;
      list<nuiXMLNode*>::const_iterator end = mpChildren.end();
      
      for (it = mpChildren.begin(); it!=end; ++it)
      {
        res += (*it)->Write(rStream, level + 1);
      }
      
      //#FIXME
      //DUMPT(("</%s>\n",name.GetChars()));
      //res.Add(tab).Add(_T("</")).Add(name).Add(_T(">\n"));
      res += rStream.WriteText(tab);
      res += rStream.WriteText(nglString(_T("</")));
      res += rStream.WriteText(name);
      res += rStream.WriteText(nglString(_T(">\n")));
      
      //      printf("res [%d]:\n%s\n**************\n\n", res.GetLength(), res.GetChars());
    }
    else
    {
      //#FIXME
      //DUMP(("/>\n"));
      //res.Add(_T("/>\n"));
      res += rStream.WriteText(nglString(_T("/>\n")));
    }
  }
  else
  {
    nglString value;
    XMLizeString(value, mValue);
    if (mName == _T("##text"))
    {
      //#FIXME
      //DUMPT(("%s\n",value.GetChars()));
      //res.Add(tab).Add(value).Add(_T("\n"));
      res += rStream.WriteText(tab);
      res += rStream.WriteText(value);
      res += rStream.WriteText(nglString(_T("\n")));
    }
    else if (mName == _T("##comment"))
    {
      //#FIXME
      //DUMPT(("<!-- %s -->\n",value.GetChars()));
      //res.Add(tab).Add(_T("<!-- ")).Add(value).Add(_T(" -->\n"));
      res += rStream.WriteText(tab);
      res += rStream.WriteText(nglString(_T("<!-- ")));
      res += rStream.WriteText(value);
      res += rStream.WriteText(nglString(_T(" -->\n")));
    }
    else if (mName == _T("##command"))
    {
      //#FIXME
      //DUMPT(("<!%s>\n",value.GetChars()));
      //res.Add(tab).Add(_T("<!")).Add(value).Add(_T(">\n"));
      res += rStream.WriteText(tab);
      res += rStream.WriteText(nglString(_T("<!")));
      res += rStream.WriteText(value);
      res += rStream.WriteText(nglString(_T(">\n")));
    }
  }
  
  return res;
}

nglString nuiXMLNode::Dump(uint level) const
{
  nglString tab;
  nglString res;
  nglString scratch1;
  nglString scratch2;
  nglString name;
  tab.Fill(_T("  "),level);
  
  if (mName.GetLeft(2) != _T("##"))
  {
    XMLizeString(name, mName);
    //#FIXME
    //DUMPT(("<%s",name.GetChars()));
    res.Add(tab).Add(_T(" <")).Add(name);
    
    //    uint i;
    
    nuiXMLAttributeList::const_iterator attrib = mAttributes.begin();
    nuiXMLAttributeList::const_iterator attrib_end = mAttributes.end();
    
    while (attrib != attrib_end)
    {
      
      XMLizeString(scratch1, attrib->first.GetChars());
      XMLizeString(scratch2, attrib->second.GetChars());
      
      
      
      //      DUMP((" %s=\"%s\"",
      //        scratch1.GetChars(),
      //        scratch2.GetChars()
      //        ));
      
      
      //#FIXME
      res.Add(_T(' ')).Add(scratch1).Add(_T("=\"")).Add(scratch2).Add(_T("\""));
      
      
      ++attrib;
    }
    
    if (!mpChildren.empty())
    {
      //#FIXME
      //DUMP((">\n"));
      res.Add(_T(">\n"));
      
      list<nuiXMLNode*>::const_iterator it;
      list<nuiXMLNode*>::const_iterator end = mpChildren.end();
      
      for (it = mpChildren.begin(); it!=end; ++it)
      {
        res += (*it)->Dump(level+1);
      }
      
      //#FIXME
      //DUMPT(("</%s>\n",name.GetChars()));
      res.Add(tab).Add(_T("</")).Add(name).Add(_T(">\n"));
      
      //      printf("res [%d]:\n%s\n**************\n\n", res.GetLength(), res.GetChars());
    }
    else
    {
      //#FIXME
      //DUMP(("/>\n"));
      res.Add(_T("/>\n"));
    }
  }
  else
  {
    nglString value;
    XMLizeString(value, mValue);
    if (mName == _T("##text"))
    {
      //#FIXME
      //DUMPT(("%s\n",value.GetChars()));
      res.Add(tab).Add(value).Add(_T("\n"));
    }
    else if (mName == _T("##comment"))
    {
      //#FIXME
      //DUMPT(("<!-- %s -->\n",value.GetChars()));
      res.Add(tab).Add(_T("<!-- ")).Add(value).Add(_T(" -->\n"));
    }
    else if (mName == _T("##command"))
    {
      //#FIXME
      //DUMPT(("<!%s>\n",value.GetChars()));
      res.Add(tab).Add(_T("<!")).Add(value).Add(_T(">\n"));
    }
  }
  
  return res;
}

nuiXMLNode* nuiXMLNode::GetParent()
{
  return mpParent;
}

const nuiXMLNode* nuiXMLNode::GetParent() const
{
  return mpParent;
}

const nuiXMLNode* nuiXMLNode::GetRoot() const
{
  if (mpParent)
    return mpParent->GetRoot();
  return this;
}

nuiXMLNode* nuiXMLNode::GetRoot()
{
  if (mpParent)
    return mpParent->GetRoot();
  return this;
}

void nuiXMLNode::SetParent(nuiXMLNode* pParent)
{
  mpParent = pParent;
}

void nuiXMLNode::SetTag(void* pTag)
{
  mpTag = pTag;
}

void* nuiXMLNode::GetTag() const
{
  return mpTag;
}


bool GetToken(nglString& src, nglString& dest)
{
  int idx;
  //  OUT("%s\n",src.GetChars());
  dest.Wipe();
  
  src.TrimLeft(); // Eat the starting blank
  if (src[0]=='"')
  {
    // Read a string
    for (idx=1; idx<src.GetLength(); idx++)
    {
      if (src[idx]=='\\')
      {
        // Skip next char
        idx++;
      }
      else if (src[idx]=='"')
      {
        // This is the end of the string
        dest=src.GetLeft(idx+1);
        src.DeleteLeft(idx+1);
        return true;
      }
    }
    return false;
  }
  else if (src[0]=='=')
  {
    // This is the equal sign
    dest=_T("=");
    src.DeleteLeft(1);
    return true;
  }
  else if (src[0]=='>')
  {
    // This is the end of the tag
    dest=_T(">");
    src.Delete();
    return true;
  }
  else if (src[0]=='/')
  {
    // This is the end of a self contained tag
    if (src[1]!='>')
      return false; // Malformed tag end
    
    dest=_T("/>");
    src.Delete();
    return true;
  }
  
  for (idx=0; idx<src.GetLength(); idx++)
  {
    // Read an id:
    if (src[idx]==' ' || src[idx]=='\t' 
        || src[idx]==0xa || src[idx]==0xd 
        || src[idx]=='>' || src[idx]=='/' 
        || src[idx]=='=')
    {
      // This is the end of the string
      dest=src.GetLeft(idx);
      src.DeleteLeft(idx);
      return true;
    }
  }
  return false;
}

bool nuiXMLNode::ParseXMLCommand(xmlLexer* pLexer, nglString& str)
{
  str.TrimLeft();
  if (!str.Compare(_T("DOCTYPE"),0,7,true))
  {
    // This is the DTD
    int start = str.Find(_T("["));
    int stop = str.FindLast(_T("]"));
    
    if (start != -1 && stop != -1 && start < stop)
    {
      nglString dtd = str.Extract(start,stop-start);
      
      int pos = dtd.Find(_T("<!ENTITY"),0);
      while (pos!=-1)
      {
        int end = dtd.Find(_T(">"),pos);
        if (end == -1)
          return false;
        nglString entity = dtd.Extract(pos+ 8,end - pos - 8); /// strlen("<!ENTITY") == 8
        entity.Trim();
        int quote1 = entity.Find(_T("\""),0);
        if (quote1 == -1)
          return false;
        int quote2 = entity.Find(_T("\""),quote1+1);
        if (quote2 == -1)
          return false;
        
        nglString value = entity.Extract(quote1+1,quote2-quote1-1);
        nglString name = entity.Extract(0,quote1);
        name.Trim();
        
        pos += quote2;
        pos = dtd.Find(_T(">"),pos);
        if (pos == -1)
          return false;
        
        // Ok, now we're quite sure to have found a valid entity, lets add it to the lexer:
        name.Insert((nglChar)'&',0);
        name += _T(";");
        pLexer->AddEntity(name,value);
        
        pos = dtd.Find(_T("<!ENTITY"),pos);
      }
    }
  }
  
  return true;
}

bool nuiXMLNode::ParseXMLNode(xmlLexer* pLexer, nglString& str)
{
  if (str[0]=='<')
  {
    // This is a tag...
    // Get the type of the tag:
    if (str[1]=='!')
    {
      if (str[2]=='-' && str[3]=='-')
      {
        // This is a comment tag
        SetName(nglString(_T("##comment")));
        str.DeleteLeft(4);
        str.DeleteRight(3);
        str.TrimLeft();
        str.TrimRight();
        
        DeXMLizeString(str);
        SetValue(str);
        return true;
      }
      else
      {
        str.DeleteLeft(2);
        return ParseXMLCommand(pLexer,str);
      }
      /*
       SetName(nglString(_T("##command")));
       str.DeleteLeft(2);
       str.DeleteRight(1);
       SetValue(str);
       return true;
       */
    }
    else if (str[1]=='?')
    {
      // This is a command tag
      if (str[2]=='-' && str[3]=='-')
      {
        // This is a comment tag
        SetName(nglString(_T("##comment")));
        str.DeleteLeft(4);
        str.DeleteRight(3);
        str.TrimLeft();
        str.TrimRight();
        
        DeXMLizeString(str);
        SetValue(str);
      }
      else
      {
        SetName(nglString(_T("##command")));
        str.DeleteLeft(2);
        str.DeleteRight(1);
        
        DeXMLizeString(str);
        SetValue(str);
      }
      return true;
    }
    
    // This is a normal node.
    
    // Get the node name:
    str.DeleteLeft(1); // Eat the '<' starting char.
    
    nglString tok;
    if (!GetToken(str,tok))
      return false;
    
    if (tok[0]=='"' || tok[0]=='>' || tok[0]=='/' || tok[0]=='=')
      return false; // Malformed tag name
    
    DeXMLizeString(tok);
    SetName(tok);
    
    // Retreive attributes:
    while (GetToken(str,tok) && (tok[0]!='>' && tok[0]!='/'))
    {
      if (tok[0]=='"' || tok[0]=='=')
        return false; // Malformed tag name
      
      nglString attrname;
      nglString attrval;
      
      attrname = tok;
      
      if (!GetToken(str,tok))
        return false;
      
      if (tok[0]!='=')
        return false;
      
      if (!GetToken(str,tok))
        return false;
      
      if (tok[0]!='"')
        return false;
      
      attrval=tok;
      attrval.DeleteLeft(1);
      attrval.DeleteRight(1);
      
      DeXMLizeString(attrname);
      DeXMLizeString(attrval);
      SetAttribute(attrname,attrval);
    }
    
    if (tok[0]=='/')
    {
      // This was a self contained tag, do not recurse thru the contained tags.
      return true;
    }
    
    while (pLexer->GetToken(str))
    {
      
      if (str[0]=='<' && str[1]=='/')
      {
        // Check if this is the end of this node:
        str.DeleteLeft(2);
        if (!GetToken(str,tok))
          return false;
        if (tok[0]=='"' || tok[0]=='>' || tok[0]=='/' || tok[0]=='=')
          return false; // Malformed tag name
        
        if (tok != GetName())
          return false; // Error, wrong end tag name
        //Ok, we're returning now...
        return true;
      }
      
      // Then this is a sub node:
      nuiXMLNode* pNode;
      pNode = new nuiXMLNode(nglString(_T("##building")),this);
      if (!pNode)
        return false;
      
      if (!pNode->ParseXMLNode(pLexer,str))
        return false;
      
    }
    // We didn't met the tag end...
    return false;
  }
  else
  {
    SetName(nglString(_T("##text")));
    DeXMLizeString(str);
    SetValue(str);
    
    return true;
  }
}

//////////////////////////
// nuiXML



nuiXML::nuiXML(const nglString& rName)
: nuiXMLNode (rName)
{
  mDTDName = _T("");
  mDTDFile = _T("");
  mStyleSheetType = _T("");
  mStyleSheetFile = _T("");
}

nuiXML::nuiXML(const char* pName)
: nuiXMLNode (nglString(pName))
{
  mDTDName = _T("");
  mDTDFile = _T("");
  mStyleSheetType = _T("");
  mStyleSheetFile = _T("");
}

nuiXML::~nuiXML()
{
}

bool nuiXML::Load(nglIStream& rStream)
{
  nuiXMLBuilder parser;
  bool res = parser.Parse(&rStream, this);
  return res;
  
  nglTime Start;
  uint8 start[16];
  if (rStream.Peek(start, 10, 1))
  {
    // Skip the unicode BOM:
    //    Bytes    Encoding Form
    //      00 00 FE FF   UTF-32, big-endian
    //      FF FE 00 00   UTF-32, little-endian
    //      FF FE   UTF-16, little-endian
    //      FE FF   UTF-16, big-endian
    //      EF BB BF   UTF-8
    if (start[0] == 0x00)
    {
      if (rStream.ReadUInt8() != 0x00) return false;
      if (rStream.ReadUInt8() != 0x00) return false;
      if (rStream.ReadUInt8() != 0xFE) return false;
      if (rStream.ReadUInt8() != 0xFF) return false;
      // big-endian UTF-32
    }
    else if (start[0] == 0xFF)
    {
      if (rStream.ReadUInt8() != 0xFF) return false;
      if (rStream.ReadUInt8() != 0xFE) return false;
      if (start[2] == 0x00)
      {
        if (rStream.ReadUInt8() != 0x00) return false;
        if (rStream.ReadUInt8() != 0x00) return false;
        // little endian UTF-32
      }
      else
      {
        // little endian UTF-16
      }
    }
    else if (start[0] == 0xFE)
    {
      if (rStream.ReadUInt8() != 0xFE) return false;
      if (rStream.ReadUInt8() != 0xFF) return false;
      // big endian UTF-16
    }
    else if (start[0] == 0xEF)
    {
      if (rStream.ReadUInt8() != 0xEF) return false;
      if (rStream.ReadUInt8() != 0xBB) return false;
      if (rStream.ReadUInt8() != 0xBF) return false;
      // UTF-8
    }
  }
  
  rStream.SetTextEncoding(eUTF8);
  xmlLexer lexer((nglIStream*)&rStream);
  nglString str;
  
  
  if (!lexer.GetToken(str))
    return false;
  
  if (str[0]!='<')
  {
    return false;
  }
  
  if (str[1]=='?')
  {
    if (!ParseXMLHeader(&lexer,str))
      return false;
    if (!lexer.GetToken(str))
      return false;
  }
  
  do 
  {
    if (!ParseXMLNode(&lexer,str))
      return false;
  }
  while (lexer.GetToken(str));
  
  nglTime End;
  
  //  NGL_OUT(_T("XML %s Parsing took %g seconds"), mName.GetChars(), End - Start);
  return true;
}

bool nuiXML::ParseXMLHeader(xmlLexer* pLexer, nglString& str)
{
  return true;
}

bool nuiXML::Save(nglOStream& rStream) const
{
  Write(rStream);
//  nglString res = Dump();
//  rStream.WriteText(res);
  return true;
}

nglString nuiXML::Dump(uint level) const
{
  nglString res;
  res += _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  
  if( !mDTDName.IsEmpty()  && !mDTDFile.IsEmpty())
  {
    nglString dtd;
    dtd.CFormat(_T("<!DOCTYPE %s  SYSTEM '%s'>\n"), mDTDName.GetChars(), mDTDFile.GetChars());
    res += dtd;
  }
  
  if( !mStyleSheetType.IsEmpty() && !mStyleSheetFile.IsEmpty())
  {
    nglString styleSheet;
    styleSheet.CFormat(_T("<?xml-stylesheet type=\"%s\" href=\"%s\" ?>\n"), mStyleSheetType.GetChars(), mStyleSheetFile.GetChars());
    res+= styleSheet;
  }
  
  res += nuiXMLNode::Dump(level);
  
  return res;
}

int64 nuiXML::Write(nglOStream& rStream, uint level) const
{
  nglString res;
  res += _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  
  if (!mDTDName.IsEmpty()  && !mDTDFile.IsEmpty())
  {
    nglString dtd;
    dtd.CFormat(_T("<!DOCTYPE %s  SYSTEM '%s'>\n"), mDTDName.GetChars(), mDTDFile.GetChars());
    res += dtd;
  }
  
  if( !mStyleSheetType.IsEmpty() && !mStyleSheetFile.IsEmpty())
  {
    nglString styleSheet;
    styleSheet.CFormat(_T("<?xml-stylesheet type=\"%s\" href=\"%s\" ?>\n"), mStyleSheetType.GetChars(), mStyleSheetFile.GetChars());
    res+= styleSheet;
  }
  
  return rStream.WriteText(res) + nuiXMLNode::Write(rStream);
}


void nuiXML::SetDTD(nglString name, nglString file)
{
  mDTDName = name;
  mDTDFile = file;
}

void nuiXML::SetStyleSheet(nglString type, nglString file)
{
  mStyleSheetType = type;
  mStyleSheetFile = file;
}

const nglString& nuiXML::GetDTDName()
{
  return mDTDName;
}

const nglString& nuiXML::GetDTDFile()
{
  return mDTDFile;
}

const nglString& nuiXML::GetStyleSheetType()
{
  return mStyleSheetType;
}

const nglString& nuiXML::GetStyleSheetFile()
{
  return mStyleSheetFile;
}

