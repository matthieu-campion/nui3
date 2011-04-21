//
//  nuiTextLayout.h
//  TextEdit
//
//  Created by Sébastien Métrot on 4/21/11.
//  Copyright 2011 MXP4. All rights reserved.
//

#pragma once

#include "nui.h"
#include "nuiFont.h"
#include "nuiUnicode.h"
void TextLayoutTest(const nglString& txt);

class nuiTextRun
{
public:
  nuiTextRun(nuiFont* pFont, const nglString& rString, int32 Position, int32 Length);
  
private:
  friend class nuiTextLayout;
  nuiFont* mpFont;
  const nglString& mString;
  int32 mPosition;
  int32 mLength;
  
  void AddGlyph(nuiFont* pFont, int32 Index, float X, float Y);
  
private:
  std::vector<nuiGlyphLayout> mGlyphs;
};

class nuiTextLine
{
public:
  nuiTextLine(float X, float Y);
  
  virtual ~nuiTextLine();
  
  /** @name Drawing the Line */
  //@{
  void Draw(nuiDrawContext* pContext);
  //@}
  
  const std::vector<nuiTextRun*>& GetGlyphRuns() const;
  float GetX() const;
  float GetY() const;
  void SetPosition(float X, float Y);
private:
  friend class nuiTextLayout;
  
  void AddRun(nuiTextRun* pRun);
  
  std::vector<nuiTextRun*> mRuns;
  float mX, mY;
};



class nuiTextLayout
{
public:
  nuiTextLayout(nuiFont* pFont);

  virtual ~nuiTextLayout();
  
private:
  std::vector<nuiFont*> mpFonts;
};

