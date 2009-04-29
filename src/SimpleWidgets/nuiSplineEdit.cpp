/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiSplineEdit.h"
#include "nuiDrawContext.h"
#include "nuiContour.h"

using namespace std;

#define TANSCALE (0.3f)

nuiSplineEdit::nuiSplineEdit( nuiSpline* pSpline, bool OwnSpline )
  : nuiWidget(),
    mEventsink(this)
{
  SetObjectClass(_T("nuiSplineEdit"));
  mClicked = false;
  mpSpline = pSpline;
  mOwnSpline = OwnSpline;
  mDisplayTangents = false;
  mDisplayControlPoints = true;
  mControlPointSize = 6.0f;
  mStep = 0.05f;
  mEditable = true;
  mEditTangents = true;

  mPointColor.Set(.3f,.3f,.5f);;
  mSelectedPointColor.Set(0,0,.7f);;
  mSplineColor.Set(0,0,0);
  mTangentColor.Set(0,.6f,0);
  mSelectedTangentColor.Set(.3f,.6f,.3f);

  mpSelectedNode = NULL;

  mCanAddPoints = true;
  mCanDelPoints = true;
  mCanMergePoints = false;
  mMergeDistance = 10; // default is 10 pixels

  mEventsink.Connect(mpSpline->Changed, &nuiSplineEdit::SplineChanged);
}

bool nuiSplineEdit::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  SetObjectClass(_T("nuiSplineEdit"));

  mpSelectedNode = NULL;
  mClicked = false;
  mpSpline = NULL;
  mOwnSpline = false;
  mDisplayTangents = false;
  mDisplayControlPoints = true;
  mControlPointSize = 5.0f;
  mStep = 0.05f;
  mEditable = true;
  mEditTangents = true;

  mPointColor.Set(.3f,.3f,.5f);;
  mSelectedPointColor.Set(0,0,.7f);;
  mSplineColor.Set(0,0,0);
  mTangentColor.Set(0,.6f,0);
  mSelectedTangentColor.Set(.3f,.6f,.3f);

  mCanAddPoints = true;
  mCanDelPoints = true;
  mCanMergePoints = false;
  mMergeDistance = 10; // default is 10 pixels
  return true;
}

nuiSplineEdit::~nuiSplineEdit()
{
  if (mOwnSpline && mpSpline)
    delete mpSpline;
}

nuiXMLNode* nuiSplineEdit::Serialize(nuiXMLNode* pParentNode, bool Recursive ) const
{
  return nuiWidget::Serialize(pParentNode, Recursive);
}

nuiRect nuiSplineEdit::CalcIdealSize()
{
  nuiRect Rect;
  if (mpSpline)
    Rect = mpSpline->GetRect();

  mIdealRect = Rect;
  return mIdealRect;
}

