/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTheme_h__
#define __nuiTheme_h__

//#include "nui.h"
#include "nglPath.h"
#include "nuiObject.h"
#include "nuiWidgetElements.h"

class nuiDrawContext;
class nuiWindow;
class nuiTab;
class nuiButton;
class nuiScrollBar;
class nuiSlider;
class nuiToggleButton;
class nuiContainer;
class nuiDialog;

class nuiFont;
class nuiSplitter;
class nuiLabel;

/// This class implements the nui theme system.
class NUI_API nuiTheme: public nuiObject
{
public:
  static nuiTheme* GetTheme(); ///< Return the default theme. If no theme exist one is created with the resource path set as the current working directory.
  static void SetTheme(nuiTheme* pTheme); ///< Replace the default theme with the given one. The old theme is released (and will die gracefully if not acquired by another object).

  nuiTheme();
  virtual ~nuiTheme();

  enum FontStyle
  {
    Default =0,
    Fixed =1,
    StyleCount = 2
  };

  // Window decorations:
  virtual void DrawWindowShade (nuiDrawContext* pContext, const nuiRect& rRect, const nuiColor& rColor, bool Active);
  virtual void DrawWindow(nuiDrawContext* pContext, nuiWindow* pWindow); ///< Render a normal window border
  virtual void DrawActiveWindow(nuiDrawContext* pContext, nuiWindow* pWindow); ///< Render a Top Level window border
  virtual void DrawMovingWindow(nuiDrawContext* pContext, nuiWindow* pWindow);
  virtual void AdjustWindowRect(nuiRect& rRect, nuiWindowFlags Flags, bool RectIsClient = true, bool IncludeShadow = false);
  virtual bool IsInsideWindow(nuiRect Rect,nuiSize X,nuiSize Y,nuiWindowFlags Flags);
  virtual nuiPosition GetWindowPart(nuiRect rect,nuiSize X,nuiSize Y,nuiWindowFlags Flags, bool RectIsClient = true);
  virtual void DrawWindowBackground(nuiDrawContext* pContext, nuiWindow* pWindow, bool Active);

  // Scrollbar:
  virtual void DrawScrollBarBackground(nuiDrawContext* pContext, nuiScrollBar* pScroll);
  virtual void DrawScrollBarForeground(nuiDrawContext* pContext, nuiScrollBar* pScroll);

  // Slider:
  virtual void DrawSliderBackground(nuiDrawContext* pContext, nuiSlider* pScroll);
  virtual void DrawSliderForeground(nuiDrawContext* pContext, nuiSlider* pScroll);


  // CheckBox:
  virtual void DrawCheckBox(nuiDrawContext* pContext, nuiToggleButton* pButton);  ///< Render a CheckBox

  // Menus a Menu items:
  virtual void DrawMenuWindow(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidget* pWidget = NULL); ///< draw the a menu window (border, shadow, etc...).
  virtual void DrawMenuItem(nuiDrawContext* pContext, const nuiRect& rRect, bool Hover); ///< Draw the background of a menu item.

  // Tree Elements:
  virtual void DrawTreeHandle(nuiDrawContext* pContext, const nuiRect& rRect, bool IsOpened, nuiSize TreeHandleSize, const nuiColor& rColor = nuiColor(0,0,0)); ///< Draw the typical nuiTreeView node widget as opened or closed inside the given rectangle.

  // Generic Selection Rectangles:
  virtual void DrawSelectionBackground(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidget* pWidget = NULL);
  virtual void DrawSelectionForeground(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidget* pWidget = NULL);
  virtual void DrawMarkee(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidgetElement Color); ///< Draw a markee selection rectangle around the givent frame with the correct color.

  // Element Colors:
  const nuiColor& GetElementColor(nuiWidgetElement Element) const;
  void SetElementColor(nuiWidgetElement Element,const nuiColor& rColor);

  // Fonts
  nuiFont* GetFont(FontStyle Style); ///< Retreive the font used for the given style. You must release the font once you used it!!!
  void SetFont(FontStyle Style, nuiFont* pFont); ///< Changes the font used for the given style. If Delete==true the old one will be deleted.

  // Dialog Creation
  virtual nuiDialog*  CreateDefaultDialog(nuiContainer* pParent);

  virtual nuiLabel *CreateTreeNodeLabel(const nglString &text);

protected:
  friend class nuiDrawContext;
  // Fonts and text handling:

  void LoadDefaults(); ///< Load the default color & geometry values.

  nglString mFonts[StyleCount];
  nuiFont*  mpFonts[StyleCount];

  nuiColor mTableColumnBG;
  nuiColor mSelectedTableColumnBG;
  nuiColor mSelectedTableLineBG;

  nuiColor mButtonBorder[2][2];
  nuiColor mButtonFill[2][2];

  nuiColor mTabBorder[2][2];
  nuiColor mTabFill[2][2];

  nuiColor mElementColors[eLastWidgetElement];

  static nuiTheme* mpTheme;

  nuiFont* mpWindowTitleFont;
};

#endif // __nuiTheme_h__

