/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiVBox.h"

nuiVBox::nuiVBox(uint nbcells)
: nuiBox()
{
  if (SetObjectClass(_T("nuiVBox")))
    InitAttributes();
  
  mpGrid = new nuiGrid(1, nbcells);
  AddChild(mpGrid);
  mpGrid->SetProperty(_T("Name"), _T("nuiVBox::Grid"));

}


nuiVBox::~nuiVBox()
{
}


void nuiVBox::InitAttributes()
{
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
  mpGrid->DisplayGridBorder(enable, gridBorderSize);
}


void nuiVBox::DisplayWidgetBoundingRect(bool enable)
{
  mpGrid->DisplayWidgetBoundingRect(enable);
}



void nuiVBox::PlaceInvisibleWidgets(bool enable)
{
  mpGrid->PlaceInvisibleWidgets(enable);
}


  
bool nuiVBox::SetCell(uint cell, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting, bool TrashExisting)
{
  return mpGrid->SetCell(0, cell, pWidget, position, ReplaceExisting, TrashExisting);

}



void nuiVBox::SetCellPosition(uint cell, nuiPosition position)
{
  mpGrid->SetCellPosition(0, cell, position);
}



bool nuiVBox::FindCell(nuiWidgetPtr pWidget, uint& rCell) const
{
  uint col;
  return mpGrid->FindCell(pWidget, col, rCell);
}



void nuiVBox::ClearCells(bool trash)
{
  mpGrid->ClearCells(trash);
}



nuiWidget* nuiVBox::GetCell(uint cell) const
{
  return mpGrid->GetCell(0, cell);
}



  // Cells settings:
void nuiVBox::SetCellSpacing(uint cell, nuiSize vgap)
{
  mpGrid->SetRowSpacing(cell, vgap);
}

void nuiVBox::SetAllCellsSpacing(nuiSize gap)
{
  for (uint i = 0; i < GetNbCells(); i++)
  {
    SetCellSpacing (i, gap);
  }
}


void nuiVBox::SetBoxSpacing(nuiSize hgap)
{
  mpGrid->SetColumnSpacing(0, hgap);
}

void nuiVBox::SetBoxSize(uint cells)
{
  mpGrid->SetGridSize(1, cells);
}



void nuiVBox::SetCellExpand(uint cell, nuiExpandMode mode, nuiSize ratio)
{
  mpGrid->SetRowExpand(cell, mode, ratio);
}

void nuiVBox::SetAllCellsExpand(nuiExpandMode mode)
{
  for (uint i = 0; i < GetNbCells(); i++)
  {
     mpGrid->SetRowExpand(i, mode);
  }
}


void nuiVBox::SetExpand(nuiExpandMode mode, nuiSize ratio)
{
  mpGrid->SetColumnExpand(0, mode, ratio);
}

  
void nuiVBox::SetCellMaxGrow(uint cell, nuiSize ratio)
{
  mpGrid->SetRowMaxGrow(cell, ratio);
}



void nuiVBox::SetCellMaxPixels(uint cell, nuiSize size)
{
  mpGrid->SetRowMaxPixels(cell, size);
}



    
void nuiVBox::SetCellMaxShrink(uint cell, nuiSize ratio)
{
  mpGrid->SetRowMaxShrink(cell, ratio);
}



void nuiVBox::SetCellMinPixels(uint cell, nuiSize size)
{
  mpGrid->SetRowMinPixels(cell, size);
}



  
  // Corresponding Getters:
nuiSize nuiVBox::GetCellSpacing(uint cell) const
{
  return mpGrid->GetRowSpacing(cell);
}


nuiSize nuiVBox::GetCellGrowRatio(uint cell) const
{
  return mpGrid->GetRowGrowRatio(cell);
}



nuiSize nuiVBox::GetCellShrinkRatio(uint cell) const
{
  return mpGrid->GetRowShrinkRatio(cell);
}




nuiSize nuiVBox::GetCellMaxGrow(uint cell) const
{
  return mpGrid->GetRowMaxGrow(cell);
}



nuiSize nuiVBox::GetCellMaxPixels(uint cell) const
{
  return mpGrid->GetRowMaxPixels(cell);
}




nuiSize nuiVBox::GetCellMaxShrink(uint cell) const
{
  return mpGrid->GetRowMaxShrink(cell);
}



nuiSize nuiVBox::GetCellMinPixels(uint cell) const
{
  return mpGrid->GetRowMinPixels(cell);
}




void nuiVBox::AddCells(uint pos, uint cells)
{
  mpGrid->AddRows(pos, cells);
}

void nuiVBox::AddCell(nuiWidget* pWidget, nuiPosition position)
{
  uint pos = mpGrid->GetNbRows();
  mpGrid->AddRows(pos, 1);
  mpGrid->SetCell(0, pos, pWidget, position); 
}


void nuiVBox::RemoveCells(uint pos, uint cells)
{
  mpGrid->RemoveRows(pos, cells);
}


uint nuiVBox::GetNbCells() const
{
  return mpGrid->GetNbRows();
}  

bool nuiVBox::Clear(bool Delete)
{
  return mpGrid->Clear(Delete);
}

nuiSize nuiVBox::GetDefaultSpacing() const
{
  return mpGrid->GetDefaultHSpacing();
}

void nuiVBox::SetDefaultSpacing(nuiSize set)
{
  mpGrid->SetDefaultHSpacing(set);
}


void nuiVBox::SetCellPixels(uint row, nuiSize size)
{
  mpGrid->SetRowPixels(row, size);
}

void nuiVBox::ResetCellPixels(uint row)
{
  mpGrid->ResetRowPixels(row);
}



void nuiVBox::SetSize(nuiSize size)
{
  mpGrid->SetColumnPixels(0, size);
}



