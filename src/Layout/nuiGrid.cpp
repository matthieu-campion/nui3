/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nglString.h"
#include "nuiApplication.h"
#include "nuiDrawContext.h"
#include "nuiGrid.h"

#define NUI_DEFAULT_COLUMN_EXPAND_GROW   0
#define NUI_DEFAULT_COLUMN_EXPAND_SHRINK 0
#define NUI_DEFAULT_ROW_EXPAND_GROW   0
#define NUI_DEFAULT_ROW_EXPAND_SHRINK 0

nuiGrid::nuiGrid(uint nbcolumns, uint nbrows)
: nuiSimpleContainer()
{
  if (SetObjectClass(_T("nuiGrid")))
    InitAttributes();

  mDefaultHSpacing = 0.0f;
  mDefaultVSpacing = 0.0f;

  Reset(nbcolumns, nbrows, false);
}


void nuiGrid::InitAttributes()
{
  // VerticalSpacing in "common sens" corresponds to DefaultHSpacing in "nuiGrid sens". It's better for css understanding
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("VerticalSpacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiGrid::GetDefaultHSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiGrid::SetDefaultHSpacing)));                
                
  // HorizontalSpacing in "common sens" corresponds to DefaultVSpacing in "nuiGrid sens". It's better for css understanding
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("HorizontalSpacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiGrid::GetDefaultVSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiGrid::SetDefaultVSpacing)));                

  // Spacing is a shorcut for both HorizontalSpacing and VerticalSpacing
  AddAttribute(new nuiAttribute<nuiSize>
   (nglString(_T("Spacing")), nuiUnitSize,
    nuiAttribute<nuiSize>::GetterDelegate(this, &nuiGrid::GetDefaultSpacing),
    nuiAttribute<nuiSize>::SetterDelegate(this, &nuiGrid::SetDefaultSpacing)));                
}


bool nuiGrid::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  mDefaultHSpacing = 0.0f;
  mDefaultVSpacing = 0.0f;
  mNbColumns = nuiGetVal(pNode, _T("NbColumns"), 0);
  mNbRows = nuiGetVal(pNode, _T("NbRows"), 0);

  Reset(mNbColumns, mNbRows, false);
  
  return true;
}

inline void nuiGrid::Reset(uint nbcolumns, uint nbrows, bool clear)
{
  if (clear)
    Clear(false);

  mNbColumns = nbcolumns;
  mNbRows = nbrows;

  
  mDisplayGridBorder = false;
  mGridBorderSize = 0.f;
  mDisplayWidgetBoundingRect = false;
  mPlaceInvisibleWidgets = false;


  mGrid.resize(mNbColumns);
  for (uint c = 0; c < mNbColumns; c++)
  {
    mVGaps.push_back(mDefaultVSpacing); 
    mGrid[c].resize(mNbRows, NULL);
  }
  for (uint r = 0; r < mNbRows; r++)
  {
    mHGaps.push_back(mDefaultHSpacing);
  }

  mRowHeights.resize(mNbRows, 0);
  mColumnWidths.resize(mNbColumns, 0);

  mExpandGrowColumnsCoeff = 1.f;
  mExpandShrinkColumnsCoeff = 1.f;
  mExpandGrowRowsCoeff = 1.f;
  mExpandShrinkRowsCoeff = 1.f;

  mExpandGrowColumns.resize(mNbColumns, NUI_DEFAULT_COLUMN_EXPAND_GROW);
  mExpandShrinkColumns.resize(mNbColumns, NUI_DEFAULT_COLUMN_EXPAND_SHRINK);

  mExpandGrowRows.resize(mNbRows, NUI_DEFAULT_ROW_EXPAND_GROW);
  mExpandShrinkRows.resize(mNbRows, NUI_DEFAULT_ROW_EXPAND_SHRINK);

  mMaxGrowColumns.resize(mNbColumns, 0.f);
  mMaxShrinkColumns.resize(mNbColumns, 0.f);

  mMaxGrowRows.resize(mNbRows, 0.f);
  mMaxShrinkRows.resize(mNbRows, 0.f);

  mMaximumColumnSizes.resize(mNbColumns, -1.f);
  mMinimumColumnSizes.resize(mNbColumns, 0.f);

  mMaximumRowSizes.resize(mNbRows, -1.f);
  mMinimumRowSizes.resize(mNbRows, 0.f);
  
  mRowVisible.resize(mNbRows, true);


  UpdateExpandRatio(mExpandGrowRows, mExpandGrowRowsCoeff);
  UpdateExpandRatio(mExpandShrinkRows, mExpandShrinkRowsCoeff);

  UpdateExpandRatio(mExpandShrinkColumns, mExpandShrinkColumnsCoeff);
  UpdateExpandRatio(mExpandGrowColumns, mExpandGrowColumnsCoeff);
}

nuiGrid::~nuiGrid()
{
}

void nuiGrid::DisplayGridBorder(bool enable, nuiSize gridBorderSize)
{
  mDisplayGridBorder = enable;
  if (!enable)
    mGridBorderSize = 0.f;
  else
    mGridBorderSize = gridBorderSize;

  InvalidateLayout();
}

void nuiGrid::DisplayWidgetBoundingRect(bool enable)
{
  mDisplayWidgetBoundingRect = enable;
  Invalidate();
}

void nuiGrid::PlaceInvisibleWidgets(bool enable)
{
  mPlaceInvisibleWidgets = enable;
  Invalidate();
}

nuiRect nuiGrid::CalcIdealSize()
{
  nuiSize width = 0.f;
  nuiSize height = 0.f;

  for (uint r = 0; r < mNbRows; r++)
  {
    if (!mRowVisible[r])
      continue;
      
    nuiSize max = 0.f;

    for (uint c = 0; c < mNbColumns; c++)
    {
      nuiWidget* pWidget = mGrid[c][r];
      if (pWidget != NULL)
      {
        if (mPlaceInvisibleWidgets || (pWidget->IsVisible(false)))
        {
          const nuiRect &rect = pWidget->GetIdealRect();
          NGL_ASSERT(&rect);
          max = MAX(max, rect.GetHeight());
        }
      }
    }

    if (mMaximumRowSizes[r] >= 0.f && max > mMaximumRowSizes[r])
      max = mMaximumRowSizes[r];
    if ( max < mMinimumRowSizes[r])
      max = mMinimumRowSizes[r];

    mRowHeights[r] = max;
    height += max + 2 * mHGaps[r] + mGridBorderSize;
  }
  height += mGridBorderSize;


  for (uint c = 0; c < mNbColumns; c++)
  {
    nuiSize max = 0.f;
    for (uint r = 0; r < mNbRows; r++)
    {
      if (!mRowVisible[r])
        continue;
        
      nuiWidget* pWidget = mGrid[c][r];
      if (pWidget != NULL)
      {
        if (mPlaceInvisibleWidgets || (pWidget->IsVisible(false)))
        {
          const nuiRect &rect = pWidget->GetIdealRect();
          max = MAX(max, rect.GetWidth());
        }
      }
    }
    
    if (mMaximumColumnSizes[c] >= 0.f && max > mMaximumColumnSizes[c])
      max = mMaximumColumnSizes[c];
    
    if ( max < mMinimumColumnSizes[c])
      max = mMinimumColumnSizes[c];

    mColumnWidths[c] = max;
    width += max + 2 * mVGaps[c] + mGridBorderSize;
  }
  width += mGridBorderSize;

  mSizeRect.Set(0.f, 0.f, width, height);

  return mSizeRect;
}

void nuiGrid::AdjustToExpand(nuiSize width, nuiSize height, std::vector<nuiSize>& Widths, std::vector<nuiSize>& Heights)
{
  Widths.clear();
  Heights.clear();

  nuiSize width_step = 0.f;
  nuiSize height_step = 0.f;

  if (width > 0.f)
    width_step = width / mExpandGrowColumnsCoeff;
  else if (width < 0.f)
    width_step = width / mExpandShrinkColumnsCoeff;

  if (height > 0.f) 
    height_step = height / mExpandGrowRowsCoeff;
  else if (height < 0.f)
    height_step = height / mExpandShrinkRowsCoeff;

  nuiSize w;
  for (uint c = 0; c < mNbColumns; c++)
  {
    w = mColumnWidths[c];
    if (width_step > 0.f)    
    {
      w += mExpandGrowColumns[c] * width_step;
      nuiSize w_tmp;
      w_tmp = mColumnWidths[c] * mMaxGrowColumns[c];
      if (w_tmp && w > w_tmp)
        w = w_tmp;
    }
    else if (width_step < 0.f)
    {
      w += mExpandShrinkColumns[c] * width_step;
      nuiSize w_tmp;
      w_tmp = mColumnWidths[c] * mMaxShrinkColumns[c];
      if (w < w_tmp)
        w = w_tmp;
    }
    if (mMaximumColumnSizes[c] >= 0 && w > mMaximumColumnSizes[c])
      w = mMaximumColumnSizes[c];
    else if (w < mMinimumColumnSizes[c])
      w = mMinimumColumnSizes[c];
    Widths.push_back(w);
  }

  nuiSize h;
  for (uint r = 0; r < mNbRows; r++)
  {
    h = mRowHeights[r];
    if (height_step > 0.f)
    {
      h += mExpandGrowRows[r] * height_step;
      nuiSize h_tmp;
      h_tmp = mRowHeights[r] * mMaxGrowRows[r];
      if (h_tmp && h > h_tmp)
        h = h_tmp;
    }
    else if (height_step < 0.f)
    {
      h += mExpandShrinkRows[r] * height_step;
      nuiSize h_tmp;
      h_tmp = mRowHeights[r] * mMaxShrinkRows[r];
      if (h < h_tmp)
        h = h_tmp;
    }
    if (mMaximumRowSizes[r] >= 0 && h > mMaximumRowSizes[r])
      h = mMaximumRowSizes[r];
    else if (h < mMinimumRowSizes[r])
      h = mMinimumRowSizes[r];
    Heights.push_back(h);
  }
}

bool nuiGrid::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  
  GetIdealRect(); // ensure IdealRect has happened

  nuiSize width = rRect.GetWidth() - mSizeRect.GetWidth();
  nuiSize height = rRect.GetHeight() - mSizeRect.GetHeight();

  nuiSize X = 0.f, Y = 0.f;
  uint row = 0, col = 0;

  std::vector< std::vector<nuiWidget*> >::const_iterator col_it;
  std::vector<nuiWidget*>::const_iterator row_it;

  AdjustToExpand(width, height, mWidths, mHeights);

  nuiSize sizeX = 0.f;
  nuiSize sizeY = 0.f;

  uint cpt = 0;
  std::vector< std::vector<nuiWidget* > >::const_iterator col_end = mGrid.end();
  for (col_it = mGrid.begin(); col_it != col_end; ++col_it, col++)
  {
    X += mVGaps[col] + mGridBorderSize;
    Y = 0.f;
    row = 0;

    sizeX = mWidths[col];
    
    // if (GetDebug())
    //     printf("  nuiGrid::SetRect [0x%x '%ls'] - column %d size: %f\n", this, GetProperty(_T("Name")).GetChars(), col, sizeX);
    //NGL_OUT(_T("  nuiGrid::SetRect [0x%x '%ls'] - row %d column %d size: %f\n"), this, GetProperty(_T("Name")).GetChars(), row, col, sizeX);
    
    std::vector<nuiWidget*>::const_iterator row_end = (*col_it).end();
    for (row_it = (*col_it).begin(); row_it != row_end; ++row_it)
    {
      if (!mRowVisible[row])
        continue;
      
      Y += mHGaps[row] + mGridBorderSize;

      sizeY = mHeights[row];
      
      nuiWidget* pWidget = (*row_it);
      
      if (pWidget)
      {
        nuiRect rect(X, Y, sizeX, sizeY);

        pWidget->GetIdealRect();
        pWidget->SetLayout(rect);
      }
      Y += mHGaps[row] + sizeY;

      row++;
    }
    cpt++;
    X += mVGaps[col] + sizeX;
  }
  
  return true;
}

