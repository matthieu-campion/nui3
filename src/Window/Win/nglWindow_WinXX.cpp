/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglWindow.h"
#include "ngl_winXX.h"
#include "nuiMainMenu.h"
#include <string.h>
#include <set>
#include "nui_GL/wglext.h"

#define HAVE_NV_PERFKIT 0
#define NGL_ENABLE_FBO_COPYONSWAP 1

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL                  0x020E
#endif

#ifndef WM_TOUCH
#define WM_TOUCH 0x0240
#endif

#if HAVE_NV_PERFKIT
#include "nv_perfauth.h"
#endif


#define NGL_WINDOW_EBASE      (NGL_CONTEXT_ELAST+1)

/*
#define NGL_WINDOW_ENONE      0 (in nglWindow.h)
*/
#define NGL_WINDOW_EWIN       (NGL_WINDOW_EBASE+1)
#define NGL_WINDOW_EVIDMODE   (NGL_WINDOW_EBASE+2)
#define NGL_WINDOW_ESETFLAG   (NGL_WINDOW_EBASE+3)
#define NGL_WINDOW_ENOWIN     (NGL_WINDOW_EBASE+4)
#define NGL_WINDOW_ERANGE     (NGL_WINDOW_EBASE+5)

const nglChar* gpWindowErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("System window creation failed"),
/*  2 */ _T("Full screen video mode unavailable"),
/*  3 */ _T("Can only set a flag at once"),
/*  4 */ _T("Invalid window"),
/*  5 */ _T("Parameter ouf of range"),
  NULL
};

#define MENU_SHOWCONSOLE_ID 42

static uint32 MakePOT(uint32 i)
{
  uint32 t = 1;
  while (t < i)
    t *= 2;
  return t;
}



class nglPBuffer
{
public:
  nglPBuffer()
  {
    wglGetExtensionsStringARB = NULL;

    // WGL_ARB_pbuffer
    wglCreatePbufferARB = NULL;
    wglGetPbufferDCARB = NULL;
    wglReleasePbufferDCARB = NULL;
    wglDestroyPbufferARB = NULL;
    wglQueryPbufferARB = NULL;

    // WGL_ARB_pixel_format
    wglChoosePixelFormatARB = NULL;

    // WGL_ARB_render_texture
    wglBindTexImageARB = NULL;
    wglReleaseTexImageARB = NULL;
    wglSetPbufferAttribARB = NULL;

    mPBuffer = NULL;
    mHDC = NULL;
    mHRC = NULL;
    mWidth = 0;
    mHeight = 0;
    mTexture = -1;
    mUseRenderTexture = false;


    mHWnd = NULL;
  }

  bool Create(HDC hdc, HGLRC RC, HWND hWnd, int32 width, int32 height)
  {
    mHWnd = hWnd;
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	  char *ext = NULL;
	
	  if (wglGetExtensionsStringARB)
		  ext = (char*)wglGetExtensionsStringARB(wglGetCurrentDC());
	  else
		  return false;

	  //
	  // WGL_ARB_pbuffer
	  //

	  if (strstr(ext, "WGL_ARB_pbuffer") == NULL)
		  return false;
	  else
	  {
		  wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
		  wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
		  wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
		  wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
		  wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");

		  if (!wglCreatePbufferARB || !wglGetPbufferDCARB || !wglReleasePbufferDCARB || !wglDestroyPbufferARB || !wglQueryPbufferARB)
  		  return false;
	  }

	  //
	  // WGL_ARB_pixel_format
	  //

	  if (strstr(ext, "WGL_ARB_pixel_format") == NULL)
		  return false;
	  else
	  {
		  wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		  if (!wglChoosePixelFormatARB)
        return false;
	  }

	  //
	  // WGL_ARB_render_texture
	  //

	  if (strstr( ext, "WGL_ARB_render_texture" ) != NULL)
	  {
#if 0
      mUseRenderTexture = false;
#else 
      mUseRenderTexture = true;
            
		  wglBindTexImageARB     = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
		  wglReleaseTexImageARB  = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
		  wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC)wglGetProcAddress("wglSetPbufferAttribARB");

		  if (!wglBindTexImageARB || !wglReleaseTexImageARB || !wglSetPbufferAttribARB)
        return false;
#endif
	  }

	  //-------------------------------------------------------------------------
	  // Create a p-buffer for off-screen rendering.
	  //-------------------------------------------------------------------------

	  mPBuffer = NULL;
	  mWidth = width;
	  mHeight = height;

    mPOTWidth = MakePOT(width);
	  mPOTHeight = MakePOT(height);


	  //
	  // Define the minimum pixel format requirements we will need for our 
	  // p-buffer. A p-buffer is just like a frame buffer, it can have a depth 
	  // buffer associated with it and it can be double buffered.
	  //

	  int32 pf_attr[] =
	  {
		  WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		  WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
		  WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE, // P-buffer will be used as a texture
		  WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
		  WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
		  WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
		  //WGL_ALPHA_BITS_ARB, 0,              // At least 8 bits for ALPHA channel
		  //WGL_DEPTH_BITS_ARB, 0,             // At least 16 bits for depth buffer
		  WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
      WGL_STENCIL_BITS_ARB, 8,            // Ask for a stencil buffer
		  0                                   // Zero terminates the list
	  };

	  int32 pf_attr2[] =
	  {
		  WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		  WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
		  WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
		  WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
		  WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
		  //WGL_ALPHA_BITS_ARB, 0,              // At least 8 bits for ALPHA channel
		  //WGL_DEPTH_BITS_ARB, 0,             // At least 16 bits for depth buffer
		  WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
      WGL_STENCIL_BITS_ARB, 8,            // Ask for a stencil buffer
		  0                                   // Zero terminates the list
	  };
	  uint32 count = 0;
	  int32 pixelFormat;
	  if (mUseRenderTexture)
      wglChoosePixelFormatARB(hdc,(const int*)pf_attr, NULL, 1, &pixelFormat, &count);
    else
	    wglChoosePixelFormatARB(hdc,(const int*)pf_attr2, NULL, 1, &pixelFormat, &count);

	  if (count == 0)
      return false;

	  //
	  // Set some p-buffer attributes so that we can use this p-buffer as a
	  // 2D RGBA texture target.
	  //

    int32 pb_attr[] =
    {
      WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB, // Our p-buffer will have a texture format of RGBA
      WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,   // Of texture target will be GL_TEXTURE_2D
      0                                             // Zero terminates the list
    };

    int32 pb_attr2[] =
    {
      0                                             // Zero terminates the list
    };

    //
	  // Create the p-buffer...
	  //

    if (mUseRenderTexture)
      mPBuffer = wglCreatePbufferARB(hdc, pixelFormat, mWidth, mHeight, pb_attr);
    else
      mPBuffer = wglCreatePbufferARB(hdc, pixelFormat, mWidth, mHeight, pb_attr2);

	  mHDC = wglGetPbufferDCARB(mPBuffer);
	  mHRC = wglCreateContext(mHDC);

	  if (!mPBuffer)
      return false;

	  int h;
	  int w;
	  wglQueryPbufferARB(mPBuffer, WGL_PBUFFER_WIDTH_ARB, &w);
	  wglQueryPbufferARB(mPBuffer, WGL_PBUFFER_HEIGHT_ARB, &h);

	  if (h != mHeight || w != mWidth)
      return false;

    if( !wglShareLists( RC, mHRC ) )
      return false;
      

	  //
	  // We were successful in creating a p-buffer. We can now make its context 
	  // current and set it up just like we would a regular context 
	  // attached to a window.
	  //

	  if (!wglMakeCurrent(mHDC, mHRC))
	  {
      return false;
	  }

    return true;
  }

  ~nglPBuffer()
  {
	  //
	  // Don't forget to clean up after our p-buffer...
	  //

	  if (mHRC != NULL)
	  {
		  wglMakeCurrent(mHDC, mHRC);
		  wglDeleteContext(mHRC);
		  wglReleasePbufferDCARB(mPBuffer, mHDC);
		  wglDestroyPbufferARB(mPBuffer);
		  mHRC = NULL;

      glDeleteTextures(1, &mTexture);
    }

	  if (mHDC != NULL)
	  {
		  ReleaseDC(mHWnd, mHDC);
		  mHDC = NULL;
	  }
  }

  bool BeginSession()
  {
    int flag = 0;
    wglQueryPbufferARB(mPBuffer, WGL_PBUFFER_LOST_ARB, &flag);

    NGL_ASSERT(flag == 0);

    return MakeCurrent();
  }

  bool MakeCurrent()
  {
	  if (!wglMakeCurrent( mHDC, mHRC))
	  {
      return false;
    }
    return true;
  }


  bool EndSession(HDC dc, HGLRC rc)
  {
    if (mUseRenderTexture)
    {
      if ((int)mTexture <= 0)
      {
        // Create the dynamic texture to the PBuffer
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mPOTWidth, mPOTHeight, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }
      else
      {
        // Bind the dynamic texture to the PBuffer
        glBindTexture(GL_TEXTURE_2D, mTexture);
      }

      wglMakeCurrent(dc, rc);

      if (!wglBindTexImageARB(mPBuffer, WGL_FRONT_LEFT_ARB))
        return false;

      // Draw the quad:
      DrawTextureQuad(0, 0, (float)mWidth, (float)mHeight);

      if (!wglReleaseTexImageARB(mPBuffer, WGL_FRONT_LEFT_ARB))
        return false;
    }
    else // Copy Texture
    {
      if (!wglMakeCurrent(mHDC, mHRC))
        return false;

      if ((int)mTexture <= 0)
      {
        // Create the dynamic texture to the PBuffer
	      glGenTextures(1, &mTexture);
	      glBindTexture(GL_TEXTURE_2D, mTexture);
	      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mPOTWidth, mPOTHeight, 0, GL_RGB, GL_FLOAT, 0);
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }
      else
      {
        // Bind the dynamic texture to the PBuffer
        glBindTexture(GL_TEXTURE_2D, mTexture);
      }

      // Copy the PBuffer to the texture
      glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, mWidth, mHeight);

      wglMakeCurrent(dc, rc);

      // Draw the quad:
      DrawTextureQuad(0, 0, (float)mWidth, (float)mHeight);
    }

    return true;
  }

  void DrawTextureQuad(float x, float y, float width, float height)
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(-1.0f, 1.0f, 0.0f );
    glScalef (2.0f/(float)mWidth, -2.0f/(float)mHeight, 1.0f);
    glViewport(0,0, mWidth, mHeight);
    glReadBuffer(GL_BACK);
    glDrawBuffer(GL_BACK);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float w = (float)mWidth / (float)mPOTWidth;
    float h = (float)mHeight/ (float)mPOTHeight;

    glColor3f(1.0f, 1.0f, 1.0f);

    if (mUseRenderTexture)
    {
      w = h = 1;
    }
    
    glBegin(GL_QUADS);

      glTexCoord2f(0.0f, h);
      glVertex2f(x, y);

      glTexCoord2f(w, h);
      glVertex2f(x + width, y);

      glTexCoord2f(w, 0.0f);
      glVertex2f(x + width, y + height);

      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(x, y + height);
      
    glEnd();
  }

private:
  // WGL_ARB_extensions_string
  PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

  // WGL_ARB_pbuffer
  PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
  PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
  PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
  PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
  PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;

  // WGL_ARB_pixel_format
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

  // WGL_ARB_render_texture
  PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
  PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
  PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;

  HPBUFFERARB mPBuffer;
  HDC mHDC;
  HGLRC mHRC;
  int32 mWidth;
  int32 mHeight;
  GLuint mTexture;

  HWND mHWnd;

  bool mUseRenderTexture;

  uint32 mPOTWidth;
  uint32 mPOTHeight;
};


ATOM nglWindow::mAtom = NULL;

/*
 * OS specific info
 */

nglWindow::OSInfo::OSInfo()
{
  Parent = NULL;
  WindowHandle = NULL;
}

/************************
* Drag And Drop Support *
************************/

#include "nglDataObjects.h"
#include "nglDragAndDropObjects.h"
#include "nglOleDragAndDropInterfaces_WinXX.h"

/*************************
* nglIEnumFormatEtc Impl *
*************************/

void nglEnumFormat::Clear()
{
  mFormatEtc.clear();
}

