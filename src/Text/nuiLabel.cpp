/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiApplication.h"
#include "nuiXML.h"
#include "nuiFontManager.h"

#define NUI_LABEL_HMARGIN 6.f
#define NUI_LABEL_VMARGIN 0.f

nuiLabel::nuiLabel(const nglString& Text, nuiTheme::FontStyle FontStyle)
  : nuiWidget(),
    mLabelSink(this),
    mUnderline(false),
    mStrikeThrough(false)
{
  if (SetObjectClass(_T("nuiLabel")))
    InitAttributes();

  mTextChanged = false;
  mFontChanged = true;
  mOrientation = nuiHorizontal;
  mpLayout = NULL;
  mpIdealLayout = NULL;
  mpFont = NULL;

  mTextColorSet = false;
  mBackColorSet = false;  
  
  mUseEllipsis = false;
  mClearBg = false;
  mTextPosition = nuiLeft;

  mVMargin = NUI_LABEL_VMARGIN;
  mHMargin = NUI_LABEL_HMARGIN;
  mWrapping = false;
  mIgnoreState = false;

  InitProperties();

  SetFont(FontStyle);
  SetText(Text);
}

nuiLabel::nuiLabel(const nglString& Text, nuiFont* pFont, bool AlreadyAcquired)
  : nuiWidget(),
    mLabelSink(this),
    mUnderline(false),
    mStrikeThrough(false)
{
  if (SetObjectClass(_T("nuiLabel")))
    InitAttributes();

  mTextChanged = false;
  mFontChanged = true;
  mOrientation = nuiHorizontal;
  mpLayout = NULL;
  mpIdealLayout = NULL;
  mpFont = NULL;

  mTextColorSet = false;
  mBackColorSet = false;  
  
  mUseEllipsis = false;
  mClearBg = false;
  mTextPosition = nuiLeft;

  mVMargin = NUI_LABEL_VMARGIN;
  mHMargin = NUI_LABEL_HMARGIN;
  mWrapping = false;
  mIgnoreState = false;

  InitProperties();

  SetText(Text);
  SetFont(pFont, AlreadyAcquired);
}

bool nuiLabel::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  mLabelSink.SetTarget(this);
  mpLayout = NULL;
  mpIdealLayout = NULL;
  mpFont = NULL;
  mFontChanged = true;

  /*
  mColorNormal.SetValue(pNode->GetAttribute(_T("ColorNormal")));
  mColorNormalBg.SetValue(pNode->GetAttribute(_T("ColorNormalBg")));
  mColorSelected.SetValue(pNode->GetAttribute(_T("ColorSelected")));
  mColorDisabled.SetValue(pNode->GetAttribute(_T("ColorDisabled")));
  mColorSelectedBg.SetValue(pNode->GetAttribute(_T("ColorSelectedBg")));
  mColorDisabledBg.SetValue(pNode->GetAttribute(_T("ColorDisabledBg")));
  */

  mUseEllipsis = false;
  mClearBg = false;
  mWrapping = false;
  mIgnoreState = false;

  mTextPosition = nuiGetPosition(pNode,nuiLeft);

  InitAttributes();
  InitProperties();
  SetFont(nuiTheme::Default);
  if (pNode->GetChild(nglString(_T("##text"))))
    SetText(pNode->GetChild(nglString(_T("##text")))->GetValue());
  
  return true;
}

nuiXMLNode* nuiLabel::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiWidget::Serialize(pParentNode);
  if (!pNode) 
    return NULL;
  nuiXMLNode* pTextNode = new nuiXMLNode(_T("##text"),pNode);
  pTextNode->SetValue(mText);
  pNode->SetAttribute(_T("TextPosition"),mTextPosition);
  pNode->DelAttribute(_T("Text"));