void nuiGrid::ClearCells(bool trash)
{
  for (uint c = 0; c < mNbColumns; c++)
  {
    for (uint r = 0; r < mNbRows; r++)
    {
      if (mGrid[c][r] != NULL)
      {
        nuiGrid::DelChild(mGrid[c][r], trash);
        mGrid[c][r] = NULL;
      }
    }
  }
}


bool nuiGrid::SetCell(uint column, uint row, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting, bool TrashExisting)
{
  NGL_ASSERT(column < mNbColumns);
  NGL_ASSERT(row < mNbRows);

  nuiWidget* pOldWidget = mGrid[column][row];
  if (pOldWidget)
  {
    if (!ReplaceExisting)
      return false;

    mGrid[column][row] = NULL;

    if (TrashExisting)
      pOldWidget->Trash();
    else
      nuiGrid::DelChild(pOldWidget, false);
      
  }
  mGrid[column][row] = pWidget;

  if (pWidget)
  {
    pWidget->SetPosition(position);
    nuiGrid::AddChild(pWidget);
  }
  
  InvalidateLayout();

  return true;
}

bool nuiGrid::SetCellToken(uint col, uint row, nuiTokenBase* pToken)
{
  nuiWidget* pWidget = GetCell(col, row);
  if (!pWidget)
    return false;
    
  pWidget->SetToken(pToken);
  return true;
}



