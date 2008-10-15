#include "nui.h"

#include "nuiSurface.h"

nuiSurface::nuiSurface(const nuiSize& rWidth, const nuiSize& rHeight)
  : nuiObject()
{
  SetObjectClass(_T("nuiSurface"));
  mWidth = rWidth;
  mHeight= rHeight;
  mDepthEnabled = false;
  mStencilEnabled = false;
}

nuiSurface::~nuiSurface()
{
}

nuiSize nuiSurface::GetWidth() const
{
  return mWidth;
}

nuiSize nuiSurface::GetHeight() const
{
  return mHeight;
}

void nuiSurface::EnableDepth(bool Enable)
{
  mDepthEnabled = Enable;
}
void nuiSurface::EnableStencil(bool Enable)
{
  mStencilEnabled = Enable;
}

bool nuiSurface::IsDepthEnabled() const
{
  return mDepthEnabled;
}

bool nuiSurface::IsStencilEnabled() const
{
  return mStencilEnabled;
}

