/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include <list>
#include "nuiFileTable.h"
#include "nuiLabel.h"
#include "nglPath.h"

using namespace std;

nuiFileTable::nuiFileTable(const nglPath& rPath)
: nuiTable(),
  mFileTableSink(this)
{
  SetObjectClass(_T("nuiFileTable"));
  
  mShowFiles = true;
  mShowDirs = true;
  
  mShowSize = true;
  mShowType = true;
  
  mNameColumn = 0;
  mSizeColumn = 1;
  mTypeColumn = 2;
  
  InsertColumn (mNameColumn,_T("File name"));
  SetColumnSize(mNameColumn,150);
  InsertColumn (mSizeColumn,_T("Size"));
  InsertColumn (mTypeColumn,_T("Type"));
  SetColumnSize(mTypeColumn,150);
  
  Populate(rPath);
  
  mFileTableSink.Connect(Activated, &nuiFileTable::Selected, this); 
}


bool nuiFileTable::Load(const nuiXMLNode* pNode)
{
  nuiTable::Load(pNode);
  SetObjectClass(_T("nuiFileTable"));
  nglString path;
  path = nuiGetString(pNode, _T("Path"), _T("."));
  if (path.GetRight(2) == _T("/.") || path.GetRight(2) == _T("\\."))
    path.DeleteRight(2);
  else if (path == _T("."))
    path = nglPath(ePathCurrent).GetPathName();

  mShowFiles = nuiGetBool(pNode,_T("ShowFiles"),true);
  mShowDirs = nuiGetBool(pNode,_T("ShowDirs"),true);

  mShowSize = nuiGetBool(pNode,_T("ShowSize"),true);
  mShowType = nuiGetBool(pNode,_T("ShowType"),true);

  mNameColumn = 0;
  mSizeColumn = 1;
  mTypeColumn = 2;

  InsertColumn (mNameColumn,_T("File name"));
  SetColumnSize(mNameColumn,150);
  if (mShowSize)
    InsertColumn (mSizeColumn,_T("Size"));
  if (mShowType)
  {
    InsertColumn (mTypeColumn,_T("Type"));
    SetColumnSize(mTypeColumn,150);
  }

  Populate(nglPath(path));

  mFileTableSink.Connect(Activated, &nuiFileTable::Selected, this);
  
  return true;
}

nuiFileTable::~nuiFileTable()
{
}

nuiXMLNode* nuiFileTable::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiTable::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;
  pNode->SetAttribute(_T("Path"),GetProperty(_T("Path")));
  return pNode;
}

bool nuiFileTable::Selected(const nuiEvent& rEvent)
{
  nuiTableRowList Items;
  if (GetSelected(Items))
  {
    nuiObjectPtr pItem = Items.front();
    if (pItem)
    {
      nglPath path(pItem->GetProperty(_T("Path")));
      if (!path.IsLeaf())
      {
//        NGL_OUT(_T("Exploring %ls\n"),path.GetChars());
        Clear();
      
        Populate(path);
      }
      else
      {
//        NGL_OUT(_T("'%ls' is a file:\nSize: %d\nType: %ls\n\n"),path.GetChars(),path.GetSize(),path.GetMimeType().GetChars());
      }
    }
//    else
//      NGL_OUT(_T("Selected item is NULL!!! (?)\n"));
  }
//  else
//    NGL_OUT(_T("?!? No item selected!?!\n"));

  return false;
}

class nglFileTable_sort : public binary_function<nglPath&, nglPath&, bool>
{
public:
  bool operator ()(nglPath& rA, nglPath& rB)
  {
    return rA.GetPathName().Compare(rB.GetPathName(),false) < 0;
  }
};

