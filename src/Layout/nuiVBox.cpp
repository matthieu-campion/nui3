/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiVBox.h"

nuiVBox::nuiVBox(uint32 nbcells)
: nuiBox(1, nbcells)
{
  if (SetObjectClass(_T("nuiVBox")))
    InitAttributes();
  
  //mpGrid = new nuiGrid(1, nbcells);
  //AddChild(mpGrid);
  //nuiGrid::SetProperty(_T("Name"), _T("nuiVBox::Grid"));

}


nuiVBox::~nuiVBox()
{
}


void nuiVBox::InitAttributes()
{
  InitBoxAttributes();

  // VerticalSpacing in "common sens" corresponds to DefaultHSpacing in "nuiGrid sens". It's better for css understanding
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("VerticalSpacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiVBox::GetDefaultSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiVBox::SetDefaultSpacing)));                

  // Spacing is a shorcut for VerticalSpacing, in nuiVBox
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("Spacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiVBox::GetDefaultSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiVBox::SetDefaultSpacing)));                
       
}

  
void nuiVBox::DisplayGridBorder(bool enable, nuiSize gridBorderSize)
{
  nuiGrid::DisplayGridBorder(enable, gridBorderSize);
}


void nuiVBox::DisplayWidgetBoundingRect(bool enable)
{
  nuiGrid::DisplayWidgetBoundingRect(enable);
}



void nuiVBox::PlaceInvisibleWidgets(bool enable)
{
  nuiGrid::PlaceInvisibleWidgets(enable);
}


  
bool nuiVBox::SetCell(uint32 cell, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting, bool TrashExisting)
{
  return nuiGrid::SetCell(0, cell, pWidget, position, ReplaceExisting, TrashExisting);

}



void nuiVBox::SetCellPosition(uint32 cell, nuiPosition position)
{
  nuiGrid::SetCellPosition(0, cell, position);
}



bool nuiVBox::FindCell(nuiWidgetPtr pWidget, uint& rCell) const
{
  uint32 col;
  return nuiGrid::FindCell(pWidget, col, rCell);
}



void nuiVBox::ClearCells(bool trash)
{
  nuiGrid::ClearCells(trash);
}



nuiWidget* nuiVBox::GetCell(uint32 cell) const
{
  return nuiGrid::GetCell(0, cell);
}



  // Cells settings:
void nuiVBox::SetCellSpacing(uint32 cell, nuiSize vgap)
{
  nuiGrid::SetRowSpacing(cell, vgap);
}

void nuiVBox::SetAllCellsSpacing(nuiSize gap)
{
  for (uint32 i = 0; i < GetNbCells(); i++)
  {
    SetCellSpacing (i, gap);
  }
}


void nuiVBox::SetBoxSpacing(nuiSize hgap)
{
  nuiGrid::SetColumnSpacing(0, hgap);
}

void nuiVBox::SetBoxSize(uint32 cells)
{
  nuiGrid::SetGridSize(1, cells);
}



void nuiVBox::SetCellExpand(uint32 cell, nuiExpandMode mode, nuiSize ratio)
{
  nuiGrid::SetRowExpand(cell, mode, ratio);
}

void nuiVBox::SetAllCellsExpand(nuiExpandMode mode)
{
  for (uint32 i = 0; i < GetNbCells(); i++)
  {
     nuiGrid::SetRowExpand(i, mode);
  }
}


void nuiVBox::SetExpand(nuiExpandMode mode, nuiSize ratio)
{
  nuiGrid::SetColumnExpand(0, mode, ratio);
}

  
void nuiVBox::SetCellMaxGrow(uint32 cell, nuiSize ratio)
{
  nuiGrid::SetRowMaxGrow(cell, ratio);
}



void nuiVBox::SetCellMaxPixels(uint32 cell, nuiSize size)
{
  nuiGrid::SetRowMaxPixels(cell, size);
}



    
void nuiVBox::SetCellMaxShrink(uint32 cell, nuiSize ratio)
{
  nuiGrid::SetRowMaxShrink(cell, ratio);
}



void nuiVBox::SetCellMinPixels(uint32 cell, nuiSize size)
{
  nuiGrid::SetRowMinPixels(cell, size);
}



  
  // Corresponding Getters:
nuiSize nuiVBox::GetCellSpacing(uint32 cell) const
{
  return nuiGrid::GetRowSpacing(cell);
}


nuiSize nuiVBox::GetCellGrowRatio(uint32 cell) const
{
  return nuiGrid::GetRowGrowRatio(cell);
}



nuiSize nuiVBox::GetCellShrinkRatio(uint32 cell) const
{
  return nuiGrid::GetRowShrinkRatio(cell);
}




nuiSize nuiVBox::GetCellMaxGrow(uint32 cell) const
{
  return nuiGrid::GetRowMaxGrow(cell);
}



nuiSize nuiVBox::GetCellMaxPixels(uint32 cell) const
{
  return nuiGrid::GetRowMaxPixels(cell);
}




nuiSize nuiVBox::GetCellMaxShrink(uint32 cell) const
{
  return nuiGrid::GetRowMaxShrink(cell);
}



nuiSize nuiVBox::GetCellMinPixels(uint32 cell) const
{
  return nuiGrid::GetRowMinPixels(cell);
}




void nuiVBox::AddCells(uint32 pos, uint32 cells)
{
  nuiGrid::AddRows(pos, cells);
}

void nuiVBox::AddCell(nuiWidget* pWidget, nuiPosition position)
{
  uint32 pos = nuiGrid::GetNbRows();
  nuiGrid::AddRows(pos, 1);
  if (GetNbColumns() == 0)
    nuiGrid::AddColumns(0,1);
  
  nuiGrid::SetCell(0, pos, pWidget, position); 
}


void nuiVBox::RemoveCells(uint32 pos, uint32 cells)
{
  nuiGrid::RemoveRows(pos, cells);
}


uint32 nuiVBox::GetNbCells() const
{
  return nuiGrid::GetNbRows();
}  

bool nuiVBox::Clear(bool Delete)
{
  bool res = Clear(Delete);
  nuiGrid::AddColumns(0,1);
  return res;
}

nuiSize nuiVBox::GetDefaultSpacing() const
{
  return nuiGrid::GetDefaultHSpacing();
}

void nuiVBox::SetDefaultSpacing(nuiSize set)
{
  nuiGrid::SetDefaultHSpacing(set);
}


void nuiVBox::SetCellPixels(uint32 row, nuiSize size)
{
  nuiGrid::SetRowPixels(row, size);
}

void nuiVBox::ResetCellPixels(uint32 row)
{
  nuiGrid::ResetRowPixels(row);
}



void nuiVBox::SetSize(nuiSize size)
{
  nuiGrid::SetColumnPixels(0, size);
}

nuiSize nuiVBox::GetCellPixels(uint32 cell) const
{
  return nuiGrid::GetRowPixels(cell);
}



