/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLItem.h"

#pragma once

class nuiHTMLTable : public nuiHTMLItem
{
public:
  nuiHTMLTable(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, bool Inline);
  virtual ~nuiHTMLTable();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  virtual void SetLayout(const nuiRect& rRect);
  
  class Cell;

  void SetCellSpan(int32 col, int32 row, int32 ncols, int32 nrows);

  uint32 GetRowCount() const;
  uint32 GetColCount() const;
  nuiHTMLTable::Cell& SetCell(int32 col, int32 row, nuiHTMLNode* pNode, nuiHTMLItem* pItem);
  nuiHTMLTable::Cell& GetCell(int32 col, int32 row);
  const nuiHTMLTable::Cell& GetCell(int32 col, int32 row) const;
  
  void GetItemsAt(std::vector<nuiHTMLItem*>& rHitItems, float X, float Y) const;
  
  enum Frame
  {
    eVoid = 0,    // No sides. This is the default value.
    eAbove = 2,   // The top side only.
    eBelow = 8,   // The bottom side only.
    eHSides = 2 | 8,  // The top and bottom sides only.
    eVSides = 1 | 4,  // The right and left sides only.
    eLHS = 1,     // The left-hand side only.
    eRHS = 4,     // The right-hand side only.
    eBox = 1 | 2 | 4 | 8,     // All four sides.
    eBorder = eBox   // All four sides.
  };
  
  enum Rules
  {
    eNone,
    eGroups,
    eRows,
    eCols,
    eAll
  };
  
  class Cell
  {
  public:
    Cell();
    ~Cell();
    
    void SetContents(nuiHTMLNode* pNode, nuiHTMLItem* pItem);
    void SetLayout(const nuiRect& rRect);
    void Layout(nuiHTMLContext& rCtx);
    void Draw(nuiDrawContext* pContext);
    
    int32 mCol;
    int32 mRow;
    Cell* mpMasterCell;
    nuiHTMLNode* mpNode;
    uint32 mColSpan;
    uint32 mRowSpan;
    nuiHTMLItem* mpItem;
    float mIdealWidth;
    float mIdealHeight;
    float mRequestedWidth;
    float mRequestedHeight;
    float mX;
    float mY;
    float mWidth;
    float mHeight;
    nuiHTMLItem::Unit mRequestedWidthUnit;
    nuiHTMLItem::Unit mRequestedHeightUnit;
    nglString mAbbr;
    bool mHeader;
    Frame mFrame;
    int32 mBorder;
    float mSpacing;
    float mPadding;
    nuiColor mBgColor;
    bool mBgColorSet;
  };

  void Grow(int32 col, int32 row);


protected:
  std::vector<std::vector<Cell> > mCells;

  class Col
  {
  public:
    Col();
    
    float mRequestedSize;
    nuiHTMLItem::Unit mRequestedSizeUnit;
    float mMinSize;
    float mMaxSize;
    float mSize;
    float mSpacing;
    float mBorder;
  };

  class Row
  {
  public:
    Row();
    
    float mSize;
  };
  
  std::vector<Col> mColumns;
  std::vector<Row> mRows;

  void SetRowCount(uint32 count);
  void SetColCount(uint32 count);
  
  Cell mDefaultCell;
  Cell mMainCell;
};

