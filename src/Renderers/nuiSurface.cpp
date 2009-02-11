#include "nui.h"

#include "nuiSurface.h"
#include "nuiTexture.h"

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

nuiSurface* nuiSurface::CreateSurface (const nglString& rName,
                                       const nuiSize& rWidth,
                                       const nuiSize& rHeight)
{
  nuiSurface* pSurface = NULL;
  NGL_OUT(_T("nuiSurface::CreateSurface(%ls, %.1f, %.1f)\n"), rName.GetChars(), rWidth, rHeight);
  NGL_ASSERT(mpSurfaces.find(rName) == mpSurfaces.end());
  pSurface = new nuiSurface(rWidth, rHeight);
  pSurface->SetObjectName(rName);
  pSurface->Acquire();

  mpSurfaces[rName] = pSurface;
  return pSurface;
}

nuiSurface::nuiSurface(const nuiSize& rWidth, const nuiSize& rHeight)
  : nuiObject()
{
  SetObjectClass(_T("nuiSurface"));
  mWidth = rWidth;
  mHeight= rHeight;
  mDepth = false;
  mStencil = false;
  mRenderToTexture = false;
  mpTexture = NULL;
  
  nuiSurfaceCacheSet::iterator it = mpSurfaceCaches.begin();
  nuiSurfaceCacheSet::iterator end = mpSurfaceCaches.end();
  while (it != end)
  {
    nuiSurfaceCache* pCache = *it;
    pCache->CreateSurface(this);
    ++it;
  }  
}

nuiSurface::~nuiSurface()
{
  nuiSurfaceCacheSet::iterator it = mpSurfaceCaches.begin();
  nuiSurfaceCacheSet::iterator end = mpSurfaceCaches.end();
  while (it != end)
  {
    nuiSurfaceCache* pCache = *it;
    pCache->DestroySurface(this);
    ++it;
  }
  mpSurfaces.erase(GetObjectName());
}

nuiSize nuiSurface::GetWidth() const
{
  return mWidth;
}
nuiSize nuiSurface::GetHeight() const
{
  return mHeight;
}

void nuiSurface::SetDepth(bool Enable)
{
  mDepth = Enable;
}
bool nuiSurface::GetDepth() const
{
  return mDepth;
}

bool nuiSurface::GetStencil() const
{
  return mStencil;
}
void nuiSurface::SetStencil(bool Enable)
{
  mStencil = Enable;
}

void nuiSurface::SetRenderToTexture(bool Enable)
{
  mRenderToTexture = Enable;
}
bool nuiSurface::GetRenderToTexture() const
{
  return mRenderToTexture;
}

void nuiSurface::SetTexture(nuiTexture* pTexture)
{
  mpTexture = pTexture;
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

// nuiSurfaceCache
nuiSurfaceCache::nuiSurfaceCache()
{
  nuiSurface::AddCache(this);
}

nuiSurfaceCache::~nuiSurfaceCache()
{
  nuiSurface::DelCache(this);
}

void nuiSurfaceCache::CreateSurface(nuiSurface* pSurface)
{  
}

void nuiSurfaceCache::DestroySurface(nuiSurface* pSurface)
{  
}

void nuiSurfaceCache::InvalidateSurface(nuiSurface* pSurface, bool ForceReload)
{  
}

