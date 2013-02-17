/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#if (defined _UIKIT_) || (defined _ANDROID_)
#    include "glu//GL/glu.h"
#    include "glu/libtess/tess.h"
#endif

class nuiPoint;


GLUtesselator* nuiTessellator::mpTess = NULL;
uint32 nuiTessellator::mRefs = 0;

nuiTessellator::nuiTessellator(nuiPathGenerator* pPathGenerator)
{
  mpPath = pPathGenerator;
  mpShape = NULL;

  if (!mpTess)
    mpTess = gluNewTess();

  mRefs++;
}

nuiTessellator::nuiTessellator(nuiShape* pShape)
{
  mpPath = NULL;
  mpShape = pShape;

  if (!mpTess)
    mpTess = gluNewTess();

  mRefs++;
}

nuiTessellator::~nuiTessellator()
{
  if (!--mRefs)
  {
    if (mpTess)
    {
			gluDeleteTess(mpTess);
			mpTess = NULL;
    }
  }
}

class TessellatorInfo
{
public:

  TessellatorInfo()
  {
    mEdgeFlag = true;
    mLastEdgeFlag = true;
    mLastEdgeFlag2 = true;
    mTex = 0;
    mpObject = new nuiRenderObject();
  }
  ~TessellatorInfo()
  {
  }

#ifndef CALLBACK
  #define CALLBACK
#endif
  
  static GLvoid CALLBACK StaticInternalTessBegin(GLenum type, void * polygon_data);
  static GLvoid CALLBACK StaticInternalTessEdgeFlag(GLboolean flag, void * polygon_data);
  static GLvoid CALLBACK StaticInternalTessVertex(void * vertex_data, void * polygon_data);
  static GLvoid CALLBACK StaticInternalTessEnd(void * polygon_data);
  static GLvoid CALLBACK StaticInternalTessCombine(GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData, void * polygon_data);
  static GLvoid CALLBACK StaticInternalTessError(GLenum ErrNo, void * polygon_data);
  void InternalTessBegin(GLenum type);
  void InternalTessEdgeFlag(GLboolean flag);
  void InternalTessVertex(void* vertex_data);
  void InternalTessCombine(GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData);
  void InternalTessEnd();
  void InternalTessError(GLenum ErrNo);

  nuiRenderObject* mpObject;
  nuiPath mTempPoints;
  bool mOutline;

private:
  bool mEdgeFlag;
  bool mLastEdgeFlag;
  bool mLastEdgeFlag2;
  float mTex;
};

