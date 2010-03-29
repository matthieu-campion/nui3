/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiSpline.h"

using namespace std;

//////////// nuiSplineNode:
nuiSplineNode::nuiSplineNode(float x, float y, float z )
{
  SetPosition(nuiVector(x,y,z));

  mBias = 0;
  mContinuity = 0;
  mTension = 0;
}

nuiSplineNode::nuiSplineNode(const nuiSplineNode& rNode)
{
  *this = rNode;
}

nuiSplineNode::nuiSplineNode(nuiXMLNode* pNode)
{
  mBias = 0;
  mContinuity = 0;
  mTension = 0;
  if (pNode->GetName() != _T("nuiSplineNode"))
    return;

  mBias = nuiGetVal(pNode, _T("Bias"),0.0f);
  mContinuity = nuiGetVal(pNode, _T("Continuity"),0.0f);
  mTension = nuiGetVal(pNode, _T("Tension"),0.0f);

  mPosition[0] = nuiGetVal(pNode, _T("X"),0.0f);
  mPosition[1] = nuiGetVal(pNode, _T("Y"),0.0f);
  mPosition[2] = nuiGetVal(pNode, _T("Z"),0.0f);

  mTangentIn[0] = nuiGetVal(pNode, _T("TanInX"),0.0f);
  mTangentIn[1] = nuiGetVal(pNode, _T("TanInY"),0.0f);
  mTangentIn[2] = nuiGetVal(pNode, _T("TanInZ"),0.0f);

  mTangentOut[0] = nuiGetVal(pNode, _T("TanOutX"),0.0f);
  mTangentOut[1] = nuiGetVal(pNode, _T("TanOutY"),0.0f);
  mTangentOut[2] = nuiGetVal(pNode, _T("TanOutZ"),0.0f);

  const nuiXMLNodeList& children = pNode->GetChildren();

  nuiXMLNodeList::const_iterator it;
  nuiXMLNodeList::const_iterator end = children.end();
}

nuiXMLNode* nuiSplineNode::Serialize(nuiXMLNode* pParentNode, nuiSplineMode Mode) const ///< This method saves the state in the given XML Node.
{
  nuiXMLNode* pNode = NULL;
  if (pParentNode)
    pNode = new nuiXMLNode(_T("nuiSplineNode"),pParentNode);
  else
    pNode = new nuiXML(_T("nuiSplineNode"));

  pNode->SetAttribute(_T("X"),mPosition[0]);
  pNode->SetAttribute(_T("Y"),mPosition[1]);
  pNode->SetAttribute(_T("Z"),mPosition[2]);

  if (Mode == nuiSplineModeFree || Mode == nuiSplineModeBezierCubic)
  {
    pNode->SetAttribute(_T("TanInX"),mTangentIn[0]);
    pNode->SetAttribute(_T("TanInY"),mTangentIn[1]);
    pNode->SetAttribute(_T("TanInZ"),mTangentIn[2]);

    pNode->SetAttribute(_T("TanOutX"),mTangentOut[0]);
    pNode->SetAttribute(_T("TanOutY"),mTangentOut[1]);
    pNode->SetAttribute(_T("TanOutZ"),mTangentOut[2]);
  }
  else if (Mode == nuiSplineModeBezierQuadratic)
  {
    pNode->SetAttribute(_T("TanOutX"),mTangentOut[0]);
    pNode->SetAttribute(_T("TanOutY"),mTangentOut[1]);
    pNode->SetAttribute(_T("TanOutZ"),mTangentOut[2]);
  }
  else if (Mode == nuiSplineModeTCB)
  {
    pNode->SetAttribute(_T("Tension"),mTension);
    pNode->SetAttribute(_T("Continuity"),mContinuity);
    pNode->SetAttribute(_T("Bias"),mBias);
  }

  return pNode;
}


nuiSplineNode::~nuiSplineNode()
{
}

void nuiSplineNode::SetPosition(const nuiVector& rVector)
{
  mPosition = rVector;
  Changed();
}

