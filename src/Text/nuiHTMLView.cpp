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
  mLeftMargin = 0;
  
  mMaxWidth = 640;
  
  mVSpace = 0;
  mHSpace = 8;

  mFont.SetName(_T("arial"), 0.8f);
  mFont.SetGenericName(_T("sans-serif"), 0.8f);
  mFont.SetStyle(_T("regular"), 0.5f);
  mFont.SetProportionnal(1.0f);

  mFont.SetScalable(1.0f);
  mFont.MustHaveSize(14.0f, 1.0f);
  mFont.SetItalic(false, 1.0f);
  mFont.SetBold(false, 1.0f);
  mUnderline = false;
  mStrikeThrough = false;
  mTextFgColor = nuiColor(0,0,0,255);
  mTextBgColor = nuiColor(0,0,0,0);
}

nuiHTMLContext::nuiHTMLContext(const nuiHTMLContext& rContext)
{
  *this = rContext;
}

nuiHTMLContext& nuiHTMLContext::operator=(const nuiHTMLContext& rContext)
{
  mLeftMargin = rContext.mLeftMargin;
  
  mMaxWidth = rContext.mMaxWidth;
  
  mVSpace = rContext.mVSpace;
  mHSpace = rContext.mHSpace;
  
  mFont = rContext.mFont;
  mUnderline = rContext.mUnderline;
  mStrikeThrough = rContext.mStrikeThrough;
  mTextFgColor = rContext.mTextFgColor;
  mTextBgColor = rContext.mTextBgColor;
}


/////////////////class nuiHTMLItem
nuiHTMLItem::nuiHTMLItem(nuiHTMLNode* pNode, bool Inline)
: mpNode(pNode), mInline(Inline), mEndTag(false), mLineBreak(false)
{
  mLineBreak = (pNode->GetTagType() == nuiHTMLNode::eTag_BR);
}

nuiHTMLItem::~nuiHTMLItem()
{
}

void nuiHTMLItem::Draw(nuiDrawContext* pContext)
{
}

void nuiHTMLItem::CallDraw(nuiDrawContext* pContext)
{
  pContext->PushMatrix();
  pContext->Translate(mRect.Left(), mRect.Top());
  //pContext->DrawRect(GetRect().Size(), eStrokeShape);
  Draw(pContext);
  pContext->PopMatrix();
}

void nuiHTMLItem::Layout(nuiHTMLContext& rContext)
{
  bool set = !IsEndTag();
  switch (mpNode->GetTagType())
  {
  case nuiHTML::eTag_I:
    {
      rContext.mFont.SetItalic(set, 1.0f);
    }
    break;
  case nuiHTML::eTag_B:
    {
      rContext.mFont.SetBold(set, 1.0f);
    }
    break;
  case nuiHTML::eTag_U:
    {
      rContext.mUnderline = set;
    }
    break;
  case nuiHTML::eTag_STRIKE:
    {
      rContext.mUnderline = set;
    }
    break;
  }
}

const nuiRect& nuiHTMLItem::GetRect() const
{
  return mRect;
}

void nuiHTMLItem::SetRect(const nuiRect& rRect)
{
  //printf("nuiHTMLItem::SetRect %ls\n", rRect.GetValue().GetChars());
  mRect = rRect;
}

#undef max

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

bool nuiHTMLItem::IsEndTag() const
{
  return mEndTag;
}

void nuiHTMLItem::SetEndTag(bool set)
{
  mEndTag = set;
}

bool nuiHTMLItem::IsLineBreak() const
{
  return mLineBreak;
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

void nuiHTMLBox::AddItemEnd(nuiHTMLItem* pItem)
{
  pItem->SetEndTag(true);
  mItems.push_back(pItem);
  pItem->SetParent(this);
}

void nuiHTMLBox::Draw(nuiDrawContext* pContext)
{
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    mItems[i]->CallDraw(pContext);
  }
}

float nuiHTMLBox::LayoutLine(uint32& start, uint32& end, float& y, float& h, nuiHTMLContext& rContext)
{
  float x = 0;
  //printf("box layout item process line\n");
  // Process the line
  x = rContext.mLeftMargin;
  for (int32 j = start; j <= end; j++)
  {
    nuiHTMLItem* pIt = mItems[j];
    nuiRect r(pIt->GetIdealRect());
    r.SetHeight(h);
    r.MoveTo(x, y);
    pIt->SetRect(r);
    //NGL_OUT(_T("%ls\n"), r.GetValue().GetChars());
    x += r.GetWidth() + rContext.mHSpace;
  }
  y += h + rContext.mVSpace;
  h = 0;
  start = end + 1;
  
  return x;
}

