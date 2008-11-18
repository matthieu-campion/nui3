/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRefCount_h__
#define __nuiRefCount_h__

#include "nui.h"

class NUI_API nuiRefCount
{
public:
  nuiRefCount() : mCount(0), mPermanent(false) 
  {
  }

  virtual ~nuiRefCount()           
  { 
    //NGL_ASSERT(mCount == 0); 
  }

  uint Acquire()    
  { 
    return ++mCount; 
  }

  uint Release()
  { 
    NGL_ASSERTR(mCount > 0, mCount); 
    mCount--;
    if (mCount == 0)
    {
      delete this;
      return 0;
    }
    return mCount; 
  }

  uint GetRefCount()
  {
    return mCount;
  }

  void SetPermanent(bool Permanent = true)
  {
    if (Permanent)
    {
      if (mPermanent)
        return;
      Acquire();
      mPermanent = true;
    }
    else
    {
      if (!mPermanent)
        return;
      mPermanent = false;
      Release();
    }
  }

  bool IsPermanent()
  {
    return mPermanent;
  }

private:
  uint mCount;
  bool mPermanent;
};

#endif // __nuiRefCount_h__