const nuiVector& nuiSplineNode::GetPosition() const 
{
  return mPosition;
}

void nuiSplineNode::SetTangent(const nuiVector& rTangent)
{
  mTangentIn  = rTangent;
  mTangentOut = rTangent;
  Changed();
}

void nuiSplineNode::SetIncomingTangent(const nuiVector& rTangent)
{
  mTangentIn  = rTangent;
  Changed();
}

void nuiSplineNode::SetOutgoingTangent(const nuiVector& rTangent)
{
  mTangentOut = rTangent;
  Changed();
}

void nuiSplineNode::SetTangent(const nuiVector& rTangent) const
{
  mTangentIn  = rTangent;
  mTangentOut = rTangent;
  Changed();
}

void nuiSplineNode::SetIncomingTangent(const nuiVector& rTangent) const
{
  mTangentIn  = rTangent;
  Changed();
}

void nuiSplineNode::SetOutgoingTangent(const nuiVector& rTangent) const
{
  mTangentOut = rTangent;
  Changed();
}

nuiVector nuiSplineNode::GetTangent() const
{
  return mTangentIn;
}

nuiVector nuiSplineNode::GetIncomingTangent() const
{
  return mTangentIn;
}

nuiVector nuiSplineNode::GetOutgoingTangent() const
{
  return mTangentOut;
}

void nuiSplineNode::SetBias(float Bias)
{
  if (mBias == Bias)
    return;
  mBias = Bias;
  Changed();
}

float nuiSplineNode::GetBias() const
{
  return mBias;
}

void nuiSplineNode::SetContinuity(float Continuity)
{
  if (mContinuity == Continuity)
    return;
  mContinuity = Continuity;
  Changed();
}

float nuiSplineNode::GetContinuity() const
{
  return mContinuity;
}

nuiSplineNode& nuiSplineNode::operator = (const nuiSplineNode& rNode)
{
  mPosition.Elt[0] = rNode.mPosition.Elt[0];
  mPosition.Elt[1] = rNode.mPosition.Elt[1];
  mPosition.Elt[2] = rNode.mPosition.Elt[2];
  mPosition.Elt[3] = 1.0;

  mTangentIn  = rNode.mTangentIn;
  mTangentOut = rNode.mTangentOut;
  mBias = rNode.mBias;
  mContinuity = rNode.mContinuity;

  return *this;
}

const nuiVector& nuiSplineNode::operator = (const nuiVector& rVector)
{
  mPosition.Elt[0] = rVector.Elt[0];
  mPosition.Elt[1] = rVector.Elt[1];
  mPosition.Elt[2] = rVector.Elt[2];

  return rVector;
}

void nuiSplineNode::RotateTangent(float angle, float x, float y, float z)
{
  nuiMatrix mat;
  mat.SetRotation(angle, x,y,z);
  mTangentIn = mat * mTangentIn;
  mTangentOut = mat * mTangentOut;
}

void nuiSplineNode::RotateIncomingTangent(float angle, float x, float y, float z)
{
  nuiMatrix mat;
  mat.SetRotation(angle, x,y,z);
  mTangentIn = mat * mTangentIn;
}

void nuiSplineNode::RotateOutgoingTangent(float angle, float x, float y, float z)
{
  nuiMatrix mat;
  mat.SetRotation(angle, x,y,z);
  mTangentOut = mat * mTangentOut;
}

void nuiSplineNode::ScaleTangent(float x, float y, float z)
{
  nuiMatrix mat;
  mat.SetScaling(x,y,z);
  mTangentIn = mat * mTangentIn;
  mTangentOut = mat * mTangentOut;
}

void nuiSplineNode::ScaleIncomingTangent(float x, float y, float z)
{
  nuiMatrix mat;
  mat.SetScaling(x,y,z);
  mTangentIn = mat * mTangentIn;
}

