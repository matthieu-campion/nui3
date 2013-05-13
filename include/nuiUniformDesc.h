//
//  nuiUniformDesc.h
//  nui3
//
//  Created by Sébastien Métrot on 3/4/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#pragma once

class nuiShaderProgram;

class nuiUniformDesc
{
public:
  nuiUniformDesc();
  nuiUniformDesc(const nuiUniformDesc& rDesc);
  nuiUniformDesc(const nglString& rName, GLenum Type, int count, GLuint Location, nuiShaderProgram* pProgram = NULL);

  ~nuiUniformDesc();

  nuiUniformDesc& operator=(const nuiUniformDesc& rDesc);

  void Set(const float* pV, int32 count, bool apply = false);
  void Set(const std::vector<float>& rV, bool apply = false);

  void Set(const int32* pV, int32 count, bool apply = false);
  void Set(const std::vector<int32>& rV, bool apply = false);

  void Set(float v1, bool apply = false);
  void Set(float v1, float v2, bool apply = false);
  void Set(float v1, float v2, float v3, bool apply = false);
  void Set(float v1, float v2, float v3, float v4, bool apply = false);
  void Set(const nglVector2f& rVec, bool apply = false);
  void Set(const nglVector3f& rVec, bool apply = false);
  void Set(const nglVectorf& rVec, bool apply = false);
  void Set(const nuiColor& rColor, bool Apply = false);
  void Set(int32 v1, bool apply = false);
  void Set(int32 v1, int32 v2, bool apply = false);
  void Set(int32 v1, int32 v2, int32 v3, bool apply = false);
  void Set(int32 v1, int32 v2, int32 v3, int32 v4, bool apply = false);
  void Set(const nglMatrixf& rMat, bool apply = false);

  void Set(const nuiUniformDesc& rDesc, bool apply = false);
  void Apply() const;

  void Dump() const;

  nglString mName;
  GLenum mType;
  GLenum mCount;
  GLuint mLocation;
  mutable bool mChanged;

  union
  {
    GLfloat* mpFloats;
    GLint* mpInts;
  } mValues;

  bool operator == (const nuiUniformDesc& rDesc) const;
};

