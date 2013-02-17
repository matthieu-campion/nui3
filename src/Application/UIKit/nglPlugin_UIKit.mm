/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

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
  if (!App)
  {
    App = this;
    SysInit();
  }
  IncRef();
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
  if (!App)
  {
    App = new nglPluginKernel();
    App->SysInit();
  }
  App->IncRef();
}

nglPlugin::~nglPlugin()
{
}

nglPath nglPlugin::GetPath()
{
  return nglPath();
}
