/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiTexture;

class nuiSpriteFrame
{
public:
  nuiSpriteFrame();
  virtual ~nuiSpriteFrame();
  
  bool SetTexture(nuiTexture* pTexture, const nuiRect& rRect);
  bool SetTexture(const nglPath& rPath, const nuiRect& rRect);
  bool SetTexture(const nglPath& rPath);

  void SetRect(const nuiRect& rRect);
  void SetHandle(float x, float y);
  
  const nglPath& GetTexturePath() const;
  nuiTexture* GetTexture() const;
  const nuiRect& GetRect();
  
  float GetHandleX() const;
  float GetHandleY() const;

protected:
  nuiTexture* mpTexture;
  nuiRect mRect; ///< Rectangle inside mpTexture occupied by the sprite pixels
  float mX, mY; ///< Handle position
};

class nuiSpriteDef
{
public:
  nuiSpriteDef();
  virtual ~nuiSpriteDef();
  
  
};

class nuiSprite : public nuiObject
{
public:
  nuiSprite();
  virtual ~nuiSprite();

  const nuiSpriteDef& GetDefinition() const;
  
  //! State management
  int32 GetStateCount() const;
  const nglString& GetStateName(int32 index) const;

  int32 GetState(int32 index) const;
  void SetState(int32 index);
  
};


class nuiSpriteView : public nuiSimpleContainer
{
public:
  nuiSpriteView();
  virtual ~nuiSpriteView();
  
  void AddSprite(nuiSprite* pSprite);
  void DelSprite(nuiSprite* pSprite);
  
};