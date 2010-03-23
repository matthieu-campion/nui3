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
class nuiAsyncIStream;

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
  const nglString& GetText() const;
  bool SetURL(const nglString& rURL);
  const nglString& GetURL() const;
  void Cancel();
  
  void SetFont(nuiFont* pFont, bool AlreadyAcquired);
  void SetFont(nuiFontRequest& rFontRequest);
  void SetFont(const nglString& rFontSymbol);
  void _SetFont(const nglString& rFontSymbol);
  const nglString& _GetFont() const;
  
  const nuiColor& GetTextColor() const;
  void SetTextColor(const nuiColor& Color);
  const nuiColor& GetLinkColor() const;
  void SetLinkColor(const nuiColor& Color);
  
  virtual bool MouseClicked(const nglMouseInfo& rInfo);
  virtual bool MouseUnclicked(const nglMouseInfo& rInfo);
  virtual bool MouseMoved(const nglMouseInfo& rInfo);
  
  nuiSignal1<const nglString&> LinkActivated;
  nuiSignal1<const nglString&> URLChanged;
  
  void SetAutoActivateLink(bool set);
  bool IsAutoActivateLink() const;
  
  void SetDebugBoxes(bool set);
  bool GetDebugBoxes() const;
  
  void SetAutoIdealWidth(bool set);
  bool GetAutoIdealWidth() const;
  
  void SetUseToolTips(bool set);
  bool GetUseToolTips() const;
  
  void SetMargins(float margins);
  float GetMargins() const;
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
  void _SetURL(const nglString& rURL);
  void _AutoSetURL(const nglString& rURL);
  void _SetText(const nglString& rHTMLText);
  nuiAsyncIStream* mpStream;
  void StreamDone(nuiAsyncIStream* pStream);
  
  void InitAttributes();
  bool mClicked;
  nuiSlotsSink mSlotSink;
  
  bool mAutoActivateLink;
  bool mDebugBoxes;
  
  float mMouseX;
  float mMouseY;
  
  nuiHTMLNode* mpCurrentAnchor;
  nuiRect mLastVisibleRect;
  nglString mTempURL;
  
  bool mAutoIdealWidth;
  
  bool mUseToolTips;
  
  nglString mText;
  
  float mMargins;
};

