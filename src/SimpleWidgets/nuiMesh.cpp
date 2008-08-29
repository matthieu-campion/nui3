/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiApplication.h"
#include "nuiMesh.h"
#include "nuiMeshEngine.h"
#include "nuiMainWindow.h"
#include "nglMatrix.h"

using namespace std;


/*
 * Life cycle
 */

nuiMesh::nuiMesh() :
  nuiUserArea(),
  mUserDrawSink(this)
{
  Init();
}

bool nuiMesh::Load(const nuiXMLNode* pNode)
{
  nuiUserArea::Load(pNode);
  Init();

  if (pNode->HasAttribute(_T("Source")))
  {
    nglPath path(pNode->GetAttribute(_T("Source")));
    Load(path);
  }
  
  return true;
}

nuiMesh::~nuiMesh()
{
  if (mpEngine)
    delete mpEngine;
}

void nuiMesh::Init()
{
  SetObjectClass(_T("nuiMesh"));

  SetWantKeyboardFocus(true);
  mpEngine = NULL;
  mDemo = false;
  mDemoSpeed = 50.f;
  mUserDrawSink.Connect(this->UserDraw, &nuiMesh::OnUserDraw, GetDrawContext());
}


bool nuiMesh::Load (const nglPath& rPath)
{
  /* If the user did not provide its own engine via SetEngine()
   * we build a 'default' nuiMeshEngine
   */
  if (!mpEngine)
    mpEngine = new nuiMeshEngine();

  mViewMode = eViewNone;
  return mpEngine->Load(rPath, App->GetDebug() ? 1 : 0);
}

void nuiMesh::SetEngine (nuiMeshEngine* pEngine)
{
  if (mpEngine)
    delete mpEngine;

  mpEngine = pEngine;
}


/*
 * Size handling
 */

bool nuiMesh::RequestSize(nuiRect& rRect, nuiDrawContext* pContext)
{
  /* Difficult to claim for a meaningful size here.
   * We'll get something as a 'reasonable' 3D viewport.
   */
  rRect = nuiRect((nuiSize)0, (nuiSize)0, (nuiSize)256, (nuiSize)256);
  return true;
}

bool nuiMesh::SetSize (const nuiRect& rRect)
{
  mRect = rRect;
  return true;
}


/*
 * Rendering
 */

bool nuiMesh::OnUserDraw(const nuiEvent& rEvent)
{
  if (!mpEngine)
    return false; // No engine loaded (yet)

  nuiDrawContext* pContext = (nuiDrawContext*)rEvent.mpUser;

  mpEngine->Draw(pContext, mRect);

  if (mDemo)
  {
    nglTime now;

    StartView((nuiSize)0, (nuiSize)0);
    UpdateView((nuiSize)((now - mDemoStamp) * mDemoSpeed), (nuiSize)0);
    EndView();
    Invalidate();

    mDemoStamp = now;
  }

  return true;
}


/*
 * Navigation
 */

bool nuiMesh::KeyDown(const nglKeyEvent& rEvent)
{
  if (!mpEngine)
    return false;

  uint mode = mpEngine->GetMode();
  uint mode0 = mode;
  bool invalidate = false;

  switch (rEvent.mChar)
  {
    case _T('d'):
    case _T('D'):
      mDemo = !mDemo;
      if (mDemo)
      {
        mDemoStamp = nglTime();
        invalidate = true;
      }
      break;

    case _T('+'):
      if (mDemo) mDemoSpeed += 2.f;
      break;

    case _T('-'):
      if (mDemo && mDemoSpeed > 2.f) mDemoSpeed -= 2.f;
      break;

    case _T('l'):
    case _T('L'):
      mode ^= nuiMeshEngine::ModeNoLight;
      break;

    case _T('m'):
    case _T('M'):
      mode ^= nuiMeshEngine::ModeNoMaterial;
      break;

    case _T('w'):
    case _T('W'):
      mode ^= nuiMeshEngine::ModeWireFrame;
      break;
  }

  invalidate = invalidate || (mode != mode0);
  if (invalidate)
  {
    mpEngine->SetMode(mode);
    Invalidate();
  }
  return true;
}

bool nuiMesh::KeyUp(const nglKeyEvent& rEvent)
{
  return false;
}

