/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglApplication.h"
#include "nglKeyboard.h"
#include <math.h>
#include <locale.h>
#include "ngl_winXX.h"

/*
#define NGL_APP_ENONE  0 (in nglApplication.h)
*/

const nglChar* gpApplicationErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Something bad happened"),
  NULL
};


/* Key mapping (work in progress, currently unused)
 */
typedef struct
{
  int  nglKey, winKey;
  char ascii;
} KeyMapping;

#define NGL_KEYMAP_SIZE 49

KeyMapping gKeyMapping[NGL_KEYMAP_SIZE] =
{
  {NK_UP          , VK_UP         },
  {NK_DOWN        , VK_DOWN       },
  {NK_LEFT        , VK_LEFT       },
  {NK_RIGHT       , VK_RIGHT      },
  {NK_SPACE       , VK_SPACE      },
  {NK_ENTER       , VK_RETURN     },
  {NK_TAB         , VK_TAB        },

  {NK_PAGEUP      , VK_PRIOR      },
  {NK_PAGEDOWN    , VK_NEXT       },
  {NK_HOME        , VK_HOME       },
  {NK_END         , VK_END        },
  {NK_LSHIFT      , VK_LSHIFT     },
  {NK_RSHIFT      , VK_RSHIFT     },
  {NK_LCTRL       , VK_LCONTROL   },
  {NK_RCTRL       , VK_RCONTROL   },
  {NK_LALT        , VK_LMENU      },
  {NK_RALT        , VK_RMENU      },

  {NK_BACKSPACE   , VK_BACK       },
  {NK_DELETE      , VK_DELETE     },
  {NK_INSERT      , VK_INSERT     },
  {NK_ESC         , VK_ESCAPE     },

  {NK_F1          , VK_F1         },
  {NK_F2          , VK_F2         },
  {NK_F3          , VK_F3         },
  {NK_F4          , VK_F4         },
  {NK_F5          , VK_F5         },
  {NK_F6          , VK_F6         },
  {NK_F7          , VK_F7         },
  {NK_F8          , VK_F8         },
  {NK_F9          , VK_F9         },
  {NK_F10         , VK_F10        },
  {NK_F11         , VK_F11        },
  {NK_F12         , VK_F12        },

  {NK_PAD_0       , VK_NUMPAD0    },
  {NK_PAD_1       , VK_NUMPAD1    },
  {NK_PAD_2       , VK_NUMPAD2    },
  {NK_PAD_3       , VK_NUMPAD3    },
  {NK_PAD_4       , VK_NUMPAD4    },
  {NK_PAD_5       , VK_NUMPAD5    },
  {NK_PAD_6       , VK_NUMPAD6    },
  {NK_PAD_7       , VK_NUMPAD7    },
  {NK_PAD_8       , VK_NUMPAD8    },
  {NK_PAD_9       , VK_NUMPAD9    },
  {NK_PAD_PLUS    , VK_ADD        },
  {NK_PAD_MINUS   , VK_SUBTRACT   },
  {NK_PAD_SLASH   , VK_DIVIDE     },
  {NK_PAD_ASTERISK, VK_MULTIPLY   },
  {NK_PAD_ENTER   , VK_EXECUTE    },
  {NK_PAD_PERIOD  , VK_DECIMAL    }
};                              



/*
 * Life cycle
 */

nglApplication::nglApplication()
{
  mUseIdle = false;
  mLastIdleCall = 0.0f;
  mExitPosted = false;

  // nglApplication is a kernel's client, just as plugin instances
  IncRef();
}

nglApplication::~nglApplication()
{
}

void nglApplication::Quit(int Code)
{
  PostQuitMessage(Code);
}



/*
 * Internals
 */

#define BUFSIZE 1024

int nglApplication::WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  if (!SysInit(hInstance))
    return 1;

  nglString module, s;
  nglChar buffer[BUFSIZE + 1];

  GetModuleFileName(GetModuleHandle(NULL), buffer, BUFSIZE);
  buffer[BUFSIZE] = 0;
  module = buffer;

  // Fetch application's name (mName) from module name
  s = module;
  int i = s.FindLast (_T('\\'));
  if (i != -1)
    s.DeleteLeft (i + 1);
  SetName(s);

  // Fetch application's executable path
  SetPath(nglPath(module).GetAbsolutePath());

  // Get application user args
  ParseCmdLine(lpCmdLine);






