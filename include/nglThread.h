/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiFastDelegate.h"
#include "nuiNonCopyable.h"

class nglThreadPrivate;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglThread class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NGL_API nglThread : nuiNonCopyable
{
  friend class nglThreadPrivate;
public:

  // Typedef
#ifdef WINCE
  typedef	DWORD		ID;
#elif defined _WIN32_
  typedef	DWORD		ID;
#else
  typedef	pthread_t	ID;
#endif

  // Enum
  enum Priority
  {
    Lowest = 0,
    Low,
    Normal,
    High,
    Highest,
    Critical
  };

  enum State
  {
    Running,
    Stopped,
    Closed
  };

  // Constructor(s) / Destructor
  nglThread(Priority priority = Normal, size_t StackSize = 0); // Constructor
  nglThread(const nglString& rName, Priority priority = Normal, size_t StackSize = 0);
  virtual ~nglThread(); // Destructor

  // Start / Stop
  bool Start(); ///< Start a paused thread
  bool Join(); ///< Wait until the thread is stopped
  Priority GetPriority() const;	///< Get thread priority
  bool SetPriority(Priority priority); ///< Set thread priority
  State GetState() const; ///< Return the thread's state.
  bool IsCurrent() const; ///< Return true if the call is made from this thread.
  ID GetID() const; ///< Return thread ID
  const nglString& GetName() const;
  void SetAutoDelete(bool set);
  bool GetAutoDelete() const;

  nglThreadPrivate* GetThreadPrivate() const
	{
    return mpData;
  }

  // Main method
  virtual void OnStart(); ///< Main thread method

  // Static methods
  static void Sleep(uint32 secs); ///< Sleep (seconds)
  static void MsSleep(uint32 msecs); ///< Sleep (milliseconds)
  static void USleep(uint32 usecs); ///< Sleep (microseconds)
  static nglThread* GetCurThread(); ///< Return current thread
  static ID GetCurThreadID(); ///< Return current thread ID (system calls)

  size_t GetStackSize() const
  {
    return mStackSize;
  }

private:

  // Data
  volatile State mState; ///< Status
  Priority mPriority; ///< Priority
  nglThreadPrivate* mpData; ///< Plateform dependent data
  nglString mName;
  bool mAutoDelete;
  size_t mStackSize;
};


class NGL_API nglThreadDelegate : public nglThread
{
public:
  typedef nuiFastDelegate0<> ThreadDelegate;
  nglThreadDelegate(const ThreadDelegate& rStartFunction, Priority priority = Normal, size_t StackSize = 0); // Constructor
  nglThreadDelegate(const ThreadDelegate& rStartFunction, const nglString& rName, Priority priority = Normal, size_t StackSize = 0);
  virtual ~nglThreadDelegate(); // Destructor

  virtual void OnStart(); ///< Main thread method

private:
  ThreadDelegate mDelegate;
};


// Functions
nglThread* nglGetThreadFromGlobalList(nglThread::ID ThreadID);
nglString nglGetThreadNameFromGlobalList(nglThread::ID ThreadID);
void nglAddThreadInGlobalList	(nglThread* pThread);
bool nglDelThreadFromGlobalList	(nglThread* pThread);


