/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglDataObjects.h
\brief Data Object Interface used to associate data to a particular mime type and to share it between ngl application and platform specific procedures and objects for drag and drop
*/

#ifndef __nglDataObjects_h__
#define __nglDataObjects_h__

//#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
#include "nglDataTypesRegistry.h"
#include <map>
#include <list>

#ifdef _WIN32_
#include <string.h>
#include <shlobj.h> // deals with struct DROPFILES definition
#include <shellapi.h>
#endif


class NGL_API nglDataObject
{
public:
  /** @name Life cycle */
  //@{
  nglDataObject(const nglString& rMimeType); ///< Ctor() that asserts the mime type has already been registred in the nglDataTypesRegistry member of nglKernel
  virtual ~nglDataObject();
  //@}

  /** @name Type getters */
  //@{
  const nglString&    GetMimeType();
  /*!<
    \return mime type
  */
  nglNativeObjectType GetNativeType();
  /*!<
    return native object type associated to the mime type
  */
  //@}
#ifdef _WIN32_
  /** @name WIN32 Platform specific methods */
  //@{ 
  virtual void SetFormatType(FORMATETC* pFormat)                          = 0;  ///< Fill rFomatetc structure for this particular format
  virtual void AllocFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)  = 0;  ///< Allocate pMedium fields for requested pFomatetc
  virtual void SetFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)    = 0;  ///< Fill pMedium fields for requested pFomatetc
  virtual void GetFormatData(FORMATETC* pFormat, STGMEDIUM* pMedium)      = 0;  ///< Get data from pMedium for the specfied pFormat type
  //@}
#endif
#ifdef _CARBON_
  /** @name CARBON Platform specific methods */
  //@{
  virtual void AddDragItemFlavor    (DragRef dragRef, DragItemRef& itemRef)                         = 0;
  virtual void SetDragItemFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)  = 0;
  virtual void GetFlavorData        (DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)  = 0;
  //@}
#endif
#ifdef _X11_
  /** @name X11 Platform specific methods */
  //@{ 
//FIXME
  //@}
#endif
protected:
  nglString           mMimeType;
  nglNativeObjectType mNativeType;
};

class NGL_API nglDataFilesObject : public nglDataObject
{
public:
  /** @name Life cycle */
  //@{
  nglDataFilesObject(const nglString& rMimeType);
  virtual ~nglDataFilesObject();
  
  static nglDataObject* Create(const nglString& rMimeType);
  /*!<
    Static creator functor used in the nglDataTypesRegistry member of nglKernel
    See nglDataTypesRegistry.h
    \return a the newly created nglDataFilesObject as a nglDataObject base class pointer
  */
  //@}
  
  virtual void                        AddFile(const nglString& rPath);  ///< Add a File from its path
  virtual const std::list<nglString>& GetFiles();                       ///< return File paths list
  virtual nglSize                     GetDataSize();                    ///< return total size of all file paths list
  
  void SetNumberOfFiles(size_t num);

#ifdef _WIN32_
  virtual void SetFormatType(FORMATETC* pFormat);
  virtual void AllocFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium);
  virtual void SetFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium);
  virtual void GetFormatData(FORMATETC* pFormat, STGMEDIUM* pMedium);
#endif
#ifdef _CARBON_
  std::list<DragItemRef> mItemRefs;
  virtual void AddDragItemFlavor    (DragRef dragRef, DragItemRef& itemRef);
  virtual void SetDragItemFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType);
  virtual void GetFlavorData        (DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType);
#endif
#ifdef _X11_
//FIXME
#endif

protected:
 std::list<nglString> mFilePaths;
  size_t mNumberOfFiles;
};

class NGL_API nglDataTextObject : public nglDataObject
{
public:
  /** @name Life cycle */
  //@{
  nglDataTextObject(const nglString& rMimeType);
  virtual ~nglDataTextObject();

  static nglDataObject*     Create(const nglString& rMimeType);
  /*!<
    Static creator functor used in the nglDataTypesRegistry member of nglKernel
    See nglDataTypesRegistry.h
    \return a the newly created nglDataTextObject as a nglDataObject base class pointer
  */
  //@}
  
  virtual void              SetData(const nglString& rDataText);    ///< Set rDataText to the mDataText
  virtual void              AppendData(const nglString& rDataText); ///< Append text rDataText to present mDataText
  virtual const nglString&  GetData();                              ///< return a reference to mDataText
  virtual nglSize           GetDataSize();                          ///<
  
