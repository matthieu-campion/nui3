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

#include "ngl.h"
#include "nglKernel.h"
#include "nglInputDevice_Linux.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/joystick.h>


bool                            nglInputDeviceLinux::mInitCalled = false;
nglChar*                        nglInputDeviceLinux::mpBasePath = NULL;
nglInputDeviceLinux::DeviceList nglInputDeviceLinux::mDeviceList;


nglInputDeviceLinux::nglInputDeviceLinux (const nglPath& rDevice) : nglInputDeviceInstance(), nglEvent()
{
  mFlags = Read|Error;
  mFD = open((char*)rDevice.GetPathName().GetChars(), O_RDONLY);
  if (mFD == -1)
    return;

  char byte;
  char name[128];

  // Get number of axes
  ioctl(mFD, JSIOCGAXES, &byte);
  mAxes.resize(byte);

  // Get number of buttons
  ioctl(mFD, JSIOCGBUTTONS, &byte);
  mButtons.resize(byte);

  // Fetch name
  if (ioctl(mFD, JSIOCGNAME(sizeof(name)), name) < 0)
    mName = "unkown";
  else
    mName = name;

  // Synthetize port name
  mPort.Format("%s", rDevice.GetPathName().GetChars());

  App->AddEvent(this);
}

nglInputDeviceLinux::~nglInputDeviceLinux()
{
  if (mFD != -1)
  {
    App->DelEvent(this);
    close(mFD);
  }

  /* Make sure that we don't keep a (stale) reference of this dying
   * object in the device list.
   */
  DeviceList::iterator idev;

  for (idev = mDeviceList.begin(); idev != mDeviceList.end(); idev++)
    if (*idev == this)
      *idev = NULL;
}

nglInputDevice* nglInputDeviceLinux::Enum (uint Index)
{
  if (!mInitCalled)
  {
    App->AddExit(Exit);
    mInitCalled = true;
  }

  if (!mpBasePath)
  {
    nglPath path_old(_T("/dev/js1"));
    nglPath path_new(_T("/dev/input/js1"));

    if (path_new.Exists())
      mpBasePath = (nglChar*) _T("/dev/input/js");
    else
    if (path_old.Exists())
      mpBasePath = (nglChar*) _T("/dev/js");
  }

  // No valid path to joystick devices
  if (!mpBasePath)
    return NULL;

  uint i;
  uint valid = 0;
  bool found = false;

  for (i = 0; !found; i++)
  {
    // Check existence of device node '{basepath}/js{i}'
    nglString path_str;
    path_str.Format("%s%d", mpBasePath, i);

    nglPath path(path_str);
    if (!path.Exists())
      break; // No more devices to check, end there

    // Grow vector if necessary
    if (i >= mDeviceList.size())
    {
      mDeviceList.resize(i + 1);
      mDeviceList[i] = NULL;
    }

    // If device has no associated instance, create one now
    if (mDeviceList[i] == NULL)
      mDeviceList[i] = new nglInputDeviceLinux(path);

    if (mDeviceList[i]->IsValid())
      valid++;
    found = (valid == Index + 1);
  }

  if (!found)
    return NULL;

  nglInputDeviceLinux* dev = mDeviceList[i-1];

  return dev ? CreateDevice(dev) : NULL;
}

bool nglInputDeviceLinux::IsValid()
{
  return mFD != -1;
}

void nglInputDeviceLinux::OnEvent (uint Flags)
{
  if (!(Flags & Read))
    return;

  struct js_event event;

  if (read(mFD, &event, sizeof(event)) <= 0)
    return;

  switch (event.type & ~JS_EVENT_INIT)
  {
    case JS_EVENT_BUTTON:
      mButtons[event.number] = event.value ? true : false;
      break;

    case JS_EVENT_AXIS:
      mAxes[event.number] = (float)event.value / 32767.f;
      break;
  }
}

bool nglInputDeviceLinux::Update()
{
  /* Since the Linux joystick interface is event driven, we don't
   * do anything special in the polling function. OnEvent() will
   * effectively updates the different members asynchronously.
   */
  return IsValid();
}

void nglInputDeviceLinux::Exit()
{
  DeviceList::iterator idev;

  for (idev = mDeviceList.begin(); idev != mDeviceList.end(); idev++)
  {
    nglInputDeviceLinux* dev = *idev;

    if (dev)
      delete dev;
  }
}
