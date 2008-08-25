/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

// nuiGradient.cpp

#include "nui.h"
#include "nuiColor.h"
#include "nuiRect.h"
#include "nuiGradient.h"
#include "nuiDrawContext.h"
#include "nuiContour.h"

nuiGradient::nuiGradient()
{
}

nuiGradient::nuiGradient(const nuiGradient& rGradient)
{
  mStops.assign(rGradient.GetStopList().begin(), rGradient.GetStopList().end());
}

nuiGradient::~nuiGradient()
{
}

static bool PositionIsLesser(const nuiGradientPair& rElem1, const nuiGradientPair& rElem2)
{
  return rElem1.first < rElem2.first;
}

void nuiGradient::AddStop(const nuiColor& rColor, nuiSize position)
{
  mStops.push_back(nuiGradientPair(position, rColor));
  mStops.sort(PositionIsLesser);
}

const nuiGradientStopList& nuiGradient::GetStopList() const
{
  return mStops;
}

const nuiColor nuiGradient::GetColorAt(nuiSize position) const
{
  if (mStops.size() == 0)
  {
    return nuiColor(0.f, 0.f, 0.f);
  }
  
  nuiGradientStopList::const_iterator stopIt = mStops.begin();
  nuiGradientStopList::const_iterator stopEnd = mStops.end();
  if (position <= (*stopIt).first)
  {
    return (*stopIt).second;
  }
  
  bool found = false;
  nuiGradientPair leftStop;
  for (; stopIt != stopEnd && !found; ++stopIt)
  {
    if (position > (*stopIt).first)
    {
      found = true;
      leftStop = *stopIt;
    }
  }
  
    if (stopIt == stopEnd)
    {
      return leftStop.second;
    }
    else
    {
      nuiGradientPair rightStop = *stopIt;
      float ratio = (position - leftStop.first) / (rightStop.first - leftStop.first);
      float red = ((rightStop.second.Red() - leftStop.second.Red()) * ratio) + leftStop.second.Red();
      float green = ((rightStop.second.Green() - leftStop.second.Green()) * ratio) + leftStop.second.Green();
      float blue = ((rightStop.second.Blue() - leftStop.second.Blue()) * ratio) + leftStop.second.Blue();
      float alpha = ((rightStop.second.Alpha() - leftStop.second.Alpha()) * ratio) + leftStop.second.Alpha();
      return nuiColor(red, green, blue, alpha);
    }
}


//////////////

nuiReflection::nuiReflection(float Intensity, float Center, float Curve, bool RoundLeft, bool RoundRight)
{
  mColor = nuiColor(1.f,1.f,1.f, Intensity);
  mCenter = Center;
  mCurve = Curve;
  mRoundLeft = RoundLeft;
  mRoundRight = RoundRight;
  mpSkyShape = NULL;
  mpGroundShape = NULL;
  mpSkyGradient = NULL;
  mpGroundGradient = NULL;
  mRecalc = true;
}

nuiReflection::~nuiReflection()
{
  delete mpSkyGradient;
  delete mpGroundGradient;
  delete mpSkyShape;
  delete mpGroundShape;
}

void nuiReflection::Draw(nuiDrawContext* pContext, const nuiRect& rRect, nuiShape* pShp)
{
  if (!(mRect == rRect))
    mRecalc = true;

  mRect = rRect;
  float x = mRect.Left();
  float y = mRect.Top();

  float h = mRect.GetHeight();
  if (mRecalc)
    Recalc();

  // Draw the sky:
  pContext->ResetClipShape();
  pContext->AddClipShape(*mpSkyShape);

  if (pShp)
    pContext->BlendClipShape(*pShp);

  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->DrawGradient(*mpSkyGradient, mRect, x, y, x, y+mCenter * h);

/*
  pContext->SetFillColor(nuiColor(1.0f, 0.0f, 0.0f));
  pContext->DrawRect(rRect, eFillShape);
  return;
*/

  // Draw the ground:
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->ResetClipShape();
  pContext->AddClipShape(*mpGroundShape);
  if (pShp)
    pContext->BlendClipShape(*pShp);

  pContext->DrawGradient(*mpGroundGradient, mRect, x, y + mCenter * h, x, y + h);

  // Reset the clipping:
  pContext->ResetClipShape();
}

void nuiReflection::Recalc()
{
  float x = mRect.Left();
  float y = mRect.Top();
  float w = mRect.GetWidth();
  float h = mRect.GetHeight();
  float h_2 = h * mCenter;

  mRecalc = false;

  delete mpSkyShape;
  delete mpGroundShape;
  delete mpSkyGradient;
  delete mpGroundGradient;

  // Prepare the sky shape
  mpSkyShape = new nuiShape();
  nuiContour* pContour = new nuiContour();

  pContour->LineTo(nuiPoint(x,y,0));
  pContour->LineTo(nuiPoint(x+w,y,0));
  pContour->LineTo(nuiPoint(x+w,y+h_2,0));
  pContour->LineTo(nuiPoint(x,y+h_2,0));

  mpSkyShape->AddContour(pContour);

  // Prepare the ground shape
  mpGroundShape = new nuiShape();
  pContour = new nuiContour();

  pContour->LineTo(nuiPoint(x,y+h_2,0));
  pContour->LineTo(nuiPoint(x+w,y+h_2,0));
  pContour->LineTo(nuiPoint(x+w,y+h,0));
  pContour->LineTo(nuiPoint(x,y+h,0));

  mpGroundShape->AddContour(pContour);

  mpSkyGradient = new nuiGradient();

  nuiColor tmp;
  
  // Sky:
  tmp = mColor;
  float alpha = mColor.Alpha();
  tmp.Alpha() = alpha * .2f;
  tmp.Crop();
  mpSkyGradient->AddStop(tmp, 0); 

  mpSkyGradient->AddStop(nuiColor(1.f,1.f,1.f, alpha), 0.1f); 

  tmp.Alpha() = alpha * .6f;
  tmp.Crop();
  mpSkyGradient->AddStop(tmp, .4f); 

  tmp.Alpha() = alpha * .4f;
  tmp.Crop();
  mpSkyGradient->AddStop(tmp, 1); 

  // Ground:
  mpGroundGradient = new nuiGradient();
  tmp.Alpha() = alpha * .1f;
  tmp.Crop();
  mpGroundGradient->AddStop(tmp, 0); 

  tmp.Alpha() = alpha * .4f;
  tmp.Crop();
  mpGroundGradient->AddStop(tmp, .8f); 

  tmp.Alpha() = alpha *.6f;
  tmp.Crop();
  mpGroundGradient->AddStop(tmp, 1); 
}