void nuiSplineNode::ScaleOutgoingTangent(float x, float y, float z)
{
  nuiMatrix mat;
  mat.SetScaling(x,y,z);
  mTangentOut = mat * mTangentOut;
}

//////////// nuiSpline :
nuiSpline::nuiSpline()
: mEventSink(this)
{
  mChanged = false;
  mCardinalTightness = 0.5f;
  mMode = nuiSplineModeCatmullRom;
  mCacheValid = false;
  mLastQuality = 0;

  mEventSink.Connect(InternalChanged, &nuiSpline::ElementChanged);
  mEventSink.Connect(Changed, &nuiSpline::InvalidateCache);
}

nuiSpline::nuiSpline(const nuiSpline& rSpline)
: mEventSink(this)
{
  mEventSink.Connect(InternalChanged, &nuiSpline::ElementChanged);
  mEventSink.Connect(Changed, &nuiSpline::InvalidateCache);
  mCacheValid = false;
  mLastQuality = 0;

  *this = rSpline;
}

nuiSpline& nuiSpline::operator=(const nuiSpline& rSpline)
{
  mChanged = true;
  mCardinalTightness = rSpline.mCardinalTightness;
  mMode = rSpline.mMode;
  mCacheValid = rSpline.mCacheValid;
  mLastQuality = rSpline.mLastQuality;
  mTessCache = rSpline.mTessCache;

  std::vector<nuiSplineNode>::const_iterator it;
  std::vector<nuiSplineNode>::const_iterator end = rSpline.mNodes.end();

  for (it = rSpline.mNodes.begin(); it != end; ++it)
    mNodes.push_back(*it);
  return *this;
}

nuiSpline::nuiSpline(nuiXMLNode* pNode)
: mEventSink(this)
{
  mEventSink.Connect(InternalChanged, &nuiSpline::ElementChanged);
  mEventSink.Connect(Changed, &nuiSpline::InvalidateCache);

  mChanged = false;
  mCardinalTightness = 0.5f;
  mMode = nuiSplineModeCatmullRom;
  if (pNode->GetName() != _T("nuiSpline"))
    return;

  nglString mode = pNode->GetAttribute(_T("Mode"));
  if (mode == _T("Free"))
    mMode = nuiSplineModeFree;
  else if (mode == _T("CatmullRom"))
    mMode = nuiSplineModeCatmullRom;
  else if (mode == _T("TCB"))
    mMode = nuiSplineModeTCB;
  else if (mode == _T("CubicBezier"))
    mMode = nuiSplineModeBezierCubic;
  else if (mode == _T("QuadraticBezier"))
    mMode = nuiSplineModeBezierQuadratic;
  else if (mode == _T("Cardinal"))
  {
    mMode = nuiSplineModeCardinal;
    mCardinalTightness = nuiGetVal(pNode,_T("CardinalTightness"),0.5f);
  }

  const nuiXMLNodeList& children = pNode->GetChildren();

  nuiXMLNodeList::const_iterator it;
  nuiXMLNodeList::const_iterator end = children.end();

  for (it = children.begin(); it != end; ++it)
  {
    nuiXMLNode* pChild = *it;
    if (pChild)
    {
      nglString name(pChild->GetName()); 
      if (name == _T("nuiSplineNode"))
      {
        nuiSplineNode SplineNode(pChild);
        AddNode(SplineNode);
      }
    }
  }

  mCacheValid = false;
  mLastQuality = 0;
}

nuiSpline::~nuiSpline()
{
}

