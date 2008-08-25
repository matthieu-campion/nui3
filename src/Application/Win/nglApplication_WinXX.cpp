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
  } while (msg.message != WM_QUIT);

  CallOnExit(msg.wParam);

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
