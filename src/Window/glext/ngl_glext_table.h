/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/* This internal include defines the structures for the GL extensions tables */

#ifndef __ngl_glext_table_h__
#define __ngl_glext_table_h__

#include "nglContext.h"


typedef struct
{
  const char* pName;
  uint        Index;
} GLExtNameInfo;


typedef void (*nglContext::*GLExtFuncPtr)(void);

typedef struct
{
  const char*  pName;
  GLExtFuncPtr pFunc;
} GLExtFuncInfo;

#endif // __ngl_glext_table_h__