void nglEnumFormat::CreateFormatEtcArray(nglDragAndDrop* pDraggedObject)
{
  //NGL_OUT(_T("nglEnumFormat::CreateFormatEtcArray()\n"));
  
  NGL_ASSERT(pDraggedObject);
  const std::map<nglString, nglDataObject*>& rTypesMap = pDraggedObject->GetSupportedTypesMap();
  std::map<nglString, nglDataObject*>::const_iterator it = rTypesMap.begin();
  std::map<nglString, nglDataObject*>::const_iterator end = rTypesMap.end();
  while (it != end)
  {
    FORMATETC format;
    memset(&format, 0, sizeof(format));
    nglDataObject* pObj = it->second;
    NGL_ASSERT(pObj);
    pObj->SetFormatType(&format);
    NGL_OUT(_T("FormatEtc %p / %p\n"), this, pDraggedObject);
    NGL_OUT(_T("\t%x\n"), format.cfFormat);
    NGL_OUT(_T("\t%x\n"), format.dwAspect);
    NGL_OUT(_T("\t%x\n"), format.lindex);
    NGL_OUT(_T("\t%x\n"), format.ptd);
    NGL_OUT(_T("\t%x\n"), format.tymed);
    mFormatEtc.push_back(format);
    it++;
  }
  mFormatIterator = mFormatEtc.begin();
}

//From IEnumFORMATETC
HRESULT STDMETHODCALLTYPE nglEnumFormat::Next(ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched)
{
  //NGL_OUT(_T("nglEnumFormat::Next()\n"));
  ULONG i;
  for (i = 0; i < celt && mFormatIterator != mFormatEtc.end(); i++)
  {
    *rgelt = *mFormatIterator;
    rgelt++;
    mFormatIterator++;
  }

  if (pceltFetched)
    *pceltFetched = i;
  if (i != celt)
    return S_FALSE;
  return S_OK;
}
 
