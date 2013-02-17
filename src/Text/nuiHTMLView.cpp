/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#include "nuiHTMLContext.h"
#include "nuiHTMLItem.h"
#include "nuiHTMLBox.h"
#include "nuiHTMLHeader.h"
#include "nuiHTMLTable.h"
#include "nuiHTMLText.h"
#include "nuiHTMLImage.h"
#include "nuiHTMLFont.h"

#include "nuiWebCSS.h"


/////////////////////////////// nuiHTMLView
nuiHTMLView::nuiHTMLView(float IdealWidth)
{
  if (SetObjectClass(_T("nuiHTMLView")))
    InitAttributes();
  
  mpHTML = NULL;
  mpRootBox = NULL;
  mIdealWidth = IdealWidth;
  mVSpace = 2.0f;
  mHSpace = 0.0f;
  mpContext = new nuiHTMLContext();
  mClicked = false;
  mAutoActivateLink = true;
  mpCurrentAnchor = NULL;
  mpStream = NULL;
  
  mMouseX = 0;
  mMouseY = 0;
  
  mDebugBoxes = false;
  mAutoIdealWidth = true;
  
  mUseToolTips = true;

  mMargins = 8;
  
  mCanRespectConstraint = true;
  
  mSlotSink.Connect(LinkActivated, nuiMakeDelegate(this, &nuiHTMLView::_AutoSetURL));
}

nuiHTMLView::~nuiHTMLView()
{
  Cancel();
	
  delete mpHTML;
  delete mpContext;
  delete mpRootBox;
}

void nuiHTMLView::Cancel()
{
  if (mpStream)
  {
    mpStream->Cancel();
    mpStream = NULL;
  }
}

void nuiHTMLView::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Text")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetText), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetText)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Width")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetIdealWidth), 
                nuiMakeDelegate(this, &nuiHTMLView::SetIdealWidth)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("IdealWidth")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetIdealWidth), 
                nuiMakeDelegate(this, &nuiHTMLView::SetIdealWidth)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("VSpace")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetVSpace), 
                nuiMakeDelegate(this, &nuiHTMLView::SetVSpace)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("HSpace")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetHSpace), 
                nuiMakeDelegate(this, &nuiHTMLView::SetHSpace)));  
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Font")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::_GetFont), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetFont)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("TextColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiHTMLView::GetTextColor), 
                nuiMakeDelegate(this, &nuiHTMLView::SetTextColor)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("LinkColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiHTMLView::GetLinkColor), 
                nuiMakeDelegate(this, &nuiHTMLView::SetLinkColor)));
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("URL")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiHTMLView::GetURL), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetURL)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("AutoIdealWidth")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetAutoIdealWidth), 
                nuiMakeDelegate(this, &nuiHTMLView::SetAutoIdealWidth)));  
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("UseToolTips")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetUseToolTips), 
                nuiMakeDelegate(this, &nuiHTMLView::SetUseToolTips)));  
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("DebugBoxes")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiHTMLView::GetDebugBoxes), 
                nuiMakeDelegate(this, &nuiHTMLView::SetDebugBoxes)));  
  

  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Margins")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiHTMLView::GetMargins), 
                nuiMakeDelegate(this, &nuiHTMLView::SetMargins)));
  
}


void nuiHTMLView::SetFont(nuiFont* pFont, bool AlreadyAcquired)
{
  if (!pFont)
  {
    AlreadyAcquired = true;
    pFont = nuiFont::GetFont(14.0f);
  }
  
  mpContext->mFont = nuiFontRequest(pFont, false);
  if (AlreadyAcquired)
    pFont->Release();
  
  InvalidateLayout();
}

void nuiHTMLView::SetFont(nuiFontRequest& rFontRequest)
{
  mpContext->mFont = rFontRequest;
  InvalidateLayout();
}

void nuiHTMLView::SetFont(const nglString& rFontSymbol)
{
  nuiFont* pFont = nuiFont::GetFont(rFontSymbol);
  if (pFont)
    SetFont(pFont, true);
}


void nuiHTMLView::_SetFont(const nglString& rFontSymbol)
{
  SetFont(rFontSymbol);
}

const nglString& nuiHTMLView::_GetFont() const
{
  return mpContext->mFont.mName.mElement;
}

