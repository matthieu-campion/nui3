/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiDrawContext_h__
#define __nuiDrawContext_h__

/* nuiDrawContext design

  Rendering context divided in two parts:
  - Front End = nui high level rendering API. Backward compatible with the former nui API.
  - Back End = nui low level rendering API.

  Back End:
  Accepts two kind of requests:
  - Change the rendering state.
  - Draw something.

  Rendering state changes are accumulated until a Drawing operation is requested. The state is then applied before we carry the rendering operation.

*/

//#include "nui.h"
#include "nuiFont.h"
#include "nuiRect.h"
#include "nuiShape.h"
#include "nuiGradient.h"
#include "nuiRenderArray.h"

#include "nuiRenderState.h"

#include "nuiPainter.h"

#include <stack>
#include <vector>

class nuiTexture;
class nuiSurface;

enum nuiRenderer
{
  eOpenGL = eTargetAPI_OpenGL,
  eDirect3D = eTargetAPI_Direct3D,
  eSoftware = eTargetAPI_None
};

#ifdef DrawText
#undef DrawText
#undef DrawLine
#undef DrawRect
#endif

class NUI_API nuiDrawContext
{

public:
  static nuiDrawContext *CreateDrawContext(const nuiRect& rRect, nuiRenderer mode, nglContext* pContext = NULL); 

  nuiDrawContext(const nuiRect& rRect);
  virtual ~nuiDrawContext();

  /** @name Rendering sessions: */
  //@{
  virtual void StartRendering(nuiSize ClipOffsetX = 0, nuiSize ClipOffsetY = 0)  
  { 
    mpPainter->StartRendering(ClipOffsetX, ClipOffsetY); 
  }
  void BeginSession()
  {
    mpPainter->BeginSession();
  }
  void EndSession()
  {
    mpPainter->EndSession();
  }

  void StopRendering() { SetTexture(NULL); }
  //@}

  void EnableColorBuffer(bool set);
  void SetStencilMode(nuiStencilMode, uint8 value);
  void ClearStencil(uint8 value);

  /** @name Clipping support */
  //@{
  void PushClipping();
  void PopClipping();
  bool Clip(const nuiRect& rRect); ///< This method restricts all the rendering operation to the given rectangle. The global rectangle is calculated by applying the current matrix to the rRect parameter.
  bool EnableClipping(bool set); ///< Enables or disable clipping.
  bool GetClipRect(nuiRect& rRect, bool LocalRect) const; ///< Fills rRect with the current clipping rect if clipping is enabled, or with the complete Draw Context rect otherwise.
  bool ResetClipRect(); ///< This method reset the clipping rect to the original size of the window.
  uint32 GetClipStackSize() const; ///< Return the number of Clip state saved on the clip stack.

  bool AddClipShape(nuiShape& rShape, bool Invert = false); /// Adding the clip shape draw the shape in a stencil buffer to use it as a mask for subsequent draw operations. The shape may overwrite any existing clip shape, and then extend the drawing enabled area. Until another clip shape is set or shape clipping is reset with ResetClipShape.
  bool BlendClipShape(nuiShape& rShape, bool Invert = false); /// Blending the clip shape draw the shape in a stencil buffer to use it as a mask for subsequent draw operations but not erasing existing clip shapes already present. This operation can only restric subsequent drawings, never extend the rendering area. Until another clip shape is set or shape clipping is reset with ResetClipShape.
  bool ResetClipShape(bool Invert = false); ///< Disable the last shape clipping that was set via AddClipShape.
  nuiStencilMode GetStencilMode() const; ///< Get the current stencil mode
  uint32 GetStencilValue() const; ///< Get the current stencil mode
  //@}

  void SetPainter(nuiPainter* pPainter);
  nuiPainter* GetPainter() const;

  /** @name Render state manipulation */
  //@{
  void PushState();
  void PopState();

  bool ResetState();
  const nuiRenderState& GetState() const;
  virtual void SetState(const nuiRenderState& rState)
  {
    mpPainter->SetState(rState);
  }

  void EnableBlending (bool val);
  void EnableAlphaTest (bool val);
  void EnableTexturing (bool val);

  void SetBlendFunc(nuiBlendFunc Func);
  //@}

  /** @name Texture manipulation */
  //@{
  bool IsTextureCurrent(nuiTexture* pTex) const;
  nuiTexture* GetTexture() const;
  void SetTexture(nuiTexture* pTex);
  //@}
  
  /** @name Surface manipulation */
  //@{
  bool IsSurfaceCurrent(nuiSurface* pSurface) const;
  nuiSurface* GetSurface() const;
  void SetSurface(nuiSurface* pSurface);
  void PushSurface();
  void PopSurface();
  //@}
  
