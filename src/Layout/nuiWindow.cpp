/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define RAWFLAG 8
#define NOCAPTIONFLAG 16
#define WINDOW_MINIMOVE 3 // This is the minimum mouse displacement needed to go to window moving mode.

const nuiWindowFlags nuiWindow::NoFlag = 0;
const nuiWindowFlags nuiWindow::StaticChildren = 1;
const nuiWindowFlags nuiWindow::NoMove = 2;
const nuiWindowFlags nuiWindow::NoResize = 4;
const nuiWindowFlags nuiWindow::NoClose = 32;
const nuiWindowFlags nuiWindow::Modal = 64;
const nuiWindowFlags nuiWindow::DecoratedBackground = 128;

const nuiWindowFlags nuiWindow::NoCaption = NOCAPTIONFLAG | NoClose;
const nuiWindowFlags nuiWindow::Raw = RAWFLAG | NoCaption;

nuiWindow::nuiWindow()
: nuiSimpleContainer(),
  mNuiWindowSink(this),
mpManager(NULL)
{
  mpCloseButton = NULL;
  mMoving = eNoMove;
  
  mMinimumHeight = 32;
  mMinimumWidth = 64;
  SetFlags(NoFlag);
  
  InitProperties();
}


nuiWindow::nuiWindow(const nuiRect& rRect, nuiWindowFlags Flags, nglString Title)
  : nuiSimpleContainer(),
    mNuiWindowSink(this)
{
  nuiRect rect = rRect;
  SetUserRect(rect);

  mpCloseButton = NULL;
  mMoving = eNoMove;

  mMinimumHeight = 32;
  mMinimumWidth = 64;
  SetFlags(Flags);

  mTitle = Title;
  InitProperties();
}

nuiWindow::~nuiWindow()
{
}

void nuiWindow::InitProperties()
{
  SetOverDraw(16, 0, 16, 16);

  SetObjectClass(_T("nuiWindow"));
  
  // Standard anims:
  nuiWidgetAnimation* pScaleAnim = new nuiScaleToMatrixWidgetAnim(this, 0.3, .01f);
  pScaleAnim->SetEasing(&nuiEasingCubic);
  nuiWidgetAnimation* pFadeAnim = new nuiFadeOutWidgetAnim(this, 0.3, false);
  pFadeAnim->SetEasing(&nuiEasingSinusStartFast);
  nuiMetaAnimation* pAnim = new nuiMetaAnimation();

  pAnim->AddAnimation(pScaleAnim);
  pAnim->AddAnimation(pFadeAnim);

  AddAnimation(_T("TRASH"), pAnim);

  SetPosition(nuiNoPosition); // User rect by default
  
  NUI_ADD_EVENT(CloseButtonPressed);
  NUI_ADD_EVENT(FlagsChanged);
}

void nuiWindow::SetFlags (nuiWindowFlags Flags)
{
  mStaticChildren = (bool)(Flags & StaticChildren)!=0;
  mNoResize = (bool)((Flags & NoResize)!=0);
  mNoClose = (bool)((Flags & NoClose)!=0);
  mNoMove = (bool)((Flags & NoMove)!=0);
  mNoCaption = (bool)((Flags & NOCAPTIONFLAG)!=0);
  mRawWindow = (bool)((Flags & RAWFLAG)!=0);
  mModal = (bool)((Flags & Modal)!=0);
  mDecoratedBackground = (bool)((Flags & DecoratedBackground)!=0);

  if (Flags & nuiWindow::NoClose)
  {
    if (mpCloseButton)
      mpCloseButton->Trash();
    mpCloseButton = NULL;
  }
  else
  {
    if (!mpCloseButton)
    {
      mpCloseButton = new nuiButton();
      mpCloseButton->SetObjectClass(_T("nuiCloseButton"));
      mpCloseButton->SetPosition(nuiLeft);
      AddChild(mpCloseButton);
      mpCloseButton->SetSerializeMode(eDontSaveNode);

      mpCloseButton->SetToolTip(_T("Close this Window"));
      mNuiWindowSink.Connect(((nuiButton*)mpCloseButton)->Activated, &nuiWindow::HandleCloseButtonPressed);
    }
  }

  FlagsChanged();
  Invalidate();
}

