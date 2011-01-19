/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#include "nglKernel.h"
#include "nglString.h"
#include "nglConsole.h"
#include "nglTimer.h"
#include "nglDeviceInfo.h"
#include "ngl_winXX.h"
#include <locale.h>
#include <float.h>
#include <dbt.h>


// This is for stlport to include its template code definitions in the ngl dll:
#include <string>

const nglChar* gpKernelErrorTable[] =
{
  /*  0 */ _T("No error"),
  NULL
};


/*
 * Life cycle
 */

extern void nglInitExceptionHandler();

nglKernel::nglKernel()
: mKernelEventSink(this)
{
#ifdef _DEBUG_
#if (defined WIN32) && (!defined _WIN64)
  nglInitExceptionHandler();
#endif
#endif

  Init();

  mHWnd = NULL;
  mHInstance = NULL;
  mSystemFont = NULL;
  mShowConsole = false;

  mpDirect3D = NULL;
}

nglKernel::~nglKernel()
{
  Exit(0);
  SelectFont(mHDC, NULL);
  if (mSystemFont)
    DeleteFont(mSystemFont);

  if (mpDirect3D)
    mpDirect3D->Release();

  if (mHWnd) 
  {
    DestroyWindow(mHWnd);
  }
  mHWnd = NULL;
  mHDC = NULL;

  if (mHInstance)
  {
    UnregisterClass(NGL_CORE_CLASS, mHInstance);
  }
}


/*
 * Clipboard
 */

void nglKernel::GetClipboard(nglString& rClipboard)
{
  nglString string;
  HGLOBAL hglb;
  char* lptstr;

  if (!IsClipboardFormatAvailable(CF_TEXT)) 
  {
    rClipboard = string;
    return;
  }
  if (!OpenClipboard(mHWnd)) 
  {
    rClipboard = string;
    return;
  }

  hglb = GetClipboardData(CF_TEXT); 
  if (hglb != NULL) 
  { 
    lptstr = (char*)GlobalLock(hglb); 
    if (lptstr != NULL) 
    { 
      string=lptstr; 
      GlobalUnlock(hglb); 
    } 
  } 
  CloseClipboard(); 

  rClipboard = string;
}

bool nglKernel::SetClipboard(const nglString& rString)
{
  if (OpenClipboard(mHWnd))
  {
    EmptyClipboard();

    HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, rString.GetLength()+1); 
    if (hglbCopy == NULL) 
    { 
      CloseClipboard(); 
      return false; 
    } 

    char* lptstrCopy = (char*)GlobalLock(hglbCopy); 
    memcpy(lptstrCopy, rString.GetChars(), rString.GetLength()+1); 
    GlobalUnlock(hglbCopy); 

    SetClipboardData(CF_TEXT, hglbCopy); 
    CloseClipboard();
    return true;
  }
  return false;
}

HINSTANCE nglKernel::GetHInstance() const
{
  return mHInstance;
}


/*
 * Win32 (protected) interface
 */

HWND nglKernel::GetHWnd() const
{
  return mHWnd;
}

HDC nglKernel::GetDC() const
{
  return mHDC;
}

HFONT nglKernel::GetSystemFont() const
{
  if (!mSystemFont)
  {
    mSystemFont = CreateFont(
      SYSTEMFONT_SIZEX,         // logical height of font
      0,                        // logical average character width
      0,                        // angle of escapement
      0,                        // base-line orientation angle
      FW_DONTCARE,              // font weight
      FALSE,                    // italic attribute flag
      FALSE,                    // underline attribute flag
      FALSE,                    // strikeout attribute flag
      DEFAULT_CHARSET,          // character set identifier
      OUT_DEFAULT_PRECIS,       // output precision
      CLIP_DEFAULT_PRECIS,      // clipping precision
      DEFAULT_QUALITY,          // output quality
      FF_MODERN + FIXED_PITCH,  // pitch and family
      SYSTEMFONT_NAME           // pointer to typeface name string
      );
  }

  return mSystemFont;
}



/*
 * System init & main loop
 */

