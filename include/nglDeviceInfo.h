/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
  \file  nglDeviceInfo.h
  \brief This class describe the devices that are connected to the machine. It will be extented to define new supported device classes in the future.
*/

#ifndef __nglDeviceInfo_h__
#define __nglDeviceInfo_h__

//#include "nui.h"
#include "nglPath.h"

class nglDeviceInfo
{
public:
  enum Type
  {
    eDevType_PathVolume ///< the Device is a Volume.
  };

  Type GetType() const;

protected:
  nglDeviceInfo(Type t);
  virtual ~nglDeviceInfo();

private:
  Type mType;
};

class nglPathVolumeDeviceInfo : public nglDeviceInfo, public nglPathVolume
{
public:
  nglPathVolumeDeviceInfo(const nglPathVolume& rInfo);
  virtual ~nglPathVolumeDeviceInfo();
};

#endif

