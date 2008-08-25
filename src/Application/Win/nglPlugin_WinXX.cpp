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
