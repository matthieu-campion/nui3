/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglInputDevice.h
\brief Input devices interface (not mouse or keyboard)
*/

#ifndef __nglInputDevice_h__
#define __nglInputDevice_h__

//#include "nui.h"
#include "nglString.h"


//! Input devices interface
/*!
This class manages input devices like joysticks or tablets. It uses a simple
abstraction model :

- \b Name : the name can qualify the type of input device and/or its precise
  model. It is suitable for display for end users. Example :

  <code>Analog 4-axis 10-button 1-hat Saitek joystick</code>

- \b Port : the description of the physical port where the input device is
  connected. Example :

  <code>Game port #1</code>

- \b Buttons : an input device can have zero to any number of buttons, which
  can be read as simple boolean values.

- \b Axis : an input device can have zero to any number of axis, which can
  be read as floating point values ranging from -1 to 1.

Input device instances are created via Enum() (the constructor is \e not
available). Values are read by \e polling, you have to explicitly call
Update() to update the buttons and axis values, then read them via
GetButtonState() and GetAxeValue().

Note that the core keyboard and mouse are not handled by thic class, they
are implicitly managed by nglWindow due to their interaction with the windowing
system.
 */
class NGL_API nglInputDevice
{
public:
  /** @name Enumeration */
  //@{
  static nglInputDevice* Enum (uint Index);
  /*!< Enumerate available input devices
    \param Index an arbitrary index starting from 0
    \return input device intance, must be deleted by the user

    To build an input device instance, you have to take one returned by Enum().
    Example :

\code
nglInputDevice* dev;
uint i = 0;

while ((dev = nglInputDevice::Enum(i++)))
{
  dev->Dump();
  delete dev;
}
\endcode

    You can have several instances representing the same device (queried with
    the same index), and you must delete all of them when you don't need them.
  */
  //@}

  /** @name Life cycle */
  //@{
  virtual ~nglInputDevice();
  //@}

  /** @name Status polling */
  //@{
  bool      Update();
  /*!< Polls for current buttons and axis values
    \return if false, the device could not be read (it might have been
            disconnected)
  */
  nglString GetPort();                    ///< Physical port description
  nglString GetName();                    ///< Device description
  uint      GetButtonCount();             ///< Number of buttons
  bool      GetButtonState (uint Index);  ///< State of the <p>Index</p>-nth button
  uint      GetAxeCount();                ///< Number of axis
  float     GetAxeValue (uint Index);     ///< Value of the <p>Index</p>-nth axis (-1 to 1)
  void      Dump (uint Level = 0) const;  ///< Dumps device description info at log level \p Level
  //@}

private:
  class nglInputDeviceInstance* mpDevice;

  nglInputDevice(const nglInputDevice&) {} // Undefined copy constructor
  nglInputDevice(class nglInputDeviceInstance* pDevice);

  friend class nglInputDeviceInstance;
};

#endif // __nglInputDevice_h__
