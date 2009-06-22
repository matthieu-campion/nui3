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
  void ParseI(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseB(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseU(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  nuiHTMLBox* mpRootBox;

  void WalkTree(nuiHTMLNode* pNode, const nuiHTMLContext& rContext);
  bool InterpretTree(nuiHTMLContext& rContext);
  void LayoutLine(nuiHTMLContext& rContext);
  float mIdealWidth;
  float mVSpace;
  float mHSpace;
  
  void AddElement(nuiHTMLContext& rContext, nuiWidgetPtr pWidget);
  
  nuiHTML* mpHTML;
};

class nuiHTMLContext
{
public:
  nuiHTMLContext();
  nuiHTMLContext(const nuiHTMLContext& rContext);
  nuiHTMLContext& operator=(const nuiHTMLContext& rContext);
  
  float mX;
  float mY;
  float mW;
  float mH;
  
  float mLeftMargin;
  
  float mIdealWidth;
  float mMaxWidth;
  float mMaxHeight;
  
  float mVSpace;
  float mHSpace;
  
  nuiWidgetList mLine;  
  nuiFontRequest mFont;
  bool mSetLayout;
  nuiHTMLNode* mpNode;
};

class nuiHTMLItem
{
public:
  nuiHTMLItem(nuiHTMLNode* pNode, bool Inline);
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
protected:
  nuiHTMLNode* mpNode;
  nuiRect mIdealRect;
  nuiRect mRect;
  nuiHTMLBox* mpParent;
  bool mInline;
};

class nuiHTMLBox : public nuiHTMLItem
{
public:
  nuiHTMLBox(nuiHTMLNode* pNode, bool Inline = true);
  virtual ~nuiHTMLBox();

  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
  void AddItem(nuiHTMLItem* pItem);

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

