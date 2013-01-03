/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiGrid.h
\brief Grid Layout container
*/

#ifndef __nuiGrid_h__
#define __nuiGrid_h__

#include "nuiContainer.h"

/// This class is a simple placement grid that adjusts its columns and rows sizes according to contained nuiWidget objects
class NUI_API nuiGrid : public nuiSimpleContainer
{
public:
  nuiGrid(uint32 nbcolumns = 0, uint32 nbrows = 0);
  
  void InitAttributes();
  void Reset(uint32 nbcolumns, uint32 nbrows, bool clear=true);
  
  virtual ~nuiGrid();

  void DisplayGridBorder(bool enable, nuiSize gridBorderSize = 1.f); ///< Allow to display the grid border
  void DisplayWidgetBoundingRect(bool enable); ///< Allow to display widget's bounding rect
  void PlaceInvisibleWidgets(bool enable);
  
  bool SetCell(uint32 col, uint32 row, nuiWidget* pWidget, nuiPosition position = nuiNoPosition, bool ReplaceExisting = true);  ///< returns false if cell is not empty and ReplaceExisting == true. Otherwise, thrash any existing widget and replace it with the new one.
  bool SetCellToken(uint32 col, uint32 row, nuiTokenBase* pToken);

  void SetCellPosition(uint32 col, uint32 row, nuiPosition position); ///< Sets the position of the widget inside the cell
  bool FindCell(nuiWidgetPtr pWidget, uint& rCol, uint& rRow) const; ///< Returns true if \p pWidget was found in this container. If it was found, its position in the grid are set in rCol and rRow.

  void ClearCells();

  nuiWidget* GetCell(uint32 col, uint32 row) const;               ///< returns NULL if cell is empty
  nuiTokenBase* GetCellToken(uint32 col, uint32 row) const;

  // Rows and columns settings:
  void SetColumnSpacing(uint32 column, nuiSize hgap); ///< Sets the horizontal spacing to hgap around widgets in the given column.
  void SetRowSpacing(uint32 row, nuiSize vgap);       ///< Sets the vertical spacing to hgap  around widgets in the given column.
  void SetGridSize(uint32 columns, uint32 rows);        ///< Sets the grid size in number of columns and rows, you have use this before inserting any element in the grid (via SetCell).
  void SetColumnExpand(uint32 col, nuiExpandMode mode, nuiSize ratio = 1.f);///< Sets the expand mode and ratio to the given column. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other column ratios
  void SetRowExpand(uint32 row, nuiExpandMode mode, nuiSize ratio = 1.f);   ///< Sets the expand mode and ratio to the given row. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other row ratios
  nuiExpandMode GetColumnExpand(uint32 column) const;
  nuiExpandMode GetRowExpand(uint32 row) const;
  
  void SetRowMaxGrow(uint32 row, nuiSize ratio);        ///< Sets the maximum growing percentage ratio for the given row 
  void SetColumnMaxGrow(uint32 col, nuiSize ratio);     ///< Sets the maximum growing percentage ratio for the given column 
  void SetRowMaxPixels(uint32 row, nuiSize size);       ///< Sets the maximum size in pixels for the given row
  void SetColumnMaxPixels(uint32 column, nuiSize size); ///< Sets the maximum size in pixels for the given column
    
  void SetRowMaxShrink(uint32 row, nuiSize ratio);      ///< Sets the maximum shrinking percentage ratio for the given row 
  void SetColumnMaxShrink(uint32 col, nuiSize ratio);   ///< Sets the maximum shrinking percentage ratio for the given column 
  void SetRowMinPixels(uint32 row, nuiSize size);       ///< Sets the minimum size in pixels for the given row
  void SetColumnMinPixels(uint32 column, nuiSize size); ///< Sets the minimum size in pixels for the given column
  
  void SetColumnPixels(uint32 column, nuiSize size); ///< Fix a collumn's number of pixels
  nuiSize GetColumnPixels(uint32 row) const; ///< Get a column's number of pixels
  void SetRowPixels(uint32 row, nuiSize size); ///< Fix a row's number of pixels
  nuiSize GetRowPixels(uint32 row) const; ///< Get a row's number of pixels
  void ResetRowPixels(uint32 row);                ///< make the minimum size in pixels back to its variable capactiy
  void ResetColumnPixels(uint32 col);                ///< make the minimum size in pixels back to its variable capactiy
  
  // Corresponding Getters:
  nuiSize GetColumnSpacing(uint32 column) const; ///< Gets the horizontal spacing to hgap around widgets in the given column.
  nuiSize GetRowSpacing(uint32 row) const;       ///< Gets the vertical spacing to hgap  around widgets in the given column.
  void GetGridSize(uint& columns, uint& rows) const;        ///< Gets the grid size in number of columns and rows.
  nuiSize GetColumnGrowRatio(uint32 col) const;///< Gets the ratio to the given column.
  nuiSize GetColumnShrinkRatio(uint32 col) const;///< Gets the expand ratio of the given column.
  nuiSize GetRowGrowRatio(uint32 col) const;///< Gets the expand mode and ratio of the given Row.
  nuiSize GetRowShrinkRatio(uint32 col) const;///< Gets the expand mode and ratio of the given Row.

