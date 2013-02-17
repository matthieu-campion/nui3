/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#ifndef __NUI_NO_AA__
  #include "AAPrimitives.h"
#endif

nuiOutliner::nuiOutliner(nuiPathGenerator* pPath, float linewidth)
{
  mpPath = pPath;
  SetLineWidth(linewidth);
  mLineJoin = nuiLineJoinBevel;
  mLineCap = nuiLineCapBut;
  mMiterLimit = 0.5;
}

nuiOutliner::~nuiOutliner()
{
}

void nuiOutliner::AddJoin(const nuiPoint& Point0, const nuiPoint& Point1, const nuiPoint& Point2, const nuiPoint& Point3, nuiPath& rPoints) const
{
  const double x1 = Point0[0];
  const double y1 = Point0[1];

  const double x2 = Point1[0];
  const double y2 = Point1[1];

  const double x3 = Point2[0];
  const double y3 = Point2[1];

  const double x4 = Point3[0];
  const double y4 = Point3[1];

  const double x43 = x4 - x3;
  const double y43 = y4 - y3;
  const double x21 = x2 - x1;
  const double y21 = y2 - y1;

  const double denom = y43 * x21 - x43 * y21;

  if (denom == 0.0f)
  {
    // The segments are parallel.
    rPoints.AddVertexOptim(Point3);
  }
  else
  {
    const double y13 = y1 - y3;
    const double x13 = x1 - x3;
    const double denom_inv = 1.0f/denom;
    const double ua = (x43 * y13 - y43 * x13) * denom_inv;
    const double ub = (x21 * y13 - y21 * x13) * denom_inv;

    if (mLineJoin == nuiLineJoinMiter || (ua >= 0.0f && ua <= 1.0f) && (ub >= 0.0f && ub <= 1.0f))
    {
      nuiPoint mitter((float)(x1 + ua*(x2-x1)), (float)(y1 + ua*(y2-y1)));
      rPoints.AddVertexOptim(mitter);
    }
    else if (mLineJoin == nuiLineJoinRound)
    {
      nuiArc arc(Point1, Point2,  mRealLineWidth, mRealLineWidth, 0, false, true);
      arc.Tessellate(rPoints, 0.5f);
    }
    else // Bevel
    {
      rPoints.AddVertexOptim(Point1);
      rPoints.AddVertexOptim(Point2);
    }
  }
}

void nuiOutliner::AddCap(const nuiPoint& rFirstPoint, const nuiPoint& rLastPoint, nuiPath& rPoints) const
{
  switch (mLineCap)
  {
  case nuiLineCapBut:
    // Nothing special to do
    rPoints.AddVertexOptim(rFirstPoint);
    rPoints.AddVertexOptim(rLastPoint);
    break;
  case nuiLineCapRound:
    {
      nuiArc arc(rFirstPoint, rLastPoint,  mRealLineWidth, mRealLineWidth, 0, true, true);
      arc.Tessellate(rPoints, 0.5f);
      //rPoints.AddVertexOptim(rFirstPoint);
    }
    break;
  case nuiLineCapSquare:
    {
      nuiVector vec = rLastPoint - rFirstPoint;
      vec.Normalize();
      float tmp = vec[0];
      vec[0] = vec[1];
      vec[1] = -tmp;
      vec *= mLineWidth * 0.5f;
      rPoints.AddVertexOptim(nuiPoint(rFirstPoint+vec));
      rPoints.AddVertexOptim(nuiPoint(rLastPoint+vec));
    }
    break;
  }
}

