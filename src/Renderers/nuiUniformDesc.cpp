//
//  nuiUniformDesc.cpp
//  nui3
//
//  Created by Sébastien Métrot on 3/4/13.
//  Copyright (c) 2013 libNUI. All rights reserved.
//

#include "nui.h"

//class nuiUniformDesc
nuiUniformDesc::nuiUniformDesc()
: mType(0), mCount(0), mLocation(0)
{
  mValues.mpFloats = NULL;
}

nuiUniformDesc::nuiUniformDesc(const nglString& rName, GLenum Type, int count, GLuint Location)
: mName(rName), mType(Type), mCount(count), mLocation(Location)
{
  switch (mType)
  {
    case GL_FLOAT:              mValues.mpFloats  = new float[1 * count];  break;
    case GL_FLOAT_VEC2:         mValues.mpFloats  = new float[2 * count];  break;
    case GL_FLOAT_VEC3:         mValues.mpFloats  = new float[3 * count];  break;
    case GL_FLOAT_VEC4:         mValues.mpFloats  = new float[4 * count];  break;

    case GL_INT:                mValues.mpInts    = new int32[1 * count];  break;
    case GL_INT_VEC2:           mValues.mpInts    = new int32[2 * count];  break;
    case GL_INT_VEC3:           mValues.mpInts    = new int32[3 * count];  break;
    case GL_INT_VEC4:           mValues.mpInts    = new int32[4 * count];  break;
    case GL_UNSIGNED_INT:       mValues.mpInts    = new int32[1 * count];  break;

    case GL_FLOAT_MAT2:         mValues.mpFloats  = new float[2 * 2 * count];  break;
    case GL_FLOAT_MAT3:         mValues.mpFloats  = new float[3 * 3 * count];  break;
    case GL_FLOAT_MAT4:         mValues.mpFloats  = new float[4 * 4 * count];  break;

    case GL_SAMPLER_2D:         mValues.mpInts    = new int32[1 * count];  break;
    case GL_SAMPLER_CUBE:       mValues.mpInts    = new int32[1 * count];  break;

    default:
      NGL_ASSERT(0);
  }
}

void nuiUniformDesc::Set(const float* pV, int32 count)
{
  switch (mType)
  {
    case GL_FLOAT:        NGL_ASSERT(count <= mCount * 1); break;
    case GL_FLOAT_VEC2:   NGL_ASSERT(count <= mCount * 2); break;
    case GL_FLOAT_VEC3:   NGL_ASSERT(count <= mCount * 3); break;
    case GL_FLOAT_VEC4:   NGL_ASSERT(count <= mCount * 4); break;

    case GL_FLOAT_MAT2:   NGL_ASSERT(count <= mCount * 2 * 2); break;
    case GL_FLOAT_MAT3:   NGL_ASSERT(count <= mCount * 3 * 3); break;
    case GL_FLOAT_MAT4:   NGL_ASSERT(count <= mCount * 4 * 4); break;

    default:
      NGL_ASSERT(0);
  }

  for (int32 i = 0; i < count; i++)
    mValues.mpFloats[i] = pV[i];
}


void nuiUniformDesc::Set(const std::vector<float>& rV)
{
  Set(&rV[0], rV.size());
}

void nuiUniformDesc::Set(const int32* pV, int32 count)
{
  switch (mType)
  {
    case GL_INT:            NGL_ASSERT(count <= mCount * 1); break;
    case GL_INT_VEC2:       NGL_ASSERT(count <= mCount * 2); break;
    case GL_INT_VEC3:       NGL_ASSERT(count <= mCount * 3); break;
    case GL_INT_VEC4:       NGL_ASSERT(count <= mCount * 4); break;

    case GL_UNSIGNED_INT:   NGL_ASSERT(count <= mCount * 4); break;
    case GL_SAMPLER_2D:     NGL_ASSERT(count <= mCount * 4); break;
    case GL_SAMPLER_CUBE:   NGL_ASSERT(count <= mCount * 4); break;

    default:
      NGL_ASSERT(0);
  }

  for (int32 i = 0; i < count; i++)
    mValues.mpInts[i] = pV[i];
}

void nuiUniformDesc::Set(const std::vector<int32>& rV)
{
  Set(&rV[0], rV.size());
}

void nuiUniformDesc::Set(float v1)
{
  Set(&v1, 1);
}

void nuiUniformDesc::Set(float v1, float v2)
{
  float v[] = { v1, v2 };
  Set(v, 2);
}

void nuiUniformDesc::Set(float v1, float v2, float v3)
{
  float v[] = { v1, v2, v3 };
  Set(v, 3);
}

