/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
#include "nglDataObjects.h"
#include <map>
#include <list>


void nglDataFilesObject::AddFile(const nglString& rPath)
{
  mFilePaths.push_back(rPath);
}
/*
 * nglDataFilesObject
 */
void nglDataFilesObject::SetFormatType(FORMATETC* pFormat)
{
  pFormat->cfFormat = CF_HDROP;
  pFormat->tymed = TYMED_HGLOBAL;
  pFormat->ptd = NULL;
  pFormat->dwAspect = DVASPECT_CONTENT;
  pFormat->lindex = -1;
}

void nglDataFilesObject::AllocFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)
{
  nglSize size = sizeof(DROPFILES) + (1*sizeof(nglChar));
  size += GetDataSize();
  pMedium->tymed = TYMED_HGLOBAL;
  pMedium->hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, (SIZE_T)size);
  NGL_ASSERT(pMedium->hGlobal);
  pMedium->pUnkForRelease = NULL;
}

void nglDataFilesObject::SetFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)
{
  void* pData = GlobalLock(pMedium->hGlobal);
  
  LPDROPFILES pDrop = (LPDROPFILES)pData;
  // initialize DROPFILES struct
  pDrop->pFiles = sizeof(DROPFILES);
  pDrop->fNC = FALSE;
  /*//#FIXME someday
  #if nglUSE_UNICODE
      pDrop->fWide = TRUE;
  #else // ANSI
  */
  pDrop->fWide = TRUE;
  //#endif // Unicode/Ansi
  
  // set start of filenames list (null separated)
  nglChar* pbuf = (nglChar*) ((BYTE *)pDrop + sizeof(DROPFILES));

  std::list<nglString>& files = mFilePaths; //pFiles->GetFiles();
  std::list<nglString>::iterator end = files.end();
  for (std::list<nglString>::iterator i = files.begin(); i != end; i++)
  {
    nglString path(*i);
    path.Replace('/', '\\');
    size_t len = path.GetLength();
    memcpy(pbuf, path.GetChars(), len*sizeof(nglChar));
    pbuf += len;
    *pbuf++ = '\0';
  }
  // add final null terminator
  *pbuf = '\0'; 
  GlobalUnlock(pMedium->hGlobal);
}

void nglDataFilesObject::GetFormatData(FORMATETC* pFormat, STGMEDIUM* pMedium)
{
  uint count = DragQueryFile((HDROP)pMedium->hGlobal, 0xffffffff, NULL, 0);
  uint counter = 0;
  
  while (count--)
  {
    nglChar PathName[1024];
    DragQueryFile((HDROP)pMedium->hGlobal, counter, PathName, 1024);
    mFilePaths.push_back(nglString(PathName));
    counter++;
  }
  GlobalUnlock(pMedium->hGlobal);
}

/*
 * nglDataTextObject
 */
void nglDataTextObject::SetFormatType(FORMATETC* pFormat)
{
  pFormat->cfFormat = CF_TEXT;
  pFormat->tymed = TYMED_HGLOBAL;
  pFormat->ptd = NULL;
  pFormat->dwAspect = DVASPECT_CONTENT;
  pFormat->lindex = -1;
}

void nglDataTextObject::AllocFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)
{
  nglSize size = GetDataSize();
  pMedium->tymed = TYMED_HGLOBAL;
  pMedium->hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, (SIZE_T)size);
  NGL_ASSERT(pMedium->hGlobal);
  pMedium->pUnkForRelease = NULL; 
}

void nglDataTextObject::SetFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)
{
  nglChar* lpstr = (nglChar*)GlobalLock(pMedium->hGlobal);
  strcpy(lpstr, mDataText.GetChars());
  GlobalUnlock(pMedium->hGlobal); 
}

void nglDataTextObject::GetFormatData(FORMATETC* pFormat, STGMEDIUM* pMedium)
{
  nglString s;
  nglChar* lptstr = (nglChar*)GlobalLock(pMedium->hGlobal); 
  if (lptstr != NULL) 
  { 
  //NGL_OUT(_T("String: %s\n"),lptstr); 
    s = nglString(lptstr);
  }
  GlobalUnlock(pMedium->hGlobal);
  mDataText = s;
}
