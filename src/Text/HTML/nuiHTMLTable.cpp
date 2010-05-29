/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLTable.h"

// class nuiHTMLTable : public nuiHTMLItem
nuiHTMLTable::nuiHTMLTable(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, bool Inline)
: nuiHTMLItem(pNode, pAnchor, Inline)
{
  SetRowCount(1);
  SetColCount(1);
  mDefaultCell.mRequestedWidth = 100;
  mDefaultCell.mRequestedHeight = -1;
  mDefaultCell.mRequestedWidthUnit = ePercentage;
  mDefaultCell.mRequestedHeightUnit = ePercentage;
}

nuiHTMLTable::~nuiHTMLTable()
{
  for (uint32 i = 0; i < GetRowCount(); i++)
  {
    for (uint32 j = 0; j < GetColCount(); j++)
    {
      nuiHTMLItem* pItem = GetCell(j, i).mpItem;
      delete pItem;
    }
  }
}

void nuiHTMLTable::Draw(nuiDrawContext* pContext)
{
  mDefaultCell.Draw(pContext);
  
  for (uint32 i = 0; i < GetRowCount(); i++)
  {
    for (uint32 j = 0; j < GetColCount(); j++)
    {
      GetCell(j, i).Draw(pContext);
    }
  }
}

void nuiHTMLTable::Layout(nuiHTMLContext& rContext)
{
  nuiHTMLContext ctx(rContext);
  float MaxWidth = ctx.mMaxWidth;
  switch (mDefaultCell.mRequestedWidthUnit)
  {
    case ePixels:
      MaxWidth = mDefaultCell.mRequestedWidth;
      break;
    case ePercentage:
      MaxWidth = (mDefaultCell.mRequestedWidth * MaxWidth) * 0.01f;
      break;
    case eProportional:
      MaxWidth = MaxWidth;
      break;
    default:
      NGL_ASSERT(0);
      break;
  } 
  
  MaxWidth -= (mDefaultCell.mSpacing + mDefaultCell.mPadding) * 2;
  
  // Calculate default ideal sizes:
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    for (int32 j = 0; j < GetColCount(); j++)
    {      
      Cell& rCell(GetCell(j, i));
      switch (mColumns[j].mRequestedSizeUnit)
      {
        case ePixels:
          ctx.mMaxWidth = mColumns[j].mRequestedSize;
          break;
        case ePercentage:
          ctx.mMaxWidth = mColumns[j].mRequestedSize * MaxWidth * 0.01f;
          break;
        case eProportional:
          ctx.mMaxWidth = MaxWidth;
          break;
        default:
          NGL_ASSERT(0);
          break;
      } 

      rCell.Layout(ctx);
      float w = rCell.mIdealWidth;
      if (rCell.mColSpan > 1)
        w /= rCell.mColSpan;
      mColumns[j].mIdealSize = MAX(mColumns[j].mIdealSize, w);
      mRows[j].mSize = MAX(mRows[j].mSize, rCell.mIdealHeight);
    }
  }

  float idealw = 0;
  float variableidealw = 0;
  float wratio = 0;
  for (int32 i = 0; i < GetColCount(); i++)
  {
    idealw += mColumns[i].mIdealSize;
    if (mColumns[i].mRequestedSizeUnit == eProportional)
    {
      float r = mColumns[i].mRequestedSize;
      float s =  mColumns[i].mIdealSize;
      wratio += s * r;
    }
  }

  float wdiff = (idealw - MaxWidth) / wratio;
  
  for (int32 i = 0; i < GetColCount(); i++)
  {
    switch (mColumns[i].mRequestedSizeUnit)
    {
      case ePixels:
      case ePercentage:
        mColumns[i].mSize = mColumns[i].mIdealSize;
        break;
      case eProportional:
        mColumns[i].mSize = mColumns[i].mIdealSize - (mColumns[i].mRequestedSize * mColumns[i].mIdealSize * wdiff);
        if (mColumns[i].mSize < 0)
          mColumns[i].mSize = 0;
        break;
      default:
        NGL_ASSERT(0);
        break;
    } 
  }

  for (int32 i = 0; i < GetRowCount(); i++)
  {
    float rowheight = 0;
    for (int32 j = 0; j < GetColCount(); j++)
    {
      ctx.mMaxWidth = mColumns[j].mSize;
      Cell& rCell(GetCell(j, i));
      rCell.Layout(ctx);
      if (ctx.mMaxWidth < rCell.mIdealWidth)
        mColumns[j].mSize = rCell.mIdealWidth;
      float h = rCell.mIdealHeight;
      rowheight = MAX(h, rowheight);
    }
    mRows[i].mSize = rowheight;
  }
  
  float x = 0;
  float y = mDefaultCell.mSpacing + mDefaultCell.mPadding;
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    float rowheight = mRows[i].mSize;
    x = mDefaultCell.mSpacing + mDefaultCell.mPadding;

    for (int32 j = 0; j < GetColCount(); j++)
    {
      float w = mColumns[j].mSize;
      Cell& rCell(GetCell(j, i));
      
      nuiRect r(x, y, w, rowheight);
      //NGL_OUT(_T("Cell[%d,%d] -> %ls\n"), j, i, r.GetValue().GetChars());
      rCell.SetLayout(r);
      x += w;
    }
    y += rowheight;
  }
  
  x += (mDefaultCell.mSpacing + mDefaultCell.mPadding);
  y += (mDefaultCell.mSpacing + mDefaultCell.mPadding);
  mIdealRect = nuiRect(x, y);
  mIdealRect.RoundToBiggest();
  mDefaultCell.mIdealWidth = mIdealRect.GetWidth();
  mDefaultCell.mIdealHeight = mIdealRect.GetHeight();
}

