/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

class nuiRefCount
{
public:
  nuiRefCount() : mCount(0), mPermanent(false) 
  {
  }

  virtual ~nuiRefCount()           
  { 
    //NGL_ASSERT(mCount == 0); 
  }

  uint32 Acquire()    
  { 
    return ++mCount; 
  }

  uint32 Release()
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

  uint32 GetRefCount()
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
  uint32 mCount;
  bool mPermanent;
};

