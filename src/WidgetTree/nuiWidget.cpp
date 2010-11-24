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
#include "nuiAttributeAnimation.h"
#include "nuiDrawContext.h"
#include "nuiMetaPainter.h"
#include "nuiMainWindow.h"
#include "nuiTheme.h"
#include "nuiDecoration.h"
#include "nuiWidgetMatcher.h"
#include "nuiClampedValueAttributeEditor.h"
#include "nuiColorDecoration.h"
#include "nuiTask.h"
#include "nuiMatrixNode.h"
#include "nuiCSS.h"

#ifdef _UIKIT_
//const bool gGlobalUseRenderCache = false;
const bool gGlobalUseRenderCache = true;
#else
//const bool gGlobalUseRenderCache = false;
const bool gGlobalUseRenderCache = true;
#endif

//#define NUI_LOG_GETIDEALRECT

// Use like this:  nuiAnimation::RunOnAnimationTick(nuiMakeTask(nuiDelayedPlayAnim, eAnimFromStart, Time, count, loopmode));
void nuiDelayedPlayAnim(nuiAnimation* pAnim, nuiAnimWhence Whence, double Time, int32 count, nuiAnimLoop loopmode)
{
  pAnim->Play(count, loopmode);
  pAnim->SetTime(Time, Whence);
}


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
: nuiObject(), mGenericWidgetSink(this)
{
  InitDefaultValues();
  
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



nuiWidget::nuiWidget(const nglString& rObjectName)
: nuiObject(rObjectName), mGenericWidgetSink(this)
{
  InitDefaultValues();
  
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


void nuiWidget::InitDefaultValues()
{
  mODLeft = 0;
  mODRight = 0;
  mODTop = 0;
  mODBottom = 0;
  mInteractiveOD = false;
  mInteractiveDecoration = false;
  mpDecoration = NULL;
  mpFocusDecoration = NULL;
  mShowFocus = SetFocusVisibleDefault;
  mPosition = nuiFill;
  mFillRule = nuiFill;
  mCSSPasses = 0;
  mpMatrixNodes = NULL;
  mpParent = NULL;
  mpTheme = NULL;
  mDecorationEnabled = true;
  mNeedSurfaceRedraw = false;
  mSurfaceEnabled = false;
  mpSurface = NULL;
  mSurfaceColor = nuiColor(255, 255, 255, 255);
  mSurfaceBlendFunc = nuiBlendTransp;  
  mDecorationMode = eDecorationOverdraw;
  mHotKeyMask = -1;
  mClickThru = true;
  mInSetRect = false;
  mInTransition = 0;
  mpLayoutAnimation = NULL;
  mFixedAspectRatio = false;
  mAutoClip = true;
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


static nuiAttributeEditor* GetAlphaAttributeEditor(void* pTarget, nuiAttribute<float>* pAttribute)
{
  return new nuiClampedValueAttributeEditor<float>(nuiAttrib<float>(nuiAttribBase(pTarget, pAttribute)), nuiRange(0, 0, 1, .1, .1, 0));
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
               (nglString(_T("LayoutRect")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetRect),
                nuiMakeDelegate(this, &nuiWidget::SetLayout)));
  
  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("LayoutRectUnsafe")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetRect),
                nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiWidget::InternalSetLayout)));
  
  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("UserRect")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetUserRect),
                nuiMakeDelegate(this, &nuiWidget::SetUserRect)));
  
  AddAttribute(new nuiAttribute<nuiPoint>
               (nglString(_T("UserPos")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetUserPos),
                nuiAttribute<nuiPoint>::SetterDelegate(this, &nuiWidget::SetUserPos)));
  
  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("VisibleRect")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetVisibleRect),
                nuiMakeDelegate(this, &nuiWidget::SetVisibleRect)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("UserWidth")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetUserWidth),
                nuiMakeDelegate(this, &nuiWidget::SetUserWidth)));

  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("UserHeight")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetUserHeight),
                nuiMakeDelegate(this, &nuiWidget::SetUserHeight)));
  
  
  
  
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MinIdealWidth")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMinIdealWidth),
                nuiMakeDelegate(this, &nuiWidget::SetMinIdealWidth)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MinIdealHeight")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMinIdealHeight),
                nuiMakeDelegate(this, &nuiWidget::SetMinIdealHeight)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MaxIdealWidth")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMaxIdealWidth),
                nuiMakeDelegate(this, &nuiWidget::SetMaxIdealWidth)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MaxIdealHeight")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMaxIdealHeight),
                nuiMakeDelegate(this, &nuiWidget::SetMaxIdealHeight)));
  

  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MinWidth")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMinWidth),
                nuiMakeDelegate(this, &nuiWidget::SetMinWidth)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MinHeight")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMinHeight),
                nuiMakeDelegate(this, &nuiWidget::SetMinHeight)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MaxWidth")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMaxWidth),
                nuiMakeDelegate(this, &nuiWidget::SetMaxWidth)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("MaxHeight")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetMaxHeight),
                nuiMakeDelegate(this, &nuiWidget::SetMaxHeight)));
  
  
  
  
  
  
  
  AddAttribute(new nuiAttribute<bool>
              (nglString(_T("UseRenderCache")), nuiUnitBoolean,
               nuiMakeDelegate(this, &nuiWidget::IsRenderCacheEnabled),
               nuiMakeDelegate(this, &nuiWidget::EnableRenderCache)));


  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("RedrawOnHover")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::GetRedrawOnHover),
                nuiMakeDelegate(this, &nuiWidget::SetRedrawOnHover)));
	
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("AutoClip")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::GetAutoClip),
                nuiMakeDelegate(this, &nuiWidget::SetAutoClip)));
	
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Hover")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::GetHover)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Focus")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::HasFocus)));
  
	// Decoration:
  nuiAttribute<const nglString&>* pDecoAttrib = new nuiAttribute<const nglString&>
    (nglString(_T("Decoration")), nuiUnitNone,
     nuiMakeDelegate(this, &nuiWidget::GetDecorationName),
     nuiAttribute<const nglString&>::SetterDelegate(this, &nuiWidget::SetDecoration));
  pDecoAttrib->SetEditor(nuiAttribute<const nglString&>::NewEditorDelegate(&nuiDecoration::GetAttributeEditor));
  AddAttribute(pDecoAttrib);
  
  AddAttribute(new nuiAttribute<nuiDecorationMode>
               (nglString(_T("DecorationMode")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetDecorationMode),
                nuiMakeDelegate(this, &nuiWidget::SetDecorationMode)));

  // Focus Decoration:
  nuiAttribute<const nglString&>* pFocusDecoAttrib = new nuiAttribute<const nglString&>
  (nglString(_T("FocusDecoration")), nuiUnitName,
   nuiMakeDelegate(this, &nuiWidget::GetFocusDecorationName),
   nuiAttribute<const nglString&>::SetterDelegate(this, &nuiWidget::SetFocusDecoration));
  pFocusDecoAttrib->SetEditor(nuiAttribute<const nglString&>::NewEditorDelegate(&nuiDecoration::GetAttributeEditor));
  AddAttribute(pFocusDecoAttrib);

  
  AddAttribute(new nuiAttribute<nuiDecorationMode>
               (nglString(_T("FocusDecorationMode")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::GetFocusDecorationMode),
                nuiMakeDelegate(this, &nuiWidget::SetFocusDecorationMode)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("FocusVisible")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiWidget::IsFocusVisible),
                nuiMakeDelegate(this, &nuiWidget::SetFocusVisible)));
  
  
  // nuiAttribute<nuiSize> <=> nuiAttribute<double>
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("Borders")), nuiUnitSize,
                //nuiMakeDelegate(this, &nuiWidget::GetBorderLeft),
                nuiMakeDelegate(this, &nuiWidget::SetBorders)));
  
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
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("OverDrawLeft")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetOverDrawLeft),
                nuiMakeDelegate(this, &nuiWidget::SetOverDrawLeft)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("OverDrawTop")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetOverDrawTop),
                nuiMakeDelegate(this, &nuiWidget::SetOverDrawTop)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("OverDrawRight")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetOverDrawRight),
                nuiMakeDelegate(this, &nuiWidget::SetOverDrawRight)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("OverDrawBottom")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetOverDrawBottom),
                nuiMakeDelegate(this, &nuiWidget::SetOverDrawBottom)));
  
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
  
  AddAttribute(new nuiAttribute<const nuiMatrix&>
               (nglString(_T("Matrix")), nuiUnitMatrix,
                nuiMakeDelegate(this, &nuiWidget::_GetMatrix),
                nuiMakeDelegate(this, &nuiWidget::SetMatrix)));
  
  AddAttribute(new nuiAttribute<nuiBlendFunc>
               (nglString(_T("SurfaceBlendFunc")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiWidget::GetSurfaceBlendFunc),
                nuiMakeDelegate(this, &nuiWidget::SetSurfaceBlendFunc)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("InteractiveDecoration")), nuiUnitYesNo,
                nuiMakeDelegate(this, &nuiWidget::IsDecorationInteractive),
                nuiMakeDelegate(this, &nuiWidget::EnableInteractiveDecoration)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("InteractiveOverDraw")), nuiUnitYesNo,
                nuiMakeDelegate(this, &nuiWidget::IsOverDrawInteractive),
                nuiMakeDelegate(this, &nuiWidget::EnableInteractiveOverDraw)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("LayoutAnimationDuration")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiWidget::GetLayoutAnimationDuration),
                nuiMakeDelegate(this, &nuiWidget::SetLayoutAnimationDuration)
               ));

  nuiAttribute<float>* pAlphaAttrib = new nuiAttribute<float>
  (nglString(_T("Alpha")), nuiUnitSize,
   nuiMakeDelegate(this, &nuiWidget::GetAlpha),
   nuiMakeDelegate(this, &nuiWidget::SetAlpha)
   );
  pAlphaAttrib->SetEditor(&GetAlphaAttributeEditor);
  AddAttribute(pAlphaAttrib);
  
  AddAttribute(new nuiAttribute<int32>
               (nglString(_T("Debug")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiWidget::_GetDebug),
                nuiMakeDelegate(this, &nuiWidget::SetDebug)
                ));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("FixedAspectRatio")), nuiUnitOnOff,
                nuiMakeDelegate(this, &nuiWidget::GetFixedAspectRatio), 
                nuiMakeDelegate(this, &nuiWidget::SetFixedAspectRatio)));
  
  
}

 
void nuiWidget::Init()
{
  mDebugLevel = 0; // No debug by default.
  mCanRespectConstraint = false; ///< By default the widgets don't care about the constraints imposed by their parents. Only few ones care about this.
  mNeedInvalidateOnSetRect = true;
  mDrawingInCache = false;
  mpRenderCache = NULL;
	mUseRenderCache = false;

  mTrashed = false;
  mDoneTrashed = false;
  mRedrawOnHover = false;

  mMixAlpha = true;
  mInheritAlpha = true;

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

  mBorderRight = mBorderLeft = 0.f;
  mBorderTop = mBorderBottom = 0.f;

  mMinIdealWidth = mMaxIdealWidth = mMinIdealHeight = mMaxIdealHeight = -1.0f;
  mMinWidth = mMaxWidth = mMinHeight = mMaxHeight = -1.0f;

  mpSavedPainter = NULL;

	mEnabled = true;
  mSelected = false;
  mVisible = true;
  
  mWantKeyboardFocus = false;
  mMuteKeyboardFocusDispatch = false;

  LoadIdentityMatrix();
  
  EnableRenderCache(true);
  
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
  NUI_ADD_EVENT(MovedMouse);

  NUI_ADD_EVENT(FocusChanged);
  
  NUI_ADD_EVENT(PreClicked);
  NUI_ADD_EVENT(PreUnclicked);
  NUI_ADD_EVENT(PreMouseMoved);
}

void nuiWidget::InitProperties()
{
}


nuiXMLNode* nuiWidget::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{   
  CheckValid();
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
  CheckValid();
  
  bool res = nuiObject::SetObjectClass(rName);
  ResetCSSPass();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_ALL);
  return res;
}

// virtual from nuiObject
void nuiWidget::SetObjectName(const nglString& rName)
{
  CheckValid();
  
  nuiObject::SetObjectName(rName);
  ResetCSSPass();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_ALL);
}




