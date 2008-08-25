/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*!
\file  nglOleDragAndDropInterfaces.h
\brief WIN32 Platform specific procedures and com objects for drag and drop operations
See also nglWindow.cpp for various implementations
*/

#ifndef __nglOleDragAndDropInterfaces_h__
#define __nglOleDragAndDropInterfaces_h__


//#include "nui.h"
class nglWindow;



bool IsIidFromList(REFIID riid, const IID* InterfaceIds, uint32 interfaceCount);


class nglEnumFormat;
class nglIDataObject;
class nglDropTarget;
class nglDropSource;

#define NGL_GIVE_FEEDBACK_MESSAGE   ((UINT)1)
#define NGL_GET_DATA_MESSAGE        ((UINT)2)
#define NGL_STOP_DRAGGING           ((UINT)3)


// IUnknown Implementations
#define   IMPLEMENT_IUNKNOWN_METHODS(classname)                               \
  HRESULT STDMETHODCALLTYPE classname::QueryInterface(REFIID riid, void **ppv)\
  {                                                                           \
    if ( IsIidFromList(riid, mInterfaceIds, mInterfaceCount) ) {              \
      *ppv = this;                                                            \
      AddRef();                                                               \
      return S_OK;                                                            \
    }                                                                         \
    else {                                                                    \
      *ppv = NULL;                                                            \
      return (HRESULT) E_NOINTERFACE;                                         \
    }                                                                         \
  }                                                                           \
                                                                              \
  ULONG STDMETHODCALLTYPE classname::AddRef()                                 \
  {                                                                           \
    return ++mRef;                                                            \
  }                                                                           \
                                                                              \
  ULONG STDMETHODCALLTYPE classname::Release()                                \
  {                                                                           \
    if (--mRef == 0) {                                                        \
      delete this;                                                            \
      return 0;                                                               \
    }                                                                         \
    else                                                                      \
      return mRef;                                                            \
  }
// endof IUnknown Implementations



void SetDropEffect(nglDropEffect dropEffect, DWORD* pEffect);
nglDropEffect GetDropEffect(DWORD* pEffect);
void GetDropEffects(nglDragAndDrop* pObj, DWORD* pEffect);

// IEnumFORMATETC Impl
class nglEnumFormat : public IEnumFORMATETC
{
public:
  nglEnumFormat();
  virtual ~nglEnumFormat();

  IMPLEMENT_IUNKNOWN_METHODS(nglEnumFormat);

  void CreateFormatEtcArray(nglDragAndDrop* pDraggedObject);
  void Clear();

  // From IEnumFORMATEC
  HRESULT STDMETHODCALLTYPE Next(ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
  HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
  HRESULT STDMETHODCALLTYPE Reset(void);
  HRESULT STDMETHODCALLTYPE Clone(IEnumFORMATETC ** ppenum);

protected:
  ULONG mRef;
  IID mInterfaceIds[2];
  uint32 mInterfaceCount;
 
  std::list<FORMATETC> mFormatEtc;
  std::list<FORMATETC>::iterator mFormatIterator;
};


class nglIDataObject : public IDataObject
{
public:
  nglIDataObject(nglWindow* pWindow);
  virtual ~nglIDataObject();
  
  IMPLEMENT_IUNKNOWN_METHODS(nglIDataObject);
  
  void Clear();
  void InitObject(nglDropSource*);
  void SetDraggedObject(nglDragAndDrop* pDraggedObject);
  
  // From IDataObject
  HRESULT STDMETHODCALLTYPE GetData(FORMATETC * pFormatetc, ///< in  Pointer to the FORMATETC structure
                                    STGMEDIUM * pmedium);   ///< out Pointer to the STGMEDIUM structure
  HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC * pFormatetc,  ///< Pointer to the FORMATETC structure
                                        STGMEDIUM * pmedium);    ///< Pointer to the STGMEDIUM structure
  HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC * pFormatetc);  ///< Pointer to the FORMATETC structure
  HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC * pFormatetcIn,  //Pointer to the FORMATETC structure
                                                  FORMATETC * pFormatetcOut);//Pointer to the canonical equivalent // FORMATETC structure
  HRESULT STDMETHODCALLTYPE SetData(FORMATETC * pFormatetc, //Pointer to the FORMATETC structure
                                    STGMEDIUM * pmedium,    //Pointer to STGMEDIUM structure
                                    BOOL fRelease);         //Indicates which object owns the storage // medium after the call is completed
  HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection,                  //Specifies a value from the enumeration // DATADIR
                                          IEnumFORMATETC ** ppenumFormatetc); //Address of output variable that receives the // IEnumFORMATETC interface pointer
  HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC * pFormatetc,  //Pointer to data of interest to the // advise sink
                                    DWORD advf,              //Flags that specify how the notification // takes place
                                    IAdviseSink * pAdvSink,  //Pointer to the advise sink
                                    DWORD * pdwConnection);  //Pointer to a token that identifies this // connection
  HRESULT STDMETHODCALLTYPE DUnadvise(DWORD dwConnection);   //Connection to remove
  HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA ** ppenumAdvise);  //Address of output variable that // receives the IEnumSTATDATA // interface pointer

protected:
  ULONG mRef;
  IID mInterfaceIds[2];
  uint32 mInterfaceCount;

  HWND            mHWnd;
  UINT            mMessageId;
  nglWindow*      mpWindow;
  nglDragAndDrop* mpDraggedObject;
  nglEnumFormat*  mpFormatEtc;
};



class nglDropSource : public IDropSource
{
public:
  nglDropSource(nglWindow* pWindow);
  virtual ~nglDropSource();

  IMPLEMENT_IUNKNOWN_METHODS(nglDropSource);
  
  friend class nglIDataObject;

  void SetDraggedObject(nglDragAndDrop* pDraggedObject);
  nglDragAndDrop* GetDraggedObject() const;

  void SetMessageId(UINT MessageId);
  UINT GetMessageId() const;

  void SetWindowHandle(HWND HWnd);
  HWND GetWindowHandle() const;

  bool Drag();

  void ClearObjects();

  // From IDropSource:
  HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect);  //Effect of a drop operation
  HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed,  //Status of escape key since previous call
                            DWORD grfKeyState     //Current state of keyboard modifier keys
                            );
  
protected:
  ULONG mRef;
  IID mInterfaceIds[2];
  uint32 mInterfaceCount;

  HWND            mHWnd;
  nglWindow*      mpWindow;
  nglDragAndDrop* mpDraggedObject;
  nglIDataObject* mpIDataObject;
  DWORD           mInitialButton;
  UINT            mMessageId;

  int             mX, mY;
  nglWindow::Flags mButton;
};

class nglDropTarget : public IDropTarget
{
public:
  nglDropTarget(HWND pHwnd, nglWindow* pWindow);
  virtual ~nglDropTarget();

  IMPLEMENT_IUNKNOWN_METHODS(nglDropTarget);

  bool InitOleDrop();
  bool UnInitOleDrop();

  HRESULT nglDropTarget::GetObject(IDataObject* pDataObj, bool getData);

  // From IDropTarget:
  HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
  HRESULT STDMETHODCALLTYPE DragLeave(void);
  HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
  HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

protected:
  ULONG mRef;
  IID mInterfaceIds[2];
  uint32 mInterfaceCount;
  
  HWND       mpHwnd;
  nglWindow* mpWindow;
  nglDragAndDrop* mpObject;
};

#endif//__nglOleDragAndDropInterfaces_h__

