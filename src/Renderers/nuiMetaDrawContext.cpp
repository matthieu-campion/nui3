/*
NUI - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sébastien Métrot

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nuiMetaDrawContext.h"

/// nuiPainter:
nuiPainter::State::State()
: mClip(false),
  mClipRect(),
  mClipShape(false),
  mBlending(false),
  mTexturing(false),
  mpTexture(NULL),
  mpFont(NULL),
  mpShader(NULL),
  mMatrix(),
  mClearColor(),
  mFillColor(),
  mStrokeColor(),
  mTextColor(),
  mLineWidth(1.0f),
  mAntiAliasing(false),
  mWinding(nuiShape::eNone),
  mBlendFunc(nuiBlendSource)
{
}

nuiPainter::State::State(const State& rState)
: mClip(rState.mClip),
  mClipRect(rState.mClipRect),
  mClipShape(rState.mClipShape),
  mBlending(rState.mBlending),
  mTexturing(rState.mTexturing),
  mpTexture(rState.mpTexture),
  mpFont(rState.mpFont),
  mpShader(rState.mpShader),
  mMatrix(rState.mMatrix),
  mClearColor(rState.mClearColor),
  mFillColor(rState.mFillColor),
  mStrokeColor(rState.mStrokeColor),
  mTextColor(rState.mTextColor),
  mLineWidth(rState.mLineWidth),
  mAntiAliasing(rState.mAntiAliasing),
  mWinding(rState.mWinding),
  mBlendFunc(rState.mBlendFunc)
{
  // TODO: Acquire resources
}

nuiPainter::State::~State()
{
  // TODO: Release resources
}

// nuiPainter:
nuiPainter::nuiPainter(const nuiRect& rRect)
{
  mRect = rRect;
}

nuiPainter::~nuiPainter()
{ 
}

#if 0
// Rendering sessions:
void nuiPainter::StartRendering
(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
}

void nuiPainter::BeginSession()
{ 
}

void nuiPainter::StopRendering()
{
}

// Clipping support
void nuiPainter::PushClipping()
{

}

void nuiPainter::PopClipping(){ }
bool nuiPainter::Clip(const nuiRect& rGlobalRect){ }
bool nuiPainter::Clip(nuiWidgetPtr pWidget, const nuiRect& rLocalRect, bool IgnorePosition){ } ///< This method restricts all the rendering operation to the given local rectangle.
bool nuiPainter::Clip(nuiWidgetPtr pWidget){ } ///< This method restricts all the rendering operation to the client rectangle.

bool nuiPainter::EnableClipping(bool set){ } ///< Enables or disable clipping.
bool nuiPainter::GetClipRect(nuiRect& rRect){ } ///< Fills rRect with the current clipping rect if clipping is enabled, or with the complete Draw Context rect otherwise.
bool nuiPainter::ResetClipRect(bool EmptyClipStack){ } ///< This method reset the clipping rect to the original size of the window if EmptyClipStack is true. By default it just reverts to the last saved clipping state.

bool nuiPainter::AddClipShape(nuiShape& rShape, bool Invert){ } /// Adding the clip shape draw the shape in a stencil buffer to use it as a mask for subsequent draw operations. The shape may overwrite any existing clip shape, and then extend the drawing enabled area. Until another clip shape is set or shape clipping is reset with ResetClipShape.
bool nuiPainter::BlendClipShape(nuiShape& rShape, bool Invert){ } /// Blending the clip shape draw the shape in a stencil buffer to use it as a mask for subsequent draw operations but not erasing existing clip shapes already present. This operation can only restrict subsequent drawings, never extend the rendering area. Until another clip shape is set or shape clipping is reset with ResetClipShape.
bool nuiPainter::ResetClipShape(bool Invert){ } ///< Disable the last shape clipping that was set via AddClipShape.
//@}

/** @name Render state manipulation */
//@{
void nuiPainter::PushState(){ }
void nuiPainter::PopState(){ }
bool nuiPainter::ActivateState(const nuiRenderState& rState){ }