nuiRenderObject* nuiTessellator::GenerateFromPath(float Quality)
{
  nuiPath Points;

  mpPath->Tessellate(Points, Quality);

  gluTessNormal(mpTess, 0,0,1);
  gluTessProperty(mpTess,GLU_TESS_TOLERANCE, 0);
  gluTessCallback(mpTess, GLU_TESS_BEGIN_DATA,    NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessBegin);
  gluTessCallback(mpTess, GLU_TESS_EDGE_FLAG_DATA,NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessEdgeFlag);
  gluTessCallback(mpTess, GLU_TESS_VERTEX_DATA,   NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessVertex);
  gluTessCallback(mpTess, GLU_TESS_END_DATA,      NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessEnd);
  gluTessCallback(mpTess, GLU_TESS_COMBINE_DATA,  NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessCombine);
  gluTessCallback(mpTess, GLU_TESS_ERROR_DATA,    NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessError);

  gluTessProperty(mpTess,GLU_TESS_BOUNDARY_ONLY, mOutLine?GL_TRUE:GL_FALSE);
  gluTessProperty(mpTess,GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

  TessellatorInfo Infos;
  Infos.mOutline = mOutLine;
  gluTessBeginPolygon(mpTess, &Infos); 

  gluTessBeginContour(mpTess);

  uint count = Points.GetCount();
  for (uint i = 0; i < count; i++)
  {
    nuiPoint& rPoint = Points[i];
    double vec[4] = { rPoint[0], rPoint[1], rPoint[2], 0 };
    gluTessVertex(mpTess, vec, (void*)Infos.mTempPoints.AddVertex(rPoint));
  }
  gluTessEndContour(mpTess);

  gluTessEndPolygon(mpTess); 

  return Infos.mpObject;
}

nuiRenderObject* nuiTessellator::GenerateFromShape(float Quality)
{
  gluTessNormal(mpTess, 0,0,1);
  gluTessProperty(mpTess,GLU_TESS_TOLERANCE, 0);
  gluTessCallback(mpTess, GLU_TESS_BEGIN_DATA,    NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessBegin);
  gluTessCallback(mpTess, GLU_TESS_EDGE_FLAG_DATA,NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessEdgeFlag);
  gluTessCallback(mpTess, GLU_TESS_VERTEX_DATA,   NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessVertex);
  gluTessCallback(mpTess, GLU_TESS_END_DATA,      NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessEnd);
  gluTessCallback(mpTess, GLU_TESS_COMBINE_DATA,  NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessCombine);
  gluTessCallback(mpTess, GLU_TESS_ERROR_DATA,    NUI_GLU_CALLBACK &TessellatorInfo::StaticInternalTessError);

  gluTessProperty(mpTess,GLU_TESS_BOUNDARY_ONLY, mOutLine?GL_TRUE:GL_FALSE);
  nuiShape::Winding Winding = mpShape->GetWinding();
  if (Winding == nuiShape::eNone)
    Winding = nuiShape::eNonZero;
  gluTessProperty(mpTess,GLU_TESS_WINDING_RULE, Winding);

  TessellatorInfo Infos;
  Infos.mOutline = mOutLine;

  uint32 countours = mpShape->GetContourCount();

  gluTessBeginPolygon(mpTess, &Infos); 
  
  for (uint32 contour = 0; contour < countours; contour++)
  {
    nuiPath Points;
    nuiContour* pContour = mpShape->GetContour(contour);
    NGL_ASSERT(pContour != NULL);

    pContour->Tessellate(Points, Quality);

    uint count = Points.GetCount();
    bool beginNext = true;
    if (count)
    {
      for (uint i = 0; i < count; i++)
      {
        nuiPoint& rPoint = Points[i];
        if (!beginNext && rPoint.GetType() == nuiPointTypeStop)
        {
          gluTessEndContour(mpTess);
          beginNext = true;
        }
        else
        {
          if (beginNext)
          {
            gluTessBeginContour(mpTess);
            beginNext = false;
          }

          GLdouble vec[3] = { rPoint[0], rPoint[1], rPoint[2] };
          gluTessVertex(mpTess, vec, (void*)Infos.mTempPoints.AddVertex(rPoint));
        }
      }
      gluTessEndContour(mpTess);
      beginNext = true;
    }
  }

  gluTessEndPolygon(mpTess); 

  return Infos.mpObject;
}

nuiRenderObject* nuiTessellator::Generate(float Quality)
{
  if (mpPath)
    return GenerateFromPath(Quality);
  else if (mpShape)
    return GenerateFromShape(Quality);
  return NULL;
}




GLvoid TessellatorInfo::StaticInternalTessBegin(GLenum type, void * polygon_data)
{
  ((TessellatorInfo*)polygon_data)->InternalTessBegin(type);
}

GLvoid TessellatorInfo::StaticInternalTessEdgeFlag(GLboolean flag, void * polygon_data)
{
  ((TessellatorInfo*)polygon_data)->InternalTessEdgeFlag(flag);
}

GLvoid TessellatorInfo::StaticInternalTessVertex(void * vertex_data, void * polygon_data)
{
  ((TessellatorInfo*)polygon_data)->InternalTessVertex(vertex_data);
}

GLvoid TessellatorInfo::StaticInternalTessEnd(void * polygon_data)
{
  ((TessellatorInfo*)polygon_data)->InternalTessEnd();
}

GLvoid TessellatorInfo::StaticInternalTessCombine(GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData, void * polygon_data)
{
  ((TessellatorInfo*)polygon_data)->InternalTessCombine(coords, vertex_data, weight, outData);
}

GLvoid TessellatorInfo::StaticInternalTessError(GLenum ErrNo, void * polygon_data)
{
  ((TessellatorInfo*)polygon_data)->InternalTessError(ErrNo);
}


void TessellatorInfo::InternalTessBegin(GLenum type)
{
  nuiRenderArray* pArray = new nuiRenderArray(type, false, false, mOutline);
  pArray->EnableArray(nuiRenderArray::eVertex);
  //pArray->EnableArray(nuiRenderArray::eEdgeFlag);
  mpObject->AddArray(pArray);
}

void TessellatorInfo::InternalTessEdgeFlag(GLboolean flag)
{
  mEdgeFlag = flag?true:false;
}

void TessellatorInfo::InternalTessVertex(void* vertex_data)
{
  nuiRenderArray* pArray = mpObject->GetLastArray();
  pArray->SetVertex(mTempPoints[(unsigned long int)vertex_data]);
  //pArray->SetEdgeFlag(mEdgeFlag);
  pArray->PushVertex();
}

void TessellatorInfo::InternalTessCombine(GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData)
{
  *outData = (void*)mTempPoints.AddVertex(nuiPoint((float)(coords[0]), (float)(coords[1]), (float)(coords[2])));
}

void TessellatorInfo::InternalTessEnd()
{
}

void TessellatorInfo::InternalTessError(GLenum ErrNo)
{
#ifdef __NUI_NO_GL__
  NGL_OUT(_T("nui_glu tessellation error\n"));
#elsif !defined(_OPENGL_ES_)
  NGL_OUT(_T("nui_glu tessellation error: %s\n"), gluErrorString(ErrNo));
#endif
}



