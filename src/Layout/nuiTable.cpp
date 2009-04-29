/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiWidget.h"
#include "nuiTable.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiScrollBar.h"
#include "nuiMainWindow.h"

#include <math.h>
#include <algorithm>

using namespace std;

/////////
// nuiTableRow:

#define INTERLINE 1
#define INTERCOL 50
#define HEADERHANDLESIZE 5

#define NUI_TREE_ROW_OFFSET 8 // 8 pixels offset right for each generation.
#define NUI_TREE_EXPANDER_SIZE 12 // right offset to place the + and - mini buttons.


nuiTableRow::nuiTableRow(nuiTable* pParent)
{
  mHeight = 0;
  mTotalHeight = 0;
  mWidth = 0;
  mXOffset = 0;
  mYOffset = 0;
  mX = 0;
  mY = 0;
  mpParent = pParent;
  mpParentRow = NULL;
  if (pParent)
  {
    uint i;
    for (i=0; i<pParent->GetColumnCount(); i++)
      mpCells.push_back(NULL);
  }
  mSelected = false;
  mOpen = true;
}

nuiTableRow::nuiTableRow(nuiTable* pParent, const nuiWidgetList& rList)
  : mpCells(rList)
{
  mHeight = 0;
  mTotalHeight = 0;
  mWidth = 0;
  mXOffset = 0;
  mYOffset = 0;
  mX = 0;
  mY = 0;
  mpParent = pParent;
  mpParentRow = NULL;
  if (pParent)
  {
    uint i;
    for (i=rList.size(); i<pParent->GetColumnCount(); i++)
      mpCells.push_back(NULL);
  }
  mSelected = false;
  mOpen = true;
}

nuiTableRow::~nuiTableRow()
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end; ++it)
    if (*it)
      delete (*it); 
}

bool nuiTableRow::SetParent(nuiTable* pParent)
{
  mpParent = pParent;
  return true;
}

bool nuiTableRow::SetParent(nuiTableRowPtr pParent)
{
  if (mpParentRow)
  {
    mpParentRow->DelRow(this);
    mpParentRow = NULL;
  }
  if (pParent)
    pParent->AddRow(this);
  mpParentRow = pParent;
  return true;
}

nuiTableRowPtr nuiTableRow::GetParentRow()
{
  return mpParentRow;
}


bool nuiTableRow::AddRow(nuiTableRowPtr pRow)
{
  mpRows.push_back(pRow);
  if (mOpen)
    mpParent->InvalidateLayout();
  pRow->mpParentRow = this;
  return true;
}

bool nuiTableRow::DelRow(nuiTableRowPtr pRow, bool Delete)
{
  if (!pRow)
    return false;
  if (GetRow(pRow) < 0)
    return false;
  mpRows.remove(pRow);
  if (Delete)
  {
    pRow->Clear(true);
    delete pRow;
  }
  pRow->mpParentRow = NULL;
  return true;
}

int nuiTableRow::GetRow(nuiTableRowPtr pRow)
{
  int index = 0;
  if (!pRow)
    return -1;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it != end; ++it)
  {
    if (*it == pRow)
      return index;
    index++;
  }
  return -1;
}

uint nuiTableRow::GetRowCount()
{
  return mpRows.size();
}

nuiTableRowPtr nuiTableRow::GetRow(int Row)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  if (Row<0)
    return NULL;

  for (it = mpRows.begin(); it != end && Row; ++it)
  {
    Row--;
  }
  if (Row || it == end)
    return NULL;
  return *it;
}

nuiTableRowPtr nuiTableRow::GetRow(nuiSize X, nuiSize Y)
{
  if (Y < mHeight)
    return this;

  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  nuiSize y = mHeight;
  for (it = mpRows.begin(); it != end; ++it)
  {
    nuiTableRowPtr pRow = *it;
    if (y < Y && Y < y + pRow->GetTotalHeight())
      return pRow->GetRow(X,Y - y);

    y += pRow->GetTotalHeight();
  }
  return NULL;
}


bool nuiTableRow::InsertRow(int index, nuiTableRowPtr pRow)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it != end && index; ++it)
  {
    index--;
  }
  if (index)
    return false;

  mpRows.insert(it, pRow);
  pRow->SetParent((nuiTableRowPtr) NULL);
  pRow->mpParentRow = this;
  return true;
}

nuiTableRowPtr nuiTableRow::GetAbove()
{
  nuiTableRowPtr pRow = NULL;
  if (mpParentRow)
  {
    nuiTableRowList::iterator it;
    it = find(mpParentRow->mpRows.begin(), mpParentRow->mpRows.end(), this);
    if (it == mpParentRow->mpRows.begin())
      return mpParentRow;
    if (!pRow)
    {
      it--;
      pRow = *it;
    }
  }

  if (mpParent && !pRow)
  {
    int row = mpParent->GetRow(this);
    if (row > 0)
      pRow = mpParent->GetRow(row - 1);
  }

  if (pRow)
  {
    while (!pRow->mpRows.empty())
      pRow = pRow->mpRows.back();
  }
  return pRow;
}

nuiTableRowPtr nuiTableRow::GetBelow()
{
  if (mOpen && !mpRows.empty())
    return mpRows.front();

  nuiTableRowPtr pRow = GetNextSibling();
  nuiTableRowPtr pParent = mpParentRow;
  while (!pRow && pParent)
  {
    pRow = pParent->GetNextSibling();
    pParent = pParent->mpParentRow;
  }
  if (pRow) 
    return pRow;

  if (mpParent)
  {
    int row = mpParent->GetRow(this);
    if (row >= 0)
      return mpParent->GetRow(row + 1);
  }

  return NULL;
}


bool nuiTableRow::IsVisible() const
{
  nuiTableRowPtr pRow = mpParentRow;
  while (pRow)
  {
    if (!pRow->IsOpened())
      return false;
    pRow = pRow->mpParentRow;
  }
  return true;
}