void nuiGrid::SetCellPosition(uint column, uint row, nuiPosition position)
{
  NGL_ASSERT(column < mNbColumns);
  NGL_ASSERT(row < mNbRows);

  nuiWidget* pWidget = mGrid[column][row];
  if (pWidget)
    pWidget->SetPosition(position);
}
 
bool nuiGrid::FindCell(nuiWidgetPtr pWidget, uint& rCol, uint& rRow) const 
{
  for (uint c = 0; c < mNbColumns; c++)
  {
    for (uint r = 0; r < mNbRows; r++)
    {
      if (mGrid[c][r] == pWidget)
      {
        rCol = c;
        rRow = r;
        return true;
      }
    }
  }
  return false;
}

nuiWidget* nuiGrid::GetCell(uint column, uint row) const
{
  NGL_ASSERT(column < mNbColumns);
  NGL_ASSERT(row < mNbRows);

  return ((mGrid[column])[row]); //This can be 0x0
}


nuiTokenBase* nuiGrid::GetCellToken(uint col, uint row) const
{
  nuiWidget* pWidget = GetCell(col, row);
  if (!pWidget)
    return NULL;
    
  return pWidget->GetToken();
}



void nuiGrid::SetColumnSpacing (uint column, nuiSize vgap)
{
  NGL_ASSERT(column < mNbColumns);
  mVGaps[column] = vgap;

  InvalidateLayout();
}

