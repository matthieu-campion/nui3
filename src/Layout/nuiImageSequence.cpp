/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiImageSequence.h"

nuiImageSequence::nuiImageSequence()
: nuiWidget(), mFrameIndex(0)
{
  if (SetObjectClass(_T("nuiImageSequence")))
    InitAttributes();
  mColor = nuiColor(255,255,255);
  mInterpolated = true;
  mNbFrames = 0;
  mpTempImage = NULL;
  mRefreshTextures = true;
  mUseAlpha = true;
}

nuiImageSequence::nuiImageSequence(uint32 nbFrames, nglImage* pImage, nuiOrientation orientation, const nuiColor& rColor)
: nuiWidget(), mFrameIndex(0),
mColor(rColor)
{
  if (SetObjectClass(_T("nuiImageSequence")))
    InitAttributes();
  mInterpolated = true;
  mNbFrames = nbFrames;
  mOrientation = orientation;
  mpTempImage = new nglImage(*pImage);
  mRefreshTextures = true;  
  mUseAlpha = true;
}

nuiImageSequence::nuiImageSequence(uint32 nbFrames, const nglPath& rTexturePath, nuiOrientation orientation, const nuiColor& rColor)
: nuiWidget(), mFrameIndex(0),
mColor(rColor),
mGlobalTexturePath(rTexturePath)
{
  if (SetObjectClass(_T("nuiImageSequence")))
    InitAttributes();
	
  mpTempImage = new nglImage(rTexturePath);

  mInterpolated = true;
  mNbFrames = nbFrames;
  mOrientation = orientation;  
  mRefreshTextures = true;
  mUseAlpha = true;
}




void nuiImageSequence::InitAttributes()
{
  nuiAttribute<const nuiColor&>* AttributeColor = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color")), nuiUnitNone,
   nuiAttribute<const nuiColor&>::GetterDelegate(this, &nuiImageSequence::GetColor), 
   nuiAttribute<const nuiColor&>::SetterDelegate(this, &nuiImageSequence::SetColor));
  
  nuiAttribute<const nglPath&>* AttributeTexture = new nuiAttribute<const nglPath&>
  (nglString(_T("Texture")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiImageSequence::GetTexturePath), 
   nuiMakeDelegate(this, &nuiImageSequence::SetTexturePath));
  
  nuiAttribute<bool>* AttributeInterpolation = new nuiAttribute<bool>
  (nglString(_T("Interpolation")), nuiUnitBoolean,
   nuiAttribute<bool>::GetterDelegate(this, &nuiImageSequence::IsInterpolated),
   nuiAttribute<bool>::SetterDelegate(this, &nuiImageSequence::SetInterpolated));

  nuiAttribute<uint32>* AttributeNbFrames = new nuiAttribute<uint32>
  (nglString(_T("NbFrames")), nuiUnitNone,
   nuiAttribute<uint32>::GetterDelegate(this, &nuiImageSequence::GetNbFrames),
   nuiAttribute<uint32>::SetterDelegate(this, &nuiImageSequence::SetNbFrames));

  nuiAttribute<nglString>* AttributeOrientation = new nuiAttribute<nglString>
  (nglString(_T("Orientation")), nuiUnitNone,
   nuiAttribute<nglString>::GetterDelegate(this, &nuiImageSequence::GetOrientation),
   nuiAttribute<nglString>::SetterDelegate(this, &nuiImageSequence::SetOrientation));
  
  
  
	AddAttribute(_T("Color"), AttributeColor);
	AddAttribute(_T("Texture"), AttributeTexture);
	AddAttribute(_T("Interpolation"), AttributeInterpolation);
  
	AddAttribute(_T("NbFrames"), AttributeNbFrames);
  AddAttribute(_T("Orientation"), AttributeOrientation);
}



nuiImageSequence::~nuiImageSequence()
{
  for (uint32 i = 0; i < mTextures.size(); i++)
    mTextures[i]->Release();
}

bool nuiImageSequence::Load(const nuiXMLNode* pNode)
{
  mColor.SetValue(nuiGetString(pNode, _T("Color"), _T("white")));
  mpTempImage = new nglImage(nglPath(nuiGetString(pNode, _T("Texture"), nglString::Empty)));
  mRefreshTextures = true;

  return true;
}

nuiXMLNode* nuiImageSequence::Serialize(nuiXMLNode* pNode)
{
  pNode->SetName(_T("nuiImageSequence"));
  pNode->SetAttribute(_T("Color"), mColor.GetValue());
  
  pNode->SetAttribute(_T("Texture"), GetTexturePath());
  return pNode;
}