nuiXMLNode* nuiSpline::Serialize(nuiXMLNode* pParentNode) const
{
  /*
  if (mChanged)
  {
    switch (mMode)
    {
    case CatmullRom:
      CalculateCatmullRomTangents();
      break;
    case Cardinal:
      CalculateCardinalTangents(mCardinalTightness);
      break;
    case TCB:
      CalculateTCBTangents();
      break;
    case Free:
      // Nothing to do, the tangents should be ok...
      break;
    };
    mChanged = false;
  }
  */

  nuiXMLNode* pNode = NULL;
  if (pParentNode)
    pNode = new nuiXMLNode(_T("nuiSpline"),pParentNode);
  else
    pNode = new nuiXML(_T("nuiSpline"));

  nglString mode;
  switch (mMode)
  {
  case nuiSplineModeFree:
    mode = "Free";
    break;
  case nuiSplineModeCatmullRom:
    mode = "CatmullRom";
    break;
  case nuiSplineModeCardinal:
    mode = "Cardinal";
    pNode->SetAttribute(_T("CardinalTightness"),mCardinalTightness);
    break;
  case nuiSplineModeTCB:
    mode = "TCB";
    break;
  case nuiSplineModeBezierCubic:
    mode = "CubicBezier";
  case nuiSplineModeBezierQuadratic:
    mode = "QuadraticBezier";
    break;
  }

  pNode->SetAttribute(_T("Mode"),mode);

  std::vector<nuiSplineNode>::const_iterator it;
  std::vector<nuiSplineNode>::const_iterator end = mNodes.end();
  for (it = mNodes.begin(); it != end; ++it)
    (*it).Serialize(pNode,mMode);

  return pNode;
}

void nuiSpline::InsertNode(const nuiSplineNode& rNode, uint32 index )
{
  std::vector<nuiSplineNode>::iterator it;
  std::vector<nuiSplineNode>::iterator end = mNodes.end();
  for (it = mNodes.begin(); it != end && index; ++it)
    index --;
  it = mNodes.insert(it, rNode);
  mEventSink.Connect( (*it).Changed, &nuiSpline::ElementChanged );
  InternalChanged();
}

void nuiSpline::InsertNodes(std::vector<nuiSplineNode> rNodes, uint32 index )
{
  std::vector<nuiSplineNode>::iterator newit = rNodes.begin();
  std::vector<nuiSplineNode>::iterator newend = rNodes.end();
  std::vector<nuiSplineNode>::iterator it;
  std::vector<nuiSplineNode>::iterator end = mNodes.end();
  for (it = mNodes.begin(); it != end && index; ++it)
    index --;
  
  while (newit!=newend)
  {
    it = mNodes.insert(it, (*newit));
    mEventSink.Connect( (*it).Changed, &nuiSpline::ElementChanged );
    ++newit;
  }
  InternalChanged();
}

void nuiSpline::AddNode(const nuiSplineNode& rNode)
{
  mNodes.push_back(rNode);
  mEventSink.Connect( mNodes.back().Changed, &nuiSpline::ElementChanged );
  InternalChanged();
}

void nuiSpline::AddNodes(std::vector<nuiSplineNode> rNodes)
{
  std::vector<nuiSplineNode>::iterator newit = rNodes.begin();
  std::vector<nuiSplineNode>::iterator newend = rNodes.end();
  while (newit!=newend)
  {
    mNodes.push_back((*newit));
    mEventSink.Connect( mNodes.back().Changed, &nuiSpline::ElementChanged );
    ++newit;
  }
  InternalChanged();
}


void nuiSpline::DeleteNode(uint32 index)
{
  std::vector<nuiSplineNode>::iterator it;
  std::vector<nuiSplineNode>::iterator end = mNodes.end();
  for (it = mNodes.begin(); it != end && index; ++it)
    index --;
  
  mEventSink.DisconnectSource( (*it).Changed );
  mNodes.erase(it);
  InternalChanged();
}

void nuiSpline::DeleteNodes(uint32 index,uint32 count)
{
  for (;count; count--)
  {
    std::vector<nuiSplineNode>::iterator it;
    std::vector<nuiSplineNode>::iterator end = mNodes.end();
    for (it = mNodes.begin(); it != end && index; ++it)
      index --;
    mEventSink.DisconnectSource( (*it).Changed );
    mNodes.erase(it);
  }
  InternalChanged();
}