void nuiOutliner::Tessellate(nuiPath& rPoints, const nuiPath& rVertices, uint offset, int count, float Quality) const
{
  nuiPath Left;
  nuiPath Right;

  NGL_ASSERT(count);

  const nuiPoint& rstart = rVertices[offset];
  const nuiPoint& rstop = rVertices[offset + count - 1];
  bool closed = rstart == rstop;

  int segments = count;
  if (!closed)
    segments--;

  int i;
  // Create four outlined vertex per segment:
  for (i = 0; i < segments; i++)
  {
    int p1 = i;
    int p2 = (p1+1);
    p1 += offset;
    p2 += offset;

    const nuiPoint& rPoint = rVertices[p1];
    const nuiPoint& rNextPoint = rVertices[p2];

    if (!(rPoint == rNextPoint))
    {
      nuiVector vec(rNextPoint - rPoint);
      vec.Normalize();
      vec *= mRealLineWidth;
      float tmp = vec[0];
      vec[0] = vec[1];
      vec[1] = -tmp;

      Left.AddVertexOptim(nuiPoint(rPoint + vec));
      Left.AddVertexOptim(nuiPoint(rNextPoint + vec));
      Right.AddVertexOptim(nuiPoint(rPoint - vec));
      Right.AddVertexOptim(nuiPoint(rNextPoint - vec));
    }
  }

  Right.Reverse();

  if (!Left.GetCount() || !Right.GetCount()) // Case of a path where all points have the same coordinates
  {
    switch (mLineCap)
    {
      case nuiLineCapBut:
        // The result is empty.
        break;
      case nuiLineCapRound:
        {
          const double CIRCLE_FACTOR = (1.0/3.5);
          const double X = rstart[0];
          const double Y = rstart[1];

          uint count = ToAbove((double)(2.0 * M_PI * (double)mRealLineWidth * (double)CIRCLE_FACTOR));

          float step = 2.0f * (float)M_PI / (float)count;
          for (uint i = 0; i <= count; i++)
          {
            float angle = step * (float) i;
            float x = (float)(X + sin(angle) * mRealLineWidth);
            float y = (float)(Y + cos(angle) * mRealLineWidth);
            
            rPoints.AddVertexOptim(nuiPoint(x, y, 0));
          }
        }
        break;
      case nuiLineCapSquare:
        {
          const float x = rstart[0];
          const float y = rstart[1];
          rPoints.AddVertexOptim(nuiPoint(x - mRealLineWidth, y - mRealLineWidth));
          rPoints.AddVertexOptim(nuiPoint(x + mRealLineWidth, y - mRealLineWidth));
          rPoints.AddVertexOptim(nuiPoint(x + mRealLineWidth, y + mRealLineWidth));
          rPoints.AddVertexOptim(nuiPoint(x - mRealLineWidth, y + mRealLineWidth));
          rPoints.AddVertexOptim(nuiPoint(x - mRealLineWidth, y - mRealLineWidth));
        }
        break;
    }  
  }
  else if (closed && Left.GetCount() >= 2 && Right.GetCount() >= 2) // Case of a closed path (make sure that is at least a segment).
  {
    // Left:
    int ndx1 = 0;
    int ndx2 = 0;
    for (i = 0; i < segments-1; i++)
    {
      ndx1 = i * 2;
      ndx2 = ndx1 + 2;
      AddJoin(Left[ndx1], Left[ndx1+1], Left[ndx2], Left[ndx2+1], rPoints);
    }
    if (mLineJoin == nuiLineJoinBevel)
      rPoints.AddVertexOptim(rPoints.Front());
    else
      rPoints.Back() = rPoints.Front();
    rPoints.StopPath();

    // Right:
    for (i = 0; i < segments; i++)
    {
      ndx1 = i * 2;
      ndx2 = ndx1 + 2;
      AddJoin(Right[ndx1], Right[ndx1+1], Right[ndx2], Right[ndx2+1], rPoints);
    }
    rPoints.StopPath();
  }
  else
  {
    NGL_ASSERT(!Left.IsEmpty());
    NGL_ASSERT(!Right.IsEmpty());
    int cnt = segments - 1;
    // Left:
    rPoints.AddVertexOptim(Left.Front());
    int ndx1 = 0;
    int ndx2 = 0;
    for (i = 0; i < cnt; i++)
    {
      ndx1 = i * 2;
      ndx2 = ndx1 + 2;
      AddJoin(Left[ndx1], Left[ndx1+1], Left[ndx2], Left[ndx2+1], rPoints);
    }
    AddCap(Left.Back(), Right.Front(), rPoints);

    // Right:
    for (i = 0; i < cnt; i++)
    {
      ndx1 = i * 2;
      ndx2 = ndx1 + 2;
      AddJoin(Right[ndx1], Right[ndx1+1], Right[ndx2], Right[ndx2+1], rPoints);
    }
    AddCap(Right.Back(), Left.Front(), rPoints);
    rPoints.Front() = rPoints.Back();

    rPoints.StopPath();
  }
}

bool nuiOutliner::Tessellate(nuiPath& rPoints, float Quality) const
{
  nuiPath Vertices;
  bool res = mpPath->Tessellate(Vertices, Quality);

  rPoints.Clear();

  if (!res)
    return false;

  uint total = Vertices.GetCount();

  uint offset = 0;
  uint count = total;

  //Find sub path:
  for (uint i = offset; i < total; i++)
  {
    uint ii = i+1;
    if (Vertices[i].GetType() == nuiPointTypeStop || ii == total)
    {
      count = ii - offset;
      if (Vertices[i].GetType() == nuiPointTypeStop)
        count--;
      Tessellate(rPoints, Vertices, offset, count, Quality);

      offset = ii;
    }
  }

  return res;
}

void nuiOutliner::SetLineWidth(float width)
{
  mLineWidth = width;
  mRealLineWidth = mLineWidth * 0.5f;
}

float nuiOutliner::GetLineWidth() const
{
  return mLineWidth;
}

void nuiOutliner::SetLineJoin(nuiLineJoin join)
{
  mLineJoin = join;
//  Changed();
}

nuiLineJoin nuiOutliner::GetLineJoin()const
{
  return mLineJoin;
}

void nuiOutliner::SetLineCap(nuiLineCap cap)
{
  mLineCap = cap;
//  Changed();
}