bool nuiSplineEdit::Draw(nuiDrawContext* pContext)
{
  nuiRect rect;
  float x,y;
  float halfsize = mControlPointSize / 2.0f;

  if (!mpSpline)
    return false;

//  pContext->PushState();
  pContext->ResetState();
  pContext->EnableTexturing(false);
  pContext->EnableBlending(false);

  nuiShape Shape;

  nuiContour* pContour = new nuiContour();

  pContour->AddSpline(*mpSpline);
  Shape.AddContour(pContour);

/*
  pContour = new nuiContour();
  pContour->AddRect(nuiRect(50,50,100,100), false);

  nuiPoint point;
  point = nuiVector2(100,100);
  point.SetColor(nuiColor(1,0,0));

  pContour->LineTo(point);
  point = nuiVector2(110,10);
  point.SetColor(nuiColor(0,1,0));
  pContour->LineTo(point);
//  pContour->AddRect(nuiRect(75,75,100,100), true);
  Shape.AddContour(pContour);
*/

  nuiColor SColor(0,0,0);
  nuiColor FColor(.7f,.7f,1.f,.5f);
  pContext->SetStrokeColor(SColor);
  pContext->SetFillColor(FColor);
  pContext->SetLineWidth(10.0f);
  pContext->EnableAntialiasing(true);
  Shape.SetWinding(nuiShape::eNone);

  pContext->SetBlendFunc(nuiBlendTransp);

  pContext->DrawShape(&Shape, eStrokeShape);

  pContext->EnableAntialiasing(false);
//  pContext->DrawShape(&Shape, eStrokeShape, true,true);
//  pContext->DrawShape(&Shape, eFillShape, true, true);

  vector<nuiSplineNode> Nodes;
  vector<nuiSplineNode>::iterator it;
  vector<nuiSplineNode>::iterator end;
  mpSpline->GetNodes(Nodes);
  it = Nodes.begin();
  end = Nodes.end();

  for (; it != end; ++it)
  {
    if (&(*it) != mpSelectedNode)
    {
      nuiVector vec = (*it).GetPosition();
      x = vec[0];
      y = vec[1];

      if (mDisplayControlPoints)
      {
        rect.Set( x-halfsize, y-halfsize, mControlPointSize, mControlPointSize );
        pContext->SetStrokeColor(mPointColor);
        pContext->DrawRect(rect,eStrokeShape);
      }

      if (mDisplayTangents)
      {
        nuiVector v;
        pContext->SetStrokeColor(mTangentColor);
        v = (*it).GetIncomingTangent();
        pContext->DrawLine(x,y,x+v[0]*TANSCALE,y+v[1]*TANSCALE);
        v = (*it).GetOutgoingTangent();
        pContext->DrawLine(x,y,x+v[0]*TANSCALE,y+v[1]*TANSCALE);
      }
    }
  }

  if (mpSelectedNode)
  {
    nuiRect tgin, tgout;
    x = (*mpSelectedNode).GetPosition()[0];
    y = (*mpSelectedNode).GetPosition()[1];
    rect.Set( x-halfsize, y-halfsize, mControlPointSize, mControlPointSize );
    pContext->SetFillColor(mSelectedPointColor);
    pContext->SetStrokeColor(mPointColor);
    pContext->DrawRect(rect, eFillShape);

    if (mEditTangents)
    {
      pContext->SetStrokeColor(mSelectedTangentColor);
      nuiVector v;
      v = mpSelectedNode->GetIncomingTangent();
      tgin.Set( x+v[0]*TANSCALE-halfsize, y+v[1]*TANSCALE-halfsize, mControlPointSize, mControlPointSize );
      pContext->DrawLine(x,y,x+v[0]*TANSCALE,y+v[1]*TANSCALE);

      v = mpSelectedNode->GetOutgoingTangent();
      tgout.Set( x-v[0]*TANSCALE-halfsize, y-v[1]*TANSCALE-halfsize, mControlPointSize, mControlPointSize );
      pContext->DrawLine(x,y,x-v[0]*TANSCALE,y-v[1]*TANSCALE);

      pContext->SetFillColor(mSelectedPointColor);
      pContext->DrawRect(tgin,eFillShape);
      pContext->DrawRect(tgout,eFillShape);
    }
    else if (mDisplayTangents)
    {
      nuiVector v;
      v = (*it).GetIncomingTangent();
      pContext->DrawLine(x,y,x+v[0]*TANSCALE,y+v[1]*TANSCALE);

      v = (*it).GetOutgoingTangent();
      pContext->DrawLine(x,y,x+v[0]*TANSCALE,y+v[1]*TANSCALE);
    }
  }

//  pContext->PopState();
  return true;
}

bool nuiSplineEdit::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mEditable)
  {
    if (Button == nglMouseInfo::ButtonLeft)
    {
      Invalidate();
      mpSelectedNode = mpSpline->GetNode(nuiVector(X,Y,0), mControlPointSize);
      if (mpSelectedNode)
      {
        mClicked = true;
        mOldX = X;
        mOldY = Y;
        Grab();
        return true;
      }
    }
  }
  return false;
}