void nuiSpline::DeleteNodes(std::vector<uint32> indices)
{
  std::vector<uint32>::iterator it;
  std::vector<uint32>::iterator end = indices.end();

  for (it = indices.begin(); it != end; ++it)
    DeleteNode(*it);
  Changed();
}

void nuiSpline::DeleteAll()
{
  mNodes.clear();
  InternalChanged();
}

nuiSplineNode* nuiSpline::GetNode(uint32 index)
{
  NGL_ASSERT(index < mNodes.size());
  return &mNodes[index];
}

nuiSplineNode* nuiSpline::GetNode(nuiVector vec, float maxdist )
{
  float dist = 0;
  nuiVector temp;
  nuiSplineNode* pNode = NULL;

  std::vector<nuiSplineNode>::iterator it = mNodes.begin();
  std::vector<nuiSplineNode>::iterator end = mNodes.end();

  if (it!=end)
  {
    temp = (*it).GetPosition();
    temp -= vec;
    dist = temp.Length();
    pNode = &(*it);
  }

  for (; it != end; ++it)
  {
    float tempdist;
    temp = (*it).GetPosition();
    temp -= vec;
    tempdist = temp.Length();
    if (tempdist < dist)
    {
      dist = tempdist;
      pNode = &(*it);
    }
  }
  if (dist < maxdist)
    return pNode;
  else
    return NULL;
}

const nuiSplineNode* nuiSpline::GetNode(uint32 index) const
{
  NGL_ASSERT(index < mNodes.size());
  return &mNodes[index];
}

const nuiSplineNode* nuiSpline::GetNode(nuiVector vec, float maxdist ) const
{
  float dist = 0;
  nuiVector temp;
  const nuiSplineNode* pNode = NULL;

  std::vector<nuiSplineNode>::const_iterator it = mNodes.begin();
  std::vector<nuiSplineNode>::const_iterator end = mNodes.end();

  if (it!=end)
  {
    temp = (*it).GetPosition();
    temp -= vec;
    dist = temp.Length();
    pNode = &(*it);
  }

  for (; it != end; ++it)
  {
    float tempdist;
    temp = (*it).GetPosition();
    temp -= vec;
    tempdist = temp.Length();
    if (tempdist < dist)
    {
      dist = tempdist;
      pNode = &(*it);
    }
  }
  if (dist < maxdist)
    return pNode;
  else
    return NULL;
}


/* 
// Linear interpolation:
nuiVector nuiSpline::Evaluate(float Value)
{
  nuiVector start = GetNode((uint32)Value);
  nuiVector end   = GetNode((uint32)Value + 1);
  Value -= (uint32) Value;
  start *= 1 - Value;
  end *= Value;
  start += end;
  return start;
}
*/

uint32 nuiSpline::GetNodes(vector<nuiSplineNode>&rNodes, uint32 index , int32 count ) const
{
  rNodes.clear();
  if (count < 0)
    count = mNodes.size();
  NGL_ASSERT(index < mNodes.size());
  NGL_ASSERT(index + count <= mNodes.size());
  for (uint i = index; i < (uint32)count; i++)
    rNodes.push_back(*GetNode(i));
  return index;
}

