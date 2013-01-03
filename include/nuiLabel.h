/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiLabel_h__
#define __nuiLabel_h__

//#include "nui.h"
#include "nglString.h"
#include "nuiFont.h"
#include "nuiWidget.h"
#include "nuiTheme.h"

class nuiFontRequest;

/// This widget implements a text label. It can be used as a basic building block for buttons, list headers, list elements, etc...
class NUI_API nuiLabel : public nuiWidget
{
public:
  nuiLabel(const nglString& Text = nglString::Empty, nuiTheme::FontStyle FontStyle=nuiTheme::Default);
  nuiLabel(const nglString& Text, const nglString& rObjectName, nuiTheme::FontStyle FontStyle=nuiTheme::Default);
  nuiLabel(const nglString& Text, nuiFont* pFont, bool AlreadyAcquired = false);
  virtual ~nuiLabel();

  void IgnoreState(bool ignoreState = true) { mIgnoreState = ignoreState; }

  void InitAttributes();
  void InitProperties();
  void InitDefaultValues();

  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  virtual void SetText(const nglString& Text); ///< Modify the label's text.
  virtual const nglString& GetText() const; ///< Retrieve the label's text.

  void SetThemeTextColor(const nuiColor& Color, bool Selected, bool Enabled); ///< Get the text color for the given widget state.
  void SetThemeBackgroundColor(const nuiColor& Color, bool Selected, bool Enabled); ///< Set the background color to use to clear the rect before drawing the text.
  nuiColor GetThemeTextColor(bool Selected, bool Enabled); ///< Get the text color for the given widget state.
  nuiColor GetThemeBackgroundColor(bool Selected, bool Enabled); ///< Get the background color to use to clear the rect before drawing the text. 
  void SetFont(nuiTheme::FontStyle FontStyle); //< Change the font used by the widget.
  void SetFont(nuiFont* pFont, bool AlreadyAcquired = false); //< Change the font used by the widget.
  void SetFont(nuiFontRequest& rFontRequest);
  void SetFont(const nglString& rFontSymbol);
  nuiFont* GetFont(); //< Return the font used by the widget.
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;

  void SetTextColor(const nuiColor& Color); ///< Get the text color.
  void SetBackgroundColor(const nuiColor& Color); ///< Set the background color.
  const nuiColor& GetTextColor() const; ///< Get the text color.
  const nuiColor& GetBackgroundColor() const; ///< Get the background color. 
  
  
  void SetVMargin(nuiSize VMargin); ///< Set the vertical margin.
  void SetHMargin(nuiSize HMargin); ///< Set the horizontal margin.

  nuiSize GetVMargin(); ///< Get the vertical margin.
  nuiSize GetHMargin(); ///< Get the horizontal margin.

  void SetTextPosition(nuiPosition Position); ///< Set the position of the text in the widget's rectangle.
  nuiPosition GetTextPosition(); ///< Get the position of the text in the widget's rectangle.

  void SetOrientation(nuiOrientation Orientation); ///< Set the orientation of the text in the widget's rectangle.
  nuiOrientation GetOrientation() const; ///< Get the orientation of the text in the widget's rectangle.

  void SetBackground(bool bg); ///< If \param bg is true the label will clear its background before being drawn.
  bool GetBackground() const; ///< Return true if the label will clear its background before being drawn.

  void UseEllipsis(bool useEllipsis); ///< If \param useEllipsis is true the label will display ... at the end of the text if there is not enough space to display it
  bool GetUseEllipsis() const;

  void SetWrapping(bool Wrapping);
  bool IsWrapping() const;
  virtual bool SetLayoutConstraint(const nuiWidget::LayoutConstraint& rConstraint);

protected:
  void CalcLayout();
  void OnTextChanged(const nuiEvent& rEvent);

  nuiSize mVMargin;
  nuiSize mHMargin;

  nglString mText;
  nuiTextLayout* mpLayout;
  nuiTextLayout* mpIdealLayout;
  bool mTextChanged;
  bool mFontChanged;
  bool mUseEllipsis;
  bool mClearBg;
  nuiFont* mpFont;
  nuiOrientation mOrientation;
  bool mIgnoreState;

  nuiRect mIdealLayoutRect;
  nuiRect GetLayoutRect();

  bool mWrapping;
  bool mUnderline;
  bool mStrikeThrough;

  bool mTextColorSet;
  bool mBackColorSet;
  nuiColor mTextColor;
  nuiColor mBackColor;
  
  nuiEventSink<nuiLabel> mLabelSink;
  void _SetFont(const nglString& rFontSymbol);
  const nglString& _GetFont() const;
  
  nuiPosition mTextPosition;
};

#endif // __nuiLabel_h__
