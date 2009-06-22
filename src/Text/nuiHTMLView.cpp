/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLView.h"
#include "nuiHTTP.h"
#include "nuiFontManager.h"

///////////class nuiHTMLContext
nuiHTMLContext::nuiHTMLContext()
{
  mX = 0;
  mY = 0;
  mW = 0;
  mH = 0;
  
  mLeftMargin = 0;
  
  mIdealWidth = 800;
  mMaxWidth = 0;
  mMaxHeight = 0;
  
  mVSpace = 0;
  mHSpace = 0;
  
  mSetLayout = false;
  //mLine;
  mpNode = NULL;
}

nuiHTMLContext::nuiHTMLContext(const nuiHTMLContext& rContext)
{
  *this = rContext;
}

nuiHTMLContext& nuiHTMLContext::operator=(const nuiHTMLContext& rContext)
{
  mX = rContext.mX;
  mY = rContext.mY;
  mW = rContext.mW;
  mH = rContext.mH;
  
  mLeftMargin = rContext.mLeftMargin;
  
  mIdealWidth = rContext.mIdealWidth;
  mMaxWidth = rContext.mMaxWidth;
  mMaxHeight = rContext.mMaxHeight;
  
  mVSpace = rContext.mVSpace;
  mHSpace = rContext.mHSpace;
  
  mSetLayout = rContext.mSetLayout;
  mLine = rContext.mLine;
  mpNode = rContext.mpNode;  
}


/////////////////class nuiHTMLItem
nuiHTMLItem::nuiHTMLItem(nuiHTMLNode* pNode, bool Inline)
: mpNode(pNode), mInline(Inline)
{
}

nuiHTMLItem::~nuiHTMLItem()
{
}

void nuiHTMLItem::Draw(nuiDrawContext* pContext)
{
}

void nuiHTMLItem::Layout(nuiHTMLContext& rContext)
{
  
}

const nuiRect& nuiHTMLItem::GetRect() const
{
  return mRect;
}

void nuiHTMLItem::SetRect(const nuiRect& rRect)
{
  mRect = rRect;
}

const nuiRect& nuiHTMLItem::GetIdealRect() const
{
  return mIdealRect;
}

void nuiHTMLItem::MoveTo(float x, float y)
{
  mRect.MoveTo(x, y);
}

void nuiHTMLItem::SetWidth(float w)
{
  mRect.SetWidth(w);
}

void nuiHTMLItem::SetHeight(float h)
{
  mRect.SetHeight(h);
}

void nuiHTMLItem::SetParent(nuiHTMLBox* pBox)
{
  mpParent = pBox;
}

nuiHTMLBox* nuiHTMLItem::GetParent() const
{
  return mpParent;
}

float nuiHTMLItem::GetAscender() const
{
  return mIdealRect.GetHeight();
}

float nuiHTMLItem::GetDescender() const
{
  return 0;
}

bool nuiHTMLItem::IsInline() const
{
  return mInline;
}

void nuiHTMLItem::SetInline(bool set)
{
  mInline = set;
}

////////////////////class nuiHTMLBox
nuiHTMLBox::nuiHTMLBox(nuiHTMLNode* pNode, bool Inline)
: nuiHTMLItem(pNode, Inline)
{
  
}

nuiHTMLBox::~nuiHTMLBox()
{
  for (uint32 i = 0 ; i < mItems.size(); i++)
  {
    delete mItems[i];
  }
}

void nuiHTMLBox::AddItem(nuiHTMLItem* pItem)
{
  mItems.push_back(pItem);
  pItem->SetParent(this);
}

void nuiHTMLBox::Draw(nuiDrawContext* pContext)
{
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    mItems[i]->Draw(pContext);
  }
}

