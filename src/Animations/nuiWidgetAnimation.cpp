/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

//class nuiWidgetAnimation (BASE CLASS)
nuiWidgetAnimation::nuiWidgetAnimation (nuiWidgetPtr pTarget, double DurationInSeconds)
  : nuiAnimation(),
    mpTarget(pTarget)
{
  SetDuration(DurationInSeconds);
}

nuiWidgetAnimation::~nuiWidgetAnimation()
{
}

//////// ACTUAL ANIMATION CLASSES IMPLEMENTATIONS:

//class nuiMoveRectAnim
nuiMoveRectAnim::nuiMoveRectAnim (nuiWidgetPtr pTarget, double Duration, const nuiRect& rSource, const nuiRect& rDestination)
 : nuiWidgetAnimation(pTarget, Duration),
   mSource(rSource),
   mDestination(rDestination)
{
}

void nuiMoveRectAnim::OnFrame()
{
  nuiSize t = (nuiSize)GetPosition();
  nuiSize tt = 1.0f - t;

  nuiSize X1, Y1, X2, Y2;

  X1 = mSource.Left() * tt + mDestination.Left() * t;
  Y1 = mSource.Top() * tt + mDestination.Top() * t;
  X2 = mSource.Right() * tt + mDestination.Right() * t;
  Y2 = mSource.Bottom() * tt + mDestination.Bottom() * t;

  nuiRect newrect(X1, Y1, X2, Y2, false);
  newrect.RoundToNearest();
  mpTarget->SetUserRect(newrect);
}

//class nuiMoveToRectAnim
nuiMoveToRectAnim::nuiMoveToRectAnim (nuiWidgetPtr pTarget, double Duration, const nuiRect& rDestination)
 : nuiMoveRectAnim(pTarget, Duration, nuiRect(), rDestination)
 {
 }

void nuiMoveToRectAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  mSource = mpTarget->GetIdealRect();
  nuiMoveRectAnim::Play(Count, LoopMode);
}

//class nuiMoveAnim
nuiMoveAnim::nuiMoveAnim (nuiWidgetPtr pTarget, double Duration, nuiSize X1, nuiSize Y1, nuiSize X2, nuiSize Y2)
 : nuiMoveRectAnim(pTarget, Duration, nuiRect(), nuiRect()),
   mX1(X1) ,mY1(Y1), mX2(X2), mY2(Y2)
{
}

void nuiMoveAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  mSource = mpTarget->GetIdealRect();
  mDestination = mpTarget->GetIdealRect();
  mSource.MoveTo(mX1,mY1);
  mDestination.MoveTo(mX2,mY2);
  nuiMoveRectAnim::Play(Count, LoopMode);
}


//class nuiMoveToAnim
nuiMoveToAnim::nuiMoveToAnim (nuiWidgetPtr pTarget, double Duration, nuiSize X, nuiSize Y)
  : nuiMoveAnim(pTarget, Duration, 0, 0, X, Y)
{
}

void nuiMoveToAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  mX1 = mpTarget->GetIdealRect().Left();
  mY1 = mpTarget->GetIdealRect().Top();
  nuiMoveAnim::Play(Count, LoopMode);
}

void nuiMoveAnim::OnFrame()
{
  nuiSize t = (nuiSize)GetPosition();
  nuiSize tt = 1.0f - t;
  
  nuiSize X1, Y1;
  
  X1 = mSource.Left() * tt + mDestination.Left() * t;
  Y1 = mSource.Top() * tt + mDestination.Top() * t;
  
  mpTarget->SetUserPos(X1, Y1);
}


//class nuiScaleToWidgetAnim
nuiScaleToWidgetAnim::nuiScaleToWidgetAnim (nuiWidgetPtr pTarget, double Duration, float Ratio, nuiPosition ScaleBarycenterPosition)
 : nuiMoveRectAnim(pTarget, Duration, nuiRect(), nuiRect()),
   mRatio(Ratio),
   mPosition(ScaleBarycenterPosition)
{
}

void nuiScaleToWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  mSource = mpTarget->GetRect();
  mDestination = mpTarget->GetRect();
  nuiSize nw,nh;

  nw = mRatio * mDestination.GetWidth();
  nh = mRatio * mDestination.GetHeight();

  nuiRect tmp(0.0f, 0.0f, nw, nh);
  tmp.SetPosition(mPosition, mDestination.Size());
  tmp.Move(mDestination.Left(), mDestination.Top());
  mDestination = tmp;

  nuiMoveRectAnim::Play(Count, LoopMode);
}

//class nuiScaleFromWidgetAnim
nuiScaleFromWidgetAnim::nuiScaleFromWidgetAnim (nuiWidgetPtr pTarget, double Duration, float Ratio, nuiPosition ScaleBarycenterPosition)
 : nuiMoveRectAnim(pTarget, Duration, nuiRect(), nuiRect()),
   mRatio(Ratio),
   mPosition(ScaleBarycenterPosition)
{
}

void nuiScaleFromWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  mSource = mpTarget->GetRect();
  mDestination = mpTarget->GetRect();
  nuiSize nw,nh;

  nw = mRatio * mSource.GetWidth();
  nh = mRatio * mSource.GetHeight();

  nuiRect tmp(0.0f, 0.0f, nw, nh);
  tmp.SetPosition(mPosition, mSource.Size());
  mSource = tmp;

  nuiMoveRectAnim::Play(Count, LoopMode);
}

