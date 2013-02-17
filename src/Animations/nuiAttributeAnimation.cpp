/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

//class nuiAttributeAnimation : public nuiAnimation
nuiAttributeAnimationBase::nuiAttributeAnimationBase()
: mCaptureStartOnPlay(false),
  mCaptureEndOnPlay(false),
  mpTarget(NULL)
{
  if (SetObjectClass(_T("nuiAttributeAnimationBase")))
  {
    // Init atributes
  }
}

nuiAttributeAnimationBase::~nuiAttributeAnimationBase()
{
}

nuiObjectPtr nuiAttributeAnimationBase::GetTargetObject() const
{
  return mpTarget;
}

void nuiAttributeAnimationBase::SetTargetObject(nuiObjectPtr pTarget)
{
  mpTarget = pTarget;
}

const nglString& nuiAttributeAnimationBase::GetTargetAttribute() const
{
  return mTarget;
}

void nuiAttributeAnimationBase::SetTargetAttribute(const nglString& rAttribute)
{
  mTarget = rAttribute;
}

void nuiAttributeAnimationBase::SetCaptureStartOnPlay(bool set)
{
  mCaptureStartOnPlay = set;
}

bool nuiAttributeAnimationBase::GetCaptureStartOnPlay() const
{
  return mCaptureStartOnPlay;
}

void nuiAttributeAnimationBase::SetCaptureEndOnPlay(bool set)
{
  mCaptureEndOnPlay = set;
}

bool nuiAttributeAnimationBase::GetCaptureEndOnPlay() const
{
  return mCaptureEndOnPlay;
}


//// nuiAttributeAnimation:
nuiAttributeAnimation::nuiAttributeAnimation()
: mStartValue(0),
  mEndValue(0)
{
  if (SetObjectClass(_T("nuiAttributeAnimation")))
  {
    // Init atributes
  }
}

nuiAttributeAnimation::~nuiAttributeAnimation()
{
  
}

void nuiAttributeAnimation::SetEndValue(double val)
{
  mEndValue = val;
}

double nuiAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiAttributeAnimation::SetStartValue(double val)
{
  mStartValue = val;
}

double nuiAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}

void nuiAttributeAnimation::Play(int32 Count, nuiAnimLoop LoopMode)
{
  if (mCaptureStartOnPlay)
  {
    nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
    NGL_ASSERT(attrib.IsValid());
    nglString str;
    if (attrib.ToString(str))
      mStartValue = str.GetCDouble();
  }
  if (mCaptureEndOnPlay)
  {
    nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
    NGL_ASSERT(attrib.IsValid());
    nglString str;
    if (attrib.ToString(str))
      mEndValue = str.GetCDouble();
  }
  
  nuiAnimation::Play(Count, LoopMode);
}

#define SET_ATTRIB(X) \
if (nuiAttributeTypeTrait<X>::mTypeId == t) \
{ \
  nuiAttrib<X> attrib_##X(attrib); \
  NGL_ASSERT(attrib_##X.IsValid()); \
  attrib_##X.Set((X)pos); \
  return; \
}

void nuiAttributeAnimation::OnFrame()
{
  double pos = mStartValue + GetPosition() * (mEndValue - mStartValue);
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  if (!attrib.IsValid() || attrib.IsReadOnly())
    return;
  
  nuiAttributeType t = attrib.GetType();  
  SET_ATTRIB(float);
  SET_ATTRIB(double);
  SET_ATTRIB(int32);
  SET_ATTRIB(uint32);
  SET_ATTRIB(int64);
  SET_ATTRIB(uint64);
  SET_ATTRIB(int16);
  SET_ATTRIB(uint16);
  SET_ATTRIB(int8);
  SET_ATTRIB(uint8);               
  
  nglString str;
  str.SetCDouble(pos);
  attrib.FromString(str);

}

#undef SET_ATTRIB

/////////////////////////////////

// Color Attrib Animation:

/////////////////////////////////

//// nuiColorAttributeAnimation:
nuiColorAttributeAnimation::nuiColorAttributeAnimation()
{
  if (SetObjectClass(_T("nuiColorAttributeAnimation")))
  {
    // Init atributes
  }
}

nuiColorAttributeAnimation::~nuiColorAttributeAnimation()
{
}

void nuiColorAttributeAnimation::SetEndValue(const nuiColor& rVal)
{
  mEndValue = rVal;
}

const nuiColor& nuiColorAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiColorAttributeAnimation::SetStartValue(const nuiColor& rVal)
{
  mStartValue = rVal;
}

const nuiColor& nuiColorAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}



