/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglKeyboard.h
\brief Keyboard layout definition

This file contains keyboard layout macros.
*/

#ifndef __nglKeyboard_h__
#define __nglKeyboard_h__

//#include "nui.h"




#ifndef _DOXYGEN_

#ifdef __nglKeyboard_cpp__
  #define __EQU(x) =x
  #define EXTERN_KEYBOARD_DEF
#else
  #define __EQU(x)
  #define EXTERN_KEYBOARD_DEF extern
  typedef uint nglKeyCode;
#endif

#ifndef __NGL_KEY_DECL
#define __NGL_KEY_DECL(symbol,value) EXTERN_KEYBOARD_DEF NGL_API const nglKeyCode symbol __EQU(value)
#endif

/* PC-102 keys
 */
__NGL_KEY_DECL (NK_NONE          , 0);
__NGL_KEY_DECL (NK_ESC           , 1);
__NGL_KEY_DECL (NK_F1            , 2);
__NGL_KEY_DECL (NK_F2            , 3);
__NGL_KEY_DECL (NK_F3            , 4);
__NGL_KEY_DECL (NK_F4            , 5);
__NGL_KEY_DECL (NK_F5            , 6);
__NGL_KEY_DECL (NK_F6            , 7);
__NGL_KEY_DECL (NK_F7            , 8);
__NGL_KEY_DECL (NK_F8            , 9);
__NGL_KEY_DECL (NK_F9            , 10);
__NGL_KEY_DECL (NK_F10           , 11);
__NGL_KEY_DECL (NK_F11           , 12);
__NGL_KEY_DECL (NK_F12           , 13);
__NGL_KEY_DECL (NK_SYSREQ        , 14);
__NGL_KEY_DECL (NK_SCRLOCK       , 15);
__NGL_KEY_DECL (NK_PAUSE         , 16);

__NGL_KEY_DECL (NK_GRAVE         , 17);
__NGL_KEY_DECL (NK_1             , 18);
__NGL_KEY_DECL (NK_2             , 19);
__NGL_KEY_DECL (NK_3             , 20);
__NGL_KEY_DECL (NK_4             , 21);
__NGL_KEY_DECL (NK_5             , 22);
__NGL_KEY_DECL (NK_6             , 23);
__NGL_KEY_DECL (NK_7             , 24);
__NGL_KEY_DECL (NK_8             , 25);
__NGL_KEY_DECL (NK_9             , 26);
__NGL_KEY_DECL (NK_0             , 27);
__NGL_KEY_DECL (NK_MINUS         , 28);
__NGL_KEY_DECL (NK_EQUAL         , 29);
__NGL_KEY_DECL (NK_BACKSPACE     , 30);
__NGL_KEY_DECL (NK_INSERT        , 31);
__NGL_KEY_DECL (NK_HOME          , 32);
__NGL_KEY_DECL (NK_PAGEUP        , 33);
__NGL_KEY_DECL (NK_PAD_LOCK      , 34);
__NGL_KEY_DECL (NK_PAD_SLASH     , 35);
__NGL_KEY_DECL (NK_PAD_ASTERISK  , 36);
__NGL_KEY_DECL (NK_PAD_MINUS     , 37);

__NGL_KEY_DECL (NK_TAB           , 38);
__NGL_KEY_DECL (NK_Q             , 39);
__NGL_KEY_DECL (NK_W             , 40);
__NGL_KEY_DECL (NK_E             , 41);
__NGL_KEY_DECL (NK_R             , 42);
__NGL_KEY_DECL (NK_T             , 43);
__NGL_KEY_DECL (NK_Y             , 44);
__NGL_KEY_DECL (NK_U             , 45);
__NGL_KEY_DECL (NK_I             , 46);
__NGL_KEY_DECL (NK_O             , 47);
__NGL_KEY_DECL (NK_P             , 48);
__NGL_KEY_DECL (NK_LBRACKET      , 49);
__NGL_KEY_DECL (NK_RBRACKET      , 50);
__NGL_KEY_DECL (NK_ENTER         , 51);
__NGL_KEY_DECL (NK_DELETE        , 52);
__NGL_KEY_DECL (NK_END           , 53);
__NGL_KEY_DECL (NK_PAGEDOWN      , 54);
__NGL_KEY_DECL (NK_PAD_7         , 55);
__NGL_KEY_DECL (NK_PAD_8         , 56);
__NGL_KEY_DECL (NK_PAD_9         , 57);
__NGL_KEY_DECL (NK_PAD_PLUS      , 58);