nuiTableRowPtr nuiTableRow::GetFirstVisibleParent()
{
  nuiTableRowPtr pRow = mpParentRow;
  while (pRow)
  {
    if (pRow->IsVisible())
      return pRow;
    pRow = pRow->mpParentRow;
  }
  return NULL;
}

nuiTableRowPtr nuiTableRow::GetNextSibling()
{
  if (!mpParentRow)
  {
    if (!mpParent)
      return NULL;
    int i = mpParent->GetRow(this);
    return mpParent->GetRow(i+1);
  }

  nuiTableRowList::iterator it = find(mpParentRow->mpRows.begin(), mpParentRow->mpRows.end(), this);
  if ( it == mpParentRow->mpRows.end() )
    return NULL;

  ++it;
  if ( it == mpParentRow->mpRows.end() )
    return NULL;
  return *it;
}

nuiWidgetPtr nuiTableRow::GetCell(uint col)
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();
  for (it = mpCells.begin(); it != end && col; ++it)
    col--;

  if (it != end)
    return (*it);

  return NULL;
}

bool nuiTableRow::SetCell(uint col, nuiWidgetPtr pWidget, bool Delete )
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  for (it = mpCells.begin(); it != end && col; ++it)
    col--;

  if (it != end)
  {
    if (*it && Delete)
      (*it)->Trash();

    (*it) = pWidget;
    mpParent->AddChild(pWidget);
    return true;
  }

  return false;
}

bool nuiTableRow::InsertCell(uint col, nuiWidgetPtr pWidget)
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  for (it = mpCells.begin(); it != end && col; ++it)
    col--;

  if (it != end || !col)
  {
    mpCells.insert(it,pWidget);
    mpParent->AddChild(pWidget);
    return true;
  }

  return false;
}

bool nuiTableRow::RemoveCell(uint col, bool Delete)
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  for (it = mpCells.begin(); it != end && col; ++it)
    col--;

  if (it != end)
  {
    if (*it && Delete)
      (*it)->Trash();

    mpCells.erase(it);
    return true;
  }

  return false;
}

bool nuiTableRow::ClearCell(uint col, bool Delete)
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  for (it = mpCells.begin(); it != end && col; ++it)
    col--;

  if (it != end)
  {
    if (*it && Delete)
      (*it)->Trash();

    (*it) = NULL;
    return true;
  }

  return false;
}

bool nuiTableRow::Clear(bool Delete)
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  if (Delete)
    for (it = mpCells.begin(); it != end; ++it)
      if (*it)
        (*it)->Trash();

  mpCells.clear();

  return true;
}


uint nuiTableRow::GetSize()
{
  return mpCells.size();
}

bool nuiTableRow::Draw (nuiDrawContext* pContext, uint min, uint max, int SelectedColumn, bool Header, nuiTableRowPtr pCursorRow)
{
  uint count = 0;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  pContext->ResetState();

  pContext->PushMatrix();
  
  nuiRect rect(0.0f,0.0f,mWidth, mHeight);
  if (Header)
  {
    pContext->SetStrokeColor(mpParent->GetColor(eSelectedTableColumnBg));
    pContext->DrawLine(0, mHeight, mWidth, mHeight); 
  }
  else 
  {
    pContext->EnableBlending(true);
    pContext->SetBlendFunc(nuiBlendTransp);

    pContext->SetFillColor(mpParent->GetColor(eTableColumnBg));
    nuiRect r(rect);
    r.MoveTo(mX, mY);
    pContext->DrawRect(r,eFillShape);

    if (IsSelected())
    {
      pContext->SetFillColor(mpParent->GetColor(eSelectedTableLineBg));
      nuiRect r(rect);
      r.MoveTo(mXOffset + mX, mYOffset + mY);
      pContext->DrawRect(r,eFillShape);
    }
    pContext->EnableBlending(false);
  }

  pContext->PushMatrix();

  if (mpRows.size())
  {
    pContext->SetStrokeColor(nuiColor(.5f,.5f,.5f));
    pContext->DrawLine(5.0f, (float)(GLint) (mHeight / 2), 12.0f, (float)(GLint) (mHeight / 2));

    if (!mOpen)
      pContext->DrawLine( 8.0f, (float)(GLint) (mHeight / 2 - 3),  8.0f, (float)(GLint) (mHeight / 2 + 4));

    pContext->SetStrokeColor(nuiColor(0.f,0.f,0.f));
    pContext->DrawLine( 4.0f, (float)(GLint) (mHeight / 2) -1, 11.0f, (float)(GLint) (mHeight / 2) -1);

    if (!mOpen)
      pContext->DrawLine( 7.0f, (float)(GLint) (mHeight / 2 - 4), 7.0f, (float)(GLint) (mHeight / 2 + 3));
  }
  
  for (it = mpCells.begin(); it != end; ++it)
  {
    nuiWidgetPtr pWidget = *it;
    if (pWidget)
    {
      rect = pWidget->GetRect().Size();
      if ((count<=max) && (count>=min))
      {
        pContext->PushClipping();
        pContext->EnableClipping(true);
        pContext->Clip(pWidget->GetRect());

        if (count == (uint)SelectedColumn && !Header)  
        {
          pContext->EnableBlending(true);
          pContext->SetFillColor(mpParent->GetColor(eSelectedTableColumnBg));
          nuiRect r(rect);
          r.MoveTo(mXOffset + mX, mYOffset + mY);
          pContext->DrawRect(r,eFillShape);
          pContext->EnableBlending(false);
        }

        pContext->PushMatrix();
        pContext->MultMatrix(pWidget->GetMatrix());
        mpParent->DrawTableChild(pContext, pWidget);
        pContext->PopMatrix();
      
        if (Header)
        {
          pContext->SetStrokeColor(mpParent->GetColor(eSelectedTableColumnBg));
          pContext->EnableBlending(false);
          pContext->DrawLine(rect.mRight-1, rect.mTop, rect.mRight-1, rect.mBottom);
        }

        pContext->PopClipping();
      }
    }
    else
    {
      // Empty cell...
    }
    count++;

  }

  if (Header)
  {
    pContext->SetStrokeColor(mpParent->GetColor(eSelectedTableColumnBg));
    pContext->DrawLine(rect.mLeft, rect.mTop, rect.mLeft, rect.mBottom);
  }

  pContext->PopMatrix();

  if (this == pCursorRow)
  {
    nuiRect out(0.0f,0.0f,mWidth,mHeight);
    out.MoveTo(mX, mY);
    nuiRect in(out);
    out.Grow(-1, -1);

    bool en = pContext->GetState().mBlending;
    pContext->EnableBlending(true);
    pContext->SetStrokeColor(nuiColor(0.5f,0.5f,0.5f,0.5f));
    pContext->DrawRect(out,eStrokeShape);
    pContext->SetStrokeColor(nuiColor(1.f,1.f,1.f,0.5f));
    pContext->DrawRect(in,eStrokeShape);
    pContext->EnableBlending(en);
  }

  pContext->PopMatrix();
  pContext->EnableBlending(false);

  if (mpRows.size())
  {
    nuiSize y = mYOffset + mHeight;
    if (mOpen)
    {
      pContext->SetStrokeColor(nuiColor(.5f,.5f,.5f));
      pContext->DrawLine( mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y - 4, mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y );

      nuiTableRowList::iterator rowit;
      nuiTableRowList::iterator rowend = mpRows.end();
      for (rowit = mpRows.begin(); rowit != rowend; )
      {
        nuiTableRowPtr pRow = *rowit;
        ++rowit;
        if (pRow)
        {
          pRow->Draw(pContext, min, max, SelectedColumn, Header, pCursorRow);

          pContext->SetStrokeColor(nuiColor(.5f,.5f,.5f));

          if (rowit == rowend)
          {
            pContext->DrawLine( mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y, mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y + pRow->mHeight /2 );
          }
          else
          {
            pContext->DrawLine( mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y, mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y + pRow->mTotalHeight );
          }

          if (pRow->mpRows.size())
          {
            pContext->DrawLine( mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y + pRow->mHeight /2, pRow->mXOffset  - (NUI_TREE_EXPANDER_SIZE / 2 + NUI_TREE_ROW_OFFSET), y + pRow->mHeight /2 );
          }
          else
          {
            pContext->DrawLine( mXOffset - (2 + NUI_TREE_EXPANDER_SIZE / 2), y + pRow->mHeight /2, pRow->mXOffset , y + pRow->mHeight /2 );
          }

          y += pRow->mTotalHeight;
        }
      }
    }
  }

  return true;
}

