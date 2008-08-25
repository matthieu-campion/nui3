/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#define __nglKeyboard_cpp__
#undef __nglKeyboard_h__
#include "nglKeyboard.h"


/* Key name table
 */
const nglChar *gpKeyName[NGL_KEY_MAX + 1] =
{
  _T(""),             // NK_NONE
  _T("Esc"),          // NK_ESC
  _T("F1"),           // NK_F1
  _T("F2"),           // NK_F2
  _T("F3"),           // NK_F3
  _T("F4"),           // NK_F4
  _T("F5"),           // NK_F5
  _T("F6"),           // NK_F6
  _T("F7"),           // NK_F7
  _T("F8"),           // NK_F8
  _T("F9"),           // NK_F9
  _T("F10"),          // NK_F10
  _T("F11"),          // NK_F11
  _T("F12"),          // NK_F12
  _T("SysReq"),       // NK_SYSREQ
  _T("Scroll Lock"),  // NK_SCRLOCK
  _T("Pause"),        // NK_PAUSE
  _T("`"),            // NK_GRAVE
  _T("1"),            // NK_1
  _T("2"),            // NK_2
  _T("3"),            // NK_3
  _T("4"),            // NK_4
  _T("5"),            // NK_5
  _T("6"),            // NK_6
  _T("7"),            // NK_7
  _T("8"),            // NK_8
  _T("9"),            // NK_9
  _T("0"),            // NK_0
  _T("-"),            // NK_MINUS
  _T("="),            // NK_EQUAL
  _T("BackSpace"),    // NK_BACKSPACE
  _T("Ins"),          // NK_INSERT
  _T("Home"),         // NK_HOME
  _T("Pg Up"),        // NK_PAGEUP
  _T("Num Lock"),     // NK_PAD_LOCK
  _T("Pad /"),        // NK_PAD_SLASH
  _T("Pad *"),        // NK_PAD_ASTERISK
  _T("Pad -"),        // NK_PAD_MINUS
  _T("Tab"),          // NK_TAB
  _T("Q"),            // NK_Q
  _T("W"),            // NK_W
  _T("E"),            // NK_E
  _T("R"),            // NK_R
  _T("T"),            // NK_T
  _T("Y"),            // NK_Y
  _T("U"),            // NK_U
  _T("I"),            // NK_I
  _T("O"),            // NK_O
  _T("P"),            // NK_P
  _T("["),            // NK_LBRACKET
  _T("]"),            // NK_RBRACKET
  _T("Enter"),        // NK_ENTER
  _T("Del"),          // NK_DELETE
  _T("End"),          // NK_END
  _T("Pg Down"),      // NK_PAGEDOWN
  _T("Pad 7"),        // NK_PAD_7
  _T("Pad 8"),        // NK_PAD_8
  _T("Pad 9"),        // NK_PAD_9
  _T("Pad +"),        // NK_PAD_PLUS
  _T("Caps Lock"),    // NK_CAPSLOCK
  _T("A"),            // NK_A
  _T("S"),            // NK_S
  _T("D"),            // NK_D
  _T("F"),            // NK_F
  _T("G"),            // NK_G
  _T("H"),            // NK_H
  _T("J"),            // NK_J
  _T("K"),            // NK_K
  _T("L"),            // NK_L
  _T(";"),            // NK_SEMICOLON
  _T("'"),            // NK_APOSTROPHE
  _T("#"),            // NK_NUMBERSIGN
  _T("Pad 4"),        // NK_PAD_4
  _T("Pad 5"),        // NK_PAD_5
  _T("Pad 6"),        // NK_PAD_6
  _T("Left SHIFTKEY"),      // NK_LSHIFT
  _T("\\"),           // NK_BACKSLASH
  _T("Z"),            // NK_Z
  _T("X"),            // NK_X
  _T("C"),            // NK_C
  _T("V"),            // NK_V
  _T("B"),            // NK_B
  _T("N"),            // NK_N
  _T("M"),            // NK_M
  _T("),"),           // NK_COMMA
  _T("."),            // NK_PERIOD
  _T("/"),            // NK_SLASH
  _T("Right SHIFTKEY"),      // NK_RSHIFT
  _T("Up"),           // NK_UP
  _T("Pad 1"),        // NK_PAD_1
  _T("Pad 2"),        // NK_PAD_2
  _T("Pad 3"),        // NK_PAD_3
  _T("Pad Enter"),    // NK_PAD_ENTER
  _T("Left CTRLKEY"),       // NK_LCTRL
  _T("Left ALTKEY"),        // NK_LALT
  _T("Space"),        // NK_SPACE
  _T("Right ALTKEY"),        // NK_RALT
  _T("Right CTRLKEY"),       // NK_RCTRL
  _T("Left"),         // NK_LEFT
  _T("Down"),         // NK_DOWN
  _T("Right"),        // NK_RIGHT
  _T("Pad 0"),        // NK_PAD_0
  _T("Pad ."),        // NK_PAD_PERIOD
#ifdef _CARBON_
  _T("Left CMDKEY"),       // NK_LMETA
  _T("Right CMDKEY"),       // NK_RMETA
#elif defined _WIN32_
  _T("Left WINKEY"),       // NK_LMETA
  _T("Right WINKEY"),       // NK_RMETA
#else
  _T("Left METAKEY"),       // NK_LMETA
  _T("Right METAKEY"),       // NK_RMETA
#endif
  _T("Menu")          // NK_MENU
};


const nglChar* GetKeyName(nglKeyCode Key)
{
  if (Key > NGL_KEY_MAX)
    Key = NK_NONE;
  
  return gpKeyName[Key];
}
