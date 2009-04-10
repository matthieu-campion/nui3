/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiApplication.h"
#include "nuiList.h"
#include "nuiDrawContext.h"
#include "nuiMainWindow.h"
#include "nuiXML.h"
#include "nuiLabel.h"

using namespace std;

nuiList::nuiList(nuiOrientation Orientation)
  : nuiSimpleContainer(),
    mEventSink(this)
{
  SetObjectClass(_T("nuiList"));
  SetMouseCursor(eCursorHand);
  mBorderSize = mDefaultBorderSize;
  mOrientation = Orientation;
  mMultiSelectable = false;
  mUnselectable = false;
  mFixedAspectRatio = false;
  mpLastItem = NULL;
  mpLastDestinationItem = NULL;
  mClicked = false;
  mMoved = false;
  mCanMoveItems = false;
  mpGrabedItem = NULL;

  mCursorLine = 0;
  mSelection = false;
  mKeyboardSelect = false;
  mDisplayCursor = false;
  mMoveOnly = false;
  mSelectionStart = 0;

  SetWantKeyboardFocus(true);

  InitProperties();
  mEventSink.Connect(ChildAdded, &nuiList::OnChildAdded);
  mMoveAnimDuration = .25;
  mMoveAnimEasing = nuiEasingCubicRev;
}

bool nuiList::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiList"));
  SetMouseCursor(eCursorHand);
  mEventSink.SetTarget(this);
  mBorderSize = nuiGetVal ( pNode, _T("BorderSize"), mDefaultBorderSize);
  mOrientation = nuiGetOrientation ( pNode, nuiVertical);
  mMultiSelectable = pNode->GetAttribute(nglString(_T("MultiSelectable"))) == _T("true");
  mUnselectable = pNode->GetAttribute(nglString(_T("Unselectable"))) == _T("true");
  mFixedAspectRatio = pNode->GetAttribute(nglString(_T("FixedAspectRatio"))) != _T("false");
  mpLastItem =  NULL;
  mpLastDestinationItem = NULL;
  mClicked = false;
  mMoved = false;
  mCanMoveItems = false;
  mpGrabedItem = NULL;

  mCursorLine = 0;
  mSelection = false;
  mKeyboardSelect = false;
  mDisplayCursor = false;
  mMoveOnly = false;
  mSelectionStart = 0;

  InitProperties();
  
  return true;
}

void nuiList::InitProperties()
{  
  AddEvent(_T("ListClicked"), Clicked);
  AddEvent(_T("ListDoubleClicked"), DoubleClicked);
  NUI_ADD_EVENT(Activated);
  NUI_ADD_EVENT(SelectionChanged);
}

nuiList::~nuiList()
{
}

bool nuiList::Draw(nuiDrawContext* pContext)
{
  nuiTheme *pTheme = GetTheme();

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
    {
      if (pItem->IsSelected())
        pTheme->DrawSelectionBackground(pContext, pItem->GetRect(), pItem);

      DrawChild(pContext, pItem);

      if (pItem->IsSelected())
        pTheme->DrawSelectionForeground(pContext, pItem->GetRect(), pItem);
    }
  }
  delete pIt;

  pTheme->Release();
  return true;
}

nuiRect nuiList::CalcIdealSize()
{
  nuiRect rect;
  nuiSize Height=0,Width=0;

  mPositions.clear();
  if (mOrientation == nuiVertical)
  {
    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      nuiRect Rect;
      Rect = pItem->GetIdealRect();
      Rect.RoundToAbove();

      mPositions.push_back(Rect);
      nuiSize w = Rect.GetWidth();
      nuiSize h = Rect.GetHeight();
      Width = MAX(Width, w+2);
      Height +=h;
      Height = (nuiSize)ToAbove(Height);
    }
    delete pIt;
  }
  else
  {
    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      nuiRect Rect;
      Rect = pItem->GetIdealRect();
      Rect.RoundToAbove();

      mPositions.push_back(Rect);
      nuiSize w=Rect.GetWidth();
      nuiSize h=Rect.GetHeight();

      Height = MAX(Height, h+2);
      Width += w;
      Width = (nuiSize)ToAbove(Width);
    }
    delete pIt;
  }

  rect.Set(mRect.mLeft, mRect.mTop, Width, Height);
  mPositions.push_front(rect);
//  OUT("Ideal Size: %d x %d\n",Width,Height);
  mIdealRect = rect;

  return mIdealRect;
}

