/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiTextStyle.h"
#include "nuiUnicode.h"

class nuiTextGlyph
{
public:
  int32 mIndex; ///< Glyph index in the font
  int32 mCluster; ///< Position of the source char in the original unicode code point vector
  float mX, mY; ///< Position on screen
  nuiTexture* mpTexture; ///< Texture that contains this glyph
  nuiRect mDestRect; ///< Destination rectangle of the laid ou glyph
  nuiRect mSourceRect; ///< Rectangle of the glyph in the texture
};



class nuiTextRun : public nuiRefCount
{
public:
  nuiTextRun(const nuiTextLayout& rLayout, nuiUnicodeScript Script, int32 Position, int32 Length, const nuiTextStyle& rStyle);
  virtual ~nuiTextRun();
  void SetFont(nuiFont* pFont);
  
  nuiUnicodeScript GetScript() const;
  int32 GetPosition() const;
  const nglUChar* GetUnicodeChars() const;
  int32 GetLength() const;
  
  float GetAdvanceX() const;
  float GetAdvanceY() const;
  
  float GetAscender() const;
  float GetDescender() const;
  
  std::vector<nuiTextGlyph>& GetGlyphs();
  const nuiTextGlyph* GetGlyph   (int32 Offset) const;
  const nuiTextGlyph* GetGlyphAt (float X, float Y) const;

  int32 GetGlyphCount() const;
  nuiFont* GetFont() const;
  
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  
  nuiRect GetRect() const;
  
  bool IsPrepared() const;
  void SetPrepared(bool set);
private:
  friend class nuiTextLayout;
  friend class nuiFontBase;
  nuiTextStyle mStyle;
  const nuiTextLayout& mLayout;
  int32 mPosition;
  int32 mLength;
  nuiUnicodeScript mScript;
  
  bool mUnderline : 1;
  bool mStrikeThrough : 1;
  
  std::vector<nuiTextGlyph> mGlyphs;
  float mX;
  float mY;
  float mAdvanceX;
  float mAdvanceY;
};