/*
  pNode->SetAttribute(_T("ColorNormal"),mColorNormal.GetValue());
  pNode->SetAttribute(_T("ColorNormalBg"),mColorNormalBg.GetValue());
  pNode->SetAttribute(_T("ColorSelected"),mColorSelected.GetValue());
  pNode->SetAttribute(_T("ColorDisabled"),mColorDisabled.GetValue());
  pNode->SetAttribute(_T("ColorSelectedBg"),mColorSelectedBg.GetValue());
  pNode->SetAttribute(_T("ColorDisabledBg"),mColorDisabledBg.GetValue());
*/
  return pNode;
}


nuiLabel::~nuiLabel()
{
  //printf("~nuiLabel: '%ls'\n", mText.GetChars());
  delete mpLayout;
  delete mpIdealLayout;
  if (mpFont)
    mpFont->Release();
}

void nuiLabel::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
  (nglString(_T("Text")), nuiUnitName,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetText), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetText)));

  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("TextColor")), nuiUnitNone,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetTextColor), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetTextColor)));
  

  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("BackgroundColor")), nuiUnitNone,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetBackgroundColor), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetBackgroundColor)));
  

  AddAttribute(new nuiAttribute<bool>
  (nglString(_T("Background")), nuiUnitYesNo,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetBackground), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetBackground)));
  
  AddAttribute(new nuiAttribute<bool>
  (nglString(_T("Underline")), nuiUnitYesNo,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetUnderline), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetUnderline)));
  
  AddAttribute(new nuiAttribute<bool>
  (nglString(_T("StrikeThrough")), nuiUnitYesNo,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetStrikeThrough), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetStrikeThrough)));
  
  
  AddAttribute(new nuiAttribute<nuiPosition>
  (nglString(_T("TextPosition")), nuiUnitPosition,
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::GetTextPosition), 
   nuiFastDelegate::MakeDelegate(this, &nuiLabel::SetTextPosition)));
  

  AddAttribute(new nuiAttribute<const nglString&>
  (nglString(_T("Font")), nuiUnitName,
    nuiFastDelegate::MakeDelegate(this, &nuiLabel::_GetFont), 
    nuiFastDelegate::MakeDelegate(this, &nuiLabel::_SetFont)));
}

void nuiLabel::InitProperties()
{
  mCanRespectConstraint = true;
}

void nuiLabel::_SetFont(const nglString& rFontSymbol)
{
  SetFont(rFontSymbol);
}

const nglString& nuiLabel::_GetFont() const
{
  if (mpFont)
    return mpFont->GetObjectName();
  return nglString::Null;
}


void nuiLabel::SetTextColor(const nuiColor& Color)
{
  mTextColorSet = true;
  mTextColor = Color;
  Invalidate();
}

void nuiLabel::SetBackgroundColor(const nuiColor& Color)
{
  mBackColorSet = true;
  mBackColor = Color;
  SetBackground(true);
  Invalidate();
}

const nuiColor& nuiLabel::GetTextColor() const
{
  return mTextColor;
}

const nuiColor& nuiLabel::GetBackgroundColor() const
{
  return mBackColor;
}

