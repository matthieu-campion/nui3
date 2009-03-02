/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#include "nui.h"
#include "nuiWidget.h"
#include "nuiTopLevel.h"
#include "nuiXML.h"
#include "nuiAnimation.h"
#include "nuiDrawContext.h"
#include "nuiMetaPainter.h"
#include "nuiMainWindow.h"
#include "nuiTheme.h"
#include "nuiDecoration.h"
#include "nuiWidgetMatcher.h"
#include <limits>

#ifdef _UIKIT_
const bool gGlobalUseRenderCache = true;
#else
const bool gGlobalUseRenderCache = false;
//const bool gGlobalUseRenderCache = true;
#endif

bool nuiWidget::mSelfClippingDefault = true;

void nuiWidget::SetSelfClippingDefault(bool set)
{
  mSelfClippingDefault = set;
}

bool nuiWidget::GetSelfClippingDefault()
{
  return mSelfClippingDefault;
}

//#define NUI_LOG_GETIDEALRECT

#ifdef NUI_WIDGET_STATS
static uint wcount = 0;
static uint maxwcount = 0;

class nuiWidgetCounter
{
public:
  nuiWidgetCounter()
  {
  }
  
  ~nuiWidgetCounter()
  {
    Dump();
  }
  
  void Dump()
  {
    std::map<nglString, uint>::iterator it = mCounters.begin(); 
    std::map<nglString, uint>::iterator end = mCounters.end(); 
    
    while (it != end)
    {
      NGL_OUT(_T("%ls: %d\n", it->first.GetChars(), it->second));
      ++it;
    }
  }

  void Add(const nglString& rString)
  {
    std::map<nglString, uint>::iterator it = mCounters.find(rString); 
    if (it == mCounters.end())
      mCounters.insert(std::pair<nglString, uint>(rString, 1));
    else
      it->second++;
  }
  
private:
  std::map<nglString, uint> mCounters;
};

static nuiWidgetCounter gWidgetCounter;
#endif

nuiWidget::nuiWidget()
: mGenericWidgetSink(this),
  mODLeft(0),
  mODRight(0),
  mODTop(0),
  mODBottom(0),
  mInteractiveOD(false),
  mInteractiveDecoration(false),
  mpDecoration(NULL),
  mpFocusDecoration(NULL),
  mShowFocus(true),
  mPosition(nuiFill),
  mFillRule(nuiFill),
  mMatrixIsIdentity(true),
  mCSSPasses(0),
  mpParent(NULL),
  mpTheme(NULL),
  mDecorationEnabled(true),
  mNeedSurfaceRedraw(false),
  mSurfaceEnabled(false),
  mpSurface(NULL),
  mSurfaceColor(255, 255, 255, 255),
  mSurfaceBlendFunc(nuiBlendTransp)
{
  if (SetObjectClass(_T("nuiWidget")))
    InitAttributes();
    
#ifdef NUI_WIDGET_STATS
  wcount++;
  maxwcount = MAX(wcount, maxwcount);
  NGL_OUT(_T("max widgets: %d (total %d)\n"), maxwcount, wcount);
#endif
  
  Init();

  InitDefaultDecorations();
  
  // Property bindings:
  InitProperties();
  
}





bool nuiWidget::Load(const nuiXMLNode* pNode)
{
  nuiObject::Load(pNode);
#ifdef NUI_WIDGET_STATS
  wcount++;
  maxwcount = MAX(wcount, maxwcount);
  NGL_OUT(_T("max widgets: %d (total %d)\n", maxwcount, wcount));
#endif
  
  Init();
  mpTheme = NULL;

  nglString str = pNode->Dump(0);
  // Retrieve the size of the widget from the XML description (ignored if not present):
  if ( pNode->HasAttribute(_T("X")) 
    && pNode->HasAttribute(_T("Y")) 
    && pNode->HasAttribute(_T("Width")) 
    && pNode->HasAttribute(_T("Height")))
  {
    mRect.mLeft = pNode->GetAttribute(nglString(_T("X"))).GetCFloat();
    mRect.mRight = mRect.mLeft + pNode->GetAttribute(nglString(_T("Width"))).GetCFloat();
    mRect.mTop = pNode->GetAttribute(nglString(_T("Y"))).GetCFloat();
    mRect.mBottom = mRect.mTop + pNode->GetAttribute(nglString(_T("Height"))).GetCFloat();
    mIdealRect = mRect;
    mVisibleRect = GetOverDrawRect(true, true);
    SetUserRect(mRect);
  }
  else
  {
    if ( pNode->HasAttribute(_T("X")) 
      && pNode->HasAttribute(_T("Y")))
    {
      SetUserPos(pNode->GetAttribute(nglString(_T("X"))).GetCFloat(),
        pNode->GetAttribute(nglString(_T("Y"))).GetCFloat());
    }
  }

  SetVisible(nuiGetBool(pNode,nglString(_T("Visible")),true));
  SetEnabled(nuiGetBool(pNode,nglString(_T("Enabled")), true));
  SetSelected(nuiGetBool(pNode,nglString(_T("Selected")), false));
  mStateLocked = nuiGetBool(pNode,_T("StateLocked"),false);

  // Property bindings:
  InitProperties();

  uint i, count = pNode->GetChildrenCount();
  for (i = 0; i < count; i++)
  {
    nuiXMLNode* pChild = pNode->GetChild(i);
    if (pChild->GetName() == _T("nuiPropertyBag"))
    {
      uint j;
      for (j=0; j<pChild->GetAttributeCount(); j++)
      {
        SetProperty(pChild->GetAttributeName(j),pChild->GetAttributeValue(j));
      }
    }
  }
    
  return true;
}

bool nuiWidget::AttrIsEnabled()
{
	return IsEnabled();
}



bool nuiWidget::AttrIsVisible()
{
	return IsVisible();
}




void nuiWidget::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("Enabled")), nuiUnitBoolean,
     nuiMakeDelegate(this, &nuiWidget::AttrIsEnabled),
     nuiMakeDelegate(this, &nuiWidget::SetEnabled)));
  
  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("Visible")), nuiUnitBoolean,
     nuiMakeDelegate(this, &nuiWidget::AttrIsVisible),
     nuiMakeDelegate(this, &nuiWidget::SetVisible)));

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("Selected")), nuiUnitBoolean,
     nuiMakeDelegate(this, &nuiWidget::AttrIsSelected),
     nuiMakeDelegate(this, &nuiWidget::SetSelected)));

  
  
  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("UserRect")), nuiUnitNone,
                nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiWidget::GetUserRect),
                nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiWidget::SetUserRect)));
  
  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("VisibleRect")), nuiUnitNone,
                nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiWidget::GetVisibleRect),
                nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiWidget::SetVisibleRect)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("UserWidth")), nuiUnitNone,
                nuiAttribute<nuiSize>::GetterDelegate(this, &nuiWidget::GetUserWidth),
                nuiAttribute<nuiSize>::SetterDelegate(this, &nuiWidget::SetUserWidth)));

  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("UserHeight")), nuiUnitNone,
                nuiAttribute<nuiSize>::GetterDelegate(this, &nuiWidget::GetUserHeight),
                nuiAttribute<nuiSize>::SetterDelegate(this, &nuiWidget::SetUserHeight)));
  

  AddAttribute(new nuiAttribute<bool>
              (nglString(_T("UseRenderCache")), nuiUnitBoolean,
               nuiMakeDelegate(this, &nuiWidget::IsRenderCacheEnabled),
               nuiMakeDelegate(this, &nuiWidget::EnableRenderCache)));


  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("RedrawOnHover")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::GetRedrawOnHover),
                nuiMakeDelegate(this, &nuiWidget::SetRedrawOnHover)));
	
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Hover")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::GetHover)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Focus")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::HasFocus)));
  
	// Decoration:
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Decoration")), nuiUnitName,
                nuiAttribute<const nglString&>::GetterDelegate(this, &nuiWidget::GetDecorationName),
                nuiAttribute<const nglString&>::SetterDelegate(this, &nuiWidget::SetDecoration)));


  AddAttribute(new nuiAttribute<nuiDecorationMode>
               (nglString(_T("DecorationMode")), nuiUnitNone,
                nuiAttribute<nuiDecorationMode>::GetterDelegate(this, &nuiWidget::GetDecorationMode),
                nuiAttribute<nuiDecorationMode>::SetterDelegate(this, &nuiWidget::SetDecorationMode)));

  // Focus Decoration:
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("FocusDecoration")), nuiUnitName,
                nuiAttribute<const nglString&>::GetterDelegate(this, &nuiWidget::GetFocusDecorationName),
                nuiAttribute<const nglString&>::SetterDelegate(this, &nuiWidget::SetFocusDecoration)));
  
  
  AddAttribute(new nuiAttribute<nuiDecorationMode>
               (nglString(_T("FocusDecorationMode")), nuiUnitNone,
                nuiAttribute<nuiDecorationMode>::GetterDelegate(this, &nuiWidget::GetFocusDecorationMode),
                nuiAttribute<nuiDecorationMode>::SetterDelegate(this, &nuiWidget::SetFocusDecorationMode)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("FocusVisible")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::IsFocusVisible),
                nuiMakeDelegate(this, &nuiWidget::SetFocusVisible)));
  
  
  // nuiAttribute<nuiSize> <=> nuiAttribute<double>
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("BorderLeft")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetBorderLeft),
                nuiMakeDelegate(this, &nuiWidget::SetBorderLeft)));

  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("BorderTop")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetBorderTop),
                nuiMakeDelegate(this, &nuiWidget::SetBorderTop)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("BorderRight")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetBorderRight),
                nuiMakeDelegate(this, &nuiWidget::SetBorderRight)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("BorderBottom")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetBorderBottom),
                nuiMakeDelegate(this, &nuiWidget::SetBorderBottom)));
  
  // nuiAttribute<nuiSize> <=> nuiAttribute<double>
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("ActualBorderLeft")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetActualBorderLeft)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("ActualBorderTop")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetActualBorderTop)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("ActualBorderRight")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetActualBorderRight)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("ActualBorderBottom")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetActualBorderBottom)));
  
  nuiAttribute<nuiPosition>* AttributePosition = new nuiAttribute<nuiPosition>
              (nglString(_T("Position")), nuiUnitPosition,
               nuiMakeDelegate(this, &nuiWidget::GetPosition), 
               nuiMakeDelegate(this, &nuiWidget::SetPosition));
  AddAttribute(_T("Position"), AttributePosition);
  
  nuiAttribute<nuiPosition>* AttributeFillRule = new nuiAttribute<nuiPosition>
              (nglString(_T("FillRule")), nuiUnitPosition,
               nuiMakeDelegate(this, &nuiWidget::GetFillRule), 
               nuiMakeDelegate(this, &nuiWidget::SetFillRule));
              AddAttribute(_T("FillRule"), AttributeFillRule);

  AddAttribute(new nuiAttribute<nuiMouseCursor>
               (nglString(_T("Cursor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMouseCursor),
                nuiMakeDelegate(this, &nuiWidget::SetAttrMouseCursor)));
  
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("WantKeyBoardFocus")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::GetWantKeyboardFocus),
                nuiMakeDelegate(this, &nuiWidget::SetWantKeyboardFocus)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("EnableSurface")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::IsSurfaceEnabled),
                nuiMakeDelegate(this, &nuiWidget::EnableSurface)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("SurfaceColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiWidget::GetSurfaceColor),
                nuiMakeDelegate(this, &nuiWidget::SetSurfaceColor)));

  AddAttribute(new nuiAttribute<const nuiMatrix&>
               (nglString(_T("SurfaceMatrix")), nuiUnitMatrix,
                nuiMakeDelegate(this, &nuiWidget::GetSurfaceMatrix),
                nuiMakeDelegate(this, &nuiWidget::SetSurfaceMatrix)));

  AddAttribute(new nuiAttribute<nuiBlendFunc>
               (nglString(_T("SurfaceBlendFunc")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiWidget::GetSurfaceBlendFunc),
                nuiMakeDelegate(this, &nuiWidget::SetSurfaceBlendFunc)));
}

 
void nuiWidget::Init()
{
  mDebugLevel = 0; // No debug by default.
  mCanRespectConstraint = false; ///< By default the widgets don't care about the constraints imposed by their parents. Only few ones care about this.
  mNeedInvalidateOnSetRect = true;
  mDrawingInCache = false;
  mAutoClipSelf = mSelfClippingDefault;
  mpRenderCache = NULL;
	mUseRenderCache = false;

  mTrashed = false;
  mRedrawOnHover = false;

  mMixAlpha = true;
  mInheritAlpha = true;

  mpAnimationTimer = NULL;
  mAnimateLayout = false;
  mObjCursor = eCursorDoNotSet;
  mAlpha = 1.0f;
  mHasFocus = false;
  mNeedRender = true;
  mNeedSelfLayout = true;
  mNeedLayout = true;
  mNeedIdealRect = true;
  mNeedSelfRedraw = true;
  mStateLocked = false;
  mHover = false;
  mHasUserPos = false;
  mHasUserSize = false;
  mHasUserWidth = false;
  mHasUserHeight = false;
  mForceIdealSize = false;
  mSelectionExclusive = false;

  mMouseEventEnabled = true;
  mLocalMouseEventEnabled = true;

  mBorderRight = mBorderLeft = 0.f;
  mBorderTop = mBorderBottom = 0.f;

  mMinWidth = mMaxWidth = mMinHeight = mMaxHeight = -1.0f;

  mpSavedPainter = NULL;

	mEnabled = true;
  mSelected = false;
  mVisible = true;
  
  mWantKeyboardFocus = false;
  
  LoadIdentityMatrix();

  EnableRenderCache(true);
//  EnableRenderCache(false);
  
  // Events:
  NUI_ADD_EVENT(Trashed);
  NUI_ADD_EVENT(Destroyed);
  NUI_ADD_EVENT(ParentChanged);

  NUI_ADD_EVENT(Selected);
  NUI_ADD_EVENT(Deselected);
  NUI_ADD_EVENT(Disabled);
  NUI_ADD_EVENT(StateChanged);

  NUI_ADD_EVENT(Shown);
  NUI_ADD_EVENT(Hiden);
  NUI_ADD_EVENT(VisibilityChanged);

  NUI_ADD_EVENT(HoverOn);
  NUI_ADD_EVENT(HoverOff);
  NUI_ADD_EVENT(HoverChanged);

  NUI_ADD_EVENT(UserRectChanged);
  NUI_ADD_EVENT(HotRectChanged);

  NUI_ADD_EVENT(Clicked);
  NUI_ADD_EVENT(Unclicked);

}