bool nuiList::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  nuiSize Height = (nuiSize)rRect.GetHeight();
  nuiSize Width = (nuiSize)rRect.GetWidth();
  nuiSize pageincr = 0, value = 0;
  nuiRect Rect;
  nuiSize w,h;

  list<nuiRect>::iterator rit=mPositions.begin();

  Rect=(*rit);

  ++rit;
  if (mOrientation == nuiVertical)
  {
    pageincr = rRect.GetHeight() - 2.0f;

    Height = (nuiSize)-value;

    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      Rect=(*rit);
      if (mFixedAspectRatio)
      {
        nuiSize rratio,rw,rh;
        rratio = (nuiSize)Rect.GetWidth() / (nuiSize)Rect.GetHeight();

        rw = (nuiSize)Width;
        rh = rw / rratio;

        h=rh;

      }
      else
      {
        h=(nuiSize)Rect.GetHeight();
      }
      
      Rect.Set(mBorderSize + 1,Height,Width - 2*mBorderSize -2,h);
      Rect.RoundToAbove();

      Height += h + mBorderSize;
      Height = (nuiSize)ToAbove(Height);
      pItem->SetLayout(Rect);
      ++rit;
    }
    delete pIt;
  }
  else
  {
    pageincr = rRect.GetWidth() - 2.0f;

    Width = (nuiSize) -value;
    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      Rect=(*rit);
      if (mFixedAspectRatio)
      {
        nuiSize rratio,rw,rh;
        rratio = (nuiSize)Rect.GetWidth() / (nuiSize)Rect.GetHeight();

        rh = (nuiSize)Height;
        rw = rh * rratio;
        
        w=rw;
      }
      else
      {
        w=(nuiSize)(int)((nuiSize)Rect.GetWidth());
      }
      
      Rect.Set(Width, mBorderSize+1, w, Height - 2*mBorderSize -2);
      Rect.RoundToAbove();
      Width += w + mBorderSize;
      Width = (nuiSize)ToAbove(Width);
      pItem->SetLayout(Rect);
      ++rit;
    }
    delete pIt;
  }

  return true;
}

bool nuiList::CanMoveItems()
{
  return mCanMoveItems;
}

void nuiList::SetCanMoveItems(bool set)
{
  mCanMoveItems = set;
}


void nuiList::SetOrientation(nuiOrientation orientation)
{
  mOrientation = orientation;
  InvalidateLayout();
}

nuiOrientation nuiList::GetOrientation()
{
  return mOrientation;
}

bool nuiList::GetUnselectable()
{
  return mUnselectable;
}

bool nuiList::GetMultiSelectable()
{
  return mMultiSelectable;
}

void nuiList::SetUnselectable(bool set)
{
  mUnselectable = set;
}

void nuiList::SetMultiSelectable(bool set)
{
  mMultiSelectable = set;
}

uint nuiList::DeselectAll()
{
  uint count = 0;
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsSelected())
    {
      pItem->SetSelected(false);
      count++;
    }
  }
  delete pIt;
  Invalidate ();
  return count;
}

uint nuiList::SelectAll()
{              
  uint count = 0;
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();

    if (!pItem->IsSelected())
    {
      pItem->SetSelected(true);
      count++;
    }
  }
  delete pIt;
  return count;
}

uint nuiList::GetSelected(nuiWidgetList& selitems)
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsSelected())
      selitems.push_back(pItem);
  }
  delete pIt;

  return selitems.size();
}


nuiWidget* nuiList::GetSelected ()
{
 IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsSelected())
    {
      delete pIt;
      return pItem;
    }
  }
  delete pIt;
  return NULL;
}



nuiTokenBase* nuiList::GetSelectedToken ()
{
 IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsSelected())
    {
      nuiTokenBase* token = pItem->GetToken();
      delete pIt;
      return token;
    }
  }
  delete pIt;
  return NULL;
}



uint nuiList::GetUnselected(nuiWidgetList& unselitems)
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (!pItem->IsSelected())
      unselitems.push_back(pItem);
  }
  delete pIt;
  return unselitems.size();
}



nuiSize nuiList::GetBorderSize()
{
  return mBorderSize;
}

void nuiList::SetBorderSize(nuiSize BorderSize)
{
  if (mBorderSize == BorderSize)
    return;
  mBorderSize = BorderSize;
  InvalidateLayout();
}

nuiSize nuiList::mDefaultBorderSize = 0;

nuiSize nuiList::GetDefaultBorderSize()
{
  return mDefaultBorderSize;
}