bool nuiLabel::Draw(nuiDrawContext* pContext)
{
//  NGL_OUT(_T("Draw 0x%x\n"), this);
  nuiColor Color = GetTextColor();
  Color.Alpha() *= GetAlpha(true);
  nuiColor ColorBg = GetBackgroundColor();
  ColorBg.Alpha() *= GetAlpha(true);

  CalcLayout();

  pContext->SetFont(mpFont);
  
  if (mIgnoreState)
  {
    if (!mBackColorSet)
      ColorBg = GetColor(eNormalTextBg);
    if (!mTextColorSet)
      Color = GetColor(eNormalTextFg);
  }
  else
  {
    if (IsEnabled())
    {
      if (IsSelected())
      {
        if (!mBackColorSet)
          ColorBg = GetColor(eSelectedTextBg);
        if (!mTextColorSet)
          Color = GetColor(eSelectedTextFg);
      }
      else
      {
        if (!mBackColorSet)
          ColorBg = GetColor(eNormalTextBg);
        if (!mTextColorSet)
          Color = GetColor(eNormalTextFg);
      }
    }
    else
    {
      if (IsSelected())
      {
        if (!mBackColorSet)
          ColorBg = GetColor(eSelectedTextBg);
        if (!mTextColorSet)
          Color = GetColor(eDisabledTextFg);
      }
      else
      {
        if (!mBackColorSet)
          ColorBg = GetColor(eDisabledTextBg);
        if (!mTextColorSet)
          Color = GetColor(eDisabledTextFg);
      }
    }
  }

  nglGlyphInfo GlyphInfo;
  mpLayout->GetMetrics(GlyphInfo);

  nglFontInfo info;
  mpFont->GetInfo(info);

  nuiRect rect = mIdealLayoutRect;

  rect.SetPosition(mTextPosition, mRect.Size());
  //pContext->DrawRect(rect, eStrokeShape);
  
  rect.mTop += (nuiSize)ToNearest(info.Ascender) + mVMargin;
  rect.mLeft +=  mHMargin;

  if (mClearBg)
  {
    if (ColorBg.Alpha() < 1.0)
    {
      pContext->SetBlendFunc(nuiBlendTransp);
      pContext->EnableBlending(true);
    }
    pContext->SetFillColor(ColorBg);   
    pContext->DrawRect(mRect.Size(),eFillShape);
    if (ColorBg.Alpha() < 1.0)
    {
      pContext->SetBlendFunc(nuiBlendSource);
      pContext->EnableBlending(false);
    }
  }

  if (mpLayout)
  {
    /* currently disabled as it doesn't look that good in many cases
    if (GetState() == nuiDisabled && !mIgnoreState)
    {
      nuiColor c = Color;
      c.Red()   *= 2;
      c.Green() *= 2;
      c.Blue()  *= 2;
      pContext->SetTextColor(c);
      pContext->DrawText((nuiSize)ToNearest(rect.mLeft+1), (nuiSize)ToNearest(rect.mTop+1), *mpLayout);
    }
    */
    pContext->SetTextColor(Color);

    pContext->DrawText((nuiSize)ToNearest(rect.mLeft), (nuiSize)ToNearest(rect.mTop), *mpLayout);
  }
  else
  {
//    NGL_OUT(_T("FUCK 0x%x!"), this);
  }
  return true;
}

void nuiLabel::CalcLayout()
{
  if (mpFont)
  {
    if (mTextChanged || mFontChanged || !mpLayout)
    {
      if (mFontChanged)
      {
        if (mpLayout)
          delete mpLayout;
        if (mpIdealLayout)
          delete mpIdealLayout;
        mpLayout = new nuiFontLayout(*mpFont, 0, 0, mOrientation);
        mpIdealLayout = new nuiFontLayout(*mpFont, 0, 0, mOrientation);
        mpLayout->SetUnderline(mUnderline);
        mpIdealLayout->SetUnderline(mUnderline);
        mpLayout->SetStrikeThrough(mStrikeThrough);
        mpIdealLayout->SetStrikeThrough(mStrikeThrough);
        mFontChanged = false;
      }

      mpLayout->Init(0,0);
      mpIdealLayout->Init(0,0);
      if (mWrapping)
      {
        //NGL_OUT(_T("Setting wrapping to %f\n"), mConstraint.mMaxWidth);
        float wrap1 = mConstraint.mMaxWidth;
        float wrap2 = GetMaxIdealWidth();
        float wrap = 0;
        if (wrap1 > 0 && wrap2 > 0)
          wrap = MIN(wrap1, wrap2);
        else
          wrap = MAX(wrap1, wrap2);
        
        mpLayout->SetWrapX(wrap - mBorderLeft - mBorderRight);
        mpIdealLayout->SetWrapX(wrap - mBorderLeft - mBorderRight);
      }
      else
      {
        mpLayout->SetWrapX(0);
        mpIdealLayout->SetWrapX(0);
      }

      mpLayout->Layout(mText);
      mpIdealLayout->Layout(mText);
      GetLayoutRect();
      mTextChanged = false;
      mFontChanged = false;
    }
  }
}