nuiSplinePoint nuiSpline::Evaluate(float Value) const
{
  nuiSplinePoint result;
  uint32 p1 = ToZero(Value);
  uint32 p2 = p1+1;
  const nuiSplineNode* pN1 = GetNode(p1);
  const nuiSplineNode* pN2 = NULL;

  uint size = mNodes.size();
  if (p2 < size)
    pN2 = GetNode(p2);

  if (!pN1)
  {
    if (!pN2)
    {
      result = nuiVector(0,0,0);
      return result;
    }
    else
    {
      result = *pN2;
      return result;
    }
  }
  else if (!pN2)
  {
      result = *pN1;
      return result;
  }

  switch (mMode)
  {
  case nuiSplineModeFree:
  case nuiSplineModeCatmullRom:
  case nuiSplineModeCardinal:
  case nuiSplineModeTCB:
    {
      nuiVector start = pN1->GetPosition();
      nuiVector P1 = start;
      nuiVector T1 = pN1->GetOutgoingTangent();
      nuiVector P2 = pN2->GetPosition();
      nuiVector T2 = pN2->GetIncomingTangent();
      Value -= ToZero(Value);
      float s = Value;
      float s2 = s*s;
      float s3 = s2*s;
      float a  = -2*s3 + 3*s2;
      float h1 = 1.0f - a; // calculate basis function 1
      float h2 = a;     // calculate basis function 2
      float h3 =    s3 - 2*s2 + s; // calculate basis function 3
      float h4 =    s3 -  s2;      // calculate basis function 4
      //result = start;
      P1 *= h1;
      P2 *= h2;
      T1 *= h3;
      T2 *= h4;
      result += P1;
      result += P2;
      result += T1;
      result += T2;
    }
    break;
  case nuiSplineModeBezierCubic:
    {
      /*
      ____________________________________________
      4 Basis functions for a cubic bezier spline|
      --------------------------------------------

      B1(t) = t * t * t
      B2(t) = 3 * t * t * (1 - t)
      B3(t) = 3 * t * (1 - t) * (1 - t)
      B4(t) = (1 - t) * (1 - t) * (1 - t)
      */

      nuiVector start = pN1->GetPosition();
      nuiVector P1 = start;
      nuiVector T1 = pN1->GetOutgoingTangent();
      T1 += P1;
      nuiVector P2 = pN2->GetPosition();
      nuiVector T2 = pN2->GetIncomingTangent();
      T2 += P2;
      Value -= ToZero(Value);
      float b1,b2,b3,b4;
      float oneminusval = 1 - Value;

      b2 = Value*Value;
      b1 = b2*Value;
      b2 *= 3*(oneminusval);
      b3 = oneminusval * oneminusval;
      b4 = b3 * oneminusval;
      b3 *= 3 * Value;

      P1 *= b4;
      T1 *= b3;
      T2 *= b2;
      P2 *= b1;

      result += P1;
      result += T1;
      result += T2;
      result += P2;
    }
    break;
  case nuiSplineModeBezierQuadratic:
    {
      /*
      ____________________________________________
      3 Basis functions for a quadratic bezier spline|
      --------------------------------------------

      B1(t) = t * t
      B2(t) = 2 * t * (1 - t)
      B3(t) = (1 - t) * (1 - t)
      */

      nuiVector start = pN1->GetPosition();
      nuiVector P1 = start;
      nuiVector T1 = pN1->GetOutgoingTangent();
      T1 += P1;
      nuiVector P2 = pN2->GetPosition();
      nuiVector T2 = pN2->GetIncomingTangent();
      T2 += P2;
      Value -= ToZero(Value);
      float b1,b2,b3;
      float oneminusval = 1 - Value;

      b1 = Value * Value;
      b2 = 2.0f * Value * (oneminusval);
      b3 = oneminusval * oneminusval;

      P1 *= b3;
      T1 *= b2;
      //T2 *= b2;
      P2 *= b1;

      result += P1;
      result += T1;
      //result += T2;
      result += P2;
    }
    break;
  }

  return result;
}

// .007 radians threshold looks OK.
#define NUI_SPLINE_ANGLE_THRESHOLD .007f
// 4 pixels mini in between two actual points.
#define NUI_SPLINE_DIST_THRESHOLD 4.0f

