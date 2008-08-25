/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiShape.h"
#include "nuiDrawContext.h"
#include "nuiPolyLine.h"
#include "nuiRectPath.h"
#include "nuiArc.h"
#include "nuiContour.h"
#include "nuiTessellator.h"
#include "nuiOutliner.h"


// class nuiShape
nuiShape::nuiShape()
: mEventSink(this)
{
  mWinding = eNone;
  mReferences = 0;

}

nuiShape::nuiShape(const nuiShape& rShape)
: mEventSink(this)
{
  NGL_ASSERT(0);
}

nuiShape& nuiShape::operator=(const nuiShape& rShape)
{
  NGL_ASSERT(0);
  return *this;
}

nuiShape::~nuiShape()
{
  // BEWARE: it the debuger gets you here then you have a nuiShape leak (some object have forgotten to release their shapes)!
  NGL_ASSERT(!mReferences);
  Clear();
  //NUI_ADD_EVENT(Changed);
}

void nuiShape::Clear()
{
  std::vector<nuiContour*>::const_iterator it;
  std::vector<nuiContour*>::const_iterator end = mpContours.end();

  for (it = mpContours.begin(); it != end; ++it)
    delete (*it);

  EmptyCaches();

  mpContours.clear();
}

bool nuiShape::Load(const nuiXMLNode* pNode)
{
  Clear();
  mWinding = eNone;
  if (pNode->GetName() != _T("nuiShape"))
    return false;

  nglString winding = pNode->GetAttribute(_T("Winding"));
  if (winding == _T("None"))
    mWinding = eNone;
  else if (winding == _T("Odd"))
    mWinding = eOdd;
  else if (winding == _T("NonZero"))
    mWinding = eNonZero;
  else if (winding == _T("Positive"))
    mWinding = ePositive;
  else if (winding == _T("Negative"))
    mWinding = eNegative;
  else if (winding == _T("AbsGEqTwo"))
    mWinding = eAbsGEqTwo;

  const nuiXMLNodeList& children = pNode->GetChildren();

  nuiXMLNodeList::const_iterator it;
  nuiXMLNodeList::const_iterator end = children.end();

  for (it = children.begin(); it != end; ++it)
  {
    nuiXMLNode* pChild = *it;
    if (pChild)
    {
      if (pChild->GetName() == _T("nuiContour"))
      {
        nuiContour* pContour = new nuiContour(pChild);
        AddContour(pContour);
        mEventSink.Connect( pContour->Changed, &nuiShape::ElementChanged, pContour);
      }
    }
  }
  
  return true;
}

nuiXMLNode* nuiShape::Serialize(nuiXMLNode* pParentNode) const
{
  nuiXMLNode* pNode = NULL;
  if (pParentNode)
    pNode = new nuiXMLNode(_T("nuiShape"),pParentNode);
  else
    pNode = new nuiXML(_T("nuiShape"));

  nglString winding;
  switch (mWinding)
  {
  case eOdd:
    winding = "Odd";
    break;
  case eNonZero:
    winding = "NonZero";
    break;
  case ePositive:
    winding = "Positive";
    break;
  case eNegative:
    winding = "Negative";
    break;
  case eAbsGEqTwo:
    winding = "AbsGEqTwo";
    break;
  case eNone:
  default:
    winding = "None";
    break;
  };

  pNode->SetAttribute(_T("Winding"),winding);

  std::vector<nuiContour*>::const_iterator it;
  std::vector<nuiContour*>::const_iterator end = mpContours.end();

  for (it = mpContours.begin(); it != end; ++it)
    (*it)->Serialize(pNode);

  return pNode;
}

uint nuiShape::Acquire()
{
  return ++mReferences;
}

uint nuiShape::Release()
{
  if (--mReferences)
    return mReferences;
  delete this;
  return 0;
}

void nuiShape::AddContour(nuiContour* pContour)
{
  mpContours.push_back(pContour);
  //Changed();
  mEventSink.Connect( pContour->Changed, &nuiShape::ElementChanged, pContour);
}

void nuiShape::AddContour()
{
  nuiContour* pContour = new nuiContour();
  mpContours.push_back(pContour);
  //Changed();
  mEventSink.Connect( pContour->Changed, &nuiShape::ElementChanged, pContour);
}

void nuiShape::CloseContour()
{
  if (mpContours.empty())
    return;
  mpContours.back()->Close();
}

void nuiShape::ArcTo(float X, float Y, float XRadius, float YRadius, float Angle, bool LargeArc, bool Sweep)
{
  if (mpContours.empty())
    AddContour();

  mpContours.back()->ArcTo(nuiPoint(X, Y), XRadius, YRadius, Angle, LargeArc, Sweep);
}

void nuiShape::AddLines(const nuiPath& rVertices)
{
  if (mpContours.empty())
    AddContour();

  mpContours.back()->AddLines(rVertices);
}

