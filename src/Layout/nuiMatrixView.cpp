/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiMatrixViewFormater nuiMatrixView::mDefaultFormater;
nuiRange nuiMatrixView::mDefaultRange(0.0f, 0.0f, 100.0f, 1.0f, 10.0f, 0.0f);




nuiSize nuiMatrixView::mDefaultSensitivity = 10;
nglKeyCode nuiMatrixView::mGrabMouseKey = NK_LMOD;
nglKeyCode nuiMatrixView::mRelativeKey = NK_LALT;
nglKeyCode nuiMatrixView::mFineSensitivityKey = NK_LSHIFT;



//***********************************************************************************************************************
//***********************************************************************************************************************
//
// nuiMatrixViewFormater
//
//***********************************************************************************************************************
//***********************************************************************************************************************

nuiMatrixViewFormater::nuiMatrixViewFormater()
  : nuiFormater()
{
}


nuiMatrixViewFormater::~nuiMatrixViewFormater()
{
}


bool nuiMatrixViewFormater::Format (double dValue, nglString& rResult) const
{
  rResult.CFormat(_T("%.0f"), dValue);
  return true;
}


double nuiMatrixViewFormater::GetCDouble (const nglString& text) const
{
  return text.GetCDouble();
}




//***********************************************************************************************************************
//***********************************************************************************************************************
//
// nuiMatrixViewItem
//
//***********************************************************************************************************************
//***********************************************************************************************************************

nuiMatrixViewItem::nuiMatrixViewItem(nuiMatrixView* pParent, uint col, uint row, const nuiRange& rRange, const nuiFormater& rFormater)
  : nuiFormatedLabel(rFormater, rRange.GetValue(), nuiTheme::Default), mpParent(pParent), mDesc(col, row, rRange)
{
  SetObjectClass(_T("nuiMatrixViewItem"));
  mpParent->GetItemDesc(mDesc);
}


nuiMatrixViewItem::~nuiMatrixViewItem()
{

}

  
void nuiMatrixViewItem::SetValue(double value)
{
  mDesc.SetValue(value);
  nuiFormatedLabel::SetValue(mDesc.GetValue());

  mpParent->GetItemDesc(mDesc);
  
}  

void nuiMatrixViewItem::SetRelativeValue(double delta)
{
  double value = mDesc.GetValue();

  mDesc.SetValue(value +delta);
  double newvalue = mDesc.GetValue();
  nuiFormatedLabel::SetValue(newvalue);

  mpParent->GetItemDesc(mDesc);

}  
  
  
  
double nuiMatrixViewItem::GetValue()
{
  return mDesc.GetValue();
}


uint nuiMatrixViewItem::GetCol()
{
  return mDesc.GetColumn();
}  
  
uint nuiMatrixViewItem::GetRow()
{
  return mDesc.GetRow();
}

nuiRange&  nuiMatrixViewItem::GetRange()
{ 
  return mRange;
}

const nuiMatrixViewItemDesc& nuiMatrixViewItem::GetDesc() const
{
  return mDesc;
}





bool nuiMatrixViewItem::Draw(nuiDrawContext* pContext)
{
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);

  CalcLayout();

  nuiRect rect = mIdealLayoutRect;

  rect.SetPosition(GetPosition(), mRect.Size());

  pContext->SetFillColor(mDesc.GetFillColor());
  pContext->SetStrokeColor(mDesc.GetStrokeColor());
  pContext->DrawRect(mRect.Size(),eStrokeAndFillShape);
  
  nuiFormatedLabel::Draw(pContext);

  return true;
}


//***********************************************************************************************************************
//***********************************************************************************************************************
//
// nuiMatrixView
//
//***********************************************************************************************************************
//***********************************************************************************************************************


nuiMatrixView::nuiMatrixView (uint nbcolumns, uint nbrows, bool EnableColumnHeader, bool EnableRowHeader,
                              const nuiFormater& rFormater, const nuiRange& rRange,
                              nuiSize defaultHSpacing, nuiSize defaultVSpacing)
: nuiGrid(nbcolumns, nbrows),
  nglTimer(0.5), // 0.5 seconds for the wheel up/down timer handler
  mFormater(rFormater),
  mRange(rRange),
  mEventSink(this)
  
