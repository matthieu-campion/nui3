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

class nuiTexture;
class nuiShaderProgram;

enum nuiRenderer
{
  eSoftware = eTargetAPI_None,
  eOpenGL = eTargetAPI_OpenGL,
  eOpenGL2 = eTargetAPI_OpenGL2,
  eDirect3D = eTargetAPI_Direct3D,
  eMeta
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
  void StartRendering();
  void BeginSession();
  void EndSession();
  void StopRendering();
  //@}

  void EnableColorBuffer(bool set);

  /** @name Clipping support */
  //@{
  void PushClipping();
  void PopClipping();
  bool Clip(const nuiRect& rRect); ///< This method restricts all the rendering operation to the given rectangle. The global rectangle is calculated by applying the current matrix to the rRect parameter.
  bool EnableClipping(bool set); ///< Enables or disable clipping.
  bool GetClipRect(nuiRect& rRect, bool LocalRect) const; ///< Fills rRect with the current clipping rect if clipping is enabled, or with the complete Draw Context rect otherwise.
  bool ResetClipRect(); ///< This method reset the clipping rect to the original size of the window.
  uint32 GetClipStackSize() const; ///< Return the number of Clip state saved on the clip stack.
  //@}

  void SetPainter(nuiPainter* pPainter);
  nuiPainter* GetPainter() const;

  void SetMainPainter(nuiPainter* pPainter);
  nuiPainter* GetMainPainter() const;

  /** @name Render state manipulation */
  //@{
  void PushState();
  void PopState();

  bool ResetState();
  const nuiRenderState& GetState() const;
  void SetState(const nuiRenderState& rState);

  void EnableBlending (bool val);
  void EnableAlphaTest (bool val);
  void EnableTexturing (bool val);

  void SetBlendFunc(nuiBlendFunc Func);
  //@}

  /** @name Texture manipulation */
  //@{
  bool IsTextureCurrent(nuiTexture* pTex, int slot = 0) const;
  nuiTexture* GetTexture(int slot = 0) const;
  void SetTexture(nuiTexture* pTex, int slot = 0);
  //@}

  /** @name Shader manipulation */
  //@{
  bool IsShaderCurrent(nuiShaderProgram* pShader) const;
  nuiShaderProgram* GetShader() const;
  void SetShader(nuiShaderProgram* pShader, nuiShaderState* pShaderState);
  void SetShaderState(nuiShaderState* pState);
  nuiShaderState* GetShaderState() const;
  //@}

  /** @name Matrix operations */
  //@{
  void PushMatrix();
  void PopMatrix();
  void LoadMatrix(const nuiMatrix& Matrix);
  void GetMatrix(nuiMatrix& Matrix) const;
  const nuiMatrix& GetMatrix() const;
  void MultMatrix(const nuiMatrix& Matrix);
  void Translate(const nuiVector& Vector);
  void Translate(nuiSize X, nuiSize Y, nuiSize Z = 0.0f);
  void Scale(nuiSize X, nuiSize Y, nuiSize Z = 1.0f);
  void LoadIdentity();
  //@}

  /** @name Matrix operations */
  //@{
  void PushProjectionMatrix();
  void PopProjectionMatrix();
  void Set2DProjectionMatrix(const nuiRect& rRect);
  void SetPerspectiveProjectionMatrix(const nuiRect& rRect, float FovY, float Aspect, float Near, float Far);
  void SetOrthoProjectionMatrix(const nuiRect& rRect, float Left, float Right, float Bottom, float Top, float Near, float Far);
  void SetFrustumProjectionMatrix(const nuiRect& rRect, float Left, float Right, float Bottom, float Top, float Near, float Far);
  void LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& Matrix);
  void GetProjectionMatrix(nuiMatrix& Matrix) const;
  const nuiMatrix& GetProjectionMatrix() const;
  void MultProjectionMatrix(const nuiMatrix& Matrix);
  void LoadProjectionIdentity();
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

  void SetWinding(nuiShape::Winding Rule); ///< Set the shapes filling/rasterization rule. This one is used only if the rendering shape have their rule set to nuiShape::eNone
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
  void Clear(bool color = true, bool depth = true, bool stencil = true); ///< Initializes the current drawing buffer with the default values.
  void DrawImage(const nuiRect& rDest, const nuiRect& rSource);
  void DrawImageQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource);
  void DrawShade (const nuiRect& rSourceRect, const nuiRect& rShadeRect, const nuiColor& rTint); ///< Draw a shade extending from SourceRect to ShadeRect. The shade may look quite diferent depending on the actual renderer.
  void BlurRect(const nuiRect& rRect, uint Strength) {}; ///< Blur a rectangle on screen.
  void DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, const nuiVector2& rP1, const nuiVector2& rP2);
  void DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, nuiSize x1, nuiSize y1, nuiSize x2, nuiSize y2);
  void DrawArray(nuiRenderArray* pArray);
  void DrawObject(const nuiRenderObject& rObject);
  //@}

  int GetWidth() const;
  int GetHeight() const;

  /** @name Fonts and text handling */
  //@{
  bool SetFont (nuiFont* pFont, bool AlreadyAcquired=false); ///< Set the font that will be used by the subsequent calls to DrawText.
  nuiFont* GetFont() const; ///< Retrieve the current font.

  void SetTextColor(const nuiColor& rColor);
  nuiColor GetTextColor() const;
  void DrawText(nuiSize x, nuiSize y, const nglString& rString, bool AlignGlyphPixels = true); /// Draw text at the given coordinates and the current font.
  void DrawText(nuiSize x, nuiSize y, const nuiTextLayout& rLayout, bool AlignGlyphPixels = true); /// Draw text layout at the given coordinates and the current font. The given layout must use the same font as the current font otherwise the results are unpredictable.
  //@}

  /** @name Global Draw Settings Manipulation */
  //@{
  void PermitAntialiasing(bool Set);
  bool IsAntialiasingPermited() const;
  //@}

  void AddBreakPoint();

  void SetSurface(nuiSurface* pSurface);
  nuiSurface* GetSurface() const;
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
  nuiPainter* mpMainPainter;
  nuiPainter* mpSavedPainter;
  nuiTexture* mpAATexture;
  GLint mClipShapeValue;
  
  uint32 mStateChanges;
};

#endif // __nuiDrawContext_h__
