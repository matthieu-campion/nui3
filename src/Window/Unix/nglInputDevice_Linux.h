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

#ifndef __nglInputDeviceLinux_h__
#define __nglInputDeviceLinux_h__

#include "ngl.h"
#include "nglEvent.h"
#include "nglPath.h"
#include "nglInputDevice.h"
#include "../nglInputDeviceInstance.h"
#include <vector>


class nglInputDeviceLinux : public nglInputDeviceInstance, public nglEvent
{
public:
  static nglInputDevice* Enum (uint Index);

  nglInputDeviceLinux (const nglPath& rDevice);
  virtual ~nglInputDeviceLinux();

  bool IsValid();

protected:
  bool Update();
  void OnEvent (uint Flags);

private:
  typedef std::vector<nglInputDeviceLinux*> DeviceList;

  static bool       mInitCalled;
  static nglChar*   mpBasePath; // "/dev/js" or "/dev/input/js"
  static DeviceList mDeviceList;

  static void Exit();
};

#endif // __nglInputDeviceLinux_h__