//class nuiScaleMatrixWidgetAnim
nuiScaleMatrixWidgetAnim::nuiScaleMatrixWidgetAnim (nuiWidgetPtr pTarget, double Duration, float SourceRatio, float DestinationRatio, nuiPosition ScaleBarycenterPosition)
 : nuiWidgetAnimation(pTarget, Duration),
   mSourceRatio(SourceRatio),
   mDestinationRatio(DestinationRatio),
   mPosition(ScaleBarycenterPosition)
{
}

void nuiScaleMatrixWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiRect r(mpTarget->GetRect());
  float w = r.GetWidth();
  float h = r.GetHeight();

  switch (mPosition)
  {
  case nuiLeft:
    mX = 0;
    mY = h * .5f;
    break;
  case nuiRight:
    mX = w;
    mY = h * .5f;
    break;
  case nuiTop:
    mX = w * .5f;
    mY = 0;
    break;
  case nuiBottom:
    mX = w * .5f;
    mY = h;
    break;
  case nuiTopLeft:
    mX = 0;
    mY = 0;
    break;
  case nuiTopRight:
    mX = w;
    mY = 0;
    break;
  case nuiBottomLeft:
    mX = 0;
    mY = h;
    break;
  case nuiBottomRight:
    mX = w;
    mY = h;
    break;
  case nuiCenter:
    mX = w *.5f;
    mY = h *.5f;
    break;
  default:
    NGL_ASSERT(false); ///< Other positions are not compatible.
  }

  nuiWidgetAnimation::Play(Count, LoopMode);
}

void nuiScaleMatrixWidgetAnim::OnFrame()
{
  nuiSize t = (nuiSize)GetPosition();
  nuiSize tt = 1.0f - t;

  float ratio = tt * mSourceRatio + t * mDestinationRatio;

  nuiMatrix mat;

  mat.Translate(-ratio * mX, -ratio * mY, 0);
  mat.Scale(ratio,ratio,ratio);
  mat.Translate(mX, mY, 0);
  mpTarget->SetMatrix(mat);
}

//class nuiScaleToMatrixWidgetAnim
nuiScaleToMatrixWidgetAnim::nuiScaleToMatrixWidgetAnim (nuiWidgetPtr pTarget, double Duration, float DestinationRatio, nuiPosition ScaleBarycenterPosition)
 : nuiScaleMatrixWidgetAnim(pTarget, Duration, 0, DestinationRatio, ScaleBarycenterPosition)
{
}

void nuiScaleToMatrixWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiVector norm(1,0,0);
  norm = mpTarget->GetMatrix() * norm;
  mSourceRatio = norm.Length();
  nuiScaleMatrixWidgetAnim::Play(Count, LoopMode);
}

//class nuiScaleFromMatrixWidgetAnim
nuiScaleFromMatrixWidgetAnim::nuiScaleFromMatrixWidgetAnim (nuiWidgetPtr pTarget, double Duration, float SourceRatio, nuiPosition ScaleBarycenterPosition)
 : nuiScaleMatrixWidgetAnim(pTarget, Duration, SourceRatio, 0, ScaleBarycenterPosition)
{
}

void nuiScaleFromMatrixWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiVector norm(1,1,0);
  mDestinationRatio = (float)sqrt((mpTarget->GetMatrix() * norm).Length());
  nuiScaleMatrixWidgetAnim::Play(Count, LoopMode);
}


/////// ALPHA ANIMS:

//class nuiFadeWidgetAnim
nuiFadeWidgetAnim::nuiFadeWidgetAnim (nuiWidgetPtr pTarget, double Duration, float SourceRatio, float DestinationRatio)
 : nuiWidgetAnimation(pTarget, Duration),
   mSourceAlpha(SourceRatio),
   mDestinationAlpha(DestinationRatio)
{
}

void nuiFadeWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiWidgetAnimation::Play(Count, LoopMode);
}

void nuiFadeWidgetAnim::OnFrame()
{
  //NGL_OUT(_T("nuiFadeWidgetAnim::OnFrame()\n"));
  nuiSize t = (nuiSize)GetPosition();
  nuiSize tt = 1.0f - t;

  float ratio = tt * mSourceAlpha + t * mDestinationAlpha;

//  NGL_OUT(_T("nuiFadeWidgetAnim::OnFrame() [%f %f/%f a=%f]\n"), t, tt, Duration, ratio);

  mpTarget->SetAlpha(ratio);
}

//class nuiFadeInWidgetAnim
nuiFadeInWidgetAnim::nuiFadeInWidgetAnim (nuiWidgetPtr pTarget, double Duration, bool StartFromCurrentAlpha)
 : nuiFadeWidgetAnim(pTarget, Duration, 0, 1), mStartFromCurrentAlpha(StartFromCurrentAlpha)
{
}

void nuiFadeInWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  if (mStartFromCurrentAlpha)
    mSourceAlpha = mpTarget->GetMixedAlpha();
  nuiWidgetAnimation::Play(Count, LoopMode);
}

//class nuiFadeOutWidgetAnim
nuiFadeOutWidgetAnim::nuiFadeOutWidgetAnim (nuiWidgetPtr pTarget, double Duration, bool StartFromCurrentAlpha)
 : nuiFadeWidgetAnim(pTarget, Duration, 1, 0), mStartFromCurrentAlpha(StartFromCurrentAlpha)
{
}

void nuiFadeOutWidgetAnim::Play(int32 Count, nuiAnimLoop LoopMode)
{
  if (mStartFromCurrentAlpha)
    mSourceAlpha = mpTarget->GetMixedAlpha();
  nuiWidgetAnimation::Play(Count, LoopMode);
}

