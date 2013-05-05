#include "nui.h"

nuiSurfaceMap nuiSurface::mpSurfaces;
nuiSurfaceCacheSet nuiSurface::mpSurfaceCaches;

nuiSurface* nuiSurface::GetSurface (const nglString& rName, bool Acquired)
{
  nuiSurface* pSurface = NULL;

  nuiSurfaceMap::const_iterator it = mpSurfaces.find(rName);
  if (it != mpSurfaces.end())
  {
    pSurface = it->second;
    if (!Acquired)
      pSurface->Acquire();
  }
  return pSurface;
}

nuiSurface* nuiSurface::CreateSurface (const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat)
{
  nuiSurface* pSurface = NULL;
  //NGL_OUT(_T("nuiSurface::CreateSurface(%s, %.1f, %.1f)\n"), rName.GetChars(), Width, Height);
  nuiSurfaceMap::const_iterator it = mpSurfaces.find(rName);
  if (it != mpSurfaces.end())
  {
    pSurface = it->second;
    if (pSurface->GetWidth() == Width && pSurface->GetHeight() == Height && pSurface->GetPixelFormat() == PixelFormat)
    {
      pSurface->Acquire();
      return pSurface;
    }
  }

  pSurface = new nuiSurface(rName, Width, Height, PixelFormat);
  pSurface->Acquire();
  mpSurfaces[rName] = pSurface;

//  NGL_OUT(_T("nuiSurface CreateSurface [0x%x] NAME: [%s] COUNT [%d]\n"), pSurface, rName.GetChars(), mpSurfaces.size());

  return pSurface;
}

nuiSurface::nuiSurface(const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat)
  : nuiObject()
{
  SetObjectClass(_T("nuiSurface"));
  SetObjectName(rName);
  static bool tr = false;
  if (tr)
  {
    mTrace = true;
    tr = false;
  }
  mPermanent = false;
  mWidth = Width;
  mHeight= Height;
  mPixelFormat = PixelFormat;
  mDepth = 0;
  mStencil = 0;
  mRenderToTexture = true;
  mpTexture = NULL;
  mDirty = true;
  
  mpTexture = nuiTexture::GetTexture(this);
  mpTexture->Acquire();
  
//  NGL_OUT(_T("nuiSurface CTOR [0x%x] SIZE[%dx%d]\n"), this, Width, Height);
}

nuiSurface::~nuiSurface()
{
  mpSurfaces.erase(GetObjectName());

  if (mpTexture)
    mpTexture->Release();
//  NGL_OUT(_T("nuiSurface DTOR [0x%x] NAME: [%s] COUNT [%d]\n"), this, GetObjectName().GetChars(), mpSurfaces.size());

  auto it = mPainters.begin();
  auto end = mPainters.end();

  while (it != end)
  {
    nuiPainter* pPainter = *it;
    pPainter->DestroySurface(this);
    ++it;
  }
}

int32 nuiSurface::GetWidth() const
{
  return mWidth;
}

int32 nuiSurface::GetHeight() const
{
  return mHeight;
}

nglImagePixelFormat nuiSurface::GetPixelFormat() const
{
  return mPixelFormat;
}


void nuiSurface::SetDepth(int32 bits)
{
  mDepth = bits;
}

int32 nuiSurface::GetDepth() const
{
  return mDepth;
}

int32 nuiSurface::GetStencil() const
{
  return mStencil;
}
void nuiSurface::SetStencil(int32 bits)
{
  mStencil = bits;
}

bool nuiSurface::GetRenderToTexture() const
{
  return mRenderToTexture;
}

nuiTexture* nuiSurface::GetTexture() const
{
  return mpTexture;
}


void nuiSurface::AddCache(nuiSurfaceCache* pCache)
{
  mpSurfaceCaches.insert(pCache);
}

void nuiSurface::DelCache(nuiSurfaceCache* pCache)
{
  mpSurfaceCaches.erase(pCache);
}

void nuiSurface::SetPermanent(bool Permanent)
{
  if (Permanent)
  {
    if (mPermanent)
      return;
    Acquire();
    mPermanent = true;
  }
  else
  {
    if (!mPermanent)
      return;
    mPermanent = false;
    Release();
  }
}

bool nuiSurface::IsPermanent()
{
  return mPermanent;
}

void nuiSurface::Resize(int32 width, int32 height)
{
  auto it = mPainters.begin();
  auto end = mPainters.end();

  while (it != end)
  {
    nuiPainter* pPainter = *it;
    pPainter->ResizeSurface(this, width, height);
    ++it;
  }

  mWidth = width;
  mHeight = height;

  mpTexture->ResizeSurface(width, height);
}

void nuiSurface::AddPainter(nuiPainter* pPainter)
{
  mPainters.insert(pPainter);
}

void nuiSurface::DelPainter(nuiPainter* pPainter)
{
  mPainters.erase(pPainter);
}
