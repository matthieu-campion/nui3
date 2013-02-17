/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


nuiArchive::nuiArchive(bool Loading)
{
  mLoading = Loading;
}

nuiArchive::~nuiArchive()
{
}

bool nuiArchive::IsLoading() const
{
  return mLoading;
}

bool nuiArchive::IsSaving() const
{
  return !IsLoading();
}

