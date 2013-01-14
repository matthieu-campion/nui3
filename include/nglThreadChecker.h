/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once


#include "nui.h"
#include "nglThread.h"
#include "nglLightLock.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ThreadState class, for dead lock checker
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class nglThreadState
{
public:

  enum State
  {
    eWait=0,
    eLock
  };

  enum Warning
  {
    eWarningNone=0,
    eWarningLong
  };

  nglThreadState(nglLock* pLock, State state);
  ~nglThreadState();

  nglLock* GetLockPointer() const;
  State GetState() const;
  nglString GetStateToString() const;
  double GetTimeStamp() const;
  double GetTimeLength(double currentTime) const;
  Warning GetWarning() const;
  void SetWarning(Warning warning);
  nglString GetWarningToString() const;


private:

  nglLock* mpLock;
  State mState;
  Warning mWarning;
  double mTimeStamp;
};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglThreadChecker class, for dead lock checker
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class nglThreadChecker : public nglThread
{
public:

  //**********************************************
  // interface for applications
  //
  static void EnableChecker(bool set, bool start = true);                ///< main method. enable/disable the nglThreadChecker. it's disabled by default.
  static bool IsCheckerEnabled();                
  static void SetCheckerThreshold(double threshold);  ///< set the timeout value in seconds for dead-locks checker. default is 10s.
  static void EnableLongLockChecker(bool set);        ///< enable/disable the checker for long lock operation. it's disabled by default.
  static void SetLongLockThreshold(double threshold); ///< set the timeout value in seconds for long locks checker. default is 10s.
  static const nglString& Dump();                     ///< dump the checker contents in a output string

  
  //**********************************************
  // interface for internal nui usage 
  //
  static bool Lock(nglThread::ID threadID, nglLock* pLock);
  static bool Wait(nglThread::ID threadID, nglLock* pLock);
  static bool Unlock(nglThread::ID threadID, nglLock* pLock);

  static void AddRef();
  static void ReleaseRef();
  
  static void RegisterThread(nglThread::ID ID, const nglString& rName);
  static bool UnregisterThread(nglThread::ID ID);
  
  static bool GetStates(std::map<nglThread::ID, std::list<nglThreadState> >& states); ///< get map threadID->list of locks (critical sections or light locks) states
  static bool GetThreadName(nglThread::ID ID, nglString& rName);
  
private:
  nglThreadChecker();
  virtual ~nglThreadChecker();

  void Stop();

  bool _Lock(nglThread::ID threadID, nglLock* pLock);
  bool _Wait(nglThread::ID threadID, nglLock* pLock);
  bool _Unlock(nglThread::ID threadID, nglLock* pLock);

  void _RegisterThread(nglThread::ID ID);
  bool _UnregisterThread(nglThread::ID ID);


  void _GetStates(std::map<nglThread::ID, std::list<nglThreadState> >& states);

  const nglString& _Dump(double currentTime);

  void BuildLocksMap();
  void FindDeadLock_1stPass(nglThread::ID waitingThread, nglString& log);
  bool FindDeadLock_2ndPass(nglLock* pWaitingLock, nglString& log);


  virtual void OnStart(); ///< Main thread method to process the checkers
  void Checker(double currentTime);
  
  bool IsRunning() const;
  bool mRunning;
  
  std::map<nglThread::ID, std::list<nglThreadState> > mThreadStates; 

  static nglLightLock mAtomicLock; ///< to protect the mEnabled attribute reading/writing
  static std::map<nglThread::ID, nglString> mThreadNames;
  static nglLightLock mNamesAtomic; ///< to protect the mThreadNames reading/writing
  
  static uint32 mCount;
  
  // static, for dead lock checker
  static bool mEnabled;
  static double mTreshold;
  static bool mLongLockEnabled;
  static double mLongLockTreshold;
  
  static nglThreadChecker* mpChecker;
  std::map<nglLock*, nglThread::ID> mLockmap;
  std::set<nglThread::ID> mTestedThread;

  nglString mDump;
};