const nuiColor& nuiHTMLView::GetTextColor() const
{
  return mpContext->mTextFgColor;
}

void nuiHTMLView::SetTextColor(const nuiColor& Color)
{
  mpContext->mTextFgColor = Color;
  ReLayout();
  Invalidate();
}

const nuiColor& nuiHTMLView::GetLinkColor() const
{
  return mpContext->mLinkColor;
}

void nuiHTMLView::SetLinkColor(const nuiColor& Color)
{
  mpContext->mLinkColor = Color;
  ReLayout();
  Invalidate();
}


nuiRect nuiHTMLView::CalcIdealSize()
{
//  if (GetDebug())
//  {
//    printf("?");    
//  }
  
  float IdealWidth = mIdealWidth;
  if (mAutoIdealWidth && (mRect.GetWidth() > 0))
    IdealWidth = mRect.GetWidth();
  if (mConstraint.mMaxWidth >= 0)
    IdealWidth = mConstraint.mMaxWidth;
  //Clear();

  mpContext->mMaxWidth = IdealWidth;

  if (!mpRootBox)
    return nuiRect(IdealWidth, 400.0f);
  
  nuiHTMLContext context(*mpContext);
  mpHTML->UpdateStyle(context);
  mpRootBox->Layout(context);
  return nuiRect(mpRootBox->GetIdealRect().GetWidth(), mpRootBox->GetIdealRect().GetHeight());
}

void nuiHTMLView::ReLayout()
{
  if (!mpRootBox)
    return;
  
//  if (GetDebug())
//  {
//    printf("/");    
//  }

  nuiHTMLContext context(*mpContext);
  mpHTML->UpdateStyle(context);
  mpRootBox->Layout(context);
  mpRootBox->SetLayout(mpRootBox->GetIdealRect());
  mLastVisibleRect = nuiRect();
}


bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
//  if (GetDebug())
//  {
//    printf(".");    
//  }
  
  nuiWidget::SetRect(rRect);

  if (mAutoIdealWidth && (mRect.GetWidth() > 0))
    SetIdealWidth(rRect.GetWidth());

  ReLayout();
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  if (!mpRootBox)
    return true;
  
  if (!(mLastVisibleRect == mVisibleRect))
  {
    mpRootBox->UpdateVisibility(mVisibleRect);
    mLastVisibleRect = mVisibleRect;
  }
  
  nuiSimpleContainer::Draw(pContext);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableBlending(true);
  if (mpRootBox)
  {
    mpRootBox->CallDraw(pContext);
  }
  
  if (mDebugBoxes)
  {
    std::vector<nuiHTMLItem*> items;
    mpRootBox->GetItemsAt(items, mMouseX, mMouseY);
    if (!items.empty())
    {
      float alpha = 0.5f;
      for (uint32 i = 0; i < items.size(); i++)
      {
        int32 ii = items.size() - 1 - i;
        nuiHTMLItem* pItem = items[ii];
        
        pContext->SetFillColor(nuiColor(0.0, 0.0, 1.0, alpha * .5f));
        pContext->SetStrokeColor(nuiColor(0.0, 0.0, 1.0, alpha));

        nuiRect r(pItem->GetGlobalRect());
        pContext->DrawRect(r, eStrokeAndFillShape);
        
        alpha *= 0.5f;
      }
    }
  }
  return true;
}

void nuiHTMLView::SetIdealWidth(float IdealWidth)
{
  if (mIdealWidth == IdealWidth)
    return;
  
  mIdealWidth = IdealWidth;
  if (mpContext)
    mpContext->mMaxWidth = mIdealWidth;
  InvalidateLayout();
}

float nuiHTMLView::GetIdealWidth() const
{
  return mIdealWidth;
}

float nuiHTMLView::GetVSpace() const
{
  return mVSpace;
}

float nuiHTMLView::GetHSpace() const
{
  return mHSpace;
}

void nuiHTMLView::SetVSpace(float VSpace)
{
  mVSpace = VSpace;
  InvalidateLayout();
}

void nuiHTMLView::SetHSpace(float HSpace)
{
  mHSpace = HSpace;
  InvalidateLayout();
}

