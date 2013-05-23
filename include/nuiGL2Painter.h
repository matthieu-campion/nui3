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
  
  virtual void DrawArray(nuiRenderArray* pArray);
  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();

  
protected:
  virtual void ResetOpenGLState();

  void SetViewport();

  // Immediate mode setup:
  void SetVertexPointers(const nuiRenderArray& rArray);

  //  VBO Setup:
  void SetVertexBuffersPointers(const nuiRenderArray& rArray, const VertexBufferInfo& rInfo);
  void SetStreamBuffersPointers(const nuiRenderArray& rArray, const VertexBufferInfo& rInfo, int index);

  // Reset vertices modes
  void ResetVertexPointers(const nuiRenderArray& rArray);
  

  nuiShaderProgram* mpShader_TextureVertexColor;
  nuiShaderProgram* mpShader_TextureAlphaVertexColor;
  nuiShaderProgram* mpShader_TextureDifuseColor;
  nuiShaderProgram* mpShader_TextureAlphaDifuseColor;
  nuiShaderProgram* mpShader_DifuseColor;
  nuiShaderProgram* mpShader_VertexColor;

  nuiMatrix mSurfaceMatrix;

  VertexBufferInfo* mpCurrentVertexBufferInfo;
  nuiRenderArray* mpLastArray;
};

#define nuiCheckForGLErrors() { NGL_ASSERT(nuiCheckForGLErrorsReal()); }

#endif //   #ifndef __NUI_NO_GL__

#endif //__nuiGL2Painter_h__

