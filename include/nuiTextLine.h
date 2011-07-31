/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiTextLine
{
public:
  nuiTextLine(const nuiTextLayout& rLayout, float X, float Y);
  
  virtual ~nuiTextLine();
  
  int32 GetRunCount() const;
  nuiTextRun* GetRun(int32 index) const;
  
  const std::vector<nuiTextRun*>& GetGlyphRuns() const;
  float GetX() const;
  float GetY() const;
  void SetPosition(float X, float Y);
  
  float GetAdvanceX() const;
  float GetAdvanceY() const;
  
  float GetAscender() const;
  float GetDescender() const;
  
  nuiRect GetRect() const;
  
  int32 GetGlyphCount() const;
  
  const nuiTextGlyph* GetGlyph   (int32 Offset) const;
  const nuiTextGlyph* GetGlyphAt (float X, float Y) const;
private:
  friend class nuiTextLayout;
  
  void AddRun(nuiTextRun* pRun);
  
  std::vector<nuiTextRun*> mpRuns;
  const nuiTextLayout& mLayout;
  float mX, mY;
};

