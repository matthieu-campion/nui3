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
#include <vector>
#include <map>

enum nuiExpandMode
{
  nuiExpandFixed,
  nuiExpandGrow,
  nuiExpandShrink,
  nuiExpandShrinkAndGrow
};

/// This class is a simple placement grid that adjusts its columns and rows sizes according to contained nuiWidget objects
class NUI_API nuiGrid : public nuiSimpleContainer
{
public:
  nuiGrid(uint nbcolumns = 0, uint nbrows = 0);
  
  void InitAttributes();
  void Reset(uint nbcolumns, uint nbrows, bool clear=true);
  
  bool Load(const nuiXMLNode* pNode);
  virtual ~nuiGrid();

  void DisplayGridBorder(bool enable, nuiSize gridBorderSize = 1.f); ///< Allow to display the grid border
  void DisplayWidgetBoundingRect(bool enable); ///< Allow to display widget's bounding rect
  void PlaceInvisibleWidgets(bool enable);
  
  bool SetCell(uint col, uint row, nuiWidget* pWidget, nuiPosition position = nuiNoPosition, bool ReplaceExisting = true, bool TrashExisting=true);  ///< returns false if cell is not empty and ReplaceExisting == true. Otherwise, thrash any existing widget and replace it with the new one.
  bool SetCellToken(uint col, uint row, nuiTokenBase* pToken);

  void SetCellPosition(uint col, uint row, nuiPosition position); ///< Sets the position of the widget inside the cell
  bool FindCell(nuiWidgetPtr pWidget, uint& rCol, uint& rRow) const; ///< Returns true if \p pWidget was found in this container. If it was found, its position in the grid are set in rCol and rRow.

  void ClearCells(bool trash=true);

  nuiWidget* GetCell(uint col, uint row) const;               ///< returns NULL if cell is empty
  nuiTokenBase* GetCellToken(uint col, uint row) const;

  // Rows and columns settings:
  void SetColumnSpacing(uint column, nuiSize hgap); ///< Sets the horizontal spacing to hgap around widgets in the given column.
  void SetRowSpacing(uint row, nuiSize vgap);       ///< Sets the vertical spacing to hgap  around widgets in the given column.
  void SetGridSize(uint columns, uint rows);        ///< Sets the grid size in number of columns and rows, you have use this before inserting any element in the grid (via SetCell).
  void SetColumnExpand(uint col, nuiExpandMode mode, nuiSize ratio = 1.f);///< Sets the expand mode and ratio to the given column. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other column ratios
  void SetRowExpand(uint row, nuiExpandMode mode, nuiSize ratio = 1.f);   ///< Sets the expand mode and ratio to the given row. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other row ratios
  
  void SetRowMaxGrow(uint row, nuiSize ratio);        ///< Sets the maximum growing percentage ratio for the given row 
  void SetColumnMaxGrow(uint col, nuiSize ratio);     ///< Sets the maximum growing percentage ratio for the given column 
  void SetRowMaxPixels(uint row, nuiSize size);       ///< Sets the maximum size in pixels for the given row
  void SetColumnMaxPixels(uint column, nuiSize size); ///< Sets the maximum size in pixels for the given column
    
  void SetRowMaxShrink(uint row, nuiSize ratio);      ///< Sets the maximum shrinking percentage ratio for the given row 
  void SetColumnMaxShrink(uint col, nuiSize ratio);   ///< Sets the maximum shrinking percentage ratio for the given column 
  void SetRowMinPixels(uint row, nuiSize size);       ///< Sets the minimum size in pixels for the given row
  void SetColumnMinPixels(uint column, nuiSize size); ///< Sets the minimum size in pixels for the given column
  
  void SetColumnPixels(uint column, nuiSize size); ///< Fix a collumn's number of pixels
  void SetRowPixels(uint row, nuiSize size); ///< Fix a row's number of pixels
  void ResetRowPixels(uint row);                ///< make the minimum size in pixels back to its variable capactiy
  void ResetColumnPixels(uint col);                ///< make the minimum size in pixels back to its variable capactiy
  
