/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#ifdef HAVE_FREETYPE

#include "nglKernel.h"
#include "nglFontInstance.h"


nglFontInstance::FontListType nglFontInstance::gFontList;


/*
 * Life cycle
 */

nglFontInstance::nglFontInstance (const nglPath& rPath, uint Face)
{
  //NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("nglFontInstance::nglFontInstance 0x%x (%ls - %d)\n"), this, rPath.GetChars(), Face);)
  mPath     = rPath;
  mFace     = Face;
  mpMemBase = NULL;
  mMemSize  = 0;
  mOwnMemory = false;
  mStatic = false;

  Acquire();
}

nglFontInstance::nglFontInstance (const FT_Byte* pBase, FT_Long Size, uint Face, bool StaticBuffer)
{
  //NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("nglFontInstance::nglFontInstance 0x%x (memory %d - %d)\n"), this, Size, Face);)
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

nglFontInstance::nglFontInstance (const nglFontInstance& rInstance)
{
  mPath     = rInstance.mPath;
  mFace     = rInstance.mFace;
  mpMemBase = rInstance.mpMemBase;
  mMemSize  = rInstance.mMemSize;
  //mOwnMemory = rInstance.mOwnMemory;
  mStatic = rInstance.mStatic;
  mOwnMemory = !mStatic;
  
  Acquire();
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("nglFontInstance::nglFontInstance COPY CTOR 0x%x (%ls - %d)\n"), this, mPath.GetChars(), mFace);)
}

nglFontInstance::~nglFontInstance()
{
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("nglFontInstance::~nglFontInstance DTOR 0x%x (%ls - %d)\n"), this, mPath.GetChars(), mFace);)
  if (mOwnMemory)
    delete[] mpMemBase;
}

nglPath nglFontInstance::GetPath() const
{
  return mPath;
}

uint nglFontInstance::GetFace() const
{
  return mFace;
}

FTC_FaceID nglFontInstance::Install(nglFontInstance * pInstance)
{
  nglFontInstance* font;
  
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
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("Install '%ls' %d"), pInstance->mPath.GetPathName().GetChars(), pInstance->mFace); )
  gFontList.insert(pInstance);
  return (FTC_FaceID)pInstance;
}

FTC_FaceID nglFontInstance::Uninstall(nglFontInstance * pInstance)
{
  nglFontInstance* font;

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
        NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("Uninstall '%ls' %d"), p.GetChars(), f); )
        gFontList.erase(it);
      }
      
      return (FTC_FaceID)font;
    }
  }

  return NULL;
}


nglFontInstance* nglFontInstance::Lookup (const FTC_FaceID FaceID)
{
  nglFontInstance* ifont = (nglFontInstance*)FaceID;
#ifdef _DEBUG_
  /* Check that it's really a pointer to a nglFontInstance,
   * parse our instance list for that.
   */
  FontListType::iterator it = gFontList.find(ifont);
  FontListType::iterator end = gFontList.end();
  NGL_ASSERT(it != end);
#endif // _DEBUG_

  return ifont;
}

FT_Error nglFontInstance::FaceRequestHandler (FTC_FaceID FaceID, FT_Library pLibrary, FT_Pointer pData, FT_Face* pFace)
{
  nglFontInstance* ifont = Lookup(FaceID);

  return ifont ? ifont->OnFaceRequest(pLibrary, pData, pFace) : FT_Err_Cannot_Open_Resource;
}

FT_Error nglFontInstance::OnFaceRequest (FT_Library pLibrary, FT_Pointer pData, FT_Face* pFace)
{
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("Hard loading '%ls'"), mPath.GetPathName().GetChars(), mFace); )

  if (!mpMemBase) // Load the file in memory instead of counting on ft2 to understand our pathes...
  {
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


  std::string tmp(mPath.GetPathName().GetStdString());
  return mpMemBase ?
    FT_New_Memory_Face(pLibrary, mpMemBase, mMemSize, mFace, pFace) :
    FT_New_Face(pLibrary, tmp.c_str(), mFace, pFace);

}

void nglFontInstance::Dump()
{
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("nglFontInstance::Dump\n"));)

  FontListType::iterator i;
  nglFontInstance* pFontInstance = NULL;
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
  nglFontInstance::Dump();
}

void nglFontInstance::OnExit()
{
  FontListType::iterator i;
  nglFontInstance* pFontInstance = NULL;
  uint c = 0;
  
  NGL_DEBUG( uint cnt = gFontList.size(); NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Releasing %d installed font%ls"), cnt, PLURAL(cnt)); )

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