nuiRect nuiTableRow::CalcIdealSize(nuiDrawContext* pContext)
{
  mHeight = 0;
  nuiSize Width = 0;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  for (it = mpCells.begin(); it != end; ++it)
  {
    nuiWidgetPtr pWidget = *it;
    if (pWidget)
    {
      nuiRect rect;
      rect = pWidget->GetIdealRect();
      uint h = (uint)rect.GetHeight();
      mHeight = MAX(mHeight, h + INTERLINE);
      Width += rect.GetWidth();
    }
  }

  float TotalWidth = Width;
  float TotalHeight = mHeight;

  if (mOpen)
  {
    nuiTableRowList::iterator rowit;
    nuiTableRowList::iterator rowend = mpRows.end();
    for (rowit = mpRows.begin(); rowit != rowend; ++rowit)
    {
      nuiTableRowPtr pRow = *rowit;
      if (pRow)
      {
        nuiRect rect = pRow->CalcIdealSize(pContext);
        TotalHeight += rect.GetHeight();
        TotalWidth = MAX(TotalWidth, rect.GetWidth());
      }
    }
  }

  mTotalHeight = TotalHeight;
  if (mpRows.size())
    TotalWidth += NUI_TREE_ROW_OFFSET;

  return nuiRect(0.0f,0.0f, TotalWidth, TotalHeight);
}

bool nuiTableRow::SetRects(const list<nuiSize>* pSizes, nuiSize X, nuiSize Y, nuiSize XOffset, nuiSize YOffset)
{
  if (mpRows.size())
    XOffset += NUI_TREE_EXPANDER_SIZE;
  mXOffset = XOffset;
  mYOffset = YOffset;
  mX = X;
  mY = Y;
  nuiSize x = -X;
  mWidth = 0;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  list<nuiSize>::const_iterator sit = pSizes->begin();
  list<nuiSize>::const_iterator send = pSizes->end();

  bool first = true;
  for (it = mpCells.begin(); it != end && sit != send; ++it)
  {
    nuiRect rect;
    nuiWidgetPtr pWidget = *it;
    nuiSize w = (*sit);
    if (pWidget)
    {
      if ( first )
        rect.Set(x + XOffset,Y + YOffset,w - XOffset,mHeight - INTERLINE);
      else
        rect.Set(x ,Y + YOffset,w,mHeight - INTERLINE);
      first = false;
      pWidget->SetLayout(rect);
    }
    x+=w;
    mWidth +=w;
    sit++;
  }

  if (mOpen)
  {
    nuiTableRowList::iterator rowit;
    nuiTableRowList::iterator rowend = mpRows.end();
    YOffset += mHeight;
    for (rowit = mpRows.begin(); rowit != rowend; ++rowit)
    {
      nuiTableRowPtr pRow = *rowit;
      if (pRow)
      {
        pRow->SetRects(pSizes, X, Y, XOffset + NUI_TREE_ROW_OFFSET, YOffset);
        YOffset += pRow->GetTotalHeight();
      }
    }
  }

  return true;
}