void nuiShape::AddPath(const nuiPath& rVertices)
{
  if (mpContours.empty() || mpContours.back()->GetElementsCount())
    AddContour();

  const uint count = rVertices.GetCount();
  for (uint i = 0; i < count; i++)
  {
    const nuiPoint& rPoint = rVertices[i];
    switch (rPoint.GetType())
    {
    case nuiPointTypeStop:
      AddContour();
      break;
    case nuiPointTypeNormal:
      if (rPoint[0] == 0.0f && rPoint[1] == 0.0f)
      {
        NGL_ASSERT(false);
      }
      LineTo(rPoint);
      break;
    default:
      NGL_ASSERT(false);
      break;
    }
  }
}

void nuiShape::LineTo(const nuiPoint& rVertex)
{
  if (mpContours.empty())
    AddContour();

  mpContours.back()->LineTo(rVertex);
}

void nuiShape::AddSpline(const nuiSpline& rSpline)
{
  if (mpContours.empty())
    AddContour();

  mpContours.back()->AddSpline(rSpline);
}

void nuiShape::AddPathGenerator(nuiPathGenerator* pPath)
{
  if (mpContours.empty())
    AddContour();

  mpContours.back()->AddPathGenerator(pPath);
}

nuiContour* nuiShape::GetContour(uint Index) const
{
  NGL_ASSERT(Index < mpContours.size());
  return mpContours[Index];
}

uint nuiShape::GetContourCount() const
{
  return mpContours.size();
}


bool nuiShape::Tessellate(nuiPath& rVertices, float Quality)
{
  bool res = true;
  std::vector<nuiContour*>::iterator it;
  std::vector<nuiContour*>::iterator end = mpContours.end();

  for (it = mpContours.begin(); it != end; ++it)
  {
    res = res && (*it)->Tessellate(rVertices,Quality);
    rVertices.StopPath();
  }
  return res;
}

nuiShape::Winding nuiShape::GetWinding() const
{
  return mWinding;
}

void nuiShape::SetWinding(nuiShape::Winding Rule)
{
  mWinding = Rule;
  //Changed();
}

void nuiShape::EmptyCaches()
{
}



bool nuiShape::ElementChanged(const nuiEvent& rEvent)
{
  EmptyCaches();
  //Changed();
  return false;
}

#define CIRCLE_FACTOR (1.0/3.5)

void nuiShape::AddCircle(float X, float Y, float Radius, bool CCW, float Quality)
{
  if (Radius <= 0)
    return;

  nuiContour* pContour = new nuiContour();
  AddContour(pContour);

  uint count = ToAbove((double)(2.0 * Quality * M_PI * (double)Radius * CIRCLE_FACTOR));
  float step = 2.0f * (float)M_PI / (float)count;
  if (CCW)
    step = -step;
  for (uint i = 0; i <= count; i++)
  {
    float angle = step * (float) i;
    float x = X + sin(angle) * Radius;
    float y = Y + cos(angle) * Radius;

    pContour->LineTo(nuiPoint(x, y, 0));
  }
}

void nuiShape::AddEllipse(float X, float Y, float XRadius, float YRadius, bool CCW, float Quality)
{
  nuiContour* pContour = new nuiContour();
  AddContour(pContour);

  uint count = ToAbove((double)(M_PI * Quality * (XRadius+YRadius) * CIRCLE_FACTOR));
  float step = 2.0f * (float)M_PI / (float)count;
  if (CCW)
    step = -step;
  for (uint i = 0; i <= count; i++)
  {
    float angle = step * (float) i;
    float x = X + sin(angle) * XRadius;
    float y = Y + cos(angle) * YRadius;

    pContour->LineTo(nuiPoint(x, y, 0));
  }
}

void nuiShape::AddRect(const nuiRect& rRect, bool CCW)
{
  AddContour();
  mpContours.back()->AddRect(rRect, CCW);
}