void nuiHTMLBox::Layout(nuiHTMLContext& rContext)
{
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    mItems[i]->Layout(rContext);
  }
  
  if (IsInline())
  {
    //#TODO #FIXME  Wrap the children 
    for (uint32 i = 0; i < mItems.size(); i++)
    {
      nuiHTMLItem* pItem = mItems[i];
      nuiRect r(pItem->GetIdealRect());
      r.MoveTo(rContext.mX, rContext.mY);
      pItem->SetRect(r);
      rContext.mY += r.GetHeight();
    }
  }
  else
  {
    rContext.mX = rContext.mLeftMargin;
    // Put each children on a new line
    for (uint32 i = 0; i < mItems.size(); i++)
    {
      nuiHTMLItem* pItem = mItems[i];
      nuiRect r(pItem->GetIdealRect());
      r.MoveTo(rContext.mX, rContext.mY);
      pItem->SetRect(r);
      rContext.mY += r.GetHeight();
    }
  }
}

void nuiHTMLBox::PushContext(const nuiHTMLContext& rContext)
{
  mContextStack.push(rContext);
}

void nuiHTMLBox::PopContext(nuiHTMLContext& rContext)
{
  rContext = mContextStack.top();
  mContextStack.pop();
}

///////////////////////////////////////// nuiHTMLText
nuiHTMLText::nuiHTMLText(nuiHTMLNode* pNode, const nglString& rText)
: nuiHTMLItem(pNode, true), mText(rText), mpLayout(NULL), mpFont(NULL)
{
  
}

nuiHTMLText::~nuiHTMLText()
{
  if (mpFont)
    mpFont->Release();
  delete mpLayout;
}

void nuiHTMLText::Draw(nuiDrawContext* pContext)
{
  pContext->DrawText(mRect.Left(), mRect.Right(), *mpLayout);
}

void nuiHTMLText::Layout(nuiHTMLContext& rContext)
{
  delete mpLayout;
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(rContext.mFont);
  pFont->Acquire();
  mpFont->Release();
  mpFont = pFont;
  
  mpLayout = new nuiFontLayout(*mpFont, 0, 0, nuiHorizontal);
  mpLayout->Layout(mText);
  mIdealRect = mpLayout->GetRect();
}

float nuiHTMLText::GetAscender() const
{
  return mpLayout->GetAscender();
}

float nuiHTMLText::GetDescender() const
{
  return mpLayout->GetDescender();
}



/////////////////////////////// nuiHTMLView
nuiHTMLView::nuiHTMLView(float IdealWidth)
{
  mpHTML = NULL;
  mIdealWidth = IdealWidth;
  mVSpace = 2.0f;
  mHSpace = 0.0f;
}

nuiHTMLView::~nuiHTMLView()
{
  delete mpHTML;
}

nuiRect nuiHTMLView::CalcIdealSize()
{
  float IdealWidth = mIdealWidth;
  if (mRect.GetWidth() > 0)
    IdealWidth = mRect.GetWidth();
  Clear();
  nuiHTMLContext context;
  context.mSetLayout = true;
  WalkTree(mpHTML, context);
  //  return nuiRect(context.mMaxWidth, context.mH);
  return nuiRect(800, 600);
}

void nuiHTMLView::AddElement(nuiHTMLContext& rContext, nuiWidgetPtr pWidget)
{
  AddChild(pWidget);
  
  nuiRect r(pWidget->GetIdealRect());
  if ((rContext.mW != rContext.mLeftMargin) && (rContext.mW + r.GetWidth() > rContext.mIdealWidth))
    LayoutLine(rContext);
  
  rContext.mH = MAX(rContext.mH, r.GetHeight());
  rContext.mW += r.GetWidth();
  if (!rContext.mLine.empty())
    rContext.mW += rContext.mHSpace;
  rContext.mLine.push_back(pWidget);
}