#if 0
  int res = 0;
  int format = -1;

  // Create a dummy context to be able to query ARB's wglChoosePixelFormatARB
  PIXELFORMATDESCRIPTOR pfd;
  pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion        = 1;
  pfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;
  pfd.iPixelType      = PFD_TYPE_RGBA;
  pfd.cColorBits      = 0; // FIXME
  pfd.cRedBits        = 0;
  pfd.cRedShift       = 0;
  pfd.cGreenBits      = 0;
  pfd.cGreenShift     = 0;
  pfd.cBlueBits       = 0;
  pfd.cBlueShift      = 0;
  pfd.cAlphaBits      = 0;
  pfd.cAlphaShift     = 0;
  pfd.cAccumBits      = 0;
  pfd.cAccumRedBits   = 0;
  pfd.cAccumGreenBits = 0;
  pfd.cAccumBlueBits  = 0;
  pfd.cAccumAlphaBits = 0;
  pfd.cDepthBits      = 0;
  pfd.cStencilBits    = 0;
  pfd.cAuxBuffers     = 0;
  pfd.iLayerType      = 0;
  pfd.bReserved       = 0;
  pfd.dwLayerMask     = 0;
  pfd.dwVisibleMask   = 0;
  pfd.dwDamageMask    = 0;

  WNDCLASS wc;

  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = NULL;
  wc.hCursor       = NULL;
  wc.lpszMenuName  = NULL;
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
  wc.hbrBackground = NULL;
  wc.lpfnWndProc   = &::DefWindowProc;
  wc.lpszClassName = NGL_CONTEXT_CLASS;
  if (!RegisterClass( &wc ))
  {
    NGL_DEBUG( OutputDebugString(_T("NGL: error: unable to create ") NGL_CONTEXT_CLASS _T(" window class\n")); )
      return false;
  }

  HWND tmpWin = CreateWindowEx(
    WS_EX_APPWINDOW,
    NGL_CONTEXT_CLASS,
    _T("Dummy Tmp Window from NGL"),
    WS_POPUP,
    0, 0, 64, 64,
    NULL,
    NULL,
    NULL,
    NULL);
  NGL_LOG(_T("nglWindow"), NGL_LOG_DEBUG, _T("Create tmp gl ctx window hwnd = 0x%x"), tmpWin);

  HDC tmpDC = ::GetDC(tmpWin);
  int pf = ChoosePixelFormat(tmpDC, &pfd);
  res = SetPixelFormat(tmpDC, pf, &pfd);
  HGLRC rc = wglCreateContext(tmpDC);
  res = wglMakeCurrent(tmpDC, rc);

  res = wglMakeCurrent(NULL, NULL);
  res = wglDeleteContext(rc);
  res = ReleaseDC(tmpWin, tmpDC);
  res = DestroyWindow(tmpWin);

#endif // 0


  // We're done, tell it to the user
  CallOnInit();

  return SysLoop(); 
}

int nglApplication::SysLoop()
{
  MSG msg;

  do
  {
    if (!GetIdle())
    {
      // Breathe (only returns at next message)
      GetMessage(&msg, NULL, 0, 0);

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // Process (rest of) msg queue
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && msg.message != WM_QUIT)
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }

    if (msg.message != WM_QUIT)
    {
      // We're done : enter idle state
      CallOnIdle();
    }
    else
    {
      if (!OnQuitRequested())
      {
        msg.message = 0;
      }
    }
  } while (msg.message != WM_QUIT);

  Exit(msg.wParam);

  return msg.wParam; // Return the exit code to the system
}


#ifdef _MAYAPLUGIN_

// Go get this one in the maya code! in #include <maya/MFnPlugin.h>
extern HINSTANCE MhInstPlugin;

MStatus initializePlugin (MObject obj)
{
  WNDCLASS wc;
  int ws;

  hInstance=MhInstPlugin;
  hPrevInstance=NULL;

  InitMain(MhInstPlugin,NULL,"",0);

  MStatus ret=Init(obj);

  return ret;
}

MStatus uninitializePlugin (MObject obj)
{
  MStatus ret;

  ret=App->Exit(obj);
  UnregisterClass("nglMain",App->hInstance);
  UnregisterClass("ngl",App->hInstance);
  return ret;
}

#endif // _MAYAPLUGIN_



/*
 * Video services
 */

/*
bool nglApplication::GetResolution (float& rHorizontal, float& rVertical)
{
  HDC hDC = GetDC();

  int xres = GetDeviceCaps(hDC, LOGPIXELSX);
  int yres = GetDeviceCaps(hDC, LOGPIXELSY);

  rHorizontal = (float)xres;
  rVertical = (float)yres;

  return (xres && yres);
}

bool nglApplication::GetGamma (float& rRed, float& rGreen, float& rBlue)
{
  rRed   = 1.0;
  rGreen = 1.0;
  rpBlue  = 1.0;

  return false;
}

bool nglApplication::SetGamma (float Red, float Green, float Blue)
{
  return false;
}
*/