void nuiWidget::InitProperties()
{
  mClippingOptims = false;
}


nuiXMLNode* nuiWidget::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{   
  nuiXMLNode* pNode = NULL;

  if (mSerializeMode == eDontSaveNode)
    return NULL;

  if (mSerializeMode != eSkipNode)
  {
    pNode = nuiObject::Serialize(pParentNode,Recursive);

    if (!pNode)
      return NULL;

    if (HasUserRect())
    {
      pNode->SetAttribute(_T("X"),mUserRect.mLeft);
      pNode->SetAttribute(_T("Y"),mUserRect.mTop);
      pNode->SetAttribute(_T("Width"),mUserRect.GetWidth());
      pNode->SetAttribute(_T("Height"),mUserRect.GetHeight());
    }
    else
    {
      if (mHasUserPos)
      {
        pNode->SetAttribute(_T("X"),mUserRect.mLeft);
        pNode->SetAttribute(_T("Y"),mUserRect.mTop);
      }
    }
  }
  else
    pNode = pParentNode;

  return pNode;
}

bool nuiWidget::SetObjectClass(const nglString& rName)
{
  bool res = nuiObject::SetObjectClass(rName);
  ResetCSSPass();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_ALL);
  return res;
}

// virtual from nuiObject
void nuiWidget::SetObjectName(const nglString& rName)
{
  nuiObject::SetObjectName(rName);
  ResetCSSPass();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_ALL);
}




nglString nuiWidget::Dump()
{
  nuiXML* xml = new nuiXML(GetObjectName());
  
  Serialize(xml, true);
  
  nglString dump = xml->Dump();
  
  delete xml;
  return dump;
}


nuiWidget::~nuiWidget()
{
  Destroyed();
  
#ifdef NUI_WIDGET_STATS
  wcount--;
  gWidgetCounter.Add(GetProperty("Class"));
#endif
	
  if (GetDebug())
  {
    NGL_OUT(_T("nuiWidget::~nuiWidget() [0x%x '%ls':'%ls']\n"), this, GetObjectClass().GetChars(), GetObjectName().GetChars());
  }
  
  ClearAnimations();

  if (mpAnimationTimer)
    delete mpAnimationTimer;

  nuiTopLevel* pRoot = GetTopLevel();
  if (pRoot)
    pRoot->AdviseObjectDeath(this);

  //NGL_ASSERT(!pRoot || IsTrashed());

  if (mpTheme)
    mpTheme->Release();
    
  if (mpDecoration)
  {
    mpDecoration->Release();
  }

  if (mpSurface)
    mpSurface->Release();
  delete mpRenderCache;
}

bool nuiWidget::IsTrashed(bool combined) const 
{ 
  if (!combined || !mpParent)
    return mTrashed;
  if (!mTrashed)
    return mpParent->IsTrashed(true);
  return mTrashed;
}

nuiContainerPtr nuiWidget::GetParent() const
{
  return mpParent;
}

nuiTopLevel* nuiWidget::GetTopLevel() const
{
  NGL_ASSERT(mpParent != this);
  if (mpParent)
    return mpParent->GetTopLevel();
  else
    return NULL;
}


nuiContainerPtr nuiWidget::GetRoot() const
{
  if (mpParent)
    return mpParent->GetRoot();
  else
    return NULL;
}

bool nuiWidget::SetParent(nuiContainerPtr pParent)
{
  NGL_ASSERT(!IsTrashed(false));
  bool res = true;

  nuiTopLevel* pRoot = GetTopLevel();
  if (pRoot)
  {
    pRoot->ReleaseToolTip(this);
    if (!pParent) // If we used to be connected to a root but the current parenting changes that: tell the trunk of the tree about it!
    {
      pRoot->AdviseSubTreeDeath(this);
      CallDisconnectTopLevel(pRoot);
    }
  }

  mpParent = pParent;
  ParentChanged();

  SetLayoutConstraint(LayoutConstraint()); ///< Reset the constraints when the widget is reparented
  InvalidateLayout();
  
  nuiTopLevel* pTopLevel = GetTopLevel();
  if (pTopLevel)
    CallConnectTopLevel(pTopLevel);
  
  DebugRefreshInfo();
  return res;
}

void nuiWidget::LocalToGlobal(int& x, int& y) const
{
  if (!mMatrixIsIdentity)
  {
    nuiVector vec((double)x,(double)y, 0);
    vec = GetMatrix() * vec;
    x = ToBelow(vec[0]);
    y = ToBelow(vec[1]);
  }

  x += (int)mRect.mLeft;
  y += (int)mRect.mTop;

  if (mpParent)
    mpParent->LocalToGlobal(x,y);
}

void nuiWidget::LocalToGlobal(nuiSize& x, nuiSize& y) const
{
  if (!mMatrixIsIdentity)
  {
    nuiVector vec(x, y, 0);
    vec = GetMatrix() * vec;
    x = vec[0];
    y = vec[1];
  }

  x += mRect.mLeft;
  y += mRect.mTop;

  if (mpParent)
    mpParent->LocalToGlobal(x,y);
}

void nuiWidget::LocalToGlobal(nuiRect& rRect) const
{
  if (!mMatrixIsIdentity)
  {
    nuiVector vec1(rRect.mLeft,rRect.mTop,0);
    nuiVector vec2(rRect.mRight,rRect.mBottom,0);
    vec1 = GetMatrix() * vec1;
    vec2 = GetMatrix() * vec2;
    rRect.mLeft   = vec1[0];
    rRect.mTop    = vec1[1];
    rRect.mRight  = vec2[0];
    rRect.mBottom = vec2[1];
  }

  rRect.Move(mRect.mLeft, mRect.mTop);

  if (mpParent)
  {
    mpParent->LocalToGlobal(rRect);
  }
}

void nuiWidget::GlobalToLocal(int& x, int& y) const
{
  if (mpParent)
    mpParent->GlobalToLocal(x,y);
  x -= (int)mRect.mLeft;
  y -= (int)mRect.mTop;

  if (!mMatrixIsIdentity)
  {
    nuiVector vec((double)x,(double)y,0);
    nuiMatrix mat;
    GetMatrix(mat);
    mat.InvertHomogenous();
    vec = mat * vec;
    x = ToBelow(vec[0]);
    y = ToBelow(vec[1]);
  }
}

void nuiWidget::GlobalToLocal(nuiSize& x, nuiSize& y) const
{
  if (mpParent)
    mpParent->GlobalToLocal(x,y);
  x -= mRect.mLeft;
  y -= mRect.mTop;

  if (!mMatrixIsIdentity)
  {
    nuiVector vec(x,y,0);
    nuiMatrix mat;
    GetMatrix(mat);
    mat.InvertHomogenous();
    vec = mat * vec;
    x = vec[0];
    y = vec[1];
  }
}

void nuiWidget::GlobalToLocal(nuiRect& rRect) const
{
  if (mpParent)
  {
    mpParent->GlobalToLocal(rRect);
  }
  rRect.Move(-mRect.mLeft, -mRect.mTop);

  if (!mMatrixIsIdentity)
  {
    nuiMatrix mat;
    GetMatrix(mat);
    mat.InvertHomogenous();
    nuiVector vec1(rRect.mLeft,rRect.mTop,0);
    nuiVector vec2(rRect.mRight,rRect.mBottom,0);
    vec1 = mat * vec1;
    vec2 = mat * vec2;
    rRect.mLeft   = vec1[0];
    rRect.mTop    = vec1[1];
    rRect.mRight  = vec2[0];
    rRect.mBottom = vec2[1];
  }
}

void nuiWidget::LocalToLocal(nuiWidgetPtr pWidget,int& x, int& y)
{                           
  if (GetRoot() != pWidget->GetRoot())
    return;

  LocalToGlobal(x,y);
  pWidget->GlobalToLocal(x,y);
}

void nuiWidget::LocalToLocal(nuiWidgetPtr pWidget, nuiSize& x, nuiSize& y)
{                           
  if (GetRoot() != pWidget->GetRoot())
    return;

  LocalToGlobal(x,y);
  pWidget->GlobalToLocal(x,y);
}

void nuiWidget::LocalToLocal(nuiWidgetPtr pWidget,nuiRect& rRect)
{                           
  if (GetRoot() != pWidget->GetRoot())
    return;

  LocalToGlobal(rRect);
  pWidget->GlobalToLocal(rRect);
}

const nuiRect& nuiWidget::GetRect() const
{
  return mRect;
}

nuiRect nuiWidget::GetBorderedRect() const
{
  nuiRect rect = GetRect();
  rect.Bottom() += GetActualBorderBottom();
  rect.Top() -= GetActualBorderTop();
  rect.Left() -= GetActualBorderLeft();
  rect.Right() += GetActualBorderRight();
  return rect;
}

