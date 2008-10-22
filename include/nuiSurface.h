#ifndef __nuiSurface_h__
#define __nuiSurface_h__

#include "nuiObject.h"
#include "nuiRefCount.h"

#include <map>

class nuiSurface;
class nuiSurfaceCache;
class nuiPainter;
class nuiTexture;

typedef std::map<nglString, nuiSurface*, nglString::LessFunctor> nuiSurfaceMap;
typedef std::set<nuiSurfaceCache*> nuiSurfaceCacheSet;

class NUI_API nuiSurface : public nuiObject, public nuiRefCount
{
public:
  static nuiSurface* GetSurface (const nglString& rName); ///< Get a surface from its ID
  static nuiSurface* CreateSurface (const nglString& rName,
                                    const nuiSize& rWidth,
                                    const nuiSize& rHeight); ///< Create a surface

  static void AddCache(nuiSurfaceCache* pCache);
  static void DelCache(nuiSurfaceCache* pCache);

  nuiSize GetWidth() const;
  nuiSize GetHeight() const;

  void SetDepth(bool Enable);
  bool GetDepth() const;

  void SetStencil(bool Enable);  
  bool GetStencil() const;

  void SetRenderToTexture(bool Enable);
  bool GetRenderToTexture() const;

  void SetTexture(nuiTexture* pTexture, bool Acquired);
  nuiTexture* GetTexture() const;

protected:
  nuiSurface(const nuiSize& rWidth, const nuiSize& rHeight);
  virtual ~nuiSurface();

private:
  nuiSize mWidth;
  nuiSize mHeight;
  
  bool mDepth;
  bool mStencil;
  bool mRenderToTexture;
  
  nuiTexture* mpTexture;

  static nuiSurfaceMap mpSurfaces;
  static nuiSurfaceCacheSet mpSurfaceCaches;
};

class nuiSurfaceCache
{
public:
  nuiSurfaceCache();
  virtual ~nuiSurfaceCache();
  
  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void InvalidateSurface(nuiSurface* pSurface, bool ForceReload);
};

#endif//__nuiSurface_h__
