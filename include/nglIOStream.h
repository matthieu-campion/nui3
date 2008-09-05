/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


/*!
\file  nglIOStream.h
\brief Input/Output data stream base class
*/

#ifndef __nglIOStream_h__
#define __nglIOStream_h__

//#include "nui.h"
#include "nglIStream.h"
#include "nglOStream.h"


//! Base class for input streams
/*!
This is a pure virtual class, all object capable of providing data should
inherit from nglIStream (see nglIFile for instance).
*/
class NGL_API nglIOStream : public nglOStream, public nglIStream
{
public:
  // This is just a simple new base class.
};

#endif //__nglIOStream_h__