void nuiHTMLBox::Layout(nuiHTMLContext& rContext)
{
  nuiHTMLContext context(rContext);
  float X = 0;
  float Y = 0;
  float W = 0;
  
  //printf("box layout start\n");
  uint32 line_start = 0;
  uint32 line_end = 0;
  float lineh = 0;
  
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    mItems[i]->Layout(context);
  }
  
  if (IsInline())
  {
    //#TODO #FIXME  Wrap the children 
    for (uint32 i = 0; i < mItems.size(); i++)
    {
      //printf("box layout item %d start\n", i);

      nuiHTMLItem* pItem = mItems[i];
      nuiRect r(pItem->GetIdealRect());

      // Layout the line if needed:
      if ((X + r.GetWidth() > context.mMaxWidth) || pItem->IsLineBreak())
      {
        float w = LayoutLine(line_start, line_end, Y, lineh, context);

        W = MAX(W, w);
        lineh = 0;
        X = 0;
      }
      
      lineh = MAX(lineh, r.GetHeight());
      X += r.GetWidth() + context.mVSpace;
      
      line_end = i;
      //printf("box layout item %d done\n", i);
    }

    if (line_end)
    {
      float w = LayoutLine(line_start, line_end, Y, lineh, context);
      
      W = MAX(W, w);
      lineh = 0;
      X = 0;
    }
  }
  else
  {
    float x = context.mLeftMargin;
    // Put each children on a new line
    for (uint32 i = 0; i < mItems.size(); i++)
    {
      //printf("box layout item block line %d\n", i);
      nuiHTMLItem* pItem = mItems[i];
      nuiRect r(pItem->GetIdealRect());
      r.MoveTo(X + x, Y);
      pItem->SetRect(r);
      Y += r.GetHeight() + context.mVSpace;
      W = MAX(W, r.Right());
    }
  }
  mIdealRect.Set(0.0f, 0.0f, W, Y);
  //printf("text layout done (%ls)\n", mIdealRect.GetValue().GetChars());
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
  pContext->SetTextColor(mTextFgColor);
  //nuiColor mTextBgColor;
  pContext->SetFont(mpFont, false);
  pContext->DrawText(0, mpLayout->GetAscender() , *mpLayout);
}

void nuiHTMLText::Layout(nuiHTMLContext& rContext)
{
  if (mpLayout)
  {
    return;
  }
  delete mpLayout;
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(rContext.mFont);
  //nuiFont* pFont = nuiFont::GetFont(12);
  pFont->Acquire();
  if (mpFont)
    mpFont->Release();
  mpFont = pFont;
  
  mpLayout = new nuiFontLayout(*mpFont, 0, 0, nuiHorizontal);
  mpLayout->SetUnderline(rContext.mUnderline);
  mpLayout->SetStrikeThrough(rContext.mStrikeThrough);
  mpLayout->Layout(mText);
  mIdealRect = mpLayout->GetRect();
  //printf("text layout done (%ls)\n", mIdealRect.GetValue().GetChars());
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
  mpRootBox = NULL;
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
//  context.mSetLayout = true;
//  WalkTree(mpHTML, context);
  //  return nuiRect(context.mMaxWidth, context.mH);
  nuiHTMLContext context;
  context.mMaxWidth = IdealWidth;
  if (!mpRootBox)
    return nuiRect(IdealWidth, 400.0f);
  mpRootBox->Layout(context);
  return nuiRect(mpRootBox->GetIdealRect().GetWidth(), mpRootBox->GetIdealRect().GetHeight());
}

bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  if (!mpRootBox)
    return true;
  nuiHTMLContext context;
  context.mMaxWidth = mRect.GetWidth();
  mpRootBox->Layout(context);
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  nuiSimpleContainer::Draw(pContext);
  if (mpRootBox)
    mpRootBox->CallDraw(pContext);
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
    mpRootBox = new nuiHTMLBox(mpHTML);
    ParseTree(mpHTML, mpRootBox);
    nuiHTMLContext context;
    mpRootBox->Layout(context);
    InvalidateLayout();
  }
  return res;
}

