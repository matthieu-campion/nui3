/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglImage.h"
#include "nuiTexture.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"
#include "AAPrimitives.h"

#include "nuiSurface.h"

using namespace std;

nuiTextureMap nuiTexture::mpTextures;

#if 1
static inline void LOG_GETTEXTURE(nuiTexture* pTexture)
{
}
#else
static inline void LOG_GETTEXTURE(nuiTexture* pTexture)
{
  printf("nuiTexture::GetTexture: ");
  if (!pTexture) 
  {
    printf("NULL Texture\n"); 
  }
  printf("0x%p '%ls'\n", pTexture, pTexture->GetSource().GetChars());
}
#endif

nuiTexture* nuiTexture::GetTexture (nglIStream* pInput, nglImageCodec* pCodec)
{
  nuiTexture* pTexture = NULL;
  pTexture = new nuiTexture(pInput, pCodec);
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture (const nglPath& rPath, nglImageCodec* pCodec)
{
  nuiTexture* pTexture = NULL;
  nuiTextureMap::iterator it = mpTextures.find(rPath.GetPathName());
  if (it == mpTextures.end())
    pTexture = new nuiTexture(rPath, pCodec);
  else
    pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture (nglImageInfo& rInfo, bool Clone)
{
  nuiTexture* pTexture = NULL;
  if (Clone)
    pTexture = new nuiTexture(rInfo,Clone);
  else
  {
    nglString name;
    name.Format(_T("Info 0x%x"),&rInfo);
    nuiTextureMap::iterator it = mpTextures.find(name);
    if (it == mpTextures.end())
      pTexture = new nuiTexture(rInfo,Clone);
    else
      pTexture = it->second;
  }
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture (const nglImage& rImage)
{
  nuiTexture* pTexture = NULL;
  nglString name;
  name.Format(_T("Image 0x%x"),&rImage);
  nuiTextureMap::iterator it = mpTextures.find(name);
  if (it == mpTextures.end())
    pTexture = new nuiTexture(rImage);
  else
    pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture (nglImage* pImage, bool OwnImage)
{
  nuiTexture* pTexture = NULL;
  nglString name;
  name.Format(_T("Image 0x%x"),pImage);
  nuiTextureMap::iterator it = mpTextures.find(name);
  if (it == mpTextures.end())
    pTexture = new nuiTexture(pImage,OwnImage);
  else
    pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture(const nuiXMLNode* pNode)
{
  nuiTexture* pTexture = NULL;
  nuiTextureMap::iterator it = mpTextures.find(nuiGetString(pNode, _T("Source")));
  if (it == mpTextures.end())
    pTexture = new nuiTexture(pNode);
  else
    pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture(nuiSurface* pSurface)
{
  nuiTexture* pTexture = pSurface->GetTexture();
  if (!pTexture)
  {
    nglString name;
    name.Format(_T("Surface 0x%x"), pSurface);
    nuiTextureMap::iterator it = mpTextures.find(name);
    if (it == mpTextures.end())
      pTexture = new nuiTexture(pSurface);    
    else
      pTexture = it->second;
  }
  
  LOG_GETTEXTURE(pTexture);
  return pTexture;  
}

nuiTexture* nuiTexture::GetTexture(const nglString& rName)
{
  nuiTexture* pTexture = NULL;
  nuiTextureMap::iterator it = mpTextures.find(rName);
  if (it == mpTextures.end())
    return NULL;
  pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
  LOG_GETTEXTURE(pTexture);
  return pTexture;
}

//#TODO remove this and do something to have more general AAPrimitives.*
#define psz (phf * 2)
#define psm (psz - 1)
#define phs (phf * phf)
#define pdb (psz * 2)
#define pct (psz + phf)
#define prs ((phf-1)*(phf-1))

nuiTexture* nuiTexture::GetAATexture()
{
  nuiTexture* pTexture = NULL;
#ifndef __NUI_NO_AA__
  nuiTextureMap::iterator it = mpTextures.find(_T("nuiTextureAA"));
  if (it == mpTextures.end())
  {
    nglImageInfo info(true);
    info.mBufferFormat = eImageFormatRaw;  ///< Buffer data format can be raw (user accessible) or proprietary (opaque to user, such as S3TC/DXTC)
    info.mPixelFormat = eImagePixelAlpha;   ///< Pixel components and respective components bit resolution
    info.mWidth = pdb;                        ///< Image width in pixels (0 if \a mpBuffer is NULL)
    info.mHeight = pdb;                       ///< Image height in pixels (0 if \a mpBuffer is NULL)
    info.mBytesPerPixel = 1;                ///< Pixel allocation size in bytes (>= pixel bit depth, 0 if \a mpBuffer is NULL)
    info.mBitDepth = 8 * info.mBytesPerPixel;                     ///< Pixel bit depth (sum of components bit resolution, 0 if \a mpBuffer is NULL)
    info.mBytesPerLine = pdb * info.mBytesPerPixel;                 ///< Pixel row allocation size in bytes (>= pixel size * image width, 0 if \a mpBuffer is NULL)
    info.AllocateBuffer();
    uint8* buffer = (uint8*)info.mpBuffer;
    memset(buffer, 0, pdb * pdb * info.mBytesPerPixel);
    glAAGenerateAABuffer(0, 0, buffer);
    pTexture = nuiTexture::GetTexture(info);
    pTexture->SetSource(_T("nuiTextureAA"));
    pTexture->SetWrapS(GL_REPEAT);
    pTexture->SetWrapT(GL_REPEAT);
    pTexture->EnableAutoMipMap(true);
    pTexture->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    pTexture->SetMagFilter(GL_LINEAR);
  }
  else
  {
    pTexture = it->second;
  }
  
  if (pTexture)
    pTexture->Acquire();
#endif
  return pTexture;
}

void nuiTexture::ClearAll()
{
  nuiTextureMap::iterator it = mpTextures.begin();
  nuiTextureMap::iterator end = mpTextures.end();

  while (it != end)
  {
    nglString TexName(it->first);
    nuiTexture* pTex = it->second;
    //delete pTex; // the destructor of an nuiTexture removes that texture from the nuiTextureMap automatically so the only way not to get lost with the next it is to restart at the begining.
    pTex->Release();
    it = mpTextures.begin();
  }

  mpTextures.clear();
  mpSharedContext = NULL;
  TexturesChanged();
}

void nuiTexture::ForceReloadAll(bool Rebind)
{
  nuiTextureMap::iterator it = mpTextures.begin();
  nuiTextureMap::iterator end = mpTextures.end();

  while (it != end)
  {
    it->second->ForceReload(Rebind);
    ++it;
  }
  TexturesChanged();
}



//--------------------------------
nuiTexture::nuiTexture(nglIStream* pInput, nglImageCodec* pCodec)
  : nuiObject()
{
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();
  mpImage = new nglImage(pInput, pCodec);
  mpSurface = NULL;
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  static uint count = 0;
  nglString name;
  name.Format(_T("Stream #%d"),count);
  SetProperty(_T("Source"),name);
  mpTextures[name] = this;

  Init();
}

extern float NUI_SCALE_FACTOR;

nuiTexture::nuiTexture (const nglPath& rPath, nglImageCodec* pCodec)
  : nuiObject()
{
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();

  mpImage = NULL;
  
  float scale = 1.0f;
  if (NUI_SCALE_FACTOR > 1)
  {
    nglPath p(rPath);
    nglString path(p.GetRemovedExtension());
    nglString ext(p.GetExtension());
    nglString res(path);
    res.Add(_T("@2x.")).Add(ext);
    p = res;
    mpImage = new nglImage(p, pCodec);
    if (mpImage && mpImage->IsValid())
    {
      scale = 2.0f;
    }
    else
    {
      delete mpImage;
      mpImage = NULL;
    }
  }
  
  if (!mpImage)
  {
    mpImage = new nglImage(rPath, pCodec);
  }

  mpSurface = NULL;
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  SetProperty(_T("Source"),rPath.GetPathName());
  mpTextures[rPath.GetPathName()] = this;

  Init();
  SetScale(scale);
}

nuiTexture::nuiTexture (nglImageInfo& rInfo, bool Clone)
  : nuiObject()
{
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();
  mpImage = new nglImage(rInfo, eClone);
  mpSurface = NULL;
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;
       
  static uint count = 0;
  nglString name;
  if (Clone)
    name.Format(_T("ClonedInfo #%d"),count++);
  else 
    name.Format(_T("Info 0x%x"),&rInfo);
  SetProperty(_T("Source"),name);
  mpTextures[name] = this;

  Init();
}

nuiTexture::nuiTexture (const nglImage& rImage)
  : nuiObject()
{
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();
  mpImage = new nglImage(rImage);
  mpSurface = NULL;
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  nglString name;
  name.Format(_T("Image 0x%x"),mpImage);
  SetProperty(_T("Source"),name);
  mpTextures[name] = this;

  Init();
}

nuiTexture::nuiTexture (nglImage* pImage, bool OwnImage)
  : nuiObject()
{
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();
  mpImage = pImage;
  mpSurface = NULL;
  mOwnImage = OwnImage;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  nglString name;
  name.Format(_T("Image 0x%x"),mpImage);
  SetProperty(_T("Source"),name);
  mpTextures[name] = this;

  Init();
}

nuiTexture::nuiTexture(const nuiXMLNode* pNode)
{
  nuiObject::Load(pNode);
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();
  mpSurface = NULL;
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  nglPath path(nuiGetString(pNode, _T("Source")));
  mpImage = new nglImage(path);

  SetProperty(_T("Source"),path.GetPathName());

  mpTextures[path.GetPathName()] = this;

  Init();
}

nuiTexture::nuiTexture(nuiSurface* pSurface)
{
  if (SetObjectClass(_T("nuiTexture")))
    InitAttributes();

  mpImage = NULL;
  mpSurface = pSurface;
  mOwnImage = false;
  mForceReload = false;
  mRetainBuffer = false;

  nglString name;
  name.Format(_T("Surface 0x%x"), mpSurface);
  SetProperty(_T("Source"), name);
  mpTextures[name] = this;

  Init();
}

void nuiTexture::Init()
{
	mScale = 1;
  mRealWidth = 0;
  mRealHeight = 0;

  if (mpImage)
  {
    mRealWidth = (nuiSize)mpImage->GetWidth();
    mRealHeight = (nuiSize)mpImage->GetHeight();
    
    mPixelFormat = mpImage->GetPixelFormat();
  }
  else if (mpSurface)
  {
    mRealWidth = (nuiSize)mpSurface->GetWidth();
    mRealHeight = (nuiSize)mpSurface->GetHeight();
    
    mPixelFormat = eImagePixelRGBA;
  }
  mRealWidthPOT = mRealWidth;
  mRealHeightPOT = mRealHeight;

  //NGL_OUT(_T("nuiTexture::Init() (0x%x - [%f %f] source='%ls') COUNT: %d\n"), this, mRealWidth, mRealHeight, GetProperty(_T("Source")).GetChars(), mpTextures.size());

  if (mRealWidth > 0 && mRealHeight > 0)
  // Find the nearest bounding power of two size:
  {
    uint i;
    nuiSize val = 1;
    for (i=0; i<32; i++)
    {
      if (mRealWidthPOT <= val)
      {
        mRealWidthPOT = val;
        break;
      }
      val*=2;
    }

    val = 1;
    for (i=0; i<32; i++)
    {
      if (mRealHeightPOT <= val)
      {
        mRealHeightPOT = val;
        break;
      }
      val*=2;
    }
  }

  mMinFilter = GL_LINEAR;
  mMagFilter = GL_LINEAR;
//   mMinFilter = GL_NEAREST;
//   mMagFilter = GL_NEAREST;
#ifdef _OPENGL_ES_
  mWrapS = GL_CLAMP_TO_EDGE;
  mWrapT = GL_CLAMP_TO_EDGE;
#else
  mWrapS = GL_CLAMP;
  mWrapT = GL_CLAMP;
#endif
  mEnvMode = GL_MODULATE;
  mAutoMipMap = false;
  
  nuiTextureCacheSet::iterator it = mTextureCaches.begin();
  nuiTextureCacheSet::iterator end = mTextureCaches.end();
  while (it != end)
  {
    nuiTextureCache* pCache = *it;
    pCache->CreateTexture(this);
    ++it;
  }

  TexturesChanged();
}

bool nuiTexture::IsValid() const
{
  if (mpSurface)
    return GetWidth() && GetHeight();
  return mpImage && mpImage->IsValid() && GetWidth() && GetHeight();
}


nuiXMLNode* nuiTexture::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiObject::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;
  pNode->SetAttribute(_T("Source"),GetProperty(_T("Source")));
  return pNode;
}


nuiTexture::~nuiTexture()
{
  nuiTextureCacheSet::iterator it = mTextureCaches.begin();
  nuiTextureCacheSet::iterator end = mTextureCaches.end();
  while (it != end)
  {
    nuiTextureCache* pCache = *it;
    pCache->DestroyTexture(this);
    ++it;
  }
  
  
//  NGL_OUT(_T("nuiTexture::~nuiTexture(0x%x - [%f %f] source='%ls')\n"), this, mRealWidth, mRealHeight, GetProperty(_T("Source")).GetChars());

  if (mOwnImage)
    delete mpImage;

  if (mpSurface)
  {
    //mpSurface->Release();
  }
  mpTextures.erase(GetProperty(_T("Source")));
  TexturesChanged();
}

void nuiTexture::ForceReload(bool Rebind)
{
  if (!Rebind)
  {
    mForceReload = true;
  }
  else
  {
    mForceReload = false;
  }

  nuiTextureCacheSet::iterator it = mTextureCaches.begin();
  nuiTextureCacheSet::iterator end = mTextureCaches.end();
  while (it != end)
  {
    nuiTextureCache* pCache = *it;
    pCache->InvalidateTexture(this, mForceReload);
    ++it;
  }
}

void nuiTexture::ResetForceReload()
{
  mForceReload = false;
}

bool nuiTexture::IsPowerOfTwo() const
{
  return (mRealHeight == mRealHeightPOT) && (mRealWidth == mRealWidthPOT);
}


nglImage* nuiTexture::GetImage() const
{
  return mpImage;
}

void nuiTexture::ReleaseBuffer()
{
  if (mOwnImage) {
    mpImage->ReleaseBuffer();
  }
}

nuiSurface* nuiTexture::GetSurface() const
{
  return mpSurface;
}

void nuiTexture::ImageToTextureCoord(nuiAltSize& x, nuiAltSize& y) const
{
  nuiSize _x = x, _y = y;
  ImageToTextureCoord(_x, _y);
  x = _x;
  y = _y;
}

void nuiTexture::TextureToImageCoord(nuiAltSize& x, nuiAltSize& y) const
{
  nuiSize _x = x, _y = y;
  ImageToTextureCoord(_x, _y);
  x = _x;
  y = _y;
}

void nuiTexture::ImageToTextureCoord(nuiSize& x, nuiSize& y) const
{
  if (mRealWidth)
    x /= GetWidth();
  else  if (mpImage)
    x /= mpImage->GetWidth();


  if (mRealHeight)
    y /= GetHeight();
  else if (mpImage)
    y /= mpImage->GetHeight();
}

void nuiTexture::TextureToImageCoord(nuiSize& x, nuiSize& y) const
{
  if (mRealWidth)
    x *= GetWidth();
  else if (mpImage)
    x *= mpImage->GetWidth();

  if (mRealHeight)
    y *= GetHeight();
  else if (mpImage)
    y *= mpImage->GetHeight();
}

void nuiTexture::ImageToTextureCoord(nuiRect& rRect) const
{
  nuiSize x, y, xx, yy;
  x = rRect.Left();
  y = rRect.Top();
  xx = rRect.Right();
  yy = rRect.Bottom();
  ImageToTextureCoord(x, y);
  ImageToTextureCoord(xx, yy);
  
  rRect.Set(x, y, xx, yy, false);
}

void nuiTexture::TextureToImageCoord(nuiRect& rRect) const
{
  nuiSize x, y, xx, yy;
  x = rRect.Left();
  y = rRect.Top();
  xx = rRect.Right();
  yy = rRect.Bottom();
  TextureToImageCoord(x, y);
  TextureToImageCoord(xx, yy);
  
  rRect.Set(x, y, xx, yy, false);
}


GLuint nuiTexture::GetMinFilter() const
{
  return mMinFilter;
}

GLuint nuiTexture::GetMagFilter() const
{
  return mMagFilter;
}

GLuint nuiTexture::GetWrapS() const    
{
  return mWrapS;
}

GLuint nuiTexture::GetWrapT() const    
{
  return mWrapT;
}

GLuint nuiTexture::GetEnvMode() const  
{
  return mEnvMode;
}


void nuiTexture::SetMinFilter(GLuint Filter)
{
  mMinFilter = Filter;
}

void nuiTexture::SetMagFilter(GLuint Filter)
{
  mMagFilter = Filter;
}

void nuiTexture::SetWrapS(GLuint WrapS)     
{
  mWrapS = WrapS;
}

void nuiTexture::SetWrapT(GLuint WrapT)     
{
  mWrapT = WrapT;
}

void nuiTexture::SetEnvMode(GLuint Mode)    
{
  mEnvMode = Mode;
}

void nuiTexture::SetRetainBuffer(bool Retain)
{
  mRetainBuffer = Retain;
}

bool nuiTexture::SetSource(const nglString& rName)
{
  mpTextures.erase(GetProperty(_T("Source")));
  SetProperty(_T("Source"), rName);
  mpTextures[rName] = this;
  TexturesChanged();
  return true;
}

nglString nuiTexture::GetSource() const
{
  return GetProperty(_T("Source"));
}

uint32 nuiTexture::GetWidth() const
{
  return mRealWidth / mScale;  
}

uint32 nuiTexture::GetHeight() const
{
  return mRealHeight / mScale;
}

uint32 nuiTexture::GetUnscaledWidth() const
{
  return mRealWidth;  
}

uint32 nuiTexture::GetUnscaledHeight() const
{
  return mRealHeight;
}

uint32 nuiTexture::GetWidthPOT() const
{
  return mRealWidthPOT;
}

uint32 nuiTexture::GetHeightPOT() const
{
  return mRealHeightPOT;
}

float nuiTexture::GetScale() const
{
  return mScale;
}

void nuiTexture::SetScale(float scale)
{
  mScale = scale;
}

void nuiTexture::EnableAutoMipMap(bool Set)
{
  mAutoMipMap = Set;
  ForceReload(true);
}

bool nuiTexture::GetAutoMipMap() const
{
  return mAutoMipMap;
}

const nuiTextureMap& nuiTexture::Enum()
{
  return mpTextures;
}

nglImagePixelFormat nuiTexture::GetPixelFormat() const
{
  return mPixelFormat;
}

void nuiTexture::InitAttributes()
{
//  AddAttribute(new nuiAttribute<uint32>
//               (nglString(_T("MinFilter")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiTexture::GetMinFilter),
//                nuiMakeDelegate(this, &nuiTexture::SetMinFilter)));
//  
//  AddAttribute(new nuiAttribute<uint32>
//               (nglString(_T("MagFilter")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiTexture::GetMagFilter),
//                nuiMakeDelegate(this, &nuiTexture::SetMagFilter)));
//  
//  AddAttribute(new nuiAttribute<uint32>
//               (nglString(_T("WrapS")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiTexture::GetWrapS),
//                nuiMakeDelegate(this, &nuiTexture::SetWrapS)));
//  
//  AddAttribute(new nuiAttribute<uint32>
//               (nglString(_T("WrapT")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiTexture::GetWrapT),
//                nuiMakeDelegate(this, &nuiTexture::SetWrapT)));

  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("Width")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiTexture::GetWidth)));
  
  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("Height")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiTexture::GetHeight)));
  
  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("UnscaledWidth")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiTexture::GetUnscaledWidth)));
  
  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("UnscaledHeight")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiTexture::GetUnscaledHeight)));
  
  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("WidthPOT")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiTexture::GetWidthPOT)));
  
  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("HeightPOT")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiTexture::GetHeightPOT)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Scale")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiTexture::GetScale),
                nuiMakeDelegate(this, &nuiTexture::SetScale)));
  
  //  AddAttribute(new nuiAttribute<uint32>
