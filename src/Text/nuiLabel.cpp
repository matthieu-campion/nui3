/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define NUI_LABEL_HMARGIN 6.f
#define NUI_LABEL_VMARGIN 0.f

nuiLabel::nuiLabel(const nglString& Text, nuiTheme::FontStyle FontStyle)
  : nuiWidget(),
    mLabelSink(this)
{
  InitDefaultValues();

  if (SetObjectClass(_T("nuiLabel")))
    InitAttributes();

  InitProperties();

  SetFont(FontStyle);
  SetText(Text);
}

nuiLabel::nuiLabel(const nglString& Text, nuiFont* pFont, bool AlreadyAcquired)
  : nuiWidget(),
    mLabelSink(this)
{
  InitDefaultValues();

  if (SetObjectClass(_T("nuiLabel")))
    InitAttributes();

  InitProperties();

  SetText(Text);
  SetFont(pFont, AlreadyAcquired);
}


nuiLabel::nuiLabel(const nglString& Text, const nglString& rObjectName, nuiTheme::FontStyle FontStyle)
: nuiWidget(rObjectName), mLabelSink(this)
{
  InitDefaultValues();
  
  if (SetObjectClass(_T("nuiLabel")))
    InitAttributes();
  
  InitProperties();
  
  SetFont(FontStyle);
  SetText(Text);
}



void nuiLabel::InitDefaultValues()
{
  //EnableSurface(true);
  //SetSurfaceBlendFunc(nuiBlendTop); // This is needed to handle text blending corectly for now.

  mUnderline = false;
  mStrikeThrough = false;
  
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
}


nuiLabel::~nuiLabel()
{
  //printf("~nuiLabel: '%s'\n", mText.GetChars());
  delete mpLayout;
  delete mpIdealLayout;
  if (mpFont)
    mpFont->Release();
}

void nuiLabel::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
  (nglString(_T("Text")), nuiUnitName,
   nuiMakeDelegate(this, &nuiLabel::GetText), 
   nuiMakeDelegate(this, &nuiLabel::SetText)));

  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("TextColor")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiLabel::GetTextColor), 
   nuiMakeDelegate(this, &nuiLabel::SetTextColor)));
  

  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("BackgroundColor")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiLabel::GetBackgroundColor), 
   nuiMakeDelegate(this, &nuiLabel::SetBackgroundColor)));
  

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("Background")), nuiUnitYesNo,
    nuiMakeDelegate(this, &nuiLabel::GetBackground), 
    nuiMakeDelegate(this, &nuiLabel::SetBackground)));

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("Wraping")), nuiUnitYesNo,
    nuiMakeDelegate(this, &nuiLabel::IsWrapping), 
    nuiMakeDelegate(this, &nuiLabel::SetWrapping)));

  AddAttribute(new nuiAttribute<bool>
  (nglString(_T("Underline")), nuiUnitYesNo,
   nuiMakeDelegate(this, &nuiLabel::GetUnderline), 
   nuiMakeDelegate(this, &nuiLabel::SetUnderline)));
  
  AddAttribute(new nuiAttribute<bool>
  (nglString(_T("StrikeThrough")), nuiUnitYesNo,
   nuiMakeDelegate(this, &nuiLabel::GetStrikeThrough), 
   nuiMakeDelegate(this, &nuiLabel::SetStrikeThrough)));
  
  
  AddAttribute(new nuiAttribute<nuiPosition>
  (nglString(_T("TextPosition")), nuiUnitPosition,
   nuiMakeDelegate(this, &nuiLabel::GetTextPosition), 
   nuiMakeDelegate(this, &nuiLabel::SetTextPosition)));
  

  AddAttribute(new nuiAttribute<const nglString&>
  (nglString(_T("Font")), nuiUnitName,
    nuiMakeDelegate(this, &nuiLabel::_GetFont), 
    nuiMakeDelegate(this, &nuiLabel::_SetFont)));
  
  AddAttribute(new nuiAttribute<nuiOrientation>
  (nglString(_T("Orientation")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiLabel::GetOrientation),
   nuiMakeDelegate(this, &nuiLabel::SetOrientation)));
  
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("HMargin")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiLabel::GetHMargin), 
                nuiMakeDelegate(this, &nuiLabel::SetHMargin)));
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("VMargin")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiLabel::GetVMargin), 
                nuiMakeDelegate(this, &nuiLabel::SetVMargin)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("UseEllipsis")), nuiUnitYesNo,
                nuiMakeDelegate(this, &nuiLabel::GetUseEllipsis), 
                nuiMakeDelegate(this, &nuiLabel::UseEllipsis)));
  
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
  nuiColor ColorBg = GetBackgroundColor();

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

  nuiGlyphInfo GlyphInfo;
  mpLayout->GetMetrics(GlyphInfo);

  nuiFontInfo info;
  mpFont->GetInfo(info);
  //info.Dump(0);

  float alpha = GetMixedAlpha();
  Color.Multiply(alpha);
  ColorBg.Multiply(alpha);
  
  nuiRect rect = mpLayout->GetRect();

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
      delete mpLayout;
      delete mpIdealLayout;
      mpLayout = NULL;
      mpIdealLayout = NULL;
      mpLayout = new nuiTextLayout(mpFont, mOrientation);
      mpLayout->SetUnderline(mUnderline);
      mpLayout->SetStrikeThrough(mStrikeThrough);
      mpIdealLayout = new nuiTextLayout(mpFont, mOrientation);
      mpIdealLayout->SetUnderline(mUnderline);
      mpIdealLayout->SetStrikeThrough(mStrikeThrough);
      mFontChanged = false;

      NGL_ASSERT(mpLayout);
      NGL_ASSERT(mpIdealLayout);

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
        
        NGL_ASSERT(mpLayout);
        NGL_ASSERT(mpIdealLayout);
        mpLayout->SetWrapX(wrap - mBorderLeft - mBorderRight);
        mpIdealLayout->SetWrapX(wrap - mBorderLeft - mBorderRight);
      }
      else
      {
        NGL_ASSERT(mpLayout);
        NGL_ASSERT(mpIdealLayout);
        mpLayout->SetWrapX(0);
        mpIdealLayout->SetWrapX(0);
      }

      NGL_ASSERT(mpLayout);
      NGL_ASSERT(mpIdealLayout);

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
  CalcLayout();

  if (mpLayout)
  {
    mIdealRect = mIdealLayoutRect;
//    if (GetDebug(1))
//    {
//      printf("New ideal rect: %s\n", mIdealRect.GetValue().GetChars());
//    }
  }

  mIdealRect.RoundToBiggest();
  //NGL_OUT(_T("%s [%s]"), mText.GetChars(), mIdealRect.GetValue().GetChars());
  return mIdealRect;
}

