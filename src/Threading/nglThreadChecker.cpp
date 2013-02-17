/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ThreadState class, for dead lock checker
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
nglThreadState::nglThreadState(nglLock* pLock, State state)
{
  mpLock = pLock;
  mState = state;
  mTimeStamp = nglTime();
  mWarning = eWarningNone;
}
  
nglThreadState::~nglThreadState()
{

}

nglLock* nglThreadState::GetLockPointer() const
{
  return mpLock;
} 

nglThreadState::State nglThreadState::GetState() const
{
  return mState;
}

nglString nglThreadState::GetStateToString() const
{
 nglString str;
  switch (mState)
  {
  case eWait:
    str = _T("Wait");
    break;
  case eLock:
    str = _T("Lock");
    break;
  default:
    str = _T("Error");
    break;
  };
  
  return str;
}

  
double nglThreadState::GetTimeStamp() const
{
  return mTimeStamp;
}

double nglThreadState::GetTimeLength(double currentTime) const
{
  return (currentTime - mTimeStamp);
}
  
nglThreadState::Warning nglThreadState::GetWarning() const
{
  return mWarning;
}

void nglThreadState::SetWarning(nglThreadState::Warning warning)
{
  mWarning = warning;
}

nglString nglThreadState::GetWarningToString() const
{
  nglString str;
  
  switch (mWarning)
  {
  case eWarningLong:
    str.Format(_T("long!"));
    break;
  case eWarningNone: break;
  }
  
  return str;
}










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglThreadChecker class, for dead lock checker
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


nglThreadChecker::nglThreadChecker()
  : nglThread("nglThreadChecker")
{
}

nglThreadChecker::~nglThreadChecker()
{

}


//*****************************************************************************************
//
// static public members
//
//*****************************************************************************************

void nglThreadChecker::EnableChecker(bool set, bool start)
{
  if (mEnabled == set)
    return;

  if (!set)
  {
    // stop checker
    if (mpChecker->IsRunning())
    {
      mpChecker->Stop();
      mpChecker->Join();
    }
    mEnabled = false;
  }    
  else
  {
    // start checker
    if (!mpChecker)
    {
      mpChecker = new nglThreadChecker();
    }
    if (start)
      mpChecker->Start();
    mEnabled = true;
  }
  
}

bool nglThreadChecker::IsRunning() const
{
  return mRunning;
}


bool nglThreadChecker::IsCheckerEnabled()
{
  return mEnabled;
}


void nglThreadChecker::SetCheckerThreshold(double threshold)
{
  mAtomicLock.Lock();
  mTreshold = threshold;
  mAtomicLock.Unlock();
}


void nglThreadChecker::EnableLongLockChecker(bool set)
{
  mAtomicLock.Lock();
  mLongLockEnabled = set;
  mAtomicLock.Unlock();
}


void nglThreadChecker::SetLongLockThreshold(double threshold)
{
  mAtomicLock.Lock();
  mLongLockTreshold = threshold;
  mAtomicLock.Unlock();
}


//**************************************************************************


void nglThreadChecker::AddRef()
{
  mCount++;
}

void nglThreadChecker::ReleaseRef()
{
  mCount--;
}


//**************************************************************************


void nglThreadChecker::RegisterThread(nglThread::ID ID, const nglString& rName)
{
  mNamesAtomic.Lock();
  mThreadNames[ID] = rName;
  mNamesAtomic.Unlock();


  if (mpChecker)
    mpChecker->_RegisterThread(ID);
}

bool nglThreadChecker::UnregisterThread(nglThread::ID ID)
{
  if (!mpChecker)
    return false;

  // remove thread'sname
  mNamesAtomic.Lock();
  std::map<nglThread::ID,nglString>::iterator it = mThreadNames.find(ID);
  if (it != mThreadNames.end())
    mThreadNames.erase(it);
  mNamesAtomic.Unlock();
    
  // remove thread's states
  return mpChecker->_UnregisterThread(ID);
}


