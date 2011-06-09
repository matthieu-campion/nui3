/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once


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
  
  const std::vector<nuiTextGlyph>& GetGlyphs() const;
  nuiFont* GetFont() const;
  
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  
  nuiRect GetRect() const;
  
private:
  friend class nuiTextLayout;
  friend class nuiFontBase;
  nuiTextStyle mStyle;
  const nuiTextLayout& mLayout;
  int32 mPosition;
  int32 mLength;
  nuiUnicodeScript mScript;
  
  bool mUnderline;
  bool mStrikeThrough;
  
  std::vector<nuiTextGlyph> mGlyphs;
  float mAdvanceX, mAdvanceY;
};

