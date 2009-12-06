/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLImage.h"
#include "nuiHTTP.h"

/////////////
//class nuiHTMLImage : public nuiHTMLItem
nuiHTMLImage::nuiHTMLImage(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor)
: nuiHTMLItem(pNode, pAnchor, true), mpTexture(NULL), mWidth(8), mHeight(8)
{
  const nuiHTMLAttrib* pSrc = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_SRC);
  const nuiHTMLAttrib* pAlt = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_ALT);
  
  if (!pSrc || !pAlt)
    return;
  
  nglString url = pSrc->GetValue();
  printf("incomming url: %ls\n", url.GetChars());
  url = GetAbsoluteURL(url);
  
  printf("url: %ls\n", url.GetChars());
  
  // First look up the cache:
  mpTexture = nuiTexture::GetTexture(nglString(url));
  if (!mpTexture)
  {
    mpTexture = nuiTexture::GetTexture(url, NULL);
    if (!mpTexture->IsValid())
    {
      mpTexture->Release();
      mpTexture = NULL;
    
      nuiHTTPRequest* pRequest = new nuiHTTPRequest(url);
      pRequest->SendRequest(nuiMakeDelegate(this, &nuiHTMLImage::HTTPDone));
      return;
    }
  }
  
  mpTexture->SetSource(url);
  
  mWidth = mpTexture->GetWidth();
  mHeight = mpTexture->GetHeight();
}

nuiHTMLImage::~nuiHTMLImage()
{
  
}

void nuiHTMLImage::HTTPDone(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse)
{
  if (!pResponse)
    return;
  
  nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
  mpTexture = nuiTexture::GetTexture(&mem);
  
  if (!mpTexture)
    return;
  
  if (!mpTexture->IsValid())
  {
    mpTexture->Release();
    mpTexture = NULL;
    return;
  }
  
  mpTexture->SetSource(pRequest->GetURL());
  
  mWidth = mpTexture->GetWidth();
  mHeight = mpTexture->GetHeight();
  
  InvalidateLayout();
}

void nuiHTMLImage::Draw(nuiDrawContext* pContext)
{
  if (!mpTexture || !mpTexture->IsValid())
    return;
  
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
}