bool nuiFileTable::Populate(const nglPath& rPath)
{
  uint count = 0;
  list<nglPath> FileList;

  mPath = rPath;
  rPath.GetChildren(&FileList);
  FileList.sort(nglFileTable_sort());

  list<nglPath>::iterator it;
  list<nglPath>::iterator end = FileList.end();

  if (mShowDirs)
  {
    for (it = FileList.begin(); it != end; ++it)
    {
      if (!(*it).IsLeaf())
      {
        nuiWidgetList pList;
        pList.push_back(new nuiLabel(_T("[ ")+(*it).GetNodeName()+_T(" ]")));
        if (mShowSize)
          pList.push_back(NULL);
        if (mShowType)
        {
          nuiLabel* pType = new nuiLabel(_T("Directory"));
          pType->SetPosition(nuiCenter);
          pList.push_back(pType);
        }
        nuiObjectPtr pObj = InsertRow(count,pList);
        if (pObj)
        {
          pObj->SetProperty(_T("Path"),(*it).GetPathName());
          count++;
        }
      }
    }
  }

  if (mShowFiles)
  {
    for (it = FileList.begin(); it != end; ++it)
    {
      nuiWidgetList pList;
      if ((*it).IsLeaf())
      {
        pList.push_back(new nuiLabel((*it).GetNodeName()));
        if (mShowSize)
        {
          nglString size;
          size.Format("%i",(*it).GetSize());
          nuiLabel* pSize = new nuiLabel(size);
          pSize->SetFont(nuiTheme::Fixed);
          pSize->SetPosition(nuiRight);
          pList.push_back(pSize);
        }

        if (mShowType)
        {
          nuiLabel* pType;
          nglString type = (*it).GetMimeType();
          if (type.GetLength())
            pType = new nuiLabel(type);
          else
            pType = new nuiLabel(_T("unknown type"));
          pType->SetPosition(nuiCenter);
          pList.push_back(pType);
        }

        nuiObjectPtr pObj = InsertRow(count,pList);
        if (pObj)
        {
          pObj->SetProperty(_T("Path"),(*it).GetPathName());
          count++;
        }
      }
    }
  }

  if (mShowDirs)
  {
    nuiObjectPtr pObj= InsertRow(0);
    pObj->SetProperty(_T("Path"),rPath.GetParent().GetPathName());
    SetCell(0,0, new nuiLabel(_T("..")));
  }

//  mCursorLine = 0;
  mpCursorRow = mpRows.front();
  SetProperty(_T("Path"),rPath.GetPathName());
  return true;
}

nglPath nuiFileTable::GetPath()
{
  return mPath;
}

void nuiFileTable::SetShowSize(bool Show)
{
  if (mShowSize == Show)
    return;
  Clear();
  if (Show)
  {
    mSizeColumn = 1;
    mTypeColumn = 2;
    InsertColumn(mSizeColumn,_T("Size"));
  }
  else
  {
    RemoveColumn(mSizeColumn);
    mSizeColumn = -1;
    mTypeColumn = 1;
  }
  mShowSize = Show;
  Populate(nglPath(GetProperty(_T("Path"))));
  InvalidateLayout();
}

bool nuiFileTable::GetShowSize()
{
  return mShowSize;
}

void nuiFileTable::SetShowType(bool Show)
{
  if (mShowType == Show)
    return;
  Clear();
  mTypeColumn = mShowSize?2:1;
  if (Show)
  {
    InsertColumn(mTypeColumn,_T("Type"));
    SetColumnSize(mTypeColumn, 150);
  }
  else
    RemoveColumn(mTypeColumn);
  mShowType = Show;
  Populate(nglPath(GetProperty(_T("Path"))));
  InvalidateLayout();
}

bool nuiFileTable::GetShowType()
{
  return mShowType;
}

void nuiFileTable::SetShowDirs(bool Show)
{
  if (mShowDirs == Show)
    return;
  mShowDirs = Show;
  Clear();
  Populate(nglPath(GetProperty(_T("Path"))));
}

bool nuiFileTable::GetShowDirs()
{
  return mShowDirs;
}

void nuiFileTable::SetShowFiles(bool Show)
{
  if (mShowFiles == Show)
    return;
  mShowFiles = Show;
  Clear();
  Populate(nglPath(GetProperty(_T("Path"))));
}

bool nuiFileTable::GetShowFiles()
{
  return mShowFiles;
}


bool nuiFileTable::KeyDown(const nglKeyEvent& rEvent)
{
  if ((rEvent.mKey == NK_BACKSPACE))
  {
    if (mShowDirs)
    {
      DeselectAll();
      mpRows.front()->SetSelected(true);
      Activated();
      return true;
    }
  }
  return nuiTable::KeyDown(rEvent);
}

