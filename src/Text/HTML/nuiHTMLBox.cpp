/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLBox.h"

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
  float lastlineh = 0;
  
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
      
      bool linebreak = pItem->IsLineBreak();
      if (linebreak)
        printf("\nlinebreak\n\n");
      
      // Layout the line if needed:
      if ((X + r.GetWidth() > context.mMaxWidth) || linebreak)
      {
        float w = LayoutLine(line_start, line_end, Y, lineh, context);
        
        W = MAX(W, w);
        lastlineh = lineh;
        lineh = 0;
        X = 0;
      }

      if (linebreak && lineh == 0)
        Y += lastlineh;
      
      {
        lineh = MAX(lineh, r.GetHeight());
        X += r.GetWidth() + context.mVSpace;
        
        line_end = i;
      }
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

