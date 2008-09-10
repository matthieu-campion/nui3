/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiBox__
#define __nuiBox__



#include "nui.h"
#include "nuiGrid.h"

// this is an interface for hboxes and vboxes
class nuiBox : public nuiGrid
{
public:  
  virtual void DisplayGridBorder(bool enable, nuiSize gridBorderSize = 1.f) = 0;  ///< Allow to display the grid border
  virtual void DisplayWidgetBoundingRect(bool enable) = 0;  ///< Allow to display widget's bounding rect
  virtual void PlaceInvisibleWidgets(bool enable) = 0;
  
  virtual bool SetCell(uint cell, nuiWidget* pWidget, nuiPosition position = nuiNoPosition, bool ReplaceExisting = true, bool TrashExisting=true) = 0;   ///< returns false if cell is not empty and ReplaceExisting == false. Otherwise, thrash any existing widget and replace it with the new one.
  virtual void SetCellPosition(uint cell, nuiPosition position) = 0;  ///< Sets the position of the widget inside the cell
  virtual bool FindCell(nuiWidgetPtr pWidget, uint& rCell) const = 0;  ///< Returns true if \p pWidget was found in this container. If it was found, its position in the grid are set in rCell.
  
  virtual void ClearCells(bool trash=true) = 0;
  
  virtual nuiWidget* GetCell(uint cell) const = 0;                ///< returns NULL if cell is empty
  
  // Cells settings:
  virtual void SetCellSpacing(uint cell, nuiSize gap) = 0;  ///< Sets the spacing to gap  around widgets in the given cell. (hbox: horizontally, vbox: vertically)
  virtual void SetAllCellsSpacing(nuiSize set) = 0; ////< Sets the spacing to hgap around widgets in all cells. (hbox: horizontally, vxob: vertically)
  virtual void SetBoxSpacing(nuiSize gap) = 0; // Sets the spacing to gap around the box (hbox: vertically, vxob: horizontally)
  virtual void SetBoxSize(uint cells) = 0;         ///< Sets the grid size in number of cells, you have use this before inserting any element in the grid (via SetCell).
  virtual void SetCellExpand(uint cell, nuiExpandMode mode, nuiSize ratio = 1.f) = 0; ///< Sets the expand mode and ratio to the given cell. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other cell ratios
  virtual void SetAllCellsExpand(nuiExpandMode mode)=0; ///< Sets the expand mode and ratio to all cells.
  
  virtual void SetExpand(nuiExpandMode mode, nuiSize ratio=1.f) = 0; ///< Sets the expand mode and ratio (hbox: vertically / vbox: horizontally)
  
  virtual void SetCellMaxGrow(uint cell, nuiSize ratio) = 0;         ///< Sets the maximum growing percentage ratio for the given cell 
  virtual void SetCellMaxPixels(uint cell, nuiSize size) = 0;        ///< Sets the maximum size in pixels for the given cell
  
  virtual void SetCellMaxShrink(uint cell, nuiSize ratio) = 0;       ///< Sets the maximum shrinking percentage ratio for the given cell 
  virtual void SetCellMinPixels(uint cell, nuiSize size) = 0;        ///< Sets the minimum size in pixels for the given cell
  
  virtual void SetCellPixels(uint cell, nuiSize size) = 0;  ///< Force the size of a cell.
  virtual void ResetCellPixels(uint cell) = 0;  ///< make the minimum size in pixels back to its variable capactiy
  
  virtual void SetSize(nuiSize size) = 0;  ///< Set the requested size of the whole box (hbox: height, vbox: width)
  
  // Corresponding Getters:
  virtual nuiSize GetCellSpacing(uint cell) const = 0;  ///< Gets the horizontal spacing around widgets in the given cell.
  virtual nuiSize GetCellGrowRatio(uint cell) const = 0; ///< Gets the ratio to the given cell.
  virtual nuiSize GetCellShrinkRatio(uint cell) const = 0; ///< Gets the expand ratio of the given cell.
  
  virtual nuiSize GetCellMaxGrow(uint cell) const = 0;         ///< Gets the maximum growing percentage ratio for the given cell 
  virtual nuiSize GetCellMaxPixels(uint cell) const = 0;        ///< Gets the maximum size in pixels for the given cell
  
  virtual nuiSize GetCellMaxShrink(uint cell) const = 0;       ///< Gets the maximum shrinking percentage ratio for the given cell 
  virtual nuiSize GetCellMinPixels(uint cell) const = 0;        ///< Gets the minimum size in pixels for the given cell
  
  // Add cells:
  virtual void AddCells(uint pos, uint cells) = 0;
  virtual void AddCell(nuiWidget* pWidget, nuiPosition position = nuiNoPosition) = 0;
  
  virtual uint GetNbCells() const = 0;
  
  virtual void RemoveCells(uint pos, uint cells) = 0;
  
  virtual bool Clear(bool Delete = true) = 0;
  
  virtual nuiSize GetDefaultSpacing() const = 0;
  virtual void SetDefaultSpacing(nuiSize set) = 0;
  
  
  virtual ~nuiBox()
  {
  }
  
protected:
  nuiBox(uint32 nbcellsH, uint32 nbcellsV)
  : nuiGrid(nbcellsH, nbcellsV)
  {
  }
};


#endif
