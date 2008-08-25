/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiInit_h__
#define __nuiInit_h__

class nglDeviceInfo;

class NUI_API nuiKernel // See nglKernel to see what the methods do...
{
public:
  nuiKernel();
  virtual ~nuiKernel();
  // Init/Exit:
  virtual void OnInit();
  virtual void OnExit(int Code);
  // Device management:
  virtual void OnDeviceAdded(const nglDeviceInfo* pDeviceInfo);
  virtual void OnDeviceRemoved(const nglDeviceInfo* pDeviceInfo);
};

bool nuiInit(void* OSInstance, nuiKernel* pKernel = NULL); // OSInstance is the handle of the application's instance (HINSTANCE) in win32, NULL on other systems.
bool nuiUninit();

#endif //__nuiInit_h__