{
  SetObjectClass(_T("nuiMatrixView"));
  
  if (defaultHSpacing || defaultVSpacing)
  {
    nuiGrid::SetDefaultHSpacing(defaultHSpacing);
    nuiGrid::SetDefaultVSpacing(defaultVSpacing);
    nuiGrid::Reset(nbcolumns, nbrows);
  }

  mColumnHeader = EnableColumnHeader;
  mRowHeader = EnableRowHeader;
  mClicked = false;
  mCanChange = false;
  mChanged=false;
  mInited =false;

	NUI_ADD_EVENT(SelectionChanged);
	NUI_ADD_EVENT(ValueChanged);
	NUI_ADD_EVENT(InteractiveValueChanged);
  
  uint k;


  mCanSelectAll = false;
  mAllSelected = false;
  mRowSelected.resize(nbrows);
  mColumnSelected.resize(nbcolumns);
  
  for (k=0; k < nbrows; k++)
    mRowSelected[k]=false;
    
  for (k=0; k < nbcolumns; k++)
    mColumnSelected[k]=false;


}


nuiMatrixView::~nuiMatrixView()
{
  
}

// static
nglKeyCode nuiMatrixView::GetGrabMouseKey()
{
  return mGrabMouseKey;
}

nglKeyCode nuiMatrixView::GetRelativeKey()
{
  return mRelativeKey;
}

nglKeyCode nuiMatrixView::GetFineSensitivityKey()
{
  return mFineSensitivityKey;
}



void nuiMatrixView::Init()
{
  mInited =true;

  uint32 nbcolumns = nuiGrid::GetNbColumns();
  uint32 nbrows = nuiGrid::GetNbRows();
  
  // default init for cells 
  bool enableTitle = false;
  uint firstCol=0;
  uint firstRow=0;
  
  if (mColumnHeader && mRowHeader)
  {
    enableTitle = true;
  }


  if (mColumnHeader)
    firstRow=1;

  if (mRowHeader)
    firstCol=1;

  
  if (mColumnHeader)
  {
    for (uint i=firstCol; i < nbcolumns; i++)
    {
      nglString label;
      label.Format (_T("column %d"), i);
      SetColumnHeader (i, new nuiLabel(label));
    }
  }

  if (mRowHeader)
  {
    for (uint i=firstRow; i < nbrows; i++)
    {
      nglString label;
      label.Format (_T("row %d"), i);
      SetRowHeader (i, new nuiLabel(label));
    }
  }
   
  
  for (uint i=firstRow; i < nbrows; i++)
  {
    for (uint j=firstCol; j < nbcolumns; j++)
    {
      nuiMatrixViewItem* item = new nuiMatrixViewItem(this, j, i, mRange, mFormater);
      //item->SetPosition(nuiFillHorizontal);
      item->SetTextPosition(nuiCenter);
      nuiGrid::SetCell(j, i, item);
//      nuiGrid::SetColumnExpand(j, nuiExpandShrinkAndGrow);
      nuiGrid::SetColumnExpand(j, nuiExpandFixed);
    }
  }

}
 



//***************************************************************************************************
//
// getters / setters
//
//****************************************************************************************************

bool nuiMatrixView::IsColumnHeaderEnabled()
{
  return mColumnHeader;
}


bool nuiMatrixView::IsRowHeaderEnabled()
{
  return mRowHeader;
}

void nuiMatrixView::EnableSelectAll(bool canSelectAll)
{
  mCanSelectAll = canSelectAll;
}


bool nuiMatrixView::IsSelectAllEnabled()
{
  return mCanSelectAll;
}


bool nuiMatrixView::SetColumnHeader(uint col, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting)
{
  NGL_ASSERT(mInited);
  if (!mColumnHeader) return false;
  return nuiGrid::SetCell(col, 0, pWidget, position, ReplaceExisting);
}


bool nuiMatrixView::SetRowHeader(uint row, nuiWidget* pWidget, nuiPosition position, bool ReplaceExisting)
{
  NGL_ASSERT(mInited);
  if (!mRowHeader) return false;
  return nuiGrid::SetCell(0, row, pWidget, position, ReplaceExisting);
}