void nuiHTMLTable::SetLayout(const nuiRect& rRect)
{
  nuiHTMLItem::SetLayout(rRect);
  mDefaultCell.mWidth = mRect.GetWidth();
  mDefaultCell.mHeight = mRect.GetHeight();

}

void nuiHTMLTable::SetRowCount(uint32 count)
{
  int32 old = GetRowCount();
  if (count == old)
    return;
  mCells.resize(count);
  mRows.resize(count);

  int32 cols = GetColCount();
  for (; old < count; old++)
    mCells[old].resize(cols, mDefaultCell);
  
  InvalidateLayout();
}

void nuiHTMLTable::SetColCount(uint32 count)
{
  if (count == GetColCount())
    return;
  for (uint32 i = 0; i < mCells.size(); i++)
    mCells[i].resize(count, mDefaultCell);
  mColumns.resize(count);
  InvalidateLayout();
}

uint32 nuiHTMLTable::GetRowCount() const
{
  return mCells.size();
}

uint32 nuiHTMLTable::GetColCount() const
{
  return mColumns.size();
}

void nuiHTMLTable::Grow(int32 col, int32 row)
{
  bool changed = false;
  if (col + 1 > GetColCount())
    SetColCount(col + 1);
  if (row + 1 > GetRowCount())
    SetRowCount(row + 1);
}

void nuiHTMLTable::SetCellSpan(int32 col, int32 row, int32 ncols, int32 nrows)
{
  Grow(col, row);
  
  Cell& rCell(GetCell(col, row));
  rCell.mColSpan = ncols;
  rCell.mRowSpan = nrows;
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    for (int32 j = 0; j < GetColCount(); j++)
    {
      if (i && j)
      {
        Cell& rSlave(GetCell(col + j, row + i));
        rSlave = Cell();
        rSlave.mpMasterCell = &rCell;
      }
    }
  }
  
  InvalidateLayout();
}


nuiHTMLTable::Cell& nuiHTMLTable::SetCell(int32 col, int32 row, nuiHTMLNode* pNode, nuiHTMLItem* pItem)
{
  Grow(col, row);
  
  mCells[row][col].SetContents(pNode, pItem);
}

