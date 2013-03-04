//
//  nuiUniformDesc.h
//  nui3
//
//  Created by Sébastien Métrot on 3/4/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#pragma once


class nuiUniformDesc
{
public:
  nuiUniformDesc();
  nuiUniformDesc(const nglString& rName, GLenum Type, int count, GLuint Location);

  void Set(const float* pV, int32 count);
  void Set(const std::vector<float>& rV);

  void Set(const int32* pV, int32 count);
  void Set(const std::vector<int32>& rV);

  void Set(float v1);
  void Set(float v1, float v2);
  void Set(float v1, float v2, float v3);
  void Set(float v1, float v2, float v3, float v4);
  void Set(const nglVector2f& rVec);
  void Set(const nglVector3f& rVec);
  void Set(const nglVectorf& rVec);
  void Set(const nuiColor& rColor);
  void Set(int32 v1);
  void Set(int32 v1, int32 v2);
  void Set(int32 v1, int32 v2, int32 v3);
  void Set(int32 v1, int32 v2, int32 v3, int32 v4);
  void Set(const nglMatrixf& rMat);

  void Apply() const;

  nglString mName;
  GLenum mType;
  GLenum mCount;
  GLuint mLocation;

  union
  {
    GLfloat* mpFloats;
    GLint* mpInts;
  } mValues;
};

