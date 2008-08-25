/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglPlugin.h
\brief Base plugin class

Abstraction of a plugin (does not depend on the hosting application).
*/

#ifndef __nglPlugin_h__
#define __nglPlugin_h__

//#include "nui.h"
#include "nglPath.h"


//! Base plugin class
/*!
This is the base class for \e plugins, ie. program parts that are dynamically loaded
into an hosting application (ie. a running process). At instantiation time, the plugin
will make sure that the nglKernel (the abstraction of the hosting application) is present,
and if not create a fitting one. The global services are thus always available via the
\a Kernel global.

Example of hosting applications are browsers (via the ubiquitous Netscape API or MS ActiveX),
modelers (Discreet's 3DS Max, LightWave's Maya), music editors (Steinberg's Cubase), and so on.

You will find a specific nglPlugin based class for every hosting application. Developers will
also have to provide a nglKernel based class for every hosting application (this is transparent
to the user).
*/
class NGL_API nglPlugin
{
public:

  /** @name Life cycle */
  //@{
  nglPlugin();
  virtual ~nglPlugin();
  //@}

  /** @name Plugin context */
  //@{
  nglPath GetPath(); 
  /*!<
    \return current plugin's executable path (includes the binary file name)
  */
  //@}

private:
  nglPlugin(const nglPlugin&) {} // Undefined copy constructor

#ifdef _WIN32_
  //friend int _cdecl main(int argc, char** argv);
  friend BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved);
  BOOL DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved);

  HINSTANCE mInstance;
#endif // _WIN32_

#ifdef _CARBON_
  /// There is no plugin support in ngl for carbon or macho at the moment.
#endif // _CARBON_

#ifdef _UNIX_
  /// There is no plugin support in ngl for unix at the moment.
#endif // _UNIX_
};


/*
 * Internal (intermediate) macros
 */

#ifndef _DOXYGEN_

#ifdef _WIN32_
  #define __NGL_PLUGIN_CREATE(UserPluginClass, InstanceHook) \
  class UserPluginClass* UserPlugin;\
  BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)\
  { \
    if (fdwReason == DLL_PROCESS_ATTACH)\
    { \
      UserPlugin InstanceHook new UserPluginClass();\
    }\
    return UserPlugin->DllMain(hInstance, fdwReason, lpvReserved);\
  }
#endif // _WIN32_

#define NGL_PLUGIN_CREATE(UserPluginClass) \
  __NGL_PLUGIN_CREATE(UserPluginClass, =)

#define NGL_PLUGIN_CREATE_EX(UserPluginClass, UserInstance) \
  __NGL_PLUGIN_CREATE(UserPluginClass, = UserInstance =)

#endif // _DOXYGEN_

#endif // __nglPlugin_h__