  /** @name Matrix operations */
  //@{
  void PushMatrix();
  void PopMatrix();
  void LoadMatrix(const nuiMatrix& Matrix);
  void GetMatrix(nuiMatrix& Matrix) const;
  const nuiMatrix& GetMatrix() const;
  void MultMatrix(const nuiMatrix& Matrix);
  void Translate(const nuiVector2& Vector);
  void Translate(nuiSize X, nuiSize Y);
  void Scale(nuiSize X, nuiSize Y);
  void LoadIdentity();
  //@}

  /** @name Brush manipulation */
  //@{
  void SetFillColor(const nuiColor& rColor);
  void SetStrokeColor(const nuiColor& rColor);
  const nuiColor& GetFillColor() const;
  const nuiColor& GetStrokeColor() const;

  void SetLineWidth(nuiSize Width);
  void EnableAntialiasing(bool set);
  bool GetAntialiasing() const;

  virtual void SetWinding(nuiShape::Winding Rule); ///< Set the shapes filling/rasterization rule. This one is used only if the rendering shape have their rule set to nuiShape::eNone
  nuiShape::Winding GetWinding() const; ///< Get the shapes filling/rasterization rule.
  //@}

  /** @name Drawing function */
  //@{
  void SetClearColor(const nuiColor& ClearColor); ///< Set the color used to clear the background with Clear().
  void DrawShape(nuiShape* pShape, nuiShapeMode Mode, float Quality = 0.5f); ///< Call StrokeShape and FillShape.
  void DrawRect(const nuiRect& Rect, nuiShapeMode Mode);
  void DrawLine(const nuiVector2& rOrigin, const nuiVector2& rDestination);
  void DrawLine(float x1, float y1, float x2, float y2);
  void DrawPoint(const nuiVector2& rPoint);
  void DrawPoint(float x, float y);
  void Clear(); ///< Initializes the current drawing buffer with the default values.
  void DrawImage(const nuiRect& rDest, const nuiRect& rSource);
  void DrawImageQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource);
  void DrawShade (const nuiRect& rSourceRect, const nuiRect& rShadeRect); ///< Draw a shade extending from SourceRect to ShadeRect. The shade may look quite diferent depending on the actual renderer.
  void BlurRect(const nuiRect& rRect, uint Strength) {}; ///< Blur a rectangle on screen.
  void DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, const nuiVector2& rP1, const nuiVector2& rP2);
  void DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, nuiSize x1, nuiSize y1, nuiSize x2, nuiSize y2);
  virtual void DrawArray(const nuiRenderArray& rArray);
  void DrawObject(const nuiRenderObject& rObject);
  //@}

  void SetSize(uint w, uint h); ///< Set the size of the drawing surface (for clipping calculus).
  int GetWidth() const;
  int GetHeight() const;

  /** @name Fonts and text handling */
  //@{
  bool SetFont (nuiFont* pFont, bool AlreadyAcquired=false); ///< Set the font that will be used by the subsequent calls to DrawText.
  nuiFont* GetFont() const; ///< Retrieve the current font.

  void SetTextColor(const nuiColor& rColor);
  nuiColor GetTextColor() const;
  void DrawText(nuiSize x, nuiSize y, const nglString& rString); /// Draw text at the given coordinates and the current font.
  void DrawText(nuiSize x, nuiSize y, const nuiFontLayout& rLayout); /// Draw text layout at the given coordinates and the current font. The given layout must use the same font as the current font otherwise the results are unpredictable.
  //@}

  /** @name Global Draw Settings Manipulation */
  //@{
  void PermitAntialiasing(bool Set);
  bool IsAntialiasingPermited() const;
  //@}

  /** @name OpenGL Shading Language support */
  //@{
  void SetShader(nuiShader* pShader) { mCurrentState.mpShader = pShader; }
  nuiShader* GetShader() const { return mCurrentState.mpShader; }
  void DisableShader() { SetShader(NULL); }
  //@}

  void AddBreakPoint()
  {
    mpPainter->AddBreakPoint();
  }
  
protected:
  // Render state stack:
  std::stack<nuiRenderState*> mpRenderStateStack;
  nuiRenderState mCurrentState;
  
  nuiSize mWidth,mHeight;

#ifndef CALLBACK 
  #define CALLBACK 
#endif

#if (__GNUC__ == 4) && (__APPLE_CC__ <= 5370) && (defined __APPLE__)
#define NUI_GLU_CALLBACK (GLvoid (CALLBACK *)(...))
#else
#define NUI_GLU_CALLBACK (GLvoid (CALLBACK *)())
#endif


  nuiSize mClipOffsetX;
  nuiSize mClipOffsetY;
  bool mPermitAntialising;

  nuiPainter* mpPainter;
  nuiTexture* mpAATexture;
  GLint mClipShapeValue;
  
  uint32 mStateChanges;
};

#endif // __nuiDrawContext_h__
