/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLView.h"
#include "nuiHTTP.h"
#include "nuiFontManager.h"
#include "nuiHTTP.h"
#include "nuiUnicode.h"

#include "nuiHTMLContext.h"
#include "nuiHTMLItem.h"
#include "nuiHTMLBox.h"
#include "nuiHTMLHeader.h"
#include "nuiHTMLText.h"
#include "nuiHTMLImage.h"
#include "nuiHTMLFont.h"


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
  
  mSlotSink.Connect(LinkActivated, nuiMakeDelegate(this, &nuiHTMLView::_AutoSetURL));
}

nuiHTMLView::~nuiHTMLView()
{
  delete mpHTML;
  delete mpContext;
}

void nuiHTMLView::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Text")), nuiUnitName,
                //nuiMakeDelegate(this, &nuiHTMLView::_GetFont), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetText)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Width")), nuiUnitName,
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
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("URL")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiHTMLView::GetURL), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetURL)));
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


nuiRect nuiHTMLView::CalcIdealSize()
{
  float IdealWidth = mIdealWidth;
  if (mRect.GetWidth() > 0)
    IdealWidth = mRect.GetWidth();
  Clear();
//  context.mSetLayout = true;
//  WalkTree(mpHTML, context);
  //  return nuiRect(context.mMaxWidth, context.mH);

  mpContext->mMaxWidth = IdealWidth;
  if (!mpRootBox)
    return nuiRect(IdealWidth, 400.0f);
  
  nuiHTMLContext context(*mpContext);
  mpRootBox->Layout(context);
  return nuiRect(mpRootBox->GetIdealRect().GetWidth(), mpRootBox->GetIdealRect().GetHeight());
}

void nuiHTMLView::ReLayout()
{
  if (!mpRootBox)
    return;
  
  nuiHTMLContext context(*mpContext);
  mpRootBox->Layout(context);
  mpRootBox->SetRect(mpRootBox->GetIdealRect());
}


bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  ReLayout();
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  nuiSimpleContainer::Draw(pContext);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableBlending(true);
  if (mpRootBox)
  {
    mpRootBox->CallDraw(pContext);
  }
  return true;
}

void nuiHTMLView::SetIdealWidth(float IdealWidth)
{
  mIdealWidth = IdealWidth;
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
  Clear();
  nuiHTML* pHTML = new nuiHTML();
  
  std::string str(rHTMLText.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = pHTML->Load(mem);
  
  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
    mpRootBox = new nuiHTMLBox(mpHTML, false);
    ParseTree(mpHTML, mpRootBox);

    nuiHTMLContext context(*mpContext);
    mpRootBox->Layout(context);
    InvalidateLayout();
  }
  return res;
}

bool nuiHTMLView::SetURL(const nglString& rURL)
{
  nglString url(rURL);
  nuiHTTPRequest request(rURL);
  nuiHTTPResponse* pResponse = request.SendRequest();
  if (!pResponse)
    return false;

  //NGL_OUT(_T("\n\nHTTP Headers:\n%ls\n\n"), pResponse->GetHeadersRep().GetChars());
  
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
    //NGL_OUT(_T("\n\nNew location: %ls\n\n"), url.GetChars());
    
    delete pResponse;
    return SetURL(url);
  }

  it = rHeaders.find(_T("content-type"));

  nglTextEncoding encoding = eEncodingUnknown;
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
      //NGL_OUT(_T("\n\nHTTP Encoding: %ls - %d\n\n"), enc.GetChars(), encoding);

    }
  }
  
  
  nuiHTML* pHTML = new nuiHTML();
  pHTML->SetSourceURL(url);
  nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
  
  bool res = pHTML->Load(mem, encoding);
  
  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
    mpRootBox = new nuiHTMLBox(mpHTML, false);
    ParseTree(mpHTML, mpRootBox);

    nuiHTMLContext context(*mpContext);
    mpRootBox->Layout(context);
    InvalidateLayout();
  }
  return res;
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
//          printf("tree??? '%ls'\n", pChild->GetName().GetChars());
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
        ParseBody(pChild, pBox);
        break;
        
      default:
        {        
          //printf("html??? '%ls'\n", pChild->GetName().GetChars());
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
      default:
        {        
          //printf("head??? '%ls'\n", pChild->GetName().GetChars());
        }
        break;
    }
  }
}

void nuiHTMLView::ParseTitle(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
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
        if (pChild->GetName().IsEmpty())
          ParseText(pChild, pBox);
        else
        {
          //printf("body??? '%ls'\n", pChild->GetName().GetChars());
          ParseBody(pChild, pBox);
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
    pBox->AddItem(new nuiHTMLText(pNode, words[i]));
  }
  //ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseDiv(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, false);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseTable(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, false);
  pBox->AddItem(pNewBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TR:
        ParseTableRow(pChild, pNewBox);
        break;
    }
  }
}

void nuiHTMLView::ParseImage(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLImage* pImg = new nuiHTMLImage(pNode);
  pBox->AddItem(pImg);
}

void nuiHTMLView::ParseTableRow(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, false);
  pBox->AddItem(pNewBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TD:
        ParseBody(pChild, pNewBox);
        break;
    }
  }
}


void nuiHTMLView::ParseList(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html list\n");
  nuiHTMLBox* pListBox = new nuiHTMLBox(pNode, false);
  pBox->AddItem(pListBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pListItem = pNode->GetChild(i);
    switch (pListItem->GetTagType())
    {
      case nuiHTML::eTag_LI:
      {
        nuiHTMLBox* pListItemBox = new nuiHTMLBox(pListItem, false);
        pListBox->AddItem(pListItemBox);
        ParseBody(pListItem, pListItemBox);
      }
      break;
    }
  }
  //printf("html /list\n");
}

void nuiHTMLView::ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, false);
  pNewBox->ForceLineBreak(true);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseHeader(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLHeader(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseFormatTag(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, true));
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, true));
}

void nuiHTMLView::ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, true));
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, true));
}

void nuiHTMLView::ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, false));
}

void nuiHTMLView::ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseFont(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLFont(pNode));
  
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLFont(pNode));
}

void nuiHTMLView::_SetText(const nglString& rHTMLText)
{
  SetText(rHTMLText);
}

bool nuiHTMLView::MouseClicked(const nglMouseInfo& rInfo)
{
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
          nuiHTMLNode* pNode = items[ii]->GetNode();
          if (pNode)
          {
            nuiHTMLAttrib* attrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HREF);
            if (attrib)
            {
              LinkActivated(attrib->GetValue());
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
  
  return false;
}


void nuiHTMLView::_SetURL(const nglString& rURL)
{
  SetURL(rURL);
}

void nuiHTMLView::_AutoSetURL(const nglString& rURL)
{
  if (mAutoActivateLink)
    SetURL(rURL);
}

void nuiHTMLView::SetAutoActivateLink(bool set)
{
  mAutoActivateLink = set;
}

bool nuiHTMLView::IsAutoActivateLink() const
{
  return mAutoActivateLink;
}