nuiRect nuiImageSequence::CalcIdealSize()
{
  // create the sequence textures the first time ::CalcIdealSize is called.
  // we don't do it in the decoration initialization because the decoration
  // may have been created from a css stylesheet, and its attributes are set in
  // a undefined order. We need all the attributed to be properly set before creating the textures.
  if (mRefreshTextures)
  {
    NGL_ASSERT(mpTempImage);
    
    bool res = CreateTextures();
    NGL_ASSERT(res);
    delete mpTempImage;
    mpTempImage = NULL;
    mRefreshTextures = false;
  }
  
  
  if (!mTextures.size())
    return mIdealRect;

  mIdealRect.Set(0.0f,0.0f,(nuiSize) mTexRect.GetWidth(),(nuiSize) mTexRect.GetHeight());
  return mIdealRect;
}


bool nuiImageSequence::CreateTextures()
{
  if (!mNbFrames)
    return false;
    
  if (!mpTempImage && !mTextures.size())
    return false;
  
  NGL_ASSERT(mpTempImage);
  
  if (mOrientation == nuiVertical)
    mTexRect.Set(0, 0, mpTempImage->GetWidth(), ToBelow(mpTempImage->GetHeight() / (float)mNbFrames));
  else
    mTexRect.Set(0, 0, ToBelow(mpTempImage->GetWidth() / (float)mNbFrames), mpTempImage->GetHeight());

  // clean existing textures
  for (uint32 i = 0; i < mTextures.size(); i++)
    mTextures[i]->Release();
  
  char* pBuffer = mpTempImage->GetBuffer();
  
  // create temp. buffer for destination
  uint32 dstBufferSize = mTexRect.GetWidth() * mTexRect.GetHeight() * 4;
  char* pDst = (char*)malloc(dstBufferSize);
  NGL_ASSERT(pDst);

    
  //*****************************************************************
  // frames are aligned vertically
  if (mOrientation == nuiVertical)
  {

    // copy and paste each frame from the image to an individual texture
    for (uint32 frame = 0; frame < mNbFrames; frame++)
    {
//      uint32 x = 0;
      uint32 y = frame * mTexRect.GetHeight();
      
      nglImageInfo info;
      mpTempImage->GetInfo(info);
      NGL_ASSERT((info.mPixelFormat == eImagePixelRGB) || (info.mPixelFormat == eImagePixelRGBA))

      char* pSrc = pBuffer + (y * info.mBytesPerLine);
                
      nglCopyLineFn pFunc = nglGetCopyLineFn(32, info.mBitDepth);
      NGL_ASSERT(pFunc);
      
      // copy part of the image
      pFunc(pDst, pSrc, mTexRect.GetWidth() * mTexRect.GetHeight(), false/*don't invert*/);
      
                
      // create a texture from the copyied buffer and store it
      info.mBufferFormat = eImageFormatRaw;
      info.mPixelFormat = eImagePixelRGBA;
      info.mWidth = mTexRect.GetWidth();
      info.mHeight = mTexRect.GetHeight();
      info.mBitDepth = 32;
      info.mBytesPerPixel = 4;
      info.mBytesPerLine = (mTexRect.GetWidth() * 4);
      info.mpBuffer = pDst;
                
      nuiTexture* pTex = nuiTexture::GetTexture(info, true/* clone the buffer */);
      mTextures.push_back(pTex);
      
      
      
      if (mInterpolated)
      {
        pTex->SetMinFilter(GL_LINEAR);
        pTex->SetMagFilter(GL_LINEAR);
      }
      else
      {
        pTex->SetMinFilter(GL_NEAREST);
        pTex->SetMagFilter(GL_NEAREST);
      }
      
      
    }
  }

  //*****************************************************************
  // frames are aligned horizontally
  else
  {
    
    // copy and paste each frame from the image to an individual texture
    for (uint32 frame = 0; frame < mNbFrames; frame++)
    {
      uint32 x = frame * mTexRect.GetWidth();
      
      nglImageInfo info;
      mpTempImage->GetInfo(info);
      NGL_ASSERT((info.mPixelFormat == eImagePixelRGB) || (info.mPixelFormat == eImagePixelRGBA))

      char* pSrc = pBuffer + (x * info.mBytesPerPixel);
      
      nglCopyLineFn pFunc = nglGetCopyLineFn(32, info.mBitDepth);
      NGL_ASSERT(pFunc);
      
      char* pDstPtr = pDst;

      // copy line per line
      for (uint32 y = 0; y < mTexRect.GetHeight(); y++)
      {        
        pSrc = pBuffer + (y * info.mBytesPerLine) + (x * info.mBytesPerPixel);
        
        pFunc(pDstPtr, pSrc, mTexRect.GetWidth(), false/*don't invert*/);
        
        pDstPtr += ((uint32)mTexRect.GetWidth() * 4);
      }
      
      // create a texture from the copyied buffer and store it
      info.mBufferFormat = eImageFormatRaw;
      info.mPixelFormat = eImagePixelRGBA;
      info.mWidth = mTexRect.GetWidth();
      info.mHeight = mTexRect.GetHeight();
      info.mBitDepth = 32;
      info.mBytesPerPixel = 4;
      info.mBytesPerLine = (mTexRect.GetWidth() * 4);
      info.mpBuffer = pDst;
      
      nuiTexture* pTex = nuiTexture::GetTexture(info, true/* clone the buffer */);
      mTextures.push_back(pTex);
      
      if (mInterpolated)
      {
        pTex->SetMinFilter(GL_LINEAR);
        pTex->SetMagFilter(GL_LINEAR);
      }
      else
      {
        pTex->SetMinFilter(GL_NEAREST);
        pTex->SetMagFilter(GL_NEAREST);
      }        
    }
  }
  
  // clean up
  free(pDst);
    
  
  return true;
}

