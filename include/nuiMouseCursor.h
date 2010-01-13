/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

enum nuiMouseCursor    ///< See SetCursor() and GetCursor()
{
  eCursorDoNotSet=0,  ///< Do not set/change the cursor
  eCursorNone,      ///< No cursor
  eCursorArrow,     ///< Standard arrow
  eCursorCross,     ///< Crosshair
  eCursorIBeam,     ///< Text entry
  eCursorHand,      ///< Hand
  eCursorClosedHand,///< Closed Hand (at least on macos)
  eCursorPointingHand,///< hand with a pointing finger
  eCursorHelp,      ///< Question mark with an arrow
  eCursorWait,      ///< Hourglass or watch (might be with an arrow)
  eCursorCaret,     ///< Text caret
  eCursorDnD,       ///< Drag'n'drop symbol
  eCursorForbid,    ///< DnD forbidden (slashed circle)
  eCursorMove,      ///< Four way arrow (horizontal and vertical)
  eCursorResize,    ///< Resize symbol
  eCursorResizeNS,  ///< Double arrow pointing North & South
  eCursorResizeWE,  ///< Double arrow pointing West & East
  eCursorResizeN,   ///< Resize from North
  eCursorResizeS,   ///< Resize from South
  eCursorResizeW,   ///< Resize from West
  eCursorResizeE,   ///< Resize from East
  eCursorResizeNW,  ///< Resize from North West corner
  eCursorResizeNE,  ///< Resize from North East corner
  eCursorResizeSW,  ///< Resize from South West corner
  eCursorResizeSE   ///< Resize from South East corner
};

