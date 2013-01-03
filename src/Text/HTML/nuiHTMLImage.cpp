/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLImage.h"
#include "nuiHTTP.h"
#include "nuiAsyncIStream.h"

/////////////
//class nuiHTMLImage : public nuiHTMLItem
nuiHTMLImage::nuiHTMLImage(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor)
: nuiHTMLItem(pNode, pAnchor, true), mpTexture(NULL), mWidth(8), mHeight(8), mpStream(NULL)
{
  const nuiHTMLAttrib* pSrc = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_SRC);
  const nuiHTMLAttrib* pAlt = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_ALT);
  
  if (!pSrc)
    return;
  
  nglString url = pSrc->GetValue();
  //printf("incomming url: %s\n", url.GetChars());
  url = GetAbsoluteURL(url);
  
  //printf("url: %s\n", url.GetChars());
  
  // First look up the cache:
  mpTexture = nuiTexture::GetTexture(nglString(url));
  if (!mpTexture)
  {
    mpTexture = nuiTexture::GetTexture(url, NULL);
    if (!mpTexture->IsValid())
    {
      mpTexture->Release();
      mpTexture = NULL;
    
      mpStream = new nuiAsyncIStream(url, true, nuiMakeDelegate(this, &nuiHTMLImage::StreamDone));
      return;
    }
  }
  
  mpTexture->SetSource(url);
  
  mWidth = mpTexture->GetWidth();
  mHeight = mpTexture->GetHeight();
}

nuiHTMLImage::~nuiHTMLImage()
{
  if (mpStream)
    mpStream->Cancel();
}

void nuiHTMLImage::StreamDone(nuiAsyncIStream* pStream)
{
  NGL_ASSERT(pStream == mpStream);
  mpStream = NULL;
  const nuiHTTPResponse* pResponse = pStream->GetHTTPResponse();

  mpTexture = nuiTexture::GetTexture(pStream);
  
  if (!mpTexture)
    return;
  
  if (!mpTexture->IsValid())
  {
    mpTexture->Release();
    mpTexture = NULL;
    return;
  }
  
  mpTexture->SetSource(pStream->GetURL());
  
  mWidth = mpTexture->GetWidth();
  mHeight = mpTexture->GetHeight();
  
  InvalidateLayout();
}

void nuiHTMLImage::Draw(nuiDrawContext* pContext)
{
  if (!mVisible)
    return;
  if (!mpTexture || !mpTexture->IsValid())
    return;
  
  //NGL_OUT(_T("nuiHTMLImage::Draw %s\n"), mpTexture->GetObjectName().GetChars());
  pContext->PushState();
  pContext->SetTexture(mpTexture);  
  pContext->SetFillColor(nuiColor(255, 255, 255));
  pContext->DrawImage(mIdealRect.Size(), nuiRect((float)mpTexture->GetWidth(), (float)mpTexture->GetHeight()));
  pContext->PopState();
}

void nuiHTMLImage::Layout(nuiHTMLContext& rContext)
{    
  if (!mpTexture)
    return;
  mIdealRect.Set(0.0f, 0.0f, mWidth, mHeight);
  mIdealRect.RoundToBiggest();
}
