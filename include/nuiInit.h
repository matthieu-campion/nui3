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

/* Class nuiManualKernel
 * This class cann be used to control nui when it is not used as a full nui application, that is when you use nuiInit()/nuiUninit() by hand.
 * You can call *(nuiManualKernel*)App->XXX() to access this class' API.
 */
class nuiManualKernel : public nglKernel
{
public:
#ifdef _WIN32_
  nuiManualKernel(void* hInstance, nuiKernel* pKernel) 
  { 
    mpKernel = pKernel;
    SysInit((HINSTANCE)hInstance); 
  }
#elif defined(_ANDROID_)
  nuiManualKernel(void* app, nuiKernel* pKernel) 
  { 
    mpKernel = pKernel;
    SysInit((android_app*)app); 
  }
#else
  nuiManualKernel(nuiKernel* pKernel) 
  { 
    mpKernel = pKernel;
    SysInit(); 
  }
  
#endif
  virtual ~nuiManualKernel() 
  {
    if (mpKernel)
      delete mpKernel;
  }
  
  // Hooks:
  void OnInit()
  {
    if (mpKernel)
      mpKernel->OnInit();
  }
  
  void OnExit(int Code)
  {
    if (mpKernel)
      mpKernel->OnExit(Code);
  }
  
  // Device management:
  void OnDeviceAdded(const nglDeviceInfo* pDeviceInfo)
  {
    if (mpKernel)
      mpKernel->OnDeviceAdded(pDeviceInfo);
  }
  
  void OnDeviceRemoved(const nglDeviceInfo* pDeviceInfo)
  {
    if (mpKernel)
      mpKernel->OnDeviceRemoved(pDeviceInfo);
  }
  
  void Activate()
  {
    CallOnActivation();  
  }
  
  void Deactivate()
  {
    CallOnDeactivation();
  }
  
  static nuiManualKernel* Get()
  {
    return dynamic_cast<nuiManualKernel*>(App);
  }
  
private:
  nuiKernel* mpKernel;
};



bool nuiInit(void* OSInstance, nuiKernel* pKernel = NULL); // OSInstance is the handle of the application's instance (HINSTANCE) in win32, NULL on other systems. This call will init a minimal set of features )no default decorations, no widget builder, etc).
bool nuiInitMinimal(void* OSInstance, nuiKernel* pKernel = NULL); // OSInstance is the handle of the application's instance (HINSTANCE) in win32, NULL on other systems. This call will init all subsystems (default decorations, nuiBuilder, etc).
bool nuiUninit();

#endif //__nuiInit_h__