nglString nuiWidget::Dump()
{
  CheckValid();
  nuiXML* xml = new nuiXML(GetObjectName());
  
  Serialize(xml, true);
  
  nglString dump = xml->Dump();
  
  delete xml;
  return dump;
}


nuiWidget::~nuiWidget()
{
  CheckValid();  
  Destroyed();
  
#ifdef NUI_WIDGET_STATS
  wcount--;
  gWidgetCounter.Add(GetProperty("Class"));
#endif
	
#ifdef _DEBUG_
  if (GetDebug())
  {
    NGL_OUT(_T("nuiWidget::~nuiWidget() [0x%x '%ls':'%ls']\n"), this, GetObjectClass().GetChars(), GetObjectName().GetChars());
  }
#endif
  
  ClearAnimations();

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
  {
    mpSurface->Release();
  }
  delete mpRenderCache;
}

bool nuiWidget::IsTrashed(bool combined) const 
{ 
  CheckValid();
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
  CheckValid();
  NGL_ASSERT(mpParent != this);
  if (mpParent)
    return mpParent->GetTopLevel();
  else
    return NULL;
}


nuiContainerPtr nuiWidget::GetRoot() const
{
  CheckValid();
  if (mpParent)
    return mpParent->GetRoot();
  else
    return NULL;
}

