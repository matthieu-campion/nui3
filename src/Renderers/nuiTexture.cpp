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

using namespace std;

nuiTextureMap nuiTexture::mpTextures;

nuiTexture* nuiTexture::GetTexture (nglIStream* pInput, nglImageCodec* pCodec)
{
  nuiTexture* pTexture = NULL;
  pTexture = new nuiTexture(pInput, pCodec);
  if (pTexture)
    pTexture->Acquire();
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
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture (const nuiXMLNode* pNode)
{
  nuiTexture* pTexture = NULL;
  nuiTextureMap::iterator it = mpTextures.find(nuiGetString(pNode, _T("Source")));
  if (it == mpTextures.end())
    pTexture = new nuiTexture(pNode);
  else
    pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
  return pTexture;
}

nuiTexture* nuiTexture::GetTexture (const nglString& rName)
{
  nuiTexture* pTexture = NULL;
  nuiTextureMap::iterator it = mpTextures.find(rName);
  if (it == mpTextures.end())
    return NULL;
  pTexture = it->second;
  if (pTexture)
    pTexture->Acquire();
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
  nuiTextureMap::iterator it = mpTextures.find(_T("nuiTextureAA"));
  if (it == mpTextures.end())
  {
    nglImageInfo info(true);
    info.mBufferFormat = eImageFormatRaw;  ///< Buffer data format can be raw (user accessible) or proprietary (opaque to user, such as S3TC/DXTC)
    info.mPixelFormat = eImagePixelAlpha;   ///< Pixel components and respective components bit resolution
    info.mWidth = pdb;                        ///< Image width in pixels (0 if \a mpBuffer is NULL)
    info.mHeight = pdb;                       ///< Image height in pixels (0 if \a mpBuffer is NULL)
    info.mBitDepth = 8;                     ///< Pixel bit depth (sum of components bit resolution, 0 if \a mpBuffer is NULL)
    info.mBytesPerPixel = 1;                ///< Pixel allocation size in bytes (>= pixel bit depth, 0 if \a mpBuffer is NULL)
    info.mBytesPerLine = pdb;                 ///< Pixel row allocation size in bytes (>= pixel size * image width, 0 if \a mpBuffer is NULL)
    info.AllocateBuffer();
    uint8* buffer = (uint8*)info.mpBuffer;
    glAAGenerateAABuffer(0, 0, buffer);
    pTexture = nuiTexture::GetTexture(info, _T("nuiTextureAA"));
    pTexture->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    pTexture->SetMagFilter(GL_LINEAR_MIPMAP_LINEAR);
    pTexture->EnableAutoMipMap(true);
  }
  else
  {
    pTexture = it->second;
  }
  
  if (pTexture)
    pTexture->Acquire();
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
    delete pTex; // the destructor of an nuiTexture removes that texture from the nuiTextureMap automatically so the only way not to get lost with the next it is to restart at the begining.
    it = mpTextures.begin();
  }

  mpTextures.clear();
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
}



//--------------------------------
nuiTexture::nuiTexture(nglIStream* pInput, nglImageCodec* pCodec)
  : nuiObject()
{
  SetObjectClass(_T("nuiTexture"));
  mpImage = new nglImage(pInput, pCodec);
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

nuiTexture::nuiTexture (const nglPath& rPath, nglImageCodec* pCodec)
  : nuiObject()
{
  SetObjectClass(_T("nuiTexture"));
  mpImage = new nglImage(rPath, pCodec);
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  SetProperty(_T("Source"),rPath.GetPathName());
  mpTextures[rPath.GetPathName()] = this;

  Init();
}

nuiTexture::nuiTexture (nglImageInfo& rInfo, bool Clone)
  : nuiObject()
{
  SetObjectClass(_T("nuiTexture"));
  mpImage = new nglImage(rInfo, Clone);
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
  SetObjectClass(_T("nuiTexture"));
  mpImage = new nglImage(rImage);
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
  SetObjectClass(_T("nuiTexture"));
  mpImage = pImage;
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
  SetObjectClass(_T("nuiTexture"));
  mOwnImage = true;
  mForceReload = false;
  mRetainBuffer = mRetainBuffers;

  nglPath path(nuiGetString(pNode, _T("Source")));
  mpImage = new nglImage(path);

  SetProperty(_T("Source"),path.GetPathName());

  mpTextures[path.GetPathName()] = this;

  Init();
}

void nuiTexture::Init()
{	
//  NGL_OUT(_T("nuiTexture::Init() (0x%x - [%f %f] source='%ls') COUNT: %d\n"), this, mRealWidth, mRealHeight, GetProperty(_T("Source")).GetChars(), mpTextures.size());

  mRealWidth = 0;
  mRealHeight = 0;

  if (mpImage)
  {
    mRealWidth = (nuiSize)mpImage->GetWidth();
    mRealHeight = (nuiSize)mpImage->GetHeight();
    
    mRealWidthPOT = (nuiSize)mpImage->GetWidth();
    mRealHeightPOT = (nuiSize)mpImage->GetHeight();

    // Find the nearest bounding power of two size:
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

  mpTextures.erase(GetProperty(_T("Source")));
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
    x /= mRealWidth;
  else  if (mpImage)
    x /= mpImage->GetWidth();


  if (mRealHeight)
    y /= mRealHeight;
  else if (mpImage)
    y /= mpImage->GetHeight();
}

void nuiTexture::TextureToImageCoord(nuiSize& x, nuiSize& y) const
{
  if (mRealWidth)
    x *= mRealWidth;
  else if (mpImage)
    x *= mpImage->GetWidth();

  if (mRealHeight)
    y *= mRealHeight;
  else if (mpImage)
    y *= mpImage->GetHeight();
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
  return true;
}

nglString nuiTexture::GetSource() const
{
  return GetProperty(_T("Source"));
}

uint32 nuiTexture::GetWidth() const
{
  return mRealWidth;  
}

uint32 nuiTexture::GetHeight() const
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

void nuiTexture::EnableAutoMipMap(bool Set)
{
  mAutoMipMap = Set;
  ForceReload(true);
}

bool nuiTexture::GetAutoMipMap() const
{
  return mAutoMipMap;
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

// class nuiTextureCache
nuiTextureCache::nuiTextureCache()
{
  nuiTexture::AddCache(this);
}

nuiTextureCache::~nuiTextureCache()
{
  nuiTexture::DelCache(this);
}
    
void nuiTextureCache::CreateTexture(nuiTexture* pTexture)
{  
}

void nuiTextureCache::DestroyTexture(nuiTexture* pTexture)
{  
}

void nuiTextureCache::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
{  
}


nuiTextureCacheSet nuiTexture::mTextureCaches;

bool nuiTexture::mRetainBuffers = false;

void nuiTexture::RetainBuffers(bool Set)
{
  mRetainBuffers = Set;
}

