/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

/* The nuiMemoryPool should not be used directly. See nuiMemoryPoolGuard and new (nuiMemoryPool) below.
 */
class NUI_API nuiMemoryPool
{
  friend class nuiMemoryPoolGuard;
public:
  nuiMemoryPool(size_t size);
  virtual ~nuiMemoryPool();
  
  size_t GetAvailable();
  
  /* Reserve memory for size bytes. Returns NULL if memory could not be reserved.
   The reserved memory space also includes a size_t for number of elements and a pointer to the templated Dtor function.
   Do not use directly unless you know what you are doing.
   */
  void* Allocate(size_t size);
  
  // Free memory up to pLimit. Fails an assert if pLimit does not point to an allocation boundary.
  // Do not use directly unless you know what you are doing.
  void Clear(uint8* pLimit = NULL);

protected:
  uint8* mpMemory;
  uint8* mpCurrent;
  size_t mSize;
};

/* The should be used to handle a memory pool.
 Start by creating a guard on the pool and perform allocations.
 When the guard is destroyed, it will roll back all allocations since the guard was created.
 This can be used to stack guards by scope.
 */
class NUI_API nuiMemoryPoolGuard
{
public:
  nuiMemoryPoolGuard(nuiMemoryPool& rPool);
  virtual ~nuiMemoryPoolGuard();
  
private:
  nuiMemoryPool& mrPool;
  uint8* mpMemory;
};


