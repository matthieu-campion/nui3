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
  mDefaultCell.SetContents(pNode, NULL);
  mMainCell.SetContents(pNode, NULL);

  mMainCell.mRequestedWidth = -1;
  mMainCell.mRequestedHeight = 0;
  mMainCell.mRequestedWidthUnit = eProportional;
  mMainCell.mRequestedHeightUnit = eProportional;

//  mMainCell.mRequestedWidth = -1;
//  mMainCell.mRequestedHeight = -1;
//  mMainCell.mRequestedWidthUnit = ePercentage;
//  mMainCell.mRequestedHeightUnit = ePercentage;

  mDefaultCell.mRequestedWidth = -1;
  mDefaultCell.mRequestedHeight = -1;
  mDefaultCell.mRequestedWidthUnit = ePercentage;
  mDefaultCell.mRequestedHeightUnit = ePercentage;
  
  SetRowCount(1);
  SetColCount(1);
  
  uint32 count = pNode->GetNbChildren();
  int32 cols = 0;
  int32 rowcount = GetRowCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    if (pChild->GetTagType() == nuiHTMLNode::eTag_COL)
    {
      Grow(cols, rowcount);
      
      nuiHTMLAttrib* pAttrib = NULL;
      pAttrib = pChild->GetAttribute(nuiHTMLAttrib::eAttrib_WIDTH);
      if (pAttrib)
      {
        const nglString &v(pAttrib->GetValue());
        mColumns[cols].mRequestedSizeUnit = ePixels;
        mColumns[cols].mRequestedSize = v.GetCFloat();
        
        if (v.Contains(_T("%")))
          mColumns[cols].mRequestedSizeUnit = ePercentage;
        else if (v.Contains(_T("*")))
          mColumns[cols].mRequestedSizeUnit = eProportional;
      }

      cols++;
      
    }
  }   
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
  if (!mVisible)
    return;

  mMainCell.Draw(pContext);
  
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
  if (mMainCell.mRequestedWidth >= 0)
  {
    switch (mMainCell.mRequestedWidthUnit)
    {
      case ePixels:
        MaxWidth = mMainCell.mRequestedWidth;
        break;
      case ePercentage:
        MaxWidth = (mMainCell.mRequestedWidth * MaxWidth) * 0.01f;
        break;
      case eProportional:
        MaxWidth = MaxWidth;
        break;
      default:
        NGL_ASSERT(0);
        break;
    } 
  }
  
  MaxWidth -= (mMainCell.mSpacing + mMainCell.mPadding + mMainCell.mBorder) * 2;
  
  for (int32 i = 0; i < GetColCount(); i++)
  {
    mColumns[i].mBorder = 0;
    mColumns[i].mSpacing = 0;
  }
  
  // Calculate column min and max sizes:
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    for (int32 j = 0; j < GetColCount(); j++)
    {      
      Cell& rCell(GetCell(j, i));
      ctx.mMaxWidth = 0;
      rCell.Layout(ctx);
      float w = rCell.mIdealWidth;
      mColumns[j].mMinSize = MAX(mColumns[j].mMinSize, w);

      ctx.mMaxWidth = INT_MAX;
      rCell.Layout(ctx);
      w = rCell.mIdealWidth / rCell.mColSpan;
      if (rCell.HasMaster())
      {
        Cell& rMasterCell(GetCell(rCell.mMasterCol, rCell.mMasterRow));
        w = rMasterCell.mIdealWidth / rMasterCell.mColSpan;
      }
      
      mColumns[j].mMaxSize = MAX(mColumns[j].mMaxSize, w);

      mColumns[j].mBorder = rCell.mBorder;
      mColumns[j].mSpacing = rCell.mSpacing;
    }
  }