void nuiColorAttributeAnimation::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  NGL_ASSERT(attrib.IsValid());
  
  nuiAttrib<nuiColor> color_attrib(attrib);
  nuiAttrib<const nuiColor&> const_color_attrib(attrib);

  if (mCaptureStartOnPlay)
  {
    if (color_attrib)
      mStartValue = color_attrib.Get();
    else if (const_color_attrib)
      mStartValue = const_color_attrib.Get();
  }
  if (mCaptureEndOnPlay)
  {
    if (color_attrib)
      mEndValue = color_attrib.Get();
    else if (const_color_attrib)
      mEndValue = const_color_attrib.Get();
  }
  
  nuiAnimation::Play(Count, LoopMode);
}

void nuiColorAttributeAnimation::OnFrame()
{
  const float r0 = mStartValue.Red();
  const float g0 = mStartValue.Green();
  const float b0 = mStartValue.Blue();
  const float a0 = mStartValue.Alpha();

  const float r1 = mEndValue.Red();
  const float g1 = mEndValue.Green();
  const float b1 = mEndValue.Blue();
  const float a1 = mEndValue.Alpha();
  
  const float rd = r1 - r0;
  const float gd = g1 - g0;
  const float bd = b1 - b0;
  const float ad = a1 - a0;
  
  const float pos = (float)GetPosition();

  const float r = r0 + pos * rd;
  const float g = g0 + pos * gd;
  const float b = b0 + pos * bd;
  const float a = a0 + pos * ad;
  
  nuiColor col(r, g, b, a);

//  NGL_OUT(_T("ColorAnim: pos[%.4f] %s\n"), pos, col.GetValue().GetChars());

  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  NGL_ASSERT(attrib.IsValid());
  
  nuiAttrib<nuiColor> color_attrib(attrib);
  nuiAttrib<const nuiColor&> const_color_attrib(attrib);
  
  if (color_attrib)
    color_attrib.Set(col);
  else if (const_color_attrib)
    const_color_attrib.Set(col);
}














/////////////////////////////////

// Rect Attrib Animation:

/////////////////////////////////

//// nuiRectAttributeAnimation:
nuiRectAttributeAnimation::nuiRectAttributeAnimation()
{
  if (SetObjectClass(_T("nuiRectAttributeAnimation")))
  {
    // Init atributes
  }
  
  mAnimLeft = true;
  mAnimRight = true;
  mAnimTop = true;
  mAnimBottom = true;
  mAnimWidth = true;
  mAnimHeight = true;

  mAutoRound = false;
}

nuiRectAttributeAnimation::~nuiRectAttributeAnimation()
{
}

void nuiRectAttributeAnimation::SetEndValue(const nuiRect& rVal)
{
  mEndValue = rVal;
}

const nuiRect& nuiRectAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiRectAttributeAnimation::SetStartValue(const nuiRect& rVal)
{
  mStartValue = rVal;
}

const nuiRect& nuiRectAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}



void nuiRectAttributeAnimation::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiRect> rect_attrib(attrib);
  nuiAttrib<const nuiRect&> const_rect_attrib(attrib);
  
  if (mCaptureStartOnPlay)
  {
    if (rect_attrib.IsValid())
      mStartValue = rect_attrib.Get();
    else if (const_rect_attrib)
      mStartValue = const_rect_attrib.Get();
  }
  if (mCaptureEndOnPlay)
  {
    if (rect_attrib)
      mEndValue = rect_attrib.Get();
    else if (const_rect_attrib)
      mEndValue = const_rect_attrib.Get();
  }
  
  nuiAnimation::Play(Count, LoopMode);
}

void nuiRectAttributeAnimation::OnFrame()
{
  float x0 = mStartValue.Left();
  float y0 = mStartValue.Top();
  float w0 = mStartValue.GetWidth();
  float h0 = mStartValue.GetHeight();
  
  const float x1 = mEndValue.Left();
  const float y1 = mEndValue.Top();
  const float w1 = mEndValue.GetWidth();
  const float h1 = mEndValue.GetHeight();

  if (!mAnimLeft)
    x0 = x1;
  if (!mAnimTop)
    y0 = y1;
  if (!mAnimWidth)
    w0 = w1;
  if (!mAnimHeight)
    h0 = h1;
  if (!mAnimRight)
    x0 = mEndValue.Right() - w0;
  if (!mAnimBottom)
    y0 = mEndValue.Bottom() - h0;
  
  const float xd = x1 - x0;
  const float yd = y1 - y0;
  const float wd = w1 - w0;
  const float hd = h1 - h0;
  
  const float pos = (float)GetPosition();
  
  const float x = x0 + pos * xd;
  const float y = y0 + pos * yd;
  const float w = w0 + pos * wd;
  const float h = h0 + pos * hd;
  
  nuiRect rect(x, y, w, h);
  
  if (mAutoRound)
    rect.RoundToNearest();
  
  //NGL_OUT(_T("rect anim: %s\n"), rect.GetValue().GetChars());
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  
  nuiAttrib<nuiRect> rect_attrib(attrib);
  nuiAttrib<const nuiRect&> const_rect_attrib(attrib);
  
  if (rect_attrib)
    rect_attrib.Set(rect);
  else if (const_rect_attrib)
    const_rect_attrib.Set(rect);
}