void nuiGrid::SetRowSpacing    (uint row, nuiSize hgap)
{
  NGL_ASSERT(row < mNbRows);
  mHGaps[row] = hgap;

  InvalidateLayout();
}

void nuiGrid::SetGridSize(uint columns, uint rows)
{
  Reset(columns, rows, false);
}

void nuiGrid::SetColumnExpand(uint col, nuiExpandMode mode, nuiSize ratio)
{
  NGL_ASSERT(ratio >= 1.f);
  NGL_ASSERT(col < mNbColumns);

  switch (mode)
  {
  case nuiExpandGrow:
    mExpandGrowColumns[col] = ratio;
    UpdateExpandRatio(mExpandGrowColumns, mExpandGrowColumnsCoeff);
    break;
  case nuiExpandShrink:
    mExpandShrinkColumns[col] = ratio;
    UpdateExpandRatio(mExpandShrinkColumns, mExpandShrinkColumnsCoeff);
    break;
  case nuiExpandShrinkAndGrow:
    mExpandGrowColumns[col] = ratio;
    UpdateExpandRatio(mExpandGrowColumns, mExpandGrowColumnsCoeff);
    mExpandShrinkColumns[col] = ratio;
    UpdateExpandRatio(mExpandShrinkColumns, mExpandShrinkColumnsCoeff);
    break;
  case nuiExpandFixed:
    mExpandGrowColumns[col] = 0.f;
    mExpandShrinkColumns[col] = 0.f;
    UpdateExpandRatio(mExpandGrowColumns, mExpandGrowColumnsCoeff);
    UpdateExpandRatio(mExpandShrinkColumns, mExpandShrinkColumnsCoeff);
    break;
  }

  InvalidateLayout();
}

void nuiGrid::SetRowExpand(uint row, nuiExpandMode mode, nuiSize ratio)
{
  NGL_ASSERT(ratio >= 1.f);
  NGL_ASSERT(row < mNbRows);

  switch (mode)
  {
  case nuiExpandGrow:
    mExpandGrowRows[row] = ratio;
    UpdateExpandRatio(mExpandGrowRows, mExpandGrowRowsCoeff);
    break;
  case nuiExpandShrink:
    mExpandShrinkRows[row] = ratio;
    UpdateExpandRatio(mExpandShrinkRows, mExpandShrinkRowsCoeff);
    break;
  case nuiExpandShrinkAndGrow:
    mExpandGrowRows[row] = ratio;
    UpdateExpandRatio(mExpandGrowRows, mExpandGrowRowsCoeff);
    mExpandShrinkRows[row] = ratio;
    UpdateExpandRatio(mExpandShrinkRows, mExpandShrinkRowsCoeff);
    break;
  case nuiExpandFixed:
    mExpandShrinkRows[row] = 0.f;
    mExpandGrowRows[row] = 0.f;
    UpdateExpandRatio(mExpandShrinkRows, mExpandShrinkRowsCoeff);
    UpdateExpandRatio(mExpandGrowRows, mExpandGrowRowsCoeff);
    break;
  }

  InvalidateLayout();
}

void nuiGrid::UpdateExpandRatio(std::vector<nuiSize>& rRatios, nuiSize& rCoeff)
{
  rCoeff = 0.f;
  std::vector<nuiSize>::const_iterator end = rRatios.end();
  for (std::vector<nuiSize>::const_iterator it = rRatios.begin(); it != end; ++it)
    rCoeff += *it;

  if (!rCoeff)
    rCoeff = 1.f;  
}

void nuiGrid::SetRowMaxGrow(uint row, nuiSize ratio)
{
  NGL_ASSERT(row < mNbRows);
  NGL_ASSERT(ratio >= 1.f)

    mMaxGrowRows[row] = ratio;
  
  InvalidateLayout();
}