nuiWindowFlags nuiWindow::GetFlags ()
{
  nuiWindowFlags Flags = NoFlag;
  if (mStaticChildren) Flags |= StaticChildren;
  if (mNoResize) Flags |= NoResize;
  if (mNoClose) Flags |= NoClose;
  if (mNoCaption) Flags |= NoCaption;
  if (mRawWindow) Flags |= Raw;
  if (mModal) Flags |= Modal;
  if (mDecoratedBackground) Flags |= DecoratedBackground;

  return Flags;
}

nuiRect nuiWindow::CalcIdealSize()
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();
  nuiRect rect;
  
  LayoutConstraint  constraint;
  nuiRect r(0.0f, 0.0f, mUserRect.GetWidth(), mUserRect.GetHeight());
  r = GetRectFromFlags(r, GetFlags(), true);
  constraint.mMaxWidth = r.GetWidth();
  constraint.mMaxHeight = r.GetHeight();
  
  for (it=mpChildren.begin(); it != end; ++it)
  {
    nuiWidgetPtr pItem = *it;
    // Children are static, honor their size and position requests.
    if (!mStaticChildren)
      pItem->SetLayoutConstraint(constraint);
    rect.Union(rect,pItem->GetIdealRect());
  }

  mIdealRect = GetRectFromFlags(rect, GetFlags(), true);
  return mIdealRect.Size(); 
}

bool nuiWindow::SetRect(const nuiRect& rRect)
{
  //NGL_OUT(_T("nuiWindow::SetRect\n"));
  nuiWidget::SetRect(rRect);

  nuiRect Rect = GetRectFromFlags(rRect.Size(), GetFlags(), false);

  LayoutConstraint  constraint;
  constraint.mMaxWidth = Rect.GetWidth();
  constraint.mMaxHeight = Rect.GetHeight();

  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();

  for (it=mpChildren.begin(); it != end; ++it)
  {
    nuiWidgetPtr pItem = *it;
    
    if (pItem != mpCloseButton)
    {
      if (mStaticChildren)
      {
        // Children are static, honor their size and position requests.
        pItem->SetLayout(pItem->GetIdealRect());
      }
      else
      {
        // Children are not static, let's ignore their request...
        pItem->SetLayoutConstraint(constraint);
        pItem->GetIdealRect();
        pItem->SetLayout(Rect);
      }
    }
    else
    {
      nuiRect r(Rect);
      r.Move(Rect.GetWidth() - 18, -21);
      r.SetSize((long)12,(long)15);
      pItem->SetLayout(r);
    }
  }

  return true;
}


// Rendering:
bool nuiWindow::Draw(nuiDrawContext* pContext)
{
  pContext->PushState();
  pContext->ResetState();
  nuiWindowManager* pRoot = dynamic_cast<nuiWindowManager*>(mpParent);

  pContext->EnableBlending(false);

  if (!mRawWindow)
  {
    nuiTheme* pTheme = GetTheme();
    NGL_ASSERT(pTheme);
    pTheme->DrawWindowShade(pContext, mRect.Size(), nuiColor(1.0f, 1.0f, 1.0f, GetMixedAlpha()), IsParentActive());

    pContext->PushClipping();
    pContext->Clip(GetRect().Size());
    pContext->EnableClipping(true);

    if (pRoot->GetActiveWindow() == this)
    {
      if (mMoving)
        pTheme->DrawMovingWindow(pContext, this);
      else
        pTheme->DrawActiveWindow(pContext, this);
    }
    else
      pTheme->DrawWindow(pContext,this);

    pTheme->Release();

    pContext->PopClipping();
  }

  DrawChildren(pContext);

  return true;
}

void nuiWindow::OnActivation()
{
}

void nuiWindow::OnDesactivation()
{
}

bool nuiWindow::IsModal()
{
  return mModal;
}

bool nuiWindow::SetModal(bool modal)
{
  mModal=modal;

  FlagsChanged();
  return mModal;
}