nuiRect nuiLabel::CalcIdealSize()
{
//  NGL_OUT(_T("Calc 0x%x\n"), this);
  nuiRect Rect;
  CalcLayout();

  if (mpLayout)
  {
    Rect = mIdealLayoutRect;
  }

  Rect.RoundToBiggest();
  mIdealRect = Rect;
  //NGL_OUT(_T("%ls [%ls]"), mText.GetChars(), mIdealRect.GetValue().GetChars());
  return mIdealRect;
}

bool nuiLabel::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  nuiRect ideal(mIdealLayoutRect);


  if (ideal.GetWidth() > mRect.GetWidth())
  {
    if (mUseEllipsis)
    {
      nuiSize diff = ideal.GetWidth() - mRect.GetWidth();
      int NbLetterToRemove = ToNearest(diff / (ideal.GetWidth() / mText.GetLength())) + 3;
      nglString text = mText;
      int len = text.GetLength();
      text.DeleteRight(MIN(NbLetterToRemove, len));
      text.Append(_T("..."));
      mpLayout->Init(0,0);
      mpLayout->SetWrapX(0);
      mpLayout->Layout(text);
      CalcLayout();
      GetLayoutRect();
    }
    else if (mWrapping)
    {
      mpLayout->Init(0,0);
      mpLayout->SetWrapX(mRect.GetWidth() - mBorderLeft - mBorderRight);
      mpLayout->Layout(mText);
      GetLayoutRect();
    }

    SetToolTip(mText);
  }
  else
  {
    if (GetToolTip() == mText)
      SetToolTip(nglString::Empty);
  }

  return true;
}

nuiRect nuiLabel::GetLayoutRect()
{
  mIdealLayoutRect = mpIdealLayout->GetRect();
  mIdealLayoutRect.Grow(mHMargin, mVMargin);
  mIdealLayoutRect = mIdealLayoutRect.Size();
  return mIdealLayoutRect;
}

void nuiLabel::SetText(const nglString& Text)
{
  if (GetToolTip() == mText) // Reset the tooltip shown when text is truncated as it's maybe no more usefull
    SetToolTip(nglString::Empty);
  mText = Text;
  mText.TrimRight(_T("\n\r\t"));
  mText.Replace('\r','\n');
  mTextChanged = true;
  InvalidateLayout();
}

const nglString& nuiLabel::GetText() const
{
  return mText;
}

void nuiLabel::SetThemeTextColor(const nuiColor& Color, bool Selected, bool Enabled)
{
  if (Enabled && !Selected)
    SetColor(eNormalTextFg, Color);
  else if (Enabled && Selected)
    SetColor(eSelectedTextFg, Color);
  else if (!Enabled)
    SetColor(eDisabledTextFg, Color);
  Invalidate();
}

nuiColor nuiLabel::GetThemeTextColor(bool Selected, bool Enabled)
{
  if (Enabled && !Selected)
    return GetColor(eNormalTextFg);
  else if (Enabled && Selected)
    return GetColor(eSelectedTextFg);
  else if (!Enabled)
    return GetColor(eDisabledTextFg);
  return GetColor(eNormalTextFg);
}

void nuiLabel::SetThemeBackgroundColor(const nuiColor& Color, bool Selected, bool Enabled)
{
  SetBackground(true);
  if (Enabled && !Selected)
    SetColor(eNormalTextBg, Color);
  else if (Enabled && Selected)
    SetColor(eSelectedTextBg, Color);
  else if (!Enabled)
    SetColor(eDisabledTextBg, Color);
  Invalidate();
}

nuiColor nuiLabel::GetThemeBackgroundColor(bool Selected, bool Enabled)
{
  if (Enabled && !Selected)
    return GetColor(eNormalTextBg);
  else if (Enabled && Selected)
    return GetColor(eSelectedTextBg);
  else if (!Enabled)
    return GetColor(eDisabledTextBg);
  return GetColor(eNormalTextBg);
}