void nuiGrid::SetColumnMaxGrow(uint col, nuiSize ratio)
{
  NGL_ASSERT(col < mNbColumns);
  NGL_ASSERT(ratio >= 1.f);

  mMaxGrowColumns[col] = ratio;
  
  InvalidateLayout();
}

void nuiGrid::SetRowMaxShrink(uint row, nuiSize ratio)
{
  NGL_ASSERT(row < mNbRows);
  NGL_ASSERT(ratio <= 1.f)
  NGL_ASSERT(ratio >= 0.f)

  mMaxShrinkRows[row] = ratio;

  InvalidateLayout();
}

void nuiGrid::SetColumnMaxShrink(uint col, nuiSize ratio)
{
  NGL_ASSERT(col < mNbColumns);
  NGL_ASSERT(ratio <= 1.f);
  NGL_ASSERT(ratio >= 0.f);

  mMaxShrinkColumns[col] = ratio;
  
  InvalidateLayout();
}

void nuiGrid::SetRowMaxPixels(uint row, nuiSize size)
{
  NGL_ASSERT(row < mNbRows);
  NGL_ASSERT(size >= 0.f);

  mMaximumRowSizes[row] = size;

  InvalidateLayout();
}



void nuiGrid::SetColumnMaxPixels(uint col, nuiSize size)
{
  NGL_ASSERT(col < mNbColumns);
  NGL_ASSERT(size >= 0.f);

  mMaximumColumnSizes[col] = size;

  InvalidateLayout();
}



void nuiGrid::SetRowMinPixels(uint row, nuiSize size)
{
  NGL_ASSERT(row < mNbRows);
  NGL_ASSERT(size >= 0.f);

  mMinimumRowSizes[row] = size;  

  InvalidateLayout();
}




void nuiGrid::SetColumnMinPixels(uint col, nuiSize size)
{
  NGL_ASSERT(col < mNbColumns);
  NGL_ASSERT(size >= 0.f);

  mMinimumColumnSizes[col] = size;

  InvalidateLayout();
}





void nuiGrid::SetRowPixels(uint row, nuiSize size)
{
  NGL_ASSERT(row < mNbRows);
  NGL_ASSERT(size >= 0.f);
  
  mMinimumRowSizes[row] = size;  
  mMaximumRowSizes[row] = size;  
  
  InvalidateLayout();
}


void nuiGrid::SetColumnPixels(uint column, nuiSize size)
{
  NGL_ASSERT(column < mNbColumns);
  NGL_ASSERT(size >= 0.f);
  
  mMinimumColumnSizes[column] = size;  
  mMaximumColumnSizes[column] = size;  
  
  InvalidateLayout();
}


void nuiGrid::ResetRowPixels(uint row)
{
  mMaximumRowSizes[row] = -1.0f;  
  mMinimumRowSizes[row] = -1.0f;
  InvalidateLayout();  
}

void nuiGrid::ResetColumnPixels(uint row)
{
  mMaximumColumnSizes[row] = -1.0f;  
  mMinimumColumnSizes[row] = -1.0f;
  InvalidateLayout();  
}






///////

nuiSize nuiGrid::GetColumnSpacing(uint column) const
{
  NGL_ASSERT(column < mNbColumns);
  return mVGaps[column];
}

nuiSize nuiGrid::GetRowSpacing(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mHGaps[row];
}

void nuiGrid::GetGridSize(uint& columns, uint& rows) const
{
  columns = mNbColumns;
  rows = mNbRows;
}

nuiSize nuiGrid::GetColumnGrowRatio(uint col) const
{
  NGL_ASSERT(col < mNbColumns);
  return mExpandGrowColumns[col];
}

nuiSize nuiGrid::GetColumnShrinkRatio(uint col) const
{
  NGL_ASSERT(col < mNbColumns);
  return mExpandShrinkColumns[col];
}

nuiSize nuiGrid::GetRowGrowRatio(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mExpandGrowRows[row];
}

nuiSize nuiGrid::GetRowShrinkRatio(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mExpandShrinkRows[row];
}


nuiSize nuiGrid::GetRowMaxGrow(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mMaxGrowRows[row];
}

nuiSize nuiGrid::GetColumnMaxGrow(uint col) const
{
  NGL_ASSERT(col < mNbColumns);
  return mMaxGrowColumns[col];
}

nuiSize nuiGrid::GetRowMaxPixels(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mMaximumRowSizes[row];
}

nuiSize nuiGrid::GetColumnMaxPixels(uint col) const
{
  NGL_ASSERT(col < mNbColumns);
  return mMaximumColumnSizes[col];
}