bool nuiWindow::IsParentActive() const
{
  nuiWindowManager* pParent = dynamic_cast<nuiWindowManager*>(mpParent);
  if (pParent)
    return (pParent->GetActiveWindow()) == this;
  else
    return false;
}

bool nuiWindow::SetParent(nuiContainerPtr pParent)
{
  bool res = nuiSimpleContainer::SetParent(pParent);
  nuiWindowManager* pManager = dynamic_cast<nuiWindowManager*>(pParent);
  if (pManager)
     mpManager = pManager;
  return res;
}

void nuiWindow::HandleCloseButtonPressed(const nuiEvent& rEvent)
{
  // Destroy the window if there is no handler for the event...
  if (!CloseButtonPressed.GetTargetCount())
    Trash();
  else
    CloseButtonPressed();
}

void nuiWindow::SetTitle(const nglString& rTitle)
{
  mTitle = rTitle;
  Invalidate();
}

nglString nuiWindow::GetTitle()
{
  return mTitle;
}

nuiRect nuiWindow::GetRectFromFlags(const nuiRect& rRect, nuiWindowFlags flags, bool RectIsClient)
{
  nuiRect rect = rRect;
  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);
  pTheme->AdjustWindowRect(rect, flags, RectIsClient);
  pTheme->Release();
  return rect;
}

bool nuiWindow::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  //  OUT("MouseClicked %d - %d\n",X,Y);

  if (Button == nglMouseInfo::ButtonLeft)
  {
    LocalToLocal(GetParent(), X,Y);

    mClickX = X;
    mClickY = Y;

    nuiRect _Rect = GetRect();
    _Rect.Transform(GetMatrix());
    nuiTheme* pTheme = GetTheme();
    NGL_ASSERT(pTheme);
    mClickPos = pTheme->GetWindowPart(_Rect, X, Y, GetFlags(), false);
    pTheme->Release();

    nuiRect wRect = GetRect();
    nuiRect woriginal = wRect;

    switch (mClickPos)
    {
    case nuiCenter:
      if (mNoMove)
      {
        mMoving = eNoMove;
        return true;
      }
      break;
    case nuiTopLeft:
    case nuiBottomLeft:
    case nuiLeft:
    case nuiTopRight:
    case nuiBottomRight:
    case nuiRight:
    case nuiTop:
    case nuiBottom:
      if (mNoResize)
        mMoving = eNoMove;
      break;
    default:
      return true;
      break;
    }
    mMoving = ePreMove;
    SetAlpha(0.5f);

    Invalidate();

    Grab();
    return true;
  }

  return true;
}

bool nuiWindow::MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
//  OUT("MouseUnclicked %d - %d\n",X,Y);

  if (Button == nglMouseInfo::ButtonLeft && mMoving != eNoMove)
  {
    LocalToLocal(GetParent(), X, Y);

    mClickX = X;
    mClickY = Y;
    mMoving = eNoMove;

    Invalidate();

    SetAlpha(1.0f);
    mMoving = eNoMove;
    Ungrab();
    return true;
  }

  return true;
}

