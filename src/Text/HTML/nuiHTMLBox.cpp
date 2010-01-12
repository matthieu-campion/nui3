/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLBox.h"
#include "nuiHTMLText.h"

////////////////////class nuiHTMLBox
nuiHTMLBox::nuiHTMLBox(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, bool Inline)
: nuiHTMLItem(pNode, pAnchor, Inline),
 mMarginLeft(0),
 mMarginTop(0),
 mMarginRight(0),
 mMarginBottom(0)
{
  if (pNode->GetTagType() == nuiHTMLNode::eTag_BODY) // Defaults for the body tag
    SetMargins(8);
  
  nuiHTMLAttrib* pAttrib = NULL;
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_BOTTOMMARGIN);
  if (pAttrib)
    mMarginBottom = pAttrib->GetValue().GetCFloat();

  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_TOPMARGIN);
  if (pAttrib)
    mMarginTop = pAttrib->GetValue().GetCFloat();

  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_LEFTMARGIN);
  if (pAttrib)
    mMarginLeft = pAttrib->GetValue().GetCFloat();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_RIGHTMARGIN);
  if (pAttrib)
    mMarginRight = pAttrib->GetValue().GetCFloat();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_MARGINWIDTH);
  if (pAttrib)
    mMarginLeft = mMarginRight = pAttrib->GetValue().GetCFloat();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_MARGINHEIGHT);
  if (pAttrib)
    mMarginTop = mMarginBottom = pAttrib->GetValue().GetCFloat();
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

float nuiHTMLBox::LayoutLine(uint32& start, uint32& count, float& y, float& h, nuiHTMLContext& rContext)
{
  float x = 0;
  //printf("box layout item process line\n");
  // Process the line
  x = 0;
  h = ToAbove(h);
  nuiHTMLText* pLastText = NULL;
  for (int32 j = start; j < start + count; j++)
  {
    nuiHTMLItem* pIt = mItems[j];
    nuiHTMLText* pText = dynamic_cast<nuiHTMLText*>(pIt);
    
    nuiRect r(pIt->GetIdealRect());
    r.SetHeight(h);
    r.MoveTo(x + mMarginLeft, y + mMarginTop);
    r.RoundToAbove();
    pIt->SetLayout(r);
    //NGL_OUT(_T("<%ls> %ls\n"), pIt->GetNode()->GetName().GetChars(), r.GetValue().GetChars());
    x += ToAbove(r.GetWidth());
    
    if (pLastText)
    {
      pLastText->SetNextInRun(pText);
    }

    if (pText)
    {
      pText->SetFirstInRun(!pLastText);
    }


    pLastText = pText;
  }
  //y += ToAbove(h + rContext.mVSpace);
  y += ToAbove(MAX(h, rContext.mVSpace));
  start += count;
  count = 0;
  return x;
}

void nuiHTMLBox::Layout(nuiHTMLContext& rContext)
{
#if 0 // This is some debug code that helps pointing out layout problems:
//  for (uint32 i = 0; i < GetDepth(); i++)
//    printf("  ");
//  nglString id;
//  nuiHTMLAttrib* pAttrib = mpNode->GetAttribute(nuiHTMLAttrib::eAttrib_ID);
//  if (pAttrib)
//    id.Add(_T(" id='")).Add(pAttrib->GetValue()).Add(_T("'"));
//  printf("nuiHTMLBox::Layout <%ls%ls> %ls\n", mpNode->GetName().GetChars(), id.GetChars(), mRect.GetValue().GetChars());
//
//  if (mpNode->GetTagType() == nuiHTMLNode::eTag_LI)
//  {
//    printf("list item\n");
//  }
#endif
  
  nuiHTMLContext context(rContext);
  float X = 0;
  float Y = 0;
  float W = 0;
  context.mMaxWidth -= (mMarginLeft + mMarginRight);
  
  //printf("box layout start\n");
  int32 done = 0;
  uint32 line_start = 0;
  uint32 count_in_line = 0;
  float lineh = 0;
  float lastlineh = 0;
  
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    //printf("box layout item %d start\n", i);
    
    nuiHTMLItem* pItem = mItems[i];
    pItem->Layout(context);
    nuiRect r(pItem->GetIdealRect());
    
    bool linebreak = pItem->IsLineBreak();
//    if (linebreak)
//      printf("\nlinebreak\n\n");
    
    // Layout the line if needed:
    if ((X + r.GetWidth() > context.mMaxWidth) || linebreak || !pItem->IsInline())
    {
      done += count_in_line;
      float w = LayoutLine(line_start, count_in_line, Y, lineh, context);
      W = MAX(W, w);
      lastlineh = lineh;
      lineh = 0;
      X = 0;
    }

    if (linebreak)
    {
      nglFontInfo info;
      rContext.mpFont->GetInfo(info);
      Y += info.Height;
    }
    
    lineh = MAX(lineh, r.GetHeight());
    X += r.GetWidth();
    
    count_in_line++;
    //printf("box layout item %d done\n", i);
  }
  
  if (done != mItems.size())
  {
    float w = LayoutLine(line_start, count_in_line, Y, lineh, context);
    
    W = MAX(W, w);
    lastlineh = lineh;
    lineh = 0;
    X = 0;
  }
  
  nuiRect total;
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    NGL_ASSERT(mItems[i]->mSetRectCalled);
    total.Union(total, mItems[i]->GetRect());
  }
  
  
  mIdealRect = total;
  
  mIdealRect.SetWidth(mIdealRect.GetWidth() + mMarginLeft + mMarginRight);
  mIdealRect.SetHeight(mIdealRect.GetHeight() + mMarginTop + mMarginBottom);
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

void nuiHTMLBox::GetItemsAt(std::vector<nuiHTMLItem*>& rHitItems, float X, float Y) const
{
  nuiHTMLItem::GetItemsAt(rHitItems, X, Y);
  
  // Try the children:
  X -= mRect.Left();
  Y -= mRect.Top();
  for (uint32 i = 0; i < mItems.size(); i++)
  {
    mItems[i]->GetItemsAt(rHitItems, X, Y);
  }
}

void nuiHTMLBox::UpdateVisibility(const nuiRect& rVisibleRect)
{
  nuiHTMLItem::UpdateVisibility(rVisibleRect);
  if (mVisible)
  {
    for (uint32 i = 0; i < mItems.size(); i++)
      mItems[i]->UpdateVisibility(rVisibleRect);
  }
}

void nuiHTMLBox::SetMargins(float val)
{
  mMarginLeft = mMarginTop = mMarginRight = mMarginBottom = val;
}

void nuiHTMLBox::SetMarginLeft(float val)
{
  mMarginLeft = val;
}

void nuiHTMLBox::SetMarginTop(float val)
{
  mMarginTop = val;
}

void nuiHTMLBox::SetMarginRight(float val)
{
  mMarginRight = val;
}

void nuiHTMLBox::SetMarginBottom(float val)
{
  mMarginBottom = val;
}

float nuiHTMLBox::GetMarginLeft() const
{
  return mMarginLeft;
}

float nuiHTMLBox::GetMarginTop() const
{
  return mMarginTop;
}

float nuiHTMLBox::GetMarginRight() const
{
  return mMarginRight;
}

float nuiHTMLBox::GetMarginBottom() const
{
  return mMarginBottom;
}

