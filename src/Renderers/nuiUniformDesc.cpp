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
: mType(-1), mCount(-1), mLocation(-1), mChanged(false)
{
  mValues.mpFloats = NULL;
}

nuiUniformDesc::nuiUniformDesc(const nuiUniformDesc& rDesc)
: mName(rDesc.mName), mType(rDesc.mType), mCount(rDesc.mCount), mLocation(rDesc.mLocation), mChanged(rDesc.mChanged)
{
  switch (mType)
  {
    case GL_FLOAT:              mValues.mpFloats  = new float[1 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 1 * mCount); break;
    case GL_FLOAT_VEC2:         mValues.mpFloats  = new float[2 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 2 * mCount); break;
    case GL_FLOAT_VEC3:         mValues.mpFloats  = new float[3 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 3 * mCount); break;
    case GL_FLOAT_VEC4:         mValues.mpFloats  = new float[4 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 4 * mCount); break;

    case GL_INT:                mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;
    case GL_INT_VEC2:           mValues.mpInts    = new int32[2 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 2 * mCount); break;
    case GL_INT_VEC3:           mValues.mpInts    = new int32[3 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 3 * mCount); break;
    case GL_INT_VEC4:           mValues.mpInts    = new int32[4 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 4 * mCount); break;
    case GL_UNSIGNED_INT:       mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;

    case GL_FLOAT_MAT2:         mValues.mpFloats  = new float[2 * 2 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 2 * 2 * mCount); break;
    case GL_FLOAT_MAT3:         mValues.mpFloats  = new float[3 * 3 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 3 * 3 * mCount); break;
    case GL_FLOAT_MAT4:         mValues.mpFloats  = new float[4 * 4 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 4 * 4 * mCount); break;

    case GL_SAMPLER_2D:         mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;
    case GL_SAMPLER_CUBE:       mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;

    default:
      NGL_ASSERT(0);
  }
}


nuiUniformDesc& nuiUniformDesc::operator=(const nuiUniformDesc& rDesc)
{
  mName = rDesc.mName;
  mType = rDesc.mType;
  mCount = rDesc.mCount;
  mLocation = rDesc.mLocation;
  mChanged = true;

  switch (mType)
  {
    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
      delete[] mValues.mpFloats;
      mValues.mpFloats = NULL;
      break;

    case GL_INT:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
    case GL_UNSIGNED_INT:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_CUBE:
      delete[] mValues.mpInts;
      mValues.mpInts = NULL;
      break;

    default:
      NGL_ASSERT(0);
  }

  {
    switch (mType)
    {
      case GL_FLOAT:              mValues.mpFloats  = new float[1 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 1 * mCount); break;
      case GL_FLOAT_VEC2:         mValues.mpFloats  = new float[2 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 2 * mCount); break;
      case GL_FLOAT_VEC3:         mValues.mpFloats  = new float[3 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 3 * mCount); break;
      case GL_FLOAT_VEC4:         mValues.mpFloats  = new float[4 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 4 * mCount); break;

      case GL_INT:                mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;
      case GL_INT_VEC2:           mValues.mpInts    = new int32[2 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 2 * mCount); break;
      case GL_INT_VEC3:           mValues.mpInts    = new int32[3 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 3 * mCount); break;
      case GL_INT_VEC4:           mValues.mpInts    = new int32[4 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 4 * mCount); break;
      case GL_UNSIGNED_INT:       mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;

      case GL_FLOAT_MAT2:         mValues.mpFloats  = new float[2 * 2 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 2 * 2 * mCount); break;
      case GL_FLOAT_MAT3:         mValues.mpFloats  = new float[3 * 3 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 3 * 3 * mCount); break;
      case GL_FLOAT_MAT4:         mValues.mpFloats  = new float[4 * 4 * mCount]; memcpy(mValues.mpFloats, rDesc.mValues.mpFloats, sizeof(float) * 4 * 4 * mCount); break;

      case GL_SAMPLER_2D:         mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;
      case GL_SAMPLER_CUBE:       mValues.mpInts    = new int32[1 * mCount]; memcpy(mValues.mpInts, rDesc.mValues.mpInts, sizeof(int32) * 1 * mCount); break;
        
      default:
        NGL_ASSERT(0);
    }
  }
}

