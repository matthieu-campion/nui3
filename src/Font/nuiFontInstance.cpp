/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H
#include "nuiFontInstance.h"

#ifdef HAVE_FREETYPE

nuiFontInstance::FontListType nuiFontInstance::gFontList;


/*
 * Life cycle
 */

nuiFontInstance::nuiFontInstance (const nglPath& rPath, uint Face)
{
  NGL_DEBUG( NGL_LOG("font", NGL_LOG_DEBUG, "nuiFontInstance::nuiFontInstance 0x%x (%s - %d)\n", this, rPath.GetChars(), Face);)
  mPath     = rPath;
  mFace     = Face;
  mpMemBase = NULL;
  mMemSize  = 0;
  mOwnMemory = false;
  mStatic = false;

  Acquire();
}

nuiFontInstance::nuiFontInstance (const FT_Byte* pBase, FT_Long Size, uint Face, bool StaticBuffer)
{
  //NGL_DEBUG( NGL_LOG("font", NGL_LOG_DEBUG, "nuiFontInstance::nuiFontInstance 0x%x (memory %d - %d)\n", this, Size, Face);)
  nglString fake;
  fake.Format(_T("/memory/%p,0x%x"), pBase, Size);

  mPath     = fake;
  mFace     = Face;
  mpMemBase = pBase;
  mMemSize  = Size;
  mOwnMemory = false;
  mStatic   = StaticBuffer;
  
  Acquire();
}

nuiFontInstance::nuiFontInstance (const nuiFontInstance& rInstance)
{
  mPath     = rInstance.mPath;
  mFace     = rInstance.mFace;
  mpMemBase = rInstance.mpMemBase;
  mMemSize  = rInstance.mMemSize;
  //mOwnMemory = rInstance.mOwnMemory;
  mStatic = rInstance.mStatic;
  mOwnMemory = !mStatic;
  
  Acquire();
  //NGL_DEBUG( NGL_LOG("font", NGL_LOG_DEBUG, "nuiFontInstance::nuiFontInstance COPY CTOR 0x%x (%s - %d)\n", this, mPath.GetChars(), mFace);)
}

nuiFontInstance::~nuiFontInstance()
{
  //NGL_DEBUG( NGL_LOG("font", NGL_LOG_DEBUG, "nuiFontInstance::~nuiFontInstance DTOR 0x%x (%s - %d)\n", this, mPath.GetChars(), mFace);)
  if (mOwnMemory)
    delete[] mpMemBase;
}

nglPath nuiFontInstance::GetPath() const
{
  return mPath;
}

uint nuiFontInstance::GetFace() const
{
  return mFace;
}

FTC_FaceID nuiFontInstance::Install(nuiFontInstance * pInstance)
{
  nuiFontInstance* font;
  
  FontListType::iterator it = gFontList.begin();
  FontListType::iterator end = gFontList.end();
  for (; it != end; ++it)
  {
    font = *it;
    if ((pInstance->mPath == font->mPath) && (pInstance->mFace == font->mFace))
    {
      font->Acquire();
      return (FTC_FaceID)font;
    }
  }

  // Not matching installed font : create (copy), install and return it
  pInstance->Acquire();
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("Install '%s' %d"), pInstance->mPath.GetPathName().GetChars(), pInstance->mFace); )
  gFontList.insert(pInstance);
  return (FTC_FaceID)pInstance;
}

FTC_FaceID nuiFontInstance::Uninstall(nuiFontInstance * pInstance)
{
  nuiFontInstance* font;

  FontListType::iterator it = gFontList.begin();
  FontListType::iterator end = gFontList.end();
  for (; it != end; ++it)
  {
    font = *it;
    if ((pInstance->mPath == font->mPath) && (pInstance->mFace == font->mFace))
    {
      nglPath p(font->mPath.GetPathName());
      int32 f = font->mFace;
      if (!font->Release())
      {
        NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("Uninstall '%s' %d"), p.GetChars(), f); )
        gFontList.erase(it);
      }
      
      return (FTC_FaceID)font;
    }
  }

  return NULL;
}


nuiFontInstance* nuiFontInstance::Lookup (const FTC_FaceID FaceID)
{
  nuiFontInstance* ifont = (nuiFontInstance*)FaceID;
#ifdef _DEBUG_
  /* Check that it's really a pointer to a nuiFontInstance,
   * parse our instance list for that.
   */
  FontListType::iterator it = gFontList.find(ifont);
  FontListType::iterator end = gFontList.end();
  NGL_ASSERT(it != end);
#endif // _DEBUG_

  return ifont;
}

FT_Error nuiFontInstance::FaceRequestHandler (FTC_FaceID FaceID, FT_Library pLibrary, FT_Pointer pData, FT_Face* pFace)
{
  nuiFontInstance* ifont = Lookup(FaceID);

  return ifont ? ifont->OnFaceRequest(pLibrary, pData, pFace) : FT_Err_Cannot_Open_Resource;
}

FT_Error nuiFontInstance::OnFaceRequest (FT_Library pLibrary, FT_Pointer pData, FT_Face* pFace)
{
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("Hard loading '%s'"), mPath.GetPathName().GetChars(), mFace); )

  if (!mpMemBase) // We were given a file path
  {
    // Try to load the file directly with freetype:
    std::string tmp(mPath.GetPathName().GetStdString());
    FT_Error error = FT_New_Face(pLibrary, tmp.c_str(), mFace, pFace);
    if (!error) // good!
      return 0;

    // Freetype couldn't load the file, it may be from our VFS, so try to load it in memory first:
    nglIStream* pFile = mPath.OpenRead();
    if (!pFile || pFile->GetState() != eStreamReady)
    {
      delete pFile;
      return FT_Err_Invalid_Argument;
    }
    
    mMemSize = (FT_Long)pFile->Available();
    FT_Byte* pMem = new FT_Byte[mMemSize];
    NGL_ASSERT(pMem != NULL);
    mpMemBase = pMem;
    mOwnMemory = true;
    if (pFile->ReadUInt8(pMem, mMemSize) != mMemSize)
    {
      delete pFile;
      return FT_Err_Invalid_Argument;
    }
    
    delete pFile;
  }

  // Load the font from memory
  return FT_New_Memory_Face(pLibrary, mpMemBase, mMemSize, mFace, pFace);
}

void nuiFontInstance::Dump()
{
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("nuiFontInstance::Dump\n"));)

  FontListType::iterator i;
  nuiFontInstance* pFontInstance = NULL;
  uint s = gFontList.size();
  uint c = 0;
  
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Dumping font instances (%d)\n"), s);)
    
  for (i = gFontList.begin(); i != gFontList.end(); ++i)
  {
    pFontInstance = *i;
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("installed font instance %d/%d: 0x%x\n"), c, s, pFontInstance);)
    c++;
  }
}

void nglDumpFontInstances()
{
  nuiFontInstance::Dump();
}

void nuiFontInstance::OnExit()
{
  FontListType::iterator i;
  nuiFontInstance* pFontInstance = NULL;
  uint c = 0;
  
  NGL_DEBUG( uint cnt = gFontList.size(); NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Releasing %d installed font%s"), cnt, PLURAL(cnt)); )

  for (i = gFontList.begin(); i != gFontList.end(); ++i)
  {
    pFontInstance = *i;
    pFontInstance->Release();
//     if (pFontInstance)
//       delete pFontInstance;
    c++;
  }

  gFontList.clear();
}

#endif // HAVE_FREETYPE