//               (nglString(_T("EnvMode")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiTexture::GetEnvMode)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("AutoMipMap")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiTexture::GetAutoMipMap),
                nuiMakeDelegate(this, &nuiTexture::EnableAutoMipMap)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("RetainBuffer")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiTexture::IsBufferRetained),
                nuiMakeDelegate(this, &nuiTexture::SetRetainBuffer)));
  
//  AddAttribute(new nuiAttribute<const nglString&>
//               (nglString(_T("Source")), nuiUnitBoolean,
//                nuiMakeDelegate(this, &nuiTexture::SetSource),
//                nuiMakeDelegate(this, &nuiTexture::GetSource)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("UpToDate")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiTexture::IsUptoDate)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("IsPowerOfTwo")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiTexture::IsPowerOfTwo)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("IsValid")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiTexture::IsValid)));
  
}


/////////

nglContext* nuiTexture::mpSharedContext = NULL;


void nuiTexture::SetSharedContext(nglContext* pContext)
{
  mpSharedContext = pContext;
  if (!mpSharedContext)
    ForceReloadAll(true);
}

void nuiTexture::AddCache(nuiTextureCache* pCache)
{
  mTextureCaches.insert(pCache);
}

void nuiTexture::DelCache(nuiTextureCache* pCache)
{
  mTextureCaches.erase(pCache);
}

bool nuiTexture::mRetainBuffers = false;

void nuiTexture::RetainBuffers(bool Set)
{
  mRetainBuffers = Set;
}

nuiSimpleEventSource<0> nuiTexture::TexturesChanged;

