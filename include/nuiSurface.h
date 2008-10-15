#ifndef __nuiSurface_h__
#define __nuiSurface_h__

#include "nuiObject.h"
#include "nuiRefCount.h"

#include <map>

class nuiSurface;
class nuiPainter;

typedef std::map<nglString, nuiSurface*, nglString::LessFunctor> nuiSurfaceMap;
//typedef std::set<nuiTextureCache*> nuiTextureCacheSet;

class nuiSurface : public nuiObject, public nuiRefCount
{
public:
  nuiSurface(const nuiSize& rWidth, const nuiSize& rHeight);
  virtual ~nuiSurface();

  nuiSize GetWidth() const;
  nuiSize GetHeight() const;

  bool IsDepthEnabled() const;
  bool IsStencilEnabled() const;
  
  void EnableDepth(bool Enable);
  void EnableStencil(bool Enable);

private:
  nuiSize mWidth;
  nuiSize mHeight;
  
  bool mDepthEnabled;
  bool mStencilEnabled;
};

#endif//__nuiSurface_h__
