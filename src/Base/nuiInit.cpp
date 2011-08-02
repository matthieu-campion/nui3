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
#include "nuiDecoration.h"

#if (defined _UIKIT_)
# import <Foundation/NSAutoreleasePool.h>
#include "../Font/nuiPhoneFontDB.h"
#include "nglIMemory.h"
#endif

#define NUI_FONTDB_PATH _T("nuiFonts.db4")


static uint32 gNUIReferences = 0;
#if defined(_UIKIT_)
static NSAutoreleasePool* nui_autoreleasepool = nil;
#endif

bool nuiInit(void* OSHandle = NULL, nuiKernel* pKernel)
{
  //printf("nuiInit(%d)\n", gNUIReferences);
  if (gNUIReferences == 0)
  {
#ifdef WIN32
    WSADATA wsaData;   // if this doesn't work
    //WSAData wsaData; // then try this instead
    // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:
    int res = WSAStartup(MAKEWORD(1,1), &wsaData);
#endif
    
    if (!App)
    {      
#ifdef _WIN32_
      App = new nuiManualKernel(OSHandle, pKernel);
#else
      App = new nuiManualKernel(pKernel);
#endif
      App->CallOnInit();
    }

    // On iOS, init an AutoReleasePool:
#if defined(_UIKIT_)
    nui_autoreleasepool = [[NSAutoreleasePool alloc] init];
#endif

    // Init the texture manager:
    nuiTexture::InitTextures();
    
    
    // Init the font manager:
    
#if (defined _UIKIT_) && (!TARGET_IPHONE_SIMULATOR)
    nglIMemory Memory(gpnuiPhoneFontDB, gnuiPhoneFontDBSize);
    nuiFontManager::LoadManager(Memory, nglTime());
#else
    
    //#if (!defined TARGET_IPHONE_SIMULATOR) || (!TARGET_IPHONE_SIMULATOR)
    nglPath fontdb(ePathUserAppSettings);
    fontdb += nglString(NUI_FONTDB_PATH);
    
    if (fontdb.Exists() && fontdb.IsLeaf())
    {
      nglIFile db(fontdb);
      nuiFontManager::LoadManager(db, fontdb.GetLastMod());
    }  
    else
    {
      nuiFontManager::GetManager();
    }
    //#endif
#endif
    
    nuiDecoration::InitDecorationEngine();
    
  }
  
  gNUIReferences++;
  
  return App != NULL && !App->GetError();
}

bool nuiUninit()
{
  //printf("nuiUnInit(%d)\n", gNUIReferences);
  NGL_ASSERT(gNUIReferences != 0);
  gNUIReferences--;
  
  if (!gNUIReferences)
  {
    // Destroy all the windows that are still alive:
    nuiMainWindow::DestroyAllWindows();
    
    nglPath fontdb(ePathUserAppSettings);
    fontdb += nglString(NUI_FONTDB_PATH);
    
    nuiFontManager& rManager(nuiFontManager::GetManager(false));
    if (rManager.GetFontCount())
    {
      nglOFile db(fontdb, eOFileCreate);
      if (db.IsOpen())
        rManager.Save(db);
    }
    
    // From now on, all the contexts are dead so we have to release the remaining textures without trying to free their opengl resources
    // because those have been destroyed at the same time than the opengl context
    nuiDecoration::ExitDecorationEngine();
    
    nuiManualKernel* pApp = nuiManualKernel::Get();
    if (pApp)
    {
      App->CallOnExit(0);
      nuiDecoration::ExitDecorationEngine();
      nuiFont::ClearAll();
      nuiBuilder::Get().Uninit();
      delete (pApp);
      App = NULL;
      //nuiTexture::ClearAll();
#ifdef WIN32
      WSACleanup();
#endif
      return true;
    }
    nuiFont::ClearAll();
    nuiTexture::ClearAll();

    #if defined(_UIKIT_)
    [nui_autoreleasepool release];
    nui_autoreleasepool = nil;
    #endif
    
  }
#ifdef WIN32
  WSACleanup();
#endif
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