HRESULT STDMETHODCALLTYPE nglEnumFormat::Skip(ULONG celt)
{
  //NGL_OUT(_T("nglEnumFormat::Skip()\n"));

  ULONG i;
  for (i = 0; i < celt && mFormatIterator != mFormatEtc.end(); i++)
  {
    mFormatIterator++;
  }
 if (i != celt)
    return S_FALSE;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE nglEnumFormat::Reset(void)
{
  //NGL_OUT(_T("nglEnumFormat::Reset()\n"));
  mFormatIterator = mFormatEtc.begin();
  return S_OK;
}

HRESULT STDMETHODCALLTYPE nglEnumFormat::Clone(IEnumFORMATETC ** ppenum)
{
  //NGL_OUT(_T("nglEnumFormat::Clone()\n"));
  *ppenum = NULL; //#FIXME
  return S_OK;
}


/*********************
* nglDataObject Impl *
*********************/

nglIDataObject::nglIDataObject(nglWindow* pWindow) : mpWindow(pWindow)
{
  mRef = 0;
  mInterfaceIds[0] = IID_IUnknown;
  mInterfaceIds[1] = IID_IDataObject;
  mInterfaceCount = 2;

  mpDraggedObject = NULL;
  mpFormatEtc = new nglEnumFormat();
  mpFormatEtc->AddRef();
}

nglIDataObject::~nglIDataObject() 
{ 
  NGL_OUT(_T("nglIDataObject delete [0x%x]\n"), this);
  NGL_ASSERT(!mpFormatEtc->Release());  
  NGL_ASSERT(!mRef); 
}

void nglIDataObject::Clear()
{
  mpFormatEtc->Clear();
  mpDraggedObject = NULL;
  NGL_OUT(_T("nglIDataObject::Clear() [0x%x]\n"), this);
}

void nglIDataObject::SetDraggedObject(nglDragAndDrop* pDraggedObject)
{
  NGL_OUT(_T("nglIDataObject::SetDraggedObject [0x%x] [0x%x] [0x%x] [thread id: 0x%x]\n"), this, pDraggedObject, mpDraggedObject, GetCurrentThreadId());
  NGL_ASSERT(!mpDraggedObject);

  mpDraggedObject = pDraggedObject;
  mpFormatEtc->CreateFormatEtcArray(pDraggedObject);
}

void nglIDataObject::InitObject(nglDropSource* pSource)
{
  mHWnd = pSource->mHWnd;
  //mMessageId = pSource->mMessageId;
  //NGL_ASSERT(!mpDraggedObject);
  //SetDraggedObject(pSource->GetDraggedObject());
}


// From IDataObject                              
HRESULT STDMETHODCALLTYPE nglIDataObject::GetData(FORMATETC * pFormat, STGMEDIUM * pMedium)
{
  //OutputDebugString("nglDataObject::GetData()\n");

  NGL_ASSERT(mpDraggedObject);

  nglString mime;
  if (App->GetDataTypesRegistry().GetRegisteredMimeType(pFormat->cfFormat, mime))
  {
    mpWindow->OnDragRequestData(mpDraggedObject, mime);

    if (mpDraggedObject->IsTypeSupported(mime))
    {
      nglDataObject* pObj = mpDraggedObject->GetType(mime);
      NGL_ASSERT(pObj);

      pObj->AllocFormatData(pFormat, pMedium);
      GetDataHere(pFormat, pMedium);  
      return S_OK;
    }
  }
  return DV_E_FORMATETC;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::GetDataHere(FORMATETC * pFormat, STGMEDIUM * pMedium)
{
  NGL_ASSERT(mpDraggedObject);
  //OutputDebugString("nglDataObject::GetDataHere()\n");

  nglString mime;
  if (App->GetDataTypesRegistry().GetRegisteredMimeType(pFormat->cfFormat, mime))
  {
    nglDataObject* pObj = mpDraggedObject->GetType(mime);
    NGL_ASSERT(pObj);
    
    pObj->SetFormatData(pFormat, pMedium);
  }


  return S_OK;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::QueryGetData(FORMATETC * pFormatetc)
{
  //OutputDebugString("nglIDataObject::QueryGetData\n");

  if (pFormatetc->cfFormat == CF_TEXT && mpDraggedObject->IsTypeSupported(_T("ngl/Text")))
    return S_OK;
  else if (pFormatetc->cfFormat == CF_HDROP && mpDraggedObject->IsTypeSupported(_T("ngl/Files")))
    return S_OK;
  else if (App->GetDataTypesRegistry().IsTypeRegistered(pFormatetc->cfFormat))
  {
    nglString mime;
    App->GetDataTypesRegistry().GetRegisteredMimeType(pFormatetc->cfFormat, mime);
    if (mpDraggedObject->IsTypeSupported(mime))
      return S_OK;
  }
  return S_FALSE;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::GetCanonicalFormatEtc(FORMATETC * pFormatetcIn, FORMATETC * pFormatetcOut)
{
  pFormatetcOut = NULL;
  return DATA_S_SAMEFORMATETC;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::SetData(FORMATETC * pFormatetc, STGMEDIUM * pmedium, BOOL fRelease)
{
  //OutputDebugString("nglDataObject::SetData()\n");

  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC ** ppenumFormatetc)
{
  //NGL_OUT(_T("nglDataObject::EnumFormatEtc(%s)\n"), dwDirection == DATADIR_GET ? "GET" : "SET");
  *ppenumFormatetc = (IEnumFORMATETC*)mpFormatEtc;
  mpFormatEtc->AddRef();
  return S_OK;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::DAdvise(FORMATETC * pFormatetc, DWORD advf, IAdviseSink * pAdvSink, DWORD * pdwConnection)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::DUnadvise(DWORD dwConnection)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT STDMETHODCALLTYPE nglIDataObject::EnumDAdvise(IEnumSTATDATA ** ppenumAdvise)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

/*********************
* nglDropSource Impl *
*********************/
bool nglDropSource::Drag()
{
  POINT pt;
  HWND hwndAttach;
  DWORD dwAttachThreadID;
  DWORD dwCurrentThreadID;
  HRESULT res = E_FAIL;

  mpIDataObject->SetDraggedObject(mpDraggedObject);
  
  const std::list<nglDropEffect>& dropEffects(mpDraggedObject->GetSupportedDropEffects());
  DWORD allowedDropEffects = 0;
  for (std::list<nglDropEffect>::const_iterator i = dropEffects.begin(); i != dropEffects.end(); i++)
    SetDropEffect(*i, &allowedDropEffects);

  // Find the window under the mouse pointer.
  // This window might not be owned by the current thread, which
  // means you need to use AttachThreadInput in order for mouse
  // capture (and drag and drop) to work correctly.
  GetCursorPos(&pt);
  hwndAttach = WindowFromPoint(pt);
  if (!hwndAttach)
      return false;
  mHWnd = hwndAttach;
  
  mpIDataObject->InitObject(this);
  
  // Get thread ID's
  dwAttachThreadID = GetWindowThreadProcessId(hwndAttach, NULL);
  dwCurrentThreadID = GetCurrentThreadId();

  // Attach input queues if necessary
  if (dwAttachThreadID != dwCurrentThreadID)
    AttachThreadInput(dwAttachThreadID, dwCurrentThreadID, TRUE);

  // Do the drag and drop
  mInitialButton = 0;
  DWORD dropEffect = 0;
  //OutputDebugString("DoDragDrop BEGIN\n");

  res = DoDragDrop((IDataObject*)mpIDataObject, (IDropSource*)this, allowedDropEffects, &dropEffect);
  //OutputDebugString("DoDragDrop END\n");

  // Detach input queues
  //if (dwAttachThreadID != dwCurrentThreadID)
  //  AttachThreadInput(dwAttachThreadID, dwCurrentThreadID, FALSE);
  
  if (res == DRAGDROP_S_DROP)
    return true;
  return false;
}

void nglDropSource::ClearObjects()
{
  mpIDataObject->Clear();
  mpDraggedObject = NULL;
}

// From IDropSource:
HRESULT STDMETHODCALLTYPE nglDropSource::GiveFeedback(DWORD dwEffect)
{

  nglDropEffect effect = GetDropEffect(&dwEffect);

  mpDraggedObject->SetDesiredDropEffect(effect);
  LPARAM lParam = (LPARAM)effect;

  //SendMessage(mHWnd, mMessageId, NGL_GIVE_FEEDBACK_MESSAGE, lParam);
  //return S_OK;
  return ResultFromScode(DRAGDROP_S_USEDEFAULTCURSORS);
}
HRESULT STDMETHODCALLTYPE nglDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
  //NGL_OUT(_T("nglDropSource::QueryContinueDrag\n"));

  if (fEscapePressed)
  {
    return DRAGDROP_S_CANCEL;
  }
  if ( mInitialButton == 0 )
    mInitialButton = grfKeyState & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON);

  if ( !(grfKeyState & mInitialButton) ) 
  {
    NGL_OUT(_T("nglDropSource::QueryContinueDrag return DRAGDROP_S_DROP\n"));
    /// detach inputs now
    DWORD dwAttachThreadID  = GetWindowThreadProcessId(mHWnd, NULL);
    DWORD dwCurrentThreadID = GetCurrentThreadId();
    AttachThreadInput(dwAttachThreadID, dwCurrentThreadID, FALSE);
    return DRAGDROP_S_DROP;
  }

  return S_OK;
}

/*********************
* nglDropTarget Impl *
*********************/
HRESULT nglDropTarget::GetObject(IDataObject* pDataObj, bool getData)
{
  NGL_OUT(_T("nglDropTarget::GetObject(0x%x) [0x%x]\n"), pDataObj, this);
  HRESULT res;  
  IEnumFORMATETC* pEnumFormat;
  res = pDataObj->EnumFormatEtc(DATADIR_GET, &pEnumFormat);
  STGMEDIUM Medium;

  if (res != S_OK)
    return res;
  pEnumFormat->Reset();

  ULONG fetched;
  FORMATETC Format;
  res = pEnumFormat->Next(1, &Format, &fetched);
  while (fetched && res == S_OK)
  {
    NGL_OUT(_T("nglDropTarget::GetObject type: %d\n"), Format.cfFormat);

    nglString mime;
    if (App->GetDataTypesRegistry().GetRegisteredMimeType(Format.cfFormat, mime))
    {
      NGL_OUT(_T("nglDropTarget::GetObject mime type: %s\n"), mime.GetChars());
      nglDataObject* pObj = NULL;
      pObj = mpObject->GetType(mime);
      if (!pObj)
      {
        pObj = App->GetDataTypesRegistry().Create(mime);
        mpObject->AddType(pObj);
      }

      if (getData && pObj)
      {
        pDataObj->GetData(&Format, &Medium);
        NGL_ASSERT(pObj);
        pObj->GetFormatData(&Format, &Medium);
      }

    }
    res = pEnumFormat->Next(1, &Format, &fetched);
  }
  pEnumFormat->Release();
  return S_OK;
}

HRESULT STDMETHODCALLTYPE nglDropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
  //NGL_OUT(_T("nglDropTarget::DragEnter()\n"));
  
  int x, y;
  mpWindow->GetPosition(x,y);
  x = MAX(0, (int)pt.x - x);
  y = MAX(0, (int)pt.y - y);

  *pdwEffect = DROPEFFECT_NONE;
  
  nglMouseInfo::Flags mButton = 0;
  if (grfKeyState == MK_LBUTTON)
    mButton |= nglMouseInfo::ButtonLeft;
  else if (grfKeyState == MK_MBUTTON)
    mButton |= nglMouseInfo::ButtonMiddle;
  else if (grfKeyState == MK_RBUTTON)
    mButton |= nglMouseInfo::ButtonRight;


  mpObject = new nglDragAndDrop(eDropEffectCopy);
  //NGL_OUT(_T("NEW: DataObject from nglDropTarget\n"));



  HRESULT res;
  if ((res = GetObject(pDataObj, true)) != S_OK)
    return res;

  /*
  if (mpObject->IsEmpty())
  {
    *pdwEffect = DROPEFFECT_NONE;     
    return S_OK;
  }
  */

  mpWindow->OnDragEnter();
  GetDropEffects(mpObject, pdwEffect);
  *pdwEffect = 0;


  nglDropEffect effect = mpWindow->OnCanDrop(mpObject, x, y, mButton);
  mpObject->SetDesiredDropEffect(effect);

  if (effect != eDropEffectNone)
  {
    SetDropEffect(mpObject->GetDesiredDropEffect(), pdwEffect);
  }
  else
    *pdwEffect = DROPEFFECT_NONE;
  return DragOver(grfKeyState, pt, pdwEffect);
}

HRESULT STDMETHODCALLTYPE nglDropTarget::DragLeave(void)
{
  //NGL_OUT(_T("nglDropTarget::DragLeave()\n"));
  mpWindow->OnDragLeave();
  
  NGL_ASSERT(mpObject);
  delete mpObject;
  mpObject = NULL;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE nglDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
  int x, y;
  mpWindow->GetPosition(x,y);
  x = MAX(0, (int)pt.x - x);
  y = MAX(0, (int)pt.y - y);

  nglMouseInfo::Flags mButton = 0;
  if (grfKeyState & MK_LBUTTON)
    mButton |= nglMouseInfo::ButtonLeft;
  else if (grfKeyState & MK_MBUTTON)
    mButton |= nglMouseInfo::ButtonMiddle;
  else if (grfKeyState & MK_RBUTTON)
    mButton |= nglMouseInfo::ButtonRight;

  mpObject->mModifiers = 0; 
  if (grfKeyState & MK_CONTROL && !(mpWindow->IsKeyDown(NK_LCTRL) || mpWindow->IsKeyDown(NK_RCTRL)))
  {
    mpWindow->CallOnKeyDown(nglKeyEvent(NK_LCTRL, 0, 0));
  }
  else if (!(grfKeyState & MK_CONTROL))
  {
    if (mpWindow->IsKeyDown(NK_LCTRL))
      mpWindow->CallOnKeyUp(nglKeyEvent(NK_LCTRL, 0, 0));
    if (mpWindow->IsKeyDown(NK_RCTRL))
      mpWindow->CallOnKeyUp(nglKeyEvent(NK_RCTRL, 0, 0));
  }
  if (grfKeyState & MK_SHIFT && !(mpWindow->IsKeyDown(NK_LSHIFT) || mpWindow->IsKeyDown(NK_RSHIFT)))
    mpWindow->CallOnKeyDown(nglKeyEvent(NK_LSHIFT, 0, 0));
  else if (!(grfKeyState & MK_SHIFT))
  {
    if (mpWindow->IsKeyDown(NK_LSHIFT))
      mpWindow->CallOnKeyUp(nglKeyEvent(NK_LSHIFT, 0, 0));
    if (mpWindow->IsKeyDown(NK_RSHIFT))
      mpWindow->CallOnKeyUp(nglKeyEvent(NK_RSHIFT, 0, 0));
  }
  if (grfKeyState & MK_ALT && !(mpWindow->IsKeyDown(NK_LALT) || mpWindow->IsKeyDown(NK_RALT)))
    mpWindow->CallOnKeyDown(nglKeyEvent(NK_LALT, 0, 0));
  else if (!(grfKeyState & MK_ALT))
  {
    if (mpWindow->IsKeyDown(NK_LALT))
      mpWindow->CallOnKeyUp(nglKeyEvent(NK_LALT, 0, 0));
    if (mpWindow->IsKeyDown(NK_RALT))
      mpWindow->CallOnKeyUp(nglKeyEvent(NK_RALT, 0, 0));
  } 

  GetDropEffects(mpObject, pdwEffect);
  *pdwEffect = 0;
  
  nglDropEffect dropEffect = mpWindow->OnCanDrop(mpObject, x, y, mButton);
  mpObject->SetDesiredDropEffect(dropEffect);
 
  if (dropEffect != eDropEffectNone)
  {
    SetDropEffect(dropEffect, pdwEffect);
  }
  else
    *pdwEffect = DROPEFFECT_NONE;
  
  HRESULT res = S_OK;
  return res;
}

HRESULT STDMETHODCALLTYPE nglDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{

  int x, y;
  mpWindow->GetPosition(x,y);
  x = MAX(0, (int)pt.x - x);
  y = MAX(0, (int)pt.y - y);

  nglMouseInfo::Flags mButton = 0;
  if (grfKeyState == MK_LBUTTON)
    mButton |= nglMouseInfo::ButtonLeft;
  else if (grfKeyState == MK_MBUTTON)
    mButton |= nglMouseInfo::ButtonMiddle;
  else if (grfKeyState == MK_RBUTTON)
    mButton |= nglMouseInfo::ButtonRight;

  GetDropEffects(mpObject, pdwEffect);
  *pdwEffect = 0;

  nglDropEffect dropEffect = mpWindow->OnCanDrop(mpObject, x, y, mButton);
  mpObject->SetDesiredDropEffect(dropEffect);

  if (dropEffect != eDropEffectNone)
  { 
    //GetObject(pDataObj, true);
    mpWindow->OnDropped(mpObject, x, y, mButton);
    
    SetDropEffect(dropEffect, pdwEffect);
  }
  else
    *pdwEffect = DROPEFFECT_NONE;

  NGL_ASSERT(mpObject);
  delete mpObject;
  mpObject = NULL;
  return S_OK;
}





/*
 * Constructors
 */

nglWindow::nglWindow (uint Width, uint Height, bool IsFullScreen)
{
  Register();
  mpMainMenu = NULL;
  nglContextInfo context; // Get default context
  nglWindowInfo info(Width, Height, IsFullScreen);

  InternalInit (context, info, NULL);
}

nglWindow::nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  Register();
  mpMainMenu = NULL;
  InternalInit (rContext, rInfo, pShared);
}

void nglWindow::Exit()
{
  if (mAtom != NULL)
    UnregisterClass(NGL_WINDOW_CLASS,App->GetHInstance());
}

DWORD nglWindow::WM_CREATED = WM_USER;

bool nglWindow::InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  NGL_OUT(_T("nglWindow::InternalInit this = 0x%x\n"), this);
  SetError(NGL_WINDOW_ENONE);

  mInModalState = 0;
 
  if (!mAtom)
  {
    //WM_CREATED = RegisterWindowMessage(_T("NUI_WM_CREATED"));

    WNDCLASS wc;

    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = App->GetHInstance();
    wc.hIcon         = LoadIcon( App->GetHInstance(), _T("0") );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.lpszMenuName  = NULL;
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
    wc.hbrBackground = NULL;
    wc.lpfnWndProc   = &::WndProc;
    wc.lpszClassName = NGL_WINDOW_CLASS;
    mAtom = RegisterClass( &wc );
    if (!mAtom)
    {
      NGL_DEBUG( OutputDebugString(_T("NGL: error: unable to create ") NGL_WINDOW_CLASS _T(" window class\n")); )
      return false;
    }
    App->AddExit(&nglWindow::Exit);
  }

  // Portable members
  mOSInfo    = rInfo.OSInfo;
  mFlags     = rInfo.Flags;
  mEventMask = rInfo.Events;
  mCursor    = rInfo.Cursor;
  mMouseMode = rInfo.MouseMode;
  mKeyRepeat = rInfo.KeyRepeat;
  memset (mpKeyState, 0, sizeof(bool)*NGL_KEY_MAX);

  // Internal status
  mMouseButtonStatus = nglMouseInfo::ButtonNone;
  mOldMouseX = 0;
  mOldMouseY = 0;
  mState = eHidden;
  mFullScreen = (rInfo.Flags & FullScreen) ? true : false;

  mFBO_BackBuffer = 0;
  mFBO_BackBufferTexture = 0;
  mFBO_BackBufferStencil = 0;
  mFBO_BackBufferDepth = 0;
  mBackBufferTexture = -1;
  mBackTextureWidth = 0;
  mBackTextureHeight = 0;
  mSkipBackBufferCopy = false;
  mpPBuffer = NULL;
  mPBufferDisabled = false;

  if (rInfo.Width == 0 && rInfo.Height == 0)
  {
    SetError(_T("window"), NGL_WINDOW_ERANGE);
    return false;
  }

  RECT rect;
  long x,y,w,h;
  long ws, wsx;
  HWND Parent = rInfo.OSInfo.Parent;
  
  w = rInfo.Width;
  h = rInfo.Height;

  if (mFullScreen) 
  {
    nglVideoMode mode;
    uint bitdepth;
    // mSavedVideoMode = nglVideoMode(); This is implicitly done when nglWindow is built

    bitdepth = rContext.FrameBitsR;
    bitdepth+= rContext.FrameBitsG;
    bitdepth+= rContext.FrameBitsB;
    // If bitdepth==24, we could try 32bpp _then_ 24bpp, but 24bpp modes should die anyway :)
    if (bitdepth == 24)
      bitdepth = 32;
    if (nglVideoMode::Find(mode, w, h, bitdepth) && mode.SetMode())
    {
      ws  = WS_POPUP;
      wsx = WS_EX_APPWINDOW | WS_EX_TOPMOST;
      x = 0;
      y = 0;
    }
    else
      mFullScreen = false; // Fallback to window'ed mode
  }

  if (!mFullScreen)
  {
    ws = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    wsx = (Parent) ? 0 : WS_EX_APPWINDOW;
    x = rInfo.XPos;
    y = rInfo.YPos;

    if (!Parent)
    {
      if (!(mFlags & NoBorder)) 
      {
        ws=WS_CAPTION | WS_MINIMIZEBOX  | WS_SYSMENU;
      }
      else
      {
        ws= WS_POPUP;
      }

      if (!(mFlags & NoResize)) 
        ws|=WS_SIZEBOX | WS_MAXIMIZEBOX; 
    }
    else
    {
      ws = WS_CHILD | WS_VISIBLE;
    }

    rect.left=x;
    rect.right=x+w;
    rect.top=y;
    rect.bottom=y+h;
    AdjustWindowRectEx(&rect,ws,FALSE,wsx);
    w=rect.right-rect.left;
    h=rect.bottom-rect.top;
    x=rect.left;
    y=rect.top;

    switch (rInfo.Pos)
    {
    case nglWindowInfo::ePosAuto: 
      x=y=CW_USEDEFAULT;
      break;
    case nglWindowInfo::ePosMouse: 
      { 
        POINT p; 
        RECT r;

        GetClientRect(GetDesktopWindow(),&r);
        GetCursorPos(&p);
        x=p.x-w/2; 
        y=p.y-h/2; 
        if (x<0) x=0;
        if (y<0) y=0;
        if (x+w>r.right) x=r.right-w;
        if (y+h>r.bottom) y=r.bottom-h;

      }
      break;
    case nglWindowInfo::ePosCenter:
      { 
        RECT r;
        GetClientRect(GetDesktopWindow(),&r);
        x=(r.right-r.left-w)/2; 
        y=(r.bottom-r.top-h)/2; 
      }
      break;
    }
  }

  const nglChar* title = rInfo.Title.GetChars();
  mOSInfo.WindowHandle = mHWnd = CreateWindowEx(
    wsx,
    NGL_WINDOW_CLASS,
    title ? title : _T("NGL - http://www.nongnu.org/ngl"),
    ws,
    x, y, w, h,
    Parent,
    NULL,
    App->GetHInstance(),
    this );
  NGL_OUT(_T("Create nglWindow hwnd = 0x%x"), mOSInfo.WindowHandle);

  RECT R;
  GetClientRect(mHWnd, &R);
  mOSInfo.GLWindowHandle = mGLHWnd = CreateWindowEx(
    0,
    _T("Static"),
    _T("NGL GL Child Window"),
    WS_CHILD | WS_VISIBLE,
    0, 0, R.right - R.left, R.bottom - R.top,
    mHWnd,
    NULL,
    App->GetHInstance(),
    this );
  NGL_OUT(_T("Create nglWindow GLhwnd = 0x%x"), mOSInfo.GLWindowHandle);

  if (!mHWnd || !mGLHWnd)
    return false;

  SetMouseMode(mMouseMode);
  SetKeyRepeat(mKeyRepeat);
  SetEventMask(mEventMask);
  SetCursor(mCursor);

  // Fiat ventana
#ifdef _DEBUG_
  nglWindowInfo info = rInfo;

  info.XPos = x;
  info.YPos = y;
  info.Width = w;
  info.Height = h;
  if (!mFullScreen) info.Flags &= ~FullScreen;
  info.Dump(NGL_LOG_INFO);
#endif // _DEBUG_
  UpdateWindow(mHWnd);
  ShowWindow(mHWnd, SW_HIDE);
  mState = eHidden;

  // Add 'Show console' accelerator to the window's system menu
  HMENU hmenu = GetSystemMenu(mHWnd,FALSE);
  if (hmenu)
  {
    AppendMenu(hmenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hmenu, MF_STRING, MENU_SHOWCONSOLE_ID, _T("&Show console\tCtrl+²"));
  }

  /* Drag and Drop stuff */

  App->GetDataTypesRegistry().RegisterDataType(_T("ngl/Text"), CF_TEXT, nglDataTextObject::Create);
  App->GetDataTypesRegistry().RegisterDataType(_T("ngl/Files"), CF_HDROP, nglDataFilesObject::Create);
  
  mpDropTarget = new nglDropTarget(mHWnd, this);
  mpDropTarget->AddRef();
  bool initiated = mpDropTarget->InitOleDrop();
  NGL_ASSERT(initiated);
  mpDropSource = new nglDropSource(this);
  mpDropSource->AddRef();
  mOnDragging = false;
//   mDragMessageId = RegisterWindowMessage(_T("nglDragAndDrop"));
//   mpDropSource->SetMessageId(mDragMessageId);

  if(!Build(mGLHWnd, rContext, pShared) || !MakeCurrent())
    return false;

  PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
  PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;
  const GLubyte* pStr = glGetString(GL_EXTENSIONS);
  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
  wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

  if (wglSwapIntervalEXT)
  {
    if (rContext.VerticalSync)
      wglSwapIntervalEXT(1);
    else
      wglSwapIntervalEXT(0);
  }

  NGL_DEBUG( nglContext::Dump(NGL_LOG_INFO); );
  if (mTargetAPI == eTargetAPI_OpenGL)
    glViewport(0,0,w,h);

  return true;
}

nglWindow::~nglWindow()
{
  // automatic deleting of the main menu
  if (mpMainMenu)
  {
    mpMainMenu->UnregisterFromWindow(this);
    delete mpMainMenu;
  }

  delete mpPBuffer;

  if (mpDropTarget)
  {
    mpDropTarget->UnInitOleDrop();
    NGL_ASSERT(!(mpDropTarget->Release()));
  }
  if (mpDropSource)
  {
    //#FIXME clear objects
    NGL_ASSERT(!mpDropSource->Release());
  }
  if (mFullScreen && GetForegroundWindow()==mHWnd)
    mSavedVideoMode.SetMode();
  GetSystemMenu(mHWnd,TRUE);
  DestroyWindow(mHWnd);
  
  Unregister();
}

void nglWindow::SetState (StateChange State)
{
  switch (State)
  {
    case eHide:
      ShowWindow(mHWnd,SW_HIDE);
      mState=eHidden;
      break;
    case eShow:
      ShowWindow(mHWnd,SW_SHOWNORMAL);
//      ShowWindow(mHWnd,SW_RESTORE);
      mState=eVisible;
      if (mFullScreen)
        SetForegroundWindow(mHWnd);
      break;
    case eMinimize:
      mState=eHidden;
      ShowWindow(mHWnd,SW_SHOW);
      ShowWindow(mHWnd,SW_SHOWMINIMIZED);
      break;
    case eMaximize:
      mState=eVisible;
      ShowWindow(mHWnd,SW_SHOW);
      ShowWindow(mHWnd,SW_SHOWMAXIMIZED);
      break;
  }
}

nglWindow::StateInfo nglWindow::GetState() const
{
  return mState;
}

uint nglWindow::GetWidth() const
{
  RECT r;
  GetClientRect(mHWnd,&r);
  return r.right-r.left;
}

uint nglWindow::GetHeight() const
{
  RECT r;
  GetClientRect(mHWnd,&r);
  return r.bottom-r.top;
}

void nglWindow::GetSize (uint& rWidth, uint& rHeight) const
{
  RECT r;
  GetClientRect(mHWnd,&r);
  rWidth = r.right-r.left;
  rHeight = r.bottom-r.top;
}

bool nglWindow::SetSize (uint Width, uint Height)
{
  RECT r;
  RECT c;
  uint x,y,w,h;

  GetWindowRect(mHWnd,&r);
  GetClientRect(mHWnd,&c);

  if (Width < 1) Width = 1;
  if (Height < 1) Height = 1;
  x=r.left;
  y=r.top;
  w=r.right-r.left-c.right+Width;
  h=r.bottom-r.top-c.bottom+Height;
  MoveWindow(mHWnd,x,y,w,h,true);

  return true;
}

void nglWindow::GetPosition (int& rXPos, int& rYPos) const
{
  POINT p;
  p.x = p.y = 0;

  ClientToScreen(mHWnd,&p);
  rXPos = p.x;
  rYPos = p.y;
}

bool nglWindow::SetPosition (int XPos, int YPos)
{
  RECT r;
  POINT p;
  p.x = p.y = 0;
  uint x, y, w, h;

  if (XPos < 0) XPos = 0;
  if (YPos < 0) YPos = 0;

  GetWindowRect(mHWnd, &r);
  ClientToScreen(mHWnd, &p);

  x = r.left - p.x + XPos;
  y = r.top - p.y + YPos;
  w = r.right - r.left;
  h = r.bottom - r.top;
  MoveWindow(mHWnd, x, y, w, h, true);

  return true;
}

bool nglWindow::GetResolution (float& rHorizontal, float& rVertical) const
{
  rHorizontal = (float)GetDeviceCaps(mDC, LOGPIXELSX);
  rVertical   = (float)GetDeviceCaps(mDC, LOGPIXELSY);
  return true; // GetDeviceCaps() can't fail according to MSDN. Preposterous :)
}


nglString nglWindow::GetTitle() const
{
  nglString result;
  nglChar title[1024];

  GetWindowText(mHWnd, title, 1024);
  if (title)
    result = title;

  return result;
}

void nglWindow::SetTitle (const nglString& rTitle)
{
  const nglChar* title = rTitle.GetChars();

  SetWindowText(mHWnd, title ? title : _T(""));
}

// IDC_HAND is only available from NT 5.0
#ifndef IDC_HAND
#define IDC_HAND IDC_ARROW
#endif

HCURSOR nglWindow::GetCursorIDC(nuiMouseCursor Cursor)
{
  HINSTANCE hInst = NULL;
  switch (Cursor)
  {
//    case eCursorNone      : FIXME
    case eCursorArrow       : return LoadCursor(hInst,IDC_ARROW);
    case eCursorCross       : return LoadCursor(hInst,IDC_CROSS);
    case eCursorHand        : return LoadCursor(hInst,IDC_HAND);
    case eCursorClosedHand  : return LoadCursor(hInst,IDC_HAND);
    case eCursorPointingHand: return LoadCursor(hInst,IDC_HAND); // TODO: use a "pointing hand" cursor instead. can't find it in the API...
    case eCursorIBeam       : return LoadCursor(hInst, IDC_IBEAM);
    case eCursorHelp        : return LoadCursor(hInst,IDC_HELP);
    case eCursorWait        : return LoadCursor(hInst,IDC_WAIT);
    case eCursorCaret       : return LoadCursor(hInst,IDC_IBEAM);
    case eCursorDnD         : return LoadCursor(hInst,IDC_ARROW);
    case eCursorForbid      : return LoadCursor(hInst,IDC_NO);
    case eCursorMove        : return LoadCursor(hInst,IDC_SIZEALL);
    case eCursorResize      : return LoadCursor(hInst,IDC_SIZEALL);
    case eCursorResizeNS    : return LoadCursor(hInst,IDC_SIZENS);
    case eCursorResizeWE    : return LoadCursor(hInst,IDC_SIZEWE);
    case eCursorResizeN     : return LoadCursor(hInst,IDC_SIZENS);
    case eCursorResizeS     : return LoadCursor(hInst,IDC_SIZENS);
    case eCursorResizeW     : return LoadCursor(hInst,IDC_SIZEWE);
    case eCursorResizeE     : return LoadCursor(hInst,IDC_SIZEWE);
    case eCursorResizeNW    : return LoadCursor(hInst,IDC_SIZENWSE);
    case eCursorResizeNE    : return LoadCursor(hInst,IDC_SIZENESW);
    case eCursorResizeSW    : return LoadCursor(hInst,IDC_SIZENESW);
    case eCursorResizeSE    : return LoadCursor(hInst,IDC_SIZENWSE);
  }
  return NULL;
}


nuiMouseCursor nglWindow::GetCursor() const
{
  return mCursor;
}

bool nglWindow::SetCursor (nuiMouseCursor Cursor)
{
  if (mCursor==Cursor)
    return true;
  mCursor=Cursor;
  if (mCursor == eCursorNone)
  {
    ShowCursor(FALSE);
  }
  else
  {
    ShowCursor(TRUE);
    HCURSOR c = GetCursorIDC(mCursor);
    if (c)
      ::SetCursor(c);
  }
  return true;
}


nglWindow::EventMask nglWindow::GetEventMask() const
{
  return mEventMask;
}

void nglWindow::SetEventMask(EventMask Events)
{
  mEventMask = Events;
}


bool nglWindow::GetKeyRepeat() const
{
  return mKeyRepeat;
}

void nglWindow::SetKeyRepeat(bool AllowRepeat)
{
  mKeyRepeat = AllowRepeat;
}

nglMouseInfo::Mode nglWindow::GetMouseMode() const
{
  return mMouseMode;
}

void nglWindow::SetMouseMode(nglMouseInfo::Mode Mode)
{
  if ((Mode == nglMouseInfo::eRelative) && (mMouseMode != Mode))
  {
    RECT r;
    RECT c;
    
    GetClientRect(mHWnd,&c);
    GetWindowRect(mHWnd,&r);
    mOldMouseX=c.right/2;
    mOldMouseY=c.bottom/2;
    if (GetActiveWindow()==mHWnd)
    {
      POINT p;
      ShowCursor(FALSE);
      p.x=mOldMouseX;
      p.y=mOldMouseY;
      ClientToScreen(mHWnd,&p);
      SetCursorPos(p.x,p.y);
    }
  }

  if ((Mode == nglMouseInfo::eAbsolute) && (mMouseMode != Mode))
  {
    if (GetActiveWindow()==mHWnd)
      ShowCursor(TRUE);
  }

  mMouseMode = Mode;
}


void nglWindow::GetMouse (nglMouseInfo& rInfo, bool Local) const
{
  POINT p;

  switch (mMouseMode)
  {
    case nglMouseInfo::eRelative:
      p.x = 0;
      p.y = 0;
      break;

    case nglMouseInfo::eAbsolute:
      GetCursorPos(&p);
      if (Local)
        ScreenToClient(mHWnd,&p);
      break;
  }
  rInfo.X = p.x;
  rInfo.Y = p.y;
  rInfo.Buttons = mMouseButtonStatus;
}

const nglWindow::OSInfo* nglWindow::GetOSInfo() const
{
  return &mOSInfo;
}

void NGL_API nglCheckForGLErrors()
{
#if 0 // Globally enable/disable OpenGL error checking
#ifdef _DEBUG_
  bool error = false;
  GLenum err = glGetError();
  while (err != GL_NO_ERROR)
  {
    switch (err)
    {
      /*
      case GL_NO_ERROR:
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, "error has been recorded. The value of this symbolic constant is guaranteed to be zero.");
      */
      break;
    case GL_INVALID_ENUM: 
      NGL_LOG(_T("nglWindow"), NGL_LOG_ALWAYS, "An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag.");
      break;
    case GL_INVALID_VALUE: 
      NGL_LOG(_T("nglWindow"), NGL_LOG_ALWAYS, "A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag.");
      break;
    case GL_INVALID_OPERATION:
      NGL_LOG(_T("nglWindow"), NGL_LOG_ALWAYS, "The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag.");
      break;
    case GL_STACK_OVERFLOW:
      NGL_LOG(_T("nglWindow"), NGL_LOG_ALWAYS, "This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag.");
      break;
    case GL_STACK_UNDERFLOW:
      NGL_LOG(_T("nglWindow"), NGL_LOG_ALWAYS, "This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag.");
      break;
    case GL_OUT_OF_MEMORY:
      NGL_LOG(_T("nglWindow"), NGL_LOG_ALWAYS, "There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded.");
      break;
    }
#ifdef _WIN32_
    //__asm int 3;
#else
    NGL_ASSERT(0);
#endif
    err = glGetError();
  }
#endif
#endif
}

static inline  void CheckFrameBufferStatus(nglContext* pContext)
{
  GLenum status;
  status = pContext->glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  switch(status) 
  {
  case GL_FRAMEBUFFER_COMPLETE_EXT:
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    NGL_ASSERT(0);
    // choose different formats
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    NGL_ASSERT(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    NGL_ASSERT(0);
    break;
  //case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
  //  NGL_ASSERT(0);
  //  break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    NGL_ASSERT(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    NGL_ASSERT(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    NGL_ASSERT(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    NGL_ASSERT(0);
    break;
  default:
    // programming error; will fail on all hardware
    NGL_ASSERT(0);
    break;
  }
}

#define OUT_FB_ERROR(X) case X: NGL_OUT(_T("FrameBuffer Error: %s\n"), _T(#X)); NGL_ASSERT(0);  break;

#define CHECK_FRAMEBUFFER_STATUS()                              \
{                                                               \
  GLenum status;                                                \
  status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);     \
  switch(status)                                                \
  {                                                             \
  case GL_FRAMEBUFFER_COMPLETE_EXT:                             \
    break;                                                      \
  OUT_FB_ERROR(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)                \
  OUT_FB_ERROR(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)        \
  OUT_FB_ERROR(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)                \
  OUT_FB_ERROR(GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)                   \
  OUT_FB_ERROR(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)               \
  OUT_FB_ERROR(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)               \
  OUT_FB_ERROR(GL_FRAMEBUFFER_UNSUPPORTED_EXT)                          \
  default:                                                      \
    /* programming error; will fail on all hardware */          \
    NGL_ASSERT(0);                                                  \
  }                                                             \
}

static const GLenum nglFBOSwapTarget = GL_TEXTURE_2D;

void nglWindow::BeginSession()
{
  if (mTargetAPI == eTargetAPI_OpenGL)
  {
    MakeCurrent();

    nglCheckForGLErrors();
    //mValidBackBufferRequestedNotGranted = true;

    int w = GetWidth();
    int h = GetHeight();

    if (mValidBackBufferRequestedNotGranted)
    {
      if (NGL_ENABLE_FBO_COPYONSWAP && CheckExtension(_T("GL_EXT_framebuffer_object")))
      {
        if (!CheckExtension(_T("GL_ARB_texture_non_power_of_two")))
        {
          int ww = 1;
          while (ww < w)
            ww *= 2;
          int hh = 1;
          while (hh < w)
            hh *= 2;

          w = ww;
          h = hh;
        }

        if (!mFBO_BackBuffer)
        {

          glGenFramebuffersEXT(1, &mFBO_BackBuffer);  nglCheckForGLErrors();

          glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO_BackBuffer);  nglCheckForGLErrors();
          glGenRenderbuffersEXT(1, &mFBO_BackBufferDepth);  nglCheckForGLErrors();

          glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

          //if (!mFBO_BackBufferTexture)
          {
            // initialize color texture
            glGenTextures(1, &mFBO_BackBufferTexture);  nglCheckForGLErrors();
            glBindTexture(nglFBOSwapTarget, mFBO_BackBufferTexture);  nglCheckForGLErrors();
            glTexParameterf(nglFBOSwapTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  nglCheckForGLErrors();
            glTexImage2D(nglFBOSwapTarget, 0, GL_RGB8, w, h, 0, GL_RGB, GL_INT, NULL);  nglCheckForGLErrors();
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, nglFBOSwapTarget, mFBO_BackBufferTexture, 0);  nglCheckForGLErrors();
            CHECK_FRAMEBUFFER_STATUS();
          }
          
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO_BackBuffer);  nglCheckForGLErrors();

        CheckFrameBufferStatus(this);

        glMatrixMode(GL_PROJECTION);
        nglCheckForGLErrors();
        glViewport(0,0, GetWidth(), GetHeight());
        nglCheckForGLErrors();

        glLoadIdentity();
        nglCheckForGLErrors();
        //  glScalef (1, -1, 1);
        glTranslatef(-1.0f, 1.0f, 0.0f );
        nglCheckForGLErrors();
        glScalef (2.0f/(float)GetWidth(), -2.0f/(float)GetHeight(), 1.0f);
        nglCheckForGLErrors();

        glMatrixMode (GL_MODELVIEW);
        nglCheckForGLErrors();
        glLoadIdentity();
        nglCheckForGLErrors();

        glDisable(nglFBOSwapTarget);
      }
      else
      {
        //mPBufferDisabled = true; // Disable for now
        if (!mPBufferDisabled)
        {
          if (!mpPBuffer)
          {
            // Create a PBuffer:
            mpPBuffer = new nglPBuffer();
            if (!mpPBuffer->Create(mDC, mRC, mHWnd, GetWidth(), GetHeight()))
            {
              delete mpPBuffer;
              mpPBuffer = NULL;
              mPBufferDisabled = true;
            }
          }
          if (mpPBuffer)
            mpPBuffer->BeginSession();
        }
      }
    }

  }
  else if (mTargetAPI == eTargetAPI_Direct3D)
  {
    NGL_ASSERT(mpDirect3DDevice);
    HRESULT hr;
    hr = mpDirect3DDevice->BeginScene();

    NGL_ASSERT(S_OK == hr);
  }
}

