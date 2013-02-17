/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"


nglError::nglError()
{
  mError = 0;
}

nglError::~nglError()
{
}

uint nglError::GetError() const
{
  return mError;
}

const nglChar* nglError::GetErrorStr() const
{
  return GetErrorStr(GetError());
}

const nglChar* nglError::GetErrorStr(uint Error) const
{
  return OnError(Error);
}


/*
 * Programmer services
 */

bool nglError::SetError(uint Error) const
{
  return InternalSetError(0, Error, false);
}

bool nglError::SetError(const nglChar* pLogDomain, uint Error) const
{
  return InternalSetError(pLogDomain, Error, true);
}


/*
 * Internals
 */

bool nglError::InternalSetError (const nglChar* pLogDomain, uint Error, bool UseDomain) const
{
  uint err = Error;
  const nglChar* pError = OnError(err);
  if (!pError)
    return false;

  mError = Error;
#ifdef _DEBUG_
  if (App && UseDomain && (mError > 0)) NGL_LOG(pLogDomain, NGL_LOG_ERROR, _T("Error: %s\n"), pError);
#endif
  return true;
}

const nglChar* nglError::FetchError (const nglChar** pTable, const nglChar* pParent, uint& rError) const
{
  if (pParent)
    return pParent;

  const nglChar** table = pTable;
  while (rError > 0 && *table)
  {
    rError--;
    table++;
  }

  return *table;
}
