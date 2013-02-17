/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

void nuiBox::InitBoxAttributes()
{
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("CellMaxGrow")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiBox::GetCellMaxGrow),
                nuiMakeDelegate(this, &nuiBox::SetCellMaxGrow),
                nuiMakeDelegate(this, &nuiBox::GetBoxDimensionRange)));

  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("CellMaxPixels")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiBox::GetCellMaxPixels),
                nuiMakeDelegate(this, &nuiBox::SetCellMaxPixels),
                nuiMakeDelegate(this, &nuiBox::GetBoxDimensionRange)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("CellMinPixels")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiBox::GetCellMinPixels),
                nuiMakeDelegate(this, &nuiBox::SetCellMinPixels),
                nuiMakeDelegate(this, &nuiBox::GetBoxDimensionRange)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("CellMaxShrink")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiBox::GetCellMaxShrink),
                nuiMakeDelegate(this, &nuiBox::SetCellMaxShrink),
                nuiMakeDelegate(this, &nuiBox::GetBoxDimensionRange)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("CellPixels")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiBox::GetCellPixels),
                nuiMakeDelegate(this, &nuiBox::SetCellPixels),
                nuiMakeDelegate(this, &nuiBox::GetBoxDimensionRange)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("EqualizeCells")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiBox::GetEqualizeCells),
                nuiMakeDelegate(this, &nuiBox::SetEqualizeCells)));                
  
  AddAttribute(new nuiAttribute<nuiExpandMode>
               (nglString(_T("Expand")), nuiUnitExpand,
                nuiMakeDelegate(this, &nuiBox::GetExpand),
                nuiMakeDelegate(this, &nuiBox::SetExpand)));                
  
  AddAttribute(new nuiAttribute<nuiExpandMode>
               (nglString(_T("CellExpand")), nuiUnitExpand,
                nuiMakeDelegate(this, &nuiBox::GetCellExpand),
                nuiMakeDelegate(this, &nuiBox::_SetCellExpand),
                nuiMakeDelegate(this, &nuiBox::GetBoxDimensionRange)));
  
}

void nuiBox::_SetCellExpand(uint32 cell, nuiExpandMode mode)
{
  SetCellExpand(cell, mode);
}

uint32 nuiBox::GetBoxDimensionRange(uint32 dimension) const
{
  if (dimension == 0)
    return GetNbCells();
  return 0;
}

nuiBox::~nuiBox()
{
}

nuiBox::nuiBox(uint32 nbcellsH, uint32 nbcellsV)
: nuiGrid(nbcellsH, nbcellsV)
{
}

