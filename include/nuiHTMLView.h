/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTML.h"
#include "nuiFontManager.h"

class nuiHTMLContext;
class nuiHTMLItem;
class nuiHTMLBox;

class nuiHTMLView : public nuiSimpleContainer
{
public:
  nuiHTMLView(float IdealWidth);
  ~nuiHTMLView();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);
  
  void SetIdealWidth(float IdealWidth);
  float GetIdealWidth() const;
  float GetVSpace() const;
  float GetHSpace() const;
  void SetVSpace(float interline);
  void SetHSpace(float InterWidget);
  
  bool SetText(const nglString& rHTMLText);
  bool SetURL(const nglString& rURL);

protected:
  void ParseTree(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseHTML(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseHead(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseTitle(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseText(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseBody(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseDiv(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseFormatTag(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  nuiHTMLBox* mpRootBox;

  float mIdealWidth;
  float mVSpace;
  float mHSpace;
  
  nuiHTML* mpHTML;
};

class nuiHTMLContext
{
public:
  nuiHTMLContext();
  nuiHTMLContext(const nuiHTMLContext& rContext);
  nuiHTMLContext& operator=(const nuiHTMLContext& rContext);
  
  float mLeftMargin;
  
  float mMaxWidth;
  
  float mVSpace;
  float mHSpace;
  
  nuiFontRequest mFont;
};

class nuiHTMLItem
{
public:
  nuiHTMLItem(nuiHTMLNode* pNode, bool Inline = true);
  ~nuiHTMLItem();

  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);

  const nuiRect& GetIdealRect() const;
  const nuiRect& GetRect() const;
  void SetRect(const nuiRect& rRect);
  void MoveTo(float x, float y);
  void SetWidth(float w);
  void SetHeight(float h);
  
  void SetParent(nuiHTMLBox* pBox);
  nuiHTMLBox* GetParent() const;

  virtual float GetAscender() const;
  virtual float GetDescender() const;

  bool IsInline() const;
  void SetInline(bool set);
  bool IsEndTag() const;
  void SetEndTag(bool set);
  
protected:
  nuiHTMLNode* mpNode;
  nuiRect mIdealRect;
  nuiRect mRect;
  nuiHTMLBox* mpParent;
  bool mInline;
  bool mEndTag;
};

class nuiHTMLBox : public nuiHTMLItem
{
public:
  nuiHTMLBox(nuiHTMLNode* pNode, bool Inline = true);
  virtual ~nuiHTMLBox();

  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
  void AddItem(nuiHTMLItem* pItem);
  void AddItemEnd(nuiHTMLItem* pItem);

  void PushContext(const nuiHTMLContext& rContext);
  void PopContext(nuiHTMLContext& rContext);
    
protected:
  std::vector<nuiHTMLItem*> mItems;
  std::stack<nuiHTMLContext> mContextStack;
};

class nuiHTMLText : public nuiHTMLItem
{
public:
  nuiHTMLText(nuiHTMLNode* pNode, const nglString& rText);
  virtual ~nuiHTMLText();

  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);

  virtual float GetAscender() const;
  virtual float GetDescender() const;
  
private:
  nglString mText;
  nuiFontLayout* mpLayout;
  nuiFont* mpFont;
};