void nuiSpline::Evaluate(vector<nuiSplinePoint>& rPoints, uint steps, bool optimize) const
{
  std::vector<nuiSplinePoint> Points;
  std::vector<nuiSplinePoint>* pPoints = &rPoints;
  if (optimize)
    pPoints = &Points;
  
  PrecalcTangents();
  if (!GetSize())
    return;
  double end = (double)GetSize()-1;
  double step = end / (double)steps;
  double val = 0;
  uint i;
  for (i=0; i<=steps; i++)
  {
    pPoints->push_back(Evaluate((float)val));
    val+=step;
  }

  // Remove unneeded points:
  if (optimize)
  {
    std::vector<nuiSplinePoint>::iterator it;
    std::vector<nuiSplinePoint>::iterator rend = Points.end();
    it = Points.begin();
    nuiSplinePoint p1,p2 = *it,p3;
    rPoints.push_back(p2);
    bool first = true;
    bool added = false;
    for (++it; it!=rend; ++it)
    {
      p3= *it;
      if (!first)
      {
        nuiVector v1 = p2;
        v1 -= p1;
        nuiVector v2 = p3;
        v2 -= p2;

        float prodscal = v1 * v2;
        float angle = (float)acos(prodscal / (v1.Length() * v2.Length()));
        if ( angle > NUI_SPLINE_ANGLE_THRESHOLD && v2.Length() >= NUI_SPLINE_DIST_THRESHOLD)
        {
          rPoints.push_back(p3);
          p1 = p2;
          p2 = p3;
          added = true;
        }
        else
        {
          added = false;
        }
      }
      else
      {
        first = false;
        rPoints.push_back(p3);
        p1 = p2;
        p2 = p3;
      }
    }
    if (!added)
      rPoints.push_back(Points.back());
  }
}

void nuiSpline::PrecalcTangents() const
{
  if (mChanged)
  {
    switch (mMode)
    {
    case nuiSplineModeCatmullRom:
      CalculateCatmullRomTangents();
      break;
    case nuiSplineModeCardinal:
      CalculateCardinalTangents(mCardinalTightness);
      break;
    case nuiSplineModeTCB:
      CalculateTCBTangents();
      break;
    case nuiSplineModeFree:
    case nuiSplineModeBezierCubic:
    case nuiSplineModeBezierQuadratic:
      // Nothing to do, the tangents should be ok...
      break;
    };
    mChanged = false;
  }
}

void nuiSpline::CalculateCardinalTangents(float tightness) const
{
  std::vector<nuiSplineNode>::const_iterator first = mNodes.begin();
  std::vector<nuiSplineNode>::const_iterator last = mNodes.end();
  if (last == first)
    return;
  --last;
  if (last == first)
    return;

  std::vector<nuiSplineNode>::const_iterator it;
  std::vector<nuiSplineNode>::const_iterator next;
  std::vector<nuiSplineNode>::const_iterator previous;
  std::vector<nuiSplineNode>::const_iterator end = mNodes.end();
  for (it = first; it != end; ++it)
  {
    if (it==first)
    {
      next = it;
      ++next;
      nuiVector tangent = (*next).GetPosition();
      tangent-= (*it).GetPosition();
      (*it).SetTangent(tangent);
    }
    else if (it==last)
    {
      previous = it;
      --previous;
      nuiVector tangent = (*it).GetPosition();
      tangent-= (*previous).GetPosition();
      (*it).SetTangent(tangent);
    }
    else
    {
      next = it;
      ++next;
      previous = it;
      --previous;
      nuiVector tangent = (*next).GetPosition();
      tangent -= (*previous).GetPosition();
      tangent *= tightness;
      (*it).SetTangent(tangent);
    }
  }
  InternalChanged();
}


void nuiSpline::CalculateCatmullRomTangents() const
{
  CalculateCardinalTangents(0.5f);
}

void nuiSpline::CalculateTCBTangents() const
{
}


uint32 nuiSpline::GetSize() const
{
  return mNodes.size();
}


nuiRect nuiSpline::GetRect()
{
  float xmin = 0,ymin = 0,xmax = 0,ymax = 0;
  std::vector<nuiSplineNode>::iterator it = mNodes.begin();
  std::vector<nuiSplineNode>::iterator end = mNodes.end();

  if (it!=end)
  {
    xmin = xmax = ((*it).GetPosition())[0];
    ymin = ymax = ((*it).GetPosition())[1];
  }

  for ( ; it != end; ++it)
  {
    xmin = MIN((*it).GetPosition()[0],xmin);
    xmax = MAX((*it).GetPosition()[0],xmax);
    ymin = MIN((*it).GetPosition()[1],ymin);
    ymax = MAX((*it).GetPosition()[1],ymax);
  }
  return nuiRect(xmin, ymin, xmax - xmin, ymax - ymin);
}