nuiUniformDesc::nuiUniformDesc(const nglString& rName, GLenum Type, int count, GLuint Location, nuiShaderProgram* pProgram)
: mName(rName), mType(Type), mCount(count), mLocation(Location), mChanged(false)
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

  if (pProgram)
  {
    switch (mType)
    {
      case GL_FLOAT:
      case GL_FLOAT_VEC2:
      case GL_FLOAT_VEC3:
      case GL_FLOAT_VEC4:
      case GL_FLOAT_MAT2:
      case GL_FLOAT_MAT3:
      case GL_FLOAT_MAT4:
        glGetUniformfv(pProgram->GetProgram(), mLocation, mValues.mpFloats);
        break;

      case GL_INT:
      case GL_INT_VEC2:
      case GL_INT_VEC3:
      case GL_INT_VEC4:
      case GL_UNSIGNED_INT:
      case GL_SAMPLER_2D:
      case GL_SAMPLER_CUBE:
        glGetUniformiv(pProgram->GetProgram(), mLocation, mValues.mpInts);
        break;

      default:
        NGL_ASSERT(0);
    }
  }
}

nuiUniformDesc::~nuiUniformDesc()
{
  switch (mType)
  {
    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
      delete[] mValues.mpFloats;
      mValues.mpFloats = NULL;
      break;

    case GL_INT:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
    case GL_UNSIGNED_INT:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_CUBE:
      delete[] mValues.mpInts;
      mValues.mpInts = NULL;
      break;

    default:
      NGL_ASSERT(0);
  }
}


void nuiUniformDesc::Set(const float* pV, int32 count, bool apply)
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

  mChanged = true;

  if (apply)
    Apply();
}


void nuiUniformDesc::Set(const std::vector<float>& rV, bool apply)
{
  Set(&rV[0], rV.size());
}

void nuiUniformDesc::Set(const int32* pV, int32 count, bool apply)
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

  mChanged = true;

  if (apply)
    Apply();
}

void nuiUniformDesc::Set(const std::vector<int32>& rV, bool apply)
{
  Set(&rV[0], rV.size(), apply);
}

void nuiUniformDesc::Set(float v1, bool apply)
{
  Set(&v1, 1, apply);
}

void nuiUniformDesc::Set(float v1, float v2, bool apply)
{
  float v[] = { v1, v2 };
  Set(v, 2, apply);
}

void nuiUniformDesc::Set(float v1, float v2, float v3, bool apply)
{
  float v[] = { v1, v2, v3 };
  Set(v, 3, apply);
}

void nuiUniformDesc::Set(float v1, float v2, float v3, float v4, bool apply)
{
  float v[] = { v1, v2, v3, v4 };
  Set(v, 4, apply);
}


void nuiUniformDesc::Set(const nglVector2f& rVec, bool apply)
{
  NGL_ASSERT(mType == GL_FLOAT_VEC2 || mType == GL_FLOAT_VEC3 || mType == GL_FLOAT_VEC4);
  Set(&rVec.Elt[0], 2, apply);
}

void nuiUniformDesc::Set(const nglVector3f& rVec, bool apply)
{
  NGL_ASSERT(mType == GL_FLOAT_VEC3 || mType == GL_FLOAT_VEC4);
  Set(&rVec.Elt[0], 3, apply);
}

void nuiUniformDesc::Set(const nglVectorf& rVec, bool apply)
{
  NGL_ASSERT(mType == GL_FLOAT_VEC4);
  Set(&rVec.Elt[0], 4, apply);
}

