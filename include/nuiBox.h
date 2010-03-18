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
  
  virtual bool SetCell(uint32 cell, nuiWidget* pWidget, nuiPosition position = nuiNoPosition) = 0;   ///< returns false if cell is not empty and ReplaceExisting == false. Otherwise, thrash any existing widget and replace it with the new one.
  virtual nuiLabel* SetCell(uint32 cell, const nglString& rLabel, const nglString& rObjectName, nuiPosition position = nuiNoPosition) = 0; ///< create a nuiLabel with a specific objectname and store it in the box. Usefull to add a simple label to a box, using a css decoration.   
  virtual void SetCellPosition(uint32 cell, nuiPosition position) = 0;  ///< Sets the position of the widget inside the cell
  virtual bool FindCell(nuiWidgetPtr pWidget, uint& rCell) const = 0;  ///< Returns true if \p pWidget was found in this container. If it was found, its position in the grid are set in rCell.
  
  virtual void ClearCells() = 0;
  
  virtual nuiWidget* GetCell(uint32 cell) const = 0;                ///< returns NULL if cell is empty
  
  // Cells settings:
  virtual void SetCellSpacing(uint32 cell, nuiSize gap) = 0;  ///< Sets the spacing to gap  around widgets in the given cell. (hbox: horizontally, vbox: vertically)
  virtual void SetAllCellsSpacing(nuiSize set) = 0; ////< Sets the spacing to hgap around widgets in all cells. (hbox: horizontally, vxob: vertically)
  virtual void SetBoxSpacing(nuiSize gap) = 0; // Sets the spacing to gap around the box (hbox: vertically, vxob: horizontally)
  virtual void SetBoxSize(uint32 cells) = 0;         ///< Sets the grid size in number of cells, you have use this before inserting any element in the grid (via SetCell).
  virtual void SetCellExpand(uint32 cell, nuiExpandMode mode, nuiSize ratio = 1.f) = 0; ///< Sets the expand mode and ratio to the given cell. Ratio has to be equal or superior to 1.0, and is used to expand proportionnaly to other cell ratios
  virtual void SetAllCellsExpand(nuiExpandMode mode)=0; ///< Sets the expand mode and ratio to all cells.
  
  virtual void SetExpand(nuiExpandMode mode) = 0; ///< Sets the expand mode and ratio (hbox: vertically / vbox: horizontally)
  virtual nuiExpandMode GetExpand() const = 0;
  
  virtual void SetCellMaxGrow(uint32 cell, nuiSize ratio) = 0;         ///< Sets the maximum growing percentage ratio for the given cell 
  virtual void SetCellMaxPixels(uint32 cell, nuiSize size) = 0;        ///< Sets the maximum size in pixels for the given cell
  
  virtual void SetCellMaxShrink(uint32 cell, nuiSize ratio) = 0;       ///< Sets the maximum shrinking percentage ratio for the given cell 
  virtual void SetCellMinPixels(uint32 cell, nuiSize size) = 0;        ///< Sets the minimum size in pixels for the given cell
  
  virtual void SetCellPixels(uint32 cell, nuiSize size) = 0;  ///< Force the size of a cell.
  virtual void ResetCellPixels(uint32 cell) = 0;  ///< make the minimum size in pixels back to its variable capactiy
  
  virtual void SetSize(nuiSize size) = 0;  ///< Set the requested size of the whole box (hbox: height, vbox: width)
  
  // Corresponding Getters:
  virtual nuiSize GetCellSpacing(uint32 cell) const = 0;  ///< Gets the horizontal spacing around widgets in the given cell.
  virtual nuiSize GetCellGrowRatio(uint32 cell) const = 0; ///< Gets the ratio to the given cell.
  virtual nuiSize GetCellShrinkRatio(uint32 cell) const = 0; ///< Gets the expand ratio of the given cell.
  
  virtual nuiSize GetCellMaxGrow(uint32 cell) const = 0;         ///< Gets the maximum growing percentage ratio for the given cell 
  virtual nuiSize GetCellMaxPixels(uint32 cell) const = 0;        ///< Gets the maximum size in pixels for the given cell
  
  virtual nuiSize GetCellMaxShrink(uint32 cell) const = 0;       ///< Gets the maximum shrinking percentage ratio for the given cell 
  virtual nuiSize GetCellMinPixels(uint32 cell) const = 0;        ///< Gets the minimum size in pixels for the given cell

  virtual nuiSize GetCellPixels(uint32 cell) const = 0;        ///< Gets the size in pixels for the given cell
  virtual nuiExpandMode GetCellExpand(uint32 cell) const = 0;        ///< Gets the size in pixels for the given cell
  
  // Add cells:
  virtual void AddCells(uint32 pos, uint32 cells) = 0;
  virtual void AddCell(nuiWidget* pWidget, nuiPosition position = nuiNoPosition) = 0;
  virtual void AddCell(uint32 pos, nuiWidget* pWidget, nuiPosition position = nuiNoPosition) = 0;
  virtual nuiLabel* AddCell(const nglString& rLabel, const nglString& rObjectName = nglString::Null, nuiPosition position = nuiNoPosition) = 0; ///< create a nuiLabel with a specific objectname and store it in the box. Usefull to add a simple label to a box, using a css decoration.   
  
  virtual uint32 GetNbCells() const = 0;
  
  virtual void RemoveCells(uint32 pos, uint32 cells) = 0;
  
  virtual bool Clear() = 0;
  
  virtual nuiSize GetDefaultSpacing() const = 0;
  virtual void SetDefaultSpacing(nuiSize set) = 0;
  
  virtual void SetEqualizeCells(bool set) = 0;
  virtual bool GetEqualizeCells() const = 0;
  
  virtual ~nuiBox();
  
protected:
  nuiBox(uint32 nbcellsH, uint32 nbcellsV);
  
  void InitBoxAttributes();
  uint32 GetBoxDimensionRange(uint32 dimension) const;
  void _SetCellExpand(uint32 cell, nuiExpandMode mode);
};


#endif
