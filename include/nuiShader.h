/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiShader_h__
#define __nuiShader_h__

//#include "nui.h"
#include "nglString.h"

class nglContext;
class nuiXMLNode;

class NUI_API nuiShader
{
public:
  nuiShader(nglContext* pContext, const nglString& rName, const nglString& rVertexShader = nglString::Empty, const nglString& rFragmentShader = nglString::Empty);
  nuiShader(nuiXMLNode* pDesc);
  virtual ~nuiShader();

  bool Compile();
  bool Bind();
  bool UnBind();
  nglString GetErrorMessage();

  void SetTexture1D(int num, GLuint TextureID);
  void SetTexture2D(int num, GLuint TextureID);
private:
#ifdef GL_ARB_shader_objects
  GLhandleARB mProgram;
  GLhandleARB mVS;
  GLhandleARB mFS;
#endif
  nglContext* mpContext;

  nglString mVertexShader;
  nglString mFragmentShader;
  const char* mpVertexShader;
  const char* mpFragmentShader;
  
  nglString mErrorMessage;
};

#endif