bool nuiSplineEdit::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mEditable)
  {
    if (mClicked)
    {
      mClicked = false;
      Ungrab();
      return true;
    }
  }
  return false;
}

bool nuiSplineEdit::MouseMoved    (nuiSize X, nuiSize Y)
{
  if (mClicked)
  {
    if (mpSelectedNode)
    {
      mpSelectedNode->SetPosition(nuiVector(X,Y,0));
      Invalidate();
      return true;
    }
    else
    {
/*
      float x = X - mOldX;
      float y = Y - mOldY;

      mpSpline->Translate(nuiVector(x,y,0));

      mOldX = X;
      mOldY = Y;
*/
    }
  }

  return false;
}

void nuiSplineEdit::DisplayTangents(bool display)
{
  if (display == mDisplayTangents)
    return;
  mDisplayTangents = display;
  Invalidate();
}

bool nuiSplineEdit::AreTangentDisplayed()
{
  return mDisplayTangents;
}

void nuiSplineEdit::DisplayControlPoints(bool display)
{
  if (display == mDisplayControlPoints)
    return;
  mDisplayControlPoints = display;
  Invalidate();
}

bool nuiSplineEdit::AreControlPointsDisplayed()
{
  return mDisplayControlPoints;
}

void nuiSplineEdit::SetStep(float Step)
{
  if (mStep == Step)
    return;
  mStep = Step;
  Invalidate();
}

float nuiSplineEdit::GetStep()
{
  return mStep;
}

void nuiSplineEdit::SetEditable(bool editable)
{
  if (editable == mEditable)
    return;
  mEditable = editable;
  if (mClicked)
  {
    mClicked = false;
    Ungrab();
  }
  Invalidate();
}


bool nuiSplineEdit::IsEditable()
{
  return mEditable;
}

bool nuiSplineEdit::SplineChanged(const nuiEvent& rEvent)
{
  InvalidateLayout();
  return false;
}

void nuiSplineEdit::SetSplineColor(const nuiColor& rColor)
{
  mSplineColor = rColor;
  Invalidate();
}

nuiColor nuiSplineEdit::GetSplineColor()
{
  return mSplineColor;
}

void nuiSplineEdit::SetTangentColor(const nuiColor& rColor)
{
  mTangentColor = rColor;
  Invalidate();
}

nuiColor nuiSplineEdit::GetTangentColor()
{
  return mTangentColor;
}

void nuiSplineEdit::SetPointColor(const nuiColor& rColor)
{
  mPointColor = rColor;
  Invalidate();
}

nuiColor nuiSplineEdit::GetPointColor()
{
  return mPointColor;
}

void nuiSplineEdit::SetSelectedPointColor(const nuiColor& rColor)
{
  mSelectedPointColor = rColor;
  Invalidate();
}

nuiColor nuiSplineEdit::GetSelectedPointColor()
{
  return mSelectedPointColor;
}

void nuiSplineEdit::SetCanAddPoints( bool add)
{
  if (add == mCanAddPoints)
    return;
  mCanAddPoints = add;
  Invalidate();
}

bool nuiSplineEdit::GetCanAddPoints()
{
  return mCanAddPoints;
}

void nuiSplineEdit::SetCanDelPoints( bool del)
{
  if (del == mCanDelPoints)
    return;
  mCanDelPoints = del;
  Invalidate();
}

bool nuiSplineEdit::GetCanDelPoints()
{
  return mCanDelPoints;
}

void nuiSplineEdit::SetCanMergePoints( bool merge)
{
  if (merge == mCanMergePoints)
    return;
  mCanMergePoints = merge;
  Invalidate();
}

bool nuiSplineEdit::GetCanMergePoints()
{
  return mCanMergePoints;
}

void nuiSplineEdit::SetMergeDistance( float distance )
{
  if (distance == mMergeDistance)
    return;
  mMergeDistance = distance;
  Invalidate();
}

float nuiSplineEdit::GetMergeDistance()
{
  return mMergeDistance;
}

