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
  mRequestedWidth = 100;
  mRequestedWidthUnit = ePercentage;
}

nuiHTMLTable::~nuiHTMLTable()
{
}

void nuiHTMLTable::Draw(nuiDrawContext* pContext)
{
  for (uint32 i = 0; i < GetRowCount(); i++)
  {
    for (uint32 j = 0; j < GetColCount(); j++)
    {
      nuiHTMLItem* pItem = GetCell(j, i).mpItem;
      if (pItem)
        pItem->Draw(pContext);
    }
  }
}

void nuiHTMLTable::Layout(nuiHTMLContext& rContext)
{
  nuiHTMLContext ctx(rContext);
  
  std::vector<float> Rows;
  Rows.resize(GetRowCount());
  
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    for (int32 j = 0; j < GetColCount(); j++)
    {
      nuiHTMLItem* pItem = GetCell(j, i).mpItem;
      if (pItem)
      {
        pItem->Layout(ctx);
        nuiRect r(pItem->GetIdealRect());
        mColumns[j].mIdealSize = MAX(mColumns[j].mIdealSize, r.GetWidth());
        Rows[j] = MAX(Rows[j], r.GetHeight());
      }
    }
  }

  float w = 0;
  for (int32 i = 0; i < GetRowCount(); i++)
    w += mColumns[i].mIdealSize;

  float h = 0;
  for (int32 i = 0; i < GetColCount(); i++)
    h += Rows[i];

  float wr = 0;
  
  for (int32 i = 0; i < GetRowCount(); i++)
  {
    for (int32 j = 0; j < GetColCount(); j++)
    {
      nuiHTMLItem* pItem = GetCell(j, i).mpItem;
      if (pItem)
      {
        nuiRect r(pItem->GetIdealRect());
        mColumns[j].mIdealSize = MAX(mColumns[j].mIdealSize, r.GetWidth());
        Rows[j] = MAX(Rows[j], r.GetHeight());
        pItem->SetLayout(r);
      }
    }
  }
}

void nuiHTMLTable::SetRowCount(uint32 count)
{
  if (count == GetRowCount())
    return;
  mCells.resize(count);
  InvalidateLayout();
}

void nuiHTMLTable::SetColCount(uint32 count)
{
  if (count == GetColCount())
    return;
  for (uint32 i = 0; i < mCells.size(); i++)
    mCells[i].resize(count);
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
  if (col + 1 < GetColCount())
    SetColCount(col + 1);
  if (row + 1 < GetRowCount())
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
  mWidthUnit(ePixels), mHeightUnit(ePixels),
  mHeader(false),
  mFrame(eVoid),
  mBorder(0),
  mSpacing(0),
  mPadding(0)
{
}

nuiHTMLTable::Cell::~Cell()
{
  delete mpItem;
}
  
void nuiHTMLTable::Cell::SetContents(nuiHTMLNode* pNode, nuiHTMLItem* pItem)
{
  mpNode = pNode;
  delete mpItem;
  mpItem = pItem;
}


// class Col
nuiHTMLTable::Col::Col()
{
  mRequestedSize = 0;
  mSizeUnit = eProportional;
  mSize = 1;
  mIdealSize = 0;
}
  