  void SetTextSize(uint32 size);

#ifdef _WIN32_
  virtual void SetFormatType(FORMATETC* pFormat);
  virtual void AllocFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium);
  virtual void SetFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium);
  virtual void GetFormatData(FORMATETC* pFormat, STGMEDIUM* pMedium);
#endif
#ifdef _CARBON_
  virtual void AddDragItemFlavor    (DragRef dragRef, DragItemRef& itemRef);
  virtual void SetDragItemFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType);
  virtual void GetFlavorData        (DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType);
#endif
#ifdef _X11_
//#FIXME
#endif
protected:
  nglString   mDataText;
  uint32      mTextSize;
};

template <class C>
class NGL_API nglInternalDataObject : public nglDataObject
{

public:
  nglInternalDataObject<C>(const nglString& rMimeType, const C& data)
  : nglDataObject(rMimeType), mData(data)
  {
    nglInternalDataObject::RegisterInternalMimeType(rMimeType);
  }
  
  virtual ~nglInternalDataObject()
  {
  }
  
  void SetData(const C& data)
  {
    mData = data;
  }
  
  const C& GetData() const
  {
    return mData;
  }
  
  static nglDataObject* Create(const nglString& rMimeType)
  {
    return new nglInternalDataObject<C>(rMimeType);
  }
  
  static void RegisterInternalMimeType(const nglString& rMimeType)
  {
    bool res = App->GetDataTypesRegistry().IsTypeRegistered(rMimeType);
    if (!res)
    {
#ifdef _WIN32_
      App->GetDataTypesRegistry().RegisterDataType(rMimeType, CF_PRIVATEFIRST, &nglInternalDataObject<C>::Create);
#endif
      
#ifdef _CARBON_
      App->GetDataTypesRegistry().RegisterDataType(rMimeType, 'nui3', &nglInternalDataObject<C>::Create);
#endif
    }
  }

#ifdef _WIN32_
  virtual void SetFormatType(FORMATETC* pFormat)
  {
    pFormat->cfFormat = CF_PRIVATEFIRST;
    pFormat->tymed = TYMED_HGLOBAL;
    pFormat->ptd = NULL;
    pFormat->dwAspect = DVASPECT_CONTENT;
    pFormat->lindex = -1;
  }

  virtual void AllocFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)
  {
    nglSize size = sizeof(C*);
    pMedium->tymed = TYMED_HGLOBAL;
    pMedium->hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, (SIZE_T)size);
    NGL_ASSERT(pMedium->hGlobal);
    pMedium->pUnkForRelease = NULL; 
  }

  virtual void SetFormatData(FORMATETC * pFormat, STGMEDIUM * pMedium)
  {
    void** dataPtr = (void**)GlobalLock(pMedium->hGlobal);
    *dataPtr = &mData;
    GlobalUnlock(pMedium->hGlobal); 
  }

  virtual void GetFormatData(FORMATETC* pFormat, STGMEDIUM* pMedium)
  {
    void** dataPtr = (void**)GlobalLock(pMedium->hGlobal); 
    mData = (C)*(C*)dataPtr;
    GlobalUnlock(pMedium->hGlobal);
  }
#endif

#ifdef _CARBON_
  virtual void AddDragItemFlavor(DragRef dragRef, DragItemRef& itemRef)
  {
    OSErr err = noErr;
    err = ::AddDragItemFlavor(dragRef, itemRef++, 'nui3', NULL, 0, 0);
    //PrintErr(err);
    NGL_ASSERT(!err);
  }

  virtual void SetDragItemFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)
  {
    ::SetDragItemFlavorData(dragRef, itemRef, flavorType, &mData, sizeof(mData), 0);
  }

  virtual void GetFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)
  {
    OSErr err = noErr;
    Size dataSize;
    err = GetFlavorDataSize (dragRef, itemRef, flavorType, &dataSize);

    NGL_ASSERT(!err);

    //NGL_OUT("ReceiveData: dataSize: %d\n", dataSize);
    err = ::GetFlavorData(dragRef, itemRef, flavorType, &mData, &dataSize, 0);

    NGL_ASSERT(!err);
  }
#endif

#ifdef _X11_
  //#FIXME
#endif
private:
    C mData;

    nglInternalDataObject<C>(const nglString& rMimeType)
      : nglDataObject(rMimeType)
    {
    }
};

#endif // __nglDragAndDropObjects_h__

