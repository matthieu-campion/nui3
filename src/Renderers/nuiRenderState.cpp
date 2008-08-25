/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiRenderState.h"
#include "nuiFont.h"
#include "nuiTexture.h"



nuiRenderState::nuiRenderState()
{
  mBlending = false;
  mTexturing = false;
  mColorBuffer = true;
  mStencilBuffer = false;

  mLineCap = nuiLineCapBut;
  mLineJoin = nuiLineJoinBevel;

  mBlendFunc = nuiBlendTransp;

  mpTexture = NULL;

  mAntialiasing = false;
  mWinding = nuiShape::eNonZero;
  mLineWidth = 1.0f;

  mpFont = NULL;
  mpShader = NULL;

  mStencilValue = 1;
  mStencilMode = nuiIgnoreStencil;
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
  mStencilBuffer  = rState.mColorBuffer;
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


  mStencilValue = rState.mStencilValue;
  mStencilMode = rState.mStencilMode;

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
    (mBlending       == rState.mBlending)     &&
    (mTexturing      == rState.mTexturing)    &&
    (mColorBuffer    == rState.mColorBuffer)  &&
    (mStencilBuffer  == rState.mColorBuffer)  &&
    (mWinding        == rState.mWinding)      &&
    (mAntialiasing   == rState.mAntialiasing) &&
    (mLineWidth      == rState.mLineWidth)    &&
    (mBlendFunc      == rState.mBlendFunc)    &&
    (mTextColor      == rState.mTextColor)    &&
    (mClearColor     == rState.mClearColor)   &&
    (mStrokeColor    == rState.mStrokeColor)  &&
    (mFillColor      == rState.mFillColor)    &&
    (mLineCap        == rState.mLineCap)      &&
    (mLineJoin       == rState.mLineJoin)     &&
    (mStencilValue   == rState.mStencilValue) &&
    (mStencilMode    == rState.mStencilMode)  &&
    (mpTexture       == rState.mpTexture)     &&
    (mpFont          == rState.mpFont)        &&
    (mpShader        == rState.mpShader);
  
  return state;
}