nuiSize nuiGrid::GetRowMaxShrink(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mMaxShrinkRows[row];
}

nuiSize nuiGrid::GetColumnMaxShrink(uint col) const
{
  NGL_ASSERT(col < mNbColumns);
  return mMaxShrinkColumns[col];
}

nuiSize nuiGrid::GetRowMinPixels(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mMinimumRowSizes[row];  
}

nuiSize nuiGrid::GetColumnMinPixels(uint col) const
{
  NGL_ASSERT(col < mNbColumns);
  return mMinimumColumnSizes[col];
}



/////////// cells visibility

void nuiGrid::SetCellVisible(uint col, uint row, bool set)
{
  nuiWidget* pWidget = GetCell(col, row);
  pWidget->SetVisible(set);
  pWidget->SetEnabled(set);
}

bool nuiGrid::IsCellVisible(uint col, uint row) const
{
  nuiWidget* pWidget = GetCell(col, row);
  return pWidget->IsVisible();  
}

void nuiGrid::SetRowVisible(uint row, bool set)
{
  NGL_ASSERT(row < mNbRows);
  mRowVisible[row] = set;
  
  for (uint col=0; col < mNbColumns; col++)
    SetCellVisible(col, row, set);
}

bool nuiGrid::IsRowVisible(uint row) const
{
  NGL_ASSERT(row < mNbRows);
  return mRowVisible[row];
}







///////
void nuiGrid::AddRows(uint pos, uint rows)
{
  NGL_ASSERT(pos <= mNbRows);
  NGL_ASSERT(rows > 0);

  mNbRows += rows;
  std::vector<nuiWidget*>::iterator begin;
  for (uint c = 0; c < mNbColumns; c++)
  {
    begin = mGrid[c].begin() + pos;
    mGrid[c].insert(begin, rows, NULL);
  }
 
  std::vector<nuiSize>::iterator it;
  it = mHGaps.begin()+ pos;
  mHGaps.insert(it, rows, mDefaultHSpacing);

  it = mRowHeights.begin()+ pos;
  mRowHeights.insert(it, rows, 0);

  it = mExpandGrowRows.begin()+ pos;
  mExpandGrowRows.insert(it, rows, NUI_DEFAULT_ROW_EXPAND_GROW);

  it = mExpandShrinkRows.begin()+ pos;
  mExpandShrinkRows.insert(it, rows, NUI_DEFAULT_ROW_EXPAND_SHRINK);

  it = mMaxGrowRows.begin()+ pos;
  mMaxGrowRows.insert(it, rows, 0.f);

  it = mMaxShrinkRows.begin()+ pos;
  mMaxShrinkRows.insert(it, rows, 0.f);

  it = mMaximumRowSizes.begin()+ pos;
  mMaximumRowSizes.insert(it, rows, -1.f);
  
  it = mMinimumRowSizes.begin()+ pos;
  mMinimumRowSizes.insert(it, rows, 0.f);
  
  
  std::vector<bool>::iterator it2;
  it2 = mRowVisible.begin() + pos;
  mRowVisible.insert(it2, rows, true);
  

  UpdateExpandRatio(mExpandGrowRows, mExpandGrowRowsCoeff);
  UpdateExpandRatio(mExpandShrinkRows, mExpandShrinkRowsCoeff);

  InvalidateLayout();
}

void nuiGrid::RemoveRows(uint pos, uint rows)
{
  NGL_ASSERT(pos < mNbRows);
  NGL_ASSERT(rows >= 0);

  mNbRows -= rows;
  std::vector<nuiWidget*>::iterator begin;
  std::vector<nuiWidget*>::iterator end;
  for (uint c = 0; c < mNbColumns; c++)
  {
    std::vector<nuiWidget*>::iterator erasor = begin = mGrid[c].begin() + pos;
    end   = mGrid[c].begin() + pos + rows;
    while (erasor != end)
    {
      nuiWidget* pWidget = *erasor;
      if (pWidget)
        pWidget->Trash();
      erasor++;
    }
    mGrid[c].erase(begin, end);
  }
 
  std::vector<nuiSize>::iterator it;
  std::vector<nuiSize>::iterator it_end;
  
  it = mHGaps.begin()+ pos;
  it_end = mHGaps.begin()+ pos + rows;
  mHGaps.erase(it, it_end);

  it = mRowHeights.begin()+ pos;
  it_end = mRowHeights.begin()+ pos + rows;
  mRowHeights.erase(it, it_end);

  it = mExpandGrowRows.begin()+ pos;
  it_end = mExpandGrowRows.begin()+ pos + rows;
  mExpandGrowRows.erase(it, it_end);

  it = mExpandShrinkRows.begin()+ pos;
  it_end = mExpandShrinkRows.begin()+ pos + rows;
  mExpandShrinkRows.erase(it, it_end);

  it = mMaxGrowRows.begin()+ pos;
  it_end = mMaxGrowRows.begin()+ pos + rows;
  mMaxGrowRows.erase(it, it_end);

  it = mMaxShrinkRows.begin()+ pos;
  it_end = mMaxShrinkRows.begin()+ pos + rows;
  mMaxShrinkRows.erase(it, it_end);

  it = mMaximumRowSizes.begin()+ pos;
  it_end = mMaximumRowSizes.begin()+ pos + rows;
  mMaximumRowSizes.erase(it, it_end);
  
  it = mMinimumRowSizes.begin()+ pos;
  it_end = mMinimumRowSizes.begin()+ pos + rows;
  mMinimumRowSizes.erase(it, it_end);

  std::vector<bool>::iterator it2;
  std::vector<bool>::iterator it2_end;
  it2 = mRowVisible.begin() + pos;
  it2_end = mRowVisible.begin() + pos + rows;
  mRowVisible.erase(it2, it2_end);

  UpdateExpandRatio(mExpandGrowRows, mExpandGrowRowsCoeff);
  UpdateExpandRatio(mExpandShrinkRows, mExpandShrinkRowsCoeff);

  InvalidateLayout();
}