bool nuiWidget::SetParent(nuiContainerPtr pParent)
{
  CheckValid();
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
  CheckValid();
  if (!IsMatrixIdentity())
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
  CheckValid();
  if (!IsMatrixIdentity())
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
  CheckValid();
  if (!IsMatrixIdentity())
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
  CheckValid();
  if (mpParent)
    mpParent->GlobalToLocal(x,y);
  x -= (int)mRect.mLeft;
  y -= (int)mRect.mTop;

  if (!IsMatrixIdentity())
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
  CheckValid();
  if (mpParent)
    mpParent->GlobalToLocal(x,y);
  x -= mRect.mLeft;
  y -= mRect.mTop;

  if (!IsMatrixIdentity())
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
  CheckValid();
  if (mpParent)
  {
    mpParent->GlobalToLocal(rRect);
  }
  rRect.Move(-mRect.mLeft, -mRect.mTop);

  if (!IsMatrixIdentity())
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

void nuiWidget::LocalToLocal(nuiWidgetPtr pWidget,int& x, int& y) const
{                           
  CheckValid();
  if (GetRoot() != pWidget->GetRoot())
    return;

  LocalToGlobal(x,y);
  pWidget->GlobalToLocal(x,y);
}

void nuiWidget::LocalToLocal(nuiWidgetPtr pWidget, nuiSize& x, nuiSize& y) const
{                           
  CheckValid();
  if (GetRoot() != pWidget->GetRoot())
    return;

  LocalToGlobal(x,y);
  pWidget->GlobalToLocal(x,y);
}

void nuiWidget::LocalToLocal(nuiWidgetPtr pWidget,nuiRect& rRect) const
{                           
  CheckValid();
  if (GetRoot() != pWidget->GetRoot())
    return;

  LocalToGlobal(rRect);
  pWidget->GlobalToLocal(rRect);
}

const nuiRect& nuiWidget::GetRect() const
{
  CheckValid();
  return mRect;
}

nuiRect nuiWidget::GetBorderedRect() const
{
  CheckValid();
  nuiRect rect = GetRect();
  rect.Bottom() += GetActualBorderBottom();
  rect.Top() -= GetActualBorderTop();
  rect.Left() -= GetActualBorderLeft();
  rect.Right() += GetActualBorderRight();
  return rect;
}

void nuiWidget::InvalidateRect(const nuiRect& rRect)
{
  CheckValid();
  #ifdef _DEBUG_LAYOUT
  if (GetDebug())
    NGL_OUT(_T("  nuiWidget::InvalidateRect '%ls' [%ls] %ls\n"), GetObjectClass().GetChars(), GetObjectName().GetChars(), rRect.GetValue().GetChars());
  #endif
  
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
  CheckValid();
  nuiRect r = rRect;
  nuiRect rect = GetRect();
  nuiRect size = GetOverDrawRect(true, true);
  r.Intersect(r, size);

  nuiVector vec1(r.Left(),r.Top(),0);
  nuiVector vec2(r.Right(),r.Bottom(),0);
  if (!IsMatrixIdentity())
  {
    nuiMatrix m(GetMatrix());
    //m.InvertHomogenous();

    vec1 = m * vec1;
    vec2 = m * vec2;
  }

  r.Set(vec1[0], vec1[1], vec2[0], vec2[1], false);

  mNeedRender = true;
  if (mSurfaceEnabled)
  {
    mNeedSelfRedraw = true;
    
    AddInvalidRect(r);
  }

  r.Move(rect.Left(), rect.Top());

  if (mpParent)
    mpParent->BroadcastInvalidateRect(pSender, r);
  DebugRefreshInfo();
}

void nuiWidget::Invalidate()
{
  CheckValid();
  if ((mNeedRender && mNeedSelfRedraw))
    return;

  //printf("nuiWidget::Invalidate '%ls [%ls]'\n", GetObjectClass().GetChars() , GetObjectName().GetChars());
  
  if (!IsVisible(true))
  {
#ifdef _DEBUG_
    if (GetDebug())
    {
      NGL_OUT(_T("  nuiWidget::Invalidate '%ls' [%ls]\n"), GetObjectClass().GetChars(), GetObjectName().GetChars());
    }
#endif
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
  CheckValid();
  if (mNeedSurfaceRedraw || !mpSurface)
    return;
  
  mNeedSurfaceRedraw = true;
  mNeedRender = true;

  if (mpParent)
    mpParent->InvalidateRect(GetRect());
  DebugRefreshInfo();
}

nuiSurface* nuiWidget::GetSurface() const
{
  CheckValid();
  return mpSurface;
}

const nuiMatrix& nuiWidget::GetSurfaceMatrix() const
{
  CheckValid();
  return mSurfaceMatrix;
}

void nuiWidget::SetSurfaceMatrix(const nuiMatrix& rMatrix)
{
  CheckValid();
  mSurfaceMatrix = rMatrix;
  InvalidateSurface();
}

const nuiColor& nuiWidget::GetSurfaceColor() const
{
  CheckValid();
  return mSurfaceColor;
}

void nuiWidget::SetSurfaceColor(const nuiColor& rColor)
{
  CheckValid();
  mSurfaceColor = rColor;
  InvalidateSurface();
}

nuiBlendFunc nuiWidget::GetSurfaceBlendFunc() const
{
  CheckValid();
  return mSurfaceBlendFunc;
}

void nuiWidget::SetSurfaceBlendFunc(nuiBlendFunc BlendFunc)
{
  CheckValid();
  mSurfaceBlendFunc = BlendFunc;
  InvalidateSurface();
}


void nuiWidget::SilentInvalidate()
{
  CheckValid();
  mNeedRender = true;
  #ifdef _DEBUG_LAYOUT
  if (GetDebug())
  {
    NGL_OUT(_T("  nuiWidget::SilentInvalidate '%ls' [%ls]\n"), GetObjectClass().GetChars(), GetObjectName().GetChars());
  }
  #endif
  
  mNeedSelfRedraw = true;
  InvalidateSurface();
  if (mpRenderCache)
    mpRenderCache->Reset(NULL);
  DebugRefreshInfo();
}

void nuiWidget::BroadcastInvalidate(nuiWidgetPtr pSender)
{
  CheckValid();
  if (mpParent && !mNeedRender)
  {
    mpParent->BroadcastInvalidate(pSender);
  }

  mNeedRender = true;

  if (mpSurface)
    mNeedSelfRedraw = true;

  DebugRefreshInfo();
}

void nuiWidget::SilentInvalidateLayout()
{
  CheckValid();
  #ifdef _DEBUG_LAYOUT
  if (GetDebug())
  {
    NGL_OUT(_T("  nuiWidget::SilentInvalidateLayout '%ls' [%ls]\n"), GetObjectClass().GetChars(), GetObjectName().GetChars());
  }
  #endif
  mNeedSelfLayout = true;
  mNeedLayout = true;
  mNeedIdealRect = true;
  mNeedRender = true;
  mNeedSelfRedraw = true;
  DebugRefreshInfo();
}

void nuiWidget::InvalidateLayout()
{
  CheckValid();
  if (!mNeedSelfLayout && HasUserRect())
  {
    UpdateLayout();
    return;
  }
  
  bool broadcast = !mNeedLayout;
  SilentInvalidateLayout();

  if (mpParent && broadcast)
  {
    //NGL_OUT(_T("InvalidateLayout + Broadcast from %ls\n"), GetObjectClass().GetChars());
    mpParent->BroadcastInvalidateLayout(this, false);
  }
  DebugRefreshInfo();
}

void nuiWidget::ForcedInvalidateLayout()
{
  CheckValid();
  bool broadcast = !mNeedLayout;
  SilentInvalidateLayout();
  
  if (mpParent && broadcast)
  {
    //NGL_OUT(_T("InvalidateLayout + Broadcast from %ls\n"), GetObjectClass().GetChars());
    mpParent->BroadcastInvalidateLayout(this, false);
  }
  DebugRefreshInfo();
}

void nuiWidget::BroadcastInvalidateLayout(nuiWidgetPtr pSender, bool BroadCastOnly)
{
  CheckValid();
  if (!mNeedSelfLayout && HasUserSize()) // A child can't change the ideal position of its parent so we can stop broadcasting if the parent has a fixed ideal size.
  {
    UpdateLayout();
    return;
  }
  
  mNeedRender = true;
  if (mpSurface)
    mNeedSelfRedraw = true;

  if (!BroadCastOnly)
  {
    mNeedSelfLayout = true;
    mNeedIdealRect = true;
  }

  if (mpParent)
  {
    mpParent->BroadcastInvalidateLayout(pSender, BroadCastOnly);
    //NGL_OUT(_T("nuiWidget::BroadcastInvalidateLayout %ls / %ls / 0x%x\n"), pSender->GetObjectClass().GetChars(), pSender->GetObjectName().GetChars(), pSender);
  }

#ifdef DEBUG
  if (GetDebug() || pSender->GetDebug())
  {
    //NGL_OUT(_T("nuiWidget::BroadcastInvalidateLayout SKIP %ls / %ls / 0x%x\n"), pSender->GetObjectClass().GetChars(), pSender->GetObjectName().GetChars(), pSender);
  }
#endif

  mNeedLayout = true;

  DebugRefreshInfo();
}

bool nuiWidget::Draw(nuiDrawContext* pContext)
{
  CheckValid();
  return true;
}

bool nuiWidget::InternalDrawWidget(nuiDrawContext* pContext, const nuiRect& _self, const nuiRect& _self_and_decorations, bool ApplyMatrix)
{
  CheckValid();
  pContext->PushState();
  pContext->ResetState();
  if (ApplyMatrix && !IsMatrixIdentity())
    pContext->MultMatrix(GetMatrix());
  
  if (mAutoClip)
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
    mpDecoration->ClientToGlobalRect(sizerect, this);
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
  if (mAutoClip)
  {
    pContext->PushClipping();
    pContext->Clip(_self);
  }
  
  pContext->PushState();
  Draw(pContext);
  pContext->PopState();
  
  if (mAutoClip)
    pContext->PopClipping();
  
  ////////////////////// Draw the Overlay
  if (mpDecoration)
  {
    pContext->PushState();
    nuiRect sizerect(GetRect().Size());
    mpDecoration->ClientToGlobalRect(sizerect, this);
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
  
  if (mAutoClip)
    pContext->PopClipping();
  
  pContext->PopState();
  
  return true;
}

#if 0

2 options alter the rendering:
- RenderCache on/off
- Surface on/off

4 cases:

  None:
    InternalDrawWidget(pContext); // Draw!

  RenderCache:
    if (mNeedRender)
      InternalDrawWidget(pRenderCache); // Fill the cache
    DrawRenderCache(pContext); // Draw the cache to the screen

  Surface:
    if (mNeedRender) // need to update the surface contents
      InternalDrawWidget(pSurface);
    DrawSurface(pContext);

  Surface + RenderCache:
    if (mNeedRender)
    {
      InternalDrawWidget(pRenderCache);
      DrawRenderCache(pSurface);
    }
    DrawSurface(pContext);
    
 
#endif



bool nuiWidget::DrawWidget(nuiDrawContext* pContext)
{
  CheckValid();
  if (!IsVisible())
    return false;

  //NGL_ASSERT(!mNeedLayout);
  //if (mNeedLayout)
  // printf("need layout bug on 0x%X [%ls - %ls]\n", this, GetObjectClass().GetChars(), GetObjectName().GetChars());

  if (mSurfaceEnabled)
  {
    bool drawingincache = mpParent ? mpParent->IsDrawingInCache(true) : false;
    
    nuiRect clip;
    pContext->GetClipRect(clip, true);
    nuiRect _self = GetOverDrawRect(true, false);
    nuiRect _self_and_decorations = GetOverDrawRect(true, true);
    nuiRect inter;
    
    bool rendertest = mNeedRender;
    if (mNeedRender)
    {
      if (mNeedSelfRedraw)
      {
        UpdateSurface(_self_and_decorations.Size());

        NGL_ASSERT(mpSurface);
        
        mNeedRender = false;
        
        if (mDirtyRects.empty())
        {
          mDirtyRects.push_back(nuiRect(mpSurface->GetWidth(), mpSurface->GetHeight()));
        }

        int count = mDirtyRects.size();
        
        //printf("drawing %d partial rects\n", count);
        
        for (int i = 0; i < count; i++)
        {
          //printf("\t%d: %ls\n", i, mDirtyRects[i].GetValue().GetChars());
          mpSurface->ResetState();
          mpSurface->ResetClipRect();
          mpSurface->SetStrokeColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
          mpSurface->SetFillColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
          mpSurface->LoadMatrix(nglMatrixf());
          mpSurface->Translate(_self_and_decorations.Left(), _self_and_decorations.Top());
          mpSurface->Set2DProjectionMatrix(nuiRect(mpSurface->GetWidth(), mpSurface->GetHeight()));
          mpSurface->Clip(mDirtyRects[i]);
          mpSurface->EnableClipping(true);
          
          // clear the surface with transparent black:
          mpSurface->PushState();
          mpSurface->SetClearColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
          mpSurface->Clear();  
          mpSurface->PopState();
          
          InternalDrawWidget(mpSurface, _self, _self_and_decorations, true);
        
        }

        mpSurface->SetDirty(true);
        mDirtyRects.clear();
        mNeedSelfRedraw = false;
      }
    }
    
    //nuiMatrix m = pContext->GetMatrix();
    //nglString d;
    //m.GetValue(d);
    //    NGL_OUT(_T("nglWidget(0x%p):\n%ls\n"), this, d.GetChars());
    //NGL_ASSERT(m.Array[12] > 0);
    
    mNeedSurfaceRedraw = false;
    if (!drawingincache)
      DrawSurface(pContext);
    
    DebugRefreshInfo();
  }
  else
  {
    bool drawingincache = mpParent ? mpParent->IsDrawingInCache(true) : false;
    
    nuiRect clip;
    pContext->GetClipRect(clip, true);
    nuiRect _self = GetOverDrawRect(true, false);
    nuiRect _self_and_decorations = GetOverDrawRect(true, true);
    nuiRect inter;
    
    _self.Intersect(_self, mVisibleRect);
    _self_and_decorations.Intersect(_self_and_decorations, mVisibleRect);
    if (!inter.Intersect(_self_and_decorations, clip)) // Only render at the last needed moment. As we are currently offscreen or clipped entirely we will redraw another day.
      return false;
    
    nuiDrawContext* pSavedCtx = pContext;
    
    bool rendertest = mNeedRender;
    if (gGlobalUseRenderCache && mUseRenderCache)
    {
      NGL_ASSERT(mpRenderCache);
      
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
        mNeedRender = false;
        if (!IsMatrixIdentity())
        {
          pContext->PushMatrix();
          pContext->MultMatrix(GetMatrix());
        }
        
        mpRenderCache->ReDraw(pContext);
        
        if (!IsMatrixIdentity())
          pContext->PopMatrix();
      }
      
    }
    else
    {
      if (!drawingincache && !pContext->GetPainter()->GetDummyMode())
      {
        mNeedRender = false;
        
        InternalDrawWidget(pContext, _self, _self_and_decorations, true);
        mNeedSelfRedraw = false;
      }
    }
    
    pContext = pSavedCtx;
    
    DebugRefreshInfo();
  }

  return true;
}

void nuiWidget::DrawSurface(nuiDrawContext* pContext)
{
  CheckValid();
//  NGL_OUT(_T("nuiWidget::DrawSurface %d x %d\n"), (uint32)mpSurface->GetWidth(), (uint32)mpSurface->GetHeight());
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

  pContext->SetBlendFunc(mSurfaceBlendFunc);
  
  pContext->MultMatrix(mSurfaceMatrix);

  nuiRect src, dst;
  src = GetOverDrawRect(true, true);
  dst = src;
  pContext->DrawImage(dst, src);
  pContext->PopState();
  pContext->PopMatrix();
  pContext->PopProjectionMatrix();
}

bool nuiWidget::IsKeyDown (nglKeyCode Key) const
{
  CheckValid();
  nuiTopLevelPtr pRoot = GetTopLevel();

  return pRoot ? pRoot->IsKeyDown(Key) : false;
}

bool nuiWidget::DispatchTextInput(const nglString& rUnicodeText)
{
  CheckValid();
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

void nuiWidget::DispatchTextInputCancelled()
{
  CheckValid();
  TextInputCancelled();
  if (mpParent)
    mpParent->DispatchTextInputCancelled();
}


bool nuiWidget::DispatchKeyDown(const nglKeyEvent& rEvent, nuiKeyModifier Mask)
{
  CheckValid();
  Mask &= mHotKeyMask;
  if (TriggerHotKeys(rEvent, true, true, Mask))
  {
    return true;
  }
  
  if (KeyDown(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, true, false, Mask))
  {
    return true;
  }
  
  if (mpParent)
  {
    return mpParent->DispatchKeyDown(rEvent, Mask);
  }
  
  return false;
}

bool nuiWidget::DispatchKeyUp(const nglKeyEvent& rEvent, nuiKeyModifier Mask)
{
  CheckValid();
  Mask &= mHotKeyMask;
  if (TriggerHotKeys(rEvent, false, true, Mask))
  {
    return true;
  }
  
  if (KeyUp(rEvent))
  {
    return true;
  }
  
  if (TriggerHotKeys(rEvent, false, false, Mask))
  {
    return true;
  }
  
  if (mpParent)
  {
    return mpParent->DispatchKeyUp(rEvent, Mask);
  }
  
  return false;
}

// Event callbacks:
bool nuiWidget::TextInput(const nglString& rUnicodeText)
{
  CheckValid();
  return false;
}

void nuiWidget::TextInputCancelled()
{
  CheckValid();
  UnFocus();
}

bool nuiWidget::KeyDown(const nglKeyEvent& rEvent)
{
  CheckValid();
  return false;
}

bool nuiWidget::KeyUp(const nglKeyEvent& rEvent)
{
  CheckValid();
  return false;
}

bool nuiWidget::TriggerHotKeys(const nglKeyEvent& rEvent, bool KeyDown,  bool Priority, nuiKeyModifier Mask)
{
  CheckValid();
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

  if (!Modifiers)
    Modifiers = nuiNoKey;
  
  if (Modifiers != (Modifiers & Mask))
    return false;
  
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

void nuiWidget::SetHotKeyMask(nuiKeyModifier Mask)
{
  CheckValid();
  mHotKeyMask = Mask; 
}

nuiKeyModifier nuiWidget::GetHotKeyMask() const
{
  CheckValid();
  return mHotKeyMask;
}


void nuiWidget::EnableMouseEvent(bool enable)
{
  CheckValid();
  mMouseEventEnabled = enable;
}

bool nuiWidget::MouseEventsEnabled() const
{
  CheckValid();
  return mMouseEventEnabled;
}

bool nuiWidget::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  CheckValid();
  return false;
}

bool nuiWidget::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  CheckValid();
  return false;
}

bool nuiWidget::MouseMoved(nuiSize X, nuiSize Y)
{
  CheckValid();
  return false;
}

// Multitouch events:
bool nuiWidget::MouseClicked(const nglMouseInfo& rInfo)
{
  CheckValid();
  return MouseClicked(rInfo.X, rInfo.Y, rInfo.Buttons);
}

bool nuiWidget::MouseUnclicked(const nglMouseInfo& rInfo)
{
  CheckValid();
  return MouseUnclicked(rInfo.X, rInfo.Y, rInfo.Buttons);
}

bool nuiWidget::MouseMoved(const nglMouseInfo& rInfo)
{
  CheckValid();
  return MouseMoved(rInfo.X, rInfo.Y);
}

bool nuiWidget::MouseGrabbed(nglTouchId id)
{
  CheckValid();
  return MouseGrabbed();
}

bool nuiWidget::MouseUngrabbed(nglTouchId id)
{
  CheckValid();
  return MouseUngrabbed();
}


////// Private event management:
bool nuiWidget::DispatchMouseClick(const nglMouseInfo& rInfo)
{
  CheckValid();
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab(rInfo.TouchId);
  if (IsDisabled() && !hasgrab)
    return false;

  float X = rInfo.X;
  float Y = rInfo.Y;

  if (IsInsideFromRoot(X,Y) || hasgrab)
  {
    GlobalToLocal(X,Y);
    nglMouseInfo info(rInfo);
    info.X = X;
    info.Y = Y;

    bool res = PreClicked(info);
    if (!res)
    {
      res = MouseClicked(info);
      res |= Clicked(info);
    }

    return res | (!mClickThru);
  }
  return false;
}

bool nuiWidget::DispatchMouseUnclick(const nglMouseInfo& rInfo)
{
  CheckValid();
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab(rInfo.TouchId);
  if (IsDisabled() && !hasgrab)
    return false;

  float X = rInfo.X;
  float Y = rInfo.Y;

  if (IsInsideFromRoot(X,Y) || hasgrab)
  {
    GlobalToLocal(X,Y);
    nglMouseInfo info(rInfo);
    info.X = X;
    info.Y = Y;
    
    bool res = PreUnclicked(info);
    if (!res)
    {
      res = MouseUnclicked(info);
      res |= Unclicked(info);
    }

    return res | (!mClickThru);
  }
  return false;
}

nuiWidgetPtr nuiWidget::DispatchMouseMove(const nglMouseInfo& rInfo)
{
  CheckValid();
  if (!mMouseEventEnabled || mTrashed)
    return NULL;

  bool inside = false;
  bool res = false;
  bool hasgrab = HasGrab(rInfo.TouchId);
  float X = rInfo.X;
  float Y = rInfo.Y;

  if (IsDisabled() && !hasgrab)
    return NULL;
    
  if (IsInsideFromRoot(X, Y))
  {
    inside = true;
  }

  GlobalToLocal(X, Y);
  nglMouseInfo info(rInfo);
  info.X = X;
  info.Y = Y;
  
  if (PreMouseMoved(info))
    return this;
  res = MouseMoved(info);
  res |= MovedMouse(info) | (!mClickThru);
  return (res && inside) ? this : NULL;
}


bool nuiWidget::DispatchGrab(nuiWidgetPtr pWidget)
{
  CheckValid();
  if (mpParent)
    return mpParent->DispatchGrab(pWidget);
  return false;
}

bool nuiWidget::DispatchUngrab(nuiWidgetPtr pWidget)
{
  CheckValid();
  if (mpParent)
    return mpParent->DispatchUngrab(pWidget);
  return false;
}

bool nuiWidget::DispatchHasGrab(nuiWidgetPtr pWidget)
{
  CheckValid();
  if (mpParent)
    return mpParent->DispatchHasGrab(pWidget);
  return false;  
}

bool nuiWidget::DispatchHasGrab(nuiWidgetPtr pWidget, nglTouchId TouchId)
{
  CheckValid();
  if (mpParent)
    return mpParent->DispatchHasGrab(pWidget, TouchId);
  return false;  
}

bool nuiWidget::HasGrab()
{
  CheckValid();
  return DispatchHasGrab(this);
}
bool nuiWidget::HasGrab(nglTouchId TouchId)
{
  CheckValid();
  return DispatchHasGrab(this, TouchId);
}

bool nuiWidget::Grab()
{
  CheckValid();
  if (!DispatchGrab(this))
  {
    ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
    return false;
  }
  return true;
}

bool nuiWidget::Ungrab()
{
  CheckValid();
  if (!DispatchUngrab(this))
  {
    ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
    return false;
  }  
  return true;
}


bool nuiWidget::GetWantKeyboardFocus() const
{
  CheckValid();
  return mWantKeyboardFocus;
}

void nuiWidget::SetWantKeyboardFocus(bool Set)
{
  CheckValid();
  mWantKeyboardFocus = Set;
}

bool nuiWidget::GetMuteKeyboardFocusDispatch() const
{
  CheckValid();
  return mMuteKeyboardFocusDispatch;
}

void nuiWidget::SetMuteKeyboardFocusDispatch(bool Set)
{
  CheckValid();
  mMuteKeyboardFocusDispatch = Set;
}

bool nuiWidget::Focus()
{
  CheckValid();
  if (HasFocus())
    return true;

  mHasFocus = true;
  Invalidate();

  nuiTopLevelPtr pRoot = GetTopLevel();

  if (pRoot)
    return pRoot->SetFocus(this);

  return false;
}

bool nuiWidget::UnFocus()
{
  CheckValid();
  if (!HasFocus())
    return false;

  mHasFocus = false;
  nuiTopLevelPtr pRoot = GetTopLevel();

  Invalidate();
  if (pRoot)
    return pRoot->SetFocus(NULL);

  return false;
}


bool nuiWidget::MouseGrabbed()
{
  CheckValid();
  return false;
}

bool nuiWidget::MouseUngrabbed()
{
  CheckValid();
  return false;
}

bool nuiWidget::IsParentActive() const
{
  CheckValid();
  if (mpParent)
    return mpParent->IsParentActive();
  return false;
}

float nuiWidget::GetMixedAlpha() const
{
  CheckValid();
  if (mpParent)
    return mpParent->GetMixedAlpha() * mAlpha; // No transparency by default
  return mAlpha; // No transparency by default
}

float nuiWidget::GetAlpha() const
{
  CheckValid();
  return mAlpha; // No transparency by default
}

void nuiWidget::SetAlpha(float Alpha)
{
  CheckValid();
  mAlpha = nuiClamp(Alpha, 0.0f, 1.0f);
  Invalidate();
  DebugRefreshInfo();
}

void nuiWidget::CallOnTrash()
{
  CheckValid();
  mTrashed = true;

  nuiTopLevel* pRoot = GetTopLevel();
  if (pRoot)
  {
    //NGL_OUT(_T("nuiWidget OnTrash [0x%x '%ls']\n"), this, GetObjectClass().GetChars());
    pRoot->AdviseObjectDeath(this);
  }
  else
  {
    //NGL_OUT(_T("nuiWidget OnTrash NO ROOT! [0x%x '%ls']\n"), this, GetObjectClass().GetChars());
  }
  
  OnTrash();
}

void nuiWidget::OnTrash()
{
  CheckValid();
}

bool nuiWidget::Trash()
{
  CheckValid();

  if (!mTrashed)
    CallOnTrash();

  nuiAnimation* pAnim = GetAnimation(_T("TRASH"));
  if (pAnim && (pAnim->GetTime()==0 && pAnim->GetDuration()>0))
  {
    StartAnimation(_T("TRASH"));
  }
  else
  {
    NGL_ASSERT(mTrashed);
    NGL_ASSERT(!mDoneTrashed);
    if (mDoneTrashed)
      return false;

    mDoneTrashed = true;

    nuiTopLevel* pRoot = GetTopLevel();

    if (!pRoot)
    {
      if (mpParent)
        mpParent->DelChild(this);
      return true;
    }
		
    while (!mHotKeyEvents.empty())
    {
      DelHotKey(mHotKeyEvents.begin()->first);
    }
    
    pRoot->Trash(this);
    Trashed();
    Invalidate();

    NGL_ASSERT(!HasGrab()); /// done with CallOnTrash
//    pRoot->AdviseObjectDeath(this); /// done with CallOnTrash

    DebugRefreshInfo();
  }

  return true;
}

bool nuiWidget::HasInheritedProperty (const nglString& rName)
{
  CheckValid();
  if (HasProperty(rName))
    return true;
  if (mpParent)
    return mpParent->HasInheritedProperty(rName);
  return false;
}

nglString nuiWidget::GetInheritedProperty (const nglString& rName)
{
  CheckValid();
  if (HasProperty(rName))
    return GetProperty(rName);
  if (mpParent)
    return mpParent->GetInheritedProperty(rName);
  return nglString::Null;
}


void nuiWidget::OnSetFocus(nuiWidgetPtr pWidget)
{
  CheckValid();
}

void nuiWidget::DispatchFocus(nuiWidgetPtr pWidget)
{
  CheckValid();
  mHasFocus = (pWidget == this);
  OnSetFocus(pWidget);
  FocusChanged();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHTAG_STATE);
  DebugRefreshInfo();
}

bool nuiWidget::HasFocus() const
{
  CheckValid();
  return mHasFocus;
}

nuiDrawContext* nuiWidget::GetDrawContext()
{
  CheckValid();
  nuiContainerPtr pRoot=GetRoot();
  if (pRoot && pRoot!=this)
    return pRoot->GetDrawContext();
  else
    return NULL;
}

bool nuiWidget::HasLayoutChanged() const
{
  CheckValid();
  return mNeedSelfLayout;
}

bool nuiWidget::GetHover() const
{
  CheckValid();
  return mHover;
}

void nuiWidget::SetHover(bool Hover)
{
  CheckValid();
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
  if (mRedrawOnHover || (mpDecoration && mpDecoration->GetRedrawOnHover()))
    Invalidate();
  DebugRefreshInfo();
}

bool nuiWidget::IsEnabled(bool combined) const
{
  CheckValid();
  if (!combined || !mpParent)
    return mEnabled;
  if (mEnabled && !mStateLocked) 
    return mpParent->IsEnabled(true);
  return mEnabled;
}


bool nuiWidget::AttrIsSelected() const
{
  CheckValid();
  return IsSelected();
}


bool nuiWidget::IsSelected(bool combined) const
{
  CheckValid();
  if (!combined || !mpParent)
    return mSelected;
  if (!mSelected && !mStateLocked) 
    return mpParent->IsSelected(true);
  return mSelected;
}

void nuiWidget::SetEnabled(bool set)
{
  CheckValid();
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
  CheckValid();
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
  CheckValid();
  if (mPosition == mode)
    return;
  
  mPosition = mode;
  InvalidateLayout();
}

nuiPosition nuiWidget::GetPosition() const
{
  CheckValid();
  return mPosition;
}

void nuiWidget::SetFillRule(nuiPosition mode)
{
  CheckValid();
  if (mFillRule == mode)
    return;
  mFillRule = mode;

  if (mPosition == nuiFill)
    InvalidateLayout();
}

nuiPosition nuiWidget::GetFillRule() const
{
  CheckValid();
  return mFillRule;
}


void nuiWidget::LockState()
{
  CheckValid();
  mStateLocked = true;
  DebugRefreshInfo();
}

void nuiWidget::UnlockState()
{
  CheckValid();
  mStateLocked = false;
  DebugRefreshInfo();
}


void nuiWidget::SetVisible(bool Visible)
{
  CheckValid();
  
  nuiAnimation* pHideAnim = GetAnimation(_T("HIDE"));
  nuiAnimation* pShowAnim = GetAnimation(_T("SHOW"));

  if (mVisible == Visible)
  {
    if (Visible && (!pHideAnim || !pHideAnim->IsPlaying()))
      return;
    else if (!Visible && (!pShowAnim || !pShowAnim->IsPlaying()))
      return;
    if (pHideAnim)
      pHideAnim->Stop();
    if (pShowAnim)
      pShowAnim->Stop();
  }

  if (pHideAnim && !Visible && (pHideAnim->GetPosition()==0 && pHideAnim->GetDuration()>0))
  {
    StartAnimation(_T("HIDE"));
  }
  else
  {
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
    if (Visible)
      StartAnimation(_T("SHOW"));
  }
}

void nuiWidget::SilentSetVisible(bool Visible)
{
  CheckValid();
  if (mVisible == Visible)
    return;
  
  mVisible = Visible;
}


bool nuiWidget::IsVisible(bool combined) const
{
  CheckValid();
  if (!combined || !mpParent)
    return mVisible;
  if (mVisible) 
    return mpParent->IsVisible(true);
  return mVisible;
}

void nuiWidget::StartTransition()
{
  CheckValid();
  mInTransition++;
}

void nuiWidget::StopTransition()
{
  CheckValid();
  mInTransition--;
}

bool nuiWidget::IsInTransition() const
{
  CheckValid();
  return mInTransition != 0;
}


// virtual
bool nuiWidget::SetMouseCursor(nuiMouseCursor Cursor)
{
  CheckValid();
  mObjCursor = Cursor;
  DebugRefreshInfo();
  return true;
}

void nuiWidget::SetAttrMouseCursor(nuiMouseCursor Cursor)
{
  CheckValid();
  mObjCursor = Cursor;
  DebugRefreshInfo();
}



nuiMouseCursor nuiWidget::GetMouseCursor() const
{
  CheckValid();
  return mObjCursor;
}



bool nuiWidget::IsInsideFromRoot(nuiSize X, nuiSize Y, nuiSize GrowOffset)
{
  CheckValid();
  if (!IsVisible(false))
    return false;

  GlobalToLocal(X, Y);
  return IsInsideFromSelf(X,Y, GrowOffset);
}

bool nuiWidget::IsInsideFromParent(nuiSize X, nuiSize Y, nuiSize GrowOffset)
{
  CheckValid();
  if (!IsVisible(false))
    return false;
  return IsInsideFromSelf(X - mRect.Left(), Y - mRect.Top(), GrowOffset);
}

bool nuiWidget::IsInsideFromSelf(nuiSize X, nuiSize Y, nuiSize GrowOffset)
{
  CheckValid();
  if (!IsVisible(false))
    return false;
  
  if (mInteractiveDecoration)
  {
    nuiRect r = mVisibleRect;
    r.Intersect(mVisibleRect, GetOverDrawRect(true, true));
    r.Grow(GrowOffset, GrowOffset);
    return r.IsInside(X, Y);
  }
  if (mInteractiveOD)
  {
    nuiRect r = mVisibleRect;
    r.Intersect(r, GetOverDrawRect(true, false));
    r.Grow(GrowOffset, GrowOffset);
    return r.IsInside(X, Y);
  }

  nuiRect r(GetRect().Size());
  r.Grow(GrowOffset, GrowOffset);
  return r.IsInside(X,Y);
}


bool nuiWidget::SetToolTip(const nglString& rToolTip)
{
  CheckValid();
//  if (GetDebug())
//  {
//    NGL_OUT(_T("nuiWidget::SetToolTip for 0x%x %ls / %ls to %ls\n"), this, GetObjectClass().GetChars(), GetObjectName().GetChars(), rToolTip.GetChars());
//  }
  SetProperty(_T("ToolTip"), rToolTip);
  return true;
}

nglString nuiWidget::GetToolTip() const
{
  return GetProperty(_T("ToolTip"));
}

bool nuiWidget::ActivateToolTip(nuiWidgetPtr pWidget, bool Now)
{
  CheckValid();
//  if (GetDebug())
//  {
//    NGL_OUT(_T("nuiWidget::ActivateToolTip for 0x%x %ls / %ls\n"), this, GetObjectClass().GetChars(), GetObjectName().GetChars());
//  }
  
  nglString tt = GetToolTip();

  if (tt.IsEmpty())
  {
    return false;
  }

  nuiTopLevelPtr pRoot = GetTopLevel();
  NGL_ASSERT(pRoot);

  if (pRoot)
  {
    bool res = pRoot->ActivateToolTip(pWidget, Now);
    NGL_ASSERT(res);
    return res;
  }

  return false;
}

bool nuiWidget::ReleaseToolTip(nuiWidgetPtr pWidget)
{
  CheckValid();
  nuiTopLevelPtr pRoot = GetTopLevel();
  if (pRoot)
    return pRoot->ReleaseToolTip(pWidget);
  return false;
}


/// Drag
bool nuiWidget::Drag(nglDragAndDrop* pDragObj)
{
  CheckValid();
  nuiMainWindow* pWin = (nuiMainWindow*)GetTopLevel();
  return pWin->Drag(this, pDragObj);
}




void nuiWidget::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
  CheckValid();
  // Do nothing, drag and drop is not supported by default.
}

