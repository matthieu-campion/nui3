/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiStringTemplateNode
{
public:
  typedef nuiFastDelegate2<const nglString&, nuiObject*, bool> OutputDelegate;
  
  nuiStringTemplateNode() {}
  virtual ~nuiStringTemplateNode() {}
  virtual bool Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate) = 0;
};

class nuiStringTemplateContainer : public nuiStringTemplateNode
{
public:
  nuiStringTemplateContainer();
  virtual ~nuiStringTemplateContainer();

  virtual bool Generate(nuiObject* pDataSource, const OutputDelegate& rOutputDelegate);

  void AddNode(nuiStringTemplateNode* pNode);
  void Clear();
  
private:
  std::vector<nuiStringTemplateNode*> mpNodes;
};

class nuiStringTemplate : public nuiStringTemplateContainer
{
public:
  nuiStringTemplate(const nglString& rTemplateSource = nglString::Null);
  virtual ~nuiStringTemplate();
  
  bool Parse(const nglString& rSource);
  
private:
  class ParseContext;
  bool Parse(ParseContext& rContext);
  bool ParseFor(ParseContext& rContext);
  bool ParseClose(ParseContext& rContext);
  bool ParseEnd(ParseContext& rContext);
  bool ParseTextUntilCommand(ParseContext& rContext);
};