nuiSize nuiTableRow::GetHeight()
{
  return mHeight;
}

nuiSize nuiTableRow::GetTotalHeight()
{
  return mTotalHeight;
}

void nuiTableRow::SetSelected(bool Flag, bool ChangeChildren)
{
  mSelected = Flag;

  if (!ChangeChildren)
    return;

  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpCells.end();

  for (it = mpCells.begin(); it != end; ++it)
  {
    nuiWidgetPtr pWidget = *it;
    if (pWidget)
    {
      if (Flag)
      {
        if (!pWidget->IsSelected())
          pWidget->SetSelected(true);
      }
      else
      {
        if (pWidget->IsSelected())
          pWidget->SetSelected(false);
      }    
    }
  }
}

bool nuiTableRow::IsSelected()
{
  return mSelected;
}

void nuiTableRow::SelectAll()
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  SetSelected(true);
  for (it = mpRows.begin(); it != end; ++it)
  {
    (*it)->SelectAll();
  }

}

void nuiTableRow::DeselectAll()
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  SetSelected(false);
  for (it = mpRows.begin(); it != end; ++it)
  {
    (*it)->DeselectAll();
  }

}


void nuiTableRow::Open(bool Set)
{
  if (mOpen == Set)
    return;
  mOpen = Set; 
  mpParent->InvalidateLayout();
}

bool nuiTableRow::IsOpened()
{
  return mOpen;
}

bool nuiTableRow::HandleClick(nuiSize X, nuiSize Y)
{
  if (mpRows.empty())
    return false;
  if (X < mXOffset && X > mXOffset - NUI_TREE_EXPANDER_SIZE )
  {
    Open(!IsOpened());
    return true;
  }
  return false;
}



/////////
// nuiTable:

nuiTable::nuiTable()
  : nuiSimpleContainer(),
    mHeader(NULL),
    mEventSink(this)

{
  SetObjectClass(_T("nuiTable"));
  mSelectedCol = 0;
  mClickedColumn = 0;
  mClicked = false;
  mResize = false;
  mDisplayHeaders = true;
  mHeader.SetParent(this);

//  mCursorLine = 0;
  mpCursorRow = NULL;
  mSelection = false;
  mKeyboardSelect = false;
  mDisplayCursor = false;
  mMoveOnly = false;

  mMultiSelectable = false;
  mUnselectable = false;

  mpLastItem = NULL;

  InitProperties();
}

bool nuiTable::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  mHeader = NULL;
  
  SetObjectClass(_T("nuiTable"));
  mClickedColumn = 0;
  mClicked = false;
  mResize = false;
  mHeader.SetParent(this);

//  mCursorLine = 0;
  mpCursorRow = NULL;
  mSelection = false;
  mKeyboardSelect = false;
  mDisplayCursor = false;
  mMoveOnly = false;

  mMultiSelectable = pNode->GetAttribute(nglString(_T("MultiSelectable"))) == _T("true");
  mUnselectable = pNode->GetAttribute(nglString(_T("Unselectable"))) == _T("true");
  mSelectedCol = nuiGetVal(pNode,_T("SelectedColumn"), -1);
  mDisplayHeaders = nuiGetBool(pNode,_T("DisplayHeaders"), true);;

  mpLastItem = NULL;

  InitProperties();
  return true;
}

nuiXMLNode* nuiTable::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode,Recursive);
  if (!pNode) 
    return NULL;
  pNode->SetAttribute(_T("MultiSelectable"),mMultiSelectable);
  pNode->SetAttribute(_T("Unselectable"),mUnselectable);
  pNode->SetAttribute(_T("SelectedColumn"), (int64)mSelectedCol);
  return pNode;
}

void nuiTable::InitProperties()
{
  
  AddEvent(_T("TableClicked"), Clicked);
  NUI_ADD_EVENT(Activated);
  NUI_ADD_EVENT(SelectionChanged);
  
  SetWantKeyboardFocus(true);
}

nuiTable::~nuiTable()
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end; ++it)
    if (*it)
      delete (*it); 
}

bool nuiTable::AddChild(nuiWidgetPtr pChild)
{
  // the children of a nuiTable are NOT exportable by default!
  if (pChild)
    pChild->SetSerializeMode(eDontSaveNode);
  return nuiSimpleContainer::AddChild(pChild);
}

bool nuiTable::InsertColumn(uint col, nuiWidgetPtr pWidget)
{
  pWidget->SetProperty(_T("ColumnWidth"),_T("70"));
  return mHeader.InsertCell(col, pWidget);
}

bool nuiTable::InsertColumn(uint col, const nglString& rHeader)
{
  nuiWidgetPtr pWidget = new nuiLabel(rHeader);
  pWidget->SetProperty(_T("ColumnWidth"),_T("70"));

  mHeader.InsertCell(col, pWidget);
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end; ++it)
    if (*it)
      (*it)->InsertCell(col, NULL); 
  return true;
}

bool nuiTable::RemoveColumn(uint col, bool Delete)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end; ++it)
    if (*it)
      return (*it)->RemoveCell(col, Delete); 
  
  mHeader.RemoveCell(col,Delete);
  return false;
}

bool nuiTable::SetColumn(uint col, const nglString& rText)
{
  nglString size = mHeader.GetCell(col)->GetProperty(_T("ColumnWidth"));
  mHeader.SetCell(col, new nuiLabel(rText));
  mHeader.GetCell(col)->SetProperty(_T("ColumnWidth"), size);
  return true;
}

bool nuiTable::SetColumn(uint col, nuiWidgetPtr pWidget)
{
  nglString size = mHeader.GetCell(col)->GetProperty(_T("ColumnWidth"));
  pWidget->SetProperty(_T("ColumnWidth"), size);
  mHeader.SetCell(col,pWidget);
  return true;
}

bool nuiTable::ClearColumn(uint col, bool Delete)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end; ++it)
    if (*it)
      return (*it)->ClearCell(col, Delete); 

  return false;
}

