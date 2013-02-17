/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


nuiImageAnimation::nuiImageAnimation(uint32 nbFrames, const nglPath& rTexturePath, bool framesInSingleFile, nuiOrientation orientation)
: nuiWidget()
{
  mpSequence = new nuiImageSequence(nbFrames, rTexturePath, framesInSingleFile, orientation);
}

nuiImageAnimation::~nuiImageAnimation()
{
  delete mpSequence;
  mpSequence = NULL;
}
  

uint32 nuiImageAnimation::GetFrameIndex() const
{
  return mpSequence->GetFrameIndex();
}


void nuiImageAnimation::SetFrameIndex(uint32 index)
{
  if (!mpSequence)
    return;
  mpSequence->SetFrameIndex(index);
}


nuiRect nuiImageAnimation::CalcIdealSize()
{
  if (!mpSequence)
    return nuiRect(0,0,0,0);
  return mpSequence->CalcIdealSize();
}




bool nuiImageAnimation::Draw(nuiDrawContext* pContext)
{
  if (!mpSequence)
    return false;
  bool res = nuiWidget::Draw(pContext);
  mpSequence->Draw(pContext);
  return res;
}