void nuiList::SetDefaultBorderSize(nuiSize BorderSize)
{
  mDefaultBorderSize = BorderSize;
}

nuiWidgetPtr nuiList::GetItem(nuiSize X,nuiSize Y)
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->GetRect().IsInside(X,Y))
    {
      delete pIt;
      return pItem;
    }
  }
  delete pIt;
  return NULL;
}

nuiWidgetPtr nuiList::GetIdealItem(nuiSize X,nuiSize Y)
{
  if (mOrientation == nuiVertical)
  {
    IteratorPtr pIt;
    float y = 0;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      nuiRect r(pItem->GetIdealRect());

      if (Y >= y && Y < (y + r.GetHeight()))
      {
        delete pIt;
        return pItem;
      }
      y += r.GetHeight();
    }
    delete pIt;
  }
  else if (mOrientation == nuiHorizontal)
  {
    IteratorPtr pIt;
    float x = 0;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      nuiRect r(pItem->GetIdealRect());
      if (X >= x && X < x + r.GetWidth())
      {
        delete pIt;
        return pItem;
      }
      x += r.GetWidth();
    }
    delete pIt;
  }
  
  return NULL;
}


bool nuiList::KeyDown(const nglKeyEvent& rEvent)
{
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
      mSelectionStart = mCursorLine;
    }
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mChar=='\n') // Activate Selection
  {
    Activated();
    return true;
  }
  else if (rEvent.mKey == NK_SPACE)
  {
    if (!mMultiSelectable)
      DeselectAll();

    if (mpChildren[mCursorLine]->IsSelected())
      mpChildren[mCursorLine]->SetSelected(false);
    else if (!mpChildren[mCursorLine]->IsSelected())
      mpChildren[mCursorLine]->SetSelected(true);

    SelectionChanged();
    Invalidate();
  }
  else if (rEvent.mKey == NK_UP)
  {
    if (mCursorLine > 0)
    {
      mCursorLine--;

      if (!mMoveOnly)
      {
        if (!mKeyboardSelect)
          DeselectAll();
        mpChildren[mCursorLine]->SetSelected(true);
      }
      SelectionChanged();
      Invalidate();
    }
  }
  else if (rEvent.mKey == NK_DOWN)
  {
    if ((int32)mCursorLine < (int32)(mpChildren.size() - 1))
    {
      mCursorLine++;
      if (!mMoveOnly)
      {
        if (!mKeyboardSelect)
          DeselectAll();
        mpChildren[mCursorLine]->SetSelected(true);
        Selected();
      }
      SelectionChanged();
      Invalidate();
    }
  }
  else if (rEvent.mKey == NK_PAGEUP)
  {
    uint incr = 0; //(uint)mpScrollBar->GetRange().GetPageIncrement();
    if (mCursorLine < incr)
      mCursorLine = 0;
    else
      mCursorLine -= incr;
    mSelection = mKeyboardSelect;
    if (!mMoveOnly)
    {
      if (!mKeyboardSelect)
        DeselectAll();
      mpChildren[mCursorLine]->SetSelected(true);
    }
    SelectionChanged();
    Invalidate();
  }
  else if (rEvent.mKey == NK_PAGEDOWN)
  {
    uint incr = 0;//(uint)mpScrollBar->GetRange().GetPageIncrement();
    if (mCursorLine + incr >= mpChildren.size())
      mCursorLine = mpChildren.size() - 1;
    else
      mCursorLine += incr;

    mSelection = mKeyboardSelect;
    if (!mMoveOnly)
    {
      if (!mKeyboardSelect)
        DeselectAll();
      mpChildren[mCursorLine]->SetSelected(true);
    }
    SelectionChanged();
    Invalidate();
  }
  else if (rEvent.mKey == NK_HOME)
  {
    mCursorLine = 0;
    mSelection = mKeyboardSelect;
    Invalidate();
    SelectionChanged();
    ShowRow(mCursorLine);
    return true;
  }
  else if (rEvent.mKey == NK_END)
  {
    if ((mCursorLine = mpChildren.size()))
    {
      mCursorLine--;
    }
    mSelection = mKeyboardSelect;
    Invalidate();
    SelectionChanged();
    ShowRow(mCursorLine);
    return true;
  }
  else // Standard text key
  {
  }
  ShowRow(mCursorLine);
  return false;
}

bool nuiList::KeyUp(const nglKeyEvent& rEvent)
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