void nuiRectAttributeAnimation::SetLeftAnim(bool set)
{
  mAnimLeft = set;
}

void nuiRectAttributeAnimation::SetRightAnim(bool set)
{
  mAnimRight = set;
}

void nuiRectAttributeAnimation::SetTopAnim(bool set)
{
  mAnimTop = set;
}

void nuiRectAttributeAnimation::SetBottomAnim(bool set)
{
  mAnimBottom = set;
}

void nuiRectAttributeAnimation::SetWidthAnim(bool set)
{
  mAnimWidth = set;
}

void nuiRectAttributeAnimation::SetHeightAnim(bool set)
{
  mAnimHeight = set;
}


bool nuiRectAttributeAnimation::GetLeftAnim() const
{
  return mAnimLeft;
}

bool nuiRectAttributeAnimation::GetRightAnim() const
{
  return mAnimRight;
}

bool nuiRectAttributeAnimation::GetTopAnim() const
{
  return mAnimTop;
}

bool nuiRectAttributeAnimation::GetBottomAnim() const
{
  return mAnimBottom;
}

bool nuiRectAttributeAnimation::GetWidthAnim() const
{
  return mAnimWidth;
}

bool nuiRectAttributeAnimation::GetHeightAnim() const
{
  return mAnimHeight;
}

void nuiRectAttributeAnimation::SetAutoRound(bool set)
{
  mAutoRound = set;
}

bool nuiRectAttributeAnimation::GetAutoRound() const
{
  return mAutoRound;
}



/////////////////////////////////

// Matrix Attrib Animation:

/////////////////////////////////

//// nuiMatrixAttributeAnimation:
nuiMatrixAttributeAnimation::nuiMatrixAttributeAnimation()
{
  if (SetObjectClass(_T("nuiMatrixAttributeAnimation")))
  {
    // Init atributes
  }
}

nuiMatrixAttributeAnimation::~nuiMatrixAttributeAnimation()
{
}

void nuiMatrixAttributeAnimation::SetStartValue(const nuiMatrix& rVal)
{
  mStartValue = rVal;
}
const nuiMatrix& nuiMatrixAttributeAnimation::GetStartValue() const
{
  return mStartValue;
}

void nuiMatrixAttributeAnimation::SetEndValue(const nuiMatrix& rVal)
{
  mEndValue = rVal;
}
const nuiMatrix& nuiMatrixAttributeAnimation::GetEndValue() const
{
  return mEndValue;
}

void nuiMatrixAttributeAnimation::Play(int32 Count, nuiAnimLoop LoopMode)
{
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  NGL_ASSERT(attrib.IsValid());

  nuiAttrib<nuiMatrix> matrix_attrib(attrib);
  nuiAttrib<const nuiMatrix&> const_matrix_attrib(attrib);
  
  if (mCaptureStartOnPlay)
  {
    if (matrix_attrib)
      mStartValue = matrix_attrib.Get();
    else if (const_matrix_attrib)
      mStartValue = const_matrix_attrib.Get();
  }
  
  if (mCaptureEndOnPlay)
  {
    if (matrix_attrib)
      mEndValue = matrix_attrib.Get();
    else if (const_matrix_attrib)
      mEndValue = const_matrix_attrib.Get();
  }
  nuiAnimation::Play(Count, LoopMode);
}

