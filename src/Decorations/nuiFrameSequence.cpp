/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiFrameSequence.h"

nuiFrameSequence::nuiFrameSequence(const nglString& rName)
: nuiDecoration(rName),
mBorderEnabled(true)
{
  if (SetObjectClass(_T("nuiFrameSequence")))
    InitAttributes();
  mColor = nuiColor(255,255,255);
  mInterpolated = true;
  mNbFrames = 0;
}

nuiFrameSequence::nuiFrameSequence(const nglString& rName, uint32 nbFrames, nglImage* pImage, nuiOrientation orientation, const nuiRect& rClientRect, const nuiColor& rColor)
: nuiDecoration(rName),
mColor(rColor),
mClientRect(rClientRect),
mBorderEnabled(true)
{
  if (SetObjectClass(_T("nuiFrameSequence")))
    InitAttributes();
  mInterpolated = true;
  mNbFrames = nbFrames;
  mOrientation = orientation;
  
  UpdateTexSize(pImage);
    
}

nuiFrameSequence::nuiFrameSequence(const nglString& rName, uint32 nbFrames, const nglPath& rTexturePath, nuiOrientation orientation, const nuiRect& rClientRect, const nuiColor& rColor)
: nuiDecoration(rName),
mColor(rColor),
mClientRect(rClientRect),
mBorderEnabled(true),
mGlobalTexturePath(rTexturePath)
{
  if (SetObjectClass(_T("nuiFrameSequence")))
    InitAttributes();
	
  nglImage* pImage = new nglImage(rTexturePath);

  mInterpolated = true;
  mNbFrames = nbFrames;
  mOrientation = orientation;
  
  UpdateTexSize(pImage);
  
  delete pImage;
}




void nuiFrameSequence::InitAttributes()
{
  nuiAttribute<bool>* AttributeBorder = new nuiAttribute<bool>
  (nglString(_T("EnableBorder")), nuiUnitBoolean,
   nuiAttribute<bool>::GetterDelegate(this, &nuiFrameSequence::IsBorderEnabled),
   nuiAttribute<bool>::SetterDelegate(this, &nuiFrameSequence::EnableBorder));
  
  nuiAttribute<const nuiRect&>* AttributeRect = new nuiAttribute<const nuiRect&>
  (nglString(_T("ClientRect")), nuiUnitNone,
   nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiFrameSequence::GetSourceClientRect),
   nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiFrameSequence::SetSourceClientRect));
  
  nuiAttribute<const nuiColor&>* AttributeColor = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color")), nuiUnitNone,
   nuiAttribute<const nuiColor&>::GetterDelegate(this, &nuiFrameSequence::GetColor), 
   nuiAttribute<const nuiColor&>::SetterDelegate(this, &nuiFrameSequence::SetColor));
  
  nuiAttribute<const nglPath&>* AttributeTexture = new nuiAttribute<const nglPath&>
  (nglString(_T("Texture")), nuiUnitNone,
   nuiFastDelegate::MakeDelegate(this, &nuiFrameSequence::GetTexturePath), 
   nuiFastDelegate::MakeDelegate(this, &nuiFrameSequence::SetTexturePath));
  
  nuiAttribute<bool>* AttributeInterpolation = new nuiAttribute<bool>
  (nglString(_T("Interpolation")), nuiUnitBoolean,
   nuiAttribute<bool>::GetterDelegate(this, &nuiFrameSequence::IsInterpolated),
   nuiAttribute<bool>::SetterDelegate(this, &nuiFrameSequence::SetInterpolated));

  nuiAttribute<uint32>* AttributeNbFrames = new nuiAttribute<uint32>
  (nglString(_T("NbFrames")), nuiUnitNone,
   nuiAttribute<uint32>::GetterDelegate(this, &nuiFrameSequence::GetNbFrames),
   nuiAttribute<uint32>::SetterDelegate(this, &nuiFrameSequence::SetNbFrames));

  nuiAttribute<nglString>* AttributeOrientation = new nuiAttribute<nglString>
  (nglString(_T("Orientation")), nuiUnitNone,
   nuiAttribute<nglString>::GetterDelegate(this, &nuiFrameSequence::GetOrientation),
   nuiAttribute<nglString>::SetterDelegate(this, &nuiFrameSequence::SetOrientation));
  
  
  
	AddAttribute(_T("EnableBorder"), AttributeBorder);
	AddAttribute(_T("ClientRect"), AttributeRect);
	AddAttribute(_T("Color"), AttributeColor);
	AddAttribute(_T("Texture"), AttributeTexture);
	AddAttribute(_T("Interpolation"), AttributeInterpolation);
  
	AddAttribute(_T("NbFrames"), AttributeNbFrames);
  AddAttribute(_T("Orientation"), AttributeOrientation);
}



