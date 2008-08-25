/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "DecorationManager.h"
#include "nuiGradientDecoration.h"



DecorationManager::DecorationManager()
{
  if (mDecorations.size() == 0)
    Init();
}

DecorationManager::~DecorationManager()
{

}



void DecorationManager::Init()
{
  mDecorations[DECO_TOOLBAR] = new nuiGradientDecoration(DECO_TOOLBAR, nuiRect(5,5,0,0), nuiColor(201,201,201), nuiColor(131,131,131));

  mDecorations[DECO_PANE_TITLE] = new nuiGradientDecoration(DECO_PANE_TITLE, nuiRect(5,5,0,0), nuiColor(91,110,141), nuiColor(34,66,114));
  mDecorations[DECO_PANE_CONTENTS] = new nuiGradientDecoration(DECO_PANE_CONTENTS, nuiRect(5,5,0,0), nuiColor(236,231,231), nuiColor(255,255,255), nuiVertical, 1, nuiColor(204,204,204));
}


nuiDecoration* DecorationManager::Get(const nglString& rName)
{
  nuiDecoration* pDeco = nuiDecoration::Get(rName);
  if (pDeco)
    return pDeco;
    
  return NULL;
}


//static 
std::map<nglString, nuiDecoration*> DecorationManager::mDecorations;
