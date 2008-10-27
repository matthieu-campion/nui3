/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiInit.h"
#include "nuiTexture.h"
#include "nuiFontManager.h"
#include "nglThreadChecker.h"

#define NUI_FONTDB_PATH _T("nuiFonts.db4")

class __NglKernel__ : public nglKernel
{
public:
#ifdef _WIN32_
  __NglKernel__(void* hInstance, nuiKernel* pKernel) 
  { 
    mpKernel = pKernel;
    SysInit((HINSTANCE)hInstance); 
  }
#else
  __NglKernel__(nuiKernel* pKernel) 
  { 
    mpKernel = pKernel;
    SysInit(); 
  }

#endif
  ~__NglKernel__() 
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

private:
  nuiKernel* mpKernel;
};


static uint32 gNUIReferences = 0;

bool nuiInit(void* OSHandle = NULL, nuiKernel* pKernel)
{
  //printf("nuiInit(%d)\n", gNUIReferences);
  if (gNUIReferences == 0)
  {
    if (!App)
#ifdef _WIN32_
      App = new __NglKernel__(OSHandle, pKernel);
#else
      App = new __NglKernel__(pKernel);
#endif
  }

  gNUIReferences++;
  
  
  // Init the font manager:
  nglPath fontdb(ePathUserAppSettings);
  fontdb += nglString(NUI_FONTDB_PATH);

  if (fontdb.Exists() && fontdb.IsLeaf())
  {
    nglIFile db(fontdb);
    nuiFontManager::LoadManager(db);
  }  
  
  return App != NULL && !App->GetError();
}

bool nuiUninit()
{
  nglPath fontdb(ePathUserAppSettings);
  fontdb += nglString(NUI_FONTDB_PATH);

  nuiFontManager& rManager(nuiFontManager::GetManager(false));
  if (rManager.GetFontCount())
  {
    nglOFile db(fontdb, eOFileCreate);
    rManager.Save(db);
  }
  
  //printf("nuiUnInit(%d)\n", gNUIReferences);
  NGL_ASSERT(gNUIReferences != 0);
  gNUIReferences--;

  if (!gNUIReferences)
  {
    __NglKernel__* pApp = dynamic_cast<__NglKernel__*>(App);
    if (pApp)
    {
      delete (pApp);
      App = NULL;
      return true;
    }
    // From now on, all the contexts are dead so we have to release the remaining textures without trying to free their opengl resources
    // because those have been destroyed at the same time than the opengl context
    nuiTexture::ForceReloadAll(true);
    nuiTexture::ClearAll();
  }
  return false;
}

/////// nuiKernel
//class nuiKernel // See nglKernel to see what the methods do...
nuiKernel::nuiKernel()
{
}

nuiKernel::~nuiKernel()
{
}

// Init/Exit:
void nuiKernel::OnInit()
{
}

void nuiKernel::OnExit(int Code)
{
}

// Device management:
void nuiKernel::OnDeviceAdded(const nglDeviceInfo* pDeviceInfo)
{
}

void nuiKernel::OnDeviceRemoved(const nglDeviceInfo* pDeviceInfo)
{
}