//  for (int32 i = 0; i < GetColCount(); i++)
//  {
//    mColumns[i].mMinSize += (mColumns[i].mBorder + mColumns[i].mSpacing) * 2;
//    mColumns[i].mMaxSize += (mColumns[i].mBorder + mColumns[i].mSpacing) * 2;
//  }
  
  
  float idealw = 0;
  float variableidealw = 0;
  float minvariableidealw = 0;
  float wratio = 0;

  for (int32 i = 0; i < GetColCount(); i++)
  {
    switch (mColumns[i].mRequestedSizeUnit)
    {
      case ePixels:
        mColumns[i].mSize = MAX(mColumns[i].mSize, mColumns[i].mRequestedSize);
        break;
      case ePercentage:
        mColumns[i].mSize = MAX(mColumns[i].mSize, MaxWidth * 0.01f * mColumns[i].mRequestedSize);
        break;
      case eProportional:
        mColumns[i].mSize = MAX(mColumns[i].mSize, mColumns[i].mMaxSize);
        break;
      default:
        NGL_ASSERT(0);
        break;
    } 

    mColumns[i].mSize = MAX(mColumns[i].mSize, mColumns[i].mMinSize);

  
    idealw += mColumns[i].mSize;
    if (mColumns[i].mRequestedSizeUnit == eProportional && mColumns[i].mRequestedSize >= 0)
    {
      float r = mColumns[i].mRequestedSize;
      float s = mColumns[i].mSize;
      variableidealw += s;
      minvariableidealw += mColumns[i].mMinSize;
      wratio += s * r;
    }
  }
  
  if (idealw > MaxWidth)
  {
    // We need to reduce some column sizes
    float wmax = MaxWidth;
    float fixed = idealw - variableidealw;
    float wdiff = MaxWidth - fixed;
    //wdiff = MAX(wdiff, minvariableidealw);
    wdiff /= wratio;
    for (int32 i = 0; i < GetColCount(); i++)
    {
      switch (mColumns[i].mRequestedSizeUnit)
      {
        case ePixels:
        case ePercentage:
          // We can't do anything about these
          break;
        case eProportional:
          {
            float s = mColumns[i].mRequestedSize * wdiff;
            s *= mColumns[i].mMaxSize;
            s = MAX(mColumns[i].mMinSize, s);
            mColumns[i].mSize = s;
            
          }
          break;
        default:
          NGL_ASSERT(0);
          break;
      } 
    }
  }

  float x = 0;
  float y = mMainCell.mSpacing + mMainCell.mBorder;
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    float rowheight = 0;
    x = mMainCell.mSpacing + mMainCell.mBorder;

    for (int32 j = 0; j < GetColCount(); j++)
    {
      float w = mColumns[j].mSize;
      Cell& rCell(GetCell(j, i));
      
      ctx.mMaxWidth = w - (mColumns[j].mBorder + mColumns[j].mSpacing) * 1;
      rCell.Layout(ctx);
      float h = rCell.mIdealHeight / rCell.mRowSpan;
      if (rCell.HasMaster())
      {
        Cell& rMasterCell(GetCell(rCell.mMasterCol, rCell.mMasterRow));
        h = rMasterCell.mIdealHeight / rMasterCell.mRowSpan;
      }
    
      rowheight = MAX(h, rowheight);
    }

    for (int32 j = 0; j < GetColCount(); j++)
    {
      float w = mColumns[j].mSize;
      Cell& rCell(GetCell(j, i));
      nuiRect r(x, y, w, rowheight);

      rCell.SetLayout(r);
      if (rCell.HasMaster())
      {
        Cell& rMasterCell(GetCell(rCell.mMasterCol, rCell.mMasterRow));
        float s = rMasterCell.mSpacing + rMasterCell.mBorder;
        float x = rMasterCell.mX - s;
        float y = rMasterCell.mY - s;
        nuiRect rect(x, y, r.Right(), r.Bottom(), false);
        ctx.mMaxWidth = rect.GetWidth();
        rMasterCell.Layout(ctx);
        rMasterCell.SetLayout(rect);
        
      }
      x += w;
    }
    
    y += rowheight;
  }
  
  x += (mMainCell.mSpacing + mMainCell.mBorder);
  y += (mMainCell.mSpacing + mMainCell.mBorder);
  mIdealRect = nuiRect(x, y);
  mIdealRect.RoundToBiggest();
  mMainCell.mIdealWidth = mIdealRect.GetWidth();
  mMainCell.mIdealHeight = mIdealRect.GetHeight();
}

void nuiHTMLTable::SetLayout(const nuiRect& rRect)
{
  nuiHTMLItem::SetLayout(rRect);
  mMainCell.mWidth = mRect.GetWidth();
  mMainCell.mHeight = mRect.GetHeight();

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
  ncols = MAX(ncols, 1);
  nrows = MAX(nrows, 1);
  Grow(col + ncols - 1, row + nrows - 1);
  
  Cell& rCell(GetCell(col, row));
  rCell.mColSpan = ncols;
  rCell.mRowSpan = nrows;
  for (int32 i = 0; i < nrows; i++)
  {
    for (int32 j = 0; j < ncols; j++)
    {
      if (i || j)
      {
        Cell& rSlave(GetCell(col + j, row + i));
        rSlave = Cell();
        rSlave.mMasterCol = col;
        rSlave.mMasterRow = row;
      }
    }
  }
  
  InvalidateLayout();
}