void nuiUniformDesc::Set(const nuiColor& rColor, bool apply)
{
  float v[] = { rColor.Red(), rColor.Green(), rColor.Blue(), rColor.Alpha() };
  switch (mType)
  {
    case GL_FLOAT:      Set((v[0] + v[1] + v[2]) / 3.0f, apply); break;
    case GL_FLOAT_VEC2: Set((v[0] + v[1] + v[2]) / 3.0f, v[3], apply); break;
    case GL_FLOAT_VEC3: Set(v, 3, apply); break;
    case GL_FLOAT_VEC4: Set(v, 4, apply); break;
    default:            NGL_ASSERT(0); break;
  }
}

void nuiUniformDesc::Set(int32 v1, bool apply)
{
  Set(&v1, 1, apply);
}

void nuiUniformDesc::Set(int32 v1, int32 v2, bool apply)
{
  int32 v[] = { v1, v2 };
  Set(v, 2, apply);
}

void nuiUniformDesc::Set(int32 v1, int32 v2, int32 v3, bool apply)
{
  int32 v[] = { v1, v2, v3 };
  Set(v, 3, apply);
}

void nuiUniformDesc::Set(int32 v1, int32 v2, int32 v3, int32 v4, bool apply)
{
  int32 v[] = { v1, v2, v3, v4 };
  Set(v, 4, apply);
}

void nuiUniformDesc::Set(const nglMatrixf& rMat, bool apply)
{
  float v[16];
  switch (mType)
  {
    case GL_FLOAT_MAT2:
      for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; i++)
          v[i * 2 + j] = rMat.Array[i * 4 + j];
      Set(v, 4, apply);
      break;
    case GL_FLOAT_MAT3:
      for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; i++)
          v[i * 3 + j] = rMat.Array[i * 4 + j];
      Set(v, 9, apply);
      break;
    case GL_FLOAT_MAT4:
      Set(rMat.Array, 16, apply);
      break;
    default :
      NGL_ASSERT(0);
      break;
  }
}

static bool copycmp(int32 *p1, const int32* p2, int count)
{
  uint32 v1 = 0;
  uint32 v2 = 0;
  uint32 pa = 0;
  uint32 pb = 0;

  for (int i = 0; i < count; i++)
  {
    v1 = *p1;
    v2 = *p2++;
    *p1++ = v2;

    // Accumulate the test:
    pa ^= v1;
    pb ^= v2;
  }

  return pa == pb;
}

static bool copycmp(float *p1, const float* p2, int count)
{
  return copycmp((int32*)p1, (const int32*)p2, count);
}


void nuiUniformDesc::Set(const nuiUniformDesc& rDesc, bool apply)
{
  NGL_ASSERT(mType == rDesc.mType);
  NGL_ASSERT(mCount == rDesc.mCount);
  NGL_ASSERT(mLocation == rDesc.mLocation);

  bool c = false;
  const float* pSFloats = rDesc.mValues.mpFloats;
  const int* pSInts = rDesc.mValues.mpInts;
  switch (mType)
  {
    case GL_FLOAT:              c = !copycmp(mValues.mpFloats, pSFloats, 1 * mCount); break;
    case GL_FLOAT_VEC2:         c = !copycmp(mValues.mpFloats, pSFloats, 2 * mCount); break;
    case GL_FLOAT_VEC3:         c = !copycmp(mValues.mpFloats, pSFloats, 3 * mCount); break;
    case GL_FLOAT_VEC4:         c = !copycmp(mValues.mpFloats, pSFloats, 4 * mCount); break;

    case GL_INT:                c = !copycmp(mValues.mpInts, pSInts, 1 * mCount); break;
    case GL_INT_VEC2:           c = !copycmp(mValues.mpInts, pSInts, 2 * mCount); break;
    case GL_INT_VEC3:           c = !copycmp(mValues.mpInts, pSInts, 3 * mCount); break;
    case GL_INT_VEC4:           c = !copycmp(mValues.mpInts, pSInts, 4 * mCount); break;
    case GL_UNSIGNED_INT:       c = !copycmp(mValues.mpInts, pSInts, 1 * mCount); break;

    case GL_FLOAT_MAT2:         c = !copycmp(mValues.mpFloats, pSFloats, 2 * 2 * mCount); break;
    case GL_FLOAT_MAT3:         c = !copycmp(mValues.mpFloats, pSFloats, 3 * 3 * mCount); break;
    case GL_FLOAT_MAT4:         c = !copycmp(mValues.mpFloats, pSFloats, 4 * 4 * mCount); break;

    case GL_SAMPLER_2D:         c = !copycmp(mValues.mpInts, pSInts, 1 * mCount); break;
    case GL_SAMPLER_CUBE:       c = !copycmp(mValues.mpInts, pSInts, 1 * mCount); break;

    default:
      NGL_ASSERT(0);
  }

  mChanged = mChanged || c;
  if (apply)
    Apply();
}

