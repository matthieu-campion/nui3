/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiRectView::nuiRectView(const nuiRect& rRect)
: nuiWidget()
{
  mMousePosition = nuiNoPosition;
  mClicked = false;
  mCurrentRect = rRect;
  mDisplayRectOnlyOnHover = false;
}

nuiRectView::~nuiRectView()
{
  
}

bool nuiRectView::Draw(nuiDrawContext* pContext)
{
  nuiRect r(mCurrentRect);

  pContext->EnableBlending(true);
  SetStrokeColor(pContext, eShapeStroke);
  
  nuiSize x, y, xx, yy, w, h, ww, hh;
  
  x = mCurrentRect.Left() + 2;
  y = mCurrentRect.Top() + 2;
  xx = mCurrentRect.Right() - 3;
  yy = mCurrentRect.Bottom() -3;
  w = mCurrentRect.GetWidth();
  h = mCurrentRect.GetHeight();
  ww = w * .2;
  hh = h * .2;
  
  switch (mMousePosition)
  {
    // Center:
    case nuiCenter:
      {
        pContext->DrawLine(x+ww, y+hh, xx-ww, yy-hh);
        pContext->DrawLine(x+ww, yy-hh, xx-ww, y+hh);
      }
      break;
      
    // Borders:
    case nuiLeft:
      {
        pContext->DrawLine(x, y, x, yy);
      }
      break;
    case nuiRight:
      {
        pContext->DrawLine(xx, y, xx, yy);
      }
      break;
    case nuiTop:
      {
        pContext->DrawLine(x, y, xx, y);
      }
      break;
    case nuiBottom:
      {
        pContext->DrawLine(x, yy, xx, yy);
      }
      break;

    // Corners:
    case nuiTopLeft:
      {
        pContext->DrawLine(x, y, x + ww, y);
        pContext->DrawLine(x, y, x, y + hh);
      }
      break;
    case nuiTopRight:
      {
        pContext->DrawLine(xx - ww, y, xx, y);
        pContext->DrawLine(xx, y, xx, y + hh);
      }
      break;
    case nuiBottomRight:
      {
        pContext->DrawLine(xx - ww, yy, xx, yy);
        pContext->DrawLine(xx, yy, xx, yy - hh);
      }
      break;
    case nuiBottomLeft:
      {
        pContext->DrawLine(x, yy, x + ww, yy);
        pContext->DrawLine(x, yy, x, yy - hh);
      }
      break;
    case nuiNoPosition: break;                                                                                                         
    case nuiTile: break;
	case nuiFill: break;
	case nuiFillHorizontal: break;
	case nuiFillVertical: break;
	case nuiFillLeft: break;
	case nuiFillRight: break;
	case nuiFillTop: break;
	case nuiFillBottom: break;
  }

  bool DisplayRect = true;
  
  if (mDisplayRectOnlyOnHover && !GetHover())
  {
    DisplayRect = false;
  }
  
  if (DisplayRect)
    pContext->DrawRect(r, eStrokeShape);

  return true;
}

nuiRect nuiRectView::CalcIdealSize()
{
  nuiRect r(mCurrentRect);
  r.Grow(16, 16);
  return r.Size();
}

bool nuiRectView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mMousePosition = mCurrentRect.GetPosition(X, Y, 3, 3);

    if (mMousePosition != nuiNoPosition)
    {
      mClicked = true;
      mMouseX = X;
      mMouseY = Y;
      Grab();
      Invalidate();
      
      return true;
    }
  }
  
  return false;
}

bool nuiRectView::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = false;
    Ungrab();
    InvalidateLayout();
    RectChanged(mCurrentRect);
    return true;
  }

  return false;
}

