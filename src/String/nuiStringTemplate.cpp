/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

class nuiSTN_Text : public nuiStringTemplateNode
{
public:
  nuiSTN_Text(const nglString& rText)
  : mText(rText)
  {
    //NGL_OUT("nuiSTN_Text: '%s'\n", mText.GetChars());
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
    //NGL_OUT("nuiSTN_Attribute: '%s'\n", mAttribute.GetChars());
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

class nuiSTN_For : public nuiStringTemplateContainer
{
public:
  nuiSTN_For(const nglString& rVariable, const nglString& rContainer)
  : mVariable(rVariable), mContainer(rContainer)
  {
    //NGL_OUT("nuiSTN_For: '%s' in '%s'\n", mVariable.GetChars(), mContainer.GetChars());
  }
  
  virtual bool Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate)
  {
    nuiAttribBase container(pDataSource->GetAttribute(mContainer));
    nuiAttribBase variable(pDataSource->GetAttribute(mVariable));
    nuiAttribute<nglString>* pAttribute = NULL;
    nglString val;
    if (!variable.IsValid())
    {
      pAttribute = new nuiAttribute<nglString>(mVariable, nuiUnitNone, val);
      pDataSource->AddInstanceAttribute(mVariable, pAttribute);
      variable = pDataSource->GetAttribute(mVariable);
    }

    if (container.IsValid())
    {
      int32 count = container.GetIndexRange(0);
      for (int32 i = 0; i < count; i++)
      {
        nglString val;
        container.ToString(i, val);
        variable.FromString(val);
        
        bool res = nuiStringTemplateContainer::Generate(pDataSource, rOutputDelegate);
      }
    }
    return true;
  }
  
private:
  nglString mVariable;
  nglString mContainer;
  std::vector<nuiStringTemplateNode*> mpNodes;
  
};

///////////////////////////
nuiStringTemplateContainer::nuiStringTemplateContainer()
{
}

nuiStringTemplateContainer::~nuiStringTemplateContainer()
{
  Clear();
}

void nuiStringTemplateContainer::AddNode(nuiStringTemplateNode* pNode)
{
  mpNodes.push_back(pNode);
}

void nuiStringTemplateContainer::Clear()
{
  for (int32 i = 0; i < mpNodes.size(); i++)
    delete mpNodes[i];
  mpNodes.clear();
}

bool nuiStringTemplateContainer::Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate)
{
  for (int32 i = 0; i < mpNodes.size(); i++)
    if (!mpNodes[i]->Generate(pDataSource, rOutputDelegate))
      return false;
  
  return true;
}




///////////////////////////

nuiStringTemplate::nuiStringTemplate(const nglString& rTemplateSource)
{
  if (!rTemplateSource.IsNull())
    Parse(rTemplateSource);
}

nuiStringTemplate::~nuiStringTemplate()
{
}

class nuiStringTemplate::ParseContext
{
public:
  ParseContext(const nglString& rString, nuiStringTemplateContainer* pContainer)
  : mString(rString), mPos(0), mNextPos(0), mLen(rString.GetLength()), mTag(0)
  {
    PushContainer(pContainer);
    
    NextChar();
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
  
  bool GetAlpha(nglString& rString)
  {
    rString.Wipe();
    while (nglIsAlpha(mChar) && IsValid())
    {
      rString.Add(mChar);
      if (!NextChar())
        return false;
    }
    
    return true;
  }

  bool IsValid() const
  {
    return mNextPos < mLen;
  }

  void PushContainer(nuiStringTemplateContainer* pContainer)
  {
    mContainers.push_back(pContainer);
  }
  
  nuiStringTemplateContainer* GetContainer() const
  {
    return mContainers.back();
  }

  void PopContainer()
  {
    mContainers.pop_back();
  }
  
  void AddNode(nuiStringTemplateNode* pNode)
  {
    GetContainer()->AddNode(pNode);
  }
  

  const nglString& mString;
  int32 mPos;
  int32 mNextPos;
  const int32 mLen;
  nglUChar mChar;
  nglUChar mTag;
  
  std::list<nuiStringTemplateContainer*> mContainers;
};

bool nuiStringTemplate::Parse(const nglString& rSource)
{
  ParseContext context(rSource, this);
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
    
    if (cc == '{' && (c == '{' || c == '%'))
    {
      nglString txt(rContext.mString.Extract(start, current - start - 1));
      rContext.AddNode(new nuiSTN_Text(txt));
      
      rContext.mTag = c;

      if (!rContext.NextChar())
        return false;
      
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
  rContext.AddNode(new nuiSTN_Text(txt));
  
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
    
    ParseTextUntilCommand(rContext);
    
    if (!rContext.IsValid())
      return false;
    
    if (rContext.mTag == '{')
    {
      // Variable access:
      start = rContext.mPos;

      // Skip blanks
      if (!rContext.SkipBlank())
        return false;
      
      if (!rContext.ToNext('}'))
        return false;
      current = rContext.mPos;
      
      if (!rContext.NextChar())
        return false;
      
      if (rContext.mChar == '}')
      {
        nglString t(rContext.mString.Extract(start, current - start));
        t.Trim();
        
        rContext.AddNode(new nuiSTN_Attribute(t));
        
        start = rContext.mPos;
      }
      else
        return false;

      if (!rContext.NextChar())
        return false;
    }
    else if (rContext.mTag == '%')
    {
      // Parse Keyword

      // Skip blanks
      if (!rContext.SkipBlank())
        return false;
      
      nglString cmd;
      if (!rContext.GetAlpha(cmd))
        return false;

      if (cmd == "for")
      {
        if (!ParseFor(rContext))
          return false;
      }
      else if (cmd == "end")
      {
        if (!ParseEnd(rContext))
          return false;
      }
      else
        return false;
    }
    else
    {
      if (!rContext.NextChar())
        return false;
    }
        
    cc = c;

  }
  

  return true;
}

bool nuiStringTemplate::ParseFor(ParseContext& rContext)
{
  if (!rContext.SkipBlank())
    return false;
  
  nglString var;
  if (!rContext.GetAlpha(var))
    return false;
  
  if (!rContext.SkipBlank())
    return false;
  
  nglString in;
  if (!rContext.GetAlpha(in))
    return false;
  if (in != "in")
    return false;
  
  if (!rContext.SkipBlank())
    return false;
  
  nglString container;
  if (!rContext.GetAlpha(container))
    return false;

  nuiSTN_For* pFor = new nuiSTN_For(var, container);
  rContext.AddNode(pFor);
  rContext.PushContainer(pFor);
  
  return ParseClose(rContext);
}

bool nuiStringTemplate::ParseClose(ParseContext& rContext)
{  
  if (!rContext.SkipBlank())
    return false;
  
  if (!rContext.ToNext('%'))
    return false;
  
  if (!rContext.NextChar())
    return false;
  
  if (rContext.mChar != '}')
    return false;
  
  if (!rContext.NextChar())
    return false;
  
  return true;
}


bool nuiStringTemplate::ParseEnd(ParseContext& rContext)
{
  rContext.PopContainer();

  return ParseClose(rContext);
}