void nuiHTMLView::LayoutLine(nuiHTMLContext& rContext)
{
  // Process the line
  if (rContext.mSetLayout)
  {
    int32 size = rContext.mLine.size();
    rContext.mX = rContext.mLeftMargin;
    for (int32 j = 0; j < size; j++)
    {
      nuiWidgetPtr pWidget = rContext.mLine[j];
      nuiRect r(pWidget->GetIdealRect());
      r.SetHeight(rContext.mH);
      r.MoveTo(rContext.mX, rContext.mY);
      pWidget->SetLayout(r);
      NGL_OUT(_T("%ls\n"), r.GetValue().GetChars());
      rContext.mX += r.GetWidth();
      rContext.mY += rContext.mHSpace;
    }
  }
  rContext.mMaxWidth = MAX(rContext.mMaxWidth, rContext.mW);
  rContext.mLine.clear();
  rContext.mY += rContext.mH + rContext.mVSpace;
  rContext.mH = 0;
  rContext.mW = 0;
}

bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  Clear();
  nuiHTMLContext context;
  context.mSetLayout = true;
  WalkTree(mpHTML, context);
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  nuiSimpleContainer::Draw(pContext);
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
  delete mpHTML;
  mpHTML = new nuiHTML();
  
  std::string str(rHTMLText.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = mpHTML->Load(mem);
  
  InvalidateLayout();
  return res;
}

bool nuiHTMLView::SetURL(const nglString& rURL)
{
  nuiHTTPRequest request(rURL);
  nuiHTTPResponse* pResponse = request.SendRequest();
  if (!pResponse)
    return false;
  
  nuiHTML* pHTML = new nuiHTML();
  nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
  
  bool res = pHTML->Load(mem);
  
  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
    InvalidateLayout();
  }
  return res;
}



bool nuiHTMLView::InterpretTree(nuiHTMLContext& rContext)
{
  nuiHTMLNode* pNode = rContext.mpNode;
  
  switch (pNode->GetType())
  {
    case nuiHTML::eNode_Root:
    case nuiHTML::eNode_Text:
    case nuiHTML::eNode_Start:
    case nuiHTML::eNode_End:
    case nuiHTML::eNode_StartEnd:
      NGL_OUT(_T("Interpret %ls - %ls\n"), pNode->GetName().GetChars(), pNode->GetText().GetChars());
      if (!pNode->GetText().IsEmpty())
        AddElement(rContext, new nuiLabel(pNode->GetText()));
      return true;
    case nuiHTML::eNode_ProcIns:
    case nuiHTML::eNode_Comment:
    case nuiHTML::eNode_DocType:
    case nuiHTML::eNode_CDATA:
    case nuiHTML::eNode_Section:
    case nuiHTML::eNode_Asp:
    case nuiHTML::eNode_Jste:
    case nuiHTML::eNode_Php:
    case nuiHTML::eNode_XmlDecl:
    default:
      return false;
      break;
  }
  return true;
}

void nuiHTMLView::WalkTree(nuiHTMLNode* pNode, const nuiHTMLContext& rContext)
{
  nuiHTMLContext context(rContext);
  context.mpNode = pNode;
  if (!InterpretTree(context))
    return;
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    WalkTree(pChild, context);
  }
  LayoutLine(context);
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
        ParseHTML(pNode, pBox);
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
        ParseHead(pNode, pBox);
        break;
        
      case nuiHTML::eTag_BODY:
        ParseBody(pNode, pBox);
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
        ParseTitle(pNode, pBox);
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
        ParseDiv(pNode, pBox);
        break;
      case nuiHTML::eTag_P:
        ParseP(pNode, pBox);
        break;
      case nuiHTML::eTag_I:
        ParseI(pNode, pBox);
        break;
      case nuiHTML::eTag_A:
        ParseA(pNode, pBox);
        break;
      case nuiHTML::eTag_B:
        ParseB(pNode, pBox);
        break;
      case nuiHTML::eTag_BR:
        ParseBr(pNode, pBox);
        break;
      case nuiHTML::eTag_SPAN:
        ParseI(pNode, pBox);
        break;
      case nuiHTML::eTag_U:
        ParseI(pNode, pBox);
        break;
      default:
        ParseText(pNode, pBox);
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
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseI(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  
  ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseB(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  
  ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseU(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}