void nuiWidget::InvalidateRect(const nuiRect& rRect)
{
  //NGL_OUT(_T("  nuiWidget::InvalidateRect '%ls' [%ls] %ls\n"), GetProperty("Class").GetChars(), GetProperty("Name").GetChars(), rRect.GetValue().GetChars());
  if (IsVisible(true))
  {
    nuiRect tmp(rRect);
    tmp.RoundToBiggest();
    BroadcastInvalidateRect(this, tmp);
  }
  mNeedSelfRedraw = true;
  InvalidateSurface();
  DebugRefreshInfo();
}

void nuiWidget::BroadcastInvalidateRect(nuiWidgetPtr pSender, const nuiRect& rRect)
{
  nuiRect r = rRect;
  nuiRect rect = GetRect();
  nuiRect size = GetOverDrawRect(true, true);
  r.Intersect(r, size);

  nuiVector vec1(r.Left(),r.Top(),0);
  nuiVector vec2(r.Right(),r.Bottom(),0);
  if (!mMatrixIsIdentity)
  {
    nuiMatrix m(GetMatrix());
    //m.InvertHomogenous();

    vec1 = m * vec1;
    vec2 = m * vec2;
  }

  r.Set(vec1[0], vec1[1], vec2[0], vec2[1], false);
  r.Move(rect.Left(), rect.Top());

  if (mpParent)
  {
    mNeedRender = true;
    mpParent->BroadcastInvalidateRect(pSender, r);
  }
  else
    mNeedRender = true;
  DebugRefreshInfo();
}

void nuiWidget::Invalidate()
{
  if (mNeedRender && mNeedSelfRedraw)
    return;

  //printf("nuiWidget::Invalidate '%ls [%ls]'\n", GetProperty("Class").GetChars() , GetProperty("Name").GetChars());
  
  if (!IsVisible(true))
  {
    mNeedSelfRedraw = true;
    InvalidateSurface();
    return;
  }

  //nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  nuiWidget::InvalidateRect(GetVisibleRect());
  SilentInvalidate();

  if (mpParent)
    mpParent->BroadcastInvalidate(this);
  DebugRefreshInfo();
}

void nuiWidget::InvalidateSurface()
{
  if (mNeedSurfaceRedraw)
    return;
  
  mNeedSurfaceRedraw = true;
  if (mpParent)
    mpParent->InvalidateRect(GetRect());
  DebugRefreshInfo();
}

nuiSurface* nuiWidget::GetSurface() const
{
  return mpSurface;
}

const nuiMatrix& nuiWidget::GetSurfaceMatrix() const
{
  return mSurfaceMatrix;
}

void nuiWidget::SetSurfaceMatrix(const nuiMatrix& rMatrix)
{
  mSurfaceMatrix = rMatrix;
  InvalidateSurface();
}

const nuiColor& nuiWidget::GetSurfaceColor() const
{
  return mSurfaceColor;
}

void nuiWidget::SetSurfaceColor(const nuiColor& rColor)
{
  mSurfaceColor = rColor;
  InvalidateSurface();
}

nuiBlendFunc nuiWidget::GetSurfaceBlendFunc() const
{
  return mSurfaceBlendFunc;
}

void nuiWidget::SetSurfaceBlendFunc(nuiBlendFunc BlendFunc)
{
  mSurfaceBlendFunc = BlendFunc;
  InvalidateSurface();
}


void nuiWidget::SilentInvalidate()
{
  mNeedRender = true;
  mNeedSelfRedraw = true;
  InvalidateSurface();
  if (mpRenderCache)
    mpRenderCache->Reset(NULL);
  DebugRefreshInfo();
}

void nuiWidget::BroadcastInvalidate(nuiWidgetPtr pSender)
{
  if (mpParent && !mNeedRender)
  {
    mNeedRender = true;
    mpParent->BroadcastInvalidate(pSender);
  }
  else
    mNeedRender = true;
  DebugRefreshInfo();
}

void nuiWidget::SilentInvalidateLayout()
{
  mNeedSelfLayout = true;
  mNeedLayout = true;
  mNeedIdealRect = true;
  mNeedRender = true;
  mNeedSelfRedraw = true;
  DebugRefreshInfo();
}

void nuiWidget::InvalidateLayout()
{
  bool broadcast = !mNeedLayout;
  SilentInvalidateLayout();

  if (mpParent && broadcast)
  {
    //NGL_OUT(_T("InvalidateLayout + Broadcast from %ls\n"), GetProperty("Class").GetChars());
    mpParent->BroadcastInvalidateLayout(this, false);
  }
  DebugRefreshInfo();
}

void nuiWidget::BroadcastInvalidateLayout(nuiWidgetPtr pSender, bool BroadCastOnly)
{
  mNeedRender = true;

  if (!BroadCastOnly)
  {
    mNeedSelfLayout = true;
    mNeedIdealRect = true;
  }

  /*
  if (HasUserRect())
  BroadCastOnly = true;
  */

  if (mpParent && !mNeedLayout)
  {
    mpParent->BroadcastInvalidateLayout(pSender, BroadCastOnly);
  }

  mNeedLayout = true;

  DebugRefreshInfo();
}

bool nuiWidget::Draw(nuiDrawContext* pContext)
{
  return true;
}

bool nuiWidget::InternalDrawWidget(nuiDrawContext* pContext, const nuiRect& _self, const nuiRect& _self_and_decorations, bool ApplyMatrix)
{
  pContext->PushState();
  pContext->ResetState();
  if (ApplyMatrix && !mMatrixIsIdentity)
    pContext->MultMatrix(GetMatrix());
  
  if (mAutoClipSelf)
  {
    pContext->PushClipping();
    if (mpDecoration)
      pContext->Clip(_self_and_decorations);
    pContext->EnableClipping(true);
  }
  
  uint32 clipdepth = pContext->GetClipStackSize();
  
  
  ////////////////////// Draw the Underlay
  if (mpDecoration)
  {      
    pContext->PushState();
    nuiRect sizerect(GetRect().Size());
    mpDecoration->ClientToGlobalRect(sizerect);
    mpDecoration->DrawBack(pContext, this, sizerect);
    pContext->PopState();
  }
  
  if (HasFocus())
  {
    pContext->PushState();
    DrawFocus(pContext, false);
    pContext->PopState();
  }
  
  ////////////////////// Draw the widget
  if (mAutoClipSelf)
  {
    pContext->PushClipping();
    pContext->Clip(_self);
  }
  
  pContext->PushState();
  Draw(pContext);
  pContext->PopState();
  
  if (mAutoClipSelf)
    pContext->PopClipping();
  
  ////////////////////// Draw the Overlay
  if (mpDecoration)
  {
    pContext->PushState();
    nuiRect sizerect(GetRect().Size());
    mpDecoration->ClientToGlobalRect(sizerect);
    mpDecoration->DrawFront(pContext, this, sizerect);
    pContext->PopState();
  }
  
  if (HasFocus())
  {
    pContext->PushState();
    DrawFocus(pContext, true);
    pContext->PopState();
  }
  
  mDrawingInCache = false;
  
  uint32 newclipdepth = pContext->GetClipStackSize();
  NGL_ASSERT(clipdepth == newclipdepth);
  
  if (mAutoClipSelf)
    pContext->PopClipping();
  
  pContext->PopState();
  
  return true;
}

bool nuiWidget::DrawWidget(nuiDrawContext* pContext)
{
  if (!IsVisible())
    return false;

  //NGL_ASSERT(!mNeedLayout);
  //if (mNeedLayout)
  // printf("need layout bug on 0x%X [%ls - %ls]\n", this, GetProperty("Class").GetChars(), GetProperty("Name").GetChars());

  bool drawingincache = mpParent ? mpParent->IsDrawingInCache(true) : false;

  nuiRect clip;
  pContext->GetClipRect(clip, true);
  nuiRect _self = GetOverDrawRect(true, false);
  nuiRect _self_and_decorations = GetOverDrawRect(true, true);
  nuiRect inter;
  
  if (!mSurfaceEnabled)
  {
    _self.Intersect(_self, mVisibleRect);
    _self_and_decorations.Intersect(_self_and_decorations, mVisibleRect);
    if (!inter.Intersect(_self_and_decorations, clip)) // Only render at the last needed moment. As we are currently offscreen or clipped entirely we will redraw another day.
      return false;
  }

  UpdateSurfaceRect(mRect);
  
  if (mNeedRender || !mpSurface)
  {
    bool used_surface = false;
    if (mNeedSelfRedraw && mSurfaceEnabled)
    {
      used_surface = true;
      
      pContext->PushClipping();
      pContext->PushState();
      pContext->PushProjectionMatrix();
      pContext->PushMatrix();
      pContext->PushSurface();

      pContext->ResetState();
      pContext->ResetClipRect();
      pContext->LoadMatrix(nglMatrixf());
      
      NGL_ASSERT(mpSurface);
      pContext->SetSurface(mpSurface);
      pContext->Set2DProjectionMatrix(nuiRect(0.0f, 0.0f, mpSurface->GetWidth(), mpSurface->GetHeight()));
      
      // clear the surface with transparent black:
      pContext->PushState();
      pContext->SetClearColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
      pContext->Clear();  
      pContext->PopState();
    }
    
    if (gGlobalUseRenderCache && !mSurfaceEnabled && mUseRenderCache && mpRenderCache)
    {
      if (mNeedSelfRedraw)
      {
        mpSavedPainter = pContext->GetPainter();
        mpRenderCache->Reset(mpSavedPainter);
        pContext->SetPainter(mpRenderCache);
        
        mDrawingInCache = true;
        
        InternalDrawWidget(pContext, _self, _self_and_decorations, false);
        
        pContext->SetPainter(mpSavedPainter);
        mNeedSelfRedraw = false;
      }
      
      if (!drawingincache && !pContext->GetPainter()->GetDummyMode())
      {
        Validate();
        if (!mMatrixIsIdentity)
        {
          pContext->PushMatrix();
          pContext->MultMatrix(GetMatrix());
        }
        
        mpRenderCache->ReDraw(pContext);
        
        if (!mMatrixIsIdentity)
          pContext->PopMatrix();
      }
      
    }
    else
    {
      if (!drawingincache && !pContext->GetPainter()->GetDummyMode())
      {
        Validate();
        
        InternalDrawWidget(pContext, _self, _self_and_decorations, true);
        mNeedSelfRedraw = false;
      }
    }

    if (used_surface)
    {
      pContext->PopSurface();
      pContext->PopProjectionMatrix();
      pContext->PopMatrix();
      pContext->PopState();
      pContext->PopClipping();
    }
  }

  
  if (mSurfaceEnabled)
  {
    mNeedSurfaceRedraw = false;
    DrawSurface(pContext);
  }
  
  DebugRefreshInfo();
  return true;
}

void nuiWidget::DrawSurface(nuiDrawContext* pContext)
{
  //NGL_OUT(_T("nuiWidget::DrawSurface %d x %d\n"), (uint32)mpSurface->GetWidth(), (uint32)mpSurface->GetHeight());
  pContext->PushProjectionMatrix();
  pContext->PushMatrix();
  pContext->PushState();
  pContext->ResetState();
  pContext->EnableTexturing(true);
  nuiTexture* pTexture = mpSurface->GetTexture();
  NGL_ASSERT(pTexture);
  pContext->SetTexture(pTexture);
  pContext->EnableBlending(true);
  pContext->SetFillColor(mSurfaceColor);
  //pContext->SetFillColor(nuiColor(1.0f, 1.0f, 1.0f, 1.0f));
  pContext->SetBlendFunc(mSurfaceBlendFunc);
  pContext->MultMatrix(mSurfaceMatrix);

  nuiRect src, dst;
  src = GetRect().Size();
  dst = src;
  //dst.Scale(0.5, 0.5);
  //dst.Scale(2, 2);
  pContext->DrawImage(dst, src);
  pContext->PopState();
  pContext->PopMatrix();
  pContext->PopProjectionMatrix();
}