void nuiWidget::OnDragStop(bool canceled)
{
  CheckValid();
  // Do nothing, drag and drop is not supported by default.
}


/// Drop

nglDropEffect nuiWidget::OnCanDrop(nglDragAndDrop* pDragObject,nuiSize X,nuiSize Y)
{
  CheckValid();
  return eDropEffectNone; // Or decline the DnD operation by default.
}

void nuiWidget::OnDropped(nglDragAndDrop* pDragObject,nuiSize X,nuiSize Y, nglMouseInfo::Flags Button)
{
  CheckValid();
  // Do nothing, drag and drop is not supported by default.
}

void nuiWidget::OnDropLeave()
{
  CheckValid();
  // Do nothing, drag and drop is not supported by default.
}

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
  CheckValid();
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

    if (mMaxIdealWidth > 0.0f)
      mIdealRect.Right() = MIN(mMaxIdealWidth, mIdealRect.GetWidth());
    if (mMaxIdealHeight > 0.0f)
      mIdealRect.Bottom() = MIN(mMaxIdealHeight, mIdealRect.GetHeight());
    if (mMinIdealWidth > 0.0f)
      mIdealRect.Right() = MAX(mMinIdealWidth, mIdealRect.GetWidth());
    if (mMinIdealHeight > 0.0f)
      mIdealRect.Bottom() = MAX(mMinIdealHeight, mIdealRect.GetHeight());

    if (mHasUserPos && !mHasUserSize)
      mIdealRect.MoveTo(mUserRect.mLeft, mUserRect.mTop);
    

    mIdealRect.Bottom() += GetActualBorderBottom();
    mIdealRect.Top() -= GetActualBorderTop();
    mIdealRect.Left() -= GetActualBorderLeft();
    mIdealRect.Right() += GetActualBorderRight();
    if (!HasUserPos())
      mIdealRect.MoveTo(0, 0);
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
  CheckValid();
  #ifdef _DEBUG_LAYOUT
  if (GetDebug())
    NGL_OUT(_T("nuiWidget::SetRect on '%ls' (%f, %f - %f, %f)\n"), GetObjectClass().GetChars(), rRect.mLeft, rRect.mTop, rRect.GetWidth(), rRect.GetHeight());
  #endif
  
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

  DebugRefreshInfo();
  return true;
}

