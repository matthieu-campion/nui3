/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglPlugin.h"

// nglPluginKernel:
class nglPluginKernel: public nglKernel
{
public:
  nglPluginKernel();
  virtual ~nglPluginKernel();

  virtual void OnInit();
  virtual void Quit(int Code = 0);
  const nglChar* OnError(uint &) const;

  friend class nglPlugin;
};

nglPluginKernel::nglPluginKernel()
{
}

nglPluginKernel::~nglPluginKernel()
{
}

void nglPluginKernel::OnInit()
{
}

void nglPluginKernel::Quit(int Code)
{
}

const nglChar *nglPluginKernel::OnError(uint& rCode) const
{
  return _T("Unknown Error");
}

// nglPlugin:
nglPlugin::nglPlugin()
{
}

nglPlugin::~nglPlugin()
{
}

BOOL nglPlugin::DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
    {
      mInstance = hInstance;
      // If there is no Application allready created create one:
      if (!App)
      {
        App = new nglPluginKernel();
        App->SysInit(hInstance);
      }
      App->IncRef();
    }
    break;

    case DLL_PROCESS_DETACH:
    {
      if (App)
        App->DecRef();
    }
    break;
  }
  return App ? App->DllMain(hInstance, fdwReason, lpvReserved) : FALSE;
}

nglPath nglPlugin::GetPath()
{
  nglChar FileName[2049];
  if (GetModuleFileName(mInstance, FileName, 2048))
    return nglPath(FileName);
  return nglPath();
}