nuiHTMLTable::Cell& nuiHTMLTable::GetCell(int32 col, int32 row)
{
  return mCells[row][col];
}

const nuiHTMLTable::Cell& nuiHTMLTable::GetCell(int32 col, int32 row) const
{
  return mCells[row][col];
}

void nuiHTMLTable::GetItemsAt(std::vector<nuiHTMLItem*>& rHitItems, float X, float Y) const
{
  for (uint32 i = 0; i < GetRowCount(); i++)
  {
    for (uint32 j = 0; j < GetColCount(); j++)
    {
      nuiHTMLItem* pItem = GetCell(j, i).mpItem;
      if (pItem && pItem->IsInside(X, Y))
        rHitItems.push_back(pItem);
    }
  }
}

// class nuiHTMLTable::Cell
nuiHTMLTable::Cell::Cell()
: mpMasterCell(NULL),
  mColSpan(1), mRowSpan(1),
  mpItem(NULL),
  mIdealWidth(-1), mIdealHeight(-1),
  mRequestedWidth(-1), mRequestedHeight(-1),
  mWidth(0), mHeight(0),
  mRequestedWidthUnit(ePixels), mRequestedHeightUnit(ePixels),
  mHeader(false),
  mFrame(eBorder),
  mBorder(1),
  mSpacing(2),
  mPadding(0)
{
}

nuiHTMLTable::Cell::~Cell()
{
}
  
void nuiHTMLTable::Cell::SetContents(nuiHTMLNode* pNode, nuiHTMLItem* pItem)
{
  mpNode = pNode;
  delete mpItem;
  mpItem = pItem;
}

void nuiHTMLTable::Cell::SetLayout(const nuiRect& rRect)
{
  mX = rRect.Left() + mPadding + mSpacing;
  mY = rRect.Top() + mPadding + mSpacing;
  mWidth = rRect.GetWidth() - (mPadding + mSpacing) * 2;
  mHeight = rRect.GetHeight() - (mPadding + mSpacing) * 2;
  if (mpItem)
  {
    nuiRect r(mX, mY, mWidth, mHeight);
    mpItem->SetLayout(r);
  }
}

void nuiHTMLTable::Cell::Layout(nuiHTMLContext& rCtx)
{
  nuiHTMLContext ctx(rCtx);
  ctx.mMaxWidth -= (mSpacing + mPadding) * 2;
  if (!mpItem)
  {
    mIdealWidth = 0;
    mIdealHeight = 0;
    return;
  }
  
  mpItem->Layout(rCtx);
  nuiRect r(mpItem->GetIdealRect());
  r.RoundToBiggest();
  mIdealWidth = r.GetWidth() + (mSpacing + mPadding) * 2;
  mIdealHeight = r.GetHeight() + (mSpacing + mPadding) * 2;
}


void nuiHTMLTable::Cell::Draw(nuiDrawContext* pContext)
{
  if (mBorder > 0)
  {
    pContext->PushState();
    pContext->SetLineWidth(mBorder);

    if (mFrame & 1)
      pContext->DrawLine(mX, mY, mX, mY + mHeight);
    if (mFrame & 2)
      pContext->DrawLine(mX, mY, mX + mWidth, mY);
    if (mFrame & 4)
      pContext->DrawLine(mX + mWidth, mY, mX + mWidth, mY + mHeight);
    if (mFrame & 8)
      pContext->DrawLine(mX, mY + mHeight, mX + mWidth, mY + mHeight);
    
    pContext->PopState();
  }
  if (mpItem)
    mpItem->CallDraw(pContext);
  
}

// class Col
nuiHTMLTable::Col::Col()
{
  mRequestedSize = 1;
  mRequestedSizeUnit = eProportional;
  mSize = 0;
  mIdealSize = 0;
}


// class Row
nuiHTMLTable::Row::Row()
{
  mSize = 0;
}