bool nuiPainter::ResetState(){ }
const nuiState& nuiPainter::GetState(){ }
void nuiPainter::RetrieveState(){ } ///< Mirror the real current opengl state in the current nuiRenderState object.

bool nuiPainter::EnableBlending (bool val){ }

bool nuiPainter::EnableTexturing (bool val){ }

void nuiPainter::SetBlendFunc(nuiBlendFunc Func){ }

// Texture manipulation
bool nuiPainter::IsTextureCurrent( nuiTexture* pTex , int num = 0){ }
nuiTexture* nuiPainter::GetTexture ( int num = 0){ }
bool nuiPainter::SetTexture ( nuiTexture* pTex , int num = 0){ }

// Matrix operations
void nuiPainter::PushMatrix(){ }
void nuiPainter::PopMatrix(){ }
void nuiPainter::LoadMatrix(const nglMatrixf& Matrix){ }
void nuiPainter::GetMatrix(nglMatrixf& Matrix){ }
nglMatrixf nuiPainter::GetMatrix(){ }
void nuiPainter::MultMatrix(const nglMatrixf& Matrix){ }
void nuiPainter::Translate(const nglVector2f& Vector){ }
void nuiPainter::Translate(const nglVector3f& Vector){ }
void nuiPainter::Translate(const nglVectorf& Vector){ }
void nuiPainter::Translate(nuiSize X, nuiSize Y, nuiSize Z=0.0f){ }
void nuiPainter::Scale(const nglVector2f& Vector){ }
void nuiPainter::Scale(const nglVector3f& Vector){ }
void nuiPainter::Scale(const nglVectorf& Vector){ }
void nuiPainter::Scale(nuiSize X, nuiSize Y, nuiSize Z=0.0f){ }
void nuiPainter::LoadIdentity(){ }

// Brush manipulation
void nuiPainter::SetFillColor(const nuiColor& rColor){ }
void nuiPainter::SetStrokeColor(const nuiColor& rColor){ }
const nuiColor& nuiPainter::GetFillColor(){ }
const nuiColor& nuiPainter::GetStrokeColor(){ }

void nuiPainter::SetLineWidth(nuiSize Width){ }
void nuiPainter::EnableAntiAliasing(bool set){ }
bool nuiPainter::GetAntiAliasing(){ }

// Drawing function
void nuiPainter::SetClearColor(const nuiColor& ClearColor){ } ///< Set the color used to clear the background with Clear().
void nuiPainter::DrawShape(nuiShape* pShape, nuiShapeMode Mode){ } ///< Call StrokeShape and FillShape.
void nuiPainter::DrawRect(const nuiRect& Rect, nuiShapeMode Mode){ }
void nuiPainter::DrawLine(const nglVector2f& rOrigin, const nglVector2f& rDestination){ }
void nuiPainter::DrawLine(const nglVector3f& rOrigin, const nglVector3f& rDestination){ }
void nuiPainter::DrawLine(const nglVectorf& rOrigin, const nglVectorf& rDestination){ }
void nuiPainter::DrawLine(float x1, float y1, float x2, float y2){ }
void nuiPainter::Clear(){ } ///< Initializes the current drawing buffer with the default values.
void nuiPainter::DrawImage(const nuiRect& rDest, const nuiRect& rSource){ }
void nuiPainter::DrawImageQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource){ }
void nuiPainter::DrawShade(const nuiRect& rSourceRect, const nuiRect& rShadeRect){ } ///< Draw a shade extending from SourceRect to ShadeRect. The shade may look quite diferent depending on the actual renderer.
void nuiPainter::DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, const nglVector2f& rP1, const nglVector2f& rP2){ }
void nuiPainter::DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, nuiSize x1, nuiSize y1, nuiSize x2, nuiSize y2){ }