bool nuiGrid::Clear(bool Delete)
{
  bool res = nuiSimpleContainer::Clear(Delete);

  mGrid.clear();

  mNbRows = 0;
  mNbColumns = 0;
  mWidths.clear();
  mHeights.clear();
  mColumnWidths.clear();
  mRowHeights.clear();
  mVGaps.clear();
  mHGaps.clear();

  mExpandShrinkColumns.clear();
  mExpandShrinkRows.clear();
  mExpandGrowColumns.clear();
  mExpandGrowRows.clear();

  mMaxGrowRows.clear();
  mMaxGrowColumns.clear();
  mMaxShrinkRows.clear();
  mMaxShrinkColumns.clear();

  mMaximumRowSizes.clear();
  mMaximumColumnSizes.clear();
  mMinimumRowSizes.clear();
  mMinimumColumnSizes.clear();
  
  mRowVisible.clear();


  InvalidateLayout();
  
  return res;
}

bool nuiGrid::Draw(nuiDrawContext *pContext)
{
  if (mDisplayWidgetBoundingRect)
  {
    pContext->SetStrokeColor(GetColor(eBorder));
  }

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
     nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
    {
      //#FIXME #TEST
      nuiRect r = pItem->GetIdealRect();
      if (mDisplayWidgetBoundingRect)
      {
        pContext->DrawRect(pItem->GetRect(), eStrokeShape);
      }
      
      DrawChild(pContext, pItem);
    }
  }

  if (mDisplayGridBorder)
  {
    nuiShape shape;
    pContext->SetLineWidth(mGridBorderSize);
    pContext->SetStrokeColor(GetColor(eBorder));
    nuiSize widthSoFar = 0.f;
    for (uint i = 0; i < mWidths.size(); i++)
    {
      widthSoFar += mVGaps[i];

      nuiSize heightSoFar = 0.f;
      nuiSize width = mWidths[i];
      for (uint j = 0; j < mHeights.size(); j++)
      {
        if (!mRowVisible[j])
          continue;
          
        heightSoFar += mHGaps[j];

        nuiSize height = mHeights[j];
        nuiRect rect(widthSoFar, heightSoFar, width + mGridBorderSize, height + mGridBorderSize);
        shape.AddRect(rect);

        heightSoFar += height;
        heightSoFar += mHGaps[j] + mGridBorderSize;
      }
      widthSoFar += width;
      widthSoFar += mVGaps[i] + mGridBorderSize;
    }
    pContext->DrawShape(&shape, eStrokeShape);
  }

  delete pIt;
  return true;
}