//void nuiHTMLView::WalkTree(nuiHTMLNode* pNode, const nuiHTMLContext& rContext)
//{
//  nuiHTMLContext context(rContext);
//  context.mpNode = pNode;
//  if (!InterpretTree(context))
//    return;
//  
//  uint32 count = pNode->GetNbChildren();
//  for (uint32 i = 0; i < count; i++)
//  {
//    nuiHTMLNode* pChild = pNode->GetChild(i);
//    WalkTree(pChild, context);
//  }
//  LayoutLine(context);
//}
//

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
  //printf("html\n");
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
  //printf("/html\n");
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
  //printf("html title\n");
}

void nuiHTMLView::ParseBody(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html body\n");
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
      case nuiHTML::eTag_UL:
      case nuiHTML::eTag_OL:
      case nuiHTML::eTag_DL:
        ParseList(pChild, pBox);
        break;
      case nuiHTML::eTag_P:
        ParseP(pChild, pBox);
        break;
      case nuiHTML::eTag_I:
      case nuiHTML::eTag_B:
      case nuiHTML::eTag_U:
      case nuiHTML::eTag_STRIKE:
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
  //printf("body /body\n");
}

void nuiHTMLView::ParseText(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html text\n");
  const nglString& rText(pNode->GetText());
  std::vector<nglString> words;
  rText.Tokenize(words);
  
  for (uint32 i = 0; i < words.size(); i++)
  {
    pBox->AddItem(new nuiHTMLText(pNode, words[i]));
  }
  //ParseBody(pNode, pBox);
  //printf("html /text\n");
}

void nuiHTMLView::ParseDiv(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html div\n");
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
  //printf("html /div\n");
}

void nuiHTMLView::ParseTable(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html table\n");
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
  //printf("html /table\n");
}

void nuiHTMLView::ParseTableRow(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html table row\n");
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, true);
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
  //printf("html /table row\n");
}


void nuiHTMLView::ParseList(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html list\n");
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, false);
  pBox->AddItem(pNewBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_LI:
      {
        uint32 count2 = pChild->GetNbChildren();
        for (uint32 j = 0; j < count2; j++)
        {
          nuiHTMLNode* pChild2 = pChild->GetChild(j);
          switch (pChild2->GetTagType())
          {
            case nuiHTML::eTag_LI:
              ParseBody(pChild2, pNewBox);
              break;
          }
        }
      }
      break;
    }
  }
  //printf("html /list\n");
}

void nuiHTMLView::ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html p\n");
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
  //printf("html /p\n");
}

void nuiHTMLView::ParseFormatTag(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html format\n");
  pBox->AddItem(new nuiHTMLItem(pNode));
  
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, false));
  //printf("html /format\n");
}

void nuiHTMLView::ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html anchor\n");
  ParseBody(pNode, pBox);
  //printf("html /anchor\n");
}

void nuiHTMLView::ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html break\n");
  pBox->AddItem(new nuiHTMLItem(pNode));
}

void nuiHTMLView::ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html span\n");
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
  //printf("html /span\n");
}

void nuiHTMLView::ParseFont(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLFont(pNode));
  
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLFont(pNode));
}

//class nuiHTMLFont : public nuiHTMLItem
nuiHTMLFont::nuiHTMLFont(nuiHTMLNode* pNode)
: nuiHTMLItem(pNode, true)
{
  //nuiFontRequest mFontRequest;

  mSize = 14;
  mUnderline = false;
  mStrikeThrough = false;
  mTextFgColor = nuiColor(0, 0, 0, 255);
  mTextBgColor = nuiColor(0, 0, 0, 0);
}

nuiHTMLFont::~nuiHTMLFont()
{
  
}

void nuiHTMLFont::Draw(nuiDrawContext* pContext)
{
  
}

void nuiHTMLFont::Layout(nuiHTMLContext& rContext)
{
  if (!IsEndTag())
  {
    mBackup = rContext.mFont;
    if (!mFamilyName.IsEmpty())
      rContext.mFont.SetName(mFamilyName, 1.0f);
    if (!mGenericName.IsEmpty())
      rContext.mFont.SetGenericName(mGenericName, 1.0f);
    if (mSize > 0)
      rContext.mFont.MustHaveSize(mSize, 1.0f);
    if (mUnderline >= 0)
      rContext.mUnderline = mUnderline;
    if (mStrikeThrough >= 0)
      rContext.mStrikeThrough = mStrikeThrough;
    
    rContext.mTextFgColor = mTextFgColor;
    rContext.mTextBgColor = mTextBgColor;
  }
  else
  {
    rContext.mFont = mBackup;
  }
  
}