bool nglWindow::MakeCurrent() const
{
  if (mpPBuffer)
    return mpPBuffer->MakeCurrent();
  return nglContext::MakeCurrent();
}

void nglWindow::EndSession()
{
  if (mTargetAPI == eTargetAPI_OpenGL)
  {
#if HAVE_NV_PERFKIT
    NVPerfHeartbeatOGL();
#endif


    if (mValidBackBufferRequestedNotGranted)
    {
      if (NGL_ENABLE_FBO_COPYONSWAP && mFBO_BackBuffer && mFBO_BackBufferTexture)
      {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        nglCheckForGLErrors();
        //glPushAttrib(GL_ALL_ATTRIB_BITS);
        nglCheckForGLErrors();
        glBindTexture(nglFBOSwapTarget, mFBO_BackBufferTexture);
        nglCheckForGLErrors();

        glDisable(GL_SCISSOR_TEST);
        glMatrixMode(GL_PROJECTION);
        glViewport(0,0, GetWidth(), GetHeight());
        glLoadIdentity();
        //  glScalef (1, -1, 1);
        glTranslatef(-1.0f, 1.0f, 0.0f );
        glScalef (2.0f/(float)GetWidth(), -2.0f/(float)GetHeight(), 1.0f);

        glMatrixMode (GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode (GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_RECTANGLE_ARB);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        nglCheckForGLErrors();
        glEnable(nglFBOSwapTarget);
        glDisable(GL_BLEND);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        int w = GetWidth();
        int h = GetHeight();

        if (!CheckExtension(_T("GL_ARB_texture_non_power_of_two")))
        {
          int ww = 1;
          while (ww < w)
            ww *= 2;
          int hh = 1;
          while (hh < w)
            hh *= 2;

          w = ww;
          h = hh;
        }

        float rx = (float)GetWidth() / (float)w;
        float ry = (float)GetHeight() / (float)h;

        glColor4f(1.0, 1.0, 1.0, 1.0f);
        nglCheckForGLErrors();
        glBegin(GL_QUADS);

        glTexCoord2f(0, ry);
        glVertex2i(0, 0);

        glTexCoord2f(rx, ry);
        glVertex2i(GetWidth(), 0);

        glTexCoord2f(rx, 0);
        glVertex2i(GetWidth(), GetHeight());

        glTexCoord2f(0, 0);
        glVertex2i(0, GetHeight());

        glEnd();

        glDisable(nglFBOSwapTarget);

        glMatrixMode (GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode (GL_TEXTURE);
        glPopMatrix();

        //glPopAttrib();

        nglCheckForGLErrors();

        SwapBuffers(mDC);
      }
      else if (mpPBuffer)
      {
        mpPBuffer->EndSession(mDC, mRC);
        SwapBuffers(mDC);
      }
      else
      {
        GLuint error = glGetError();
        MakeCurrent();
        int32 width  = GetWidth();
        int32 height = GetHeight();

        glViewport(0,0, width, height);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);

        if (mBackBufferTexture < 0)
        {
          glGenTextures(1, (GLuint*)&mBackBufferTexture);
          glBindTexture(GL_TEXTURE_2D, mBackBufferTexture);

          mBackTextureWidth = MakePOT(GetWidth());
          mBackTextureHeight = MakePOT(GetHeight());

          uint32 s = mBackTextureHeight * mBackTextureWidth * 3;
          char* tmp = new char[s];

          // Nasty copy from the back buffer
          glPixelStorei(GL_UNPACK_ALIGNMENT,1);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mBackTextureWidth, mBackTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tmp);

          delete tmp;
        }

        //if (!mSkipBackBufferCopy)
        {
          glBindTexture(GL_TEXTURE_2D, mBackBufferTexture);

          // Nasty copy from the back buffer
          glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
        }

        SwapBuffers( mDC );

        {
          glBindTexture(GL_TEXTURE_2D, mBackBufferTexture);
          //glBindTexture(GL_TEXTURE_2D, 6);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

          glMatrixMode(GL_TEXTURE);
          glLoadIdentity();
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();

          float w = (float)width / (float)mBackTextureWidth;
          float h = (float)height/ (float)mBackTextureHeight;

          glViewport(0,0, width, height);
          glEnable(GL_TEXTURE_2D);
          glDisable(GL_SCISSOR_TEST);
          glDisable(GL_DEPTH_TEST);
          glReadBuffer(GL_BACK);
          glDrawBuffer(GL_BACK);

          glColor3f(1.0f, 1.0f, 1.0f);

          glBegin(GL_QUADS);
          
            glTexCoord2f(0.0f, h);
            glVertex2f(0.0f, 0.0f);

            glTexCoord2f(w, h);
            glVertex2f((GLfloat)width, 0.0f);

            glTexCoord2f(w, 0.0f);
            glVertex2f((GLfloat)width, (GLfloat)height);

            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, (GLfloat)height);
          
          glEnd();
        }

        mSkipBackBufferCopy = false;

      }
    }
    else
    {
      SwapBuffers( mDC );
    }

  }
  else if (mTargetAPI == eTargetAPI_Direct3D)
  {
    NGL_ASSERT(mpDirect3DDevice);

//     hr = mpDirect3DDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA((uint8)(c.Red()*255.0f), (uint8)(c.Green()*255.0f), (uint8)(c.Blue()*255.0f), (uint8)(c.Alpha()*255.0f)));
// 
//     const DWORD VertexFVF =	D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
//     hr = mpDirect3DDevice->SetFVF(VertexFVF);
// 
//     // #FIXME 
//     hr = mpDirect3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, &rArray.GetVertices()[0], size * sizeof(nuiRenderArray::Vertex));				//draw triangle ( NEW )


    HRESULT hr = mpDirect3DDevice->EndScene();
    hr = mpDirect3DDevice->Present(NULL, NULL, NULL, NULL);
    NGL_ASSERT(S_OK == hr);
  }
}

