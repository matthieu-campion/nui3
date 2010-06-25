/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "nuiList.h"
#include "nuiMessageQueue.h"
#include "nuiAttributeEditor.h"
#include "nglLightLock.h"


//********************************************************************
//
// TITthread : implement a generic class for a thread
//
//*********************************************************************
class TITthread : public nuiObject
{
public:

  virtual ~TITthread();

  void Start();
  void Stop();

  // attributes
  bool IsLockedA();
  void SetLockedA(bool set);
  bool IsLockedB();
  void SetLockedB(bool set);
  bool IsLockedC();
  void SetLockedC(bool set);
         
  const nglString& GetName() const;
  nglThread::ID	GetID() const;
protected:

  TITthread(const nglString& rName);

  bool mLockedA;
  bool mLockedB;
  bool mLockedC;

  nuiMessageQueue mQueue;

private:
  nglThreadDelegate mThread;

  virtual void OnStart();
  virtual bool WaitForLock()=0;
  virtual bool WaitForLockOrUnlock() =0;

  bool mRunning;
  nglCriticalSection mRunCS;
  nglCriticalSection mLockCS;
  

};


//********************************************************************
//
// TITCSthread : inherits from TITthread to test the critical sections locking with the nuiThreadInspector
//
//*********************************************************************
class TITCSthread : public TITthread
{
public:
  TITCSthread(const nglString& rName);
  virtual ~TITCSthread();
  void InitAttributes();

  nuiWidget* InitGUI();
private:

  virtual bool WaitForLock();
  virtual bool WaitForLockOrUnlock();

  static nglCriticalSection mSharedCSA;
  static nglCriticalSection mSharedCSB;
  static nglCriticalSection mSharedCSC;

};



//********************************************************************
//
// TITCSthread : inherits from TITthread to test the light locks locking with the nuiThreadInspector
//
//*********************************************************************
class TITLLthread : public TITthread
{
public:
  TITLLthread(const nglString& rName);
  virtual ~TITLLthread();
  void InitAttributes();

  nuiWidget* InitGUI();
private:

  virtual bool WaitForLock();
  virtual bool WaitForLockOrUnlock();

  static nglLightLock mSharedLLA;
  static nglLightLock mSharedLLB;
  static nglLightLock mSharedLLC;

};



//********************************************************************
//
// ThreadInspectorTest : main widget
//
//*********************************************************************
class ThreadInspectorTest : public nuiVBox
{
public:
  ThreadInspectorTest();
  virtual ~ThreadInspectorTest();
  
private:

  bool CreateCSThread(const nuiEvent& rEvent);
  bool RemoveCSThread(const nuiEvent& rEvent);
  bool CreateLLThread(const nuiEvent& rEvent);
  bool RemoveLLThread(const nuiEvent& rEvent);
  
  nuiList* mpCSList;
  nuiList* mpLLList;
  std::map<nuiWidget*, TITCSthread*> mCSThreads;
  std::map<nuiWidget*, TITLLthread*> mLLThreads;
  uint32 mThreadCount;
  
  nuiEventSink<ThreadInspectorTest> mEventSink;

};