bool nuiWidget::IsKeyDown (nglKeyCode Key) const
{
  nuiTopLevelPtr pRoot = GetTopLevel();

  return pRoot ? pRoot->IsKeyDown(Key) : false;
}

bool nuiWidget::DispatchTextInput(const nglString& rUnicodeText)
{
  if (TextInput(rUnicodeText))
  {
    return true;
  }
  
  if (mpParent)
  {
    return mpParent->DispatchTextInput(rUnicodeText);
  }
  
  return false;
}

nuiWidgetPtr DeepSearchNextFocussableWidget(nuiWidgetPtr pWidget, bool TryThisNode)
{
  if (TryThisNode && pWidget->GetWantKeyboardFocus())
    return pWidget;
    
  nuiContainer* pContainer = dynamic_cast<nuiContainer*>(pWidget); // Is this a container?
  if (pContainer)
  {
    std::auto_ptr<nuiContainer::Iterator> pIt(pContainer->GetFirstChild());
    while (pIt->IsValid())
    {
      nuiWidget* pItem = pIt->GetWidget();
      
      nuiWidgetPtr pChild = DeepSearchNextFocussableWidget(pItem, true);
      if (pChild)
        return pChild;
      
      pContainer->GetNextChild(pIt.get());
    }
  }
  
  return NULL;
}

nuiWidgetPtr GetNextFocussableSibling(nuiWidgetPtr pWidget)
{
  nuiContainer* pParent = pWidget->GetParent();
  if (pParent)
  {
    std::auto_ptr<nuiContainer::Iterator> pIt(pParent->GetChildIterator(pWidget));
    pParent->GetNextChild(pIt.get());
    
    while (pIt->IsValid())
    {
      nuiWidgetPtr pSibling = pIt->GetWidget();
      nuiWidgetPtr pItem = DeepSearchNextFocussableWidget(pSibling, true);
      if (pItem)
        return pItem;
      
      pParent->GetNextChild(pIt.get());
    }
  }
  
  return NULL;
}

nuiWidgetPtr GetNextFocussableWidget(nuiWidgetPtr pWidget)
{
  nuiWidget* pItem = DeepSearchNextFocussableWidget(pWidget, false);
  if (pItem)
    return pItem;

  nuiWidgetPtr pNextWidget = pWidget;
  while (pNextWidget)
  {
    pItem = GetNextFocussableSibling(pNextWidget);
    if (pItem)
      return pItem;
    
    pNextWidget = pNextWidget->GetParent();
  }
  
  nuiTopLevel* pTop = pWidget->GetTopLevel();
  if (pTop != pWidget)
    return GetNextFocussableWidget(pTop);
    
  return NULL;
}

nuiWidgetPtr DeepSearchPreviousFocussableWidget(nuiWidgetPtr pWidget, bool TryThisNode)
{
  nuiContainer* pContainer = dynamic_cast<nuiContainer*>(pWidget); // Is this a container?
  if (pContainer)
  {
    std::auto_ptr<nuiContainer::Iterator> pIt(pContainer->GetLastChild());
    while (pIt->IsValid())
    {
      nuiWidget* pItem = pIt->GetWidget();
      
      nuiWidgetPtr pChild = DeepSearchPreviousFocussableWidget(pItem, true);
      if (pChild)
        return pChild;
      
      pContainer->GetPreviousChild(pIt.get());
    }
  }
  
  if (TryThisNode && pWidget->GetWantKeyboardFocus())
    return pWidget;

  return NULL;
}

nuiWidgetPtr GetPreviousFocussableWidget(nuiWidgetPtr pWidget)
{
  nuiWidget* pItem = NULL;
  nuiContainer* pParent = pWidget->GetParent();
  if (pParent)
  {
    std::auto_ptr<nuiContainer::Iterator> pIt(pParent->GetChildIterator(pWidget));
    pParent->GetPreviousChild(pIt.get());
    while (pIt->IsValid())
    {
      pItem = pIt->GetWidget();
      nuiWidgetPtr pChild = DeepSearchPreviousFocussableWidget(pItem, true);
      if (pChild)
        return pChild;
      
      pParent->GetPreviousChild(pIt.get());
    }
    
    return GetPreviousFocussableWidget(pParent);
  }
  
  nuiTopLevel* pTop = pWidget->GetTopLevel();
  NGL_ASSERT(pTop);
  return DeepSearchPreviousFocussableWidget(pTop, true);
}


bool nuiWidget::DispatchKeyDown(const nglKeyEvent& rEvent)
{
  if (TriggerHotKeys(rEvent, true, true))
  {
    return true;
  }
  
  if (KeyDown(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, true, false))
  {
    return true;
  }
  
  if (rEvent.mKey == NK_TAB && HasFocus())
  {
    nuiTopLevel* pTop = GetTopLevel();
    NGL_ASSERT(pTop);
    
    // The user wants to change the focussed widget
    nuiWidget* pNext = NULL;
    if (IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT))
    {
      // Backward
      pNext = pTop->GetTabBackward(this);
      if (!pNext && mpParent)
        pNext = GetPreviousFocussableWidget(this);
    }
    else
    {
      // Forward
      pNext = pTop->GetTabForward(this);
      if (!pNext && mpParent)
        pNext = GetNextFocussableWidget(this);
    }

    if (pNext)
      pNext->Focus();
    
    return true;
  }
  
  if (mpParent)
  {
    return mpParent->DispatchKeyDown(rEvent);
  }
  
  return false;
}

bool nuiWidget::DispatchKeyUp(const nglKeyEvent& rEvent)
{
  if (TriggerHotKeys(rEvent, false, true))
  {
    return true;
  }
  
  if (KeyUp(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, false, false))
  {
    return true;
  }

  if (rEvent.mKey == NK_TAB && HasFocus())
  {
    // The user has just changed the focussed widget
    // Let's eat ths remnant key and return...
    return true;
  }
  
  if (mpParent)
  {
    return mpParent->DispatchKeyUp(rEvent);
  }
  
  return false;
}

// Event callbacks:
bool nuiWidget::TextInput(const nglString& rUnicodeText)
{
  return false;
}

bool nuiWidget::KeyDown(const nglKeyEvent& rEvent)
{
  return false;
}

bool nuiWidget::KeyUp(const nglKeyEvent& rEvent)
{
  return false;
}

bool nuiWidget::TriggerHotKeys(const nglKeyEvent& rEvent, bool KeyDown,  bool Priority)
{
  nuiKeyModifier Modifiers = 0;
  
  if (IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT))
    Modifiers |= nuiShiftKey;
  
  if (IsKeyDown(NK_LALT) || IsKeyDown(NK_RALT))
    Modifiers |= nuiAltKey;
  
  if (IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL))
    Modifiers |= nuiControlKey;
  
  if (IsKeyDown(NK_LMETA) || IsKeyDown(NK_RMETA))
    Modifiers |= nuiMetaKey;
  
  if (IsKeyDown(NK_MENU))
    Modifiers |= nuiMenuKey;
    
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator it = mHotKeyEvents.begin();
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator end = mHotKeyEvents.end();
  
  for ( ; it != end; ++it)
  {
    nglString name = it->first;
    nuiHotKey* pHotKey = GetTopLevel()->GetHotKey(name);
      
    if (pHotKey->IsEnabled() && (pHotKey->HasPriority() == Priority))
    {
      bool res = false;
      if (KeyDown)
        res = pHotKey->OnKeyDown(rEvent, Modifiers, *(it->second));
      else
        res = pHotKey->OnKeyUp(rEvent, Modifiers, *(it->second));
    
      if (res)
        return res;
    }
  }
  return false;
}

void nuiWidget::EnableMouseEvent(bool enable)
{
  mMouseEventEnabled = enable;
}

bool nuiWidget::MouseEventsEnabled() const
{
  return mMouseEventEnabled;
}

void nuiWidget::EnableLocalMouseEvent(bool enable)
{
  mLocalMouseEventEnabled = enable;
}
bool nuiWidget::LocalMouseEventsEnabled() const
{
  return mLocalMouseEventEnabled;
}

bool nuiWidget::MouseClicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return false;
}

bool nuiWidget::MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return false;
}

bool nuiWidget::MouseMoved (nuiSize X, nuiSize Y)
{
  return false;
}

////// Private event management:
bool nuiWidget::DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  if (IsInside(X,Y) || hasgrab)
  {
    GlobalToLocal(X,Y);
    bool res = PreClicked(X,Y, Button);
    if (!res)
    {
      res = MouseClicked(X,Y,Button);
      res |= Clicked(X,Y,Button);
    }

    return res;
  }
  return false;
}

