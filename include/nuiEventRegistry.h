/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


// This file contains the event identifier's registry.

#ifndef __nuiEventRegistry_h__
#define __nuiEventRegistry_h__

#include "nuiEvent.h"

enum nuiEventId
{
  nuiWidgetSelected,          ///< The widget have just been selected
  nuiWidgetDeselected,        ///< The widget have just been deselected
  nuiWidgetDisabled,          ///< The widget have just been disabled
  nuiWidgetEnabled,           ///< The widget have just been enabled
  nuiWidgetStateChanged,      ///< The widget just changed its state. 

  nuiWidgetShown,             ///< Sent when a widget becomes visible. 
  nuiWidgetHiden,             ///< Sent when a widget becomes hidden. 
  nuiWidgetVisibilityChanged, ///< Sent when a widget becomes visible of hidden. 

  nuiWidgetHoverOn,           ///< Sent when the mouse starts hovering on a widget. 
  nuiWidgetHoverOff,          ///< Sent when the mouse stops hovering on a widget.
  nuiWidgetHoverChanged,      ///< Sent when the mouse starts or stops hovering on a widget.

  nuiWidgetActivated,         ///< Sent when an element have been activated.

  nuiWidgetParentChanged,     ///< Sent when the widget have been reparented.
  // nuiButton
  nuiButtonPressed,           ///< Sent when a mouse button is pressed on this widget.
  nuiButtonDePressedInactive, ///< Sent when a button is depressed.
  nuiButtonDePressed,         ///< Sent when a button is depressed.

  // nuiMenu & nuiMenuItem
  nuiMenuItemSelected,        ///< Sent when a popup menu item's have been selected.
  nuiMenuDone,                ///< Sent when a popup menu is about to be closed.

  // nuiWindow                
  nuiClicked,                 ///< Sent when a widget have been clicked by the user.
  nuiUnclicked,               ///< Sent when a widget have been unclicked by the user.
  nuiMoved,                   ///< The "moved" event is used for many event types: mouse movement, window movement, etc...

  // nuiUserArea
  nuiUserDraw,                ///< Sent when an nuiUserArea have to be drawn by the user. 

  // nuiValues & other objects
  nuiChildAdded,   ///< Sent when a widget is added to a container.
  nuiChildDeleted, ///< Sent when a widget is removed from a container.
  nuiTrashed,      ///< Sent when an object is put to the trash (still not destroyed though)
  nuiDestroyed,    ///< Sent from the actual destructor. 
  nuiValueChanged, ///< Sent when a value is changed.    
  nuiChanged,      ///< Sent when an object is changed.    

  // nuiTimer:
  nuiTimerTick,     ///< Sent by nuiTimer on each timer tick.    

  nuiAnimationStart,
  nuiAnimationStop,
  nuiAnimationLoop,
  
  nuiActionAborted,

  nuiFocus
};
#endif // __nuiEventRegistry_h__

/*
// Generic events for all objects:
  nuiSelected     ///< The object have just been selected
  nuiDeselected   ///< The object have just been deselected
  nuiDisabled     ///< The object have just been disabled
  nuiEnabled      ///< The object have just been enabled 
  nuiHover        ///< The mouse starts to hover over the object
  nuiUnhover      ///< The mouse stops to hover over the object

  nuiClicked      ///< The object received a click event.
  nuiUnclicked    ///< The object received an unclick event.
  nuiMoved        ///< The object received a mouse moved event, available also to any other movement event generation.
  
  nuiDrawBegin    ///< The system will start to draw the object just after this call.
  nuiDrawEnd      ///< The system have just finished to draw the object.
  
  nuiChanged      ///< The object contents have changed.

  nuiKeyDown      ///< The object have received a keydown event.
  nuiKeyUp        ///< The object have received a keyup event.
  
  nuiFocused      ///< The object have just received the keyboard focus.
  nuiUnfocused    ///< The object have just lost the keyboard focus.
  
  nuiLayoutChanged ///< The object's layout have just changed and needs to be recalculated.

// Secific to some objects and misc:
  nuiTick         ///< Timer tick.
  nuiTrashed      ///< The object have just been trashed! Do not use it anymore!
  nuiDraw         ///< The system is drawing the object (for objects like the user area)
  nuiValueChanged ///< A value assiciated with the object have just changed.

*/
