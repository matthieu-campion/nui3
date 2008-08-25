/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglModule.h
\brief Runtime dynamic library management
*/

#ifndef __nglModule_h__
#define __nglModule_h__

//#include "nui.h"
#include "nglError.h"
#include "nglPath.h"


// Error codes
#define NGL_MODULE_ENONE     0
#define NGL_MODULE_EPATH     1
#define NGL_MODULE_ELOAD     2


//! Runtime dynamic library management
/*!
This class can load a relocatable object file (DLL) at runtime and query
its symbol's addresses.
*/
class NGL_API nglModule : public nglError
{
public:
  /** @name Life cycle */
  //@{
  nglModule(const nglPath& rPath);
  /*!<
    \param rPath relocatable object file path

    The module will not be loaded until Load() is called. If \a rPath is
    a relative path, system specific automatic research mecanisms are used.
  */
  virtual ~nglModule();
  /*!<
    If a module was loaded, automacally unload it (or more exactly, unreference
    it).
  */
  //@}

  /** @name Loading & unloading */
  //@{
  bool Load();      ///< Load the module. Returns true if succesfull
  bool Unload();    ///< Unload the module. Returns true if succesfull
  bool IsLoaded();  ///< Return true if the module is loaded and GetSymbol() is usable
  //@}

  /** @name Symbol retrieval */
  //@{
  void* GetSymbol(const char* pName);
  /*!< Query a symbol's address
    \param pName symbol name
    \return the address of the symbol, NULL if not found
  */
  //@}

protected:
  virtual const nglChar* OnError (uint& rError) const;

private:
  nglPath   mPath;
#ifdef _WIN32_
  HINSTANCE mHandle;
#else
  void*     mHandle;
#endif // _WIN32_

  nglModule(const nglModule&) {} // Undefined copy constructor
};

#endif // __nglModule_h__