int32 nuiList::GetItemNumber(nuiWidgetPtr pWidget)
{
  int32 count = 0; 
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem == pWidget)
    {
      delete pIt;
      return count;
    }
    count++;
  }
  delete pIt;
  return -1;
}


bool nuiList::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (IsDisabled())
    return false;

  if (Button & nglMouseInfo::ButtonLeft)
  {
    nuiWidgetPtr pItem = GetItem(X,Y);
    if (pItem && pItem->IsEnabled(false))
    {
      mCursorLine = GetItemNumber(pItem);
      if (mMultiSelectable)
      {
        pItem->SetSelected(!pItem->IsSelected());
        Clicked(X,Y,Button);
      }
      else
      {
        if (!pItem->IsSelected())
        {
          DeselectAll();
          pItem->SetSelected(true);
        }
        else
        {
          if (Button & nglMouseInfo::ButtonDoubleClick)
          {
            DoubleClicked(X,Y,Button);
            Activated();
          }
          else
          {
            if (mUnselectable)
              pItem->SetSelected(false);
            Clicked(X,Y,Button);
          }
        }
        mpLastItem = pItem; 

        Grab();
        mpGrabedItem = pItem;
        mClicked = true;
      }
      SelectionChanged();
      Invalidate();
    }
    return true;

  }
  return false;
}            


bool nuiList::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mClicked && (Button & nglMouseInfo::ButtonLeft))
  {
    mpGrabedItem = NULL;
    Ungrab();
  }
  
  mClicked = false;
  
  if (mMoved && mCanMoveItems)
  {
    mMoved = false;
    // send signal
    ItemMoved(mpLastItem, mpLastDestinationItem);
  }
  return false;
}

bool nuiList::MouseMoved  (nuiSize X, nuiSize Y)
{
  if (!mClicked)
    return false;
  
  if (!mCanMoveItems)
    return false;
  
  nuiWidgetPtr pItem = GetIdealItem(X,Y);

  if ((pItem == mpLastItem) || (pItem == mpLastDestinationItem))
    return false;
  
  mpLastDestinationItem = pItem;

  // reorganize children
  MoveChild(mpLastItem, mpLastDestinationItem);
  
  mMoved = true;
  
  UpdateLayout();
  
  return false;
}            
      
void nuiList::MoveChild(nuiWidget* pSelectedChild, nuiWidget* pDestinationChild)
{
  nuiWidgetList::iterator it;
  for (it = mpChildren.begin(); it != mpChildren.end(); ++it)
  {
    nuiWidget* pItem = *it;
    if (pItem == pSelectedChild)
    {
      mpChildren.erase(it);
      break;
    }
  }
  
  if (!pDestinationChild)
  {
    mpChildren.push_back(pSelectedChild);
    return;
  }
  
  for (it = mpChildren.begin(); it != mpChildren.end(); ++it)
  {
    nuiWidget* pItem = *it;
    if (pItem == pDestinationChild)
    {
      mpChildren.insert(it, pSelectedChild);
      break;
    }
  }
  
}

void nuiList::SelectItem(uint ItemNumber)
{
  nuiWidgetPtr pItem = NULL;
  int32 i = 0;
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid() && i <= ItemNumber; GetNextChild(pIt))
  {
    pItem = pIt->GetWidget();
    i++;
  }
  delete pIt;

  if (pItem && i == ItemNumber+1)
    SelectItem(pItem);
  mCursorLine = ItemNumber;
}



void nuiList::SelectItemSilent(uint ItemNumber)
{
  nuiWidgetPtr pItem = NULL;
  int32 i = 0;
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid() && i <= ItemNumber; GetNextChild(pIt))
  {
    pItem = pIt->GetWidget();
    i++;
  }
  delete pIt;

  if (pItem && i == ItemNumber+1)
    SelectItemSilent(pItem);
}


void nuiList::SelectItem(nuiWidgetPtr pItem)
{
  if (pItem && pItem->IsEnabled(false))
  {
    if (mMultiSelectable)
    {
      pItem->SetSelected(!pItem->IsSelected());
    }
    else
    {
      if (pItem->IsSelected())
      {
        if (mUnselectable)
          pItem->SetSelected(false);
      }
      else
      {
        DeselectAll();
        pItem->SetSelected(true);
      }
      mpLastItem = pItem; 
    }

		SelectionChanged();
    Invalidate();
  }

  mCursorLine = GetItemNumber(pItem);
}



