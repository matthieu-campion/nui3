/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLHeader.h"

// class nuiHTMLHeader : public nuiHTMLBox
nuiHTMLHeader::nuiHTMLHeader(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor)
: nuiHTMLBox(pNode, pAnchor, false)
{
}

nuiHTMLHeader::~nuiHTMLHeader()
{
}

void nuiHTMLHeader::Layout(nuiHTMLContext& rContext)
{
  PushContext(rContext);

  float s = 12;
  switch (mpNode->GetTagType())
  {
  case nuiHTML::eTag_H1:
    s = 20;
    break;
  case nuiHTML::eTag_H2:
    s = 18;
    break;
  case nuiHTML::eTag_H3:
    s = 16;
    break;
  case nuiHTML::eTag_H4:
    s = 14;
    break;
  case nuiHTML::eTag_H5:
    s = 13;
    break;
  case nuiHTML::eTag_H6:
    s = 12;
    break;
  }
  
  rContext.mFont.ClearMustHaveSize();
  rContext.mFont.MustHaveSize(s, 1.0f);
  
  nuiHTMLBox::Layout(rContext);
  
  PopContext(rContext);
}