bool nuiWidget::DispatchMouseUnclick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  if (IsInside(X,Y) || hasgrab)
  {
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

nuiWidgetPtr nuiWidget::DispatchMouseMove(nuiSize X, nuiSize Y)
{
  if (!mMouseEventEnabled || mTrashed)
    return NULL;

  bool inside=false,res=false;
  bool hasgrab = HasGrab();

  if (IsDisabled() && !hasgrab)
    return NULL;
    
  SetMouseCursor(eCursorHand);

  if (IsInside(X,Y))
  {
    inside = true;
  }

  GlobalToLocal(X,Y);

  if (PreMouseMoved(X,Y))
    return this;
  res = MouseMoved(X,Y);
  res |= MovedMouse(X,Y);
  return (res && inside) ? this : NULL;
}

bool nuiWidget::HasGrab()
{
  nuiTopLevel* pTop = GetTopLevel();
  if (!pTop) 
    return false;
  return pTop->GetGrab() == this;
}

bool nuiWidget::Grab()
{
  nuiTopLevelPtr pRoot = GetTopLevel();

  if (pRoot)
    return pRoot->Grab(this);

  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  return false;
}

bool nuiWidget::Ungrab()
{
  nuiTopLevelPtr pRoot = GetTopLevel();

  if (pRoot)
    return pRoot->Ungrab(this);

  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  return false;
}


bool nuiWidget::GetWantKeyboardFocus() const
{
  return mWantKeyboardFocus;
}

void nuiWidget::SetWantKeyboardFocus(bool Set)
{
  mWantKeyboardFocus = Set;
}

bool nuiWidget::Focus()
{
  if (HasFocus())
    return true;

  Invalidate();

  nuiTopLevelPtr pRoot = GetTopLevel();

  if (pRoot)
    return pRoot->SetFocus(this);

  return false;
}

bool nuiWidget::UnFocus()
{
  if (!HasFocus())
    return false;
  nuiTopLevelPtr pRoot = GetTopLevel();

  Invalidate();
  if (pRoot)
    return pRoot->SetFocus(NULL);

  return false;
}


bool nuiWidget::MouseGrabbed()
{
  return false;
}

bool nuiWidget::MouseUngrabbed()
{
  return false;
}

bool nuiWidget::IsParentActive() const
{
  if (mpParent)
    return mpParent->IsParentActive();
  return false;
}

float nuiWidget::GetAlpha(bool MixWithParent) const
{
  if (mpParent && MixWithParent)
    return mpParent->GetAlpha(MixWithParent)*mAlpha; // No transparency by default
  return mAlpha; // No transparency by default
}

void nuiWidget::SetAlpha(float Alpha)
{
  mAlpha = Alpha;
  NGL_ASSERT(mAlpha <= 1.f);
  Invalidate();
  DebugRefreshInfo();
}

void nuiWidget::CallOnTrash()
{
  mTrashed = true;

  nuiTopLevel* pRoot = GetTopLevel();
  if (pRoot)
  {
    //NGL_OUT(_T("nuiWidget OnTrash [0x%x '%ls']\n"), this, GetProperty(_T("Class")).GetChars());
    pRoot->AdviseObjectDeath(this);
  }
  else
  {
    //NGL_OUT(_T("nuiWidget OnTrash NO ROOT! [0x%x '%ls']\n"), this, GetProperty(_T("Class")).GetChars());
  }
  
  OnTrash();
}

void nuiWidget::OnTrash()
{
}

bool nuiWidget::Trash()
{
  nuiAnimation* pAnim = GetAnimation(_T("TRASH"));
  if (pAnim && (pAnim->GetTime()==0 && pAnim->GetDuration()>0))
  {
    StartAnimation(_T("TRASH"));
  }
  else
  {
    if (mTrashed)
      return false;

    mTrashed = true;

    CallOnTrash();

    nuiTopLevel* pRoot = GetTopLevel();

    if (!pRoot)
    {
      if (mpParent)
        mpParent->DelChild(this, false);

			delete this;
      return true;
    }
		
    while (!mHotKeyEvents.empty())
    {
      DelHotKey(mHotKeyEvents.begin()->first);
    }
    
    pRoot->Trash(this);
    Trashed();
    Invalidate();

    if (pRoot)
      pRoot->AdviseObjectDeath(this);

    DebugRefreshInfo();
  }

  return true;
}

bool nuiWidget::HasInheritedProperty (const nglString& rName)
{
  if (HasProperty(rName))
    return true;
  if (mpParent)
    return mpParent->HasInheritedProperty(rName);
  return false;
}

nglString nuiWidget::GetInheritedProperty (const nglString& rName)
{
  if (HasProperty(rName))
    return GetProperty(rName);
  if (mpParent)
    return mpParent->GetInheritedProperty(rName);
  return nglString::Null;
}


void nuiWidget::OnSetFocus(nuiWidgetPtr pWidget)
{
}

void nuiWidget::DispatchFocus(nuiWidgetPtr pWidget)
{
  mHasFocus = (pWidget == this);
  FocusChanged();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  DebugRefreshInfo();
}

bool nuiWidget::HasFocus() const
{
  return mHasFocus;
}

nuiDrawContext* nuiWidget::GetDrawContext()
{
  nuiContainerPtr pRoot=GetRoot();
  if (pRoot && pRoot!=this)
    return pRoot->GetDrawContext();
  else
    return NULL;
}

bool nuiWidget::HasLayoutChanged() const
{
  return mNeedSelfLayout;
}

void nuiWidget::ValidateLayout()
{
  mNeedSelfLayout = false;
  DebugRefreshInfo();
}

void nuiWidget::Validate()
{
  mNeedRender = false;
  DebugRefreshInfo();
}

bool nuiWidget::GetHover() const
{
  return mHover;
}

void nuiWidget::SetHover(bool Hover)
{
  if (mHover == Hover)
    return;
  mHover = Hover;

  if (mHover)
  {
    HoverOn();
  }
  else
  {
    HoverOff();
  }
  HoverChanged();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  if (mRedrawOnHover)
    Invalidate();
  DebugRefreshInfo();
}

bool nuiWidget::IsEnabled(bool combined) const
{
  if (!combined || !mpParent)
    return mEnabled;
  if (mEnabled && !mStateLocked) 
    return mpParent->IsEnabled(true);
  return mEnabled;
}


bool nuiWidget::AttrIsSelected() const
{
  return IsSelected();
}


bool nuiWidget::IsSelected(bool combined) const
{
  if (!combined || !mpParent)
    return mSelected;
  if (!mSelected && !mStateLocked) 
    return mpParent->IsSelected(true);
  return mSelected;
}

void nuiWidget::SetEnabled(bool set)
{
  if (mEnabled == set)
    return;
  
  mEnabled = set;
  
  if (mEnabled)
  {
    Enabled();
    StartAnimation(_T("ENABLED"));
  }
  else
  {
    Disabled();
    StartAnimation(_T("DISABLED"));
  }
  
  StateChanged();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  Invalidate();
}

void nuiWidget::SetSelected(bool set)
{
  if (mSelected == set)
    return;
  
  mSelected = set;
  
  if (mSelected)
  {
    if (mSelectionExclusive && mpParent)
    {
      nuiContainer::IteratorPtr pIt = mpParent->GetFirstChild();
      do
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem && pItem != this)
        {
          pItem->SetSelected(false);
        }
      } while (mpParent->GetNextChild(pIt));
      delete pIt;
    }
    
    Selected();
    StartAnimation(_T("SELECT"));
  }
  else
  {
    Deselected();
    StartAnimation(_T("DESELECT"));
  }
  
	StateChanged();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  Invalidate();
}

void nuiWidget::SetPosition(nuiPosition mode)
{
  if (mPosition == mode || mode == nuiNoPosition)
    return;
  
  mPosition = mode;
  InvalidateLayout();
}

nuiPosition nuiWidget::GetPosition() const
{
  return mPosition;
}

void nuiWidget::SetFillRule(nuiPosition mode)
{
  if (mFillRule == mode)
    return;
  mFillRule = mode;

  if (mPosition == nuiFill)
    InvalidateLayout();
}

nuiPosition nuiWidget::GetFillRule() const
{
  return mFillRule;
}


void nuiWidget::LockState()
{
  mStateLocked = true;
  DebugRefreshInfo();
}

void nuiWidget::UnlockState()
{
  mStateLocked = false;
  DebugRefreshInfo();
}


void nuiWidget::SetVisible(bool Visible)
{
  if (mVisible == Visible)
    return;

  Invalidate();
  mVisible = Visible;
  if (mVisible)
  {
    mNeedLayout = false; // Force relayout
    mNeedSelfLayout = false; // Force relayout
    InvalidateLayout();
  }
  VisibilityChanged();
  DebugRefreshInfo();
 
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  if (mVisible)
    StartAnimation(_T("SHOW"));
  else
    StartAnimation(_T("HIDE"));
}

bool nuiWidget::IsVisible(bool combined) const
{
  if (!combined || !mpParent)
    return mVisible;
  if (mVisible) 
    return mpParent->IsVisible(true);
  return mVisible;
}

// virtual
bool nuiWidget::SetMouseCursor(nuiMouseCursor Cursor)
{
  mObjCursor = Cursor;
  DebugRefreshInfo();
  return true;
}

void nuiWidget::SetAttrMouseCursor(nuiMouseCursor Cursor)
{
  mObjCursor = Cursor;
  DebugRefreshInfo();
}



nuiMouseCursor nuiWidget::GetMouseCursor() const
{
  if (mObjCursor == eCursorDoNotSet && mpParent)
    return mpParent->GetMouseCursor();

  return mObjCursor;
}



bool nuiWidget::IsInside(nuiSize X, nuiSize Y)
{
  if (!IsVisible(false))
    return false;

  GlobalToLocal(X,Y);
  if (mpParent)
  {
    nuiRect rect = mRect;
    X += rect.mLeft;
    Y += rect.mTop;
  }
  return IsInsideLocal(X,Y);
}

bool nuiWidget::IsInsideLocal(nuiSize X, nuiSize Y)
{
  if (!IsVisible(false))
    return false;
  if (mInteractiveDecoration)
  {
    nuiRect r = mVisibleRect;
    LocalToLocal(GetParent(), r);
    r.Intersect(mVisibleRect, GetOverDrawRect(false, true));
    return r.IsInside(X, Y);
  }
  if (mInteractiveOD)
  {
    nuiRect r = mVisibleRect;
    LocalToLocal(GetParent(), r);
    r.Intersect(r, GetOverDrawRect(false, false));
    return r.IsInside(X, Y);
  }
  return GetRect().IsInside(X,Y);
}

bool nuiWidget::SetToolTip(const nglString& rToolTip)
{
  SetProperty(_T("ToolTip"), rToolTip);
  return true;
}

nglString nuiWidget::GetToolTip() const
{
  return GetProperty(_T("ToolTip"));
}

bool nuiWidget::ActivateToolTip(nuiWidgetPtr pWidget, bool Now)
{
  nglString tt = GetToolTip();

  if (tt.IsEmpty())
    return false;

  nuiTopLevelPtr pRoot = GetTopLevel();
  if (pRoot)
  {
    return pRoot->ActivateToolTip(pWidget, Now);
  }
  return false;
}

bool nuiWidget::ReleaseToolTip(nuiWidgetPtr pWidget)
{
  nuiTopLevelPtr pRoot = GetTopLevel();
  if (pRoot)
    return pRoot->ReleaseToolTip(pWidget);
  return false;
}


#ifndef _NODND_

/// Drag
bool nuiWidget::Drag(nglDragAndDrop* pDragObj)
{
  nuiMainWindow* pWin = (nuiMainWindow*)GetTopLevel();
  return pWin->Drag(this, pDragObj);
  
  return true;
}




void nuiWidget::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
  // Do nothing, drag and drop is not supported by default.
}
void nuiWidget::OnDragStop(bool canceled)
{
  // Do nothing, drag and drop is not supported by default.
}


/// Drop

bool nuiWidget::OnCanDrop(nglDragAndDrop* pDragObject,nuiSize X,nuiSize Y)
{
  return false; // Or decline the DnD operation by default.
}

void nuiWidget::OnDropped(nglDragAndDrop* pDragObject,nuiSize X,nuiSize Y, nglMouseInfo::Flags Button)
{
  // Do nothing, drag and drop is not supported by default.
}

void nuiWidget::OnDropLeave()
{
  // Do nothing, drag and drop is not supported by default.
}

#endif//_NODND_

uint32 GetParentCount(nuiWidget* pWidget)
{
  nuiWidgetPtr pParent = pWidget->GetParent();
  if (pParent)
    return GetParentCount(pParent) + 1;

  return 0;
}

// Layout stuff
const nuiRect& nuiWidget::GetIdealRect()
{
  if (mNeedIdealRect)
  {
    mIdealRect = CalcIdealSize();

    if (mHasUserSize && mHasUserPos)
      mIdealRect = mUserRect;
    else
    {
      if (mHasUserWidth)
        mIdealRect.SetWidth(mUserRect.GetWidth());
      if (mHasUserHeight)
        mIdealRect.SetHeight(mUserRect.GetHeight());
    }

    if (mMaxWidth > 0.0f)
      mIdealRect.Right() = MIN(mMaxWidth, mIdealRect.GetWidth());
    if (mMaxHeight > 0.0f)
      mIdealRect.Bottom() = MIN(mMaxHeight, mIdealRect.GetHeight());
    if (mMinWidth > 0.0f)
      mIdealRect.Right() = MAX(mMinWidth, mIdealRect.GetWidth());
    if (mMinHeight > 0.0f)
      mIdealRect.Bottom() = MAX(mMinHeight, mIdealRect.GetHeight());

    if (mHasUserPos && !mHasUserSize)
      mIdealRect.MoveTo(mUserRect.mLeft, mUserRect.mTop);
    

    mIdealRect.Bottom() += GetActualBorderBottom();
    mIdealRect.Top() -= GetActualBorderTop();
    mIdealRect.Left() -= GetActualBorderLeft();
    mIdealRect.Right() += GetActualBorderRight();
    mIdealRect.Move(-GetActualBorderLeft(), -GetActualBorderTop());
    mIdealRect.RoundToBiggest();

  }

#ifdef NUI_LOG_GETIDEALRECT
  nglString _log;
  uint32 parentcount = GetParentCount(this);
  _log .Add(_T("|  "), parentcount)
    .Add(mNeedIdealRect?_T(">>> "):_T(""))
    .Add(GetObjectClass())
    .Add(_T(" / "))
    .Add(GetObjectName())
    .Add(_T(" [GetIdealRect] => "))
    .Add(mIdealRect.GetValue())
    .AddNewLine();

    NGL_OUT(_log.GetChars());
#endif //NUI_LOG_GETIDEALRECT

  mNeedIdealRect = false;
  return mIdealRect;
}

