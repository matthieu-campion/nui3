/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nglDragAndDrop::nglDragAndDrop(nglDropEffect defaultDropEffect, nglImage *pFeedbackImage, uint offsetX, uint offsetY)
{
  mpFeedbackImage = pFeedbackImage;
  mOffsetX = offsetX;
  mOffsetY = offsetY;
  mDesiredDropEffect = eDropEffectNone;
  mModifiers = 0;

  AddSupportedDropEffect(defaultDropEffect);
}

nglDragAndDrop::~nglDragAndDrop()
{
  std::map<nglString,nglDataObject*>::iterator end = mObjects.end();
  std::map<nglString,nglDataObject*>::iterator it = mObjects.begin();

  while (it != end)
  {
    delete it->second;
    it++;
  }
}

void nglDragAndDrop::AddType(nglDataObject* pObject)
{
  NGL_ASSERT(pObject);
  mObjects[pObject->GetMimeType()] = pObject;
}

nglDataObject* nglDragAndDrop::GetType(const nglString& rMimeType) const
{
  std::map<nglString, nglDataObject*>::const_iterator it = mObjects.find(rMimeType);
  if (it != mObjects.end() && it->second)
    return it->second;
  return NULL;
}

bool nglDragAndDrop::IsTypeSupported(const nglString& rMimeType) const
{
  std::map<nglString,nglDataObject*>::const_iterator it = mObjects.find(rMimeType);
  if (it != mObjects.end())
    return true;
  return false;
}

const std::list<nglDropEffect>& nglDragAndDrop::GetSupportedDropEffects() const
{ 
  return mSupportedDropEffects; 
}

void nglDragAndDrop::ClearSupportedDropEffects()
{
  mSupportedDropEffects.clear();
}

void nglDragAndDrop::AddSupportedDropEffect(nglDropEffect effect)
{ 
  // first, check existance
  std::list<nglDropEffect>::iterator it;
  for (it = mSupportedDropEffects.begin(); it != mSupportedDropEffects.end(); ++it)
  {
    nglDropEffect ef = *it;
    if (ef == effect)
      return;
  }

  // add
  mSupportedDropEffects.push_back(effect);
}

void nglDragAndDrop::SetDesiredDropEffect(nglDropEffect effect)
{ 
  mDesiredDropEffect = effect;
}

nglDropEffect nglDragAndDrop::GetDesiredDropEffect() const
{ 
  return mDesiredDropEffect; 
}

const std::map<nglString, nglDataObject*>& nglDragAndDrop::GetSupportedTypesMap() const
{ 
  return mObjects;
}

const nglImage *nglDragAndDrop::GetFeedbackImage() const 
{
  return mpFeedbackImage;
}

uint nglDragAndDrop::GetFeedbackImageOffsetX() const
{
  return mOffsetX;
}

uint nglDragAndDrop::GetFeedbackImageOffsetY() const
{
  return mOffsetY;
}

int32 nglDragAndDrop::GetModifiers() const
{
  return mModifiers;
}
