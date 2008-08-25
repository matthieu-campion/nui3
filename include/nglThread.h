/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

class nglThreadPrivate;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglThread class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NGL_API nglThread
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
  nglThread(Priority priority = Normal); // Constructor
  nglThread(const nglString& rName, Priority priority = Normal);
  virtual ~nglThread(); // Destructor
  
  // Start / Stop
  bool Start(); ///< Start a paused thread
  bool Join(); ///< Wait until the thread is stopped
  Priority GetPriority();	///< Get thread priority
  bool SetPriority(Priority priority); ///< Set thread priority
  State GetState(); ///< Return the thread's state.
  bool IsCurrent(); ///< Return true if the call is made from this thread.
  ID GetID(); ///< Return thread ID
  const nglString& GetName() const;
  
  inline nglThreadPrivate* GetThreadPrivate() const
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
  
private:

  // Data
  volatile State mState; ///< Status
  Priority mPriority; ///< Priority
  nglThreadPrivate* mpData; ///< Plateform dependent data
  nglString mName;
};


// Functions
nglThread* nglGetThreadFromGlobalList(nglThread::ID ThreadID);
nglString nglGetThreadNameFromGlobalList(nglThread::ID ThreadID);
void nglAddThreadInGlobalList	(nglThread* pThread);
bool nglDelThreadFromGlobalList	(nglThread* pThread);


