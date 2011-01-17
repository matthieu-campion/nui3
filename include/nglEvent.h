/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglEvent.h
\brief Event emitter/receiver (NGL internals)
*/

#ifndef __nglEvent_h__
#define __nglEvent_h__

//#include "nui.h"


//! Base class for objects which generate events
/*!
This class is for NGL internal use only. It provides no service to the user.
*/
class NGL_API nglEvent
{
#ifdef _UNIX_
public:
  static const uint Read;
  static const uint Write;
  static const uint Error;
  static const uint Idle;

  nglEvent();
  virtual ~nglEvent();

  uint    GetFD()      { return mFD; }
  uint    GetFlags()   { return mFlags; }

  void    CallOnEvent(uint Flags);

protected:
  int     mFD;
  uint    mFlags;

  virtual void OnEvent(uint Flags) = 0;

private:
  nglEvent(const nglEvent&) {} // Undefined copy constructor
#else
public:
  virtual ~nglEvent() {}
#endif // _UNIX_
};

#endif // __nglEvent_h__