void nuiSpline::Translate(const nuiVector& rTranslation)
{
  nuiMatrix matrix;
  matrix.SetTranslation((nuiVector&)rTranslation);
  Multiply(matrix);
}

void nuiSpline::Rotate(float angle, float x, float y, float z)
{
  nuiMatrix matrix;
  matrix.SetRotation(angle, x,y,z);
  Multiply(matrix);
}

void nuiSpline::Scale(float x, float y, float z)
{
  nuiMatrix matrix;
  matrix.SetScaling(x,y,z);
  Multiply(matrix);
}

void nuiSpline::Multiply(const nuiMatrix& matrix)
{
  std::vector<nuiSplineNode>::iterator it;
  std::vector<nuiSplineNode>::iterator end = mNodes.end();

  for (it = mNodes.begin(); it != end; ++it)
  {
    (*it).SetPosition(((nuiMatrix)matrix) * (*it).GetPosition());
    (*it).SetIncomingTangent(((nuiMatrix)matrix) * (*it).GetIncomingTangent());
    (*it).SetOutgoingTangent(((nuiMatrix)matrix) * (*it).GetOutgoingTangent());
  }
  InternalChanged();
}

bool nuiSpline::ElementChanged(const nuiEvent& rEvent)
{
  mChanged = true;
  Changed();
  return false;
}

void nuiSpline::SetCardinalMode(float tightness)
{
  mMode = nuiSplineModeCardinal;
  mCardinalTightness = tightness;
  InternalChanged();
}

void nuiSpline::SetCatmullRomMode()
{
  mMode = nuiSplineModeCatmullRom;
  mCardinalTightness = 0.5f;
  InternalChanged();
}

void nuiSpline::SetTCBMode()
{
  mMode = nuiSplineModeTCB;
  InternalChanged();
}

void nuiSpline::SetFreeMode()
{
  mMode = nuiSplineModeFree;
  InternalChanged();
}

void nuiSpline::SetCubicBezierMode()
{
  mMode = nuiSplineModeBezierCubic;
  InternalChanged();
}

void nuiSpline::SetQuadraticBezierMode()
{
  mMode = nuiSplineModeBezierQuadratic;
  InternalChanged();
}


nuiSplineMode nuiSpline::GetMode() const
{
  return mMode;
}

bool nuiSpline::Tessellate(nuiPath& rVertices, float Quality) const
{
  if (mCacheValid && mLastQuality == Quality)
  {
    uint32 count = mTessCache.GetCount();
    for (uint32 i = 0; i < count; i++)
      rVertices.AddVertex(mTessCache[i]);
  }
  else
  {
    std::vector<nuiSplinePoint> Points;

    Evaluate(Points, (uint)(GetSize() * (20.0f * Quality)));

    std::vector<nuiSplinePoint>::iterator it;
    std::vector<nuiSplinePoint>::iterator end = Points.end();
    for (it = Points.begin(); it != end; ++it)
    {
      rVertices.AddVertex(*it);
      mTessCache.AddVertex(*it);
    }
    mLastQuality = Quality;
    mCacheValid = true;
  }
  return true;
}

bool nuiSpline::InvalidateCache(const nuiEvent& rEvent)
{
  mCacheValid = false;
  mTessCache.Clear();
  return false;
}

nuiPoint nuiSpline::GetStartPoint() const
{
  if (mNodes.empty())
    return nuiPoint::Zero;
  return nuiPoint(mNodes.front().GetPosition());
}

nuiPoint nuiSpline::GetEndPoint() const
{
  if (mNodes.empty())
    return nuiPoint::Zero;
  return nuiPoint(mNodes.back().GetPosition());
}


//////