nuiHTMLTable::Cell& nuiHTMLTable::SetCell(int32 col, int32 row, nuiHTMLNode* pNode, nuiHTMLItem* pItem)
{
  //printf("inserting %d,%d\n", col, row);
  Grow(col, row);
    
  int32 i = -1;
  int32 realcol = -1;

  bool IsMaster;
  bool HasMaster;
  bool skip = false;
  
  do
  {
    Grow(realcol + 1, row);
    const Cell& rCell = GetCell(realcol + 1, row);

    int32 span = rCell.mColSpan;
    if (rCell.HasMaster())
    {
      const Cell& rMaster(GetCell(rCell.mMasterCol, rCell.mMasterRow));
      span = rMaster.mColSpan;
      if (span == 1)
        i--;
      //printf("\tslave  %d (%d) span = %d\n", realcol, i, span);
    }

    IsMaster = span > 1;

    if (IsMaster)
    {
      //printf("\tmaster %d (%d)\n", realcol, i);
      realcol += rCell.mColSpan;
      skip = rCell.mColSpan > 1;    
      if (!skip)
        i++;
    }
    else
    {
      //printf("\tnormal %d (%d)\n", realcol, i);
      if (skip)
        i++;
      skip = false;
      realcol++;
    }

    if (!skip)
      i++;
  } while (i != col);
    
  //printf("\tfound %d (%d)\n\n", realcol, i);
  col = realcol;
  
  
  Cell& rCell(mCells[row][col]);
  rCell.mCol = col;
  rCell.mRow = row;
  rCell.SetContents(pNode, pItem);

  nuiHTMLAttrib* pAttrib = NULL;
  
  int32 RowSpan = 1;
  int32 ColSpan = 1;
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_COLSPAN);
  if (pAttrib)
    ColSpan = pAttrib->GetValue().GetCInt();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_ROWSPAN);
  if (pAttrib)
    RowSpan = pAttrib->GetValue().GetCInt();
  
  if (RowSpan > 1 || ColSpan > 1)
    SetCellSpan(col, row, ColSpan, RowSpan);
  
  return rCell;
}

nuiHTMLTable::Cell& nuiHTMLTable::GetCell(int32 col, int32 row)
{
  NGL_ASSERT(col < mColumns.size());
  NGL_ASSERT(row < mRows.size());
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
: mCol(-1), mRow(-1),
  mColSpan(1), mRowSpan(1),
  mMasterCol(-1), mMasterRow(-1),
  mpItem(NULL),
  mIdealWidth(-1), mIdealHeight(-1),
  mRequestedWidth(-1), mRequestedHeight(-1),
  mWidth(0), mHeight(0),
  mRequestedWidthUnit(ePixels), mRequestedHeightUnit(ePixels),
  mHeader(false),
  mFrame(eBorder),
  mBorder(1),
  mSpacing(1),
  mPadding(2),
  mBgColorSet(false)
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
  
  nuiHTMLAttrib* pAttrib = NULL;
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_BORDER);
  if (pAttrib)
    mBorder = pAttrib->GetValue().GetCFloat();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_CELLPADDING);
  if (pAttrib)
    mPadding = pAttrib->GetValue().GetCFloat();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_CELLSPACING);
  if (pAttrib)
    mSpacing = pAttrib->GetValue().GetCFloat();
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_BGCOLOR);
  if (pAttrib)
  {
    mBgColor.SetValue(pAttrib->GetValue());
    mBgColorSet = true;
  }

  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_WIDTH);
  if (pAttrib)
  {  
    const nglString &v(pAttrib->GetValue());
    mRequestedWidth = v.GetCFloat();
    
    if (v.Contains(_T("%")))
      mRequestedWidthUnit = ePercentage;
    else if (v.Contains(_T("*")))
      mRequestedWidthUnit = eProportional;
    else if (v.Contains(_T("px"), false))
      mRequestedWidthUnit = ePixels;
  }
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HEIGHT);
  if (pAttrib)
  {  
    const nglString &v(pAttrib->GetValue());
    mRequestedHeight = v.GetCFloat();
    
    if (v.Contains(_T("%")))
      mRequestedHeightUnit = ePercentage;
    else if (v.Contains(_T("*")))
      mRequestedHeightUnit = eProportional;
    else if (v.Contains(_T("px"), false))
      mRequestedHeightUnit = ePercentage;
  }
  
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_FRAME);
  if (pAttrib)
  {
    nglString v(pAttrib->GetValue());
    v.Trim();
    if (v.Compare(_T("void"), false) == 0)
      mFrame = eVoid;
    else if (v.Compare(_T("above"), false) == 0)
      mFrame = eAbove;
    else if (v.Compare(_T("below"), false) == 0)
      mFrame = eBelow;
    else if (v.Compare(_T("hsides"), false) == 0)
      mFrame = eHSides;
    else if (v.Compare(_T("vsides"), false) == 0)
      mFrame = eVSides;
    else if (v.Compare(_T("lhs"), false) == 0)
      mFrame = eLHS;
    else if (v.Compare(_T("rhs"), false) == 0)
      mFrame = eRHS;
    else if (v.Compare(_T("box"), false) == 0)
      mFrame = eBox;
    else if (v.Compare(_T("border"), false) == 0)
      mFrame = eBorder;
  }

  /*
  pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_RULES);
  if (pAttrib)
  {
    nglString v(pAttrib->GetValue());
    v.Trim();
    if (v.Compare(_T("none"), false) == 0)
      mRules = eNone;
    else if (v.Compare(_T("groups"), false) == 0)
      mRules = eGroups;
    else if (v.Compare(_T("rows"), false) == 0)
      mRules = eRows;
    else if (v.Compare(_T("cols"), false) == 0)
      mRules = eCols;
    else if (v.Compare(_T("all"), false) == 0)
      mRules = eAll;
  }
  */
  
}