nuiLineCap nuiOutliner::GetLineCap() const
{
  return mLineCap;
}

void nuiOutliner::SetMiterLimit(float limit)
{
  mMiterLimit = limit;
//  Changed();
}

float nuiOutliner::GetMiterLimit() const
{
  return mMiterLimit;
}

void nuiOutliner::SetPath(nuiPathGenerator* pPath)
{
  mpPath = pPath;
}

///////////////////
// Outline to nuiRenderObject:
static nuiRenderObject* gpCurrentObject = NULL;

void nui_glBegin(GLenum mode)
{
  static GLenum oldmode = 0;
  //if (oldmode != mode || !gpCurrentObject->GetSize())
  {
    nuiRenderArray* pArray = new nuiRenderArray(mode, false, false, true);
    gpCurrentObject->AddArray(pArray);
    //pArray->UseGLAATexture(true);
    //pArray->EnableArray(nuiRenderArray::eColor);
    pArray->EnableArray(nuiRenderArray::eVertex);
    pArray->EnableArray(nuiRenderArray::eTexCoord);

    oldmode = mode;
  }
}

void nui_glEnd()
{
  // Nothing special to do?
}

void nui_glColor4ubv(const GLubyte *v)
{
  gpCurrentObject->GetLastArray()->SetColor(v[3], v[2], v[1], v[0]);
}

void nui_glTexCoord2f(GLfloat s, GLfloat t)
{
  const float div = 1.0f / 127.0f;
  gpCurrentObject->GetLastArray()->SetTexCoords(div * s, div * t);
}

void nui_glVertex2f(GLfloat x, GLfloat y)
{
  gpCurrentObject->GetLastArray()->SetVertex(x, y, 0);
  gpCurrentObject->GetLastArray()->PushVertex();
}

#ifndef __NUI_NO_AA__
void nuiOutliner::TessellateObj(nuiRenderObject& rObject, const nuiPath& rVertices, uint offset, int count, float Quality) const
{
  NGL_ASSERT(count);


  const nuiPoint& rstart = rVertices[offset];
  const nuiPoint& rstop = rVertices[offset + count - 1];
  bool closed = rstart == rstop;

  if (count <= 2 && closed)
  {
    glAABegin(GL_POINTS);
  }
  else
  {
    glAABegin(GL_LINE_STRIP);
  }
  
  int i;
  // Create four outlined vertex per segment:
  for (i = 0; i < count; i++)
  {
    const nuiPoint& rPoint = rVertices[offset+i];
    glAAVertex2f(rPoint[0], rPoint[1]);
  }

  glAAEnd();
}

#else

void nuiOutliner::TessellateObj(nuiRenderObject& rObject, const nuiPath& rVertices, uint offset, int count, float Quality)
{
  NGL_ASSERT(count);

  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLE_STRIP, false, false, true);

  const nuiPoint& rstart = rVertices[offset];
  const nuiPoint& rstop = rVertices[offset + count - 1];
  bool closed = rstart == rstop;

  int segments = count;
  if (!closed)
    segments--;

  int i;
  // Create four outlined vertex per segment:
  for (i = 0; i < segments; i++)
  {
    int p1 = i;
    int p2 = (p1+1);
    p1 += offset;
    p2 += offset;

    const nuiPoint& rPoint = rVertices[p1];
    const nuiPoint& rNextPoint = rVertices[p2];

    if (!(rPoint == rNextPoint))
    {
      nuiVector vec(rNextPoint - rPoint);
      vec.Normalize();
      vec *= mRealLineWidth;
      float tmp = vec[0];
      vec[0] = vec[1];
      vec[1] = -tmp;

      pArray->SetVertex(rPoint + vec);
      pArray->PushVertex();
      
      pArray->SetVertex(rPoint - vec);
      pArray->PushVertex();

      pArray->SetVertex(rNextPoint + vec);
      pArray->PushVertex();

      pArray->SetVertex(rNextPoint - vec);
      pArray->PushVertex();

    }
  }

  rObject.AddArray(pArray);
}
#endif

bool nuiOutliner::TessellateObj(nuiRenderObject& rObject, float Quality) const
{
  gpCurrentObject = &rObject;

  nuiPath Vertices;
  bool res = mpPath->Tessellate(Vertices, Quality);

  if (!res)
    return false;

  uint total = Vertices.GetCount();

  uint offset = 0;
  uint count = total;

#ifndef __NUI_NO_AA__
  glAALineWidth(mLineWidth);
#endif

  //Find sub path:
  for (uint i = offset; i < total; i++)
  {
    uint ii = i+1;
    if (Vertices[i].GetType() == nuiPointTypeStop || ii == total)
    {
      count = ii - offset;
      if (Vertices[i].GetType() == nuiPointTypeStop)
        count--;
      TessellateObj(rObject, Vertices, offset, count, Quality);

      offset = ii;
    }
  }

  gpCurrentObject = NULL;

  return res;
}

