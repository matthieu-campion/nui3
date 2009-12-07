/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTMLItem.h"

class nuiHTTPRequest;
class nuiHTTPResponse;
class nuiHTTPRequest_Thread;

class nuiHTMLImage : public nuiHTMLItem
{
public:
  nuiHTMLImage(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor);
  virtual ~nuiHTMLImage();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
private:
  void HTTPDone(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse);
  
  nuiTexture* mpTexture;
  nuiHTTPRequest_Thread* mpRequest;
  float mWidth;
  float mHeight;
};