bool nuiMatrixView::GetCellValue(uint col, uint row, double& value)
{
  NGL_ASSERT(mInited);
  if ( ((col == 0) && mRowHeader) || ((row == 0) && mColumnHeader) )
  {
    NGL_OUT(_T("nuiMatrixView::GetCellValue failed  (%d,%d) : %.0f\n"), col, row);
    NGL_ASSERT(0);
    return false;
  }
    
      nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
      if (!item)
        return false;
  value = item->GetValue();
  
	return true;
}


bool nuiMatrixView::SetCellValue(uint col, uint row, double value)
{
  NGL_ASSERT(mInited);
  if ( ((col == 0) && mRowHeader) || ((row == 0) && mColumnHeader) )
  {
    NGL_OUT(_T("nuiMatrixView::SetCellValue failed  (%d,%d) : %.0f\n"), col, row, value);
    NGL_ASSERT(0);
    return false;
  }

      nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
      if (!item)
        return false;
  item->SetValue (value);

  return true;
}


void nuiMatrixView::SetCellEnabled(uint col, uint row, bool set)
{
  NGL_ASSERT(mInited);
  nuiWidget* pWidget = nuiGrid::GetCell(col, row);
  pWidget->SetEnabled(set);
}


bool nuiMatrixView::IsCellEnabled(uint col, uint row)
{
  nuiWidget* pWidget = nuiGrid::GetCell(col, row);
  return pWidget->IsEnabled();
}



const std::map<nuiMatrixViewItem*, bool>& nuiMatrixView::GetSelectedItems()
{
  NGL_ASSERT(mInited);
  return mSelectedItems;
}







//***************************************************************************************************
//
// events receiver
//
//****************************************************************************************************