uint nuiTable::GetColumnCount()
{
  return mHeader.GetSize();
}

int nuiTable::GetColumn(nuiSize X, nuiSize Y)
{
  float x=0,w=0;
  uint i;

  for (i = 0; i<mHeader.GetSize() && w < mRect.GetWidth(); i++)
  {
    nuiWidgetPtr pItem;
    pItem = mHeader.GetCell(i);
    if (pItem)
    {
      int ww = (int)pItem->GetProperty(_T("ColumnWidth")).GetCInt();
      if (X <= w+ww+x && X > w+x)
        return i;
      w+=ww;
    }
  }

  return -1; // not in a column
}

void nuiTable::ShowColumnHeaders(bool set)
{
  if (mDisplayHeaders == set)
    return;
  mDisplayHeaders = set;
  InvalidateLayout();
}

bool nuiTable::GetShowColumnHeaders()
{
  return mDisplayHeaders;
}

void nuiTable::SetColumnSize(uint Column, nuiSize Size)
{
  nglString size;
  size.CFormat("%f",Size);
  mHeader.GetCell(Column)->SetProperty(_T("ColumnWidth"), size);

  InvalidateLayout();
}

nuiSize nuiTable::GetColumnSize(uint Column)
{
  return mHeader.GetCell(Column)->GetProperty(_T("ColumnWidth")).GetCFloat();
}

uint nuiTable::GetRowCount()
{
  return (uint)mpRows.size();
}

nuiTableRowPtr nuiTable::GetRow(uint row)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end && row; ++it)
    row--; 

  if (it!=end)
    return *it;

  return NULL;
}

uint32 nuiTable::GetRow(nuiTableRowPtr pRow)
{
  uint32 row;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(), row = 0; it!=end; ++it, row++)
  {
    if (pRow == (*it))
      return row;
  }

  return 0; //FIXME: can't return error (like -1)
}

nuiTableRowPtr nuiTable::InsertRow(uint row, nuiTableRowPtr pRow)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end && row; ++it)
    row--; 

  if (it != end || !row)
  {
    mpRows.insert(it, pRow);
    return pRow;
  }

  return NULL;
}

nuiTableRowPtr nuiTable::InsertRow(uint row, const nuiWidgetList& rList)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end && row; ++it)
    row--; 

  if (it != end || !row)
  {
    nuiTableRowPtr pRow = new nuiTableRow(this,rList);
    mpRows.insert(it, pRow);
    return pRow;
  }

  return NULL;
}

nuiTableRowPtr nuiTable::InsertRow(uint row)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end && row; ++it)
    row--; 

  if (it != end || !row)
  {
    nuiTableRowPtr pRow = new nuiTableRow(this);
    mpRows.insert(it, pRow);
    return pRow;
  }
  return NULL;
}


bool nuiTable::SetRow(uint row, nuiTableRowPtr pRow, bool Delete)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it = mpRows.begin(); it!=end && row; ++it)
    row--; 

  if (it!=end)
  {
    if (Delete && (*it))
    {
      (*it)->Clear(Delete);
      delete (*it);
    }

    if (mpCursorRow == *it)
      mpCursorRow = pRow;

    (*it) = pRow;
    return true;
  }
  
  return false;
}

bool nuiTable::RemoveRow(uint row, bool Delete)
{
  nuiTableRowPtr pRow = GetRow(row);
  if (pRow)
  {
    mpRows.remove(pRow);
    if (Delete)
    {
      pRow->Clear(Delete);
      delete pRow;
    }
    if (mpCursorRow == pRow)
      mpCursorRow = NULL;
    return true;
  }
  return false;
}

bool nuiTable::ClearRow(uint row, bool Delete)
{
  nuiTableRowPtr pRow = GetRow(row);
  if (pRow)
    return pRow->Clear(Delete);
 
  return false;
}

nuiWidgetPtr nuiTable::GetCell(uint row, uint col)
{
  nuiTableRowPtr pRow = GetRow(row);
  if (pRow)
    return pRow->GetCell(col);
  return NULL;
}

bool nuiTable::SetCell(uint row, uint col, nuiWidgetPtr pWidget, bool Delete)
{
  nuiTableRowPtr pRow = GetRow(row);
  if (pRow)
    return pRow->SetCell(col, pWidget, Delete);
  return false;
}

bool nuiTable::ClearCell(uint row, uint col, bool Delete)
{
  nuiTableRowPtr pRow = GetRow(row);
  if (pRow)
    return pRow->ClearCell(col, Delete);
  return false;
}

nuiRect nuiTable::CalcIdealSize()
{
  nuiDrawContext* pContext = GetDrawContext();
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  nuiSize x=0,y=0;

  nuiRect HeaderSize;
  if (mDisplayHeaders)
  {
    HeaderSize = mHeader.CalcIdealSize(pContext);
    y+=HeaderSize.GetHeight() + INTERLINE*2;
  }

  for (it = mpRows.begin(); it!=end; ++it)
  {
    if (*it)
    {
      nuiRect rect;
      nuiTableRowPtr pRow = (*it);
      rect = pRow->CalcIdealSize(pContext);
      x = MAX(x, (uint)rect.GetWidth());
      y += rect.GetHeight();
    }
  } 

  if (mDisplayHeaders)
  {
    uint i;
    x = 0;
    for (i = 0; i<mHeader.GetSize(); i++)
    {
      nuiWidgetPtr pItem;
      pItem = mHeader.GetCell(i);
      if (pItem)
      {
        nuiSize ww = pItem->GetProperty(_T("ColumnWidth")).GetCFloat(); 
        x+=ww;
      }
    }
  }

  mIdealRect.Set(0.0f,0.0f,x,y);
  return mIdealRect;
}

