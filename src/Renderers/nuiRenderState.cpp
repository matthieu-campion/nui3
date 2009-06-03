/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiRenderState.h"
#include "nuiFont.h"
#include "nuiTexture.h"
#include "nuiSurface.h"


nuiRenderState::nuiRenderState()
{
  mBlending = false;
  mTexturing = false;
  mColorBuffer = true;
  mDepthTest = false;
  mDepthWrite = false;

  mLineCap = nuiLineCapBut;
  mLineJoin = nuiLineJoinBevel;

  mBlendFunc = nuiBlendTransp;

  mpTexture = NULL;

  mAntialiasing = false;
  mWinding = nuiShape::eNonZero;
  mLineWidth = 1.0f;

  mpFont = NULL;
  mpShader = NULL;

}

nuiRenderState::nuiRenderState(const nuiRenderState& rState)
{
  mpTexture = NULL;
  mpFont = NULL;
  mpShader = NULL;

  Copy(rState);
}

nuiRenderState& nuiRenderState::operator =(const nuiRenderState& rState)
{
  Copy(rState);
  return *this;
}

void nuiRenderState::Copy(const nuiRenderState& rState)
{
  mBlending       = rState.mBlending;
  mTexturing      = rState.mTexturing;
  mColorBuffer    = rState.mColorBuffer;
  mDepthTest      = rState.mDepthTest;
  mDepthWrite     = rState.mDepthWrite;
  mWinding        = rState.mWinding;
  mAntialiasing   = rState.mAntialiasing;
  mLineWidth      = rState.mLineWidth;
  mBlendFunc      = rState.mBlendFunc;
  mTextColor      = rState.mTextColor;
  mClearColor     = rState.mClearColor;
  mStrokeColor    = rState.mStrokeColor;
  mFillColor      = rState.mFillColor;
  mLineCap        = rState.mLineCap;
  mLineJoin       = rState.mLineJoin;


  nuiTexture* pOldTexture = mpTexture;
  mpTexture = rState.mpTexture;
  if (mpTexture)
    mpTexture->Acquire();
  if (pOldTexture)
    pOldTexture->Release();

  nuiFont* pOldFont = mpFont;
  mpFont = rState.mpFont;
  if (mpFont)
    mpFont->Acquire();
  if (pOldFont)
    pOldFont->Release();

  mpShader = rState.mpShader;
}

nuiRenderState::~nuiRenderState()
{
  if (mpTexture)
    mpTexture->Release();

  if (mpFont)
    mpFont->Release();
}

bool nuiRenderState::operator==(const nuiRenderState& rState) const
{
  bool state =
    (mBlending       == rState.mBlending)         &&
    (mTexturing      == rState.mTexturing)        &&
    (mColorBuffer    == rState.mColorBuffer)      &&
    (mDepthTest      == rState.mDepthTest)        &&
    (mDepthWrite     == rState.mDepthWrite)       &&
    (mWinding        == rState.mWinding)          &&
    (mAntialiasing   == rState.mAntialiasing)     &&
    (mLineWidth      == rState.mLineWidth)        &&
    (mBlendFunc      == rState.mBlendFunc)        &&
    (mTextColor      == rState.mTextColor)        &&
    (mClearColor     == rState.mClearColor)       &&
    (mStrokeColor    == rState.mStrokeColor)      &&
    (mFillColor      == rState.mFillColor)        &&
    (mLineCap        == rState.mLineCap)          &&
    (mLineJoin       == rState.mLineJoin)         &&
    (mpTexture       == rState.mpTexture)         &&
    (mpFont          == rState.mpFont)            &&
    (mpShader        == rState.mpShader);
  
  return state;
}