nuiFrameSequence::~nuiFrameSequence()
{
  for (uint32 i = 0; i < mTextures.size(); i++)
    mTextures[i]->Release();
}

bool nuiFrameSequence::Load(const nuiXMLNode* pNode)
{
  mColor.SetValue(nuiGetString(pNode, _T("Color"), _T("white")));
  mClientRect.SetValue(nuiGetString(pNode, _T("ClientRect"), _T("{0,0,0,0}")));
  nglImage* pImage = new nglImage(nglPath(nuiGetString(pNode, _T("Texture"), nglString::Empty)));
  UpdateTexSize(pImage);
  delete pImage;
  return true;
}

nuiXMLNode* nuiFrameSequence::Serialize(nuiXMLNode* pNode)
{
  pNode->SetName(_T("nuiFrameSequence"));
  pNode->SetAttribute(_T("Color"), mColor.GetValue());
  pNode->SetAttribute(_T("ClientRect"), mClientRect.GetValue());
  
  pNode->SetAttribute(_T("Texture"), GetTexturePath());
  return pNode;
}


void nuiFrameSequence::UpdateTexSize(nglImage* pImage)
{
  if (!mNbFrames)
    return;
    
  if (!pImage && !mTextures.size())
    return;
  
  NGL_ASSERT(pImage);
  
  if (mOrientation == nuiVertical)
    mTexRect.Set(0, 0, pImage->GetWidth(), ToBelow(pImage->GetHeight() / (float)mNbFrames));
  else
    mTexRect.Set(0, 0, ToBelow(pImage->GetWidth() / (float)mNbFrames), pImage->GetHeight());

  if (GetSourceClientRect() == nuiRect(0,0,0,0))
    SetSourceClientRect(mTexRect);  
  
  if (pImage)
  {
    // clean existing textures
    for (uint32 i = 0; i < mTextures.size(); i++)
      mTextures[i]->Release();
    
    char* pBuffer = pImage->GetBuffer();
    
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
        uint32 x = 0;
        uint32 y = frame * mTexRect.GetHeight();
        
        nglImageInfo info;
        pImage->GetInfo(info);
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
        pImage->GetInfo(info);
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
    
  }
    
  
}

uint32 nuiFrameSequence::GetFrameIndex(nuiWidget* pWidget) const
{
  std::map<nuiWidget*, uint32>::const_iterator it = mIndex.find(pWidget);
  if (it == mIndex.end())
    return 0;
  
  return it->second;
}

void nuiFrameSequence::SetFrameIndex(nuiWidget* pWidget, uint32 index)
{
  mIndex[pWidget] = index;
}

uint32 nuiFrameSequence::GetNbFrames() const
{
  return mNbFrames;
}


void nuiFrameSequence::SetNbFrames(uint32 nbFrames)
{
  mNbFrames = nbFrames;
  UpdateTexSize();
}


void nuiFrameSequence::SetOrientation(nglString orientation)
{
  if (!orientation.Compare(_T("Vertical"), false))
    mOrientation = nuiVertical;
  else
    mOrientation = nuiHorizontal;
  
  UpdateTexSize();
  
}

nglString nuiFrameSequence::GetOrientation()
{
  if (mOrientation == nuiVertical)
    return nglString(_T("Vertical"));
  
  return nglString(_T("Horizontal"));
}



const nglPath& nuiFrameSequence::GetTexturePath() const
{
  if (HasProperty(_T("Texture")))
    return GetProperty(_T("Texture"));
  
  return mGlobalTexturePath;
}

