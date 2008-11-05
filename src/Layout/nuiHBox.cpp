/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiHBox.h"

nuiHBox::nuiHBox(uint nbcells)
: nuiBox(nbcells, 1)
{
  if (SetObjectClass(_T("nuiHBox")))
    InitAttributes();
  
//  mpGrid = new nuiGrid(nbcells, 1);
//  AddChild(mpGrid);
//  nuiGrid::SetProperty(_T("Name"), _T("nuiHBox::Grid"));

}


nuiHBox::~nuiHBox()
{
}
  
  
void nuiHBox::InitAttributes()
{
  // HorizontalSpacing in "common sens" corresponds to DefaultVSpacing in "nuiGrid sens". It's better for css understanding
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("HorizontalSpacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiHBox::GetDefaultSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiHBox::SetDefaultSpacing)));                

  // Spacing is a shorcut for VerticalSpacing, in nuiVBox
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("Spacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiHBox::GetDefaultSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiHBox::SetDefaultSpacing)));                

}
  
  
void nuiHBox::DisplayGridBorder(bool enable, nuiSize gridBorderSize)
{
  nuiGrid::DisplayGridBorder(enable, gridBorderSize);
}


void nuiHBox::DisplayWidgetBoundingRect(bool enable)
{
  nuiGrid::DisplayWidgetBoundingRect(enable);
}



void nuiHBox::PlaceInvisibleWidgets(bool enable)
{
  nuiGrid::PlaceInvisibleWidgets(enable);
}


  
bool nuiHBox::SetCell(uint cell, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting, bool TrashExisting)
{
  return nuiGrid::SetCell(cell, 0, pWidget, position, ReplaceExisting, TrashExisting);
}



void nuiHBox::SetCellPosition(uint cell, nuiPosition position)
{
  nuiGrid::SetCellPosition(cell, 0, position);
}



bool nuiHBox::FindCell(nuiWidgetPtr pWidget, uint& rCell) const
{
  uint row;
  return nuiGrid::FindCell(pWidget, rCell, row);
}



void nuiHBox::ClearCells(bool trash)
{
  nuiGrid::ClearCells(trash);
}



nuiWidget* nuiHBox::GetCell(uint cell) const
{
  return nuiGrid::GetCell(cell, 0);
}



  // Cells settings:
void nuiHBox::SetCellSpacing(uint cell, nuiSize hgap)
{
  nuiGrid::SetColumnSpacing(cell, hgap);
}


void nuiHBox::SetAllCellsSpacing(nuiSize hgap)
{
  for (uint i = 0; i < GetNbCells(); i++)
  {
    SetCellSpacing (i, hgap);
  }
}

void nuiHBox::SetBoxSpacing(nuiSize vgap)
{
  nuiGrid::SetRowSpacing(0, vgap);
}


void nuiHBox::SetBoxSize(uint cells)
{
  nuiGrid::SetGridSize(cells, 1);
}



void nuiHBox::SetCellExpand(uint cell, nuiExpandMode mode, nuiSize ratio)
{
  nuiGrid::SetColumnExpand(cell, mode, ratio);
}

void nuiHBox::SetAllCellsExpand(nuiExpandMode mode)
{
  for (uint i = 0; i < GetNbCells(); i++)
  {
     nuiGrid::SetColumnExpand(i, mode);
  }
}


void nuiHBox::SetExpand(nuiExpandMode mode, nuiSize ratio)
{
  nuiGrid::SetRowExpand(0, mode, ratio);
}



  
void nuiHBox::SetCellMaxGrow(uint cell, nuiSize ratio)
{
  nuiGrid::SetColumnMaxGrow(cell, ratio);
}



void nuiHBox::SetCellMaxPixels(uint cell, nuiSize size)
{
  nuiGrid::SetColumnMaxPixels(cell, size);
}



    
void nuiHBox::SetCellMaxShrink(uint cell, nuiSize ratio)
{
  nuiGrid::SetColumnMaxShrink(cell, ratio);
}



void nuiHBox::SetCellMinPixels(uint cell, nuiSize size)
{
  nuiGrid::SetColumnMinPixels(cell, size);
}



  
  // Corresponding Getters:
nuiSize nuiHBox::GetCellSpacing(uint cell) const
{
  return nuiGrid::GetColumnSpacing(cell);
}





nuiSize nuiHBox::GetCellGrowRatio(uint cell) const
{
  return nuiGrid::GetColumnGrowRatio(cell);
}



nuiSize nuiHBox::GetCellShrinkRatio(uint cell) const
{
  return nuiGrid::GetColumnShrinkRatio(cell);
}




nuiSize nuiHBox::GetCellMaxGrow(uint cell) const
{
  return nuiGrid::GetColumnMaxGrow(cell);
}



nuiSize nuiHBox::GetCellMaxPixels(uint cell) const
{
  return nuiGrid::GetColumnMaxPixels(cell);
}




nuiSize nuiHBox::GetCellMaxShrink(uint cell) const
{
  return nuiGrid::GetColumnMaxShrink(cell);
}



nuiSize nuiHBox::GetCellMinPixels(uint cell) const
{
  return nuiGrid::GetColumnMinPixels(cell);
}




void nuiHBox::AddCells(uint pos, uint cells)
{
  nuiGrid::AddColumns(pos, cells);
}

void nuiHBox::AddCell(nuiWidget* pWidget, nuiPosition position)
{
  uint pos = nuiGrid::GetNbColumns();
  nuiGrid::AddColumns(pos, 1);
  if (GetNbRows() == 0)
    nuiGrid::AddRows(0,1);
  
  nuiGrid::SetCell(pos, 0, pWidget, position); 
}



void nuiHBox::RemoveCells(uint pos, uint cells)
{
  nuiGrid::RemoveColumns(pos, cells);
}


uint nuiHBox::GetNbCells() const
{
  return nuiGrid::GetNbColumns();
}  

bool nuiHBox::Clear(bool Delete)
{
  return nuiGrid::Clear(Delete);
}

nuiSize nuiHBox::GetDefaultSpacing() const
{
  return nuiGrid::GetDefaultVSpacing();
}

void nuiHBox::SetDefaultSpacing(nuiSize set)
{
  nuiGrid::SetDefaultVSpacing(set);
}

void nuiHBox::SetCellPixels(uint column, nuiSize size)
{
  nuiGrid::SetColumnPixels(column, size);
}

void nuiHBox::ResetCellPixels(uint column)
{
  nuiGrid::ResetColumnPixels(column);
}



void nuiHBox::SetSize(nuiSize size)
{
  nuiGrid::SetRowPixels(0, size);
}


