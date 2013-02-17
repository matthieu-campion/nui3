/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"

nuiHBox::nuiHBox(uint32 nbcells)
: nuiBox(nbcells, 1)
{
  if (SetObjectClass(_T("nuiHBox")))
    InitAttributes();
}


nuiHBox::~nuiHBox()
{
}
  
  
void nuiHBox::InitAttributes()
{
  InitBoxAttributes();

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


  
bool nuiHBox::SetCell(uint32 cell, nuiWidget* pWidget, nuiPosition position)
{
  return nuiGrid::SetCell(cell, 0, pWidget, position);
}

nuiLabel* nuiHBox::SetCell(uint32 cell, const nglString& rLabel, const nglString& rObjectName, nuiPosition position)
{
  nuiLabel* pLabel = new nuiLabel(rLabel);
  if (!rObjectName.IsNull())
    pLabel->SetObjectName(rObjectName);
  bool res = nuiGrid::SetCell(cell, 0, pLabel, position);
  if (!res)
    return NULL;
  
  return pLabel;
}


void nuiHBox::SetCellPosition(uint32 cell, nuiPosition position)
{
  nuiGrid::SetCellPosition(cell, 0, position);
}



bool nuiHBox::FindCell(nuiWidgetPtr pWidget, uint& rCell) const
{
  uint32 row;
  return nuiGrid::FindCell(pWidget, rCell, row);
}



void nuiHBox::ClearCells()
{
  nuiGrid::ClearCells();
}



nuiWidget* nuiHBox::GetCell(uint32 cell) const
{
  return nuiGrid::GetCell(cell, 0);
}



  // Cells settings:
void nuiHBox::SetCellSpacing(uint32 cell, nuiSize hgap)
{
  nuiGrid::SetColumnSpacing(cell, hgap);
}


void nuiHBox::SetAllCellsSpacing(nuiSize hgap)
{
  for (uint32 i = 0; i < GetNbCells(); i++)
  {
    SetCellSpacing (i, hgap);
  }
}

void nuiHBox::SetBoxSpacing(nuiSize vgap)
{
  nuiGrid::SetRowSpacing(0, vgap);
}


void nuiHBox::SetBoxSize(uint32 cells)
{
  nuiGrid::SetGridSize(cells, 1);
}



void nuiHBox::SetCellExpand(uint32 cell, nuiExpandMode mode, nuiSize ratio)
{
  nuiGrid::SetColumnExpand(cell, mode, ratio);
}

void nuiHBox::SetAllCellsExpand(nuiExpandMode mode)
{
  for (uint32 i = 0; i < GetNbCells(); i++)
  {
     nuiGrid::SetColumnExpand(i, mode);
  }
}


void nuiHBox::SetExpand(nuiExpandMode mode)
{
  nuiGrid::SetRowExpand(0, mode, 1.0f);
}

nuiExpandMode nuiHBox::GetExpand() const
{
  return nuiGrid::GetRowExpand(0);
}



  
void nuiHBox::SetCellMaxGrow(uint32 cell, nuiSize ratio)
{
  nuiGrid::SetColumnMaxGrow(cell, ratio);
}



void nuiHBox::SetCellMaxPixels(uint32 cell, nuiSize size)
{
  nuiGrid::SetColumnMaxPixels(cell, size);
}



    
void nuiHBox::SetCellMaxShrink(uint32 cell, nuiSize ratio)
{
  nuiGrid::SetColumnMaxShrink(cell, ratio);
}



void nuiHBox::SetCellMinPixels(uint32 cell, nuiSize size)
{
  nuiGrid::SetColumnMinPixels(cell, size);
}



  
  // Corresponding Getters:
nuiSize nuiHBox::GetCellSpacing(uint32 cell) const
{
  return nuiGrid::GetColumnSpacing(cell);
}





nuiSize nuiHBox::GetCellGrowRatio(uint32 cell) const
{
  return nuiGrid::GetColumnGrowRatio(cell);
}



nuiSize nuiHBox::GetCellShrinkRatio(uint32 cell) const
{
  return nuiGrid::GetColumnShrinkRatio(cell);
}




nuiSize nuiHBox::GetCellMaxGrow(uint32 cell) const
{
  return nuiGrid::GetColumnMaxGrow(cell);
}



nuiSize nuiHBox::GetCellMaxPixels(uint32 cell) const
{
  return nuiGrid::GetColumnMaxPixels(cell);
}




nuiSize nuiHBox::GetCellMaxShrink(uint32 cell) const
{
  return nuiGrid::GetColumnMaxShrink(cell);
}



nuiSize nuiHBox::GetCellMinPixels(uint32 cell) const
{
  return nuiGrid::GetColumnMinPixels(cell);
}


nuiExpandMode nuiHBox::GetCellExpand(uint32 cell) const
{
  return nuiGrid::GetColumnExpand(cell);
}


void nuiHBox::AddCells(uint32 pos, uint32 cells)
{
  nuiGrid::AddColumns(pos, cells);
}

void nuiHBox::AddCell(uint32 pos, nuiWidget* pWidget, nuiPosition position)
{
  nuiGrid::AddColumns(pos, 1);
  if (GetNbRows() == 0)
    nuiGrid::AddRows(0,1);
  
  nuiGrid::SetCell(pos, 0, pWidget, position); 
}


void nuiHBox::AddCell(nuiWidget* pWidget, nuiPosition position)
{
  uint32 pos = nuiGrid::GetNbColumns();
  AddCell(pos, pWidget, position);
}


nuiLabel* nuiHBox::AddCell(const nglString& rLabel, const nglString& rObjectName, nuiPosition position)
{
  nuiLabel* pLabel = new nuiLabel(rLabel);
  if (!rObjectName.IsNull())
    pLabel->SetObjectName(rObjectName);
  nuiHBox::AddCell(pLabel, position);
  
  return pLabel;
}



void nuiHBox::RemoveCells(uint32 pos, uint32 cells)
{
  nuiGrid::RemoveColumns(pos, cells);
}


uint32 nuiHBox::GetNbCells() const
{
  return nuiGrid::GetNbColumns();
}  

bool nuiHBox::Clear()
{
  nuiSize spacing = nuiGrid::GetRowSpacing(0);
  nuiSize growRatio = nuiGrid::GetRowGrowRatio(0);
  nuiSize shrinkRatio = nuiGrid::GetRowShrinkRatio(0);
  nuiSize maxGrow = nuiGrid::GetRowMaxGrow(0);
  nuiSize maxShrink = nuiGrid::GetRowMaxShrink(0);
  nuiSize maxPixels = nuiGrid::GetRowMaxPixels(0);
  nuiSize minPixels = nuiGrid::GetRowMinPixels(0);

  bool res = nuiGrid::Clear();
  nuiGrid::AddRows(0,1);

  nuiGrid::SetRowSpacing(0, spacing);
  if (growRatio > 0.f)
    nuiGrid::SetRowExpand(0, nuiExpandGrow, growRatio);
  if (shrinkRatio > 0.f)
    nuiGrid::SetRowExpand(0, nuiExpandShrink, shrinkRatio);
  if (maxGrow >= 1.f)
    nuiGrid::SetRowMaxGrow(0, maxGrow);
  if (maxShrink >= 0.f)
    nuiGrid::SetRowMaxShrink(0, maxShrink);
  if (maxPixels >= 0.f)
    nuiGrid::SetRowMaxPixels(0, maxPixels);
  if (minPixels >= 0.f)
    nuiGrid::SetRowMinPixels(0, minPixels);

  return res;
}

nuiSize nuiHBox::GetDefaultSpacing() const
{
  return nuiGrid::GetDefaultVSpacing();
}

void nuiHBox::SetDefaultSpacing(nuiSize set)
{
  nuiGrid::SetDefaultVSpacing(set);
}

void nuiHBox::SetCellPixels(uint32 column, nuiSize size)
{
  nuiGrid::SetColumnPixels(column, size);
}

void nuiHBox::ResetCellPixels(uint32 column)
{
  nuiGrid::ResetColumnPixels(column);
}

nuiSize nuiHBox::GetCellPixels(uint32 cell) const
{
  return nuiGrid::GetColumnPixels(cell);
}


void nuiHBox::SetSize(nuiSize size)
{
  nuiGrid::SetRowPixels(0, size);
}


void nuiHBox::SetEqualizeCells(bool set)
{
  SetEqualizeColumns(set);
}

bool nuiHBox::GetEqualizeCells() const
{
  return nuiGrid::GetEqualizeColumns();
}