void nuiFrameSequence::SetTexturePath(const nglPath& rPath)
{
  SetProperty(_T("Texture"), mGlobalTexturePath);
  nglImage* pImage = new nglImage(rPath);
  NGL_ASSERT(pImage);
  
  UpdateTexSize(pImage);
  delete pImage;
}


// virtual
void nuiFrameSequence::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  nuiRenderArray array(GL_TRIANGLES);
  array.EnableArray(nuiRenderArray::eVertex, true);
  array.EnableArray(nuiRenderArray::eTexCoord, true);
  array.EnableArray(nuiRenderArray::eColor, false);
  
  
  nuiSize w = mTexRect.GetWidth(), h = mTexRect.GetHeight();
  
  uint32 index;
  std::map<nuiWidget*, uint32>::iterator it = mIndex.find(pWidget);
  if (it == mIndex.end())
    index = 0;
  else
    index = it->second;
  
  NGL_ASSERT(mTextures.size() > index);
  nuiTexture* pTexture = mTextures[index];
  
  float X0 = 0;
  float X1 = (float)mClientRect.Left();
  float X2 = (float)mClientRect.Right();
  float X3 = (float)w;
  
  float Y0 = 0;
  float Y1 = (float)mClientRect.Top();
  float Y2 = (float)mClientRect.Bottom();
  float Y3 = (float)h;
  
  const float x0 = (float)rDestRect.Left();
  const float x1 = x0 + X1;
  const float x3 = (float)rDestRect.Right();
  const float x2 = x3 - (X3 - X2);
  
  const float y0 = (float)rDestRect.Top();
  const float y1 = y0 + Y1;
  const float y3 = (float)rDestRect.Bottom();
  const float y2 = y3 - (Y3 - Y2);
  
  pTexture->ImageToTextureCoord(X0, Y0);
  pTexture->ImageToTextureCoord(X1, Y1);
  pTexture->ImageToTextureCoord(X2, Y2);
  pTexture->ImageToTextureCoord(X3, Y3);
  
  //////// TOP PART
  // TopLeft rect:
  array.SetVertex(x0, y0);
  array.SetTexCoords(X0, Y0);
  array.PushVertex();
  
  array.SetVertex(x1, y0);
  array.SetTexCoords(X1, Y0);
  array.PushVertex();
  
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  ///
  array.SetVertex(x0, y0);
  array.SetTexCoords(X0, Y0);
  array.PushVertex();
  
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  array.SetVertex(x0, y1);
  array.SetTexCoords(X0, Y1);
  array.PushVertex();
  
  // TopMiddle rect:
  array.SetVertex(x1, y0);
  array.SetTexCoords(X1, Y0);
  array.PushVertex();
  
  array.SetVertex(x2, y0);
  array.SetTexCoords(X2, Y0);
  array.PushVertex();
  
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  ///
  array.SetVertex(x1, y0);
  array.SetTexCoords(X1, Y0);
  array.PushVertex();
  
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  // TopRight rect:
  array.SetVertex(x2, y0);
  array.SetTexCoords(X2, Y0);
  array.PushVertex();
  
  array.SetVertex(x3, y0);
  array.SetTexCoords(X3, Y0);
  array.PushVertex();
  
  array.SetVertex(x3, y1);
  array.SetTexCoords(X3, Y1);
  array.PushVertex();
  
  ///
  array.SetVertex(x2, y0);
  array.SetTexCoords(X2, Y0);
  array.PushVertex();
  
  array.SetVertex(x3, y1);
  array.SetTexCoords(X3, Y1);
  array.PushVertex();
  
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  ///// MIDDLE PART
  // LeftSide rect:
  array.SetVertex(x0, y1);
  array.SetTexCoords(X0, Y1);
  array.PushVertex();
  
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  ///
  array.SetVertex(x0, y1);
  array.SetTexCoords(X0, Y1);
  array.PushVertex();
  
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  array.SetVertex(x0, y2);
  array.SetTexCoords(X0, Y2);
  array.PushVertex();
  
  // Middle (client) rect:
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  ///
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  // RightSide rect:
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  array.SetVertex(x3, y1);
  array.SetTexCoords(X3, Y1);
  array.PushVertex();
  
  array.SetVertex(x3, y2);
  array.SetTexCoords(X3, Y2);
  array.PushVertex();
  
  ///
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  array.SetVertex(x3, y2);
  array.SetTexCoords(X3, Y2);
  array.PushVertex();
  
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  
  
  ///// BOTTOM PART
  // BottomLeft rect:
  array.SetVertex(x0, y2);
  array.SetTexCoords(X0, Y2);
  array.PushVertex();
  
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  array.SetVertex(x1, y3);
  array.SetTexCoords(X1, Y3);
  array.PushVertex();
  
  ///
  array.SetVertex(x0, y2);
  array.SetTexCoords(X0, Y2);
  array.PushVertex();
  
  array.SetVertex(x1, y3);
  array.SetTexCoords(X1, Y3);
  array.PushVertex();
  
  array.SetVertex(x0, y3);
  array.SetTexCoords(X0, Y3);
  array.PushVertex();
  
  // BottomMiddle rect:
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  array.SetVertex(x2, y3);
  array.SetTexCoords(X2, Y3);
  array.PushVertex();
  
  ///
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  array.SetVertex(x2, y3);
  array.SetTexCoords(X2, Y3);
  array.PushVertex();
  
  array.SetVertex(x1, y3);
  array.SetTexCoords(X1, Y3);
  array.PushVertex();
  
  // BottomRight rect:
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  array.SetVertex(x3, y2);
  array.SetTexCoords(X3, Y2);
  array.PushVertex();
  
  array.SetVertex(x3, y3);
  array.SetTexCoords(X3, Y3);
  array.PushVertex();
  
  ///
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  array.SetVertex(x3, y3);
  array.SetTexCoords(X3, Y3);
  array.PushVertex();
  
  array.SetVertex(x2, y3);
  array.SetTexCoords(X2, Y3);
  array.PushVertex();
  
  nuiColor color = mColor;
  if (mUseWidgetAlpha)
  {
    float widgetAlpha = pWidget->GetAlpha();
    color.Alpha() *= widgetAlpha;
    
    //    wprintf(_T("nuiFrame::Draw (0x%x) alpha %.2f => color (%.2f, %.2f, %.2f, %.2f)\n"), this, widgetAlpha,
    //    color.Alpha(), color.Red(), color.Green(), color.Blue());
  }
  
  pContext->EnableTexturing(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(pTexture);
  pContext->SetFillColor(color);
  pContext->DrawArray(array);
}





