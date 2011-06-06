/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiFontInstancenuiFontInstance.h
\brief Font instance handler for FreeType cache manager
*/

#pragma once

//#include "nui.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_IMAGE_H


//! Font instance handler for FreeType cache manager
/*!
See src/font/README for more details.
*/
class nuiFontInstance : public nuiRefCount
{
public:
  nuiFontInstance(const nglPath& rPath, uint Face);
  nuiFontInstance(const FT_Byte* pBase, FT_Long Size, uint Face, bool StaticBuffer);
  nuiFontInstance(const nuiFontInstance& rInstance);
  ~nuiFontInstance();
  
  nglPath  GetPath() const;
  uint     GetFace() const;

  static FTC_FaceID       Install(nuiFontInstance * pInstance);
  static FTC_FaceID       Uninstall(nuiFontInstance * pInstance);
  static nuiFontInstance* Lookup(const FTC_FaceID FaceID);
  static FT_Error         FaceRequestHandler (FTC_FaceID FaceID, FT_Library pLibrary, FT_Pointer pData, FT_Face* pFace);
  static void             OnExit();
  static void             Dump();
  friend void nglDumpFontInstances();

private:
  typedef std::set<nuiFontInstance*> FontListType;

  static FontListType gFontList;

  nglPath        mPath;
  uint           mFace;
  const FT_Byte* mpMemBase;
  FT_Long        mMemSize;

  FT_Error OnFaceRequest (FT_Library pLibrary, FT_Pointer pData, FT_Face* pFace);
  bool mOwnMemory;
  bool mStatic;
};