void nuiMatrixAttributeAnimation::OnFrame()
{
  const nuiMatrix::ValueType pos = (nuiMatrix::ValueType)GetPosition();
  nuiMatrix frameValue = mStartValue;
  frameValue.Elt.M11 += (mEndValue.Elt.M11 - mStartValue.Elt.M11) * pos;
  frameValue.Elt.M12 += (mEndValue.Elt.M12 - mStartValue.Elt.M12) * pos;
  frameValue.Elt.M13 += (mEndValue.Elt.M13 - mStartValue.Elt.M13) * pos;
  frameValue.Elt.M14 += (mEndValue.Elt.M14 - mStartValue.Elt.M14) * pos;
  frameValue.Elt.M21 += (mEndValue.Elt.M21 - mStartValue.Elt.M21) * pos;
  frameValue.Elt.M22 += (mEndValue.Elt.M22 - mStartValue.Elt.M22) * pos;
  frameValue.Elt.M23 += (mEndValue.Elt.M23 - mStartValue.Elt.M23) * pos;
  frameValue.Elt.M24 += (mEndValue.Elt.M24 - mStartValue.Elt.M24) * pos;
  frameValue.Elt.M31 += (mEndValue.Elt.M31 - mStartValue.Elt.M31) * pos;
  frameValue.Elt.M32 += (mEndValue.Elt.M32 - mStartValue.Elt.M32) * pos;
  frameValue.Elt.M33 += (mEndValue.Elt.M33 - mStartValue.Elt.M33) * pos;
  frameValue.Elt.M34 += (mEndValue.Elt.M34 - mStartValue.Elt.M34) * pos;
  frameValue.Elt.M41 += (mEndValue.Elt.M41 - mStartValue.Elt.M41) * pos;
  frameValue.Elt.M42 += (mEndValue.Elt.M42 - mStartValue.Elt.M42) * pos;
  frameValue.Elt.M43 += (mEndValue.Elt.M43 - mStartValue.Elt.M43) * pos;
  frameValue.Elt.M44 += (mEndValue.Elt.M44 - mStartValue.Elt.M44) * pos;

  nglString frameValueStr;
  frameValue.GetValue(frameValueStr);
//  NGL_OUT(_T("MatrixAnim: pos[%.4f] %s\n"), pos, frameValueStr.GetChars());
  
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  NGL_ASSERT(attrib.IsValid());
  
  nuiAttrib<nuiMatrix> matrix_attrib(attrib);
  nuiAttrib<const nuiMatrix&> const_matrix_attrib(attrib);
  
  if (matrix_attrib)
    matrix_attrib.Set(frameValue);
  else if (const_matrix_attrib)
    const_matrix_attrib.Set(frameValue);
}

/////////////////////////////////

// Matrix Attrib Animation:

/////////////////////////////////

//// nuiRotateMatrixAttributeAnimation:
nuiRotateMatrixAttributeAnimation::nuiRotateMatrixAttributeAnimation()
{
  if (SetObjectClass(_T("nuiRotateMatrixAttributeAnimation")))
  {
    // Init atributes
  }
  
  mStartX = mStartY = mEndX = mEndY = mStartAngle = 0;
  mEndAngle = 360;
}

nuiRotateMatrixAttributeAnimation::~nuiRotateMatrixAttributeAnimation()
{
}

void nuiRotateMatrixAttributeAnimation::SetStartValue(float angle, float xcenter, float ycenter)
{
  mStartAngle = angle;
  mStartX = xcenter;
  mStartY = ycenter;
}

float nuiRotateMatrixAttributeAnimation::GetStartAngle() const
{
  return mStartAngle;
}

float nuiRotateMatrixAttributeAnimation::GetStartX() const
{
  return mStartX;
}

float nuiRotateMatrixAttributeAnimation::GetStartY() const
{
  return mStartY;
}

void nuiRotateMatrixAttributeAnimation::SetEndValue(float angle, float xcenter, float ycenter)
{
  mEndAngle = angle;
  mEndX = xcenter;
  mEndY = ycenter;
}

float nuiRotateMatrixAttributeAnimation::GetEndAngle() const
{
  return mEndAngle;
}

float nuiRotateMatrixAttributeAnimation::GetEndX() const
{
  return mEndX;
}

float nuiRotateMatrixAttributeAnimation::GetEndY() const
{
  return mEndY;
}

void nuiRotateMatrixAttributeAnimation::OnFrame()
{
  const float pos = GetPosition();
  const float angle = mStartAngle + pos * (mEndAngle - mStartAngle);
  const float x = mStartX + pos * (mEndX - mStartX);
  const float y = mStartY + pos * (mEndY - mStartY);
  
  nuiMatrix r;
  nuiMatrix t;
  nuiMatrix tt;
  nuiMatrix m;
  r.SetRotation(angle, 0, 0, 1);
  t.SetTranslation(-x, -y, 0);
  tt.SetTranslation(x, y, 0);
  m = tt * r * t;
  
  nuiAttribBase attrib(mpTarget->GetAttribute(mTarget));
  NGL_ASSERT(attrib.IsValid());
  
  nuiAttrib<nuiMatrix> matrix_attrib(attrib);
  nuiAttrib<const nuiMatrix&> const_matrix_attrib(attrib);
  
  if (matrix_attrib)
    matrix_attrib.Set(m);
  else if (const_matrix_attrib)
    const_matrix_attrib.Set(m);
}