bool nuiMesh::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mpEngine)
    return false;

  if (Button == nglMouseInfo::ButtonWheelDown)
  {
    // Show next camera:
    mpEngine->SetCamera((mpEngine->GetCamera() + 1) % mpEngine->GetCameraCnt());
    Invalidate();
    return true;
  }
  else if (Button == nglMouseInfo::ButtonWheelUp)
  {
    // Show previous camera:
    mpEngine->SetCamera((mpEngine->GetCamera() + mpEngine->GetCameraCnt() + 1) % mpEngine->GetCameraCnt());
    Invalidate();
    return true;
  }
  else if (Button == nglMouseInfo::ButtonLeft && !mDemo)
  {
    StartView(X, Y);
    return true;
  }

  return false;
}

bool nuiMesh::MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button == nglMouseInfo::ButtonLeft && !mDemo)
    EndView();
  return false;
}

bool nuiMesh::MouseMoved (nuiSize X, nuiSize Y)
{
  if (!mDemo && UpdateView((X - mXRef) * mXScale, (Y - mYRef) * mYScale))
    return true;

  return false;
}


void nuiMesh::StartView (nuiSize X, nuiSize Y)
{
  // Record mouse pointer position and current camera view vector
  mXRef = X;
  mYRef = Y;

  mpEngine->GetCamera(&mEyeRef, &mTargetRef);

  /* Compute horizon now (since mEyeRef and mTargetRef are constants) :
   * it is both perpendicular to the camera axis and the vertical up axis
   */
  nglVectorf axis(mEyeRef - mTargetRef);
  mHorizon = axis ^ nglVectorf(0.f, 1.f, 0.f);
  mHorizon.Normalize();
  mUp      = axis ^ mHorizon;
  mUp.Normalize();

  axis.Normalize();
  mGimbalAngle = (float)RAD2DEG(acos(axis * nglVectorf(0.f, 1.f, 0.f)));

  /* Mouse (pixels) to rotation/zoom angle ratio, use diagonal to keep up with weird ratios */
  float diag = nglVector2f(mRect.GetWidth(), mRect.GetHeight()).Length();

  if (IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL))
  {
    // Link camera panning amplitude with eye/target distance
    float len = axis.Length();

    mViewMode = eViewPan;
    mXScale = len / diag;
    mYScale = len / diag;
  }
  else
  if (IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT))
  {
    mViewMode = eViewZoom;
    mYScale = 1.5f / diag;
  }
  else
  {
    mViewMode = eViewRotate;
    mXScale = 2.5f * 180.f / diag;
    mYScale = 1.5f * 180.f / diag;
  }
  Grab();
}

bool nuiMesh::UpdateView (float DeltaX, float DeltaY)
{
  switch (mViewMode)
  {
    case eViewRotate:
    {
      nglVectorf eye = mEyeRef;
      nglMatrixf rot_x, rot_y;

      DeltaY = MIN(DeltaY, mGimbalAngle - 1.f);
      DeltaY = MAX(DeltaY, mGimbalAngle - 179.f);
      rot_x.SetRotation(DeltaY, mHorizon);
      rot_y.SetRotation(-DeltaX, nglVectorf(0.f, 1.f, 0.f));

      /* We rotate the eye position around the target position, using a
       * regular spherical referential
       */
      eye -= mTargetRef;
      {
        eye = rot_x * eye;
        eye = rot_y * eye;
      }
      eye += mTargetRef;

      mpEngine->SetCamera(&eye, NULL);
      Invalidate();
    }
    break;

    case eViewZoom:
    {
      nglVectorf eye = mEyeRef;

      eye -= mTargetRef;
      {
        eye *= 1.f + DeltaY;
      }
      eye += mTargetRef;

      mpEngine->SetCamera(&eye, NULL);
      Invalidate();
    }
    break;

    case eViewPan:
    {
      nglVectorf eye = mEyeRef;
      nglVectorf target = mTargetRef;
      nglVectorf delta = mHorizon * DeltaX - mUp * DeltaY;

      eye    += delta;
      target += delta;

      mpEngine->SetCamera(&eye, &target);
      Invalidate();
    }
    break;

    case eViewNone:
      return false;
  }
  return true;
}

void nuiMesh::EndView()
{
  if (mViewMode != eViewNone)
  {
    mViewMode = eViewNone;
    Ungrab();
  }
}