const nuiRect& nuiFrameSequence::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiFrameSequence::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
}

void nuiFrameSequence::EnableBorder(bool set)
{
  mBorderEnabled = set;
}

bool nuiFrameSequence::IsBorderEnabled() const
{
  return mBorderEnabled;
}

nuiSize nuiFrameSequence::GetBorder(nuiPosition position) const
{
  if (!mBorderEnabled)
    return 0;
  
  nuiSize w = mTexRect.GetWidth(); 
  nuiSize h = mTexRect.GetHeight();

  switch (position)
  {
    case nuiLeft:
      return mClientRect.Left();
      break;
    case nuiRight:
      return w - mClientRect.Right();
      break;
    case nuiTop:
      return mClientRect.Top();
      break;
    case nuiBottom:
      return h - mClientRect.Bottom();
      break;
    case nuiFillHorizontal:
      return w - mClientRect.GetWidth();
      break;
    case nuiFillVertical:
      return h - mClientRect.GetHeight();
      break;
    case nuiNoPosition: break;
    case nuiTopLeft: break;
    case nuiTopRight: break;
    case nuiBottomLeft: break;
    case nuiBottomRight: break;
    case nuiCenter: break;
    case nuiTile: break;
    case nuiFill: break;
    case nuiFillLeft: break;
    case nuiFillRight: break;
    case nuiFillTop: break;
    case nuiFillBottom: break;
  }
  //we should'nt arrive here
  return NULL;
}


const nuiColor& nuiFrameSequence::GetColor() const
{
	return mColor;
}

void nuiFrameSequence::SetColor(const nuiColor& color)
{
	mColor = color;
}

nuiRect nuiFrameSequence::GetIdealClientRect() const
{
  return mClientRect.Size();
}


bool nuiFrameSequence::IsInterpolated()
{
  return mInterpolated;
}

void nuiFrameSequence::SetInterpolated(bool set)
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


