/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglGuard class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T> class nglGuard
{
public:
  
  // Constructor(s) / Destructor
  nglGuard(T& rLock)
	: mrLock(rLock)
  {
    mrLock.Lock();
  }
  
  ~nglGuard()
  {
    mrLock.Unlock();
  }
  
private:
  
  // Private constructor
  nglGuard()
  {
  }
  
  nglGuard(const nglGuard & rGuard)
  {
  }
  
		// Operator
  nglGuard& operator= (const nglGuard& rGuard)
  {
  }
  
  // Data
  T &			mrLock;
};


template <class T> class nglUnGuard
{
public:

  // Constructor(s) / Destructor
  nglUnGuard(T& rLock)
    : mrLock(rLock)
  {
    mrLock.Unlock();
  }

  ~nglUnGuard()
  {
    mrLock.Lock();
  }

private:

  // Private constructor
  nglUnGuard()
  {
  }

  nglUnGuard(const nglUnGuard & rGuard)
  {
  }

  // Operator
  nglUnGuard& operator= (const nglUnGuard& rGuard)
  {
  }

  // Data
  T &			mrLock;
};