static int total = 0;
static int skipped = 0;
static int applied = 0;

void nuiUniformDesc::Apply() const
{
  total++;

//  if (!(total % 10000))
//    NGL_OUT("Uniform stats: %d on %d applied (%d skipped) %f cache hit\n", applied, total, skipped, (float)skipped/(float)total);

  if (!mChanged)
  {
    skipped++;
    return;
  }
  applied++;

  mChanged = false;

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


bool nuiUniformDesc::operator == (const nuiUniformDesc& rDesc) const
{
  if (rDesc.mType != mType)
    return false;

  if (rDesc.mCount != mCount)
    return false;

  if (rDesc.mLocation != mLocation)
    return false;

  int32 count = 0;
  switch (mType)
  {
    case GL_FLOAT:        count = 1; break;
    case GL_FLOAT_VEC2:   count = 2; break;
    case GL_FLOAT_VEC3:   count = 3; break;
    case GL_FLOAT_VEC4:   count = 4; break;

    case GL_FLOAT_MAT2:   count = 2 * 2; break;
    case GL_FLOAT_MAT3:   count = 3 * 3; break;
    case GL_FLOAT_MAT4:   count = 4 * 4; break;

    case GL_INT:          count = 1; break;
    case GL_INT_VEC2:     count = 2; break;
    case GL_INT_VEC3:     count = 3; break;
    case GL_INT_VEC4:     count = 4; break;

    case GL_UNSIGNED_INT: count = 1; break;
    case GL_SAMPLER_2D:   count = 1; break;
    case GL_SAMPLER_CUBE: count = 1; break;

    default:
      NGL_ASSERT(0);
  }

  for (int32 i = 0; i < count; i++)
    if (rDesc.mValues.mpInts[i] != mValues.mpInts[i])
      return false;

  if (rDesc.mName != mName)
    return false;

  return true;
}


void nuiUniformDesc::Dump() const
{
  nglString v;
  int32 count = 0;
  switch (mType)
  {
    case GL_FLOAT:        count = 1; break;
    case GL_FLOAT_VEC2:   count = 2; break;
    case GL_FLOAT_VEC3:   count = 3; break;
    case GL_FLOAT_VEC4:   count = 4; break;

    case GL_FLOAT_MAT2:   count = 2 * 2; break;
    case GL_FLOAT_MAT3:   count = 3 * 3; break;
    case GL_FLOAT_MAT4:   count = 4 * 4; break;

    case GL_INT:          count = 1; break;
    case GL_INT_VEC2:     count = 2; break;
    case GL_INT_VEC3:     count = 3; break;
    case GL_INT_VEC4:     count = 4; break;

    case GL_UNSIGNED_INT: count = 1; break;
    case GL_SAMPLER_2D:   count = 1; break;
    case GL_SAMPLER_CUBE: count = 1; break;

    default:
      NGL_ASSERT(0);
  }

  for (int32 i = 0; i < count * mCount; i++)
    v.Add(mValues.mpFloats[i]).Add(" ");
  NGL_OUT("%s: %s\n", mName.GetChars(), v.GetChars());
}