bool nuiHTMLTable::Cell::HasMaster() const
{
  return mMasterCol >= 0 || mMasterRow >= 0;
}

bool nuiHTMLTable::Cell::IsMaster() const
{
  return mColSpan > 1 || mRowSpan > 1;
}

void nuiHTMLTable::Cell::SetLayout(const nuiRect& rRect)
{
  mX = rRect.Left() + mSpacing + mBorder;
  mY = rRect.Top() + mSpacing + mBorder;
  mWidth = rRect.GetWidth() - (mSpacing + mBorder) * 2;
  mHeight = rRect.GetHeight() - (mSpacing + mBorder) * 2;
  if (!HasMaster() && mpItem)
  {
    nuiRect r(mX + mPadding, mY + mPadding, mWidth - mPadding * 2, mHeight - mPadding * 2);
    mpItem->SetLayout(r);
  }
}

void nuiHTMLTable::Cell::Layout(nuiHTMLContext& rCtx)
{
  nuiHTMLContext ctx(rCtx);
  if (!mpItem)
  {
    mIdealWidth = 0;
    mIdealHeight = 0;
  }

  float rw = 0;
  if (mRequestedWidth >= 0)
  {
    switch (mRequestedWidthUnit)
    {
      case ePixels:
        rw = mRequestedWidth;
        break;
      case ePercentage:
        rw = mRequestedWidth * rCtx.mMaxWidth * 0.01f;
        break;
      case eProportional:
        rw = 0;
        break;
      default:
        NGL_ASSERT(0);
        break;
    }
    
    if (rw > ctx.mMaxWidth)
      ctx.mMaxWidth = rw;
  }

  ctx.mMaxWidth -= (mSpacing + mPadding) * 2;
  
  nuiRect r;
  if (mpItem)
  {
    mpItem->Layout(rCtx);
    r = mpItem->GetIdealRect();
  }
  r.RoundToBiggest();
  mIdealWidth = r.GetWidth() + (mSpacing + mPadding + mBorder) * 2;
  mIdealHeight = r.GetHeight() + (mSpacing + mPadding + mBorder) * 2;
  
  if (mRequestedWidth >= 0)
  {
    if (rw > mIdealWidth)
      mIdealWidth = rw;
  }
  
  if (mRequestedHeight >= 0)
  {
    float rr = 0;
    switch (mRequestedHeightUnit)
    {
      case ePixels:
        rr = mRequestedHeight;
        break;
      case ePercentage:
        rr = 0;
        break;
      case eProportional:
        rr = 0;
        break;
      default:
        NGL_ASSERT(0);
        break;
    }
    
    if (rr > mIdealHeight)
      mIdealHeight = rr;
  }
}


void nuiHTMLTable::Cell::Draw(nuiDrawContext* pContext)
{
  if (HasMaster())
    return;
  
  if (mBgColorSet)
  {
    nuiRect r(mX, mY, mWidth, mHeight);
    
    pContext->SetFillColor(mBgColor);
    pContext->DrawRect(r, eFillShape);
    pContext->SetFillColor(nuiColor());
  }
  
  if (mBorder > 0)
  {
    int32 b = mBorder / 2;
    pContext->SetLineWidth(mBorder);

    if (mFrame & 1)
      pContext->DrawLine(mX - b, mY - b, mX - b, mY + mHeight + b);
    if (mFrame & 2)
      pContext->DrawLine(mX - b, mY - b, mX + mWidth + b, mY - b);
    if (mFrame & 4)
      pContext->DrawLine(mX + mWidth + b, mY - b, mX + mWidth + b, mY + mHeight + b);
    if (mFrame & 8)
      pContext->DrawLine(mX - b, mY + mHeight + b, mX + mWidth + b, mY + mHeight + b);
    
    pContext->SetLineWidth(1);
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
  mMinSize = 0;
  mMaxSize = 0;
}


// class Row
nuiHTMLTable::Row::Row()
{
  mSize = 0;
}


