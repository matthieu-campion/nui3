/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglDeviceInfo.cpp
\brief This class describe the devices that are connected to the machine. It will be extented to define new supported device classes in the future.
*/

#include "nui.h"

//class nglDeviceInfo
nglDeviceInfo::Type nglDeviceInfo::GetType() const
{
  return mType;
}

nglDeviceInfo::nglDeviceInfo(Type t)
{
  mType = t;
}

nglDeviceInfo::~nglDeviceInfo()
{
}

// class nglPathVolumeDeviceInfo : public nglPathVolume
nglPathVolumeDeviceInfo::nglPathVolumeDeviceInfo(const nglPathVolume& rInfo)
: nglDeviceInfo(nglDeviceInfo::eDevType_PathVolume),
  nglPathVolume(rInfo)
{
}

nglPathVolumeDeviceInfo::~nglPathVolumeDeviceInfo()
{
}

