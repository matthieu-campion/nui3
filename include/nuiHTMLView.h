/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTML.h"
#include "nuiFontManager.h"


class nuiHTMLItem;
class nuiHTMLBox;
class nuiHTMLFont;
class nuiHTMLContext;

class nuiHTMLView : public nuiSimpleContainer
{
public:
  nuiHTMLView(float IdealWidth = 800);
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
  const nglString& GetURL() const;

  
  void SetFont(nuiFont* pFont, bool AlreadyAcquired);
  void SetFont(nuiFontRequest& rFontRequest);
  void SetFont(const nglString& rFontSymbol);
  void _SetFont(const nglString& rFontSymbol);
  const nglString& _GetFont() const;
  
  
  const nuiColor& GetTextColor() const;
  void SetTextColor(const nuiColor& Color);
  
protected:
  
  void ParseTree(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseHTML(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseHead(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseTitle(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseText(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseBody(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseDiv(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseTable(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseTableRow(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseList(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseHeader(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseFormatTag(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseFont(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  void ParseImage(nuiHTMLNode* pNode, nuiHTMLBox* pBox);
  nuiHTMLBox* mpRootBox;

  float mIdealWidth;
  float mVSpace;
  float mHSpace;
  
  nuiHTML* mpHTML;
  nuiHTMLContext* mpContext;

  void ReLayout();
private:
  void _SetText(const nglString& rHTMLText);
  
  void InitAttributes();
};

