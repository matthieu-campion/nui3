#include "nui.h"

#include "nuiSurface.h"
#include "nuiTexture.h"
#include "nuiMetaPainter.h"

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
  //NGL_OUT(_T("nuiSurface::CreateSurface(%ls, %.1f, %.1f)\n"), rName.GetChars(), Width, Height);
  NGL_ASSERT(mpSurfaces.find(rName) == mpSurfaces.end());
  pSurface = new nuiSurface(rName, Width, Height, PixelFormat);
  pSurface->Acquire();
  mpSurfaces[rName] = pSurface;

//  NGL_OUT(_T("nuiSurface CreateSurface [0x%x] NAME: [%ls] COUNT [%d]\n"), pSurface, rName.GetChars(), mpSurfaces.size());

  return pSurface;
}

nuiSurface::nuiSurface(const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat)
  : nuiObject(), nuiDrawContext(nuiRect(Width, Height))
{
  SetObjectClass(_T("nuiSurface"));
  SetObjectName(rName);
  mCount = 0;
  mPermanent = false;
  mWidth = Width;
  mHeight= Height;
  mPixelFormat = PixelFormat;
  mDepth = false;
  mStencil = false;
  mRenderToTexture = false;
  mpTexture = NULL;
  mpSurfacePainter = new nuiMetaPainter(nuiRect(Width, Height), NULL);
#ifdef DEBUG
  mpSurfacePainter->DBGSetReferenceObject(this);
#endif
  mpSurfacePainter->SetDrawChildrenImmediat(true);
  mDirty = true;
  
  SetPainter(mpSurfacePainter);
  
  nuiSurfaceCacheSet::iterator it = mpSurfaceCaches.begin();
  nuiSurfaceCacheSet::iterator end = mpSurfaceCaches.end();
  while (it != end)
  {
    nuiSurfaceCache* pCache = *it;
    pCache->CreateSurface(this);
    ++it;
  }

//  NGL_OUT(_T("nuiSurface CTOR [0x%x] SIZE[%dx%d]\n"), this, Width, Height);
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

  if (mpPainter == mpSurfacePainter)
    SetPainter(NULL);
  delete mpSurfacePainter;

//  NGL_OUT(_T("nuiSurface DTOR [0x%x] NAME: [%ls] COUNT [%d]\n"), this, GetObjectName().GetChars(), mpSurfaces.size());
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

uint32 nuiSurface::Acquire()    
{ 
  return ++mCount; 
}

uint32 nuiSurface::Release()
{ 
  NGL_ASSERTR(mCount > 0, mCount); 
  mCount--;
  if (mCount == 0)
  {
    delete this;
    return 0;
  }
  return mCount; 
}

uint32 nuiSurface::GetRefCount()
{
  return mCount;
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

nuiMetaPainter* nuiSurface::GetSurfacePainter() const
{
  return mpSurfacePainter;
}

void nuiSurface::Wipe()
{
  mpSurfacePainter->Reset(NULL);
  mDirty = true;
}

void nuiSurface::Realize(nuiDrawContext* pDestinationPainter)
{
  //NGL_OUT(_T("nuiSurface::Realize() [%x]\n"), this);
  mpSurfacePainter->ReDraw(pDestinationPainter);
  mDirty = false;
}

bool nuiSurface::IsDirty() const
{
  return mDirty;
}
