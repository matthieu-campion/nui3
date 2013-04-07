/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiSignalsSlots.h"

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
  const nuiRect& GetRect() const;
  
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
  nuiSpriteAnimation(const nglPath& rPath);
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

class nuiSpriteDef : public nuiObject
{
public:
  nuiSpriteDef(const nglString& rObjectName);
  nuiSpriteDef(const nglPath& rObjectPath);
  virtual ~nuiSpriteDef();
  
  void AddAnimation(nuiSpriteAnimation* pAnim);
  int32 GetAnimationCount() const;
  const nuiSpriteAnimation* GetAnimation(int32 index) const;
  int32 GetAnimation(const nglString& rName) const;
  
  static nuiSpriteDef* GetSprite(const nglString& rSpriteDefName);
  
protected:
  static std::map<nglString, nuiSpriteDef*> mSpriteMap;
  void Init();
  static void Uninit();
  std::vector<nuiSpriteAnimation*> mpAnimations;
};

class nuiSprite : public nuiObject
{
  friend class nuiSpriteDef;
  
public:
  nuiSprite(const nglString& rSpriteDefName);
  nuiSprite(const nglPath& rSpriteDefPath, bool forceReplace);
  nuiSprite(nuiSpriteDef* pSpriteDef);
  virtual ~nuiSprite();

  const nuiSpriteDef* GetDefinition() const;

  void AddChild(nuiSprite* pChild);
  void DelChild(nuiSprite* pChild);
  nuiSprite* GetParent() const;
  
  /** @name Matrix Transformation Support */
  //@{
  void AddMatrixNode(nuiMatrixNode* pNode);
  void DelMatrixNode(uint32 index);
  int32 GetMatrixNodeCount() const;
  nuiMatrixNode* GetMatrixNode(uint32 index) const;

  void LoadIdentityMatrix(); ///< Beware: you can't use Get/SetPosition/Angle/X or Y after that as the matrixes they use will gone.
  bool IsMatrixIdentity() const;
  void GetMatrix(nuiMatrix& rMatrix) const;
  nuiMatrix GetMatrix() const;
  void SetMatrix(const nuiMatrix& rMatrix);
  //@}
  
  void Draw(nuiDrawContext* pContext);

  void Animate(float passedtime);

  void SetAnimation(const nglString& rAnimationName);
  void SetAnimation(int32 index);
  const nglString& GetCurrentAnimationName() const;

  void SetFrameTime(float framepos);
  float GetFrameTime() const;
  
  float GetSpeed() const;
  void SetSpeed(float speed); ///< Default is 1

  void SetPosition(float X, float Y);
  void SetX(float X);
  void SetY(float Y);
  float GetX() const;
  float GetY() const;

  float GetAngle() const;
  void SetAngle(float angle);
  const nglVectorf& GetPivot() const;
  void SetPivot(const nglVectorf& rPivot);

  
  float GetScaleX() const;
  float GetScaleY() const;
  float GetScale() const;
  void SetScaleX(float value);
  void SetScaleY(float value);
  void SetScale(float value);
  
  void SetColor(const nuiColor& rColor);
  const nuiColor& GetColor() const;

  void SetAlpha(float Alpha);
  float GetAlpha() const;
  
  void SetBlendFunc(nuiBlendFunc f);
  nuiBlendFunc GetBlendFunc() const;
  void SetBlending(bool value);
  bool GetBlending() const;

  nuiSignal2<nuiSprite*, const nglMouseInfo&, bool> MouseClicked;
  nuiSignal2<nuiSprite*, const nglMouseInfo&, bool> MouseUnclicked;
  nuiSignal2<nuiSprite*, const nglMouseInfo&, bool> MouseMoved;
  
  nuiSimpleEventSource<0> AnimEnd;
  
  void GetSpritesAtPoint(float x, float y, std::vector<nuiSprite*>& rSprites);
protected:
              
  void Init();
  void InitAttributes();

  static nuiMatrix mIdentityMatrix;
  std::vector<nuiMatrixNode*>* mpMatrixNodes;
  std::vector<nuiSprite*> mpChildren;
  void SetParent(nuiSprite* pParent);

  nuiSprite* mpParent;
  nuiSpriteDef* mpSpriteDef;
  int32 mCurrentAnimation;
  float mCurrentFrame;
  float mSpeed;
  float mScale;
  float mScaleX;
  float mScaleY;
  
  nuiMatrixNode_Scale* mpScale;
  nuiMatrixNode_Translation* mpPosition;
  nuiMatrixNode_Pivot* mpPivot;

  nuiColor mColor;
  float mAlpha;
  nuiBlendFunc mBlendFunc;
  bool mBlending;
private:
  
  void _SetAnimation(const nglString& rAnimationName);
  static uint32 mSpriteCounter;

};


class nuiSpriteView : public nuiSimpleContainer
{
public:
  nuiSpriteView();
  virtual ~nuiSpriteView();
  
  void AddSprite(nuiSprite* pSprite);
  void DelSprite(nuiSprite* pSprite);

  nuiRect CalcIdealRect();

  bool Draw(nuiDrawContext* pContext);

  void GetSpritesAtPoint(float x, float y, std::vector<nuiSprite*>& rSprites);
  const std::vector<nuiSprite*>& GetSprites() const;
  int32 GetSpriteCount() const;
  
  bool MouseClicked(const nglMouseInfo& rEvent);
  bool MouseUnclicked(const nglMouseInfo& rEvent);
  bool MouseMoved(const nglMouseInfo& rEvent);
  
protected:
  std::vector<nuiSprite*> mpSprites;
  double mLastTime;
};


