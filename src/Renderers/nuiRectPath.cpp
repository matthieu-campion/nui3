/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiRectPath.h"


// nuiRectPath:
nuiRectPath::nuiRectPath(const nuiRect& rRect, bool CCW)
{
  mRect = rRect;
  mCCW = CCW;
}

nuiRectPath::nuiRectPath(nuiXMLNode* pNode)
{
  if (pNode->GetName() != _T("nuiRectPath"))
    return;

  nuiSize x,y,w,h;
  x = nuiGetVal(pNode, _T("X"),0.0f);
  y = nuiGetVal(pNode, _T("Y"),0.0f);
  w = nuiGetVal(pNode, _T("Width"),0.0f);
  h = nuiGetVal(pNode, _T("Height"),0.0f);
  mRect.Set(x,y,w,h);
  mCCW = nuiGetBool(_T("CCW"),false);
}

nuiRectPath::~nuiRectPath()
{
}

nuiXMLNode* nuiRectPath::Serialize(nuiXMLNode* pParentNode) const
{
  nuiXMLNode* pNode = nuiPathGenerator::Serialize(pParentNode);

  pNode->SetName(_T("nuiRectPath"));

  pNode->SetAttribute(_T("CCW"),mCCW);

  pNode->SetAttribute(_T("X"),mRect.mLeft);
  pNode->SetAttribute(_T("Y"),mRect.mTop);
  pNode->SetAttribute(_T("Width"),mRect.GetWidth());
  pNode->SetAttribute(_T("Height"),mRect.GetHeight());

  return pNode;
}


bool nuiRectPath::Tessellate(nuiPath& rVertices, float Quality) const
{
  if (mCCW)
  {
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
  }
  else
  {
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mTop, 0));
    rVertices.AddVertex(nuiPoint(mRect.mRight,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mBottom, 0));
    rVertices.AddVertex(nuiPoint(mRect.mLeft ,mRect.mTop, 0));
  }
  return true;
}

nuiPoint nuiRectPath::GetStartPoint() const
{
  return nuiPoint(mRect.mLeft, mRect.Top());
}

nuiPoint nuiRectPath::GetEndPoint() const
{
  return nuiPoint(mRect.mLeft, mRect.Top());
}