void nglWindow::Invalidate()
{
  InvalidateRect(mHWnd,NULL,false);
}


#ifndef _NOGFX_
int ngl_scode_table[0x80] = {
/*  0 */ NK_ESC, NK_1, NK_2, NK_3, NK_4, NK_5, NK_6, NK_7, NK_8, 
/* 10 */ NK_9, NK_0, NK_MINUS, NK_EQUAL, NK_BACKSPACE, NK_TAB, NK_Q, NK_W, NK_E, NK_R, 
/* 20 */ NK_T, NK_Y, NK_U, NK_I, NK_O, NK_P, NK_LBRACKET, NK_RBRACKET, NK_ENTER, NK_LCTRL, 
/* 30 */ NK_A, NK_S, NK_D, NK_F, NK_G, NK_H, NK_J, NK_K, NK_L, NK_SEMICOLON,
/* 40 */ NK_APOSTROPHE, NK_GRAVE, NK_LSHIFT, NK_NUMBERSIGN, NK_Z, NK_X, NK_C, NK_V, NK_B, NK_N, 
/* 50 */ NK_M, NK_COMMA, NK_PERIOD, NK_SLASH, NK_RSHIFT, NK_PAD_ASTERISK, NK_LALT, NK_SPACE, NK_CAPSLOCK, NK_F1,
/* 60 */ NK_F2, NK_F3, NK_F4, NK_F5, NK_F6, NK_F7, NK_F8, NK_F9, NK_F10, NK_PAUSE,
/* 70 */ NK_SCRLOCK, NK_PAD_7, NK_PAD_8, NK_PAD_9, NK_PAD_MINUS, NK_PAD_4, NK_PAD_5, NK_PAD_6, NK_PAD_PLUS, NK_PAD_1,
/* 80 */ NK_PAD_2, NK_PAD_3, NK_PAD_0, NK_PAD_PERIOD, 0, 0, NK_BACKSLASH, NK_F11, NK_F12,  NK_HOME,
/* 90 */ NK_UP, NK_LMETA, NK_RMETA, NK_MENU, 0, 0, 0, 0, 0, 0,
/* 100 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 110 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 120 */ 0, 0, 0, 0, 0, 0, 0
};