uint32 nuiImageSequence::GetFrameIndex(nuiWidget* pWidget) const
{
  return mFrameIndex;
}

void nuiImageSequence::SetFrameIndex(uint32 index)
{
  mFrameIndex = index;
}

uint32 nuiImageSequence::GetNbFrames() const
{
  return mNbFrames;
}


void nuiImageSequence::SetNbFrames(uint32 nbFrames)
{
  mNbFrames = nbFrames;
  mRefreshTextures = true;  
}


void nuiImageSequence::SetOrientation(nglString orientation)
{
  if (!orientation.Compare(_T("Vertical"), false))
    mOrientation = nuiVertical;
  else
    mOrientation = nuiHorizontal;
  
  mRefreshTextures = true;  
}

nglString nuiImageSequence::GetOrientation()
{
  if (mOrientation == nuiVertical)
    return nglString(_T("Vertical"));
  
  return nglString(_T("Horizontal"));
}



const nglPath& nuiImageSequence::GetTexturePath() const
{
  if (HasProperty(_T("Texture")))
    return GetProperty(_T("Texture"));
  
  return mGlobalTexturePath;
}

void nuiImageSequence::SetTexturePath(const nglPath& rPath)
{
  SetProperty(_T("Texture"), mGlobalTexturePath);
  mpTempImage = new nglImage(rPath);
  NGL_ASSERT(mpTempImage);
  mRefreshTextures = true;
  
}


// virtual
bool nuiImageSequence::Draw(nuiDrawContext* pContext, nuiWidget* pWidget)
{
  //  int x=0,y=0;
  if (!mTextures.size() || (mFrameIndex >= mTextures.size()))
    return false;
  
  pContext->PushState();
  pContext->ResetState();
  pContext->SetTexture(mTextures[mFrameIndex]);

  pContext->EnableTexturing(true);
  
  float alpha = 1.0f;
  
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);

  if (mUseAlpha && pWidget)
  {
    alpha = pWidget->GetAlpha();
  }
  
  const nuiRect& destRect = mRect.Size();
  
  nuiColor color = nuiColor(1.0f, 1.0f, 1.0f, alpha);
    
  pContext->SetFillColor(color);
  pContext->DrawImage(destRect,mTexRect);
  
  pContext->EnableBlending(false);
  pContext->EnableTexturing(false);
  
  pContext->PopState();
  return true;
  
}






const nuiColor& nuiImageSequence::GetColor() const
{
	return mColor;
}

void nuiImageSequence::SetColor(const nuiColor& color)
{
	mColor = color;
}


bool nuiImageSequence::IsInterpolated()
{
  return mInterpolated;
}

void nuiImageSequence::SetInterpolated(bool set)
{
  mInterpolated = set;
  
  for (uint32 i = 0; i < mTextures.size(); i++)
  {
    nuiTexture* pTexture = mTextures[i];
    if (mInterpolated)
    {
      pTexture->SetMinFilter(GL_LINEAR);
      pTexture->SetMagFilter(GL_LINEAR);
    }
    else
    {
      pTexture->SetMinFilter(GL_NEAREST);
      pTexture->SetMagFilter(GL_NEAREST);    
    }
  }
}