void nuiWidget::SetBorders(nuiSize XY)
{
  CheckValid();
  SetBorder(XY, XY);
}

void nuiWidget::SetBorder(nuiSize X, nuiSize Y)
{
  CheckValid();
  if (mBorderLeft == X && mBorderRight == X && mBorderTop == Y && mBorderBottom == Y)
    return;
  
  mBorderLeft = mBorderRight = X;
  mBorderTop = mBorderBottom = Y;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderLeft(nuiSize border)
{
  CheckValid();
  if (mBorderLeft == border)
    return;
  
  mBorderLeft = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderTop(nuiSize border)
{
  CheckValid();
  if (mBorderTop == border)
    return;
  
  mBorderTop = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderRight(nuiSize border)
{
  CheckValid();
  if (mBorderRight == border)
    return;
    
  mBorderRight = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetBorderBottom(nuiSize border)
{
  CheckValid();
  if (mBorderBottom == border)
    return;
  
  mBorderBottom = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

nuiSize nuiWidget::GetBorderLeft() const
{
  CheckValid();
  return mBorderLeft;
}

nuiSize nuiWidget::GetBorderTop() const
{
  CheckValid();
  return mBorderTop;
}

nuiSize nuiWidget::GetBorderRight() const
{
  CheckValid();
  return mBorderRight;
}

nuiSize nuiWidget::GetBorderBottom() const
{
  CheckValid();
  return mBorderBottom;
}

nuiSize nuiWidget::GetActualBorderLeft() const
{
  CheckValid();
  nuiSize Left = mBorderLeft;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Left = MAX(Left, mpDecoration->GetBorder(nuiLeft, this));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Left = MAX(Left, mpFocusDecoration->GetBorder(nuiLeft, this));
  return Left;
}

nuiSize nuiWidget::GetActualBorderTop() const
{
  CheckValid();
  nuiSize Top = mBorderTop;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Top = MAX(Top, mpDecoration->GetBorder(nuiTop, this));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Top = MAX(Top, mpFocusDecoration->GetBorder(nuiTop, this));
  return Top;
}

nuiSize nuiWidget::GetActualBorderRight() const
{
  CheckValid();
  nuiSize Right = mBorderRight;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Right = MAX(Right, mpDecoration->GetBorder(nuiRight, this));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Right = MAX(Right, mpFocusDecoration->GetBorder(nuiRight, this));
  return Right;
}

nuiSize nuiWidget::GetActualBorderBottom() const
{
  CheckValid();
  nuiSize Bottom = mBorderBottom;
  if (mpDecoration && mDecorationMode == eDecorationBorder)
    Bottom = MAX(Bottom, mpDecoration->GetBorder(nuiBottom, this));
  if (mpFocusDecoration && mFocusDecorationMode == eDecorationBorder)
    Bottom = MAX(Bottom, mpFocusDecoration->GetBorder(nuiBottom, this));
  return Bottom;
}


void nuiWidget::SetBorder(nuiSize Left, nuiSize Right, nuiSize Top, nuiSize Bottom)
{
  CheckValid();
  if (mBorderLeft == Left && mBorderRight == Right && mBorderTop == Top && mBorderBottom == Bottom)
    return;

  mBorderLeft = Left;
  mBorderRight = Right;
  mBorderTop = Top;
  mBorderBottom = Bottom;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::GetBorder(nuiSize& rLeft, nuiSize& rRight, nuiSize& rTop, nuiSize& rBottom)
{
  CheckValid();
  rLeft = mBorderLeft;
  rRight = mBorderRight;
  rTop = mBorderTop;
  rBottom = mBorderBottom;
}

void nuiWidget::SetVisibleRect(const nuiRect& rRect)
{
  CheckValid();
  if (mVisibleRect == rRect)
    return;
  
  mVisibleRect = rRect;
  Invalidate();
}

void nuiWidget::SilentSetVisibleRect(const nuiRect& rRect)
{
  CheckValid();
  if (mVisibleRect == rRect)
    return;
  
  mVisibleRect = rRect;
  SilentInvalidate();
}

const nuiRect& nuiWidget::GetVisibleRect() const
{
  CheckValid();
  return mVisibleRect;
}

static nglString GetSurfaceName(nuiWidget* pWidget)
{
  static uint32 gSurfaceCount = 0;
  nglString str;
  str.CFormat(_T("'%ls'/'%ls' %x %d"), pWidget->GetObjectClass().GetChars(), pWidget->GetObjectName().GetChars(), pWidget, gSurfaceCount++);
  return str;
}

void nuiWidget::UpdateSurface(const nuiRect& rRect)
{
  CheckValid();
  if (mSurfaceEnabled)
  {
    if (!mpSurface || (mpSurface->GetWidth() != rRect.GetWidth() || mpSurface->GetHeight() != rRect.GetHeight()))
    {
      if (mpSurface)
      {
        mpSurface->Release();
      }
      mpSurface = NULL;

//      if (!(rRect.GetWidth() > 0 && rRect.GetHeight() > 0))
//        return;

      nglString str(GetSurfaceName(this));
      mpSurface = nuiSurface::CreateSurface(str, ToAbove(rRect.GetWidth()), ToAbove(rRect.GetHeight()), eImagePixelRGBA);
      mDirtyRects.clear();
      mDirtyRects.push_back(nuiRect(mpSurface->GetWidth(), mpSurface->GetHeight()));
    }
  }
  else
  {
    if (mpSurface)
    {
      mpSurface->Release();
    }
    mpSurface = NULL;
    mDirtyRects.clear();
  }
}

void nuiWidget::SetFixedAspectRatio(bool set)
{
  mFixedAspectRatio = set;
  InvalidateLayout();
}

bool nuiWidget::GetFixedAspectRatio() const
{
  return mFixedAspectRatio;
}

nuiRect nuiWidget::GetLayoutForRect(const nuiRect& rRect)
{
  CheckValid();
  nuiRect rect(GetIdealRect().Size());
  nuiRect r(rRect);

  if (mMaxWidth >= 0)
    rect.SetWidth(MIN(r.GetWidth(), mMaxWidth));

  if (mMaxHeight >= 0)
    rect.SetHeight(MIN(r.GetHeight(), mMaxHeight));

  if (mMinWidth >= 0)
    rect.SetWidth(MAX(r.GetWidth(), mMinWidth));

  if (mMinHeight >= 0)
    rect.SetHeight(MAX(r.GetHeight(), mMinHeight));

  // Prevent the widget from being bigger than the size provided by its parent:
  float w = r.GetWidth();
  float ww = rect.GetWidth();
  float h = r.GetHeight();
  float hh = rect.GetHeight();
  
  if (ww > w)
  {
    ww = w;
  }
  if (hh > h)
  {
    hh = h;
  }

  if (GetFixedAspectRatio())
  {
    // Give good ratio to keep things in proportions
    float tw = mIdealRect.GetWidth();
    float th = mIdealRect.GetHeight();
    float r = 1.0f;
    
    if (hh < th)
    {
      r = hh / th;
      ww = tw * r;
    }
    
    if (ww < tw)
    {
      r = ww / tw;
      hh = th * r;
    }
  }
  
  rect.SetWidth(ww);
  rect.SetHeight(hh);
  
  if (mPosition == nuiNoPosition)
  {
    rect.Move(r.Left(), r.Top());
  }
  else if (mPosition != nuiFill)
  {
    rect.SetPosition(mPosition, r);
  }
  else
  {
    if (mFillRule == nuiFill)
    {
      rect.SetPosition(mPosition, r);
    }
    else if (mFillRule == nuiTile)
    {
      rect = r;
    }
    else
    {
      nuiRect rct = r;
      float ratio,rratio,rw,rh;
      ratio  = (float)rct.GetWidth()  / (float)rct.GetHeight();
      rratio = (float)rect.GetWidth() / (float)rect.GetHeight();

      if (ratio < rratio) 
      {
        rw = (float)rct.GetWidth();
        rh = rw / rratio;
      }

      else 
      {
        rh = (float)rct.GetHeight();
        rw = rratio * rh;
      }

      rect = nuiRect(0.0f, 0.0f, rw, rh);
      rect.SetPosition(mFillRule, r);
    }
  }

  rect.Left()   += GetActualBorderLeft();
  rect.Right()  -= GetActualBorderRight();
  rect.Top()    += GetActualBorderTop();
  rect.Bottom() -= GetActualBorderBottom();

  rect.RoundToNearest();

  return rect;
}

void nuiWidget::SetLayout(const nuiRect& rRect)
{
  if (IsInTransition())
    return;
  
  CheckValid();
  nuiRect rect(GetLayoutForRect(rRect));
  
  if (GetLayoutAnimationDuration() > 0)
  {
    nuiRectAttributeAnimation* pAnim = GetLayoutAnimation(true);
    if (pAnim->IsPlaying())
      pAnim->Stop();
    pAnim->SetEndValue(rect);
    nuiDelayedPlayAnim(pAnim, eAnimFromStart, 0.0, 1, eAnimLoopForward);
  }
  else
  {
    InternalSetLayout(rect);
  }
}

void nuiWidget::InternalSetLayout(const nuiRect& rect)
{
  CheckValid();
  bool PositionChanged = (rect.Left() != mRect.Left()) || (rect.Top() != mRect.Top());
  bool SizeChanged = !rect.Size().IsEqual(mRect.Size());
  mNeedSelfLayout = mNeedSelfLayout || SizeChanged;
  
  InternalSetLayout(rect, PositionChanged, SizeChanged);
  
  mVisibleRect = GetOverDrawRect(true, true);
  
  if (PositionChanged && mpParent)
    mpParent->Invalidate();
  
  mNeedSelfLayout = false;
  mNeedLayout = false;
  DebugRefreshInfo();
}

void nuiWidget::InternalSetLayout(const nuiRect& rect, bool PositionChanged, bool SizeChanged)
{
  CheckValid();
#ifdef _DEBUG_
  if (GetDebug())
  {
    //NGL_OUT(_T("InternalSetLayout\n"));
  }
#endif
  
  if (mNeedSelfLayout)
  {
    if (mInSetRect)
      return;
    mInSetRect = true;
    SetRect(rect);
    mInSetRect = false;
    Invalidate();
  }
  else
  {
    // Is this case the widget has just been moved inside its parent. No need to re layout it, only change the rect...
    mRect = rect;
  }
  
  //#TEST
#ifdef _DEBUG_
  {
    nuiSimpleContainer* pCont = dynamic_cast<nuiSimpleContainer*> (this);
    if (pCont)
    {
      for (uint32 i = 0; i < pCont->GetChildrenCount(); i++)
      {
        nuiWidget* pChild = pCont->GetChild(i);
        NGL_ASSERT(pChild);
        NGL_ASSERT(!pChild->mNeedLayout);
        NGL_ASSERT(!pChild->mNeedSelfLayout);
      }
    }
  }
#endif  
}

nuiRect nuiWidget::CalcIdealSize()
{
  CheckValid();
  // Implement widget layout politics here
  return mIdealRect;
}


void nuiWidget::SetUserWidth(nuiSize s)
{
  CheckValid();
  if (mHasUserWidth && GetUserWidth() == s)
    return;
  
  mUserRect.SetWidth(s);
  mHasUserWidth = true;
  UserRectChanged();
  ForcedInvalidateLayout();
  DebugRefreshInfo();
}

nuiSize nuiWidget::GetUserWidth()
{
  CheckValid();
  return mUserRect.GetWidth();
}

void nuiWidget::SetUserHeight(nuiSize s)
{
  CheckValid();
  if (mHasUserHeight && GetUserHeight() == s)
    return;
  
  mUserRect.SetHeight(s);
  mHasUserHeight = true;
  UserRectChanged();
  ForcedInvalidateLayout();
  DebugRefreshInfo();
}

nuiSize nuiWidget::GetUserHeight()
{
  CheckValid();
  return mUserRect.GetHeight();
}


// User size
void nuiWidget::SetUserRect(const nuiRect& rRect)
{
  CheckValid();
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
      if (!mInSetRect)
      {
        mInSetRect = true;
        SetRect(rRect);
        mInSetRect = false;
      }
      mpParent->Invalidate();
      Invalidate();
    }
    else
    {
      ForcedInvalidateLayout();
    }
    DebugRefreshInfo();
  }
}

const nuiRect& nuiWidget::GetUserRect() const
{
  CheckValid();
  return mUserRect;
}

nuiPoint nuiWidget::GetUserPos() const
{
  CheckValid();
  return nuiPoint(mUserRect.Left(), mUserRect.Top());
}

void nuiWidget::UnsetUserRect()
{
  CheckValid();
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
  CheckValid();
  if (mHasUserSize && GetUserWidth() == X && GetUserHeight() == Y)
    return;
  
  mUserRect.SetSize(X,Y);
  mHasUserSize = true;
  mHasUserWidth = true;
  mHasUserHeight = true;
  ForcedInvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::UnsetUserSize()
{
  CheckValid();
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
  CheckValid();
  if (mHasUserPos && mUserRect.Left() == X && mUserRect.Top() == Y)
    return;
  
  mUserRect.MoveTo(X,Y);
  mHasUserPos = true;
  ForcedInvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetUserPos(nuiPoint Pos)
{
  CheckValid();
  SetUserPos(Pos[0], Pos[1]);
}

void nuiWidget::UnsetUserPos()
{
  CheckValid();
  mHasUserPos = false;
  InvalidateLayout();
  DebugRefreshInfo();
}

bool nuiWidget::HasUserRect() const
{
  CheckValid();
  return mHasUserSize && mHasUserPos;
}

bool nuiWidget::HasUserSize() const
{
  CheckValid();
  return mHasUserSize;
}

bool nuiWidget::HasUserWidth() const
{
  CheckValid();
  return mHasUserWidth;
}

bool nuiWidget::HasUserHeight() const
{
  CheckValid();
  return mHasUserHeight;
}



bool nuiWidget::HasUserPos() const
{
  CheckValid();
  return mHasUserPos;
}

void nuiWidget::ForceIdealRect(bool Force)
{
  CheckValid();
  if (mForceIdealSize == Force)
    return;
  
  mForceIdealSize = Force;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::GetHotRect(nuiRect& rRect) const
{
  CheckValid();
  rRect = mHotRect;
}

const nuiRect& nuiWidget::GetHotRect() const
{
  CheckValid();
  return mHotRect;
}

void nuiWidget::SetHotRect(const nuiRect& rRect)
{
  CheckValid();
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
  CheckValid();
  mSelectionExclusive = Exclusive;
  DebugRefreshInfo();
}

bool nuiWidget::IsSelectionExclusive() const
{
  CheckValid();
  return mSelectionExclusive;
}

void nuiWidget::SetRedrawOnHover(bool RedrawOnHover)
{
  CheckValid();
  mRedrawOnHover = RedrawOnHover;
  DebugRefreshInfo();
}

bool nuiWidget::GetRedrawOnHover() const
{
  CheckValid();
  return mRedrawOnHover;
}

nuiWidgetPtr nuiWidget::Find(const nglString& rName) 
{ 
  CheckValid();
  int slash = rName.Find('/'); 

  if (slash >= 0) 
    return NULL; 
  if (rName.IsEmpty())
    return this; 
  return NULL;
} 

/* Animation Support: */
void nuiWidget::StartAutoDraw()
{
  CheckValid();
  mGenericWidgetSink.Connect(nuiAnimation::AcquireTimer()->Tick, &nuiWidget::Animate);
  DebugRefreshInfo();
}

void nuiWidget::SetAutoDrawAnimateLayout(bool RecalcLayout)
{
  CheckValid();
  mAnimateLayout = RecalcLayout;
  DebugRefreshInfo();
}

bool nuiWidget::GetAutoDrawAnimateLayout() const
{
  CheckValid();
  return mAnimateLayout;
}

void nuiWidget::StopAutoDraw()
{
  CheckValid();
  mGenericWidgetSink.DisconnectSource(nuiAnimation::GetTimer()->Tick);
  nuiAnimation::ReleaseTimer();
  DebugRefreshInfo();
}

void nuiWidget::Animate(const nuiEvent& rEvent)
{
  CheckValid();
  if (mVisible)
  {
    if (mAnimateLayout)
      InvalidateLayout();
    else
      Invalidate();
  }
}

nuiAnimation* nuiWidget::GetAnimation(const nglString& rName)
{
  CheckValid();
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.find(rName);

  if (it == mAnimations.end())
    return NULL;

  return (*it).second;
}

void nuiWidget::GetAnimations(std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>& rAnims)
{
  CheckValid();
  rAnims = mAnimations;
}

uint32 nuiWidget::GetAnimCount()
{
  CheckValid();
  return (uint32)mAnimations.size();
}

void nuiWidget::AddAnimation(const nglString& rName, nuiAnimation* pAnim, bool TransitionAnimation)
{
  CheckValid();
  nuiAnimation* pOldAnim = GetAnimation(rName);
  if (pOldAnim)
    delete pOldAnim;

  mAnimations[rName] = pAnim;
  pAnim->SetDeleteOnStop(false); /// Cancel anim delete on stop or we'll crash as soon as the widget is destroyed or the user starts to play the anim once more.
  if (rName == _T("TRASH"))
    mGenericWidgetSink.Connect(pAnim->AnimStop, &nuiWidget::AutoTrash);
  if (rName == _T("HIDE"))
    mGenericWidgetSink.Connect(pAnim->AnimStop, &nuiWidget::AutoHide);
  
  if (TransitionAnimation)
  {
    mGenericWidgetSink.Connect(pAnim->AnimStart, &nuiWidget::AutoStartTransition);
    mGenericWidgetSink.Connect(pAnim->AnimStop, &nuiWidget::AutoStopTransition);
  }
  
  DebugRefreshInfo();
}

void nuiWidget::DelAnimation(const nglString& rName)
{
  CheckValid();
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.find(rName);

  if (it == mAnimations.end())
    return;

  mAnimations.erase(it);
  DebugRefreshInfo();
}

void nuiWidget::ClearAnimations()
{
  CheckValid();
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator it = mAnimations.begin();
  std::map<nglString, nuiAnimation*, nglString::NaturalLessFunctor>::iterator end = mAnimations.end();

  for ( ; it != end; ++it)
    delete (*it).second;

  mAnimations.clear();
  DebugRefreshInfo();
}

void nuiWidget::AnimateAll(bool Set, bool Reset)
{
  CheckValid();
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
  CheckValid();
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
    pAnim->SetTime(0);
  DebugRefreshInfo();
}

void nuiWidget::StartAnimation(const nglString& rName, double Time)
{
  CheckValid();
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
  {
    nuiDelayedPlayAnim(pAnim, eAnimFromStart, Time, 1, eAnimLoopForward);
  }
  DebugRefreshInfo();
}

void nuiWidget::StartAnimation(const nglString& rName, int32 count, nuiAnimLoop loopmode , double Time)
{
  CheckValid();
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
  {
    nuiDelayedPlayAnim(pAnim, eAnimFromStart, Time, count, loopmode);
  }
  DebugRefreshInfo();
}


void nuiWidget::StopAnimation(const nglString& rName)
{
  CheckValid();
  nuiAnimation* pAnim = GetAnimation(rName);
  if (pAnim)
    pAnim->Stop();
  DebugRefreshInfo();
}

#define LAYOUT_ANIM_NAME _T("LAYOUT_ANIM")

nuiRectAttributeAnimation* nuiWidget::GetLayoutAnimation(bool CreateIfNotAvailable)
{
  CheckValid();
  if (mpLayoutAnimation)
    return mpLayoutAnimation;

  nuiRectAttributeAnimation* pAnim = (nuiRectAttributeAnimation*)GetAnimation(LAYOUT_ANIM_NAME);
  if (!pAnim && CreateIfNotAvailable)
  {
    pAnim = new nuiRectAttributeAnimation();
    pAnim->SetDuration(0);
    pAnim->SetTargetObject(this);
    pAnim->SetTargetAttribute(_T("LayoutRectUnsafe"));
    pAnim->SetCaptureStartOnPlay(true);
    AddAnimation(LAYOUT_ANIM_NAME, pAnim);
  }
  mpLayoutAnimation = pAnim;
  return pAnim;
}

void nuiWidget::SetLayoutAnimationDuration(float duration)
{
  CheckValid();
  nuiRectAttributeAnimation* pAnim = GetLayoutAnimation(duration > 0);
  if (pAnim)
    pAnim->SetDuration(duration);
}

float nuiWidget::GetLayoutAnimationDuration()
{
  CheckValid();
  nuiRectAttributeAnimation* pAnim = GetLayoutAnimation(false);
  if (pAnim)
    return pAnim->GetDuration();
  return 0;
}

void nuiWidget::SetLayoutAnimationEasing(const nuiEasingMethod& rMethod)
{
  CheckValid();
  nuiRectAttributeAnimation* pAnim = GetLayoutAnimation(true);
  if (pAnim)
    pAnim->SetEasing(rMethod);
}


/// Matrix Operations:
nuiMatrix nuiWidget::mIdentityMatrix;

void nuiWidget::AddMatrixNode(nuiMatrixNode* pNode)
{
  CheckValid();
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();

  if (!mpMatrixNodes)
    mpMatrixNodes = new std::vector<nuiMatrixNode*>;

  pNode->Acquire();
  mpMatrixNodes->push_back(pNode);
  mGenericWidgetSink.Connect(pNode->Changed, &nuiWidget::AutoInvalidateLayout);
  
  // Usual clean up needed for the partial redraw to work correctly
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();
  
  if (mpParent)
    mpParent->BroadcastInvalidate(this);
  DebugRefreshInfo();
}

void nuiWidget::DelMatrixNode(uint32 index)
{
  if (!mpMatrixNodes)
    return;
  
  CheckValid();
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();
  
  mGenericWidgetSink.Disconnect(mpMatrixNodes->at(index)->Changed, &nuiWidget::AutoInvalidateLayout);
  mpMatrixNodes->at(index)->Release();
  mpMatrixNodes->erase(mpMatrixNodes->begin() + index);
  
  // Usual clean up needed for the partial redraw to work correctly
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();
  
  if (mpParent)
    mpParent->BroadcastInvalidate(this);
  DebugRefreshInfo();
}


int32 nuiWidget::GetMatrixNodeCount() const
{
  CheckValid();
  if (!mpMatrixNodes)
    return 0;
  return mpMatrixNodes->size();
}


nuiMatrixNode* nuiWidget::GetMatrixNode(uint32 index) const
{
  CheckValid();
  if (mpMatrixNodes)
    return mpMatrixNodes->at(index);
}


void nuiWidget::LoadIdentityMatrix()
{
  CheckValid();
  Invalidate();
  
  if (mpMatrixNodes)
  {
    for (uint32 i = 0; i < mpMatrixNodes->size(); i++)
      mpMatrixNodes->at(i)->Release();
    delete mpMatrixNodes;
    mpMatrixNodes = NULL;
  }
  
  Invalidate();
  DebugRefreshInfo();
}

bool nuiWidget::IsMatrixIdentity() const
{
  return !mpMatrixNodes;
}

void nuiWidget::GetMatrix(nuiMatrix& rMatrix) const
{
  CheckValid();
  rMatrix.SetIdentity();
  if (IsMatrixIdentity())
    return;

  for (uint32 i = 0; i < mpMatrixNodes->size(); i++)
    mpMatrixNodes->at(i)->Apply(rMatrix);
}

nuiMatrix nuiWidget::GetMatrix() const
{
  CheckValid();
  nuiMatrix m;
  GetMatrix(m);
  return m;
}

const nuiMatrix& nuiWidget::_GetMatrix() const
{
  CheckValid();
  return GetMatrix();
}

void nuiWidget::SetMatrix(const nuiMatrix& rMatrix)
{
  CheckValid();
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();

  // Special case: we only need one simple static matrix node at max
  LoadIdentityMatrix(); // So we load the identity matrix (i.e. clear any existing node)
  if (!rMatrix.IsIdentity()) // If the user wasn't asking for the identity matrix
  {
    AddMatrixNode(new nuiMatrixNode(rMatrix));
  }

  // Usual clean up needed for the partial redraw to work correctly
  nuiWidget::InvalidateRect(GetOverDrawRect(true, true));
  SilentInvalidate();
  
  if (mpParent)
    mpParent->BroadcastInvalidate(this);
  DebugRefreshInfo();
}

void nuiWidget::EnableRenderCache(bool set)
{
  CheckValid();
  if (mUseRenderCache != set)
  {
    mUseRenderCache = set;
    if (mUseRenderCache)
    {
      if (!mpRenderCache)
      {
        mpRenderCache = new nuiMetaPainter(nuiRect());
#ifdef _DEBUG_
        mpRenderCache->DBGSetReferenceObject(this);
#endif
      }
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
  CheckValid();
  return mUseRenderCache;
}

const nuiMetaPainter* nuiWidget::GetRenderCache() const
{
  CheckValid();
  if (mSurfaceEnabled && mpSurface)
    return mpSurface->GetSurfacePainter();
  return mpRenderCache;
}

void nuiWidget::EnableSurface(bool Set)
{
  CheckValid();
  if (mSurfaceEnabled == Set)
    return;
  mSurfaceEnabled = Set;

  nuiRect r(GetOverDrawRect(true, true).Size());
  UpdateSurface(r);

  Invalidate();
  DebugRefreshInfo();
}

bool nuiWidget::IsSurfaceEnabled() const
{
  CheckValid();
  return mSurfaceEnabled;
}

void nuiWidget::SetColor(nuiWidgetElement element, const nuiColor& rColor)
{
  CheckValid();
  mWidgetElementColors[element] = rColor;
  Invalidate();
  DebugRefreshInfo();
}

void nuiWidget::DelColor(nuiWidgetElement element)
{
  CheckValid();
  mWidgetElementColors.erase(element);
  Invalidate();
  DebugRefreshInfo();
}

nuiColor nuiWidget::GetColor(nuiWidgetElement element)
{
  CheckValid();
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
    float alpha = GetAlpha();
    col.Multiply(alpha);
  }
  return col;
}

void nuiWidget::SetFillColor(nuiDrawContext* pContext, nuiWidgetElement Element)
{
  CheckValid();
  pContext->SetFillColor(GetColor(Element));
}

void nuiWidget::SetStrokeColor(nuiDrawContext* pContext, nuiWidgetElement Element)
{
  CheckValid();
  pContext->SetStrokeColor(GetColor(Element));
}

void nuiWidget::SetTheme(nuiTheme* pTheme)
{
  CheckValid();
  if (mpTheme)
    mpTheme->Release();
  mpTheme = pTheme;
  if (mpTheme)
    mpTheme->Acquire();
  DebugRefreshInfo();
}

nuiTheme* nuiWidget::GetTheme()
{
  CheckValid();
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

void nuiWidget::AutoTrash(const nuiEvent& rEvent)
{
  CheckValid();
  Trash();
}

void nuiWidget::AutoHide(const nuiEvent& rEvent)
{
  CheckValid();
  SetVisible(false);
}

void nuiWidget::AutoStartTransition(const nuiEvent& rEvent)
{
  CheckValid();
  StartTransition();
}
               
void nuiWidget::AutoStopTransition(const nuiEvent& rEvent)
{
  CheckValid();
  StopTransition();
}
               

bool nuiWidget::IsDrawingInCache(bool Recurse) 
{ 
  CheckValid();
  if (mSurfaceEnabled)
    return false;
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
  mMaxWidth  = rSource.mMaxWidth;
  mMinWidth  = rSource.mMinWidth;
  mMaxHeight = rSource.mMaxHeight;
  mMinHeight = rSource.mMinHeight;

  return *this;
}

bool nuiWidget::LayoutConstraint::operator==(const LayoutConstraint& rSource) const
{
  return ( mMaxWidth  == rSource.mMaxWidth
        && mMinWidth  == rSource.mMinWidth
        && mMaxHeight == rSource.mMaxHeight
        && mMinHeight == rSource.mMinHeight );
}

bool nuiWidget::SetLayoutConstraint(const nuiWidget::LayoutConstraint& rConstraint)
{
  CheckValid();
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
  CheckValid();
  return mConstraint;
}


nuiWidgetPtr nuiWidget::GetChild(nuiSize X, nuiSize Y)
{
  CheckValid();
  return IsInsideFromParent(X,Y) ? this : NULL;
}

void nuiWidget::SetDebug(int32 DebugLevel)
{
  CheckValid();
  mDebugLevel = DebugLevel;
  InvalidateLayout();
}

int32 nuiWidget::GetDebug(bool recursive) const
{
  CheckValid();
  if (recursive)
  {
    if (mpParent)
    {
      return MAX(mDebugLevel, mpParent->GetDebug(recursive));
    }
  }
  return mDebugLevel;
}

int32 nuiWidget::_GetDebug() const
{
  CheckValid();
  return mDebugLevel;
}

nuiHotKey* nuiWidget::RegisterHotKeyKey(const nglString& rName, nglKeyCode Trigger, nuiKeyModifier Modifiers, bool Priority /*= false*/, bool FireOnKeyUp /*= false*/, const nglString& rDescription /*= nglString::Empty*/)
{
  CheckValid();
  NGL_ASSERT(GetTopLevel());
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator it = mHotKeyEvents.find(rName);
  if (it == mHotKeyEvents.end())
    mHotKeyEvents[rName] = new nuiSimpleEventSource<nuiWidgetActivated>();
  SetWantKeyboardFocus(true);
  return GetTopLevel()->RegisterHotKeyKey(rName, Trigger, Modifiers, Priority, FireOnKeyUp, rDescription);
}

nuiHotKey* nuiWidget::RegisterHotKeyChar(const nglString& rName, nglChar Trigger, nuiKeyModifier Modifiers, bool Priority /*= false*/, bool FireOnKeyUp /*= false*/, const nglString& rDescription /*= nglString::Empty*/)
{
  CheckValid();
  NGL_ASSERT(GetTopLevel());
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::const_iterator it = mHotKeyEvents.find(rName);
  if (it == mHotKeyEvents.end())
    mHotKeyEvents[rName] = new nuiSimpleEventSource<nuiWidgetActivated>();
  SetWantKeyboardFocus(true);
  return GetTopLevel()->RegisterHotKeyChar(rName, Trigger, Modifiers, Priority, FireOnKeyUp, rDescription);
}

nuiHotKey* nuiWidget::GetHotKey(const nglString& rName)
{
  CheckValid();
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
  CheckValid();
  std::map<nglString, nuiSimpleEventSource<nuiWidgetActivated>*>::iterator it = mHotKeyEvents.find(rName);
  NGL_ASSERT(it != mHotKeyEvents.end());
  return *(it->second);
}

void nuiWidget::DelHotKey(const nglString& rName)
{
  CheckValid();
  NGL_ASSERT(GetTopLevel());
  GetTopLevel()->DelHotKey(rName);
  mHotKeyEvents.erase(rName);
  if (mHotKeyEvents.empty())
    SetWantKeyboardFocus(false);
}

void nuiWidget::SetMaxIdealWidth(float MaxWidth)
{
  CheckValid();
  if (mMaxIdealWidth == MaxWidth)
    return;
  mMaxIdealWidth = MaxWidth;
  InvalidateLayout();
}

void nuiWidget::SetMaxIdealHeight(float MaxHeight)
{
  CheckValid();
  if (mMaxIdealHeight == MaxHeight)
    return;
  mMaxIdealHeight = MaxHeight;
  InvalidateLayout();
}

void nuiWidget::SetMinIdealWidth(float MinWidth)
{
  CheckValid();
  if (mMinIdealWidth == MinWidth)
    return;
  mMinIdealWidth = MinWidth;
  InvalidateLayout();
}

void nuiWidget::SetMinIdealHeight(float MinHeight)
{
  CheckValid();
  if (mMinIdealHeight == MinHeight)
    return;
  mMinIdealHeight = MinHeight;
  InvalidateLayout();
}

float nuiWidget::GetMaxIdealWidth() const
{
  CheckValid();
  return mMaxIdealWidth;
}

float nuiWidget::GetMaxIdealHeight() const
{
  CheckValid();
  return mMaxIdealHeight;
}

float nuiWidget::GetMinIdealWidth() const
{
  CheckValid();
  return mMinIdealWidth;
}

float nuiWidget::GetMinIdealHeight() const
{
  CheckValid();
  return mMinIdealHeight;
}


void nuiWidget::SetMaxWidth(float MaxWidth)
{
  CheckValid();
  if (mMaxWidth == MaxWidth)
    return;
  mMaxWidth = MaxWidth;
  InvalidateLayout();
}

void nuiWidget::SetMaxHeight(float MaxHeight)
{
  CheckValid();
  if (mMaxHeight == MaxHeight)
    return;
  mMaxHeight = MaxHeight;
  InvalidateLayout();
}

void nuiWidget::SetMinWidth(float MinWidth)
{
  CheckValid();
  if (mMinWidth == MinWidth)
    return;
  mMinWidth = MinWidth;
  InvalidateLayout();
}

void nuiWidget::SetMinHeight(float MinHeight)
{
  CheckValid();
  if (mMinHeight == MinHeight)
    return;
  mMinHeight = MinHeight;
  InvalidateLayout();
}

float nuiWidget::GetMaxWidth() const
{
  CheckValid();
  return mMaxWidth;
}

float nuiWidget::GetMaxHeight() const
{
  CheckValid();
  return mMaxHeight;
}

float nuiWidget::GetMinWidth() const
{
  CheckValid();
  return mMinWidth;
}

float nuiWidget::GetMinHeight() const
{
  CheckValid();
  return mMinIdealHeight;
}


nuiEventSource* nuiWidget::GetEvent(const nglString& rName)
{
  CheckValid();
  std::map<nglString, nuiEventSource*, nglString::LessFunctor>::iterator it = mEventMap.find(rName);
  if (it == mEventMap.end())
    return NULL;
  return (*it).second;
}

void nuiWidget::GetEvents(std::vector<nglString>& rNames)
{
  CheckValid();
  std::map<nglString, nuiEventSource*, nglString::LessFunctor>::iterator it;    
  std::map<nglString, nuiEventSource*, nglString::LessFunctor>::iterator end = mEventMap.end();
  
  for (it = mEventMap.begin(); it != end; ++it)
    rNames.push_back((*it).first);
}

void nuiWidget::AddEvent(const nglString& rName, nuiEventSource& rEvent)
{
  CheckValid();
  mEventMap[rName] = &rEvent;
}

void nuiWidget::UpdateLayout()
{
  CheckValid();
  if (mInSetRect)
    return;
  GetIdealRect();
  nuiRect r(GetRect());
  mInSetRect = true;
  SetRect(r);
  mInSetRect = false;

  Invalidate();
}

void nuiWidget::SetOverDraw(nuiSize Left, nuiSize Top, nuiSize Right, nuiSize Bottom)
{
  CheckValid();
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

void nuiWidget::SetOverDrawLeft(nuiSize border)
{
  CheckValid();
  if (mODLeft == border)
    return;
  
  mODLeft = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetOverDrawTop(nuiSize border)
{
  CheckValid();
  if (mODTop == border)
    return;
  
  mODTop = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetOverDrawRight(nuiSize border)
{
  CheckValid();
  if (mODRight == border)
    return;
  
  mODRight = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

void nuiWidget::SetOverDrawBottom(nuiSize border)
{
  CheckValid();
  if (mODBottom == border)
    return;
  
  mODBottom = border;
  InvalidateLayout();
  DebugRefreshInfo();
}

nuiSize nuiWidget::GetOverDrawLeft() const
{
  CheckValid();
  return mODLeft;
}

nuiSize nuiWidget::GetOverDrawTop() const
{
  CheckValid();
  return mODTop;
}

nuiSize nuiWidget::GetOverDrawRight() const
{
  CheckValid();
  return mODRight;
}

nuiSize nuiWidget::GetOverDrawBottom() const
{
  CheckValid();
  return mODBottom;
}


void nuiWidget::ResetOverDraw()
{
  CheckValid();
  SetOverDraw(0, 0, 0, 0);
}

void nuiWidget::GetOverDraw(nuiSize& Left, nuiSize& Top, nuiSize& Right, nuiSize& Bottom, bool IncludeDecorations) const
{
  CheckValid();
  Left = mODLeft;
  Right = mODRight;
  Top = mODTop;
  Bottom = mODBottom;
  
  if (IncludeDecorations)
  {
    if (mpDecoration)
    {    
      if (mDecorationMode != eDecorationClientOnly)
      {
        float l , r, t, b, h, v;
        mpDecoration->GetBorders(this, l, r, t, b, h, v);
        Left   = MAX(Left  , l);
        Top    = MAX(Top   , t);
        Right  = MAX(Right , r);
        Bottom = MAX(Bottom, b);
      }
    }
    
    if (mpFocusDecoration)
    {    
      if (mFocusDecorationMode != eDecorationClientOnly)
      {
        float l , r, t, b, h, v;
        mpFocusDecoration->GetBorders(this, l, r, t, b, h, v);
        Left   = MAX(Left  , l);
        Top    = MAX(Top   , t);
        Right  = MAX(Right , r);
        Bottom = MAX(Bottom, b);
      }
    }
  }  
}

nuiRect nuiWidget::GetOverDrawRect(bool LocalRect, bool IncludeDecorations) const
{
  CheckValid();
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
  CheckValid();
  if (mInteractiveOD != set)
  {
    mInteractiveOD = set;
    Invalidate();
  }
}

void nuiWidget::EnableInteractiveDecoration(bool set)
{
  CheckValid();
  if (mInteractiveDecoration != set)
  {
    mInteractiveDecoration = set;
    Invalidate();
  }
}

bool nuiWidget::IsOverDrawInteractive() const
{
  CheckValid();
  return mInteractiveOD;
}

bool nuiWidget::IsDecorationInteractive() const
{
  CheckValid();
  return mInteractiveDecoration;
}

void nuiWidget::InitDefaultDecorations()
{
  CheckValid();
  if (mDefaultDecorations.size())
    return;

  App->AddExit(&nuiWidget::ExitDefaultDecorations);
  nuiDefaultDecoration::Init();
}

void nuiWidget::ExitDefaultDecorations()
{
  mDefaultDecorations.clear();
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
  CheckValid();
  nuiDecoration* pDecoration = nuiDecoration::Get(rName);
  if (!pDecoration)
  {
    nuiColor col;
    if (col.SetValue(rName))
    {
      pDecoration = new nuiColorDecoration(rName, rName, 0, nuiColor(), eFillShape, nuiBlendTransp, nuiRect());
    }
    else // Try to load a CSS description for an object:
    {
      nuiObject* pObj = nuiCSS::CreateObject(rName);
      pDecoration = dynamic_cast<nuiDecoration*> (pObj);
      if (!pDecoration)
        delete pObj;
    }
  }
  SetDecoration(pDecoration, mDecorationMode, true);
}

void nuiWidget::SetDecorationMode(nuiDecorationMode Mode)
{
  CheckValid();
  if (mDecorationMode == Mode)
    return;
  
  mDecorationMode = Mode;
  InvalidateLayout();
}

const nglString& nuiWidget::GetDecorationName() const
{
  CheckValid();
  if (mpDecoration)
    return mpDecoration->GetObjectName();
  return nglString::Null;
}

nuiDecorationMode nuiWidget::GetDecorationMode() const
{
  CheckValid();
  return mDecorationMode;
}

void nuiWidget::SetDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode, bool AlreadyAcquired)
{
  if (mpDecoration == pDecoration && Mode == GetDecorationMode())
    return;

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
  CheckValid();
  return mpDecoration;
}


void nuiWidget::EnableDecoration(bool set)
{
  CheckValid();
  mDecorationEnabled = set;
  
  if (!mDecorationEnabled)
  {
    nuiDecoration* pDeco = GetDecoration();
    if (pDeco)
      mDecorationName = pDeco->GetObjectName();
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
  CheckValid();
  return mDecorationEnabled;
}




/// Focus Decoration:
void nuiWidget::SetFocusDecoration(const nglString& rName)
{
  CheckValid();
  nuiDecoration* pDecoration = nuiDecoration::Get(rName);
  SetFocusDecoration(pDecoration, mFocusDecorationMode);
}

void nuiWidget::SetFocusDecorationMode(nuiDecorationMode Mode)
{
  CheckValid();
  mFocusDecorationMode = Mode;
  InvalidateLayout();
}

const nglString& nuiWidget::GetFocusDecorationName() const
{
  CheckValid();
  if (mpFocusDecoration)
    return mpFocusDecoration->GetObjectName();
  return nglString::Null;
}

nuiDecorationMode nuiWidget::GetFocusDecorationMode() const
{
  CheckValid();
  return mFocusDecorationMode;
}

void nuiWidget::SetFocusDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode)
{
  CheckValid();
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
  CheckValid();
  return mpFocusDecoration;
}


//////// TopLevel Management:
void nuiWidget::CallConnectTopLevel(nuiTopLevel* pTopLevel)
{
  CheckValid();
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
  CheckValid();
  if (HasFocus())
    UnFocus();
  pTopLevel->DisconnectWidget(this);
  DisconnectTopLevel();
}
 
void nuiWidget::ConnectTopLevel()
{
  CheckValid();
}

void nuiWidget::DisconnectTopLevel()
{
  CheckValid();
}

void nuiWidget::ApplyCSSForStateChange(uint32 MatchersTag)
{
  CheckValid();
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
  CheckValid();
  InternalResetCSSPass();
}

void nuiWidget::InternalResetCSSPass()
{
  CheckValid();
  mCSSPasses = 0;
}


void nuiWidget::IncrementCSSPass()
{
  CheckValid();
  mCSSPasses++;
}

uint32 nuiWidget::GetCSSPass() const
{
  CheckValid();
  return mCSSPasses;
}

void nuiWidget::DrawFocus(nuiDrawContext* pContext, bool FrontOrBack)
{
  CheckValid();
  //#FIXME LBDEBUG : there's a bug somewhere in that, that makes ComboBox tutorial crash with Win32 (ati, vista)
  // deactivate the DrawFocus for now, in order to release the application.
  //return;


  if (!mShowFocus)
    return;
  
  if (mpFocusDecoration)
  {
    nuiRect sizerect(GetVisibleRect().Size());
    mpFocusDecoration->ClientToGlobalRect(sizerect, this);
    if (FrontOrBack)
      mpFocusDecoration->DrawFront(pContext, this, sizerect);
    else
      mpFocusDecoration->DrawBack(pContext, this, sizerect);
  }
  else
  {
    if (FrontOrBack)
    {
      nuiRect rect(GetVisibleRect());
      rect.Bottom() -= 1;
      rect.Top() += 0;
      rect.Left() += 0;
      rect.Right() -= 1;
      
      pContext->ResetState();
      pContext->SetLineWidth(2);
      pContext->SetBlendFunc(nuiBlendTransp);
      pContext->EnableBlending(true);
      //pContext->EnableTexturing(false);
      pContext->SetStrokeColor(nuiColor(64, 64, 255, 128));
      
      nuiShape shp;
      shp.AddRect(rect);
      
      pContext->DrawShape(&shp, eStrokeShape);
    }
  }
}

void nuiWidget::SetFocusVisible(bool set)
{
  CheckValid();
  mShowFocus = set;
  Invalidate();
}

#ifdef NUI_PHONE
bool nuiWidget::mShowFocusDefault = false;
#else
bool nuiWidget::mShowFocusDefault = true;
#endif

void nuiWidget::SetFocusVisibleDefault(bool set)
{
  mShowFocusDefault = set;
}


bool nuiWidget::IsFocusVisible() const
{
  CheckValid();
  return mShowFocus;
}


void nuiWidget::OnPropertyChanged(const nglString& rName, const nglString&rValue)
{
  CheckValid();
  ApplyCSSForStateChange(NUI_WIDGET_MATCHER_PROPERTY);
}

void nuiWidget::SetClickThru(bool set)
{
  CheckValid();
  mClickThru = set;  
}

bool nuiWidget::GetClickThru() const
{
  CheckValid();
  return mClickThru;
}


void nuiWidget::AddInvalidRect(const nuiRect& rRect)
{
  CheckValid();
  //printf("+++ AddInvalidRect %ls\n", rRect.GetValue().GetChars());
  int count = mDirtyRects.size();
  
  nuiRect intersect;
  nuiSize surface = rRect.GetSurface();
  if (surface == 0.0f)
    return;
  
  for (int i = 0; i<count; i++)
  {
    if (intersect.Intersect(rRect, mDirtyRects[i]))
    {
      // The rectangles intersect so we create one big out of them
      nuiRect u;
      u.Union(rRect, mDirtyRects[i]);
      // Let's remove the coalesced rect from the list
      mDirtyRects.erase(mDirtyRects.begin() + i);
      // Try to add the new big rect to the list:
      AddInvalidRect(u);
      return;
    }
  }
  
  // Found no rect to blend into, let's create a new one:
  //printf("--- AddInvalidRect OK %ls\n", rRect.GetValue().GetChars());
  mDirtyRects.push_back(rRect);
}

bool nuiWidget::GetAutoClip() const
{
  return mAutoClip;
}

void nuiWidget::SetAutoClip(bool set)
{
  if (mAutoClip == set)
    return;
  mAutoClip = set;
  Invalidate();
}


void nuiWidget::AutoInvalidate(const nuiEvent& rEvent)
{
  Invalidate();
}

void nuiWidget::AutoInvalidateLayout(const nuiEvent& rEvent)
{
  InvalidateLayout();
}


// ***************************************************************************

void nuiWidget::OnFinalize()
{
  if (!IsTrashed())
    CallOnTrash();
}

// static 
std::vector<nuiDecorationDelegate> nuiWidget::mDefaultDecorations;