bool nuiMatrixView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  //***********************************************************
  //
  // LEFT BUTTON 
  //
  //***********************************************************
  if (Button & nglMouseInfo::ButtonLeft)
  {
    uint col,row;
    
    // you did'nt clicked in a cell
    if (!GetCellCoord(X, Y, col, row))
    {
      // it may be a "deselect all" action
      if (!IsKeyDown (mGrabMouseKey))
      {
        DeselectAll();
        Invalidate();
      }
       
      return false;
    }

    // retrieve the targeted cell
    nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      
    // if the cell is disabled, don't do anything
    if (!pWidget || !pWidget->IsEnabled())
    {
      // it may be a "deselect all" action
      if (!IsKeyDown (mGrabMouseKey))
      {
        DeselectAll();
        Invalidate();
      }
       
      return false;
    }

      
    // if the cell is not a matrixview item, don't do anything
    nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
    if (!item)
    {
      // it may be a "deselect all" action
      if (!IsKeyDown (mGrabMouseKey))
      {
        DeselectAll();
        Invalidate();
      }
       
      return false;
    }

    mClickedItem = item;

    // if the item is supposed to be hidden, make it silent
    if (!mClickedItem->IsVisible())
    {
      // it may be a "deselect all" action
      if (!IsKeyDown (mGrabMouseKey))
      {
        DeselectAll();
        Invalidate();
      }
    }
    

    // double click <=> rename the targeted cell (or the selected cells)
    if (Button & nglMouseInfo::ButtonDoubleClick)
    {
      bool res = MouseDoubleClicked(col, row);
      return res;
    }

    
    mClicked=true;
    Grab ();
    mClickPos[0] = X;
    mClickPos[1] = Y;
    
     
     // selection key is pressed
    if (IsKeyDown (mGrabMouseKey))
    {
      bool res = Selected(col, row);
      Invalidate();
      return res;
    }   

    
    // is the targeted cell selected already?
    std::map<nuiMatrixViewItem*, bool>::iterator it = mSelectedItems.find(mClickedItem);
    
    // no ==> deselect all, select this single cell and prepare it to be modified
    if (it == mSelectedItems.end())
    {
      DeselectAll();
      Selected(col, row);
      mCanChange = true;
    }
    
    // yes => do nothing but preparing the cell to be modified
    else
      mCanChange = true;
  }



  //***********************************************************
  //
  // WHEEL UP AND DOWN 
  //
  //***********************************************************
  if ((Button & nglMouseInfo::ButtonWheelUp) || (Button & nglMouseInfo::ButtonWheelDown))
  {
    uint col,row;
    
    if (!GetCellCoord(X, Y, col, row))
      return false;

    // retrieve the targeted cell
    nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      
    // if the cell is disabled, don't do anything
    if (!pWidget || !pWidget->IsEnabled())
      return false;

      
    // if the cell is not a matrixview item, don't do anything
    nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
    if (!item)
      return false;

    
    // one single selected cell or more?
    uint nbSelectedCells = mSelectedItems.size();
    
    if (nbSelectedCells <= 1)
    {
      DeselectAll();
      Selected(col, row);
    }
    
    
    int32 delta;
    if (IsKeyDown(mFineSensitivityKey))
    {
      if (Button & nglMouseInfo::ButtonWheelUp)
        delta = 1;
      else
        delta = -1;
    }
    else
    {
      if (Button & nglMouseInfo::ButtonWheelUp)
        delta = 5;
      else
        delta = -5;
    }
    
    

    
    double valueOfReference = item->GetValue();
    valueOfReference += delta;
    
    // apply value modification on all selected items
    if (IsKeyDown(mRelativeKey))
    {
      std::map<nuiMatrixViewItem*, bool>::iterator it;
      for (it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
      {
        nuiMatrixViewItem* item = it->first;
        item->SetRelativeValue(delta);
      }
    }
    else
    {
      std::map<nuiMatrixViewItem*, bool>::iterator it;
      for (it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
      {
        nuiMatrixViewItem* item = it->first;
        item->SetValue(valueOfReference);
      }
    }
    
    InteractiveValueChanged();
    
    nglTimer::Start(false, true);
    
  }


  Invalidate();
  return true;
}



bool nuiMatrixView::Selected(uint col, uint row)
{
  if (mColumnHeader & mRowHeader & !col & !row)
  {
    if (mCanSelectAll)
      return SelectAll();
    else return true;
  }
    
  if (mColumnHeader && !row)
    return SelectColumn(col);
    
  if (mRowHeader & !col)
    return SelectRow(row);
    
  if (IsSelected(col, row))
    return SelectCell(col, row, false);
    
  return SelectCell(col, row, true);
}


bool nuiMatrixView::MouseDoubleClicked(uint col, uint row)
{
  // double click <=> rename the targeted cell (or the selected cells)
  nuiLabelRenamer* renamer = new nuiLabelRenamer(mClickedItem, false);
  mEventSink.Connect(renamer->Renamed, &nuiMatrixView::Renamed, renamer);
  
  return true;
}


void nuiMatrixView::Renamed(const nuiEvent& rEvent)
{
  NGL_ASSERT(rEvent.mpUser);
  nuiLabelRenamer* renamer = (nuiLabelRenamer*)rEvent.mpUser;
  nglString contents = renamer->GetText();
  contents.Trim();
  if (contents.IsEmpty())
  { 
    rEvent.Cancel();
    return;
  }
    
  double value = contents.GetCDouble();
  
  // report the new value to all selected items
  if (mSelectedItems.size() != 0)
  {
    std::map<nuiMatrixViewItem*, bool>::iterator it;
    for (it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
    {
      nuiMatrixViewItem* item = it->first;
      item->SetValue(value);
    }
  }
  
  // report the new value to the single targeted item
  else
    mClickedItem->SetValue(value);
    
  ValueChanged();
        
  rEvent.Cancel();
}



bool nuiMatrixView::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mClicked)
    return false;

  mClicked = false;
  mCanChange = false;
  mClickedItem = NULL;
  Ungrab();
  
  // send event if a changed occured
  if (mChanged)
  {
    ValueChanged();
    mChanged=false;
  }

  return true;
}


bool nuiMatrixView::MouseMoved(nuiSize X, nuiSize Y)
{
  if (!mClicked || !mCanChange)
    return false;

  nuiSize deltaX = X - mClickPos[0];
  nuiSize deltaY = mClickPos[1] - Y;
  mClickPos[0] = X;
  mClickPos[1] = Y;
  
  nuiSize delta = (abs(deltaX) > abs(deltaY))? deltaX : deltaY;

  // fine change
  if (IsKeyDown(mFineSensitivityKey))
    delta = (delta > 0)? 1 : (-1);

  // look at the selection
  if (mSelectedItems.size() >0)
  {
    if (IsKeyDown(mRelativeKey))
      RelativeSpinCells(delta);
    else
      SpinCells(delta);
    return true;
  }
  
  // no selection. change the currently clicked item
  mClickedItem->SetRelativeValue(delta);
  


  return true;
}