//**************************************************************************


bool nglThreadChecker::Wait(nglThread::ID threadID, nglLock* pLock)
{
  if (!mEnabled)
    return false;
    
 return mpChecker->_Wait(threadID, pLock);
}

bool nglThreadChecker::Lock(nglThread::ID threadID, nglLock* pLock)
{
  if (!mEnabled)
    return false;
    
  return mpChecker->_Lock(threadID, pLock);
}

bool nglThreadChecker::Unlock(nglThread::ID threadID, nglLock* pLock)
{
  if (!mEnabled)
    return false;
  
  return mpChecker->_Unlock(threadID, pLock);
}



//**************************************************************************



bool nglThreadChecker::GetThreadName(nglThread::ID ID, nglString& rName)
{
  mNamesAtomic.Lock();
  std::map<nglThread::ID,nglString>::iterator it = mThreadNames.find(ID);
  if (it == mThreadNames.end())
  {
    mNamesAtomic.Unlock();
    return false;
  }
    
  rName = it->second;
  mNamesAtomic.Unlock();
  return true;
}


bool nglThreadChecker::GetStates(std::map<nglThread::ID, std::list<nglThreadState> >& states)
{
  if (!mpChecker)
    return false;
  
  mpChecker->_GetStates(states);
  return true;
}


//**************************************************************************
static nglString CheckerDisabled("nglThreadChecker disabled");

const nglString& nglThreadChecker::Dump()
{
  if (!mpChecker)
    return CheckerDisabled;

  mpChecker->mAtomicLock.Lock();
  if (!mpChecker->IsRunning())
    mpChecker->BuildLocksMap();
  const nglString& str = mpChecker->_Dump(nglTime());
  mpChecker->mAtomicLock.Unlock();

  return str;
}







//*****************************************************************************************
//
// private members
//
//*****************************************************************************************

const nglString& nglThreadChecker::_Dump(double currentTime)
{
  mDump = nglString::Empty;

  nglString tmp;
  mDump.Append(_T("\nnglThreadChecker DUMP:\n"));
  
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it;
  for (it = mThreadStates.begin(); it != mThreadStates.end(); ++it)
  {
    nglThread::ID threadID = it->first;
    nglString name;
    GetThreadName(threadID, name);
    const std::list<nglThreadState>& stack = it->second;
    
    tmp.CFormat(_T("thread '%s' [0x%x]:\n"), name.GetChars(), threadID);
    mDump.Append(tmp);
    
    uint32 size = stack.size();
    
    if (size == 0)
    {
      mDump.Append(_T("\t\t-> empty stack\n"));
    }
    else
    {
      
      std::list<nglThreadState>::const_iterator its = stack.end();
      do
      {
        --its;
        const nglThreadState& state = *its;
        
        tmp.CFormat(_T("\t\t%d: %s '%s' [0x%x] [state='%s'] [timelength=%.3f]\n"), 
          size,
          state.GetLockPointer()->GetLabel().GetChars(),
          state.GetLockPointer()->GetName().GetChars(),
          state.GetLockPointer(),
          state.GetStateToString().GetChars(),
          state.GetTimeLength(currentTime)); 
        mDump.Append(tmp);
          
        size--;
      }
      while (its != stack.begin());
    }
  }
  mDump.Append(_T("------------------------\n"));
  return mDump;
}



//**************************************************************************


void nglThreadChecker::Stop()
{
  mRunning = false;
}



//**************************************************************************
//
// NGLTHREADCHECKER PROCESS THREAD
//
//
//**************************************************************************

void nglThreadChecker::OnStart()
{
  mRunning = true;
  
  while (mRunning)
  {
    Sleep(1.0f);
    
    mAtomicLock.Lock();

    double currentTime = nglTime();
    
    Checker(currentTime);
    
    // atomic protection off
    mAtomicLock.Unlock();
  }
}




