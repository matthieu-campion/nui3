/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglLightLock class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



nglLightLock::nglLightLock(bool registerToThreadChecker)
  : nglLock(registerToThreadChecker)
{
	ngl_atomic_set(mAtomic, 0);
}


nglLightLock::nglLightLock(const nglString& rName, bool registerToThreadChecker)
  : nglLock(rName, registerToThreadChecker)
{
	ngl_atomic_set(mAtomic, 0);
}


nglLightLock::~nglLightLock()
{

}

// virtual from nglLock
const nglString& nglLightLock::GetLabel() const
{
  static const nglString lbl(_T("light lock"));
  return lbl;
}




// virtual from nglLock
void nglLightLock::_Lock(nglThread::ID threadID)
{
  // we use the current threadID to set the atomic : this we can have usefull information if a dead-lock happens.
  while (!ngl_atomic_compare_and_swap(mAtomic, 0, (nglAtomic)threadID))
  {
    nglThread::MsSleep(1);
  }
}

// virtual from nglLock
void nglLightLock::_Unlock()
{
	ngl_atomic_set(mAtomic, 0);
}


// virtual from nglLock
bool nglLightLock::_TryLock(nglThread::ID threadID)
{
  // we use the current threadID to set the atomic : this we can have usefull information if a dead-lock happens.
  return ngl_atomic_compare_and_swap(mAtomic, 0, (nglAtomic)threadID);
}