// virtual from nglTimer
// wheel up/down events have occured. We no have to register the modification in the Commands system
void nuiMatrixView::OnTick(nglTime Lap)
{
  nglTimer::Stop();
  ValueChanged();
}



void nuiMatrixView::RelativeSpinCells(nuiSize delta)
{
  std::map<nuiMatrixViewItem*, bool>::iterator it;
  for (it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
  {
    nuiMatrixViewItem* item = it->first;
    item->SetRelativeValue(delta);
  }

  // remember that a change occured
  mChanged=true;
  
  // send event about change
  InteractiveValueChanged();
}


void nuiMatrixView::SpinCells(nuiSize delta)
{
  // take the targeted item as the reference
  double value = mClickedItem->GetValue();
  value += delta;
  std::map<nuiMatrixViewItem*, bool>::iterator it;
  for (it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
  {
    nuiMatrixViewItem* item = it->first;
    item->SetValue(value);
  }
  
  // remember that a change occured
  mChanged=true;
  
  // send event about change
  InteractiveValueChanged();

}








//***************************************************************************************************
//
// selection
//
//****************************************************************************************************


bool nuiMatrixView::SelectAll()
{
  if (mAllSelected)
    return DeselectAll();
    
  uint row, col;
  uint firstRow = 0;
  uint firstCol = 0;
  if (mColumnHeader)
    firstRow=1;
  if (mRowHeader)
    firstCol = 1;
  
  mAllSelected = true;
  

  bool eventDisabled = false;
  
  if (!SelectionChanged.IsEnabled())
    eventDisabled = true;
  
  if (!eventDisabled)
    SelectionChanged.Disable();

 
  // entire rows and cols have a special selection process
  for (row=firstRow; row < GetNbRows(); row++)
    mRowSelected[row] = true;
  for (col=firstCol; col < GetNbColumns(); col++)
    mColumnSelected[col] = true;
  
  for (row=firstRow; row < GetNbRows(); row++)
  {
    for (col=firstCol; col < GetNbColumns(); col++)
    {
      SelectCell(col, row, true, false);
    }
  }

  
  if (!eventDisabled)
    SelectionChanged.Enable();
 
	SelectionChanged();
  
  return true;  
}


bool nuiMatrixView::DeselectAll()
{
  uint row, col;
  uint firstRow = 0;
  uint firstCol = 0;
  if (mColumnHeader)
    firstRow=1;
  if (mRowHeader)
    firstCol = 1;
  
  mAllSelected = false;
  
  bool eventDisabled = false;
  
  if (!SelectionChanged.IsEnabled())
    eventDisabled = true;
  
  if (!eventDisabled)
    SelectionChanged.Disable();
 
  // entire rows and cols have a special selection process
  for (row=firstRow; row < GetNbRows (); row++)
    mRowSelected[row] = false;
  for (col=firstCol; col < GetNbColumns (); col++)
    mColumnSelected[col] = false;
    
  // list the selected items and deselect'em
//  std::map<nuiMatrixViewItem*, bool>::iterator itr;
//  for (itr = mSelectedItems.begin(); itr != mSelectedItems.end(); ++itr)
//  {
//    nuiMatrixViewItem* item = itr->first;
//    SelectCell(item->GetCol(), item->GetRow(), false);
//  }

  std::map<nuiMatrixViewItem*, bool>::iterator it     = mSelectedItems.begin();
  std::map<nuiMatrixViewItem*, bool>::iterator itEnd  = mSelectedItems.end();
  while (it != itEnd)
  {
    nuiMatrixViewItem* item = it->first;
    item->SetSelected(false);
    ++it;
  }


  
  // no selected items left
  mSelectedItems.clear ();
  
  if (!eventDisabled)
    SelectionChanged.Enable();
 
	SelectionChanged();
  
  return true;
}


bool nuiMatrixView::SelectColumn(uint col)
{
  return ForceSelectColumn(col, !mColumnSelected[col]);
}


bool nuiMatrixView::SelectRow(uint row)
{
  return ForceSelectRow(row, !mRowSelected[row]);
}



bool nuiMatrixView::ForceSelectColumn(uint col, bool set)
{
  bool eventDisabled = false;
  
  if (!SelectionChanged.IsEnabled())
    eventDisabled = true;
  
  if (!eventDisabled)
    SelectionChanged.Disable();

  
  uint firstRow=0;
  if (mColumnHeader)
    firstRow=1;
  
  // unselect
  if (!set)
  {
    for (uint k=firstRow; k < GetNbRows(); k++)
      SelectCell(col, k, false, false);

    mColumnSelected[col] = false;
  }

      
  // select
  else
  {

    for (uint k=firstRow; k < GetNbRows(); k++)
      SelectCell(col, k, true, false);
      
    mColumnSelected[col] = true;
  }
          

  if (!eventDisabled)
    SelectionChanged.Enable();
	SelectionChanged();

  return true;
}


bool nuiMatrixView::ForceSelectRow(uint row, bool set)
{
  bool eventDisabled = false;
  
  if (!SelectionChanged.IsEnabled())
    eventDisabled = true;
  
  if (!eventDisabled)
    SelectionChanged.Disable();

  
  uint firstCol=0;
  if (mRowHeader)
    firstCol=1;
  
  // unselect
  if (!set)
  {
    for (uint k=firstCol; k < GetNbColumns(); k++)
      SelectCell(k, row, false, false);

    mRowSelected[row] = false;
  }

      
  // select
  else
  {
    for (uint k=firstCol; k < GetNbColumns(); k++)
      SelectCell(k, row, true, false);
      
    mRowSelected[row] = true;
  }
          

  if (!eventDisabled)
    SelectionChanged.Enable();
    
	SelectionChanged();

  return true;
}





bool nuiMatrixView::IsSelected(uint col, uint row)
{
      nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
      if (!item)
        return false;

  std::map<nuiMatrixViewItem*, bool>::iterator it = mSelectedItems.find(item);
  return (it != mSelectedItems.end());
}



bool nuiMatrixView::SelectCell(uint col, uint row, bool select, bool checkSelection)
{
  nuiWidget* pWidget = nuiGrid::GetCell(col, row);
	nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
  if (!item)
    return false;
  
  if (!item->IsVisible())
    return false;
  
  // deselect cell
  if (!select)
  {
    std::map<nuiMatrixViewItem*, bool>::iterator it = mSelectedItems.find(item);
    if (it != mSelectedItems.end())
    {
      nuiMatrixViewItem* item = it->first;
      mSelectedItems.erase(it);
      item->SetSelected(false);
      
      if (checkSelection && IsAllColumnCellsSelected(col, false))
        mColumnSelected[col] = false;

      if (checkSelection && IsAllRowCellsSelected(row, false))
        mRowSelected[row] = false;

    }
    
    return true;
  }
  
  // select cell
  mSelectedItems[item] = true;
  item->SetSelected(true);

  if (checkSelection && IsAllColumnCellsSelected(col, true))
    mColumnSelected[col] = true;

  if (checkSelection && IsAllRowCellsSelected(row, true))
    mRowSelected[row] = true;


	SelectionChanged();
  return true;
}



bool nuiMatrixView::IsAllColumnCellsSelected(uint col, bool select)
{
  uint firstRow=0;
  if (mColumnHeader)
    firstRow=1;
    
  for (uint row=firstRow; row < GetNbRows(); row++)
  {
      nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
      if (!item)
        continue;

    if (item->IsSelected() != select)
      return false;
  }
  
  return true;
}


bool nuiMatrixView::IsAllRowCellsSelected(uint row, bool select)
{
  uint firstCol=0;
  if (mRowHeader)
    firstCol=1;
    
  for (uint col=firstCol; col < GetNbColumns(); col++)
  {
      nuiWidget* pWidget = nuiGrid::GetCell(col, row);
      nuiMatrixViewItem* item = dynamic_cast<nuiMatrixViewItem*>(pWidget);
      if (!item)
        continue;

    if (item->IsSelected() != select)
      return false;
  }
  
  return true;
}



//***************************************************************************************************
//
// tools
//
//****************************************************************************************************



bool nuiMatrixView::GetCellCoord(int mouseX, int mouseY, uint& targetCol, uint& targetRow)
{

  // search the selected spinerLabel
  nuiSize widthSoFar = 0.f;
  for (uint col= 0; col < mWidths.size(); col++)
  {
    widthSoFar += mVGaps[col];
    widthSoFar += mWidths[col];
    widthSoFar += mVGaps[col];
    
    if (widthSoFar >= mouseX)
    {
      nuiSize heightSoFar = 0.f;
      for (uint row = 0; row < mNbRows; row++)
      {
        if (!mRowVisible[row])
          continue;
        
        heightSoFar += mHGaps[row];
        heightSoFar += mHeights[row];
        heightSoFar += mHGaps[row];
        
        
        if (heightSoFar >= mouseY)
        {
          // ok, we have retrieved the item that is being targeted  : (i,j)
          targetCol = col;
          targetRow = row;

          return true;
        }
      
      } // for
    } // if
  } // for
  

  targetCol = 0;
  targetRow = 0;
  return false;
}



void nuiMatrixView::AddRows(uint pos, uint rows)
{
  NGL_ASSERT(0);
  printf("dynamic change of structure is forbidden on nuiMatrixView!\n");
}


void nuiMatrixView::AddColumns(uint pos, uint columns)
{
  NGL_ASSERT(0);
  printf("dynamic change of structure is forbidden on nuiMatrixView!\n");
}


void nuiMatrixView::RemoveRows(uint pos, uint rows)
{  
  NGL_ASSERT(0);
  printf("dynamic change of structure is forbidden on nuiMatrixView!\n");
}


void nuiMatrixView::RemoveColumns(uint pos, uint columns)
{
  NGL_ASSERT(0);
  printf("dynamic change of structure is forbidden on nuiMatrixView!\n");
}




//***************************************************************************************************
//
// drawing
//
//****************************************************************************************************

bool nuiMatrixView::Draw(nuiDrawContext* pContext)
{
  pContext->ResetState();
  
   nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);
  
  nuiSize matrixWidth = 0.f;
  nuiSize matrixHeight = 0.f;
  nuiSize headerWidth = 0.0f;
  if (mWidths.size() > 0)
    headerWidth+= mWidths[0];
  if (mVGaps.size() > 0)
    headerWidth += 2*mVGaps[0];
  nuiSize headerHeight = 0.0f;
  if (mHeights.size() > 0)
    headerHeight += mHeights[0];
  if (mHGaps.size() > 0)
    headerHeight += 2*mHGaps[0];
  nuiRect mxrect;
  
  // matrix positions (different depending on headers visibility)
  uint matrixFirstColumn=0;
  nuiSize matrixFirstX=0.0f;
  if (mRowHeader) 
  {
    matrixFirstColumn=1;
    matrixFirstX=headerWidth;
  }
  
  uint matrixFirstRow=0;
  nuiSize matrixFirstY=0.0f;
  if (mColumnHeader) 
  {
    matrixFirstRow=1;
    matrixFirstY=headerHeight;
  }
  
  // compute matrix size
  for (uint i = matrixFirstColumn; i < mWidths.size(); i++)
  {
    matrixWidth += mVGaps[i];
    matrixWidth += mWidths[i];
    matrixWidth += mVGaps[i];
  }     
  
  for (uint j = matrixFirstRow; j < mHeights.size(); j++)
  {
    if (!mRowVisible[j])
      continue;
      
    matrixHeight += mHGaps[j];
    matrixHeight += mHeights[j];
    matrixHeight += mHGaps[j];
  }
  
  
    nuiGrid::Draw (pContext);

  
  // draw selection boxes
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  std::map<nuiMatrixViewItem*, bool>::iterator itr;
  for (itr = mSelectedItems.begin(); itr != mSelectedItems.end(); ++itr)
  {
    nuiMatrixViewItem* item = itr->first;
    nuiRect rect = item->GetRect();
    
    uint32 size = item->GetDesc().GetSelectionSize();
    pContext->SetStrokeColor(item->GetDesc().GetSelectionColor());

    pContext->SetStrokeColor(item->GetDesc().GetSelectionColor());
    pContext->SetLineWidth(size);
    
    pContext->DrawRect(rect, eStrokeShape);
  }
  
  

  pTheme->Release();
  return true;

}




