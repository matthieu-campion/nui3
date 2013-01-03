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
#include "nglContext.h"


/*
#define NGL_CONTEXT_ENONE      0
*/
#define NGL_CONTEXT_ENOX       1
#define NGL_CONTEXT_ENOGLX     2
#define NGL_CONTEXT_ENOMATCH   3
#define NGL_CONTEXT_EGLCTX     4
#define NGL_CONTEXT_EBIND      5

const nglChar* gpContextErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("No X connection"),
/*  2 */ _T("GLX extension not found"),
/*  3 */ _T("No visual match your request"),
/*  4 */ _T("GL rendering context creation failed"),
/*  5 */ _T("Couldn't bind GL context to the system window"),
         NULL
};


/*
 * nglContextInfo
 */


bool nglContextInfo::Enum (uint Index, nglContextInfo& rInfo)
{
  return false;
}


#define ATTRIB_MAX 64
#define ATTRIB_PUSH1(a)   { if (pos >= ATTRIB_MAX) return NULL; attrib[pos++] = a; }
#define ATTRIB_PUSH2(a,b) { if (pos+1 >= ATTRIB_MAX) return NULL; attrib[pos++] = a; attrib[pos++] = b; }


/*
 * nglContext
 */

nglContext::nglContext()
{
  LOGI("nglContext::nglContext()");
  LOGI("nglContext::nglContext() OK");
}

nglContext::~nglContext()
{
}

bool nglContext::GetContextInfo(nglContextInfo& rInfo) const
{
  return false;
}

nglContext::GLExtFunc nglContext::LookupExtFunc (const char* pFuncName)
{
  return NULL;
}


const nglChar* nglContext::OnError (uint& rError) const
{
  return NULL;
}
