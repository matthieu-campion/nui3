/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nuiStringTemplate.h"

class nuiSTN_Text : public nuiStringTemplateNode
{
public:
  nuiSTN_Text(const nglString& rText)
  : mText(rText)
  {
    NGL_OUT("nuiSTN_Text: '%s'\n", mText.GetChars());
  }
  
  virtual bool Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate)
  {
    return rOutputDelegate(mText, pDataSource);
  }
 
private:
  nglString mText;
};

class nuiSTN_Attribute : public nuiStringTemplateNode
{
public:
  nuiSTN_Attribute(const nglString& rAttribute)
  : mAttribute(rAttribute)
  {
    NGL_OUT("nuiSTN_Attribute: '%s'\n", mAttribute.GetChars());
  }
  
  virtual bool Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate)
  {
    nuiAttribBase attrib(pDataSource->GetAttribute(mAttribute));
    nglString val;
    if (attrib.IsValid())
      attrib.ToString(val);
    return rOutputDelegate(val, pDataSource);
  }
  
private:
  nglString mAttribute;
};

nuiStringTemplate::nuiStringTemplate(const nglString& rTemplateSource)
{
  if (!rTemplateSource.IsNull())
    Parse(rTemplateSource);
}

nuiStringTemplate::~nuiStringTemplate()
{
  Clear();
}

void nuiStringTemplate::AddNode(nuiStringTemplateNode* pNode)
{
  mpNodes.push_back(pNode);
}

void nuiStringTemplate::Clear()
{
  for (int32 i = 0; i < mpNodes.size(); i++)
    delete mpNodes[i];
  mpNodes.clear();
}

bool nuiStringTemplate::Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate)
{
  for (int32 i = 0; i < mpNodes.size(); i++)
    if (!mpNodes[i]->Generate(pDataSource, rOutputDelegate))
      return false;
  
  return true;
}

class nuiStringTemplate::ParseContext
{
public:
  ParseContext(const nglString& rString)
  : mString(rString), mPos(0), mNextPos(0), mLen(rString.GetLength())
  {
    
  }
  virtual ~ParseContext() {}

  bool NextChar()
  {
    if (mNextPos >= mLen)
      return false;
    mPos = mNextPos;
    mChar = mString.GetNextUChar(mNextPos);
    
    return true;
  }
  
  bool SkipBlank()
  {
    while (IsValid() && mChar <= ' ')
      NextChar();
    
    return IsValid();
  }

  bool ToNext(nglUChar c)
  {
    while (IsValid() && mChar != c)
      NextChar();
    
    return IsValid();
  }
  

  bool IsValid() const
  {
    return mPos < mLen;
  }
  
  const nglString& mString;
  int32 mPos;
  int32 mNextPos;
  const int32 mLen;
  nglUChar mChar;
  nglUChar mCmd;
};

bool nuiStringTemplate::Parse(const nglString& rSource)
{
  ParseContext context(rSource);
  
  if (!context.NextChar())
    return false;
  return Parse(context);
}

bool nuiStringTemplate::ParseTextUntilCommand(ParseContext& rContext)
{
  int32 start = rContext.mPos;
  nglUChar cc = 0;
  
  while (rContext.IsValid())
  {
    int32 current = rContext.mPos;
    nglUChar c = rContext.mChar;
    
    if (c == '{' && cc == '{' || c == '%')
    {
      // Variable access:
      nglString txt(rContext.mString.Extract(start, current - start - 1));
      AddNode(new nuiSTN_Text(txt));
      
      rContext.mCmd = c;
      
      return true;
    }
    else
    {
      if (!rContext.NextChar())
        return false;
    }
    
    cc = c;
    
  }
  
  nglString txt(rContext.mString.Extract(start, rContext.mLen - start));
  AddNode(new nuiSTN_Text(txt));
  
  return true;
}

bool nuiStringTemplate::Parse(ParseContext& rContext)
{
  int32 start = rContext.mPos;
  nglUChar cc = 0;

  while (rContext.IsValid())
  {
    int32 current = rContext.mPos;
    nglUChar c = rContext.mChar;
    
    if (!ParseTextUntilCommand(rContext))
      return false;
    
    if (rContext.mCmd == '{')
    {
      // Variable access:
      start = rContext.mPos;

      // Skip blanks
      if (!rContext.SkipBlank())
        return false;
      
      if (!rContext.ToNext('}'))
        return false;
      current = rContext.mPos - 1;
      
      rContext.NextChar();
      if (rContext.mChar == '}')
      {
        nglString t(rContext.mString.Extract(start, current - start));
        t.Trim();
        
        AddNode(new nuiSTN_Attribute(t));
        
        start = rContext.mPos;
      }
        else
          return false;
    }
#if 0
    else if (rContext.mCmd == '%')
    {
      // Code access:
      nglString txt(rContext.mString.Extract(start, current - start - 1));
      AddNode(new nuiSTN_Text(txt));
      
      start = rContext.mPos;
      
      // Skip blanks
      if (!rContext.SkipBlank())
        return false;
      
      nglString cmd;
      if (!rContext.GetAlpha(cmd))
        return false;

      if (cmd == "if")
      {
        Check(ParseCondition(rContext));
        Check(ParseText(rContext));
        if (ParseElse(rContext))
          Check(ParseText(rContext));
        Check(ParseBlockEnd(rContext));
      }
      else
        return false;
    }
#endif
    else
    {
      if (!rContext.NextChar())
        return false;
    }
        
    cc = c;

  }
  
  nglString txt(rContext.mString.Extract(start, rContext.mLen - start));
  AddNode(new nuiSTN_Text(txt));

  return true;
}
