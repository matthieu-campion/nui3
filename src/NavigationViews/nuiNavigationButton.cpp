/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nuiNavigationButton.h"



nuiNavigationButton::nuiNavigationButton(nuiBarButtonSystemItem systemItem)
: nuiButton()
{
  
}


nuiNavigationButton::nuiNavigationButton(const nglString& rText)
: nuiButton()
{
  nuiLabel* pLabel = new nuiLabel(rText);
  AddChild(pLabel);
 
}


nuiNavigationButton::nuiNavigationButton(const nglPath& rIconTexture)
: nuiButton()
{
  nuiTexture* pTex = nuiTexture::GetTexture(rIconTexture);
  AddChild(new nuiImage(pTex));
}



// virtual 
nuiNavigationButton::~nuiNavigationButton()
{

}


  
