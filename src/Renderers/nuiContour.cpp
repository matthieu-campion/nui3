/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

//class nuiContour
nuiContour::nuiContour()
: mEventSink(this)
{
}

nuiContour::~nuiContour()
{
  std::list<nuiPathGenerator*>::iterator it;
  std::list<nuiPathGenerator*>::iterator end = mpElements.end();
  for (it = mpElements.begin(); it != end; ++it)
    if (*it)
      delete *it;
}

void nuiContour::AddLines(const nuiPath& rVertices)
{
  nuiPolyLine* pLines = NULL;
  if (!mpElements.empty())
    pLines = dynamic_cast<nuiPolyLine*>(mpElements.back());
  if (pLines)
  {
    // The last object of the contour is already a polyline, lets add our lines to it.
    pLines->AddLines(rVertices);
  }
  else
  {
    // The last object if not a polyline, lets add a new polyline contour element. 
    nuiPathGenerator* pContourElement = new nuiPolyLine(rVertices);
    mpElements.push_back( pContourElement);
//    mEventSink.Connect(pContourElement->Changed, &nuiContour::ElementChanged, pContourElement);
  }
//  Changed();
}

void nuiContour::LineTo(const nuiPoint& rVertex)
{
  nuiPolyLine* pLines = NULL;
  if (!mpElements.empty())
    pLines = dynamic_cast<nuiPolyLine*>(mpElements.back());
  if (pLines)
  {
    // The last object of the contour is already a polyline, lets add our lines to it.
    pLines->LineTo(rVertex);
  }
  else
  {
    // The last object is not a polyline, lets add a new polyline contour element. 
    nuiPathGenerator* pContourElement = new nuiPolyLine(rVertex);
    mpElements.push_back(pContourElement);
//    mEventSink.Connect(pContourElement->Changed, &nuiContour::ElementChanged, pContourElement);
  }
//  Changed();
}

void nuiContour::Close()
{
  nuiPathGenerator* pFirstPath = NULL;
  if (mpElements.empty())
    return;
  pFirstPath = mpElements.front();
  LineTo(pFirstPath->GetStartPoint());
}


void nuiContour::AddArc(const nuiPoint& rStartVertex, const nuiPoint& rStopVertex, float XRadius, float YRadius, float AngleInDegree, bool LargeArc, bool Sweep)
{
  nuiArc* pArc = new nuiArc(rStartVertex, rStopVertex, XRadius, YRadius, AngleInDegree, LargeArc, Sweep);
  mpElements.push_back(pArc);
//  mEventSink.Connect(pArc->Changed, &nuiContour::ElementChanged, pArc);
//  Changed();
}

void nuiContour::AddArc(float cX, float cY, float rX, float rY, float Theta1InDegree, float Theta2InDegree, float Phi)
{
  nuiArc* pArc = new nuiArc(cX, cY, rX, rY, Theta1InDegree, Theta2InDegree, Phi);
  mpElements.push_back(pArc);
//  mEventSink.Connect(pArc->Changed, &nuiContour::ElementChanged, pArc);
//  Changed();
}

void nuiContour::ArcTo(const nuiPoint& rStopVertex, float XRadius, float YRadius, float Angle, bool LargeArc, bool Sweep)
{
  nuiPoint StartVertex;
  if (!mpElements.empty())
  {
    nuiPathGenerator* pGenerator = mpElements.back();
    StartVertex = pGenerator->GetEndPoint();
  }

  nuiArc* pArc = new nuiArc(StartVertex, rStopVertex, XRadius, YRadius, Angle, LargeArc, Sweep);
  mpElements.push_back(pArc);
//  mEventSink.Connect(pArc->Changed, &nuiContour::ElementChanged, pArc);
//  Changed();
}

void nuiContour::AddRect(const nuiRect& rRect, bool CCW)
{
  nuiPathGenerator* pContourElement = new nuiRectPath(rRect, CCW);
  mpElements.push_back( pContourElement );
//  mEventSink.Connect(pContourElement->Changed, &nuiContour::ElementChanged, pContourElement);
//  Changed();
}


void nuiContour::AddSpline(const nuiSpline& rSpline)
{
  nuiPathGenerator* pContourElement = new nuiSpline(rSpline);
  mpElements.push_back( pContourElement );
//  mEventSink.Connect(pContourElement->Changed, &nuiContour::ElementChanged, pContourElement);
//  Changed();
}


bool nuiContour::Tessellate(nuiPath& rVertices, float Quality) const
{
  bool res = true;
  std::list<nuiPathGenerator*>::const_iterator it;
  std::list<nuiPathGenerator*>::const_iterator end = mpElements.end();

  for (it = mpElements.begin(); it != end; ++it)
    res = res && (*it)->Tessellate(rVertices,Quality);
  return res;
}

uint nuiContour::GetElementsCount() const
{
  return (uint)mpElements.size();
}

const std::list<nuiPathGenerator*>* nuiContour::GetElements() const
{
  return &mpElements;
}

void nuiContour::ElementChanged(const nuiEvent& rEvent)
{
//  Changed();
}

nuiPoint nuiContour::GetStartPoint() const
{
  if (mpElements.empty())
    return nuiPoint::Zero;
  return mpElements.front()->GetStartPoint();
}

nuiPoint nuiContour::GetEndPoint() const
{
  if (mpElements.empty())
    return nuiPoint::Zero;
  return mpElements.back()->GetEndPoint();
}

nuiRect nuiContour::GetRect()
{
  nuiRect rect;
  std::list<nuiPathGenerator*>::const_iterator it;
  std::list<nuiPathGenerator*>::const_iterator end = mpElements.end();

  for (it = mpElements.begin(); it != end; ++it)
    rect.Union(rect, (*it)->GetRect());
  return rect;
}


void nuiContour::AddPathGenerator(nuiPathGenerator* pPath)
{
  mpElements.push_back(pPath);
}