void nglThreadChecker::Checker(double currentTime)
{
  // we need to know in advance how to reach a thread from a given lock
  BuildLocksMap();
  
  // enum every registered thread
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it;
  for (it = mThreadStates.begin(); it != mThreadStates.end(); ++it)
  {
    nglThread::ID threadID = it->first;
    std::list<nglThreadState>& stack = it->second;
    nglString threadName;
    GetThreadName(threadID, threadName);
    
    
    //***********************************************************************************
    //
    // 1. Dead-lock Checker
    //
    //
    
    // get the last registered state, check if it's a "wait" state and check the timestamp
    if (stack.size() == 0)
      continue;
      
    nglThreadState& state = stack.back();
    
    // check time stamp
    if (state.GetState() == nglThreadState::eWait)
    {
      nglString log;
      FindDeadLock_1stPass(threadID, log);
      if (FindDeadLock_2ndPass(state.GetLockPointer(), log))
      {
       // it's a dead-lock. do what u have to do.        
        printf(_T("\nnglThreadChecker WARNING : dead-lock on %s '%s' [%p]!\n"), 
          state.GetLockPointer()->GetLabel().GetChars(), state.GetLockPointer()->GetName().GetChars(), state.GetLockPointer());
        printf(_T("%s\n"), log.GetChars());  
        _Dump(currentTime);
        printf(_T("%s\n"), mDump.GetChars());
        
        // if a dead-lock blocks the main thread, the UI won't be able to refresh any information about that. we choose to exit with a clear information.
        exit(2);
      }
    }
    

    
    if (!mLongLockEnabled)
      continue;

    //***********************************************************************************
    //
    // 2. long locks Checker
    //
    //
    
    // enum every registered states for this thread
    std::list<nglThreadState>::iterator its;
    for (its = stack.begin(); its != stack.end(); ++its)
    {
      nglThreadState& state = *its;
      
      // check time stamp
      double delay = currentTime - state.GetTimeStamp();
      if (delay > mLongLockTreshold)
      {
        state.SetWarning(nglThreadState::eWarningLong);
        // it's a long lock. do what u have to do.
        printf(_T("nglThreadChecker WARNING!\n"));
        printf(_T("nglThreadChecker WARNING : the %s '%s' [%p] has been locked for more than %.3fsec in thread '%s' [%p]\n"), 
        state.GetLockPointer()->GetLabel().GetChars(),
        state.GetLockPointer()->GetName().GetChars(), state.GetLockPointer(), delay, threadName.GetChars(), threadID);
        _Dump(currentTime);
        printf(_T("%s\n"), mDump.GetChars());
      }
    }

    
  }

}




//**************************************************************************



//******************************************************
//
// build a map of locked locks (critical sections and light locks) -> corresponding threads
//
void nglThreadChecker::BuildLocksMap()
{
  mLockmap.clear();
  
  // enum every thread
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it;
  for (it = mThreadStates.begin(); it != mThreadStates.end(); ++it)
  {
    nglThread::ID threadID = it->first;
    const std::list<nglThreadState>& list = it->second;

    // enum every locked lock
    std::list<nglThreadState>::const_iterator its;
    for (its = list.begin(); its != list.end(); ++its)
    {
      const nglThreadState& state = *its;
      if (state.GetState() == nglThreadState::eLock)
        mLockmap[state.GetLockPointer()] = threadID;
    }
  }
}



//**************************************************************************


  
//******************************************************
//
// FindDeadLock : 1st pass
// initialise the set of tested thread for the 2nd pass
//
void nglThreadChecker::FindDeadLock_1stPass(nglThread::ID waitingThread, nglString& log)
{
  mTestedThread.clear();
 
  // consider the 2nd pass starts with the current waiting thread
  nglString tmp;
  nglString name;
  GetThreadName(waitingThread, name);
  tmp.Format(_T("thread '%s' [0x%x] "), name.GetChars(), waitingThread);
  log.Append(tmp);
  mTestedThread.insert(waitingThread);
}