bool nuiHTMLView::SetText(const nglString& rHTMLText)
{
  if (rHTMLText == mText)
    return true;

  Cancel();
  Clear();

  mText = rHTMLText;
  if (rHTMLText.IsEmpty())
    return true;
  
  nuiHTML* pHTML = new nuiHTML(true);
  
  std::string str(mText.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = pHTML->Load(mem);
  
  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
	  
    delete mpRootBox;
	  
    mpRootBox = new nuiHTMLBox(mpHTML, mpCurrentAnchor, false);
    mpRootBox->SetMargins(mMargins);
    mpRootBox->SetLayoutChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::InvalidateLayout));
    mpRootBox->SetDisplayChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::Invalidate));
    ParseTree(mpHTML, mpRootBox);

    nuiHTMLContext context(*mpContext);
    mpHTML->UpdateStyle(context);
    mpRootBox->Layout(context);
    InvalidateLayout();
    SetHotRect(nuiRect());
  }
  return res;
}

const nglString& nuiHTMLView::GetText() const
{
  return mText;
}

bool nuiHTMLView::SetURL(const nglString& rURL)
{
  Cancel();
  
  nglString url(rURL);
  mTempURL = rURL;

  mpStream = new nuiAsyncIStream(rURL, true, nuiMakeDelegate(this, &nuiHTMLView::StreamDone));
  return true;
}

void nuiHTMLView::StreamDone(nuiAsyncIStream* pStream)
{
	// this has been deleted already..
 if(IsTrashed(true))
 {

	 return;
 }
	
//  App->GetLog().SetLevel(_T("StopWatch"), 50);
//  nuiStopWatch watch(_T("nuiHTMLView::StreamDone"));
  mpStream = NULL;
  nglString url(mTempURL);
  const nuiHTTPResponse* pResponse = pStream->GetHTTPResponse();
  URLChanged(url);
  
  
  nglTextEncoding encoding = eEncodingUnknown;

  if (pResponse)
  {
    //NGL_OUT(_T("\n\nHTTP Headers:\n%s\n\n"), pResponse->GetHeadersRep().GetChars());
    const nuiHTTPHeaderMap& rHeaders(pResponse->GetHeaders());
    nuiHTTPHeaderMap::const_iterator it = rHeaders.find(_T("location"));
    if (it != rHeaders.end())
    {
      nglString newurl = it->second;
      if (newurl[0] == '/')
      {
        url.TrimRight('/');
        url += newurl;
      }
      else
      {
        url = newurl;
      }
      //NGL_OUT(_T("\n\nNew location: %s\n\n"), url.GetChars());
      
      SetURL(url);
      return;
    }
    
    it = rHeaders.find(_T("content-type"));
    
    if (it != rHeaders.end())
    {  
      nglString contents(it->second);
      contents.ToUpper();
      int32 pos = contents.Find(_T("CHARSET="));
      if (pos >= 0)
      {
        nglString enc(contents.Extract(pos + 8));
        enc.Trim();
        encoding = nuiGetTextEncodingFromString(enc);
        //NGL_OUT(_T("\n\nHTTP Encoding: %s - %d\n\n"), enc.GetChars(), encoding);
        
      }
    }
  }  
  
  nuiHTML* pHTML = new nuiHTML(true);
  pHTML->SetSourceURL(url);
  bool res = pHTML->Load(*pStream, encoding);

  //  watch.AddIntermediate(_T("HTML Loaded"));

  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
	  
		delete mpRootBox;
	  
    mpRootBox = new nuiHTMLBox(mpHTML, mpCurrentAnchor, false);
    mpRootBox->SetMargins(mMargins);
    mpRootBox->SetLayoutChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::InvalidateLayout));
    mpRootBox->SetDisplayChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::Invalidate));
    ParseTree(mpHTML, mpRootBox);

    //    watch.AddIntermediate(_T("HTML Tree Parsed"));
    
    nuiHTMLContext context(*mpContext);
    mpHTML->UpdateStyle(context);
    mpRootBox->Layout(context);
    //    watch.AddIntermediate(_T("HTML Layouted"));
    InvalidateLayout();
    SetHotRect(nuiRect());
  }
	
}

