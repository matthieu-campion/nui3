/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#ifndef __nuiVBox__
#define __nuiVBox__

#include "nui.h"
#include "nuiGrid.h"
#include "nuiBox.h"

/// this class implements a vertical grid with a single column.
class nuiVBox : public nuiBox
{
public:
  nuiVBox(uint nbcells = 0);
  virtual ~nuiVBox();
  
  void InitAttributes();
  
  void DisplayGridBorder(bool enable, nuiSize gridBorderSize = 1.f); ///< Allow to display the grid border
  void DisplayWidgetBoundingRect(bool enable); ///< Allow to display widget's bounding rect
  void PlaceInvisibleWidgets(bool enable);
  
  bool SetCell(uint cell, nuiWidget* pWidget, nuiPosition position = nuiNoPosition, bool ReplaceExisting = true, bool TrashExisting=true);  ///< returns false if cell is not empty and ReplaceExisting == false. Otherwise, thrash any existing widget and replace it with the new one.
  void SetCellPosition(uint cell, nuiPosition position); ///< Sets the position of the widget inside the cell
  bool FindCell(nuiWidgetPtr pWidget, uint& rCell) const; ///< Returns true if \p pWidget was found in this container. If it was found, its position in the grid are set in rCell.

  void ClearCells(bool trash=true);

  nuiWidget* GetCell(uint cell) const;               ///< returns NULL if cell is empty

  // Cells settings:
  void SetCellSpacing(uint cell, nuiSize vgap); ///< Sets the vertical spacing to vgap around widgets in the given cell.
  void SetAllCellsSpacing(nuiSize vgap);  //< Sets the vertical spacing to vgap around widgets in all cells.
  void SetBoxSpacing(nuiSize hgap);
  void SetBoxSize(uint cells);        ///< Sets the grid size in number of cells, you have use this before inserting any element in the grid (via SetCell).
  void SetCellExpand(uint cell, nuiExpandMode mode, nuiSize ratio = 1.f);///< Sets the expand mode and ratio to the given cell. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other cell ratios
  void SetAllCellsExpand(nuiExpandMode mode); ///< Sets the expand mode and ratio to all cells.
  void SetExpand(nuiExpandMode mode, nuiSize ratio=1.f);///< Sets the expand mode and ratio to the vbox column
  
  void SetCellMaxGrow(uint cell, nuiSize ratio);        ///< Sets the maximum growing percentage ratio for the given cell 
  void SetCellMaxPixels(uint cell, nuiSize size);       ///< Sets the maximum size in pixels for the given cell
    
  void SetCellMaxShrink(uint cell, nuiSize ratio);      ///< Sets the maximum shrinking percentage ratio for the given cell 
  void SetCellMinPixels(uint cell, nuiSize size);       ///< Sets the minimum size in pixels for the given cell
  
  void SetCellPixels(uint row, nuiSize size); ///< Force the size of a row.
  void ResetCellPixels(uint cell);  ///< make the minimum size in pixels back to its variable capactiy

  void SetSize(nuiSize size); ///< Set the requested width of the whole box
  
  // Corresponding Getters:
  nuiSize GetCellSpacing(uint cell) const; ///< Gets the horizontal spacing to hgap around widgets in the given cell.
  void GetBoxSize(uint& cells) const;        ///< Gets the grid size in number of cells.
  nuiSize GetCellGrowRatio(uint cell) const;///< Gets the ratio to the given cell.
  nuiSize GetCellShrinkRatio(uint cell) const;///< Gets the expand ratio of the given cell.

  nuiSize GetCellMaxGrow(uint cell) const;        ///< Gets the maximum growing percentage ratio for the given cell 
  nuiSize GetCellMaxPixels(uint cell) const;       ///< Gets the maximum size in pixels for the given cell

  nuiSize GetCellMaxShrink(uint cell) const;      ///< Gets the maximum shrinking percentage ratio for the given cell 
  nuiSize GetCellMinPixels(uint cell) const;       ///< Gets the minimum size in pixels for the given cell

  // Add cells:
  void AddCells(uint pos, uint cells);
  void AddCell(nuiWidget* pWidget, nuiPosition position = nuiNoPosition);

  uint GetNbCells() const;

  void RemoveCells(uint pos, uint cells);

  virtual bool Clear(bool Delete = true);

  nuiSize GetDefaultSpacing() const;
  void SetDefaultSpacing(nuiSize set);

protected:

  nuiGrid*  mpGrid;


};

#endif