void nuiLabel::SetBackground(bool bg)
{
  mClearBg = bg;
  Invalidate();
}

bool nuiLabel::GetBackground() const
{
  return mClearBg;
}

void nuiLabel::SetFont(nuiTheme::FontStyle FontStyle)
{
  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);
  SetFont(pTheme->GetFont(FontStyle), true);
  pTheme->Release();
}

void nuiLabel::SetFont(nuiFont* pFont, bool AlreadyAcquired)
{
  if (!pFont)
  {
    AlreadyAcquired = true;
    pFont = nuiFont::GetFont(14.0f);
  }

  if (pFont == mpFont)
  {
    if (AlreadyAcquired)
    {
      mpFont->Release();
    }
    return;
  }

  if(mpFont)
    mpFont->Release();

  mpFont = pFont;
  if (!AlreadyAcquired)
    mpFont->Acquire();

  mFontChanged = true;
  InvalidateLayout();
}

void nuiLabel::SetFont(nuiFontRequest& rFontRequest)
{
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(rFontRequest);
  if (pFont)
    SetFont(pFont);
}

void nuiLabel::SetFont(const nglString& rFontSymbol)
{
  nuiFont* pFont = nuiFont::GetFont(rFontSymbol);
  if (pFont)
    SetFont(pFont);
}



nuiFont* nuiLabel::GetFont()
{
  return mpFont;
}

void nuiLabel::SetTextPosition(nuiPosition Position)
{
  if (mTextPosition == Position)
    return;
  mTextPosition = Position;
  Invalidate();
}

bool nuiLabel::OnTextChanged(const nuiEvent& rEvent)
{
  mTextChanged = true;
  InvalidateLayout();
  return false;
}

nuiPosition nuiLabel::GetTextPosition()
{
  return mTextPosition;
}


void nuiLabel::SetVMargin(nuiSize VMargin)
{
  mVMargin = VMargin;
  InvalidateLayout();
}

void nuiLabel::SetHMargin(nuiSize HMargin)
{
  mHMargin = HMargin;
  InvalidateLayout();
}

nuiSize nuiLabel::GetVMargin()
{
  return mVMargin;
}

nuiSize nuiLabel::GetHMargin()
{
  return mHMargin;
}


void nuiLabel::UseEllipsis(bool useEllipsis)
{
  mUseEllipsis = useEllipsis;
  InvalidateLayout();
}

void nuiLabel::SetOrientation(nuiOrientation Orientation)
{
  if (mOrientation == Orientation)
    return;
  
  mOrientation = Orientation;
  InvalidateLayout();
  mTextChanged = true;
}

nuiOrientation nuiLabel::GetOrientation()
{
  return mOrientation;
}

void nuiLabel::SetWrapping(bool Wrapping)
{
  if (mWrapping != Wrapping)
  {
    mWrapping = Wrapping;
    mTextChanged = true;
    InvalidateLayout();
  }
}

bool nuiLabel::IsWrapping() const
{
  return mWrapping;
}

bool nuiLabel::SetLayoutConstraint(const nuiWidget::LayoutConstraint& rConstraint)
{
  if (nuiWidget::SetLayoutConstraint(rConstraint))
  {
    //NGL_OUT(_T("nuiLabel::SetLayoutConstraint %f %f\n"), mConstraint.mMaxWidth, mConstraint.mMaxHeight);
    mTextChanged = true;
    return true;
  }
  return false;
}

void nuiLabel::SetUnderline(bool set)
{
  mUnderline = set;
  if (mpLayout)
    mpLayout->SetUnderline(mUnderline);
  Invalidate();
}

bool nuiLabel::GetUnderline() const
{
  return mUnderline;
}

void nuiLabel::SetStrikeThrough(bool set)
{
  mStrikeThrough = set;
  if (mpLayout)
    mpLayout->SetStrikeThrough(mStrikeThrough);
  Invalidate();
}

bool nuiLabel::GetStrikeThrough() const
{
  return mStrikeThrough;
}
