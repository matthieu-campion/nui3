/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

// nuiArc:
nuiArc::nuiArc(const nuiPoint& rStartVertex, const nuiPoint& rStopVertex, float XRadius, float YRadius, float Angle, bool LargeArc, bool Sweep)
 : mStartVertex(rStartVertex),
   mStopVertex(rStopVertex),
   mXRadius(XRadius),
   mYRadius(YRadius),
   mAngle(Angle),
   mLargeArc(LargeArc),
   mSweep(Sweep)
{
}

nuiArc::nuiArc(float cX, float cY, float rX, float rY, float Theta1, float Theta2, float Phi)
{
  // use -Theta1 and -Theta2 to reverse around the Y axis
  float T1 = (float)(-(M_PI * Theta1)/180.0);
  float T2 = (float)(-(M_PI * Theta2)/180.0);
  nuiVector start(rX * cos(T1), rY * sin(T1), 0.0f) , stop(rX * cos(T2), rY * sin(T2), 0.0f);

  nuiMatrix   m;
  m.SetRotation(Phi, 0.0f, 0.0f, 1.0f);

  mXRadius = rX;
  mYRadius = rY;

  mStartVertex = nuiPoint(m * start);
  mStopVertex = nuiPoint(m * stop);

  mStartVertex.Elt[0] += cX;
  mStartVertex.Elt[1] += cY;
  mStopVertex.Elt[0] += cX;
  mStopVertex.Elt[1] += cY;

  float DT = Theta1 - Theta2;
  mLargeArc = (DT > 180);
  mSweep = (DT > 0);

  mAngle = Phi;
}


nuiArc::nuiArc(nuiXMLNode* pNode)
{
  mStartVertex.Elt[0] = nuiGetVal(pNode, _T("X0"), 0.0f);
  mStartVertex.Elt[1] = nuiGetVal(pNode, _T("Y0"), 0.0f);
  mStartVertex.Elt[2] = nuiGetVal(pNode, _T("Z0"), 0.0f);
                                                  
  mStopVertex.Elt[0] = nuiGetVal(pNode, _T("X1"), 0.0f);
  mStopVertex.Elt[1] = nuiGetVal(pNode, _T("Y1"), 0.0f);
  mStopVertex.Elt[2] = nuiGetVal(pNode, _T("Z1"), 0.0f);

  mXRadius = nuiGetVal(pNode, _T("XRadius"), 0.0f);
  mYRadius = nuiGetVal(pNode, _T("YRadius"), 0.0f);
  mAngle = nuiGetVal(pNode, _T("Angle"), 0.0f);
  mLargeArc = nuiGetBool(pNode, _T("LargeArc"), false);
  mSweep = nuiGetBool(pNode, _T("Sweep"), false);
}

nuiArc::~nuiArc()
{
}

nuiXMLNode* nuiArc::Serialize(nuiXMLNode* pParentNode) const
{
  nuiXMLNode* pNode = NULL;
  if (pParentNode)
    pNode = new nuiXMLNode(_T("nuiArc"),pParentNode);
  else
    pNode = new nuiXML(_T("nuiArc"));

  pNode->SetAttribute(_T("X0"),mStartVertex.Elt[0]);
  pNode->SetAttribute(_T("Y0"),mStartVertex.Elt[1]);
  if (mStartVertex.Elt[2] != 0.0f) pNode->SetAttribute(_T("Z0"), mStartVertex.Elt[2]);

  pNode->SetAttribute(_T("X1"),mStopVertex.Elt[0]);
  pNode->SetAttribute(_T("Y1"),mStopVertex.Elt[1]);
  if (mStopVertex.Elt[2] != 0.0f) pNode->SetAttribute(_T("Z1"), mStopVertex.Elt[2]);

  if (mXRadius != 0.0f) pNode->SetAttribute(_T("XRadius"), mXRadius);
  if (mYRadius != 0.0f) pNode->SetAttribute(_T("YRadius"), mYRadius);
  if (mAngle != 0.0f) pNode->SetAttribute(_T("Angle"), mAngle);
  if (mLargeArc) pNode->SetAttribute(_T("LargeArc"), mLargeArc);
  if (mSweep) pNode->SetAttribute(_T("Sweep"), mSweep);

  return pNode;
}

