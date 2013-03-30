/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiGL2Painter_h__
#define __nuiGL2Painter_h__

//#include "nui.h"
#include "nuiDrawContext.h"

// Disable GL support on OSX Classic...
#ifdef __NGL_CFM__
//  #define __NUI_NO_GL__
#endif

#ifndef __NUI_NO_GL__

class NUI_API nuiGL2Painter : public nuiGLPainter
{
public:
  nuiGL2Painter(nglContext* pContext);
  virtual ~nuiGL2Painter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(nuiRenderArray* pArray);
  virtual void ClearColor();
  virtual void BeginSession();
  virtual void EndSession();
  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();
  virtual void LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix);
  virtual void MultProjectionMatrix(const nuiMatrix& rMatrix);
  virtual void PushProjectionMatrix();
  virtual void PopProjectionMatrix();

  
protected:
  void ApplyState(const nuiRenderState& rState, bool ForceApply);
  virtual void ResetOpenGLState();

  void SetViewport();
  
  nuiShaderProgram* mpShader_TextureVertexColor;
  nuiShaderProgram* mpShader_TextureAlphaVertexColor;
  nuiShaderProgram* mpShader_TextureDifuseColor;
  nuiShaderProgram* mpShader_TextureAlphaDifuseColor;
  nuiShaderProgram* mpShader_DifuseColor;
  nuiShaderProgram* mpShader_VertexColor;
};

#define nuiCheckForGLErrors() { NGL_ASSERT(nuiCheckForGLErrorsReal()); }

#endif //   #ifndef __NUI_NO_GL__

#endif //__nuiGL2Painter_h__