void nuiGrid::AddColumns(uint pos, uint columns)
{
  NGL_ASSERT(pos <= mNbColumns);
  NGL_ASSERT(columns > 0);
  std::vector< std::vector<nuiWidget*> >::iterator col_it = mGrid.begin() + pos;
  uint c;
  for (c = 0; c < columns; c++)
  {
    std::vector<nuiWidget*> row;
    row.resize(mNbRows, NULL);
    col_it = mGrid.insert(col_it, row);
  }
  mNbColumns += columns;

  std::vector<nuiSize>::iterator it;
  it = mVGaps.begin()+ pos;
  mVGaps.insert(it, columns, mDefaultVSpacing);

  it = mColumnWidths.begin()+ pos;
  mColumnWidths.insert(it, columns, 0);

  it = mExpandGrowColumns.begin()+ pos;
  mExpandGrowColumns.insert(it, columns, NUI_DEFAULT_COLUMN_EXPAND_GROW);
  
  it = mExpandShrinkColumns.begin()+ pos;
  mExpandShrinkColumns.insert(it, columns, NUI_DEFAULT_COLUMN_EXPAND_SHRINK);

  it = mMaxGrowColumns.begin()+ pos;
  mMaxGrowColumns.insert(it, columns, 0.f);

  it = mMaxShrinkColumns.begin()+ pos;
  mMaxShrinkColumns.insert(it, columns, 0.f);

  it = mMaximumColumnSizes.begin()+ pos;
  mMaximumColumnSizes.insert(it, columns, -1.f);
  
  it = mMinimumColumnSizes.begin()+ pos;
  mMinimumColumnSizes.insert(it, columns, 0.f);

  UpdateExpandRatio(mExpandGrowColumns, mExpandGrowColumnsCoeff);
  UpdateExpandRatio(mExpandShrinkColumns, mExpandShrinkColumnsCoeff);

  InvalidateLayout();
}

void nuiGrid::RemoveColumns(uint pos, uint columns)
{

  NGL_ASSERT(pos < mNbColumns);
  NGL_ASSERT(columns >= 0);
  std::vector< std::vector<nuiWidget*> >::iterator col_it = mGrid.begin() + pos;
  uint c;
  for (c = 0; c < columns; c++)
  {
    std::vector<nuiWidget*>::iterator row = col_it->begin();
    std::vector<nuiWidget*>::iterator end = col_it->end();

    while (row != end)
    {
      nuiWidget* pWidget = *row;
      if (pWidget)
        pWidget->Trash(); //< Trash the current widget
      row++;
    }

    col_it = mGrid.erase(col_it);
  }
  
  mNbColumns -= columns;

  std::vector<nuiSize>::iterator it;
  std::vector<nuiSize>::iterator it_end;

  it = mVGaps.begin()+ pos;
  it_end = mVGaps.begin()+ pos + columns;
  mVGaps.erase(it, it_end);

  it = mColumnWidths.begin()+ pos;
  it_end = mColumnWidths.begin()+ pos + columns;
  mColumnWidths.erase(it, it_end);

  it = mExpandGrowColumns.begin()+ pos;
  it_end = mExpandGrowColumns.begin()+ pos + columns;
  mExpandGrowColumns.erase(it, it_end);

  it = mExpandShrinkColumns.begin()+ pos;
  it_end = mExpandShrinkColumns.begin()+ pos + columns;
  mExpandShrinkColumns.erase(it, it_end);

  it = mMaxGrowColumns.begin()+ pos;
  it_end = mMaxGrowColumns.begin()+ pos + columns;
  mMaxGrowColumns.erase(it, it_end);

  it = mMaxShrinkColumns.begin()+ pos;
  it_end = mMaxShrinkColumns.begin()+ pos + columns;
  mMaxShrinkColumns.erase(it, it_end);

  it = mMaximumColumnSizes.begin()+ pos;
  it_end = mMaximumColumnSizes.begin()+ pos + columns;
  mMaximumColumnSizes.erase(it, it_end);
  
  it = mMinimumColumnSizes.begin()+ pos;
  it_end = mMinimumColumnSizes.begin()+ pos + columns;
  mMinimumColumnSizes.erase(it, it_end);

  UpdateExpandRatio(mExpandGrowColumns, mExpandGrowColumnsCoeff);
  UpdateExpandRatio(mExpandShrinkColumns, mExpandShrinkColumnsCoeff);

  InvalidateLayout();
}

nuiSize nuiGrid::GetDefaultHSpacing() const
{
  return mDefaultHSpacing;
}

nuiSize nuiGrid::GetDefaultVSpacing() const
{
  return mDefaultVSpacing;
}

void nuiGrid::SetDefaultHSpacing(nuiSize set)
{
  mDefaultHSpacing = set;
  for (uint32 i=0; i < mNbRows; i++)
    SetRowSpacing(i, set);
  InvalidateLayout();
}

void nuiGrid::SetDefaultVSpacing(nuiSize set)
{
  mDefaultVSpacing = set;
  for (uint32 i=0; i < mNbColumns; i++)
    SetColumnSpacing(i, set);
  InvalidateLayout();
}

nuiSize nuiGrid::GetDefaultSpacing() const
{
  if (mDefaultVSpacing != mDefaultHSpacing)
    return 0;
    
  return mDefaultVSpacing;
}

void nuiGrid::SetDefaultSpacing(nuiSize set)
{
  SetDefaultHSpacing(set);
  SetDefaultVSpacing(set);
}