bool nuiArc::Tessellate(nuiPath& rVertices, float Quality) const
{
  // I shamelessly stole this code from svgl
  double x1, y1, x2, y2;
  x1 = mStartVertex.Elt[0];
  y1 = mStartVertex.Elt[1];
  x2 = mStopVertex.Elt[0];
  y2 = mStopVertex.Elt[1];
  double angle = mAngle;
  double xr, yr;
  xr = mXRadius;
  yr = mYRadius;

  rVertices.AddVertexOptim(nuiPoint((float)x1, (float)y1));

  if ((x1 == x2) && (y1 == y2) && !mLargeArc)
  {
    return true;
  }

  // be sure mXRadius or mYRadius are non-zero
  if( mXRadius == 0 || mYRadius == 0) 
  {
    rVertices.AddVertexOptim(nuiPoint((float)x2, (float)y2));
    return true;
  }

  // make sure mXRadius and mYRadius are positive
  if(xr < 0)
    xr = -xr;
  if(yr < 0)
    yr = -yr;

  const double cosa = cos(angle * M_PI / 180.0);
  const double sina = sin(angle * M_PI / 180.0);


  // compute the center (see svg.pdf)
  const double xp1 = cosa * (x1 - x2) / 2.0 + sina * (y1 - y2) / 2.f;
  const double yp1 = -sina * (x1 - x2) / 2.0 + cosa * (y1 - y2) / 2.f;

  double rx2 = xr * xr, ry2 = yr * yr, xp12 = xp1 * xp1, yp12 = yp1 * yp1;

  // make sure xr and yr are large enough
  {
    double tmp = xp12 / rx2 + yp12 / ry2;
    if (tmp > 1)
    {
      rx2 *= tmp;
      ry2 *= tmp;
      tmp = sqrt(tmp);
      xr *= tmp;
      yr *= tmp;
    }
  }

  double fact = ( rx2 * ry2 / (rx2 * yp12 + ry2 * xp12)) - 1;
  if (fact < 0) 
  {
    fact = 0;
  }

  fact = sqrt(fact);
  if (mLargeArc == mSweep)
    fact = -fact;

  double xpc = xr * yp1 / yr;
  double ypc =  - yr * xp1 / xr;

  xpc *= fact;
  ypc *= fact;

  double xc = xpc * cosa - ypc * sina + (x1 + x2) / 2.f;
  double yc = xpc * sina + ypc * cosa + (y1 + y2) / 2.f;

  // determine t1 and t2, limits given by (x1, y1) (x2, y2)

  double t1;
  double deltat;
  //    double t2;
  {
    double xv1 = (xp1 - xpc) / xr;
    double yv1 = (yp1 - ypc) / yr;
    double norm1 = xv1 * xv1 + yv1 * yv1;

    double cosangle1 = xv1 / sqrt(norm1);
    if (cosangle1 < -1.0f)
      cosangle1 = -1.0f;
    if (cosangle1 > 1.0f)
      cosangle1 = 1.0f;
    t1 = acos(cosangle1);
    if (yv1 < 0)
      t1 = -t1;

    double xv2 = (-xp1 - xpc) / xr;
    double yv2 = (-yp1 - ypc) / yr;
    double norm2 = xv2 * xv2 + yv2 * yv2;
    deltat = xv1 * xv2 + yv1 * yv2;
    deltat = deltat / sqrt(norm1 * norm2);
    if (deltat < -1.0)
      deltat = -1.0;
    if (deltat > 1.0)
      deltat = 1.0;
    deltat = acos(deltat);
    if ( (xv1 * yv2 - yv1 * xv2) < 0)
      deltat = -deltat;

    if (!mSweep && (deltat > 0))
      deltat -= 2.0 * M_PI;

    if (mSweep && (deltat < 0))
      deltat += 2.0 * M_PI;
  }


  // compute vertices
  /*
  fast incremental cos and sin generation

  cos(a+dt) = cos(a) cos(dt) - sin(a)  sin(dt);
  sin(a+dt) = cos(a) sin(dt) + cos(dt) sin(a);

  <=>
  cos(a+2*dt) = cos(a + dt) cos(dt) - sin(a + dt) sin(dt)
  sin(a+2*dt) = cos(a + dt) sin(dt) + cos(a + dt) sin(a)
  */

  const int nb = ToBelow(M_PI * (xr + yr) * Quality);
  const double dt = deltat / nb;
  const double cosdt = cos(dt);
  const double sindt = sin(dt);
  double cost1_plus_ndt = cos(t1 + dt);
  double sint1_plus_ndt = sin(t1 + dt);

  for (int i = 0; i < nb; ++i) 
  {
    /*
      ellipsoid:
      x(t) = xr.cos(t)
      y(t) = yr.cos(t)
    */


    double x = xr * cost1_plus_ndt;
    double y = yr * sint1_plus_ndt;

    {
      // compute cos and sin
      double tmp = cost1_plus_ndt * cosdt - sint1_plus_ndt * sindt;
      sint1_plus_ndt = cost1_plus_ndt * sindt + sint1_plus_ndt * cosdt; 
      cost1_plus_ndt = tmp;
    }

    // rotate
    // angle = -angle, I don't know why...
    {
      double tmp = x * cosa - y * sina;
      y = x * sina + y * cosa;
      x = tmp;
    }

    // translate
    x += xc;
    y += yc;

    rVertices.AddVertexOptim(nuiPoint((float)x, (float)y));
  }
  return true;
}

nuiPoint nuiArc::GetStartPoint() const
{
  return mStartVertex;
}

nuiPoint nuiArc::GetEndPoint() const
{
  return mStopVertex;
}