  // Corresponding Getters:
  nuiSize GetColumnSpacing(uint column) const; ///< Gets the horizontal spacing to hgap around widgets in the given column.
  nuiSize GetRowSpacing(uint row) const;       ///< Gets the vertical spacing to hgap  around widgets in the given column.
  void GetGridSize(uint& columns, uint& rows) const;        ///< Gets the grid size in number of columns and rows.
  nuiSize GetColumnGrowRatio(uint col) const;///< Gets the ratio to the given column.
  nuiSize GetColumnShrinkRatio(uint col) const;///< Gets the expand ratio of the given column.
  nuiSize GetRowGrowRatio(uint col) const;///< Gets the expand mode and ratio of the given Row.
  nuiSize GetRowShrinkRatio(uint col) const;///< Gets the expand mode and ratio of the given Row.

  nuiSize GetRowMaxGrow(uint row) const;        ///< Gets the maximum growing percentage ratio for the given row 
  nuiSize GetColumnMaxGrow(uint col) const;     ///< Gets the maximum growing percentage ratio for the given column 
  nuiSize GetRowMaxPixels(uint row) const;       ///< Gets the maximum size in pixels for the given row
  nuiSize GetColumnMaxPixels(uint column) const; ///< Gets the maximum size in pixels for the given column

  nuiSize GetRowMaxShrink(uint row) const;      ///< Gets the maximum shrinking percentage ratio for the given row 
  nuiSize GetColumnMaxShrink(uint col) const;   ///< Gets the maximum shrinking percentage ratio for the given column 
  nuiSize GetRowMinPixels(uint row) const;       ///< Gets the minimum size in pixels for the given row
  nuiSize GetColumnMinPixels(uint column) const; ///< Gets the minimum size in pixels for the given column
  
  // cells visibility
  void SetCellVisible(uint col, uint row, bool set);
  bool IsCellVisible(uint col, uint row) const;
  void SetRowVisible(uint row, bool set);
  bool IsRowVisible(uint row) const;


  // Add rows/columns:
  void AddRows(uint pos, uint rows);
  void AddColumns(uint pos, uint columns);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  virtual bool Draw(nuiDrawContext* pContext);
  
  uint GetNbRows() const    { return mNbRows;     }
  uint GetNbColumns() const { return mNbColumns;  }

  void RemoveRows(uint pos, uint rows);
  void RemoveColumns(uint pos, uint columns);

  virtual bool Clear(bool Delete = true);

  nuiSize GetDefaultHSpacing() const;
  nuiSize GetDefaultVSpacing() const;
  void SetDefaultHSpacing(nuiSize set);
  void SetDefaultVSpacing(nuiSize set);
  nuiSize GetDefaultSpacing() const;
  void SetDefaultSpacing(nuiSize set);

protected:


  void    UpdateExpandRatio(std::vector<nuiSize>& rRatios, nuiSize& rCoeff);
  void    AdjustToExpand(nuiSize width, nuiSize height, 
                        std::vector<nuiSize>& Widths, std::vector<nuiSize>& Heights);

  bool mDisplayGridBorder;
  bool mDisplayWidgetBoundingRect;
  bool mPlaceInvisibleWidgets;

  nuiRect mSizeRect;

  nuiSize mGridBorderSize;

  uint    mNbColumns;
  uint    mNbRows;
  
  nuiSize mExpandGrowColumnsCoeff;
  nuiSize mExpandShrinkColumnsCoeff;
  nuiSize mExpandGrowRowsCoeff;
  nuiSize mExpandShrinkRowsCoeff;
  
  std::vector<std::vector<nuiWidget* > > mGrid;
  
  

  std::vector<nuiSize> mWidths;
  std::vector<nuiSize> mHeights;
  
  std::vector<bool> mRowVisible;

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

};

#endif//__nuiGrid_h__