  nuiSize GetRowMaxGrow(uint32 row) const;        ///< Gets the maximum growing percentage ratio for the given row 
  nuiSize GetColumnMaxGrow(uint32 col) const;     ///< Gets the maximum growing percentage ratio for the given column 
  nuiSize GetRowMaxPixels(uint32 row) const;       ///< Gets the maximum size in pixels for the given row
  nuiSize GetColumnMaxPixels(uint32 column) const; ///< Gets the maximum size in pixels for the given column

  nuiSize GetRowMaxShrink(uint32 row) const;      ///< Gets the maximum shrinking percentage ratio for the given row 
  nuiSize GetColumnMaxShrink(uint32 col) const;   ///< Gets the maximum shrinking percentage ratio for the given column 
  nuiSize GetRowMinPixels(uint32 row) const;       ///< Gets the minimum size in pixels for the given row
  nuiSize GetColumnMinPixels(uint32 column) const; ///< Gets the minimum size in pixels for the given column
  
  // cells visibility
  void SetCellVisible(uint32 col, uint32 row, bool set);
  bool IsCellVisible(uint32 col, uint32 row) const;
  void SetRowVisible(uint32 row, bool set);
  bool IsRowVisible(uint32 row) const;
  void SetColumnVisible(uint32 col, bool set);
  bool IsColumnVisible(uint32 col) const;


  // Add rows/columns:
  void AddRows(uint32 pos, uint32 rows);
  void AddColumns(uint32 pos, uint32 columns);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  virtual bool Draw(nuiDrawContext* pContext);
  
  uint32 GetNbRows() const    { return mNbRows;     }
  uint32 GetNbColumns() const { return mNbColumns;  }

  void RemoveRows(uint32 pos, uint32 rows);
  void RemoveColumns(uint32 pos, uint32 columns);

  virtual bool Clear();

  nuiSize GetDefaultHSpacing() const;
  nuiSize GetDefaultVSpacing() const;
  void SetDefaultHSpacing(nuiSize set);
  void SetDefaultVSpacing(nuiSize set);
  nuiSize GetDefaultSpacing() const;
  void SetDefaultSpacing(nuiSize set);

  void SetEqualizeColumns(bool set);
  void SetEqualizeRows(bool set);
  bool GetEqualizeColumns() const;
  bool GetEqualizeRows() const;
  
protected:

  uint32 GetDimensionRange(uint32 dimension) const;
  uint32 GetColumnDimensionRange(uint32 dimension) const;
  uint32 GetRowDimensionRange(uint32 dimension) const;
  
  void    UpdateExpandRatio(std::vector<nuiSize>& rRatios, nuiSize& rCoeff);
  void    AdjustToExpand(nuiSize width, nuiSize height, 
                        std::vector<nuiSize>& Widths, std::vector<nuiSize>& Heights);

  bool mDisplayGridBorder;
  bool mDisplayWidgetBoundingRect;
  bool mPlaceInvisibleWidgets;

  nuiRect mSizeRect;

  nuiSize mGridBorderSize;

  uint32    mNbColumns;
  uint32    mNbRows;
  
  nuiSize mExpandGrowColumnsCoeff;
  nuiSize mExpandShrinkColumnsCoeff;
  nuiSize mExpandGrowRowsCoeff;
  nuiSize mExpandShrinkRowsCoeff;
  
  std::vector<std::vector<nuiWidget* > > mGrid;
  
  

  std::vector<nuiSize> mWidths;
  std::vector<nuiSize> mHeights;
  
  std::vector<bool> mRowVisible;
  std::vector<bool> mColumnVisible;

  std::vector<nuiSize> mColumnWidths;
  std::vector<nuiSize> mRowHeights;
  std::vector<nuiSize> mVGaps;
  std::vector<nuiSize> mHGaps;

  std::vector<nuiSize> mExpandShrinkColumns;
  std::vector<nuiSize> mExpandShrinkRows;
  std::vector<nuiSize> mExpandGrowColumns;
  std::vector<nuiSize> mExpandGrowRows;

  std::vector<nuiSize> mMaxGrowRows;
  std::vector<nuiSize> mMaxGrowColumns;
  std::vector<nuiSize> mMaxShrinkRows;
  std::vector<nuiSize> mMaxShrinkColumns;

  std::vector<nuiSize> mMaximumRowSizes;
  std::vector<nuiSize> mMaximumColumnSizes;
  std::vector<nuiSize> mMinimumRowSizes;
  std::vector<nuiSize> mMinimumColumnSizes;



  nuiSize mDefaultHSpacing;
  nuiSize mDefaultVSpacing;

  bool mEqualizeColumns;
  bool mEqualizeRows;
};

#endif//__nuiGrid_h__