bool nuiTable::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  list<nuiSize> Sizes;
  nuiSize y = 0;
  nuiSize x = 0;
  nuiSize w = 0;
  nuiSize h = 0;

  // Get header sizes:
  uint i;
  for (i = 0; i<mHeader.GetSize(); i++)
  {
    nuiWidgetPtr pItem;
    pItem = mHeader.GetCell(i);
    if (pItem)
    {
      nuiSize ww = pItem->GetProperty(_T("ColumnWidth")).GetCFloat(); 
      Sizes.push_back(ww);
      x+=ww;
      w+=ww;
    }
    else
      Sizes.push_back(0);
  }

  if (mDisplayHeaders)
  {
    mHeader.SetRects(&Sizes,0,0);
    y += mHeader.GetTotalHeight();
  }

  // Set row sizes:
  for (it = mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pRow = *it;
    if (pRow)
    {
      nuiSize hh = pRow->GetTotalHeight();
      pRow->SetRects(&Sizes,0, y);
      y += hh;
      h += hh;
    }
  }  

  return true;
}

bool nuiTable::Draw(nuiDrawContext* pContext)
{
  int xstart=-1,xstop=-1;
  nuiSize x=0,y=0,w=0;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  pContext->ResetState();

  nuiRect size = mRect.Size();

  uint i;
  for (i = 0; i<mHeader.GetSize() && w < mRect.GetWidth(); i++)
  {
    nuiWidgetPtr pItem;
    pItem = mHeader.GetCell(i);
    if (pItem)
    {
      nuiSize ww = pItem->GetProperty(_T("ColumnWidth")).GetCFloat();
      if (w+ww+x >= 0 && xstart<0)
        xstart = i;
      if (w+x <=  mRect.GetWidth())
        xstop = i+1;
      w+=ww;
    }
  }

  if (mDisplayHeaders)
  {
    mHeader.Draw(pContext, xstart, xstop,mSelectedCol,true);
    y += mHeader.GetTotalHeight();
    size.mTop += mHeader.GetTotalHeight();
    pContext->Clip(size);
  }

  uint j = 0;
  for (it = mpRows.begin(); it!=end && y < mRect.GetHeight(); ++it)
  {
    nuiTableRowPtr pRow = (*it);
    if (pRow)
    {
      nuiSize yy = pRow->GetTotalHeight();
      if (y+yy >= 0)
        pRow->Draw(pContext, xstart, xstop, mSelectedCol, false, (mDisplayCursor?mpCursorRow:NULL));
      y+=yy;
    }
    j++;
  }  

  return true;
}

bool nuiTable::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!IsEnabled())
    return false;

  if (Button & nglMouseInfo::ButtonLeft)
  {
    if (mDisplayHeaders && (Y <= (int)mHeader.GetTotalHeight()))
    {
      int col = GetColumn(X,Y);
      if (col != -1)
      {
        mClicked = true;
        Grab();
        mClickedColumn = col;
        if (X > mHeader.GetCell(col)->GetRect().mRight - HEADERHANDLESIZE)
          mResize = true;
        else
          mResize = false;
        mClickX = X;
        mClickY = Y;
        return true;
      }
    }

    nuiTableRowPtr pItem = GetRowPtr(X,Y);

    if (pItem)
    {
      if (pItem->HandleClick(X ,Y)) /// Does the row 
      {
        mpCursorRow = pItem;
        Invalidate();
        return true;
      }

      nuiRect client = mRect.Size();
      if (GetColumn(X,Y)!=-1 && client.IsInside(X,Y))
      {
        if (mMultiSelectable)
        {
          if (pItem->IsSelected())
            pItem->SetSelected(false);
          else
            pItem->SetSelected(true);
          Clicked(X,Y,Button);
          mpCursorRow = pItem;
          SelectionChanged();
        }
        else
        {
          if (pItem->IsSelected())
          {
            if (Button & nglMouseInfo::ButtonDoubleClick)
            {
              Activated();
              Selected();
              SelectionChanged();
            }
            else
            {
              if (mUnselectable)
              {
                pItem->SetSelected(false);
                SelectionChanged();
              }
              Clicked(X,Y,Button);
            }
          }
          else
          {
            DeselectAll();
            pItem->SetSelected(true);
            mpCursorRow = pItem;
            SelectionChanged();
          }

          mpLastItem = pItem; 
        }
      }
      else return false;
    }
    Invalidate();
    return true;
  }
  return false;
}            

bool nuiTable::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mClicked)
  {
    mClicked = false;
    mResize = false;
    Ungrab();
  }
//  Unclicked(X,Y,Button);
  return false;
}

bool nuiTable::MouseMoved  (nuiSize X, nuiSize Y)
{
  if (!mResize)
  {
    uint i;
    for (i = 0; i < GetColumnCount(); i++)
    {
      nuiWidgetPtr pCell = mHeader.GetCell(i);
      if (pCell)
        pCell->SetMouseCursor(eCursorArrow);
    }
    SetMouseCursor(eCursorArrow);
  }

  if (mClicked)
  {
    if (mResize)
    {
      nuiSize x,y;
      x = X-mClickX;
      y = Y-mClickY;

      nuiSize size = GetColumnSize(mClickedColumn) + x;

      if (size < 10.0f)
        size = 10.0f;

      SetColumnSize(mClickedColumn, size);

      mClickX = X;
      mClickY = Y;
    }
    return true;
  }
  else if (mDisplayHeaders && (Y <= mHeader.GetTotalHeight()))
  {
    int col = GetColumn(X,Y);
    if (col != -1)
    {
      nuiWidgetPtr pCell = mHeader.GetCell(col);
      if (pCell)
      {
        if (X > pCell->GetRect().mRight - HEADERHANDLESIZE)
        {
          SetMouseCursor(eCursorResizeWE);
          pCell->SetMouseCursor(eCursorResizeWE);
        }
      }
      return true;
    }
  }
  return false;
}            

