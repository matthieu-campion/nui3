/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRenderState_h__
#define __nuiRenderState_h__

//#include "nui.h"
#include "nuiShape.h"

class nuiTexture;
class nuiFont;
class nuiShader;

enum nuiBlendFunc
{
  nuiBlendSource,        //disable blend
  nuiBlendTransp,        //GL_SRC_ALPHA             GL_ONE_MINUS_SRC_ALPHA
  nuiBlendClear,         //GL_ZERO                  GL_ZERO
  nuiBlendDest,          //GL_ZERO                  GL_ONE
  nuiBlendOver,          //GL_ONE                   GL_ONE_MINUS_SRC_ALPHA
  nuiBlendOverRev,       //GL_ONE_MINUS_DST_ALPHA   GL_ONE
  nuiBlendIn,            //GL_DST_ALPHA             GL_ZERO
  nuiBlendInRev,         //GL_ZERO                  GL_SRC_ALPHA
  nuiBlendOut,           //GL_ONE_MINUS_DST_ALPHA   GL_ZERO
  nuiBlendOutRev,        //GL_ZERO                  GL_ONE_MINUS_SRC_ALPHA
  nuiBlendTop,           //GL_DST_ALPHA             GL_ONE_MINUS_SRC_ALPHA
  nuiBlendTopRev,        //GL_ONE_MINUS_DST_ALPHA   GL_SRC_ALPHA
  nuiBlendXOR,           //GL_ONE_MINUS_DST_ALPHA   GL_ONE_MINUS_SRC_ALPHA
  nuiBlendAdd,           //GL_ONE                   GL_ONE
  nuiBlendSaturate,      //GL_SRC_ALPHA_SATURATE    GL_ONE

  nuiBlendTranspClear,   //GL_SRC_ALPHA             GL_ZERO
  nuiBlendTranspAdd,     //GL_SRC_ALPHA             GL_ONE
  nuiBlendTranspOver,    //GL_SRC_ALPHA             GL_ONE_MINUS_SRC_ALPHA
  nuiBlendTranspInRev    //GL_SRC_ALPHA             GL_SRC_ALPHA
  
};

enum nuiCulling
{
  eCullingBack = GL_BACK,
  eCullingFront = GL_FRONT,
  eCullingBoth = GL_FRONT_AND_BACK
};


void nuiGetBlendFuncFactors(nuiBlendFunc Func, GLenum& src, GLenum& dst);

#define NUI_MAX_TEXTURE_UNITS 8

class NUI_API nuiRenderState
{
public:
  nuiRenderState();
  nuiRenderState(const nuiRenderState& rState); ///< Copy constructor.
  nuiRenderState& operator=(const nuiRenderState& rState); ///< Copy the given render state in this object.
  virtual ~nuiRenderState();

  void Copy(const nuiRenderState& rState); ///< Copy the given render state in this object.

  bool operator == (const nuiRenderState& rState) const;
  
  bool mBlending;
  nuiBlendFunc mBlendFunc;

  bool mDepthTest;
  bool mDepthWrite;

  bool mCulling;
  nuiCulling mCullingMode;
  
  bool mTexturing;
  nuiTexture* mpTexture[NUI_MAX_TEXTURE_UNITS];
  nuiShaderProgram* mpShader;
  nuiShaderState* mpShaderState;

  // Rendering buffers:
  bool mColorBuffer;

  // Actual drawing params:
  nuiColor mStrokeColor;
  nuiColor mFillColor;
  nuiSize mLineWidth;
  nuiShape::Winding mWinding;

  nuiColor mClearColor;
  float mClearDepth;
  nuiColor mTextColor;
  nuiFont* mpFont;

  nuiLineCap mLineCap;
  nuiLineJoin mLineJoin;

  bool mAntialiasing;
};

#endif // __nuiRenderState_h__