__NGL_KEY_DECL (NK_CAPSLOCK      , 59);
__NGL_KEY_DECL (NK_A             , 60);
__NGL_KEY_DECL (NK_S             , 61);
__NGL_KEY_DECL (NK_D             , 62);
__NGL_KEY_DECL (NK_F             , 63);
__NGL_KEY_DECL (NK_G             , 64);
__NGL_KEY_DECL (NK_H             , 65);
__NGL_KEY_DECL (NK_J             , 66);
__NGL_KEY_DECL (NK_K             , 67);
__NGL_KEY_DECL (NK_L             , 68);
__NGL_KEY_DECL (NK_SEMICOLON     , 69);
__NGL_KEY_DECL (NK_APOSTROPHE    , 70);
__NGL_KEY_DECL (NK_NUMBERSIGN    , 71);
__NGL_KEY_DECL (NK_PAD_4         , 72);
__NGL_KEY_DECL (NK_PAD_5         , 73);
__NGL_KEY_DECL (NK_PAD_6         , 74);

__NGL_KEY_DECL (NK_LSHIFT        , 75);
__NGL_KEY_DECL (NK_BACKSLASH     , 76);
__NGL_KEY_DECL (NK_Z             , 77);
__NGL_KEY_DECL (NK_X             , 78);
__NGL_KEY_DECL (NK_C             , 79);
__NGL_KEY_DECL (NK_V             , 80);
__NGL_KEY_DECL (NK_B             , 81);
__NGL_KEY_DECL (NK_N             , 82);
__NGL_KEY_DECL (NK_M             , 83);
__NGL_KEY_DECL (NK_COMMA         , 84);
__NGL_KEY_DECL (NK_PERIOD        , 85);
__NGL_KEY_DECL (NK_SLASH         , 86);
__NGL_KEY_DECL (NK_RSHIFT        , 87);
__NGL_KEY_DECL (NK_UP            , 88);
__NGL_KEY_DECL (NK_PAD_1         , 89);
__NGL_KEY_DECL (NK_PAD_2         , 90);
__NGL_KEY_DECL (NK_PAD_3         , 91);
__NGL_KEY_DECL (NK_PAD_ENTER     , 92);

__NGL_KEY_DECL (NK_LCTRL         , 93);
__NGL_KEY_DECL (NK_LALT          , 94);
__NGL_KEY_DECL (NK_SPACE         , 95);
__NGL_KEY_DECL (NK_RALT          , 96);
__NGL_KEY_DECL (NK_RCTRL         , 97);
__NGL_KEY_DECL (NK_LEFT          , 98);
__NGL_KEY_DECL (NK_DOWN          , 99);
__NGL_KEY_DECL (NK_RIGHT         , 100);
__NGL_KEY_DECL (NK_PAD_0         , 101);
__NGL_KEY_DECL (NK_PAD_PERIOD    , 102);

/* PC-105 supplementary keys
 */
__NGL_KEY_DECL (NK_LMETA         , 103);
__NGL_KEY_DECL (NK_RMETA         , 104);
__NGL_KEY_DECL (NK_MENU          , 105);

/* PC/MAC modifier key trick
 */
#if defined _WIN32_ || defined _UNIX_
__NGL_KEY_DECL (NK_LMOD, NK_LCTRL);
__NGL_KEY_DECL (NK_RMOD, NK_RCTRL);
#endif
#ifdef __APPLE__
__NGL_KEY_DECL (NK_LMOD, NK_LMETA);
__NGL_KEY_DECL (NK_RMOD, NK_RMETA);
#endif


// Only PC-102/105 support for now
#define NGL_KEY_MAX 105

#endif // _DOXYGEN_

#ifndef __nglKeyboard_cpp__

class NGL_API nglKeyEvent
{
public:
  nglKeyEvent(nglKeyCode Key, uint32 Char, uint32 RawChar)
  {
    mKey = Key;
    mChar = Char;
    mRawChar = RawChar;
  }

  nglKeyCode mKey; ///< Low Level Scan Code
  uint32 mChar; ///< Translated Unicode Character according to the currently pressed modifier keys and previous dead keys
  uint32 mRawChar; ///< Raw unicode character (what mChar would contain is there was no modifier keys down and no previous dead keys).
};

#endif

#undef __EQU
#undef EXTERN_KEYBOARD_DEF


const nglChar* GetKeyName(nglKeyCode Key);

#endif // __nglKeyboard_h__