const nglString& nuiHTMLView::GetURL() const
{
  if (mpHTML)
    return mpHTML->GetSourceURL();
  return nglString::Null;
}

void nuiHTMLView::ParseTree(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_HTML:
        {
          ParseHTML(pChild, pBox);
          return;
        }
        break;
      default:
        {
//          printf("tree??? '%s'\n", pChild->GetName().GetChars());
//          ParseTree(pChild, pBox); // Try all children!!!
        }
        break;
    }
  }
}

void nuiHTMLView::ParseHTML(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_HEAD:
        ParseHead(pChild, pBox);
        break;
        
      case nuiHTML::eTag_BODY:
        {
          nuiHTMLBox* pBody = new nuiHTMLBox(pChild, mpCurrentAnchor, false);
          pBox->AddItem(pBody);
          ParseBody(pChild, pBody);
        }
        break;
        
      default:
        {        
          //printf("html??? '%s'\n", pChild->GetName().GetChars());
        }
        break;
    }
  }
}

void nuiHTMLView::ParseHead(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TITLE:
        ParseTitle(pChild, pBox);
        break;
      case nuiHTML::eTag_STYLE:
        ParseStyle(pChild, pBox);
        break;
      case nuiHTML::eTag_LINK:
        ParseHeadLink(pChild, pBox);
        break;
      default:
        {        
          //printf("head??? '%s'\n", pChild->GetName().GetChars());
        }
        break;
    }
  }
}

void nuiHTMLView::ParseTitle(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
}

void nuiHTMLView::ParseStyle(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  if (pNode->GetNbChildren() == 0)
    return;
  
  nuiHTMLNode* pText = pNode->GetChild(0);
  if (pText->GetType() != nuiHTML::eNode_Text)
    return;
  mpHTML->AddStyleSheet(GetURL(), pText->GetText(), false);
}

void nuiHTMLView::ParseHeadLink(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLAttrib* pRel = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_REL);
  nuiHTMLAttrib* pHRef = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HREF);
  nuiHTMLAttrib* pType = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_TYPE);

  if (!pRel)
    return;
  
  if (pRel->GetValue().Compare(_T("stylesheet"), false) != 0)
    return;
  
  if (pType && pType->GetValue().Compare(_T("text/css"), false) != 0)
    return;

  if (!pHRef)
    return;
  
  nglString url = pHRef->GetValue();
  nuiHTML::GetAbsoluteURL(mpHTML->GetSourceURL(), url);
  mpHTML->AddStyleSheet(url);
}

void nuiHTMLView::ParseBody(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_DIV:
        ParseDiv(pChild, pBox);
        break;
      case nuiHTML::eTag_TABLE:
        ParseTable(pChild, pBox);
        break;
      case nuiHTML::eTag_IMG:
        ParseImage(pChild, pBox);
        break;
      case nuiHTML::eTag_DIR:
      case nuiHTML::eTag_MENU:
      case nuiHTML::eTag_UL:
      case nuiHTML::eTag_OL:
      case nuiHTML::eTag_DL:
        ParseList(pChild, pBox);
        break;
      case nuiHTML::eTag_P:
        ParseP(pChild, pBox);
        break;
      case nuiHTML::eTag_H1:
      case nuiHTML::eTag_H2:
      case nuiHTML::eTag_H3:
      case nuiHTML::eTag_H4:
      case nuiHTML::eTag_H5:
      case nuiHTML::eTag_H6:
        ParseHeader(pChild, pBox);
        break;
      case nuiHTML::eTag_I:
      case nuiHTML::eTag_B:
      case nuiHTML::eTag_U:
      case nuiHTML::eTag_STRIKE:
      case nuiHTML::eTag_STRONG:
      case nuiHTML::eTag_EM:
        ParseFormatTag(pChild, pBox);
        break;
      case nuiHTML::eTag_BR:
        ParseBr(pChild, pBox);
        break;
      case nuiHTML::eTag_A:
        ParseA(pChild, pBox);
        break;
      case nuiHTML::eTag_SPAN:
        ParseSpan(pChild, pBox);
        break;
      case nuiHTML::eTag_FONT:
        ParseFont(pChild, pBox);
        break;
      case nuiHTML::eTag_SCRIPT:
      case nuiHTML::eTag_COMMENT:
        // Skip those tags
        break;
      default:
        if (pChild->GetType() != nuiHTML::eNode_Comment)
        {
          if (pChild->GetType() == nuiHTML::eNode_Text)
            ParseText(pChild, pBox);
          else
          {
            //printf("body??? '%s'\n", pChild->GetName().GetChars());
            ParseBody(pChild, pBox);
          }
        }
        break;
    }
  }
}

