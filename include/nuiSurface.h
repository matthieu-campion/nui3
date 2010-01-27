#ifndef __nuiSurface_h__
#define __nuiSurface_h__

#include "nuiObject.h"
#include "nuiDrawContext.h"

class nuiSurface;
class nuiSurfaceCache;
class nuiPainter;
class nuiMetaPainter;
class nuiTexture;

typedef std::map<nglString, nuiSurface*, nglString::LessFunctor> nuiSurfaceMap;
typedef std::set<nuiSurfaceCache*> nuiSurfaceCacheSet;

class NUI_API nuiSurface : public nuiObject, public nuiDrawContext
{
public:
  static nuiSurface* GetSurface (const nglString& rName, bool Acquired); ///< Get a surface from its ID
  static nuiSurface* CreateSurface (const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat = eImagePixelRGBA); ///< Create a surface

  static void AddCache(nuiSurfaceCache* pCache);
  static void DelCache(nuiSurfaceCache* pCache);

  int32 GetWidth() const;
  int32 GetHeight() const;

  nglImagePixelFormat GetPixelFormat() const;
  
  void SetDepth(bool Enable);
  bool GetDepth() const;

  void SetStencil(bool Enable);  
  bool GetStencil() const;

  void SetRenderToTexture(bool Enable);
  bool GetRenderToTexture() const;

  void SetTexture(nuiTexture* pTexture);
  nuiTexture* GetTexture() const;

  uint32 Acquire();    
  uint32 Release();
  uint GetRefCount();
  void SetPermanent(bool Permanent = true);
  bool IsPermanent();

  void Wipe(); ///< Completely reset the surface and all associated operations
  
  bool IsDirty() const;
  void SetDirty(bool set = true);
  
  nuiMetaPainter* GetSurfacePainter() const;
  void Realize(nuiDrawContext* pDestinationPainter);

protected:
  nuiSurface(const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat = eImagePixelRGBA);
  virtual ~nuiSurface();

private:
  uint32 mCount;
  bool mPermanent;

  int32 mWidth;
  int32 mHeight;
  
  bool mDepth;
  bool mStencil;
  bool mRenderToTexture;

  nglImagePixelFormat mPixelFormat;
  
  nuiTexture* mpTexture;
  nuiMetaPainter* mpSurfacePainter;

  static nuiSurfaceMap mpSurfaces;
  static nuiSurfaceCacheSet mpSurfaceCaches;
  
  bool mDirty;
};

#endif//__nuiSurface_h__