bool nuiWidget::SetRect(const nuiRect& rRect)
{
  //NGL_OUT(_T("nuiWidget::SetRect on '%ls' (%f, %f - %f, %f)\n"), GetProperty("Class").GetChars(), rRect.mLeft, rRect.mTop, rRect.GetWidth(), rRect.GetHeight());
  bool inval = mNeedInvalidateOnSetRect;
  if (!(mRect == rRect))
    inval = true;

  if (inval)
    Invalidate();
  if (mForceIdealSize) 
    mRect.Set(rRect.Left(), rRect.Top(), mIdealRect.GetWidth(), mIdealRect.GetHeight());
  else 
    mRect = rRect;
  mVisibleRect = GetOverDrawRect(true, true);
  
  if (inval)
    Invalidate();

  // This is an anti bug in the case people continue to call SetRect instead of SetLayout.
  mNeedSelfLayout = false;
  mNeedLayout = false;
  DebugRefreshInfo();
  return true;
}

void nuiWidget::SetBorder(nuiSize X, nuiSize Y)
{
  mBorderLeft = mBorderRight = X;
  mBorderTop = mBorderBottom = Y;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderLeft(nuiSize border)
{
  mBorderLeft = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderTop(nuiSize border)
{
  mBorderTop = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderRight(nuiSize border)
{
  mBorderRight = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderBottom(nuiSize border)
{
  mBorderBottom = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

nuiSize nuiWidget::GetBorderLeft() const
{
  return mBorderLeft;
}

nuiSize nuiWidget::GetBorderTop() const
{
  return mBorderTop;
}

nuiSize nuiWidget::GetBorderRight() const
{
  return mBorderRight;
}

nuiSize nuiWidget::GetBorderBottom() const
{
  return mBorderBottom;
}

nuiSize nuiWidget::GetActualBorderLeft() const
{
  nuiSize Left = mBorderLeft;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Left = MAX(Left, mpDecoration->GetBorder(nuiLeft));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Left = MAX(Left, mpFocusDecoration->GetBorder(nuiLeft));
  return Left;
}

nuiSize nuiWidget::GetActualBorderTop() const
{
  nuiSize Top = mBorderTop;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Top = MAX(Top, mpDecoration->GetBorder(nuiTop));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Top = MAX(Top, mpFocusDecoration->GetBorder(nuiTop));
  return Top;
}

nuiSize nuiWidget::GetActualBorderRight() const
{
  nuiSize Right = mBorderRight;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Right = MAX(Right, mpDecoration->GetBorder(nuiRight));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Right = MAX(Right, mpFocusDecoration->GetBorder(nuiRight));
  return Right;
}

nuiSize nuiWidget::GetActualBorderBottom() const
{
  nuiSize Bottom = mBorderBottom;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Bottom = MAX(Bottom, mpDecoration->GetBorder(nuiBottom));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Bottom = MAX(Bottom, mpFocusDecoration->GetBorder(nuiBottom));
  return Bottom;
}


void nuiWidget::SetBorder(nuiSize Left, nuiSize Right, nuiSize Top, nuiSize Bottom)
{
  mBorderLeft = Left;
  mBorderRight = Right;
  mBorderTop = Top;
  mBorderBottom = Bottom;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::GetBorder(nuiSize& rLeft, nuiSize& rRight, nuiSize& rTop, nuiSize& rBottom)
{
  rLeft = mBorderLeft;
  rRight = mBorderRight;
  rTop = mBorderTop;
  rBottom = mBorderBottom;
}

void nuiWidget::SetVisibleRect(const nuiRect& rRect)
{
  mVisibleRect = rRect;
  Invalidate();
}

const nuiRect& nuiWidget::GetVisibleRect() const
{
  return mVisibleRect;
}

static nglString GetSurfaceName(nuiWidget* pWidget)
{
  static uint32 gSurfaceCount = 0;
  nglString str;
  str.CFormat(_T("'%ls'/'%ls' %x %d"), pWidget->GetObjectClass().GetChars(), pWidget->GetObjectName().GetChars(), pWidget, gSurfaceCount++);
  return str;
}

void nuiWidget::UpdateSurfaceRect(const nuiRect& rRect)
{
  if (mSurfaceEnabled && (mpSurface->GetWidth() != mRect.GetWidth() || mpSurface->GetHeight() != mRect.GetHeight()))
  {
    //NGL_OUT(_T("UpdateSurfaceRect... %f * %f\n"), rRect.GetWidth(), rRect.GetHeight());
    nglString str(GetSurfaceName(this));
    mpSurface->Release();
    mpSurface = nuiSurface::CreateSurface(str, rRect.GetWidth(), rRect.GetHeight());
    mpSurface->Acquire();
    mpSurface->SetRenderToTexture(true);
    nuiTexture* pSurfaceTexture = nuiTexture::GetTexture(mpSurface, false);
    mpSurface->SetTexture(pSurfaceTexture);
    //#FIXME what should we do about overdraw here?
    //NGL_OUT(_T("UpdateSurfaceRectOK\n"));
  }
}

bool nuiWidget::SetLayout(const nuiRect& rRect)
{
  bool res = false;
  nuiRect rect(GetIdealRect().Size());
  nuiRect tmp(rRect.Size());
  rect.Intersect(rect, tmp);
  
  if (mPosition != nuiFill)
  {
    rect.SetPosition(mPosition, rRect);
  }
  else
  {
    if (mFillRule == nuiFill)
    {
      rect.SetPosition(mPosition, rRect);
    }
    else if (mFillRule == nuiTile)
    {
      rect = rRect;
    }
    else
    {
      nuiRect r = rRect;
      float ratio,rratio,rw,rh;
      ratio  = (float)r.GetWidth()    / (float)r.GetHeight();
      rratio = (float)rect.GetWidth() / (float)rect.GetHeight();
      
      if (ratio < rratio) 
      {
        rw = (float)r.GetWidth();
        rh = rw / rratio;
      }
      
      else 
      {
        rh = (float)r.GetHeight();
        rw = rratio * rh;
      }
      
      rect = nuiRect(0.0f, 0.0f, rw, rh);
      rect.SetPosition(mFillRule, rRect);
    }
  }
    
  rect.Left()   += GetActualBorderLeft();
  rect.Right()  -= GetActualBorderRight();
  rect.Top()    += GetActualBorderTop();
  rect.Bottom() -= GetActualBorderBottom();

  rect.RoundToNearest();
    
  bool PositionChanged = (rect.Left() != mRect.Left()) || (rect.Top() != mRect.Top());
  bool SizeChanged = !rect.Size().IsEqual(mRect.Size());
  mNeedSelfLayout = mNeedSelfLayout || mClippingOptims || SizeChanged;

  if (mNeedSelfLayout)
  {
    res = SetRect(rect);
    Invalidate();
  }
  else
  {
    // Is this case the widget has just been moved inside its parent. No need to re layout it, only change the rect...
    mRect = rect;
  }
  mVisibleRect = GetOverDrawRect(true, true);

  if (PositionChanged && mpParent)
      mpParent->Invalidate();

  mNeedSelfLayout = false;
  mNeedLayout = false;
  DebugRefreshInfo();
  return res;
}

nuiRect nuiWidget::CalcIdealSize()
{
  // Implement widget layout politics here
  return mIdealRect;
}


void nuiWidget::SetUserWidth(nuiSize s)
{
  mUserRect.SetWidth(s);
  mHasUserWidth = true;
}

nuiSize nuiWidget::GetUserWidth()
{
  return mUserRect.GetWidth();
}

void nuiWidget::SetUserHeight(nuiSize s)
{
  mUserRect.SetHeight(s);
  mHasUserHeight = true;
}

nuiSize nuiWidget::GetUserHeight()
{
  return mUserRect.GetHeight();
}


// User size
void nuiWidget::SetUserRect(const nuiRect& rRect)
{
  if (!(mUserRect == rRect) || !mHasUserPos || !mHasUserSize)
  {
    mUserRect = rRect;
    bool SizeChanged = !mUserRect.Size().IsEqual(mRect.Size());
    bool optim = HasUserRect() && !SizeChanged;
    mHasUserSize = true;
    mHasUserPos = true;
    mHasUserWidth = true;
    mHasUserHeight = true;
    UserRectChanged();

    mIdealRect = mUserRect;

    if (optim)
    {
      SetRect(rRect);
      mpParent->Invalidate();
      Invalidate();
    }
    else
    {
      InvalidateLayout();
    }
    DebugRefreshInfo();
  }
}

const nuiRect& nuiWidget::GetUserRect() const
{
  return mUserRect;
}

void nuiWidget::UnsetUserRect()
{
  if (mHasUserPos || mHasUserSize)
  {
    mHasUserSize = false;
    mHasUserPos = false;
    mHasUserWidth = false;
    mHasUserHeight = false;
    InvalidateLayout();
    DebugRefreshInfo();
  }
}

void nuiWidget::SetUserSize(nuiSize X,nuiSize Y)
{
  mUserRect.SetSize(X,Y);
  mHasUserSize = true;
  mHasUserWidth = true;
  mHasUserHeight = true;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::UnsetUserSize()
{
  if (mHasUserSize)
  {
    mHasUserSize = false;
    mHasUserWidth = false;
    mHasUserHeight =false;
    InvalidateLayout();
    DebugRefreshInfo();
  }
}

void nuiWidget::SetUserPos(nuiSize X, nuiSize Y)
{
  mUserRect.MoveTo(X,Y);
  mHasUserPos = true;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::UnsetUserPos()
{
  mHasUserPos = false;
  InvalidateLayout();
  DebugRefreshInfo();
}

bool nuiWidget::HasUserRect() const
{
  return mHasUserSize && mHasUserPos;
}

bool nuiWidget::HasUserSize() const
{
  return mHasUserSize;
}

bool nuiWidget::HasUserWidth() const
{
  return mHasUserWidth;
}

bool nuiWidget::HasUserHeight() const
{
  return mHasUserHeight;
}



bool nuiWidget::HasUserPos() const
{
  return mHasUserPos;
}

void nuiWidget::ForceIdealRect(bool Force)
{
  mForceIdealSize = Force;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::GetHotRect(nuiRect& rRect) const
{
  rRect = mHotRect;
}

const nuiRect& nuiWidget::GetHotRect() const
{
  return mHotRect;
}

void nuiWidget::SetHotRect(const nuiRect& rRect)
{
  mHotRect = rRect;
  HotRectChanged();
  if (mpParent)
  {
    nuiRect r(rRect);
    LocalToLocal(mpParent, r);
    mpParent->OnChildHotRectChanged(this, r);
  }
  DebugRefreshInfo();
}


void nuiWidget::SetSelectionExclusive(bool Exclusive)
{
  mSelectionExclusive = Exclusive;
  DebugRefreshInfo();
}

bool nuiWidget::IsSelectionExclusive() const
{
  return mSelectionExclusive;
}

void nuiWidget::SetRedrawOnHover(bool RedrawOnHover)
{
  mRedrawOnHover = RedrawOnHover;
  DebugRefreshInfo();
}

bool nuiWidget::GetRedrawOnHover() const
{
  return mRedrawOnHover;
}

nuiWidgetPtr nuiWidget::Find(const nglString& rName) 
{ 
  int slash = rName.Find('/'); 

  if (slash >= 0) 
    return NULL; 
  if (rName.IsEmpty())
    return this; 
  return NULL;
} 

/* Animation Support: */
void nuiWidget::StartAutoDraw(double FrameRate)
{
  StopAutoDraw();
  mpAnimationTimer = new nuiTimer(1.0/FrameRate);
  mGenericWidgetSink.Connect(mpAnimationTimer->Tick, &nuiWidget::Animate);
  mpAnimationTimer->Start();
  DebugRefreshInfo();
}

void nuiWidget::AnimateLayout(bool RecalcLayout)
{
  mAnimateLayout = RecalcLayout;
  DebugRefreshInfo();
}

void nuiWidget::StopAutoDraw()
{
  if (mpAnimationTimer)
    delete mpAnimationTimer;
  mpAnimationTimer = NULL;
  DebugRefreshInfo();
}

bool nuiWidget::Animate(const nuiEvent& rEvent)
{
  if (mVisible)
  {
    if (mAnimateLayout)
      InvalidateLayout();
    else
      Invalidate();
  }
  return false;
}

nuiAnimation* nuiWidget::GetAnimation(const nglString& rName)
{
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.find(rName);

  if (it == mAnimations.end())
    return NULL;

  return (*it).second;
}

void nuiWidget::GetAnimations(std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>& rAnims)
{
  rAnims = mAnimations;
}

uint32 nuiWidget::GetAnimCount()
{
  return (uint32)mAnimations.size();
}

void nuiWidget::AddAnimation(const nglString& rName, nuiAnimation* pAnim)
{
  nuiAnimation* pOldAnim = GetAnimation(rName);
  if (pOldAnim)
    delete pOldAnim;

  mAnimations[rName] = pAnim;
  if (rName == _T("TRASH"))
    mGenericWidgetSink.Connect(pAnim->AnimStop, &nuiWidget::AutoTrash);
  DebugRefreshInfo();
}

void nuiWidget::DelAnimation(const nglString& rName)
{
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.find(rName);

  if (it == mAnimations.end())
    return;

  mAnimations.erase(it);
  DebugRefreshInfo();
}

void nuiWidget::ClearAnimations()
{
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.begin();
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator end = mAnimations.end();

  for ( ; it != end; ++it)
    delete (*it).second;

  mAnimations.clear();
  DebugRefreshInfo();
}

void nuiWidget::AnimateAll(bool Set, bool Reset)
{
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.begin();
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator end = mAnimations.end();

  for ( ; it != end; ++it)
  {
    if (Reset)
      (*it).second->SetTime(0);

    if (Set)
      (*it).second->Play();
    else
      (*it).second->Stop();
  }
  DebugRefreshInfo();
}


void nuiWidget::ResetAnimation(const nglString& rName)
{
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
    pAnim->SetTime(0);
  DebugRefreshInfo();
}

void nuiWidget::StartAnimation(const nglString& rName, double Time)
{
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
  {
    pAnim->SetTime(Time);
    pAnim->Play();
  }
  DebugRefreshInfo();
}

void nuiWidget::StopAnimation(const nglString& rName)
{
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
    pAnim->Stop();
  DebugRefreshInfo();
}

/// Matrix Operations:
void nuiWidget::LoadIdentityMatrix()
{
  Invalidate();
  mMatrix.SetIdentity();
  mMatrixIsIdentity = true;
  Invalidate();
  DebugRefreshInfo();
}

void nuiWidget::GetMatrix(nuiMatrix& rMatrix) const
{
  rMatrix = mMatrix;
}

const nuiMatrix& nuiWidget::GetMatrix() const
{
  return mMatrix;
}

void nuiWidget::SetMatrix(const nuiMatrix& rMatrix)
{
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();
  
  mMatrix = rMatrix;
  mMatrixIsIdentity = mMatrix.IsIdentity();
  
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();
  
  if (mpParent)
    mpParent->BroadcastInvalidate(this);
  DebugRefreshInfo();
}

void nuiWidget::EnableRenderCache(bool set)
{
  if (mUseRenderCache != set)
  {
    mUseRenderCache = set;
    if (mUseRenderCache)
    {
      if (!mpRenderCache)
        mpRenderCache = new nuiMetaPainter(nuiRect());
      mpRenderCache->Reset(NULL);
    }
    else
    {
      delete mpRenderCache;
      mpRenderCache = NULL;
    }
    
    Invalidate();
    DebugRefreshInfo();
  }
}

bool nuiWidget::IsRenderCacheEnabled()
{
  return mUseRenderCache;
}

void nuiWidget::EnableSurface(bool Set)
{
  if (mSurfaceEnabled == Set)
    return;
  mSurfaceEnabled = Set;
  if (mSurfaceEnabled)
  {
    nglString str(GetSurfaceName(this));
    nuiRect r(GetRect());
    //NGL_OUT(_T("EnableSurface... %f * %f\n"), r.GetWidth(), r.GetHeight());
    mpSurface = nuiSurface::CreateSurface(str, r.GetWidth(), r.GetHeight());
    mpSurface->Acquire();
    mpSurface->SetRenderToTexture(true);
    nuiTexture* pSurfaceTexture = nuiTexture::GetTexture(mpSurface, false);
    mpSurface->SetTexture(pSurfaceTexture);
    //#FIXME what should we do about overdraw here?
    //NGL_OUT(_T("EnableSurfaceOK\n"));
    InvalidateSurface();
  }
  else
  {
    mpSurface->Release();
    mpSurface = NULL;
  }
  Invalidate();
  DebugRefreshInfo();
}

bool nuiWidget::IsSurfaceEnabled() const
{
  return mSurfaceEnabled;
}

void nuiWidget::SetColor(nuiWidgetElement element, const nuiColor& rColor)
{
  mWidgetElementColors[element] = rColor;
  Invalidate();
  DebugRefreshInfo();
}

void nuiWidget::DelColor(nuiWidgetElement element)
{
  mWidgetElementColors.erase(element);
  Invalidate();
  DebugRefreshInfo();
}

nuiColor nuiWidget::GetColor(nuiWidgetElement element)
{
  nuiColor col;
  std::map<nuiWidgetElement, nuiColor>::iterator it = mWidgetElementColors.find(element);
  if (it != mWidgetElementColors.end())
    col = it->second;
  else
  {
    if (mpParent)
      col = mpParent->GetColor(element);
    else
    {
      nuiTheme* pTheme = GetTheme();
      NGL_ASSERT(pTheme);
      col = pTheme->GetElementColor(element); // Return the theme color
      pTheme->Release();
    }
  }

  if (mMixAlpha)
  {
    float alpha = GetAlpha(false);
    col.Alpha() *= alpha;
  }
  return col;
}

void nuiWidget::SetFillColor(nuiDrawContext* pContext, nuiWidgetElement Element)
{
  pContext->SetFillColor(GetColor(Element));
}

void nuiWidget::SetStrokeColor(nuiDrawContext* pContext, nuiWidgetElement Element)
{
  pContext->SetStrokeColor(GetColor(Element));
}

void nuiWidget::SetTheme(nuiTheme* pTheme)
{
  if (mpTheme)
    mpTheme->Release();
  mpTheme = pTheme;
  if (mpTheme)
    mpTheme->Acquire();
  DebugRefreshInfo();
}

nuiTheme* nuiWidget::GetTheme()
{
  // Do we have a local theme?
  if (mpTheme)
  {
    mpTheme->Acquire();
    return mpTheme;
  }
  // Do we have a parent that may have a local theme?
  if (mpParent)
    return mpParent->GetTheme();
  // Revert to the global theme:
  return nuiTheme::GetTheme();
}

bool nuiWidget::AutoTrash(const nuiEvent& rEvent)
{
  Trash();
  return false;
}

void nuiWidget::EnableAutoClipSelf(bool Set, bool Recurse)
{
  if (mAutoClipSelf != Set)
  {
    mAutoClipSelf = Set;
    Invalidate();
    DebugRefreshInfo();
  }
}

bool nuiWidget::IsAutoClipSelfEnabled() const
{
  return mAutoClipSelf;
}

bool nuiWidget::IsDrawingInCache(bool Recurse) 
{ 
  if (mDrawingInCache)
    return true;
  if (Recurse && mpParent)
    return mpParent->IsDrawingInCache(Recurse);
  return false;
}

#undef max
#undef min


//class nuiWidget::LayoutConstraint
nuiWidget::LayoutConstraint::LayoutConstraint()
{
  mMaxWidth = mMaxHeight = std::numeric_limits<float>::max();
  mMinWidth = mMinHeight = 0;
}

nuiWidget::LayoutConstraint::LayoutConstraint(const LayoutConstraint& rSource)
{
  *this = rSource;
}

nuiWidget::LayoutConstraint& nuiWidget::LayoutConstraint::operator=(const LayoutConstraint& rSource)
{
  mMaxWidth = rSource.mMaxWidth;
  mMinWidth = rSource.mMinWidth;
  mMaxHeight = rSource.mMaxHeight;
  mMinHeight = rSource.mMinHeight;

  return *this;
}

bool nuiWidget::LayoutConstraint::operator==(const LayoutConstraint& rSource) const
{
  return ( mMaxWidth == rSource.mMaxWidth
        && mMinWidth == rSource.mMinWidth
        && mMaxHeight == rSource.mMaxHeight
        && mMinHeight == rSource.mMinHeight );
}

bool nuiWidget::SetLayoutConstraint(const nuiWidget::LayoutConstraint& rConstraint)
{
  if (!(mConstraint == rConstraint))
  {
    mConstraint = rConstraint;
    mNeedIdealRect = mNeedIdealRect || mCanRespectConstraint;
    //InvalidateLayout();
    return true;
  }
  return false;
}


const nuiWidget::LayoutConstraint& nuiWidget::GetLayoutConstraint() const
{
  return mConstraint;
}


nuiWidgetPtr nuiWidget::GetChild(nuiSize X, nuiSize Y)
{
  return IsInsideLocal(X,Y)?this:NULL;
}

void nuiWidget::SetDebug(int32 DebugLevel)
{
  mDebugLevel = DebugLevel;
  InvalidateLayout();
}

int32 nuiWidget::GetDebug(bool recursive) const
{
  if (recursive)
  {
    if (mpParent)
    {
      return MAX(mDebugLevel, mpParent->GetDebug(recursive));
    }
  }
  return mDebugLevel;
}

nuiHotKey* nuiWidget::RegisterHotKeyKey(const nglString& rName, nglKeyCode Trigger, nuiKeyModifier Modifiers, bool Priority /*= false*/, bool FireOnKeyUp /*= false*/, const nglString& rDescription /*= nglString::Empty*/)
{
  NGL_ASSERT(GetTopLevel());
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator it = mHotKeyEvents.find(rName);
  if (it == mHotKeyEvents.end())
    mHotKeyEvents[rName] = new nuiSimpleEventSource<nuiWidgetActivated>();
  return GetTopLevel()->RegisterHotKeyKey(rName, Trigger, Modifiers, Priority, FireOnKeyUp, rDescription);
}

nuiHotKey* nuiWidget::RegisterHotKeyChar(const nglString& rName, nglChar Trigger, nuiKeyModifier Modifiers, bool Priority /*= false*/, bool FireOnKeyUp /*= false*/, const nglString& rDescription /*= nglString::Empty*/)
{
  NGL_ASSERT(GetTopLevel());
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator it = mHotKeyEvents.find(rName);
  if (it == mHotKeyEvents.end())
    mHotKeyEvents[rName] = new nuiSimpleEventSource<nuiWidgetActivated>();
  return GetTopLevel()->RegisterHotKeyChar(rName, Trigger, Modifiers, Priority, FireOnKeyUp, rDescription);
}

nuiHotKey* nuiWidget::GetHotKey(const nglString& rName)
{
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator it = mHotKeyEvents.find(rName);
  if (it != mHotKeyEvents.end())
  {
    return GetTopLevel()->GetHotKey(rName);
  }
  else
  {
    return NULL;
  }
}

nuiSimpleEventSource<nuiWidgetActivated>& nuiWidget::GetHotKeyEvent(const nglString& rName)
{
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::iterator it = mHotKeyEvents.find(rName);
  NGL_ASSERT(it != mHotKeyEvents.end());
  return *(it->second);
}

void nuiWidget::DelHotKey(const nglString& rName)
{
  NGL_ASSERT(GetTopLevel());
  GetTopLevel()->DelHotKey(rName);
  mHotKeyEvents.erase(rName);
}

void nuiWidget::SetMaxIdealWidth(float MaxWidth)
{
  if (mMaxWidth == MaxWidth)
    return;
  mMaxWidth = MaxWidth;
  InvalidateLayout();
}

void nuiWidget::SetMaxIdealHeight(float MaxHeight)
{
  if (mMaxHeight == MaxHeight)
    return;
  mMaxHeight = MaxHeight;
  InvalidateLayout();
}

void nuiWidget::SetMinIdealWidth(float MinWidth)
{
  if (mMinWidth == MinWidth)
    return;
  mMinWidth = MinWidth;
  InvalidateLayout();
}

void nuiWidget::SetMinIdealHeight(float MinHeight)
{
  if (mMinHeight == MinHeight)
    return;
  mMinHeight = MinHeight;
  InvalidateLayout();
}

float nuiWidget::GetMaxIdealWidth() const
{
  return mMaxWidth;
}

float nuiWidget::GetMaxIdealHeight() const
{
  return mMaxHeight;
}

float nuiWidget::GetMinIdealWidth() const
{
  return mMinWidth;
}

float nuiWidget::GetMinIdealHeight() const
{
  return mMinHeight;
}


nuiEventSource* nuiWidget::GetEvent(const nglString& rName)
{
  std::map<nglString, nuiEventSource*, nglString::LessFunctor>::iterator it = mEventMap.find(rName);
  if (it == mEventMap.end())
    return NULL;
  return (*it).second;
}

void nuiWidget::GetEvents(std::vector<nglString>& rNames)
{
  std::map<nglString, nuiEventSource*, nglString::LessFunctor>::iterator it;    
  std::map<nglString, nuiEventSource*, nglString::LessFunctor>::iterator end = mEventMap.end();
  
  for (it = mEventMap.begin(); it != end; ++it)
    rNames.push_back((*it).first);
}

void nuiWidget::AddEvent(const nglString& rName, nuiEventSource& rEvent)
{
  mEventMap[rName] = &rEvent;
}

void nuiWidget::UpdateLayout()
{
  GetIdealRect();
  nuiRect r(GetBorderedRect());
  SetRect(r);
  Invalidate();
}

void nuiWidget::SetOverDraw(nuiSize Left, nuiSize Top, nuiSize Right, nuiSize Bottom)
{
  if ( ( (Left != mODLeft) || (Right != mODRight) || (Top != mODTop) || (Bottom != mODBottom) ) )
  {
    Invalidate();
    mODLeft = Left;
    mODRight = Right;
    mODTop = Top;
    mODBottom = Bottom;
    Invalidate();
  }
}

void nuiWidget::ResetOverDraw()
{
  SetOverDraw(0, 0, 0, 0);
}

void nuiWidget::GetOverDraw(nuiSize& Left, nuiSize& Top, nuiSize& Right, nuiSize& Bottom, bool IncludeDecorations) const
{
  Left = mODLeft;
  Right = mODRight;
  Top = mODTop;
  Bottom = mODBottom;
  
  if (mpDecoration && IncludeDecorations)
  {    
    if (mDecorationMode == eDecorationOverdraw || mDecorationMode == eDecorationBorder)
    {
      Left   = MAX(Left  , mpDecoration->GetBorder(nuiLeft));
      Top    = MAX(Top   , mpDecoration->GetBorder(nuiTop));
      Right  = MAX(Right , mpDecoration->GetBorder(nuiRight));
      Bottom = MAX(Bottom, mpDecoration->GetBorder(nuiBottom));
    }
  }
  
  if (mpFocusDecoration && IncludeDecorations)
  {    
    if (mFocusDecorationMode == eDecorationOverdraw || mFocusDecorationMode == eDecorationBorder)
    {
      Left   = MAX(Left  , mpFocusDecoration->GetBorder(nuiLeft));
      Top    = MAX(Top   , mpFocusDecoration->GetBorder(nuiTop));
      Right  = MAX(Right , mpFocusDecoration->GetBorder(nuiRight));
      Bottom = MAX(Bottom, mpFocusDecoration->GetBorder(nuiBottom));
    }
  }
  
}

nuiRect nuiWidget::GetOverDrawRect(bool LocalRect, bool IncludeDecorations) const
{
  nuiRect r(GetRect());
  if (LocalRect)
    r = r.Size();
  
  nuiSize Left = 0;
  nuiSize Right = 0;
  nuiSize Top = 0;
  nuiSize Bottom = 0;

  GetOverDraw(Left, Top, Right, Bottom, IncludeDecorations);
  
  r.Set(r.Left() - Left,
        r.Top() - Top,
        r.Right() + Right,
        r.Bottom() + Bottom,
        false);
  return r;
}

void nuiWidget::EnableInteractiveOverDraw(bool set)
{
  if (mInteractiveOD != set)
  {
    mInteractiveOD = set;
    Invalidate();
  }
}

void nuiWidget::EnableInteractiveDecoration(bool set)
{
  if (mInteractiveDecoration != set)
  {
    mInteractiveDecoration = set;
    Invalidate();
  }
}

bool nuiWidget::IsOverDrawInteractive() const
{
  return mInteractiveOD;
}

bool nuiWidget::IsDecorationInteractive() const
{
  return mInteractiveDecoration;
}

void nuiWidget::InitDefaultDecorations()
{
  if (mDefaultDecorations.size())
    return;

  nuiDefaultDecoration::Init();
}

// static 
void nuiWidget::SetDefaultDecoration(int32 objectClassIndex, nuiDecorationDelegate dlg)
{
  if (objectClassIndex >= mDefaultDecorations.size())
    mDefaultDecorations.resize(objectClassIndex+1);

  mDefaultDecorations[objectClassIndex] = dlg;
}


void nuiWidget::SetDecoration(const nglString& rName)
{
  nuiDecoration* pDecoration = nuiDecoration::Get(rName);
  SetDecoration(pDecoration, mDecorationMode, true);
}

void nuiWidget::SetDecorationMode(nuiDecorationMode Mode)
{
  mDecorationMode = Mode;
  InvalidateLayout();
}

const nglString& nuiWidget::GetDecorationName() const
{
  if (mpDecoration)
    return mpDecoration->GetObjectName();
  return nglString::Null;
}

nuiDecorationMode nuiWidget::GetDecorationMode() const
{
  return mDecorationMode;
}

void nuiWidget::SetDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode, bool AlreadyAcquired)
{
  if (pDecoration && !AlreadyAcquired)
    pDecoration->Acquire();
  if (mpDecoration)
    mpDecoration->Release();
  
  mpDecoration = pDecoration;

  SetDecorationMode(Mode);
  
  InvalidateLayout();
}

nuiDecoration* nuiWidget::GetDecoration() const
{
  return mpDecoration;
}


void nuiWidget::EnableDecoration(bool set)
{
  mDecorationEnabled = set;
  
  if (!mDecorationEnabled)
  {
    nuiDecoration* pDeco = GetDecoration();
    if (pDeco)
      mDecorationName = pDeco->GetName();
    SetDecoration(NULL);
  }
  else
  {
    nuiDecoration* pDeco = nuiDecoration::Get(mDecorationName);
    SetDecoration(pDeco, mDecorationMode);
  }
}

bool nuiWidget::IsDecorationEnabled() const
{
  return mDecorationEnabled;
}




/// Focus Decoration:
void nuiWidget::SetFocusDecoration(const nglString& rName)
{
  nuiDecoration* pDecoration = nuiDecoration::Get(rName);
  SetFocusDecoration(pDecoration, mFocusDecorationMode);
}

void nuiWidget::SetFocusDecorationMode(nuiDecorationMode Mode)
{
  mFocusDecorationMode = Mode;
  InvalidateLayout();
}

const nglString& nuiWidget::GetFocusDecorationName() const
{
  if (mpFocusDecoration)
    return mpFocusDecoration->GetObjectName();
  return nglString::Null;
}

nuiDecorationMode nuiWidget::GetFocusDecorationMode() const
{
  return mFocusDecorationMode;
}

void nuiWidget::SetFocusDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode)
{
  if (pDecoration)
    pDecoration->Acquire();
  if (mpFocusDecoration)
    mpFocusDecoration->Release();
  
  mpFocusDecoration = pDecoration;
  
  SetFocusDecorationMode(Mode);
  
  InvalidateLayout();
}

nuiDecoration* nuiWidget::GetFocusDecoration() const
{
  return mpFocusDecoration;
}


//////// TopLevel Management:
void nuiWidget::CallConnectTopLevel(nuiTopLevel* pTopLevel)
{
  // Apply CSS, do default stuff, etc...
  if (HasFocus())
    pTopLevel->SetFocus(this);
  pTopLevel->PrepareWidgetCSS(this, false, NUI_WIDGET_MATCHTAG_ALL);
  ConnectTopLevel();

  // cal delegate for default decoration, if the user has not set any decoration, and if there is a default decoration
  int32 index = GetObjectClassNameIndex();
  if (!GetDecoration() && mDecorationEnabled && (mDefaultDecorations.size() > index))
  {
    nuiDecorationDelegate dlg = mDefaultDecorations[index];
    if (dlg)
      dlg(this);
  }
  
  StartAnimation(_T("SHOW"));
}



void nuiWidget::CallDisconnectTopLevel(nuiTopLevel* pTopLevel)
{
  if (HasFocus())
    UnFocus();
  pTopLevel->DisconnectWidget(this);
  DisconnectTopLevel();
}
 
void nuiWidget::ConnectTopLevel()
{
}

void nuiWidget::DisconnectTopLevel()
{
}

void nuiWidget::ApplyCSSForStateChange(uint32 MatchersTag)
{
  // Apply CSS, do default stuff, etc...
  if (!IsTrashed(true))
  {
    nuiTopLevel* pTopLevel = GetTopLevel();
    if (pTopLevel)
      pTopLevel->PrepareWidgetCSS(this, true, NUI_WIDGET_MATCHTAG_ALL);
  }
}

void nuiWidget::ResetCSSPass()
{
  InternalResetCSSPass();
}

void nuiWidget::InternalResetCSSPass()
{
  mCSSPasses = 0;
}


void nuiWidget::IncrementCSSPass()
{
  mCSSPasses++;
}

uint32 nuiWidget::GetCSSPass() const
{
  return mCSSPasses;
}

void nuiWidget::DrawFocus(nuiDrawContext* pContext, bool FrontOrBack)
{
//#FIXME LBDEBUG : there's a bug somewhere in that, that makes ComboBox tutorial crash with Win32 (ati, vista)
  // deactivate the DrawFocus for now, in order to release the application.
return;


  if (!mShowFocus)
    return;
  
  if (mpFocusDecoration)
  {
    nuiRect sizerect(GetRect().Size());
    mpFocusDecoration->ClientToGlobalRect(sizerect);
    if (FrontOrBack)
      mpFocusDecoration->DrawFront(pContext, this, sizerect);
    else
      mpFocusDecoration->DrawBack(pContext, this, sizerect);
  }
  else
  {
    if (FrontOrBack)
    {
      nuiRect rect(GetRect().Size());
      rect.Bottom() += GetBorderBottom() - 1;
      rect.Top() -= GetBorderTop() + 0;
      rect.Left() -= GetBorderLeft() + 0;
      rect.Right() += GetBorderRight() - 1;
      
      pContext->SetLineWidth(2);
      pContext->SetBlendFunc(nuiBlendTransp);
      pContext->EnableBlending(true);
      pContext->SetStrokeColor(nuiColor(64, 64, 255, 128));
      
      nuiShape shp;
      shp.AddRect(rect);
      
      pContext->DrawShape(&shp, eStrokeShape);
    }
  }
}

void nuiWidget::SetFocusVisible(bool set)
{
  mShowFocus = set;
  Invalidate();
}

bool nuiWidget::IsFocusVisible() const
{
  return mShowFocus;
}



// ***************************************************************************

// static 
std::vector<nuiDecorationDelegate> nuiWidget::mDefaultDecorations;
