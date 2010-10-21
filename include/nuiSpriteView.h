/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiSprite : public nuiObject
{
public:
  nuiSprite();
  virtual ~nuiSprite();

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