bool nuiTable::GetUnselectable()
{
  return mUnselectable;
}

bool nuiTable::GetMultiSelectable()
{
  return mMultiSelectable;
}

void nuiTable::SetUnselectable(bool set)
{
  mUnselectable = set;
}

void nuiTable::SetMultiSelectable(bool set)
{
  mMultiSelectable = set;
}

uint nuiTable::DeselectAll()
{
  uint count = 0;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem)
    {
      pItem->DeselectAll();
      count++;
    }
  }
  SelectionChanged();
  return count;
}

uint nuiTable::SelectAll()
{              
  uint count = 0;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem)
    {
      pItem->SelectAll();
      count++;
    }
  }
  SelectionChanged();
  return count;
}

uint nuiTable::GetSelected(list<uint>& selitems)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  uint i = 0;
  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem && pItem->IsSelected())
      selitems.push_back(i);
    i++;
  }

  return (uint)selitems.size();
}

uint nuiTable::GetUnselected(list<uint>& unselitems)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  uint i = 0;

  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem && !pItem->IsSelected())
      unselitems.push_back(i);
    i++;
  }
  return (uint)unselitems.size();
}

uint nuiTable::GetSelected(nuiTableRowList& selitems)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  uint i = 0;
  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem && pItem->IsSelected())
      selitems.push_back(pItem);
    i++;
  }

  return (uint)selitems.size();
}

uint nuiTable::GetUnselected(nuiTableRowList& unselitems)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  uint i = 0;

  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem && !pItem->IsSelected())
      unselitems.push_back(pItem);
    i++;
  }
  return (uint)unselitems.size();
}

void nuiTable::ClearTable(bool Delete)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  for (it=mpRows.begin(); it!=end; ++it)
  {
    nuiTableRowPtr pItem = (*it);
    if (pItem)
    {
      pItem->Clear(Delete);
      delete pItem;
    }
  }
  mpRows.clear();
  SelectionChanged();
  mpCursorRow = NULL;
  InvalidateLayout();
}

bool nuiTable::Clear(bool Delete)
{
  ClearTable(Delete);
  return true;
}


nuiTableRowPtr nuiTable::GetRowPtr(nuiSize X, nuiSize Y)
{
  nuiSize y=0;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  if (mDisplayHeaders)
  {
    y += mHeader.GetTotalHeight();
  }

  for (it = mpRows.begin(); it!=end && y < mRect.GetHeight(); ++it)
  {
    nuiTableRowPtr pRow = (*it);
    if (pRow)
    {
      nuiSize yy = pRow->GetTotalHeight();
      if (Y>=y && Y<=y+yy)
        return pRow->GetRow(X,Y-y);
      y+=yy;
    }
  }  
  return NULL;
}

int nuiTable::GetRow(nuiSize X, nuiSize Y)
{
  nuiSize y=0;
  int i = 0;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();

  if (mDisplayHeaders)
  {
    y += mHeader.GetTotalHeight();
  }

  for (it = mpRows.begin(); it!=end && y < mRect.GetHeight(); ++it)
  {
    nuiTableRowPtr pRow = (*it);
    if (pRow)
    {
      nuiSize yy = pRow->GetTotalHeight();
      if (Y>=y && Y<=y+yy)
        return i;
      y+=yy;
    }
  }  
  return -1;
}

