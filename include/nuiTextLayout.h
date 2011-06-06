/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiFont.h"
#include "nuiUnicode.h"
void TextLayoutTest(const nglString& txt);

class nuiTextLayout;

class nuiTextGlyph
{
public:
  int32 mIndex;
  int32 mCluster;
  float mX, mY;
};

class nuiTextRun : public nuiRefCount
{
public:
  nuiTextRun(const nuiTextLayout& rLayout, nuiUnicodeScript Script, int32 Position, int32 Length);
  virtual ~nuiTextRun();
  void SetFont(nuiFont* pFont);
  
  nuiUnicodeScript GetScript() const;
  int32 GetPosition() const;
  const nglUChar* GetUnicodeChars() const;
  int32 GetLength() const;
  float GetAdvanceX() const;
  float GetAdvanceY() const;
  const std::vector<nuiTextGlyph>& GetGlyphs() const;
  nuiFont* GetFont() const;
  
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  
private:
  friend class nuiTextLayout;
  friend class nuiFontBase;
  nuiFont* mpFont;
  const nuiTextLayout& mLayout;
  int32 mPosition;
  int32 mLength;
  nuiUnicodeScript mScript;
  
  bool mUnderline;
  bool mStrikeThrough;

  std::vector<nuiTextGlyph> mGlyphs;
  float mAdvanceX, mAdvanceY;
};

class nuiTextLine
{
public:
  nuiTextLine(const nuiTextLayout& rLayout, float X, float Y);
  
  virtual ~nuiTextLine();
  
  /** @name Drawing the Line */
  //@{
  void Draw(nuiDrawContext* pContext);
  //@}
  
  int32 GetRunCount() const;
  nuiTextRun* GetRun(int32 index) const;
  
  const std::vector<nuiTextRun*>& GetGlyphRuns() const;
  float GetX() const;
  float GetY() const;
  void SetPosition(float X, float Y);
  
  float GetAdvanceX() const;
  float GetAdvanceY() const;
private:
  friend class nuiTextLayout;
  
  void AddRun(nuiTextRun* pRun);
  
  std::vector<nuiTextRun*> mpRuns;
  const nuiTextLayout& mLayout;
  float mX, mY;
};



class nuiTextLayout : public nuiRefCount
{
public:
  nuiTextLayout(nuiFont* pFont);
  virtual ~nuiTextLayout();

  bool LayoutText(const nglString& rString);
  
  void SetJustification(bool set);
  bool GetJustification() const;
  
  void SetFlush(float set);
  float GetFlush() const;
  
  int32 GetParagraphCount() const;
  int32 GetLineCount(int32 Paragraph) const;
  int32 GetRunCount(int32 Paragraph, int32 Line) const;
  
  nuiTextLine* GetLine(int32 Paragraph, int32 Line) const;
  nuiTextRun*  GetRun(int32 Paragraph, int32 Line, int32 Run) const;
  
  const nglUChar* GetUnicodeChars() const;

  /** @name Metrics */
  //@{
  int32  GetMetrics (nuiGlyphInfo& rInfo) const;
  /*!< Returns whole layout metrics (as a composite glyph)
   \param rInfo metrics info holder
   \return number of glyphs in the layout
   */
  float GetAscender() const;
  float GetDescender() const;
  //@}
  
  int32  GetGlyphCount() const;
  const nuiGlyphLayout* GetGlyph   (uint32 Offset) const;
  const nuiGlyphLayout* GetGlyphAt (float X, float Y) const;
  /*!< Identify a glyph at given coordinates
   \param X abscissa in layout coordinate space
   \param Y ordinate in layout coordinate space
   \return located glyph instance, NULL if none match
   
   This method searches if a glyph's bounding box in the layout encloses the
   given coordinates. If such a glyph is found, its localized instance is
   returned.
   */
  nuiRect GetRect() const;
  void SetDensity(nuiSize X, nuiSize Y);
  nuiSize GetDensityX() const;
  nuiSize GetDensityY() const;
  //@}
  
  bool AddDummyGlyph(int32 ReferencePos, void* pUserPointer, float W, float H);
  
  void SetSpacesPerTab(int count);
  int32 GetSpacesPerTab();
  
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  
  void SetWrapX(nuiSize WrapX);
  nuiSize GetWrapX() const;
  
private:
  nuiFont* mpFont;
  std::map<nuiUnicodeScript, std::set<nglUChar> > mCharsets;
  
  bool mJustify;
  float mFlush;
  int32 mSpacePerTab;
  float mXDensity;
  float mYDensity;
  nuiRect mRect;
  
  bool LayoutParagraph(int32 start, int32 length);
  
  typedef std::vector<nuiTextLine*> Paragraph;
  std::vector<Paragraph*> mpParagraphs;
  
  std::vector<nglUChar> mUnicode;
  std::vector<int32> mOffsetInString;
  std::vector<int32> mOffsetInUnicode;
};

