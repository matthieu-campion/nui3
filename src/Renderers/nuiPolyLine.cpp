/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiPolyLine.h"

// nuiPolyLine:
nuiPolyLine::nuiPolyLine(const nuiPath& rVertices)
: mVertices(rVertices)
{
}

nuiPolyLine::nuiPolyLine(const nuiPoint& rVertex)
{
  mVertices.AddVertex(rVertex);
}

nuiPolyLine::nuiPolyLine(nuiXMLNode* pNode)
{
  if (pNode->GetName() != _T("nuiPolyLine"))
    return;

  const nuiXMLNodeList& children = pNode->GetChildren();

  nuiXMLNodeList::const_iterator it;
  nuiXMLNodeList::const_iterator end = children.end();

  for (it = children.begin(); it != end; ++it)
  {
    nuiXMLNode* pChild = *it;
    if (pChild)
    {
      if (pChild->GetName() == _T("nuiPoint"))
        LineTo(nuiPoint(pChild));
    }
  }
}

nuiPolyLine::~nuiPolyLine()
{
}

nuiXMLNode* nuiPolyLine::Serialize(nuiXMLNode* pParentNode) const
{
  nuiXMLNode* pNode = nuiPathGenerator::Serialize(pParentNode);

  pNode->SetName(_T("nuiPolyLine"));

  for (uint32 i = 0; i < mVertices.GetCount(); i++)
    mVertices[i].Serialize(pNode);

  return pNode;
}


bool nuiPolyLine::Tessellate(nuiPath& rVertices, float Quality) const
{
  for (uint32 i = 0; i < mVertices.GetCount(); i++)
    rVertices.AddVertex(mVertices[i]);
  return true;
}

void nuiPolyLine::AddLines(const nuiPath& rVertices)
{
  for (uint32 i = 0; i < rVertices.GetCount(); i++)
    mVertices.AddVertex(rVertices[i]);
//  Changed();
}

void nuiPolyLine::LineTo(const nuiPoint& rVertex)
{
  mVertices.AddVertex(rVertex);
//  Changed();
}

uint nuiPolyLine::GetPointsCount() const
{
  return mVertices.GetCount();
}

nuiPoint nuiPolyLine::GetStartPoint() const
{
  if (!mVertices.GetCount())
    return nuiPoint::Zero;
  return mVertices[0];
}

nuiPoint nuiPolyLine::GetEndPoint() const
{
  if (!mVertices.GetCount())
    return nuiPoint::Zero;
  return mVertices[mVertices.GetCount() - 1];
}