void TranslateKey(WORD wParam,DWORD lParam,int *ascii, int* rawAscii, int *code, unsigned char keybstate[256])
{
  unsigned short buffer[2];
  unsigned int scode;

  scode=(lParam>>16)&127;
//  NGL_OUT(_T("wParam: 0x%x, lParam: 0x%x, scode: 0x%x\n"), wParam, lParam, scode);

//  nglDebug("\nWindows SCANCODE: %d\n",scode);
  if (ToUnicode(wParam,scode,keybstate,(LPWSTR)buffer,2,0)>0)
  {
#ifndef UNICODE
    *ascii = 0;
    WideCharToMultiByte(CP_ACP,0,(LPCWSTR)buffer,2,(char*)ascii,1,NULL,FALSE);
#else
    *ascii=*buffer;
#endif
  }
  else
    *ascii = 0;

    unsigned char emptykeybstate[256];
    memset(emptykeybstate, 0, sizeof(emptykeybstate));
    if (ToUnicode(wParam, scode, emptykeybstate, (LPWSTR)buffer, 2, 0))
    {
#ifndef UNICODE
      *rawAscii = 0;
      WideCharToMultiByte(CP_ACP,0,(LPCWSTR)buffer,2,(char*)rawAscii,1,NULL,FALSE);
#else
      *rawAscii=*buffer;
#endif
    }
    else
      *rawAscii = 0;
/*  for (int i=0;i<NGLKEYS;i++)
  {
    if (NGL2WINKEYS[i].winKey==wParam) 
    {
      *code=NGL2WINKEYS[i].nglKey;
      return;
    }
  }

  *code=*buffer;*/

  switch(wParam)
  {
  case VK_UP:
    *code=NK_UP;
    break;
  case VK_DOWN:
    *code=NK_DOWN;
    break;
  case VK_PRIOR:
    *code=NK_PAGEUP;
    break;
  case VK_NEXT:
    *code=NK_PAGEDOWN;
    break;
  case VK_LEFT:
    *code=NK_LEFT;
    break;
  case VK_RIGHT:
    *code=NK_RIGHT;
    break;
  case VK_DELETE:
    *code=NK_DELETE;
    break;
  case VK_HOME:
    *code=NK_HOME;
    break;
  case VK_END:
    *code=NK_END;
    break;
  default:
    *code=ngl_scode_table[scode-1];
    break;
  }

}

bool nglWindow::IsKeyDown (nglKeyCode Key) const
{
  if (Key == NK_META)
    return IsKeyDown(NK_LMETA) || IsKeyDown(NK_RMETA);
  else if (Key == NK_CTRL)
    return IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL);
  else if (Key == NK_SHIFT)
    return IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT);
  else if (Key == NK_ALT)
    return IsKeyDown(NK_LALT) || IsKeyDown(NK_RALT);
  
  
  if ((Key > 0) && (Key <= NGL_KEY_MAX))
    return mpKeyState[Key-1];
  else
    return false;
    
}

bool nglWindow::DoKey (bool IsUp, WPARAM wParam, LPARAM lParam)
{
  if (!(mEventMask & KeyEvents))
    return false;

  int ascii,rawAscii,code;
  unsigned char keybstate[256];
  GetKeyboardState(keybstate);

  keybstate[VK_LSHIFT] = IsKeyDown(NK_LSHIFT)?0xff:0;
  keybstate[VK_RSHIFT] = IsKeyDown(NK_RSHIFT)?0xff:0;
  keybstate[VK_SHIFT] = keybstate[VK_LSHIFT] | keybstate[VK_RSHIFT];
  keybstate[VK_LCONTROL] = IsKeyDown(NK_LCTRL)?0xff:0;
  keybstate[VK_RCONTROL] = IsKeyDown(NK_RCTRL)?0xff:0;
  keybstate[VK_CONTROL] = keybstate[VK_LCONTROL] | keybstate[VK_RCONTROL];

  TranslateKey((uint16)wParam, (DWORD)lParam, &ascii, &rawAscii, &code, keybstate);
  if (IsUp)
    return CallOnKeyUp(nglKeyEvent(code, ascii, rawAscii));
  else
    return CallOnKeyDown(nglKeyEvent(code, ascii, rawAscii));
}

void nglWindow::DoMouseUnclick( nglMouseInfo::Flags Flags, HWND hWnd, WPARAM wParam, LPARAM lParam )
{
  if (mMouseMode == nglMouseInfo::eRelative)
    lParam=0;
  else
    ReleaseCapture();
  mMouseButtonStatus &= ~Flags;

  if (!(mEventMask & MouseEvents))
    return;

  nglMouseInfo info;
  info.X = (int16)LOWORD(lParam);
  info.Y = (int16)HIWORD(lParam);
  info.Buttons = Flags;
  CallOnMouseUnclick(info);
  mOldMouseX = info.X;
  mOldMouseY = info.Y;
}

void nglWindow::DoMouseClick( nglMouseInfo::Flags Flags, HWND hWnd, WPARAM wParam, LPARAM lParam )
{
  EnableWindow(hWnd, TRUE);
  SetFocus(hWnd);
  if (GetActiveWindow() != hWnd)
  {
    SetActiveWindow(hWnd);
  }

  if (mMouseMode == nglMouseInfo::eRelative)
    lParam=0;
  else
    SetCapture(hWnd);
  mMouseButtonStatus &= ~Flags;

  if (!(mEventMask & MouseEvents))
    return;

  nglMouseInfo info;
  info.X = (int16)LOWORD(lParam);
  info.Y = (int16)HIWORD(lParam);
  info.Buttons = Flags;
  CallOnMouseClick(info);
  mOldMouseX = info.X;
  mOldMouseY = info.Y;
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  nglWindow* win = NULL;

  switch ( message ) 
  {
  case WM_CREATE:
    {
      CREATESTRUCT *cs = (CREATESTRUCT*) lParam;
      win = (nglWindow*)cs->lpCreateParams;
      SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)win);
      NGL_OUT(_T("WM_NCCREATE: 0x%x 0x%x '%s'\n"), hWnd, win, cs->lpszName);
      BOOL res = PostMessage(hWnd, nglWindow::WM_CREATED, 0, 0);
      if (!res)
      {
        DWORD qs = GetQueueStatus(QS_ALLEVENTS);
        DWORD dwError = GetLastError();
        TCHAR *errBuf = 0;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errBuf,0,NULL);
        OutputDebugString(errBuf);
        //Free(errBuf);
        return FALSE;
      }
  
      return TRUE;
    }
    break;

  default:
    {
      win = (nglWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
      if (!win)
      {
        NGL_ASSERT(message != nglWindow::WM_CREATED);
        NGL_OUT(_T("SKIPPED MSG: 0x%x 0x%x\n"), hWnd, message);
      }
      return win ?
        win->WndProc(hWnd, message, wParam, lParam) :
        DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
}

std::set<HWND> NGL_Windows;
HHOOK NGL_MsgHook = NULL;

LRESULT CALLBACK KbdMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
  KBDLLHOOKSTRUCT* pMsg = (KBDLLHOOKSTRUCT*)lParam;
  HWND hwnd1 = GetActiveWindow();
  HWND hwnd2 = GetFocus();
  if (NGL_Windows.find(hwnd2) != NGL_Windows.end()) // Our window is having the focus
  {
    //NGL_OUT(_T("KbdMsgProc sending keyboard event for our window 0x%x\n"), hwnd2);

    WPARAM msg = wParam;
    DWORD wParam = pMsg->vkCode;
    DWORD lParam = pMsg->scanCode << 16;
    /*
    wParam
      Specifies the virtual-key code of the nonsystem key. 
    lParam
      Specifies the repeat count, scan code, extended-key flag, context code, previous key-state flag, 
      and transition-state flag, as shown in the following table. 
        0-15:   Specifies the repeat count for the current message. The value is the number of times the keystroke
                is autorepeated as a result of the user holding down the key. If the keystroke is held long enough,
                multiple messages are sent. However, the repeat count is not cumulative.
        16-23:  Specifies the scan code. The value depends on the OEM.
        24:     Specifies whether the key is an extended key, such as the right-hand ALT and CTRL keys that appear 
                on an enhanced 101- or 102-key keyboard. The value is 1 if it is an extended key; otherwise, it is 0.
        25-28:  Reserved; do not use.
        29:     Specifies the context code. The value is always 0 for a WM_KEYDOWN message.
        30:     Specifies the previous key state. The value is 1 if the key is down before the message is sent, or
                it is zero if the key is up.
        31:     Specifies the transition state. The value is always zero for a WM_KEYDOWN message.
    */

    switch (pMsg->vkCode)
    {
      // Some key change the keyboard mode and wee need to let them pass to the application:
    case VK_CAPITAL:
    case VK_KANA:
    case VK_JUNJA:
    case VK_FINAL:
    case VK_HANJA:
    case VK_NUMLOCK:
    case VK_CONVERT:
    case VK_NONCONVERT:
    case VK_ACCEPT:
    case VK_MODECHANGE:
      {
        LRESULT res = CallNextHookEx(NGL_MsgHook, code, wParam, lParam);
        WndProc(hwnd2, msg, wParam, lParam);
        return res;
      }
      break;
    default:
      if (!WndProc(hwnd2, msg, wParam, lParam))
        return 1;
    }
  }
  //NGL_OUT(_T("KbdMsgProc going to next hook\n"));
  return CallNextHookEx(NGL_MsgHook, code, wParam, lParam);
}