bool nuiRectView::MouseMoved(nuiSize X, nuiSize Y)
{
  if (mClicked)
  {
    switch (mMousePosition)
    {
      case nuiCenter:
        mCurrentRect.Move(X - mMouseX, Y - mMouseY);
        mMouseX = X;
        mMouseY = Y;
        break;

      case nuiLeft:
        if (X > mCurrentRect.mRight)
          X = mCurrentRect.mRight;
        mCurrentRect.mLeft = X;
        break;
      case nuiTop:
        if (Y > mCurrentRect.mBottom)
          Y = mCurrentRect.mBottom;
        mCurrentRect.mTop = Y;
        break;
      case nuiRight:
        if (X < mCurrentRect.mLeft)
          X = mCurrentRect.mLeft;
        mCurrentRect.mRight = X;
        break;
      case nuiBottom:
        if (Y < mCurrentRect.mTop)
          Y = mCurrentRect.mTop;
        mCurrentRect.mBottom = Y;
        break;

      case nuiTopLeft:
        if (X > mCurrentRect.mRight)
          X = mCurrentRect.mRight;
        if (Y > mCurrentRect.mBottom)
          Y = mCurrentRect.mBottom;
        mCurrentRect.mLeft = X;
        mCurrentRect.mTop = Y;
        break;
      case nuiTopRight:
        if (X < mCurrentRect.mLeft)
          X = mCurrentRect.mLeft;
        if (Y > mCurrentRect.mBottom)
          Y = mCurrentRect.mBottom;
        mCurrentRect.mRight = X;
        mCurrentRect.mTop = Y;
        break;
      case nuiBottomRight:
        if (X < mCurrentRect.mLeft)
          X = mCurrentRect.mLeft;
        if (Y < mCurrentRect.mTop)
          Y = mCurrentRect.mTop;
        mCurrentRect.mRight = X;
        mCurrentRect.mBottom = Y;
        break;
      case nuiBottomLeft:
        if (X > mCurrentRect.mRight)
          X = mCurrentRect.mRight;
        if (Y < mCurrentRect.mTop)
          Y = mCurrentRect.mTop;
        mCurrentRect.mLeft = X;
        mCurrentRect.mBottom = Y;
        break;
        case nuiNoPosition: break;                                                                                                         
	    case nuiTile: break;
		case nuiFill: break;
		case nuiFillHorizontal: break;
		case nuiFillVertical: break;
		case nuiFillLeft: break;
		case nuiFillRight: break;
		case nuiFillTop: break;
		case nuiFillBottom: break;
    }
    
    RectChanged(mCurrentRect);
    SetHotRect(mCurrentRect);
    InvalidateLayout();
  }
  else
  {
    mMousePosition = mCurrentRect.GetPosition(X, Y, 3, 3);

    SetMouseCursor(eCursorArrow);
    
    switch (mMousePosition)
    {
      case nuiCenter:
        SetMouseCursor(eCursorArrow);
        break;
      case nuiTopLeft:
        SetMouseCursor(eCursorResizeNW);
        break;
      case nuiBottomLeft:
        SetMouseCursor(eCursorResizeSW);
        break;
      case nuiLeft:
        SetMouseCursor(eCursorResizeW);
        break;
      case nuiTopRight:
        SetMouseCursor(eCursorResizeNE);
        break;
      case nuiBottomRight:
        SetMouseCursor(eCursorResizeSE);
        break;
      case nuiRight:
        SetMouseCursor(eCursorResizeE);
        break;
      case nuiTop:
        SetMouseCursor(eCursorResizeN);
        break;
      case nuiBottom:
        SetMouseCursor(eCursorResizeS);
        break;
      default:
        break;
    }
    Invalidate();
  }

  return true;
}

void nuiRectView::SetHover(bool Hover)
{
  if (!Hover)
  {
    mMousePosition = nuiNoPosition;
    Invalidate();
  }
}

const nuiRect& nuiRectView::GetCurrentRect() const
{
  return mCurrentRect;
}

void nuiRectView::SetCurrentRect(const nuiRect& rRect)
{
  mCurrentRect = rRect;
  Invalidate();
}

void nuiRectView::SetDisplayRectOnlyOnHover(bool set)
{
  mDisplayRectOnlyOnHover = set;
  Invalidate();
}

bool nuiRectView::GetDisplayRectOnlyOnHover() const
{
  return mDisplayRectOnlyOnHover;
}
