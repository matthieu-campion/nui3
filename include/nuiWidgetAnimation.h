/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWidgetAnimation_h__
#define __nuiWidgetAnimation_h__

//#include "nui.h"
#include "nuiWidget.h"
#include "nuiAnimation.h"

/// nuiAnimView is a widget container that keep all its children at their original position and size.
class NUI_API nuiWidgetAnimation : public nuiAnimation
{
public:
  nuiWidgetAnimation(nuiWidgetPtr pTarget, double DurationInSeconds);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiWidgetAnimation();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode = true) const;

protected:
  nuiWidgetPtr mpTarget;
  double mDuration;
};

class NUI_API nuiMoveRectAnim : public nuiWidgetAnimation
{
public:
  nuiMoveRectAnim (nuiWidgetPtr pTarget, double DurationInSeconds, const nuiRect& rSource, const nuiRect& rDestination);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode = true) const;
  virtual void OnFrame();
protected:
  nuiRect mSource;
  nuiRect mDestination;
};

class NUI_API nuiMoveToRectAnim : public nuiMoveRectAnim
{
public:
  nuiMoveToRectAnim (nuiWidgetPtr pTarget, double DurationInSeconds, const nuiRect& rDestination);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
};

class NUI_API nuiMoveAnim : public nuiMoveRectAnim
{
public:
  nuiMoveAnim (nuiWidgetPtr pTarget, double DurationInSeconds, nuiSize X1, nuiSize Y1, nuiSize X2, nuiSize Y2);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
  void OnFrame();
protected:
  nuiSize mX1,mY1;
  nuiSize mX2,mY2;
};

class NUI_API nuiMoveToAnim : public nuiMoveAnim
{
public:
  nuiMoveToAnim (nuiWidgetPtr pTarget, double DurationInSeconds, nuiSize X, nuiSize Y);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  
  void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
};

class NUI_API nuiScaleToWidgetAnim : public nuiMoveRectAnim
{
public:
  nuiScaleToWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, float Ratio, nuiPosition ScaleBarycenterPosition = nuiCenter);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
protected:
  float mRatio;
  nuiPosition mPosition;
};

class NUI_API nuiScaleFromWidgetAnim : public nuiMoveRectAnim
{
public:
  nuiScaleFromWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, float Ratio, nuiPosition ScaleBarycenterPosition = nuiCenter);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
protected:
  float mRatio;
  nuiPosition mPosition;
};

/////// WIDGET MATRIX ANIMATIONS:
class NUI_API nuiScaleMatrixWidgetAnim : public nuiWidgetAnimation
{
public:
  nuiScaleMatrixWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, float SourceRatio, float DestinationRatio, nuiPosition ScaleBarycenterPosition = nuiCenter);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
  virtual void OnFrame();
protected:
  float mSourceRatio;
  float mDestinationRatio;
  nuiPosition mPosition;
  float mX;
  float mY;
};

class NUI_API nuiScaleToMatrixWidgetAnim : public nuiScaleMatrixWidgetAnim
{
public:
  nuiScaleToMatrixWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, float DestinationRatio, nuiPosition ScaleBarycenterPosition = nuiCenter);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
};

class NUI_API nuiScaleFromMatrixWidgetAnim : public nuiScaleMatrixWidgetAnim
{
public:
  nuiScaleFromMatrixWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, float DestinationRatio, nuiPosition ScaleBarycenterPosition = nuiCenter);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
};

/////// WIDGET ALPHA ANIMATIONS:
class NUI_API nuiFadeWidgetAnim : public nuiWidgetAnimation
{
public:
  nuiFadeWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, float SourceAlpha, float DestinationAlpha);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);
  virtual void OnFrame();
protected:
  float mSourceAlpha;
  float mDestinationAlpha;
  nuiPosition mPosition;
};

class NUI_API nuiFadeInWidgetAnim : public nuiFadeWidgetAnim
{
public:
  nuiFadeInWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, bool StartFromCurrentAlpha);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);

private:
  bool mStartFromCurrentAlpha;
};

class NUI_API nuiFadeOutWidgetAnim : public nuiFadeWidgetAnim
{
public:
  nuiFadeOutWidgetAnim (nuiWidgetPtr pTarget, double DurationInSeconds, bool StartFromCurrentAlpha);
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward);

private:
  bool mStartFromCurrentAlpha;
};

#endif // __nuiAnimation_h__
