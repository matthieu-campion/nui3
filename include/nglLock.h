/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiNonCopyable.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglLock virtual class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NGL_API nglLock : nuiNonCopyable
{
public:
  
  virtual ~nglLock();
  
  // Methods
  void Lock();
  void Unlock();
  bool TryLock();
    
  virtual const nglString& GetLabel() const = 0;
  const nglString& GetName() const;

protected:

  nglLock(bool registerToThreadChecker);
  nglLock(const nglString& rName, bool registerToThreadChecker);


  virtual void _Lock(nglThread::ID threadID) = 0;
  virtual void _Unlock()=0;
  virtual bool _TryLock(nglThread::ID threadID) = 0;


private:

  // Data
  bool mRegisterToThreadChecker;
  nglString mName;
};