bool nuiLabel::SetRect(const nuiRect& rRect)
{
  bool needRecalcLayout = false;

  if (mUseEllipsis || mWrapping)
    needRecalcLayout = (rRect.GetWidth() != mRect.GetWidth());
    
  nuiWidget::SetRect(rRect);

  nuiRect ideal(mIdealLayoutRect);


  if (needRecalcLayout || ideal.GetWidth() > mRect.GetWidth())
  {
    if (mUseEllipsis)
    {
      CalcLayout();
      nuiSize diff = ideal.GetWidth() - mRect.GetWidth();
      int NbLetterToRemove = ToNearest(diff / (ideal.GetWidth() / mText.GetLength())) + 3;
      nglString text = mText;
      if (NbLetterToRemove > 0)
      {
        int len = text.GetLength();
        text.DeleteRight(MIN(NbLetterToRemove, len));
        text.Append(_T("..."));
      }
      delete mpLayout;
      mpLayout = new nuiTextLayout(mpFont);
      mpLayout->SetWrapX(0);
      mpLayout->Layout(text);
      GetLayoutRect();
    }
    else if (mWrapping)
    {
      CalcLayout();
      delete mpLayout;
      mpLayout = new nuiTextLayout(mpFont);
      delete mpIdealLayout;
      mpIdealLayout = new nuiTextLayout(mpFont);
      mpLayout->SetWrapX(mRect.GetWidth() - mBorderLeft - mBorderRight);
      mpIdealLayout->SetWrapX(mRect.GetWidth() - mBorderLeft - mBorderRight);
      mpLayout->Layout(mText);
      mpIdealLayout->Layout(mText);
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
//  if (GetDebug())
//  {
//    printf("New layout rect: %s\n", mIdealLayoutRect.GetValue().GetChars());
//  }
  
  if (!(mIdealLayoutRect == mIdealRect))
    InvalidateLayout();
  
  return mIdealLayoutRect;
}

void nuiLabel::SetText(const nglString& Text)
{
  if (GetToolTip() == mText) // Reset the tooltip shown when text is truncated as it's maybe no more usefull
    SetToolTip(nglString::Empty);
  nglString t(mText);
  mText = Text;
  mText.TrimRight(_T("\n\r\t"));
  mText.Replace('\r','\n');
  if (t == mText) // Only redisplay if the text has really changed!
    return;
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
  SetFont(nuiFont::GetFont(12));
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

void nuiLabel::OnTextChanged(const nuiEvent& rEvent)
{
  mTextChanged = true;
  InvalidateLayout();
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
bool nuiLabel::GetUseEllipsis() const
{
  return mUseEllipsis;
}

void nuiLabel::SetOrientation(nuiOrientation Orientation)
{
  if (mOrientation == Orientation)
    return;
  
  mOrientation = Orientation;
  InvalidateLayout();
  mTextChanged = true;
}

nuiOrientation nuiLabel::GetOrientation() const
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