//******************************************************
//
// FindDeadLock : 2nd pass
// follow the road 
//
bool nglThreadChecker::FindDeadLock_2ndPass(nglLock* pWaitingLock, nglString& log)
{
  // retrieve corresponding thread
  std::map<nglLock*, nglThread::ID>::iterator itm = mLockmap.find(pWaitingLock);
  if (itm == mLockmap.end())
  {
    nglString tmp;
    tmp.CFormat("Error: unable to find lock %p '%s' (%s)", pWaitingLock, pWaitingLock->GetName().GetChars(), pWaitingLock->GetLabel().GetChars());
    return false;
  }

  NGL_ASSERT(itm != mLockmap.end());
  nglThread::ID nextThread = itm->second;
  nglString nextName;
  GetThreadName(nextThread, nextName);
  
  // has this thread alreay been tested? yes => dead-lock
  std::set<nglThread::ID>::iterator its = mTestedThread.find(nextThread);
  if (its != mTestedThread.end())
  {
    nglString tmp;
    tmp.Format(_T(" is waiting for thread '%s' [0x%x]. And there is the dead-lock!\n"), nextName.GetChars(), nextThread);
    log.Append(tmp);
    
    return true;
  }
    
  // otherwise, continue to search, and remember this thread has been tested
  mTestedThread.insert(nextThread);

  // take last locked cs from this thread
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it = mThreadStates.find(nextThread);
  NGL_ASSERT(it != mThreadStates.end());
  const std::list<nglThreadState>& list = it->second;

  // no cs, no dead-lock, test over.
  if (list.size() == 0)
  {
    nglString tmp;
    tmp.Format(_T(" is waiting for thread '%s' [0x%x], no locks in this thread. No Dead-lock then. Test Over.\n"), nextName.GetChars(), nextThread);
    log.Append(tmp);

    return false;
  }
    
  const nglThreadState& state = list.back();

  // the target thread is not waiting. there is no dead-lock!
  if (state.GetState() != nglThreadState::eWait)
  {
    nglString tmp;
    tmp.Format(_T(" is waiting for thread '%s' [0x%x]. In this thread, %s '%s' [0x%x] has already been locked. No Dead-lock then. Test Over.\n"), 
      nextName.GetChars(), nextThread, 
      state.GetLockPointer()->GetLabel().GetChars(),
      state.GetLockPointer()->GetName().GetChars(), state.GetLockPointer());
    log.Append(tmp);

    return false;
  }
    

  // recursive call

  nglString tmp;
  tmp.Format(_T(" is waiting for thread '%s' [0x%x].\nthread '%s' [0x%x] "), nextName.GetChars(), nextThread, nextName.GetChars(), nextThread);
  log.Append(tmp);

  return FindDeadLock_2ndPass(state.GetLockPointer(), log);
  
}












//******************************************************************************************
//
// private members
//
//******************************************************************************************


bool nglThreadChecker::_Wait(nglThread::ID threadID, nglLock* pLock)
{
  mAtomicLock.Lock();

  // retrieve the stack associated to the thread
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it = mThreadStates.find(threadID);
  if (it != mThreadStates.end())
  {
    std::list<nglThreadState>& stack = it->second;
    
    // register the information
    nglThreadState state(pLock, nglThreadState::eWait);
    stack.push_back(state);
  }
  else
  {
    // no stack created yet for this thread. do it now and register the information
    std::list<nglThreadState> stack;
    nglThreadState state(pLock, nglThreadState::eWait);
    stack.push_back(state);
    mThreadStates[threadID] = stack;
  }
  
  mAtomicLock.Unlock();
  return true;
}