void nuiHTMLView::ParseText(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  const nglString& rText(pNode->GetText());
  std::vector<nglString> words;
  rText.Tokenize(words);
  
  for (uint32 i = 0; i < words.size(); i++)
  {
    pBox->AddItem(new nuiHTMLText(pNode, mpCurrentAnchor, words[i]));
  }
  //ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseDiv(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseTable(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLTable* pNewBox = new nuiHTMLTable(pNode, mpCurrentAnchor, false);
  pBox->AddItem(pNewBox);
  
  uint32 count = pNode->GetNbChildren();
  int32 index = 0;
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TR:
        ParseTableRow(pChild, pNewBox, index);
        index++;
        break;
      default:
        break;
    }
  }
}

void nuiHTMLView::ParseImage(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLImage* pImg = new nuiHTMLImage(pNode, mpCurrentAnchor);
  pBox->AddItem(pImg);
}

void nuiHTMLView::ParseTableRow(nuiHTMLNode* pNode, nuiHTMLTable* pBox, int32 rowindex)
{ 
  uint32 count = pNode->GetNbChildren();
  int32 colindex = 0;
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TD:
        {
          nuiHTMLBox* pNewBox = new nuiHTMLBox(pChild, mpCurrentAnchor, false);
          ParseBody(pChild, pNewBox);
          pBox->SetCell(colindex, rowindex, pChild, pNewBox);
          
          colindex++;
        }
        break;
      case nuiHTML::eTag_TH:
        {
          static int cnt = 0;
          cnt++;
          nuiHTMLBox* pNewBox = new nuiHTMLBox(pChild, mpCurrentAnchor, false);
          ParseBody(pChild, pNewBox);
          pBox->SetCell(colindex, rowindex, pChild, pNewBox);
          
          colindex++;
        }
      break;
      default:
        break;
    }
  }
}


void nuiHTMLView::ParseList(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  int32 items = 1;
  nuiHTMLAttrib* pStart = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_START);
  if (pStart)
    items = pStart->GetValue().GetCInt();
  //printf("html list\n");
  nuiHTMLBox* pListBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pListBox->SetMarginLeft(25);
  pBox->AddItem(pListBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pListItem = pNode->GetChild(i);
    switch (pListItem->GetTagType())
    {
      case nuiHTML::eTag_LI:
      {
        nuiHTMLBox* pListItemBox = new nuiHTMLBox(pListItem, mpCurrentAnchor, false);
        pListBox->AddItem(pListItemBox);

        nglString txt;
        switch (pNode->GetTagType())
        {
          case nuiHTMLNode::eTag_DIR:
          case nuiHTMLNode::eTag_MENU:
          case nuiHTMLNode::eTag_UL:
            txt = "\xe2\x80\xa2 ";
            break;
          case nuiHTMLNode::eTag_OL:
            txt.Add(items).Add(_T(". "));
            break;
          case nuiHTMLNode::eTag_DL:
            break;
          default:
            break;
        }
        nuiHTMLText* pText = new nuiHTMLText(pListItem, mpCurrentAnchor, txt);
        pListItemBox->AddItem(pText);
        ParseBody(pListItem, pListItemBox);
      }
      break;
      default:
        break;
    }
    
    items++;
  }
  //printf("html /list\n");
}

void nuiHTMLView::ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pNewBox->ForceLineBreak(true);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseHeader(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLHeader(pNode, mpCurrentAnchor);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseFormatTag(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
}

void nuiHTMLView::ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  mpCurrentAnchor = pNode;
  pBox->AddItem(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
  mpCurrentAnchor = NULL;
}

void nuiHTMLView::ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, mpCurrentAnchor, false));
}

void nuiHTMLView::ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseFont(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLFont(pNode, mpCurrentAnchor));
  
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLFont(pNode, mpCurrentAnchor));
}