bool nuiTable::KeyDown(const nglKeyEvent& rEvent)
{
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  uint i = 0;

  for (it=mpRows.begin(); (it != end) && *it != mpCursorRow; ++it)
    i++;

  mDisplayCursor = true;

  if ((rEvent.mKey == NK_LCTRL) || (rEvent.mKey == NK_RCTRL))
  {
    mMoveOnly = true;
  }
  else if ((rEvent.mKey == NK_LSHIFT) || (rEvent.mKey == NK_RSHIFT))
  {
    if (mMultiSelectable)
    {
      mKeyboardSelect = true;
      mSelectionStart = i;
    }
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mChar=='\n') // Activate Selection
  {
    if (mpCursorRow)
    {
      mpCursorRow->SetSelected(true);
      Activated();
    }
    return true;
  }
  else if (rEvent.mKey == NK_SPACE)
  {
    if (mpCursorRow) 
    {
      bool sel = mpCursorRow->IsSelected();
      if (!mMultiSelectable)
        DeselectAll();
      mpCursorRow->SetSelected(!sel);
      SelectionChanged();
      Invalidate();
    }
  }
  else if (rEvent.mKey == NK_LEFT)
  {
    nuiTableRowPtr pRow = mpCursorRow;
    if (pRow)
    {
      if (pRow->IsOpened())
        pRow->Open(false);
      else
      {
        pRow = pRow->GetParentRow();
        if (pRow)
          mpCursorRow = pRow;
      }
      if (!mMoveOnly)
      {
        if (!mKeyboardSelect)
          DeselectAll();
        if (mpCursorRow)
          mpCursorRow->SetSelected(true);
        SelectionChanged();
      }
      Invalidate();  
    }
  }
  else if (rEvent.mKey == NK_RIGHT)
  {
    nuiTableRowPtr pRow = mpCursorRow;
    if (pRow)
    {
      if (pRow->GetRowCount())
      {
        if (!pRow->IsOpened())
          pRow->Open(true);
        else
        {
          pRow = pRow->GetRow(0);
          if (pRow)
            mpCursorRow = pRow;
        }
      }

      if (!mMoveOnly)
      {
        if (!mKeyboardSelect)
          DeselectAll();
        if (mpCursorRow)
          mpCursorRow->SetSelected(true);
        SelectionChanged();
      }
      Invalidate();
    }
  }
  else if (rEvent.mKey == NK_UP)
  {
    nuiTableRowPtr pRow = NULL;
    if (mpCursorRow)
    {
      pRow = mpCursorRow->GetAbove();
      while (pRow && pRow != mpCursorRow && !pRow->IsVisible())
        pRow = pRow->GetAbove();
    }
    if (pRow)
      mpCursorRow = pRow;

    if (!mMoveOnly)
    {
      if (!mKeyboardSelect)
        DeselectAll();
      if (mpCursorRow)
        mpCursorRow->SetSelected(true);
    }
    Invalidate();
  }
  else if (rEvent.mKey == NK_DOWN)
  {
    if (mpCursorRow)
    {
      nuiTableRowPtr pRow = NULL;
      if (mpCursorRow)
        pRow = mpCursorRow->GetBelow();
      if (pRow)
        mpCursorRow = pRow;
    }
    else
      mpCursorRow = mpRows.front();
    if (!mMoveOnly)
    {
      if (!mKeyboardSelect)
        DeselectAll();
      if (mpCursorRow)
        mpCursorRow->SetSelected(true);
      SelectionChanged();
    }
    Invalidate();
  }
  else if (rEvent.mKey == NK_PAGEUP)
  {
    if (!mpCursorRow)
      return true;
    uint incr = 0;
    nuiTableRowPtr pRow = NULL;
    do
    {
      pRow = mpCursorRow->GetAbove();
      if (pRow)
      {
        mpCursorRow = pRow;
        incr -= (uint)pRow->GetHeight();
      }
    } while ( incr > 0 && pRow );

    mSelection = mKeyboardSelect;
    if (!mMoveOnly)
    {
      if (!mKeyboardSelect)
        DeselectAll();
      mpCursorRow->SetSelected(true);
    }
    Invalidate();
  }
  else if (rEvent.mKey == NK_PAGEDOWN)
  {
    if (!mpCursorRow)
      return true;
    uint incr = 0;
    nuiTableRowPtr pRow = NULL;
    do
    {
      pRow = mpCursorRow->GetBelow();
      if (pRow)
      {
        mpCursorRow = pRow;
        incr -= (uint)pRow->GetHeight();
      }
    } while ( incr > 0 && pRow );

    mSelection = mKeyboardSelect;
    if (!mMoveOnly)
    {
      if (!mKeyboardSelect)
        DeselectAll();
      mpCursorRow->SetSelected(true);
    }
    Invalidate();
  }
  else if (rEvent.mKey == NK_HOME)
  {
    mpCursorRow = mpRows.front();
    mSelection = mKeyboardSelect;
    SelectionChanged();
    Invalidate();
  }
  else if (rEvent.mKey == NK_END)
  {
    mpCursorRow = mpRows.back();
    mSelection = mKeyboardSelect;
    SelectionChanged();
    Invalidate();
  }
  else // Standard text key
  {
  }
//  ShowRow(mpCursorRow);
  return true;
}

bool nuiTable::KeyUp (const nglKeyEvent& rEvent)
{
  if ((rEvent.mKey == NK_LSHIFT) || (rEvent.mKey == NK_RSHIFT))
  {
//    OUT("Not selecting text anymore\n");
    mKeyboardSelect = false;
  }
  else if ((rEvent.mKey == NK_LCTRL) || (rEvent.mKey == NK_RCTRL))
  {
    mMoveOnly = false;
  }
  return true;
}

bool nuiTable::ShowRow(uint32 number)
{
  nuiSize n = 0;
  nuiSize n2 = 0;
  uint32 i = 0;
  nuiTableRowList::iterator it;
  nuiTableRowList::iterator end = mpRows.end();
  for (it = mpRows.begin(); it != end && i <= number; ++it)
  {
    i++;
    n2=n;
    n+=(*it)->GetTotalHeight();
  }

  SetHotRect(nuiRect(mRect.mLeft, n2, mRect.mRight, n, false));

  return true;
}

void nuiTable::OnSetFocus(nuiWidgetPtr pWidget)
{
  nuiSimpleContainer::OnSetFocus(pWidget);

  mDisplayCursor = (pWidget==this);
  Invalidate();
}

nuiWidgetPtr nuiTable::GetChild(nuiSize X, nuiSize Y)
{
  X -= mRect.mLeft;
  Y -= mRect.mTop;

  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideLocal(X,Y))
    {
      delete pIt;
      nuiContainerPtr pContainer = dynamic_cast<nuiContainerPtr>(pItem);
      if (pContainer)
        return pContainer->GetChild(X,Y);
      else 
        return pItem;
    }
  }
  delete pIt;

  return this;
}

////// Private event management:
bool nuiTable::DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!IsEnabled())
    return false;

  if (IsInside(X,Y))
  {
    IteratorPtr pIt;
    for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (pItem)
      {
        if (IsEnabled())
          if (pItem->DispatchMouseClick(X,Y, Button))
          {
            delete pIt;
            return true;
          }
      }
    }
    delete pIt;

    GlobalToLocal(X,Y);
    bool ret = MouseClicked(X,Y,Button);
    ret |= Clicked(X,Y,Button);
    return ret;
  }
  return false;
}

bool nuiTable::DispatchMouseUnclick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!IsEnabled())
    return false;

  if (IsInside(X,Y))
  {
    IteratorPtr pIt;
    for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (pItem)
      {
        if (!IsEnabled())
          if ((pItem)->DispatchMouseUnclick(X,Y, Button))
          {
            delete pIt;
            return true;
          }
      }
    }
    delete pIt;

    GlobalToLocal(X,Y);
    bool res = PreUnclicked(X,Y, Button);
    if (!res)
    {
      res = MouseUnclicked(X,Y,Button);
      res |= Unclicked(X,Y,Button);
    }
    
    if (mWantKeyboardFocus && (Button == nglMouseInfo::ButtonLeft || Button == nglMouseInfo::ButtonRight))
      Focus();
    
    return res;
  }
  return false;
}

void nuiTable::DrawTableChild(nuiDrawContext* pContext, nuiWidget* pWidget)
{
  DrawChild(pContext, pWidget);
}

