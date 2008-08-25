/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiHBox.h"

nuiHBox::nuiHBox(uint nbcells)
: nuiBox()
{
  if (SetObjectClass(_T("nuiHBox")))
    InitAttributes();
  
  mpGrid = new nuiGrid(nbcells, 1);
  AddChild(mpGrid);
  mpGrid->SetProperty(_T("Name"), _T("nuiHBox::Grid"));

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
  mpGrid->DisplayGridBorder(enable, gridBorderSize);
}


void nuiHBox::DisplayWidgetBoundingRect(bool enable)
{
  mpGrid->DisplayWidgetBoundingRect(enable);
}



void nuiHBox::PlaceInvisibleWidgets(bool enable)
{
  mpGrid->PlaceInvisibleWidgets(enable);
}


  
bool nuiHBox::SetCell(uint cell, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting, bool TrashExisting)
{
  return mpGrid->SetCell(cell, 0, pWidget, position, ReplaceExisting, TrashExisting);
}



void nuiHBox::SetCellPosition(uint cell, nuiPosition position)
{
  mpGrid->SetCellPosition(cell, 0, position);
}



bool nuiHBox::FindCell(nuiWidgetPtr pWidget, uint& rCell) const
{
  uint row;
  return mpGrid->FindCell(pWidget, rCell, row);
}



void nuiHBox::ClearCells(bool trash)
{
  mpGrid->ClearCells(trash);
}



nuiWidget* nuiHBox::GetCell(uint cell) const
{
  return mpGrid->GetCell(cell, 0);
}



  // Cells settings:
void nuiHBox::SetCellSpacing(uint cell, nuiSize hgap)
{
  mpGrid->SetColumnSpacing(cell, hgap);
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
  mpGrid->SetRowSpacing(0, vgap);
}


void nuiHBox::SetBoxSize(uint cells)
{
  mpGrid->SetGridSize(cells, 1);
}



void nuiHBox::SetCellExpand(uint cell, nuiExpandMode mode, nuiSize ratio)
{
  mpGrid->SetColumnExpand(cell, mode, ratio);
}

void nuiHBox::SetAllCellsExpand(nuiExpandMode mode)
{
  for (uint i = 0; i < GetNbCells(); i++)
  {
     mpGrid->SetColumnExpand(i, mode);
  }
}


void nuiHBox::SetExpand(nuiExpandMode mode, nuiSize ratio)
{
  mpGrid->SetRowExpand(0, mode, ratio);
}



  
void nuiHBox::SetCellMaxGrow(uint cell, nuiSize ratio)
{
  mpGrid->SetColumnMaxGrow(cell, ratio);
}



void nuiHBox::SetCellMaxPixels(uint cell, nuiSize size)
{
  mpGrid->SetColumnMaxPixels(cell, size);
}



    
void nuiHBox::SetCellMaxShrink(uint cell, nuiSize ratio)
{
  mpGrid->SetColumnMaxShrink(cell, ratio);
}



void nuiHBox::SetCellMinPixels(uint cell, nuiSize size)
{
  mpGrid->SetColumnMinPixels(cell, size);
}



  
  // Corresponding Getters:
nuiSize nuiHBox::GetCellSpacing(uint cell) const
{
  return mpGrid->GetColumnSpacing(cell);
}





nuiSize nuiHBox::GetCellGrowRatio(uint cell) const
{
  return mpGrid->GetColumnGrowRatio(cell);
}



nuiSize nuiHBox::GetCellShrinkRatio(uint cell) const
{
  return mpGrid->GetColumnShrinkRatio(cell);
}




nuiSize nuiHBox::GetCellMaxGrow(uint cell) const
{
  return mpGrid->GetColumnMaxGrow(cell);
}



nuiSize nuiHBox::GetCellMaxPixels(uint cell) const
{
  return mpGrid->GetColumnMaxPixels(cell);
}




nuiSize nuiHBox::GetCellMaxShrink(uint cell) const
{
  return mpGrid->GetColumnMaxShrink(cell);
}



nuiSize nuiHBox::GetCellMinPixels(uint cell) const
{
  return mpGrid->GetColumnMinPixels(cell);
}




void nuiHBox::AddCells(uint pos, uint cells)
{
  mpGrid->AddColumns(pos, cells);
}

void nuiHBox::AddCell(nuiWidget* pWidget, nuiPosition position)
{
  uint pos = mpGrid->GetNbColumns();
  mpGrid->AddColumns(pos, 1);
  mpGrid->SetCell(pos, 0, pWidget, position); 
}



void nuiHBox::RemoveCells(uint pos, uint cells)
{
  mpGrid->RemoveColumns(pos, cells);
}


uint nuiHBox::GetNbCells() const
{
  return mpGrid->GetNbColumns();
}  

bool nuiHBox::Clear(bool Delete)
{
  return mpGrid->Clear(Delete);
}

nuiSize nuiHBox::GetDefaultSpacing() const
{
  return mpGrid->GetDefaultVSpacing();
}

void nuiHBox::SetDefaultSpacing(nuiSize set)
{
  mpGrid->SetDefaultVSpacing(set);
}

void nuiHBox::SetCellPixels(uint column, nuiSize size)
{
  mpGrid->SetColumnPixels(column, size);
}

void nuiHBox::ResetCellPixels(uint column)
{
  mpGrid->ResetColumnPixels(column);
}



void nuiHBox::SetSize(nuiSize size)
{
  mpGrid->SetRowPixels(0, size);
}