bool nglThreadChecker::_Lock(nglThread::ID threadID, nglLock* pLock)
{

  mAtomicLock.Lock();

  // retrieve the stack associated to the thread
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it = mThreadStates.find(threadID);
  
  
  if (it != mThreadStates.end())
  {
    std::list<nglThreadState>& stack = it->second;

    //*****************************
    //
    // 1. unregister the wait state
    //
    {
      // search for the information in the stack
      if (stack.size() > 0)
      {
        std::list<nglThreadState>::iterator its = stack.end();
        do
        {
          --its;
          const nglThreadState& state = *its;
          
          if ((state.GetLockPointer() == pLock) && (state.GetState() == nglThreadState::eWait))
          {
            // information found, unregister the previous state
            stack.erase(its);
            break;
          }
        }
        while (its != stack.begin()); 
      }


    }
    
    //*******************************
    //
    // 2. register the lock state
    //

    nglThreadState state(pLock, nglThreadState::eLock);
    stack.push_back(state);

  }  

  else
  {

    // no stack created yet for this thread. do it now and register the information
    std::list<nglThreadState> stack;
    nglThreadState state(pLock, nglThreadState::eLock);
    stack.push_back(state);
    mThreadStates[threadID] = stack;

  }

  mAtomicLock.Unlock();

  return true;
}


bool nglThreadChecker::_Unlock(nglThread::ID threadID, nglLock* pLock)
{
  // atomic synchronisation
  mAtomicLock.Lock();


  // retrieve the stack associated to the thread
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it = mThreadStates.find(threadID);
  
  
  if (it == mThreadStates.end())
  {
    mAtomicLock.Unlock();
    return false;
  }
  
  // unregister the lock state
  std::list<nglThreadState>& stack = it->second;


  // search for the information in the stack
  if (stack.size() > 0)
  {
    std::list<nglThreadState>::iterator its = stack.end();
    do
    {
      --its;
      const nglThreadState& state = *its;
      
      if ((state.GetLockPointer() == pLock) && (state.GetState() == nglThreadState::eLock))
      {
        // information found, unregister the previous state
        stack.erase(its);

        // wprintf(_T("ThreadChecker Unlock : threadID 0x%x   CriticalSection 0x%x (named '%s')\n"), threadID, pCS, pCS->GetName().GetChars());

        break;
      }
    }
    while (its != stack.begin()); 
  }


  // atomic protection off
  mAtomicLock.Unlock();
  return true;
}



//*************************************************************************************


void nglThreadChecker::_RegisterThread(nglThread::ID threadID)
{
  mAtomicLock.Lock();

  // no stack created yet for this thread. do it now and register the information
  std::list<nglThreadState> stack;
  mThreadStates[threadID] = stack;

  mAtomicLock.Unlock();
}


bool nglThreadChecker::_UnregisterThread(nglThread::ID ID)
{
  // atomic synchronisation
  mAtomicLock.Lock();
  
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it = mThreadStates.find(ID);
  if (it == mThreadStates.end())
  {
    mAtomicLock.Unlock();
    return false;
  }

  mThreadStates.erase(it);

  mAtomicLock.Unlock();
  return true;
}



//******************************************************************************************


void nglThreadChecker::_GetStates(std::map<nglThread::ID, std::list<nglThreadState> >& states)
{
  mAtomicLock.Lock();
  states = mThreadStates;
  mAtomicLock.Unlock();
}







//**************************************************************************************
//
// static attributes
//
//

bool nglThreadChecker::mEnabled = false;
bool nglThreadChecker::mLongLockEnabled = false;

double nglThreadChecker::mTreshold = 10.0f;
double nglThreadChecker::mLongLockTreshold = 10.0f;


nglThreadChecker* nglThreadChecker::mpChecker = NULL;

uint32 nglThreadChecker::mCount = 0;
nglLightLock nglThreadChecker::mAtomicLock(false /* do not register by the ThreadChecker*/);

std::map<nglThread::ID, nglString> nglThreadChecker::mThreadNames;
nglLightLock nglThreadChecker::mNamesAtomic(false /* do not register by the ThreadChecker*/);