bool nglKernel::SysInit (HINSTANCE Inst)
{
  NGL_ASSERTR(mHInstance == NULL, false);

  mHInstance = Inst;

  // Import current host/user locale
  setlocale(LC_ALL, ""); // FIXME: this call creates a bunch of memory leaks, or so boundchecker says...

  // Tell the system not to bother the user with file and path error dialogs, fail in the code instead.
  SetErrorMode(SEM_FAILCRITICALERRORS);

#ifdef HAVE_MLANG
  /* If nglStringConv relies on MLang, we need to initialize COM state.
   * See nglStringConv.cpp for more info.
   */
  CoInitialize(NULL);
#endif

  /* Register our core window class
   */
  WNDCLASSEX dummywc;
  dummywc.cbSize = sizeof(WNDCLASSEX);
  if (!GetClassInfoEx( Inst, NGL_CORE_CLASS, &dummywc))
  {
    WNDCLASS wc;

    wc.style         = CS_DBLCLKS | CS_OWNDC;
    wc.lpfnWndProc   = MainWndProcStatic;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = mHInstance;
    wc.hIcon         = LoadIcon( GetHInstance(), _T("0") );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = NGL_CORE_CLASS;

    if (!RegisterClass( &wc ))
    {
      NGL_DEBUG( OutputDebugString(_T("NGL: error: unable to create ") NGL_CORE_CLASS _T(" window class\n")); )
      return false;
    }
  }

/*
#ifndef _NOGFX_
  wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.hbrBackground = NULL;
  wc.lpfnWndProc   = WndProc;
  wc.lpszClassName = NGL_WINDOW_CLASS;
  if (!RegisterClass( &wc ))
  {
    NGL_DEBUG( OutputDebugString("NGL: error: unable to create " NGL_WINDOW_CLASS " window class\n"); )
    return false;
  }
#endif
*/

  /* Create application's main window
   */
  HWND hwnd;
  
  hwnd = CreateWindow (
    NGL_CORE_CLASS,
    _T("NGL"),
    WS_OVERLAPPEDWINDOW,
    0, 0, 320, 200,
    NULL,
    NULL,
    mHInstance,
    NULL
    );

  //GetLog().Log(_T("nglKernel"), NGL_LOG_INFO, _T("Create ngl kernel window hwnd = 0x%x"), hwnd);

  if (hwnd == NULL)
  {
    NGL_DEBUG( OutputDebugString(_T("NGL: error: unable to create main window\n")); )
    return false;
  }

  mHWnd = hwnd;
  UpdateWindow( mHWnd );
  ShowWindow( mHWnd, SW_HIDE );

  mHDC = ::GetDC(mHWnd);

  return true;
}


/* The Main Window proc is needed for some stuff such as
 * DirectInput, timers or network.
 */
LRESULT CALLBACK nglKernel::MainWndProcStatic (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  /* This is the static stub, let's redirect to the right instance's code (3 lines further)
   */
  return App->MainWndProc(hWnd, message, wParam, lParam);
}

bool nglGetDriveInfo(nglChar* name, nglPathVolume& rVol);

LRESULT nglKernel::MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_NGLTIMER)
  {
    return nglTimer::WndProc(hWnd, message, wParam, lParam);
  }

  switch ( message ) 
  {

    case WM_DEVICECHANGE:
      {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

        switch (wParam)
        {
        case DBT_DEVICEARRIVAL:
          {
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
              PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

              //if (lpdbv -> dbcv_flags & DBTF_MEDIA) // We want to be told about ALL volumes
              {
                UINT mask = lpdbv ->dbcv_unitmask;
                UINT dev = 0;
                while (mask)
                {
                  if (mask & 1)
                  {
                    nglChar name[4];
                    name[0] = dev + 'A';
                    name[1] = ':';
                    name[2] = '/';
                    name[3] = 0;

                    nglPathVolume vol;
                    nglGetDriveInfo(name, vol);
                    
                    nglPathVolumeDeviceInfo Dev(vol);

                    OnDeviceAdded(&Dev);
                  }
                  dev++;
                  mask >>= 1;
                }
              }
            }

          }
          break;
        case DBT_DEVICEREMOVECOMPLETE:
          {
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
              PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

              UINT mask = lpdbv ->dbcv_unitmask;
              UINT dev = 0;
              while (mask)
              {
                if (mask & 1)
                {
                  nglChar name[4];
                  name[0] = dev + 'A';
                  name[1] = ':';
                  name[2] = '/';
                  name[3] = 0;

                  nglPathVolume vol;
                  nglGetDriveInfo(name, vol);

                  nglPathVolumeDeviceInfo Dev(vol);

                  OnDeviceRemoved(&Dev);
                }
                dev++;
                mask >>= 1;
              }
            }
          }
          break;
        }
        
      }
      break;
/*
    case WM_NETEVENT: // User defined Network event
      return NetAllWndProc (wParam,lParam);
      break;
*/
  }

  return DefWindowProc( hWnd, message, wParam, lParam );
}

BOOL nglKernel::DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    IncRef();
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
    DecRef();
  }
  return TRUE;
}

typedef IDirect3D9* (WINAPI *Direct3DCreate9Func)(UINT SDKVersion);


LPDIRECT3D9 nglKernel::GetDirect3D() const
{
  if (!mpDirect3D)
  {
    HMODULE d3d9 = LoadLibrary(_T("d3d9.dll"));
    if (!d3d9)
    {
      MessageBox(NULL, _T("DirectX 9 Not found on this computer!"), _T("nglKernel::GetDirect3D()"), MB_OK);
      return NULL;
    }

    Direct3DCreate9Func Direct3DCreate9 = (Direct3DCreate9Func)GetProcAddress(d3d9, "Direct3DCreate9");
    if (!Direct3DCreate9)
    {
      MessageBox(NULL, _T("DirectX 9 DLL seems corrupt!"), _T("nglKernel::GetDirect3D()"), MB_OK);
      return NULL;
    }


    mpDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(mpDirect3D == NULL)
    {
      MessageBox(NULL, _T("Direct3DCreateX() failed!"), _T("nglKernel::GetDirect3D()"),MB_OK);
      return NULL;
    }
  }

  return mpDirect3D;
}

void nglKernel::NonBlockingHeartBeat()
{
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && msg.message != WM_QUIT)
  {
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }
}