LRESULT nglWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  uint click = 0;

  switch ( message ) 
  {
  case WM_GETDLGCODE:
    {
      return DLGC_WANTALLKEYS;
    }
    break;

  case WM_SIZE:
    {
      // resize the child window:
      RECT R;
      GetClientRect(mHWnd, &R);
      MoveWindow(mGLHWnd, 0, 0, R.right - R.left, R.bottom - R.top, true);
    }
    if (!IsIconic(hWnd))
    {
      if (mMouseMode == nglMouseInfo::eRelative)
      {
        mOldMouseX=(int16)LOWORD(lParam)/2;
        mOldMouseY=(int16)HIWORD(lParam)/2;
        SetCursorPos(mOldMouseX,mOldMouseX);
      }
      if (mValidBackBufferRequestedNotGranted)
      {
        if (CheckExtension(_T("GL_EXT_framebuffer_object")) && mFBO_BackBufferTexture)
        {
          MakeCurrent();
          if (mFBO_BackBuffer)
            glDeleteFramebuffersEXT(1, &mFBO_BackBuffer);
          if (mFBO_BackBufferTexture)
            glDeleteTextures(1, &mFBO_BackBufferTexture);
          if (mFBO_BackBufferDepth)
            glDeleteRenderbuffersEXT(1, &mFBO_BackBufferDepth);
          if (mFBO_BackBufferStencil)
            glDeleteRenderbuffersEXT(1, &mFBO_BackBufferStencil);
          mFBO_BackBuffer = 0;
          mFBO_BackBufferTexture = 0;
          mFBO_BackBufferStencil = 0;
          mFBO_BackBufferDepth = 0;
        }
        
        delete mpPBuffer;
        mpPBuffer = NULL;

        if (mBackBufferTexture >= 0)
        {
          glDeleteTextures(1, (GLuint*)&mBackBufferTexture);
          mBackBufferTexture = -1;
          mSkipBackBufferCopy = true;
        }
      }

      CallOnResize((int16)LOWORD(lParam),(int16)HIWORD(lParam));
      CallOnPaint();
    }
    break;

  case WM_LBUTTONDBLCLK:
    click = nglMouseInfo::ButtonDoubleClick;
  case WM_LBUTTONDOWN:
    DoMouseClick(nglMouseInfo::ButtonLeft | click, hWnd, wParam, lParam);
    break;

  case WM_LBUTTONUP:
    DoMouseUnclick(nglMouseInfo::ButtonLeft, hWnd, wParam, lParam);
    break;

  case WM_MBUTTONDBLCLK:
    click = nglMouseInfo::ButtonDoubleClick;
  case WM_MBUTTONDOWN:
    DoMouseClick(nglMouseInfo::ButtonMiddle | click, hWnd, wParam, lParam);
    break;

  case WM_MBUTTONUP:
    DoMouseUnclick(nglMouseInfo::ButtonMiddle, hWnd, wParam, lParam);
    break;

  case WM_RBUTTONDBLCLK:
    click = nglMouseInfo::ButtonDoubleClick;
  case WM_RBUTTONDOWN:
    DoMouseClick(nglMouseInfo::ButtonRight | click, hWnd, wParam, lParam);
    break;

  case WM_RBUTTONUP:
    DoMouseUnclick(nglMouseInfo::ButtonRight, hWnd, wParam, lParam);
    break;

  case WM_MOUSEWHEEL:
    {
      if (!(mEventMask & MouseEvents))
        return 0;

      nglMouseInfo info;
      POINT point;
      GetCursorPos(&point);
      ScreenToClient(hWnd,&point);
      if (mMouseMode == nglMouseInfo::eRelative)
      { 
        point.x = 0;
        point.y = 0;
      }
      info.X = point.x;
      info.Y = point.y;
      info.Buttons = (wParam>>16)<32768 ? nglMouseInfo::ButtonWheelUp : nglMouseInfo::ButtonWheelDown;

      mMouseButtonStatus |= info.Buttons;
      CallOnMouseClick(info);
      mMouseButtonStatus &= ~info.Buttons;
      CallOnMouseUnclick(info);
      mOldMouseX = info.X;
      mOldMouseY = info.Y;
      return 0;
    }
    break;

  case WM_MOUSEHWHEEL:
    {
      if (!(mEventMask & MouseEvents))
        return 0;

      nglMouseInfo info;
      POINT point;
      GetCursorPos(&point);
      ScreenToClient(hWnd,&point);
      if (mMouseMode == nglMouseInfo::eRelative)
      { 
        point.x = 0;
        point.y = 0;
      }
      info.X = point.x;
      info.Y = point.y;
      info.Buttons = (wParam>>16)<32768 ? nglMouseInfo::ButtonWheelRight : nglMouseInfo::ButtonWheelLeft;

      mMouseButtonStatus |= info.Buttons;
      CallOnMouseClick(info);
      mMouseButtonStatus &= ~info.Buttons;
      CallOnMouseUnclick(info);
      mOldMouseX = info.X;
      mOldMouseY = info.Y;
      return 0;
    }
    break;

  case WM_MOUSEMOVE:
    {
      nglMouseInfo info;

      if (mMouseMode == nglMouseInfo::eRelative)
      {
        POINT p;

        if (GetActiveWindow()!=mHWnd)
          return 0;

        p.x=(int16)LOWORD(lParam);
        p.y=(int16)HIWORD(lParam);
        info.X = p.x - mOldMouseX;
        info.Y = p.y - mOldMouseY;
        if (info.X || info.Y)
        {
          p.x = mOldMouseX;
          p.y = mOldMouseY;
          ClientToScreen(hWnd, &p);
          SetCursorPos(p.x, p.y);
        }
      }
      else
      {
        info.X = (int16)LOWORD(lParam);
        info.Y = (int16)HIWORD(lParam);
      }
      info.Buttons = mMouseButtonStatus;
      if ((mEventMask & MouseEvents) && ((mOldMouseX != info.X) || (mOldMouseY != info.Y)))
        CallOnMouseMove(info);
      mOldMouseX = info.X;
      mOldMouseY = info.Y;
      return 0;
    }
    break;

#if 0
  // case WM_GESTURE: // Panning / Zooming , etc.
  case WM_TOUCH:
    {
      UINT cInputs = LOWORD(wParam);
      PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
      if (NULL != pInputs)
      {
        if (GetTouchInputInfo((HTOUCHINPUT)lParam,
          cInputs,
          pInputs,
          sizeof(TOUCHINPUT)))
        {
          // process pInputs
          if (!CloseTouchInputHandle((HTOUCHINPUT)lParam))
          {
            // error handling
          }
        }
        else
        {
          // GetLastError() and error handling
        }
      }
      else
      {
        // error handling, presumably out of memory
      }
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
#endif

  case WM_SETFOCUS:
    {
      if (mMouseMode == nglMouseInfo::eRelative)
        ShowCursor(FALSE);
      CallOnActivation();
    }
    break;
  case WM_KILLFOCUS:
    {
      if (mMouseMode == nglMouseInfo::eRelative)
        ShowCursor(TRUE);
      CallOnDesactivation();
    }
    break;

  case WM_PAINT:
    {
      if (!IsIconic(hWnd))
      {
#if 1
        ValidateRect(mHWnd, NULL);
        CallOnPaint();
#else
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(mHWnd, &ps);
        CallOnPaint();
        EndPaint(mHWnd, &ps);
#endif
      }
    }
    break;
  case WM_ERASEBKGND:
    return 1;
    break;

  case WM_CLOSE:
//    NGL_OUT(_T("Close\n"));
    CallOnClose();
    break;
  case WM_DESTROY:
//    NGL_OUT(_T("Destruction\n"));
    CallOnDestruction();
    NGL_Windows.erase(hWnd);
    if (NGL_Windows.empty())
    {
      UnhookWindowsHookEx(NGL_MsgHook);
      NGL_MsgHook = NULL;
    }
    break;

  case WM_CHAR:
    {
//NGL_OUT(_T("WM_CHAR %d %d\n"), wParam, lParam);
      if (wParam>31)
      {
        nglString str((nglChar)wParam);
        
        return CallOnTextInput(str) ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
      }
    }
    break;
  case WM_SYSKEYDOWN:
    {
//NGL_OUT(_T("WM_SYSKEYDOWN %d %d\n"), wParam, lParam);
      bool res = DoKey(false, wParam, lParam);
      return res ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
    }                                                                                   
    break;
  case WM_KEYDOWN:
    {
//NGL_OUT(_T("WM_KEYDOWN %d %d\n"), wParam, lParam);
      int scancode = (lParam & 0xFF0000) >> 16;
      if (scancode == 0x29 && GetKeyState(VK_CONTROL))
      {
        App->GetConsole().Show(true);
        return 0;
      }
      bool res = DoKey(false, wParam, lParam);
      return res ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
  case WM_SYSKEYUP:
    {
//NGL_OUT(_T("WM_SYSKEYUP %d %d\n"), wParam, lParam);
      bool res = DoKey(true, wParam, lParam);
      return res ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
  case WM_KEYUP:
    {
//NGL_OUT(_T("WM_KEYUP %d %d\n"), wParam, lParam);
      bool res = DoKey(true, wParam, lParam);
      return res ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;

  case WM_SYSCOMMAND:
    {
      if (wParam==MENU_SHOWCONSOLE_ID)
      {
        App->GetConsole().Show(true);
        return 0;
      }
      else
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;

  case WM_SETCURSOR: 
    {
      POINT point;
      RECT rect;
      GetCursorPos(&point);
      ScreenToClient(hWnd, &point);
      GetClientRect(mHWnd, &rect);
      
      if (!IsIconic(hWnd) && PtInRect(&rect, point)) 
      {
        HCURSOR c = GetCursorIDC(mCursor);
        if (c)
        {
          ::SetCursor(c);
          return TRUE;
        }
      } 
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;


  case WM_COMMAND:
    {
      //NGL_OUT(_T("WM_COMMAND wParam 0x%x  lParam 0x%x\n"), wParam, lParam);  

      // for now, we only care about menu commands, sent from nuiMainMenuItem(s)
      if (lParam != 0)
        break;

      nuiMainMenuItem* pItem = (nuiMainMenuItem*)wParam;
      NGL_ASSERT(pItem);

      // check/uncheck the item if the flag has been set
      if (pItem->GetType() == eItemCheck)
        pItem->SetChecked(!pItem->IsChecked());
      
      // sent activated event from the menu item object.
      // it's to be connected in the user's application
      pItem->Activated();
    }
    break;

//   case WM_IME_CHAR:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
  case WM_IME_COMPOSITION:
    {
      if (lParam & GCS_COMPSTR || lParam & GCS_CURSORPOS) 
      {
        HIMC hIMC;
        DWORD dwSize;
        HGLOBAL hstr;
        LPSTR lpstr;

        hIMC = ImmGetContext(mHWnd);

        if (!hIMC)
        {
          return FALSE;
        }

        // Get the size of the result string.
        dwSize = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0);
        NGL_OUT(_T("Composition string size: %d\n"), dwSize);
        // increase buffer size for terminating null character, 
        //   maybe it is in UNICODE
        dwSize += sizeof(WCHAR);

        hstr = GlobalAlloc(GHND,dwSize);
        NGL_ASSERT(hstr != NULL);

        lpstr = (LPSTR)GlobalLock(hstr);
        NGL_ASSERT(lpstr != NULL)
        memset(lpstr, 0, dwSize);

        // Get the result strings that is generated by IME into lpstr.
        ImmGetCompositionString(hIMC, GCS_COMPSTR, lpstr, dwSize);
        NGL_OUT(_T("Composition string result: %s\n"), lpstr);
        ImmReleaseContext(hWnd, hIMC);

        // Get cursor position:
        UINT pos = 0;
        DWORD s = sizeof(pos);
        ImmGetCompositionString(hIMC, GCS_CURSORPOS, &pos, s);
        NGL_OUT(_T("Composition string cursor pos: %d\n"), pos);

        // add this string into text buffer of application
        nglString str((nglChar*)lpstr);
        OnTextCompositionUpdated(str, pos);

        GlobalUnlock(hstr);
        GlobalFree(hstr);

        float x = 0, y = 0;
        OnTextCompositionIndexToPoint(0, x, y);
        COMPOSITIONFORM Form;
        Form.dwStyle = CFS_POINT;
        Form.ptCurrentPos.x = x;
        Form.ptCurrentPos.y = y;
        ImmSetCompositionWindow(hIMC, &Form);

      }
      else if (!lParam)
      {
        OnTextCompositionCanceled();
      }
      return TRUE;
    }
    break;
//   case WM_IME_COMPOSITIONFULL:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
//   case WM_IME_CONTROL:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
  case WM_IME_ENDCOMPOSITION:
    OnTextCompositionConfirmed();
    return TRUE;
    break;
//   case WM_IME_KEYDOWN:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
//   case WM_IME_KEYUP:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
//   case WM_IME_NOTIFY:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
//   case WM_IME_SELECT:
//     return DefWindowProc( hWnd, message, wParam, lParam );
//     break;
  case WM_IME_SETCONTEXT:
    {
      float x = 0, y = 0;
      OnTextCompositionIndexToPoint(0, x, y);
      COMPOSITIONFORM Form;
      Form.dwStyle = CFS_POINT;
      Form.ptCurrentPos.x = x;
      Form.ptCurrentPos.y = y;
      HIMC imc = ImmGetContext(hWnd);
      ImmSetCompositionWindow(imc, &Form);
      ImmReleaseContext(hWnd, imc);
      lParam = lParam & (~ISC_SHOWUICANDIDATEWINDOW);
    }
    return DefWindowProc(hWnd, WM_IME_SETCONTEXT, wParam, lParam);
    break;
  case WM_IME_STARTCOMPOSITION:
    OnTextCompositionStarted();
    return TRUE;
    break;
  case WM_IME_REQUEST:
    {
      switch (wParam)
      {
      case IMR_CANDIDATEWINDOW:
        {
          CANDIDATEFORM* pForm = (CANDIDATEFORM*)lParam;
          float x = 0, y = 0;
          OnTextCompositionIndexToPoint(0, x, y);
          pForm->dwIndex = 0;
          pForm->dwStyle = CFS_CANDIDATEPOS;
          pForm->ptCurrentPos.x = x;
          pForm->ptCurrentPos.y = y;
          return TRUE;
        }
        break;
      case IMR_COMPOSITIONFONT:
        break;
      case IMR_CONFIRMRECONVERTSTRING:
        break;
      case IMR_DOCUMENTFEED:
        break;
      case IMR_QUERYCHARPOSITION:
        break;
      case IMR_RECONVERTSTRING:
        break;

      case IMR_COMPOSITIONWINDOW:
        {
          COMPOSITIONFORM* pForm = (COMPOSITIONFORM*)lParam;
          float x = 0, y = 0;
          OnTextCompositionIndexToPoint(0, x, y);
          pForm->dwStyle = CFS_POINT;
          pForm->ptCurrentPos.x = x;
          pForm->ptCurrentPos.y = y;
          return TRUE;
        }
        break;
      }
      return DefWindowProc( hWnd, message, wParam, lParam );
    }
    break;

//  case WM_NCHITTEST:
//    return HTCLIENT;

  default:
    {
      if (message == WM_CREATED)
      {
        NGL_OUT(_T("Received WM_CREATED: 0x%x 0x%x '%s'\n"), hWnd, this, GetTitle().GetChars());
        CallOnCreation();

        NGL_Windows.insert(hWnd);

        if (!NGL_MsgHook)
        {
          HMODULE dll = NULL;
          GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)KbdMsgProc, &dll);
          if (dll && dll != GetModuleHandle(NULL)) // The dll module is valid if it is != NULL and if it is not the .exe that created this process
          {
            NGL_MsgHook = SetWindowsHookEx(WH_KEYBOARD_LL, KbdMsgProc, dll, 0);
          }
        }
        CallOnPaint();
        return 0;
      }
      else
      {
//         if (mDragMessageId == message)
//         {
//           if (wParam == NGL_GET_DATA_MESSAGE)
//           {
//             nglString mime;
//             FORMATETC * pFormat = (FORMATETC*)lParam;
//             NGL_ASSERT(pFormat);
// 
//             if (App->GetDataTypesRegistry().GetRegisteredMimeType(pFormat->cfFormat, mime))
//             {
//               OnDragRequestData(mpDropSource->GetDraggedObject(), mime);
//             }
//           }
//          else
//           if (wParam == NGL_STOP_DRAGGING)
//           {
//             bool canceled = (lParam != 0) ? true : false;
//             OnDragStop(canceled);
//           }
//           return 0;
//         }
//         else
          return DefWindowProc( hWnd, message, wParam, lParam );
      }
    }
  }
  return 0;
}

#endif // _NOGFX_

nglDropEffect nglWindow::OnCanDrop(nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button)
{
  //NGL_OUT(_T("nglWindow::OnCanDrop() Button:%s, X:%d Y:%d\n"), Button & nglMouseInfo::ButtonLeft ? "Left" : Button & nglMouseInfo::ButtonRight? "Right" : "Middle", X, Y);
  return eDropEffectNone;
}
void nglWindow::OnDragEnter()
{
  //NGL_OUT(_T("nglWindow::OnDragEnter()\n"));
}

void nglWindow::OnDropped(nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button)
{
  //NGL_OUT(_T("nglWindow::OnDropped()\n"));
}

void nglWindow::OnDragLeave()
{
  //NGL_OUT(_T("nglWindow::OnDragLeave()\n"));
}


bool nglWindow::Drag(nglDragAndDrop* pDragObject)
{
  mOnDragging = true;
  DWORD id;
  
  mpDropSource->SetDraggedObject(pDragObject);
  
//  SetEventMask( NoEvents );
//  HANDLE handle = CreateThread(NULL, 0, DoDragDropThreadCallback, mpDropSource, 0, &id);

  bool dragged = mpDropSource->Drag(); ///< blocks until drop occurs or is canceled
  bool canceled = !dragged;

  //SendMessage(pDropSource->GetWindowHandle(), pDropSource->GetMessageId(), NGL_STOP_DRAGGING, (LPARAM)canceled);
  OnDragStop(canceled);

  delete pDragObject;
  mpDropSource->ClearObjects();

  return true;
}


void nglWindow::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
  ///< Has to fill data for supported types
  //SetEventMask( AllEvents );
}

void nglWindow::OnDragStop(bool canceled)
{
  SetEventMask( AllEvents );
}

bool IsIidFromList(REFIID riid, const IID* InterfaceIds, uint32 interfaceCount)
{
  for (uint32 i = 0; i < interfaceCount; i++)
  {
    if (InterfaceIds[i] == riid)
      return true;
  }
  return false;
}

nglDropSource::nglDropSource(nglWindow* pWindow) : mpWindow(pWindow) 
{ 
  mRef = 0; 
  mInterfaceIds[0] = IID_IUnknown;
  mInterfaceIds[1] = IID_IDropSource;
  mInterfaceCount = 2;

  mpDraggedObject = NULL;
  mpIDataObject = NULL;
  mpIDataObject = new nglIDataObject(pWindow);
  mpIDataObject->AddRef();
}

nglDropSource::~nglDropSource() 
{ 
  ULONG res = mpIDataObject->Release();
  NGL_ASSERT(!res);
  NGL_ASSERT(!mRef); 
}


void nglDropSource::SetDraggedObject(nglDragAndDrop* pDraggedObject)
{
  mpDraggedObject = pDraggedObject;
}

nglDragAndDrop* nglDropSource::GetDraggedObject() const
{
  return mpDraggedObject;
}

// void nglDropSource::SetMessageId(UINT MessageId)
// {
//   mMessageId = MessageId;
// }
// 
// UINT nglDropSource::GetMessageId() const
// {
//   return mMessageId;
// }

void nglDropSource::SetWindowHandle(HWND HWnd)
{
  mHWnd = HWnd;
}

HWND nglDropSource::GetWindowHandle() const
{
  return mHWnd;
}

nglDropTarget::nglDropTarget(HWND pHwnd, nglWindow* pWindow) 
: mpHwnd(pHwnd), mpWindow(pWindow)
{ 
  mRef = 0; 
  mInterfaceIds[0] = IID_IUnknown;
  mInterfaceIds[1] = IID_IDropTarget;
  mInterfaceCount = 2;
}

nglDropTarget::~nglDropTarget()
{
  NGL_ASSERT(!mRef);
}

bool nglDropTarget::InitOleDrop()
{
  HRESULT res;

  res = OleInitialize(NULL);
  res = RegisterDragDrop(mpHwnd, this);
  if (res != S_OK)
    return false;

  return true;
}
bool nglDropTarget::UnInitOleDrop()
{
  HRESULT res;
  res = RevokeDragDrop(mpHwnd);
  OleUninitialize();
  if (res != S_OK)
    return false;
  return true;
}

nglEnumFormat::nglEnumFormat()
{
  mRef = 0;
  mInterfaceIds[0] = IID_IUnknown;
  mInterfaceIds[1] = IID_IEnumFORMATETC;
  mInterfaceCount = 2;
}

nglEnumFormat::~nglEnumFormat()
{
  Clear();
  NGL_ASSERT(!mRef);
}


void SetDropEffect(nglDropEffect dropEffect, DWORD* pEffect)
{
  switch (dropEffect)
  {
  case eDropEffectNone:
    *pEffect |= DROPEFFECT_NONE;
    break;
  case eDropEffectCopy:
    *pEffect |= DROPEFFECT_COPY;
    break;
  case eDropEffectMove:
    *pEffect |= DROPEFFECT_MOVE;
    break;
  case eDropEffectLink:
    *pEffect |= DROPEFFECT_LINK;
    break;
  case eDropEffectScroll:
    *pEffect |= DROPEFFECT_SCROLL;
    break;
  default:
    break;
  }
}

nglDropEffect GetDropEffect(DWORD* pEffect)
{
  switch (*pEffect)
  {
  case DROPEFFECT_NONE:
    return eDropEffectNone;
    break;
  case DROPEFFECT_COPY:
    return eDropEffectCopy;
    break;
  case DROPEFFECT_MOVE:
    return eDropEffectMove;
    break;
  case DROPEFFECT_LINK:
    return eDropEffectLink;
    break;
  case DROPEFFECT_SCROLL:
    return eDropEffectScroll;
    break;
  default:
    break;
  }
  return eDropEffectNone;
}

void GetDropEffects(nglDragAndDrop* pObj, DWORD* pEffect)
{
  pObj->ClearSupportedDropEffects();

  if ( *pEffect & DROPEFFECT_NONE )
    pObj->AddSupportedDropEffect(eDropEffectNone);
  if ( *pEffect & DROPEFFECT_COPY )
    pObj->AddSupportedDropEffect(eDropEffectCopy);
  if ( *pEffect & DROPEFFECT_MOVE )
    pObj->AddSupportedDropEffect(eDropEffectMove);
  if ( *pEffect & DROPEFFECT_LINK )
    pObj->AddSupportedDropEffect(eDropEffectLink);
  if ( *pEffect & DROPEFFECT_SCROLL )
    pObj->AddSupportedDropEffect(eDropEffectScroll);
}

void nglWindow::EnterModalState()
{
  uint32 storeModalState = mInModalState;
  mInModalState++;
  NGL_OUT(_T("nglWindow::EnterModalState() [%p - %d]"), this, mInModalState);
  if (GetState() == eHide || GetState() == eMinimize)
    SetState(eShow);

  MSG msg;

  HWND mainhwnd = App->GetHWnd();

  do
  {
    // Process (rest of) msg queue
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && msg.message != WM_QUIT && (mInModalState > storeModalState))
    {
      ATOM atom = GetClassLong(msg.hwnd, GCW_ATOM);
      bool ok = true;

      // Only skip user events for the nglWindow windows that is not the current one by
      // checking the class of the window (i.e. the ATOM that was allocated by RegisterClass);
      if (msg.hwnd != mHWnd && mAtom == atom)
      {
        if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
        {
          ok = false;
        }
        else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
        {
          ok = false;
        }
      }

      if (ok)
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      //OutputDebugStr(_T("!"));
    }
    //OutputDebugStr(_T("#"));
  } while (msg.message != WM_QUIT && (mInModalState > storeModalState));
  
  if (msg.message == WM_QUIT)
  {
    PostQuitMessage(msg.wParam);
  }

  NGL_OUT(_T("nglWindow::EnterModalState(DONE) [%p - %d]"), this, mInModalState);
}


void nglWindow::ExitModalState()
{
  NGL_ASSERT(mInModalState > 0);
  mInModalState--;
  NGL_OUT(_T("nglWindow::ExitModalState() [%p - %d]"), this, mInModalState);
}

void nglWindow::StartTextInput(int32 X, int32 Y, int32 W, int32 H)
{
  //#FIXME
}

void nglWindow::EndTextInput()
{
  //#FIXME
}

bool nglWindow::IsEnteringText() const
{
  //#FIXME
  return false;
}