void nuiUniformDesc::Set(float v1, float v2, float v3, float v4)
{
  float v[] = { v1, v2, v3, v4 };
  Set(v, 4);
}


void nuiUniformDesc::Set(const nglVector2f& rVec)
{
  NGL_ASSERT(mType == GL_FLOAT_VEC2 || mType == GL_FLOAT_VEC3 || mType == GL_FLOAT_VEC4);
  Set(&rVec.Elt[0], 2);
}

void nuiUniformDesc::Set(const nglVector3f& rVec)
{
  NGL_ASSERT(mType == GL_FLOAT_VEC3 || mType == GL_FLOAT_VEC4);
  Set(&rVec.Elt[0], 3);
}

void nuiUniformDesc::Set(const nglVectorf& rVec)
{
  NGL_ASSERT(mType == GL_FLOAT_VEC4);
  Set(&rVec.Elt[0], 4);
}

void nuiUniformDesc::Set(const nuiColor& rColor)
{
  float v[] = { rColor.Red(), rColor.Green(), rColor.Blue(), rColor.Alpha() };
  switch (mType)
  {
    case GL_FLOAT:      Set((v[0] + v[1] + v[2]) / 3.0f); break;
    case GL_FLOAT_VEC2: Set((v[0] + v[1] + v[2]) / 3.0f, v[3]); break;
    case GL_FLOAT_VEC3: Set(v, 3); break;
    case GL_FLOAT_VEC4: Set(v, 4); break;
    default:            NGL_ASSERT(0); break;
  }
}

void nuiUniformDesc::Set(int32 v1)
{
  Set(&v1, 1);
}

void nuiUniformDesc::Set(int32 v1, int32 v2)
{
  int32 v[] = { v1, v2 };
  Set(v, 2);
}

void nuiUniformDesc::Set(int32 v1, int32 v2, int32 v3)
{
  int32 v[] = { v1, v2, v3 };
  Set(v, 3);
}

void nuiUniformDesc::Set(int32 v1, int32 v2, int32 v3, int32 v4)
{
  int32 v[] = { v1, v2, v3, v4 };
  Set(v, 4);
}

void nuiUniformDesc::Set(const nglMatrixf& rMat)
{
  float v[16];
  switch (mType)
  {
    case GL_FLOAT_MAT2:
      for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; i++)
          v[i * 2 + j] = rMat.Array[i * 4 + j];
      Set(v, 4);
      break;
    case GL_FLOAT_MAT3:
      for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; i++)
          v[i * 3 + j] = rMat.Array[i * 4 + j];
      Set(v, 9);
      break;
    case GL_FLOAT_MAT4:
      Set(rMat.Array, 4);
      break;
    default :
      NGL_ASSERT(0);
      break;
  }
}


void nuiUniformDesc::Apply() const
{
  switch (mType)
  {
    case GL_FLOAT:              glUniform1fv(mLocation, mCount, mValues.mpFloats);  break;
    case GL_FLOAT_VEC2:         glUniform2fv(mLocation, mCount, mValues.mpFloats);  break;
    case GL_FLOAT_VEC3:         glUniform3fv(mLocation, mCount, mValues.mpFloats);  break;
    case GL_FLOAT_VEC4:         glUniform4fv(mLocation, mCount, mValues.mpFloats);  break;

    case GL_INT:                glUniform1iv(mLocation, mCount, mValues.mpInts);  break;
    case GL_INT_VEC2:           glUniform2iv(mLocation, mCount, mValues.mpInts);  break;
    case GL_INT_VEC3:           glUniform3iv(mLocation, mCount, mValues.mpInts);  break;
    case GL_INT_VEC4:           glUniform4iv(mLocation, mCount, mValues.mpInts);  break;
    case GL_UNSIGNED_INT:       glUniform1iv(mLocation, mCount, mValues.mpInts);  break;

    case GL_FLOAT_MAT2:         glUniformMatrix2fv(mLocation, mCount, GL_FALSE, mValues.mpFloats);  break;
    case GL_FLOAT_MAT3:         glUniformMatrix3fv(mLocation, mCount, GL_FALSE, mValues.mpFloats);  break;
    case GL_FLOAT_MAT4:         glUniformMatrix4fv(mLocation, mCount, GL_FALSE, mValues.mpFloats);  break;

    case GL_SAMPLER_2D:         glUniform1iv(mLocation, mCount, mValues.mpInts);  break;
    case GL_SAMPLER_CUBE:       glUniform1iv(mLocation, mCount, mValues.mpInts);  break;

    default:
      NGL_ASSERT(0);
  }
}