void nuiShape::AddRoundRect(const nuiRect& rRect, float Radius, bool CCW, float Quality)
{
  nuiRect rect(rRect);
  rect.Right() = MAX(rect.Left(), rect.Right()-1);
  rect.Bottom() = MAX(rect.Top(), rect.Bottom()-1);

  Radius = MIN(Radius, MIN(rect.GetWidth()/2, rect.GetHeight()/2));

  nuiContour* pContour = new nuiContour();
  AddContour(pContour);

  if (!CCW)
  {
    // Top Left
    pContour->LineTo(nuiPoint(rect.Left(), rect.Top() + Radius));
    pContour->ArcTo(nuiPoint(rect.Left() + Radius, rect.Top()), Radius, Radius, 0, false, true);

    // Top
    pContour->LineTo(nuiPoint(rect.Right() - Radius, rect.Top(), 0));

    // Top Right
    pContour->ArcTo(nuiPoint(rect.Right(), rect.Top() + Radius), Radius, Radius, 0, false, true);

    // Right
    pContour->LineTo(nuiPoint(rect.Right(), rect.Bottom() - Radius, 0));

    // Bottom Right
    pContour->ArcTo(nuiPoint(rect.Right() - Radius, rect.Bottom()), Radius, Radius, 0, false, true);

    // Bottom
    pContour->LineTo(nuiPoint(rect.Left() + Radius, rect.Bottom(), 0));


    // Bottom Left
    pContour->ArcTo(nuiPoint(rect.Left(), rect.Bottom() - Radius), Radius, Radius, 0, false, true);

    // Left
    pContour->LineTo(nuiPoint(rect.Left(), rect.Top() + Radius));
  }
  else
  {
    // Top Left
    pContour->LineTo(nuiPoint(rect.Left() + Radius, rect.Top()));
    pContour->ArcTo(nuiPoint(rect.Left(), rect.Top() + Radius), Radius, Radius, 0, false, false);

    // Top
    pContour->LineTo(nuiPoint(rect.Left(), rect.Bottom() - Radius, 0));

    // Top Right
    pContour->ArcTo(nuiPoint(rect.Left() + Radius, rect.Bottom()), Radius, Radius, 0, false, false);

    // Right
    pContour->LineTo(nuiPoint(rect.Right() - Radius, rect.Bottom(), 0));

    // Bottom Right
    pContour->ArcTo(nuiPoint(rect.Right(), rect.Bottom() - Radius), Radius, Radius, 0, false, false);

    // Bottom
    pContour->LineTo(nuiPoint(rect.Right(), rect.Top() +  Radius, 0));


    // Bottom Left
    pContour->ArcTo(nuiPoint(rect.Right() - Radius, rect.Top()), Radius, Radius, 0, false, false);

    // Left
    pContour->LineTo(nuiPoint(rect.Left() + Radius, rect.Top()));
  }
  pContour->Close();
}

nuiRect nuiShape::GetRect()
{
  nuiRect rect;
  std::vector<nuiContour*>::const_iterator it;
  std::vector<nuiContour*>::const_iterator end = mpContours.end();

  for (it = mpContours.begin(); it != end; ++it)
    rect.Union(rect, (*it)->GetRect());
  return rect;
}

nuiPoint nuiShape::GetStartPoint() const
{
  if (mpContours.empty())
    return nuiPoint();
  return mpContours.front()->GetStartPoint();
}

nuiPoint nuiShape::GetEndPoint() const
{
  if (mpContours.empty())
    return nuiPoint();
  return mpContours.back()->GetEndPoint();
}

nuiRenderObject* nuiShape::Fill(float Quality)
{
  nuiTessellator* pTess = new nuiTessellator(this);
  pTess->SetFill(true);
  nuiRenderObject* pObj = pTess->Generate(Quality);
  delete pTess;

  return pObj;
}

nuiRenderObject* nuiShape::Outline(float Quality, float LineWidth, nuiLineJoin LineJoin, nuiLineCap LineCap, float MiterLimit)
{
  nuiOutliner* pOutliner = new nuiOutliner(NULL, LineWidth);
  pOutliner->SetLineJoin(LineJoin);
  pOutliner->SetLineCap(LineCap);
  pOutliner->SetMiterLimit(MiterLimit);

  nuiRenderObject* pObj = new nuiRenderObject();

  uint32 contours = GetContourCount();
  for (uint32 i = 0; i < contours; i++)
  {
    nuiContour* pContour = GetContour(i);

    pOutliner->SetPath(pContour);
    pOutliner->TessellateObj(*pObj, Quality);
  }

  delete pOutliner;

  return pObj;
}

/*
nuiRenderObject* nuiShape::Outline(float Quality, float LineWidth, nuiLineJoin LineJoin, nuiLineCap LineCap, float MiterLimit)
{
  nuiShape* pTmpShape = new nuiShape();

  nuiOutliner* pOutliner = new nuiOutliner(NULL, LineWidth);
  pOutliner->SetLineJoin(LineJoin);
  pOutliner->SetLineCap(LineCap);
  pOutliner->SetMiterLimit(MiterLimit);

  uint32 contours = GetContourCount();
  for (uint32 i = 0; i < contours; i++)
  {
    nuiContour* pContour = GetContour(i);

    pOutliner->SetPath(pContour);

    nuiPath Vertices;
    pOutliner->Tessellate(Vertices, Quality);
    pTmpShape->AddPath(Vertices);
    pTmpShape->CloseContour();
  }

  delete pOutliner;

  nuiRenderObject* pObj = pTmpShape->Fill(Quality);
  delete pTmpShape;
  return pObj;
}
*/

void nuiShape::AddArc(float cX, float cY, float rX, float rY, float Theta1InDegree, float Theta2InDegree, float Phi, float Quality)
{
  AddContour();
  mpContours.back()->AddArc(cX, cY, rX, rY, Theta1InDegree, Theta2InDegree, Phi);
}