void nuiPainter::DrawArray(const nuiRenderArray& rRenderArray){ }
void nuiPainter::DrawArray(const nuiRenderArray* pRenderArray){ }

void nuiPainter::SetRect(const nuiRect& rRect)
{
  mRect = rRect;
}

const nuiRect& nuiPainter::GetRect()
{
  return mRect;
}

// Fonts and text handling
bool nuiPainter::SetFont ( nuiFont* pFont ){ } ///< Set the font that will be used by the subsequent calls to DrawText.
nuiFont* nuiPainter::GetFont(){ } ///< Retrieve the current font.

void nuiPainter::SetTextColor(const nuiColor& rColor){ }
nuiColor nuiPainter::GetTextColor(){ }
void nuiPainter::DrawText(nuiSize x, nuiSize y, const nglString& rString){ } /// Draw text at the given coordinates and the current font.
void nuiPainter::DrawText(nuiSize x, nuiSize y, const nuiFontLayout& rLayout){ } /// Draw text layout at the given coordinates and the current font. The given layout must use the same font as the current font otherwise the results are unpredictable.

// Global Draw Settings Manipulation
void nuiPainter::PermitAntialiasing(bool Set){ }
bool nuiPainter::IsAntialiasingPermited(){ }


// nuiMetaRenderer:
void nuiMetaRenderer::StoreOpCode(OpCode code)
{
  StoreInt((int32)code);
}

void nuiMetaRenderer::StoreRect(const nuiRect& rRect)
{
  StoreFloat(rRect.Left());
  StoreFloat(rRect.Top());
  StoreFloat(rRect.Right());
  StoreFloat(rRect.Bottom());
}

void nuiMetaRenderer::StoreVector(const nglVectorf& rVector)
{
  for (uint i = 0; i < 4; i++)
    StoreFloat(rVector.Elt[i]);
}

void nuiMetaRenderer::StoreColor(const nuiColor& rColor)
{
  StoreFloat(rColor.GetRed());
  StoreFloat(rColor.GetGreen());
  StoreFloat(rColor.GetBlue());
  StoreFloat(rColor.GetAlpha());
}

void nuiMetaRenderer::StoreBlendFunc(nuiBlendFunc func)
{
  StoreInt(func);
}

void nuiMetaRenderer::StoreBool(bool Val)
{
  StoreInt(Val?1:0);
}

void nuiMetaRenderer::StoreFloat(float Val)
{
  StoreBuffer(&Val, sizeof(float), 1);
}

void nuiMetaRenderer::StoreInt(int32 Val)
{
  StoreBuffer(&Val, sizeof(int32), 1);
}

void nuiMetaRenderer::StorePointer(void* pVal)
{
  StoreBuffer(&pVal, sizeof(void*), 1);
}

void nuiMetaRenderer::StoreShapeMode(nuiShapeMode mode)
{
  StoreInt((int32) mode);
}

void nuiMetaRenderer::StoreBuffer(void* pBuffer, uint ElementSize, uint ElementCount)
{
  uint size = ElementSize * ElementCount;
  memcpy(&mOperations[mOperationPos], pBuffer, size);
}

nuiMetaRenderer::OpCode nuiMetaRenderer::FetchOpCode()
{
  return (OpCode)FetchInt();
}

nuiRect nuiMetaRenderer::FetchRect()
{
  float x0, y0, x1, y1;
  x0 = FetchFloat();
  y0 = FetchFloat();
  x1 = FetchFloat();
  y1 = FetchFloat();
  return nuiRect(x0, y0, x1, y1, false);
}

nglVectorf nuiMetaRenderer::FetchVector()
{
  float x, y, z, w;
  x = FetchFloat();
  y = FetchFloat();
  z = FetchFloat();
  w = FetchFloat();
  return nglVectorf(x, y, z, w);
}

