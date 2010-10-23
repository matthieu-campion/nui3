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
  
  nuiTexture* GetTexture() const;
  const nuiRect& GetRect();
  
  float GetHandleX() const;
  float GetHandleY() const;

protected:
  nuiTexture* mpTexture;
  nuiRect mRect; ///< Rectangle inside mpTexture occupied by the sprite pixels
  float mX, mY; ///< Handle position
};

class nuiSpriteAnimation
{
public:
  nuiSpriteAnimation();
  virtual ~nuiSpriteAnimation();
  
  int32 GetFrameCount() const;
  void AddFrame(nuiSpriteFrame* pFrame);
  const nuiSpriteFrame* GetFrame(int32 index) const;
  
  void SetDuration(float seconds);
  void SetFPS(float FPS);
  float GetDuration() const;
  float GetFPS() const;
  
  void SetName(const nglString& rName);
  const nglString& GetName() const;
  
protected:
  nglString mName;
  std::vector<nuiSpriteFrame*> mpFrames;
  float mFPS;
};

class nuiSpriteDef
{
public:
  nuiSpriteDef();
  virtual ~nuiSpriteDef();
  
  void AddAnimation(nuiSpriteAnimation* pAnim);
  int32 GetAnimationCount() const;
  const nuiSpriteAnimation* GetAnimation(int32 index) const;
  
protected:
  std::vector<nuiSpriteAnimation*> mpAnimations;
  
};

class nuiSprite : public nuiObject
{
public:
  nuiSprite(nuiSpriteDef* pSpriteDef);
  virtual ~nuiSprite();

  const nuiSpriteDef& GetDefinition() const;
  
  //! State management
  int32 GetStateCount() const;
  const nglString& GetStateName(int32 index) const;

  int32 GetState() const;
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