void nuiList::SelectItemSilent(nuiWidgetPtr pItem)
{
  if (pItem && pItem->IsEnabled(false))
  {
    if (mMultiSelectable)
    {
      pItem->SetSelected(!pItem->IsSelected());
      mCursorLine = GetItemNumber(pItem);
    }
    else
    {
      if (pItem->IsSelected())
      {
        if (mUnselectable)
          pItem->SetSelected(false);
      }
      else
      {
        DeselectAll();
        pItem->SetSelected(true);
      }
      mpLastItem = pItem; 
    }

		// silent...
    //SelectionChanged();
    
    Invalidate();
  }
}




// nuiList path management helper:
bool nuiList::PopulateFiles(const nglPath& rPath)
{
  list<nglPath> FileList;

  rPath.GetChildren(&FileList);

  list<nglPath>::iterator it;
  list<nglPath>::iterator end = FileList.end();

  for (it = FileList.begin(); it != end; ++it)
  {
    if ((*it).IsLeaf())
    {
      nuiLabel* pLabel = new nuiLabel((*it).GetNodeName());
      pLabel->SetProperty(_T("Path"),(*it).GetAbsolutePath().GetPathName());
      AddChild(pLabel);
    }
  }

  return true;
}

bool nuiList::PopulateDirs(const nglPath& rPath)
{
  list<nglPath> FileList;

  rPath.GetChildren(&FileList);

  list<nglPath>::iterator it;
  list<nglPath>::iterator end = FileList.end();

  for (it = FileList.begin(); it != end; ++it)
  {
    if (!(*it).IsLeaf())
    {
      nuiLabel* pLabel = new nuiLabel(_T("[ ")+(*it).GetNodeName()+_T(" ]"));
      pLabel->SetProperty(_T("Path"),(*it).GetAbsolutePath().GetPathName());
      AddChild(pLabel);
    }
  }

  SetProperty(_T("Path"),rPath.GetAbsolutePath().GetPathName());
  return true;
}

bool nuiList::Populate(const nglPath& rPath, bool Files, bool Dirs)
{
  list<nglPath> FileList;

  rPath.GetChildren(&FileList);

  list<nglPath>::iterator it;
  list<nglPath>::iterator end = FileList.end();

  if (Dirs)
  {
    for (it = FileList.begin(); it != end; ++it)
    {
      if (!(*it).IsLeaf())
      {
        nuiLabel* pLabel = new nuiLabel(_T("[ ")+(*it).GetNodeName()+_T(" ]"));
        pLabel->SetProperty(_T("Path"),(*it).GetAbsolutePath().GetPathName());
        AddChild(pLabel);
      }
    }
  }

  if (Files)
  {
    for (it = FileList.begin(); it != end; ++it)
    {
      if ((*it).IsLeaf())
      {
        nuiLabel* pLabel = new nuiLabel((*it).GetNodeName());
        pLabel->SetProperty(_T("Path"),(*it).GetAbsolutePath().GetPathName());
        AddChild(pLabel);
      }
    }
  }

  SetProperty(_T("Path"),rPath.GetAbsolutePath().GetPathName());
  return true;
}

bool nuiList::ShowRow(int32 number)
{
  if (number >= mpChildren.size())
    return false;
  nuiWidget* pItem = mpChildren[number];
  SetHotRect(pItem->GetRect());
  return true;
}


void nuiList::SetFixedAspectRatio(bool flag)
{
  if (mFixedAspectRatio == flag)
    return;

  mFixedAspectRatio = flag;
  InvalidateLayout();
}

bool nuiList::GetFixedAspectRatio()
{
  return mFixedAspectRatio;
}

bool nuiList::OnChildAdded(const nuiEvent& rEvent)
{
  if (mMoveAnimDuration)
  {
    const nuiTreeEvent<nuiWidget>& rTreeEvent((const nuiTreeEvent<nuiWidget>&)rEvent);
    rTreeEvent.mpChild->SetLayoutAnimationDuration(mMoveAnimDuration);
    rTreeEvent.mpChild->SetLayoutAnimationEasing(mMoveAnimEasing);
  }
  return false;
}

void nuiList::SetMoveAnimationDuration(float duration)
{
  mMoveAnimDuration = duration;
  SetChildrenLayoutAnimationDuration(duration);
}

void nuiList::SetMoveAnimationEasing(const nuiEasingMethod& rMethod)
{
  mMoveAnimEasing = rMethod;
  SetChildrenLayoutAnimationEasing(rMethod);
}