bool nuiWindow::MouseMoved(nuiSize X, nuiSize Y)
{
  nuiSize dx, dy;

  LocalToLocal(GetParent(), X, Y);
  dx = X - mClickX;
  dy = Y - mClickY;

  if (mMoving == ePreMove)
  {
    if (abs((int)dx) > WINDOW_MINIMOVE || abs((int)dy) > WINDOW_MINIMOVE)
    {
      mMoving = eMove;
      SetAlpha(0.5f);
    }
    else
      return true;
  }

  mClickX = X;
  mClickY = Y;

  if (mMoving == eMove)
  {
    nuiRect wRect = GetIdealRect();
    wRect.RoundToNearest();
    nuiRect woriginal = wRect;

    switch (mClickPos)
    {
    case nuiCenter:
      {
        wRect.Move(dx, dy);
      }
      break;
    case nuiTopLeft:
      {
        wRect.mTop += dy;
        wRect.mLeft += dx;
        if (wRect.GetHeight() < mMinimumHeight)
          wRect.mTop += wRect.GetHeight() - mMinimumHeight;
        if (wRect.GetWidth() < mMinimumWidth)
          wRect.mLeft += wRect.GetWidth() - mMinimumWidth;
      }
      break;
    case nuiBottomLeft:
      {
        wRect.mBottom += dy;
        wRect.mLeft += dx;
        if (wRect.GetHeight() < mMinimumHeight)
          wRect.SetSize(wRect.GetWidth(), mMinimumHeight);
        if (wRect.GetWidth() < mMinimumWidth)
          wRect.mLeft += wRect.GetWidth() - mMinimumWidth;
      }
      break;
    case nuiLeft:
      {
        wRect.mLeft += dx;
        if (wRect.GetWidth() < mMinimumWidth)
          wRect.mLeft += wRect.GetWidth() - mMinimumWidth;
      }
      break;
    case nuiTopRight:
      {
        wRect.mTop += dy;
        wRect.mRight += dx;
        if (wRect.GetHeight() < mMinimumHeight)
          wRect.mTop += wRect.GetHeight() - mMinimumHeight;
        if (wRect.GetWidth() < mMinimumWidth)
          wRect.SetSize(mMinimumWidth, wRect.GetHeight());
      }
      break;
    case nuiBottomRight:
      {
        wRect.mBottom += dy;
        wRect.mRight += dx;
        if (wRect.GetWidth() < mMinimumWidth)
          wRect.SetSize(mMinimumWidth, wRect.GetHeight());
        if (wRect.GetHeight() < mMinimumHeight)
          wRect.SetSize(wRect.GetWidth(), mMinimumHeight);
      }
      break;
    case nuiRight:
      {
        wRect.mRight += dx;
        if (wRect.GetWidth() < mMinimumWidth)
          wRect.SetSize(mMinimumWidth, wRect.GetHeight());
      }
      break;
    case nuiTop:
      {
        wRect.mTop += dy;
        if (wRect.GetHeight() < mMinimumHeight)
          wRect.mTop += wRect.GetHeight() - mMinimumHeight;
      }
      break;
    case nuiBottom:
      {
        wRect.mBottom += dy;
        if (wRect.GetHeight() < mMinimumHeight)
          wRect.SetSize(wRect.GetWidth(), mMinimumHeight);
      }
      break;
    default:
      return true;
      break;
    }

    wRect.RoundToNearest();
    if (mpManager)
      mpManager->ValidateWindowRect(wRect);
    
    if (GetRect().GetWidth() == wRect.GetWidth() && GetRect().GetHeight() == wRect.GetHeight())
      SetUserPos(wRect.mLeft, wRect.mTop);
    else
      SetUserRect(wRect);

   return true;
  }
  else
  {
    if (IsInsideFromParent(X,Y))
    {
      nuiRect _Rect = GetRect();
      _Rect.Transform(GetMatrix());
      nuiTheme* pTheme = GetTheme();
      NGL_ASSERT(pTheme);
      mClickPos= pTheme->GetWindowPart(_Rect, X, Y, GetFlags(), false);
      pTheme->Release();

      SetMouseCursor(eCursorDoNotSet);

      switch (mClickPos)
      {
      case nuiCenter:
        if (!mNoMove)
          SetMouseCursor(eCursorHand);
        break;
      case nuiTopLeft:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeNW);
        break;
      case nuiBottomLeft:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeSW);
        break;
      case nuiLeft:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeW);
        break;
      case nuiTopRight:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeNE);
        break;
      case nuiBottomRight:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeSE);
        break;
      case nuiRight:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeE);
        break;
      case nuiTop:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeN);
        break;
      case nuiBottom:
        if (!mNoResize)
          SetMouseCursor(eCursorResizeS);
        break;
      default:
        break;
      }
    }
  }

  return true; // Always block mouse event that are not from this window
}

void nuiWindow::Invalidate()
{
  if (mpParent)
  {
    nuiRect bigrect(GetRect());
    nuiTheme* pTheme = GetTheme();
    NGL_ASSERT(pTheme);
    pTheme->AdjustWindowRect(bigrect, GetFlags(), true, true);
    pTheme->Release();

    mpParent->InvalidateRect(bigrect);
  }
  nuiSimpleContainer::Invalidate();
}
