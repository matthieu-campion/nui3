/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglGuard.h"
#include "nglThread.h"
#include "nglLock.h"
#include "nglAtomic.h"

// Declaration
class nglCriticalSectionPrivate;
class nglCriticalSection;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CriticalSection class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NGL_API nglCriticalSection : public nglLock
{
public:
  
  // Constructor(s) / Destructor
  nglCriticalSection(bool registerToThreadChecker = true);
  nglCriticalSection(const nglString& rName, bool registerToThreadChecker = true);
  virtual ~nglCriticalSection();

  virtual const nglString& GetLabel() const;

protected:

  // virtual from nglLock
  virtual void _Lock(nglThread::ID threadID);
  virtual void _Unlock();
  virtual bool _TryLock(nglThread::ID threadID);

#ifdef WIN32
  CRITICAL_SECTION& GetNativeHandle() const;
#endif
private:

  // Data
  nglCriticalSectionPrivate* mpPrivate;
};


// Typedef
typedef NGL_API nglGuard<nglCriticalSection> nglCriticalSectionGuard;
typedef NGL_API nglUnGuard<nglCriticalSection> nglCriticalSectionUnGuard;




