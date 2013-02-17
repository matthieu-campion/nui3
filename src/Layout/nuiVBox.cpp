/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiVBox::nuiVBox(uint32 nbcells)
: nuiBox(1, nbcells)
{
  if (SetObjectClass(_T("nuiVBox")))
    InitAttributes();
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
    nuiMakeDelegate(this, &nuiVBox::GetDefaultSpacing),
    nuiMakeDelegate(this, &nuiVBox::SetDefaultSpacing)));                

  // Spacing is a shorcut for VerticalSpacing, in nuiVBox
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("Spacing")), nuiUnitSize,
    nuiMakeDelegate(this, &nuiVBox::GetDefaultSpacing),
    nuiMakeDelegate(this, &nuiVBox::SetDefaultSpacing)));                
       
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


  
bool nuiVBox::SetCell(uint32 cell, nuiWidget* pWidget, nuiPosition position)
{
  return nuiGrid::SetCell(0, cell, pWidget, position);

}

nuiLabel* nuiVBox::SetCell(uint32 cell, const nglString& rLabel, const nglString& rObjectName, nuiPosition position)
{
  nuiLabel* pLabel = new nuiLabel(rLabel);
  if (!rObjectName.IsNull())
    pLabel->SetObjectName(rObjectName);
  bool res = nuiGrid::SetCell(0, cell, pLabel, position);
  if (!res)
    return NULL;
  
  return pLabel;
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



void nuiVBox::ClearCells()
{
  nuiGrid::ClearCells();
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


void nuiVBox::SetExpand(nuiExpandMode mode)
{
  nuiGrid::SetColumnExpand(0, mode, 1.0f);
}


nuiExpandMode nuiVBox::GetExpand() const
{
  return nuiGrid::GetColumnExpand(0);
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


nuiExpandMode nuiVBox::GetCellExpand(uint32 cell) const
{
  return nuiGrid::GetRowExpand(cell);
}



void nuiVBox::AddCells(uint32 pos, uint32 cells)
{
  nuiGrid::AddRows(pos, cells);
}

void nuiVBox::AddCell(nuiWidget* pWidget, nuiPosition position)
{
  uint32 pos = nuiGrid::GetNbRows();
  AddCell(pos, pWidget, position);
}

void nuiVBox::AddCell(uint32 pos, nuiWidget* pWidget, nuiPosition position)
{
  nuiGrid::AddRows(pos, 1);
  if (GetNbColumns() == 0)
    nuiGrid::AddColumns(0,1);
  
  nuiGrid::SetCell(0, pos, pWidget, position); 
}



nuiLabel* nuiVBox::AddCell(const nglString& rLabel, const nglString& rObjectName, nuiPosition position)
{
  nuiLabel* pLabel = new nuiLabel(rLabel);
  if (!rObjectName.IsNull())
    pLabel->SetObjectName(rObjectName);
  nuiVBox::AddCell(pLabel, position);
  
  return pLabel;
}



void nuiVBox::RemoveCells(uint32 pos, uint32 cells)
{
  nuiGrid::RemoveRows(pos, cells);
}


uint32 nuiVBox::GetNbCells() const
{
  return nuiGrid::GetNbRows();
}  

bool nuiVBox::Clear()
{
  nuiSize spacing = nuiGrid::GetColumnSpacing(0);
  nuiSize growRatio = nuiGrid::GetColumnGrowRatio(0);
  nuiSize shrinkRatio = nuiGrid::GetColumnShrinkRatio(0);
  nuiSize maxGrow = nuiGrid::GetColumnMaxGrow(0);
  nuiSize maxShrink = nuiGrid::GetColumnMaxShrink(0);
  nuiSize maxPixels = nuiGrid::GetColumnMaxPixels(0);
  nuiSize minPixels = nuiGrid::GetColumnMinPixels(0);
  
  bool res = nuiGrid::Clear();
  nuiGrid::AddColumns(0,1);
  
  nuiGrid::SetColumnSpacing(0, spacing);
  if (growRatio > 0)
    nuiGrid::SetColumnExpand(0, nuiExpandGrow, growRatio);
  if (shrinkRatio > 0)
    nuiGrid::SetColumnExpand(0, nuiExpandShrink, shrinkRatio);
  if (maxGrow >= 1.f)
    nuiGrid::SetColumnMaxGrow(0, maxGrow);
  if (maxShrink >= 0.f)
    nuiGrid::SetColumnMaxShrink(0, maxShrink);
  if (maxPixels >= 0.f)
    nuiGrid::SetColumnMaxPixels(0, maxPixels);
  if (minPixels >= 0.f)    
    nuiGrid::SetColumnMinPixels(0, minPixels);

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


void nuiVBox::SetEqualizeCells(bool set)
{
  SetEqualizeRows(set);
}

bool nuiVBox::GetEqualizeCells() const
{
  return nuiGrid::GetEqualizeRows();
}