nuiColor nuiMetaRenderer::FetchColor()
{
  float r, g, b, a;
  r = FetchFloat();
  g = FetchFloat();
  b = FetchFloat();
  a = FetchFloat();
  return nuiColor(r, g, b, a);
}

nuiBlendFunc nuiMetaRenderer::FetchBlendFunc()
{
  return (nuiBlendFunc)FetchInt();
}

bool nuiMetaRenderer::FetchBool()
{
  return FetchInt()?true:false;
}

float nuiMetaRenderer::FetchFloat()
{
  float tmp;
  FetchBuffer(&tmp, sizeof(float), 1);
  return tmp;
}

int32 nuiMetaRenderer::FetchInt()
{
  int32 tmp;
  FetchBuffer(&tmp, sizeof(int32), 1);
  return tmp;
}

void* nuiMetaRenderer::FetchPointer()
{
  void* pTmp;
  FetchBuffer(&pTmp, sizeof(void*), 1);
  return pTmp;
}

nuiShapeMode nuiMetaRenderer::FetchShapeMode()
{
  return (nuiShapeMode)FetchInt();
}

void nuiMetaRenderer::FetchBuffer(void* pBuffer, uint ElementSize, uint ElementCount)
{
  uint size = ElementSize * ElementCount;
  memcpy(pBuffer, &mOperations[mOperationPos], size);
}


#if 0
StartRendering      // (nuiSize ClipOffsetX = 0, nuiSize ClipOffsetY = 0)
BeginSession        //
StopRendering       //

PushClipping        //
PopClipping         //

Clip                // (const nuiRect& rGlobalRect)
EnableClipping      // (bool set)
ResetClipRect       // (bool EmptyClipStack = false)

AddClipShape        // (nuiShape& rShape, bool Invert = false)
BlendClipShape      // (nuiShape& rShape, bool Invert = false)
ResetClipShape      // (bool Invert = false)

PushRenderState     //
PopRenderState      //
ActivateRenderState // (const nuiRendererState& rState)

ResetRenderState    //

EnableBlending      // (bool val)

EnableScissorTest   // (bool val)
EnableStencilTest   // (bool val)

EnableTexturing     // (bool val)

SetBlendFunc        // (nuiBlendFunc Func)
SetTexture          // (nuiTexture* pTex , int num = 0)

PushMatrix          //
PopMatrix           //
LoadMatrix          // (const nglMatrixf& Matrix)
MultMatrix          // (const nglMatrixf& Matrix)
Translate           // (const nglVectorf& Vector)
Scale               // (const nglVectorf& Vector)
LoadIdentity        //

SetFillColor        // (const nuiColor& rColor)
SetStrokeColor      // (const nuiColor& rColor)

SetLineWidth        // (nuiSize Width)
EnableAntiAliasing  // (bool set)

SetWinding          // 

SetClearColor       // (const nuiColor& ClearColor)
SetTextColor        // (const nuiColor& ClearColor)

DrawShape           // (nuiShape* pShape, nuiShapeMode Mode)
DrawRect            // (const nuiRect& Rect, nuiShapeMode Mode)
DrawLine            // (const nglVectorf& rOrigin, const nglVectorf& rDestination)
DrawImage           // (const nuiRect& rDest, const nuiRect& rSource)
DrawImageQuad       // (float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource)
DrawShade           // (const nuiRect& rSourceRect, const nuiRect& rShadeRect)
BlurRect            // (const nuiRect& rRect, uint Strength)
DrawGradient        // (const nuiGradient& rGradient, const nuiRect& rEnclosingRect, const nglVector2f& rP1, const nglVector2f& rP2)
DrawArray           // (const nuiRenderArray& rRenderArray)

Clear               //
SetSize             // (int w,int h)

SetFont      // (nuiFont* pFont)

DrawText            // (nuiSize x, nuiSize y, const nuiFontLayout& rLayout)

PermitAntialiasing  // (bool Set)
#endif
#endif