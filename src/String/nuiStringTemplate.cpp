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

bool nuiStringTemplate::Parse(const nglString& rSource)
{
  int32 i = 0;
  int32 len = rSource.GetLength();
  int32 start = 0;
  nglUChar cc = 0;

  while (i < len)
  {
    int32 current = i;
    nglUChar c = rSource.GetNextUChar(i);
    
    if (c == '{' && cc == '{')
    {
      nglString txt(rSource.Extract(start, current - start));
      AddNode(new nuiSTN_Text(txt));
      
      // Skip blanks
      c = ' ';
      while (i < len && c == ' ')
        c = rSource.GetNextUChar(i);
      
      if (i == len)
        return false;
      
      start = i;
      while (i < len && c != '}' )
      {
        current = i;
        c = rSource.GetNextUChar(i);
      }
      
      if (c == '}')
      {
        c = rSource.GetNextUChar(i);
        if (c == '}')
        {
          nglString t(rSource.Extract(start, current - start));
          t.Trim();
          
          AddNode(new nuiSTN_Attribute(txt));
        }
        else
          return false;
      }
      else
        return false;
            
    }
        
    cc = c;
  }
  
  return true;
}