void nuiHTMLView::_SetText(const nglString& rHTMLText)
{
  SetText(rHTMLText);
}

bool nuiHTMLView::MouseClicked(const nglMouseInfo& rInfo)
{
  if (!mpRootBox)
    return false;
  
  if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
  {
    Grab();
    mClicked = true;
  }
  return false;
}

bool nuiHTMLView::MouseUnclicked(const nglMouseInfo& rInfo)
{
  if (mClicked)
  {
    if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
    {
      std::vector<nuiHTMLItem*> items;
      mpRootBox->GetItemsAt(items, rInfo.X, rInfo.Y);
      if (!items.empty())
      {
        for (uint32 i = 0; i < items.size(); i++)
        {
          int32 ii = items.size() - 1 - i;
          nuiHTMLItem* pItem = items[ii];
          nuiHTMLNode* pNode = pItem->GetAnchor();
          if (pNode)
          {
            nuiHTMLAttrib* pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HREF);
            nuiHTMLAttrib* pAttribTarget = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_TARGET);

            if (pAttrib)
            {
              nglString url(pAttrib->GetValue());
              nuiHTML::GetAbsoluteURL(mpHTML->GetSourceURL(), url);

              if (pAttribTarget && (pAttribTarget->GetValue() == _T("_blank")))
              {
                nuiURL nuiurl(url);
                nuiurl.OpenBrowser();
              }
              else
                LinkActivated(url);
              return true;
            }
          }
        }
      }
      Ungrab();
    }
  }
  return false;
}

bool nuiHTMLView::MouseMoved(const nglMouseInfo& rInfo)
{
  if (!mpRootBox)
    return false;
  
  mMouseX = rInfo.X;
  mMouseY = rInfo.Y;

  if (mDebugBoxes)
    Invalidate();
  
  std::vector<nuiHTMLItem*> items;
  
  
  mpRootBox->GetItemsAt(items, rInfo.X, rInfo.Y);
  if (!items.empty())
  {
    for (uint32 i = 0; i < items.size(); i++)
    {
      int32 ii = items.size() - 1 - i;
      nuiHTMLItem* pItem = items[ii];
      nuiHTMLNode* pNode = pItem->GetAnchor();
      if (pNode)
      {
        nuiHTMLAttrib* pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HREF);
        if (pAttrib)
        {
          nglString url(pAttrib->GetValue());
          nuiHTML::GetAbsoluteURL(mpHTML->GetSourceURL(), url);

          if (mUseToolTips)
            SetToolTip(url);

          SetMouseCursor(eCursorPointingHand);
          return true;
        }
      }
    }
  }
  
  if (mUseToolTips)
    SetToolTip(nglString::Empty);
  SetMouseCursor(eCursorCaret);
  return false;
}


void nuiHTMLView::_SetURL(const nglString& rURL)
{
  SetURL(rURL);
}

void nuiHTMLView::_AutoSetURL(const nglString& rURL)
{
  if (mAutoActivateLink)
  {
    SetURL(rURL);
  }
}

void nuiHTMLView::SetAutoActivateLink(bool set)
{
  mAutoActivateLink = set;
}

bool nuiHTMLView::IsAutoActivateLink() const
{
  return mAutoActivateLink;
}

void nuiHTMLView::SetDebugBoxes(bool set)
{
  mDebugBoxes = set;
  Invalidate();
}

bool nuiHTMLView::GetDebugBoxes() const
{
  return mDebugBoxes;
}

void nuiHTMLView::SetAutoIdealWidth(bool set)
{
  if (set == mAutoIdealWidth)
    return;

  mAutoIdealWidth = set;
  InvalidateLayout();
}

bool nuiHTMLView::GetAutoIdealWidth() const
{
  return mAutoIdealWidth;
}

void nuiHTMLView::SetUseToolTips(bool set)
{
  mUseToolTips = set;
}

bool nuiHTMLView::GetUseToolTips() const
{
  return mUseToolTips;
}

void nuiHTMLView::SetMargins(float margins)
{
  mMargins = margins;
  if (mpRootBox)
    mpRootBox->SetMargins(margins);
}

float nuiHTMLView::GetMargins() const
{
  return mMargins;
}
