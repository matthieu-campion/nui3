/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "ThreadInspectorTest.h"
#include "nuiHBox.h"
#include "nuiAttributeEditor.h"
#include "nuiGradientDecoration.h"
#include "nuiColorDecoration.h"

#define NOTIF_LOCKA _T("NOTIF_LOCKA")
#define NOTIF_UNLOCKA _T("NOTIF_UNLOCKA")
#define NOTIF_LOCKB _T("NOTIF_LOCKB")
#define NOTIF_UNLOCKB _T("NOTIF_UNLOCKB")
#define NOTIF_LOCKC _T("NOTIF_LOCKC")
#define NOTIF_UNLOCKC _T("NOTIF_UNLOCKC")


ThreadInspectorTest::ThreadInspectorTest()
  : nuiVBox(0), mThreadCount(0), mEventSink(this)
{
  SetExpand(nuiExpandShrinkAndGrow);

  // explanations
  nuiLabel* pLabel = new nuiLabel(_T("Open the nuiIntrospector(SHIFT+CTRL+D), go to the Thread Inspector, run the Thread Checker, play with the test tool below, and see how the Thread Inspector reacts.\n\nIf you create a dead-lock, the nglThreadChecker will make the application exit, and dump usefull information on the standart output.\n"));
  pLabel->SetWrapping(true);
  AddCell(pLabel);
  SetCellMinPixels(0, 90);




  //***********************************
  // test tool

  nuiDecoration* pDeco = new nuiGradientDecoration(_T("ThreadInspectorTestDeco"),
    nuiRect(3, 3, 0, 0), nuiColor(246,246,246), nuiColor(220,220,220), nuiColor(210,210,210), nuiColor(255,255,255), nuiVertical, 1, nuiColor(180,180,180), eStrokeAndFillShape);


  // buttons
  pLabel = new nuiLabel(_T("test for Critical Sections locking"));
  pLabel->SetDecoration(pDeco, eDecorationBorder);
  AddCell(pLabel);

  nuiHBox* pBox = new nuiHBox(0);
  AddCell(pBox);

  nuiButton* pCreateBtn = new nuiButton(_T("Create CS Thread"));
  pBox->AddCell(pCreateBtn);
  mEventSink.Connect(pCreateBtn->Activated, &ThreadInspectorTest::CreateCSThread);
  nuiButton* pRemoveBtn = new nuiButton(_T("Terminate CS Thread"));
  pBox->AddCell(pRemoveBtn);
  mEventSink.Connect(pRemoveBtn->Activated, &ThreadInspectorTest::RemoveCSThread);

  // threads list
  mpCSList = new nuiList();
  AddCell(mpCSList);


  AddCell(NULL);
  SetCellPixels(GetNbCells()-1, 15);


  pLabel = new nuiLabel(_T("test for Light Locks locking"));
  pLabel->SetDecoration(pDeco, eDecorationBorder);
  AddCell(pLabel);

  pBox = new nuiHBox(0);
  AddCell(pBox);

  pCreateBtn = new nuiButton(_T("Create LL Thread"));
  pBox->AddCell(pCreateBtn);
  mEventSink.Connect(pCreateBtn->Activated, &ThreadInspectorTest::CreateLLThread);
  pRemoveBtn = new nuiButton(_T("Terminate LL Thread"));
  pBox->AddCell(pRemoveBtn);
  mEventSink.Connect(pRemoveBtn->Activated, &ThreadInspectorTest::RemoveLLThread);

  // threads list
  mpLLList = new nuiList();
  AddCell(mpLLList);

}


ThreadInspectorTest::~ThreadInspectorTest()
{
  std::map<nuiWidget*, TITCSthread*>::iterator it;
  for (it = mCSThreads.begin(); it != mCSThreads.end(); ++it)
  {
    TITCSthread* pThread = it->second;
    pThread->Stop();
  }

  std::map<nuiWidget*, TITLLthread*>::iterator it2;
  for (it2 = mLLThreads.begin(); it2 != mLLThreads.end(); ++it2)
  {
    TITLLthread* pThread = it2->second;
    pThread->Stop();
  }

}


void ThreadInspectorTest::CreateCSThread(const nuiEvent& rEvent)
{
  // create thread
  nglString name;
  name.Format(_T("CStest%d"), mThreadCount);
  mThreadCount++;
  TITCSthread* pThread = new TITCSthread(name);

  // create UI control in the list
  nuiHBox* pBox = new nuiHBox(0);
  mpCSList->AddChild(pBox);
  pBox->SetBorder(0, 3);

  // store the relation widget box -> thread
  mCSThreads[pBox] = pThread;

  pThread->Start();

  nglString label;
  label.Format(_T("CSthread '%ls' [0x%x]"), pThread->GetName().GetChars(), pThread->GetID());
  nuiLabel* pLabel = new nuiLabel(label);
  pBox->AddCell(pLabel);

  pBox->AddCell(pThread->InitGUI());

  rEvent.Cancel();
}


void ThreadInspectorTest::RemoveCSThread(const nuiEvent& rEvent)
{
  nuiWidget* pWidget = mpCSList->GetSelected();
  if (!pWidget)
  {
    rEvent.Cancel();
    return;
  }

  // retrieve thread associated to the selected widget from the list
  std::map<nuiWidget*, TITCSthread*>::iterator it = mCSThreads.find(pWidget);
  NGL_ASSERT(it != mCSThreads.end());

  // request stop to the thread
  TITCSthread* pThread = it->second;
  pThread->Stop();

  // clean
  delete pThread;
  mpCSList->DelChild(pWidget);
  mCSThreads.erase(it);

  rEvent.Cancel();
}




void ThreadInspectorTest::CreateLLThread(const nuiEvent& rEvent)
{
  // create thread
  nglString name;
  name.Format(_T("LLtest%d"), mThreadCount);
  mThreadCount++;
  TITLLthread* pThread = new TITLLthread(name);

  // create UI control in the list
  nuiHBox* pBox = new nuiHBox(0);
  mpLLList->AddChild(pBox);
  pBox->SetBorder(0, 3);

  // store the relation widget box -> thread
  mLLThreads[pBox] = pThread;

  pThread->Start();

  nglString label;
  label.Format(_T("LLthread '%ls' [0x%x]"), pThread->GetName().GetChars(), pThread->GetID());
  nuiLabel* pLabel = new nuiLabel(label);
  pBox->AddCell(pLabel);

  pBox->AddCell(pThread->InitGUI());

  rEvent.Cancel();
}


void ThreadInspectorTest::RemoveLLThread(const nuiEvent& rEvent)
{
  nuiWidget* pWidget = mpLLList->GetSelected();
  if (!pWidget)
  {
    rEvent.Cancel();
    return;
  }

  // retrieve thread associated to the selected widget from the list
  std::map<nuiWidget*, TITLLthread*>::iterator it = mLLThreads.find(pWidget);
  NGL_ASSERT(it != mLLThreads.end());

  // request stop to the thread
  TITLLthread* pThread = it->second;
  pThread->Stop();

  // clean
  delete pThread;
  mpLLList->DelChild(pWidget);
  mLLThreads.erase(it);

  rEvent.Cancel();
}



//*********************************************************************************
//
// TITthread
//
//*********************************************************************************
TITthread::TITthread(const nglString& rName)
  : mRunCS(_T("CSrunning"))
  , mLockCS(_T("CSlocking"))
  , mThread(nuiMakeDelegate(this, &TITthread::OnStart), rName)
{
  mRunning = false;

}

TITthread::~TITthread()
{


}

// virtual
void TITthread::Start()
{
  mRunning = true;
  mThread.Start();
}

void TITthread::Stop()
{
  SetLockedA(false);
  SetLockedB(false);
  SetLockedC(false);

  nglCriticalSectionGuard guard(mRunCS);
  mRunning = false;

  mThread.Join();
}

// virtual
void TITthread::OnStart()
{
  while (mRunning)
  {
    mThread.MsSleep(10);
    {
      nglCriticalSectionGuard guard(mRunCS);

      mLockedA = false;
      mLockedB = false;
      mLockedC = false;

      if (!WaitForLock())
        continue;

      // will block 'til the blocked cs
      while (mLockedA || mLockedB || mLockedC)
      {
        mThread.MsSleep(10);

        if (!WaitForLockOrUnlock())
          continue;
      }

    }

  }
}

const nglString& TITthread::GetName() const
{
  return mThread.GetName();
}

nglThread::ID TITthread::GetID() const
{
  return mThread.GetID();
}


bool TITthread::IsLockedA()
{
  return mLockedA;
}


void TITthread::SetLockedA(bool set)
{
  nglCriticalSectionGuard guard(mLockCS);

  if (set)
    mQueue.Post(new nuiNotification(NOTIF_LOCKA));
  else
    mQueue.Post(new nuiNotification(NOTIF_UNLOCKA));
}

bool TITthread::IsLockedB()
{
  return mLockedB;
}

void TITthread::SetLockedB(bool set)
{
  nglCriticalSectionGuard guard(mLockCS);

  if (set)
    mQueue.Post(new nuiNotification(NOTIF_LOCKB));
  else
    mQueue.Post(new nuiNotification(NOTIF_UNLOCKB));
}

bool TITthread::IsLockedC()
{
  return mLockedC;
}

void TITthread::SetLockedC(bool set)
{
  nglCriticalSectionGuard guard(mLockCS);

  if (set)
    mQueue.Post(new nuiNotification(NOTIF_LOCKC));
  else
    mQueue.Post(new nuiNotification(NOTIF_UNLOCKC));
}





//*********************************************************************************
//
// TITCSthread
//
//*********************************************************************************
TITCSthread::TITCSthread(const nglString& rName)
  : TITthread(rName)
{
  mLockedA = false;
  mLockedB = false;
  mLockedC = false;

  if (SetObjectClass(_T("TITCSThread")))
    InitAttributes();

}

TITCSthread::~TITCSthread()
{


}

void TITCSthread::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("LockedA")), nuiUnitOnOff,
     nuiMakeDelegate(this, &TITthread::IsLockedA),
     nuiMakeDelegate(this, &TITthread::SetLockedA)));

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("LockedB")), nuiUnitOnOff,
     nuiMakeDelegate(this, &TITthread::IsLockedB),
     nuiMakeDelegate(this, &TITthread::SetLockedB)));

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("LockedC")), nuiUnitOnOff,
     nuiMakeDelegate(this, &TITthread::IsLockedC),
     nuiMakeDelegate(this, &TITthread::SetLockedC)));
}


nuiWidget* TITCSthread::InitGUI()
{
  nuiHBox* pBox = new nuiHBox();
  nuiVBox* pVBox = new nuiVBox(2);
  pBox->AddCell(pVBox);

  nuiAttribBase Base(GetAttribute(_T("LockedA")));
  pVBox->SetCell(0, Base.GetEditor(), nuiCenter);
  pVBox->SetCell(1, new nuiLabel(_T("shared cs A"), nuiFont::GetFont(8)));

  pVBox = new nuiVBox(2);
  pBox->AddCell(pVBox);

  Base = GetAttribute(_T("LockedB"));
  pVBox->SetCell(0, Base.GetEditor(), nuiCenter);
  pVBox->SetCell(1, new nuiLabel(_T("shared cs B"), nuiFont::GetFont(8)));

  pVBox = new nuiVBox(2);
  pBox->AddCell(pVBox);

  Base = GetAttribute(_T("LockedC"));
  pVBox->SetCell(0, Base.GetEditor(), nuiCenter);
  pVBox->SetCell(1, new nuiLabel(_T("shared cs C"), nuiFont::GetFont(8)));

  return pBox;
}


bool TITCSthread::WaitForLock()
{
  nuiNotification* pNotif = mQueue.Get(0);
  if (!pNotif)
    return false;

  // the thread receives the order to lock one of the shared cs
  if (!pNotif->GetName().Compare(NOTIF_LOCKA))
  {
    mLockedA = true;
    mSharedCSA.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKB))
  {
    mLockedB = true;
    mSharedCSB.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKC))
  {
    mLockedC = true;
    mSharedCSC.Lock();
  }
  else
  {
    delete pNotif;
    return false;
  }

  delete pNotif;
  return true;
}


bool TITCSthread::WaitForLockOrUnlock()
{
  nuiNotification* pNotif = mQueue.Get(0);
  if (!pNotif)
    return false;


  // the thread receives the order to lock one of the shared cs
  if (!pNotif->GetName().Compare(NOTIF_LOCKA))
  {
    mLockedA = true;
    mSharedCSA.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKB))
  {
    mLockedB = true;
    mSharedCSB.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKC))
  {
    mLockedC = true;
    mSharedCSC.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_UNLOCKA))
  {
    mSharedCSA.Unlock();
    mLockedA = false;
  }
  else if (!pNotif->GetName().Compare(NOTIF_UNLOCKB))
  {
    mSharedCSB.Unlock();
    mLockedB = false;
  }
  else if (!pNotif->GetName().Compare(NOTIF_UNLOCKC))
  {
    mSharedCSC.Unlock();
    mLockedC = false;
  }
  else
  {
    delete pNotif;
    return false;
  }

  delete pNotif;
  return true;

}



// static
nglCriticalSection TITCSthread::mSharedCSA(nglString(_T("CSshared A")));
nglCriticalSection TITCSthread::mSharedCSB(nglString(_T("CSshared B")));
nglCriticalSection TITCSthread::mSharedCSC(nglString(_T("CSshared C")));












//*********************************************************************************
//
// TITLLthread
//
//*********************************************************************************
TITLLthread::TITLLthread(const nglString& rName)
  : TITthread(rName)
{
  mLockedA = false;
  mLockedB = false;
  mLockedC = false;

  if (SetObjectClass(_T("TITLLThread")))
    InitAttributes();
}

TITLLthread::~TITLLthread()
{


}

void TITLLthread::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("LockedA")), nuiUnitOnOff,
     nuiAttribute<bool>::GetterDelegate(this, &TITthread::IsLockedA),
     nuiAttribute<bool>::SetterDelegate(this, &TITthread::SetLockedA)));

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("LockedB")), nuiUnitOnOff,
     nuiAttribute<bool>::GetterDelegate(this, &TITthread::IsLockedB),
     nuiAttribute<bool>::SetterDelegate(this, &TITthread::SetLockedB)));

  AddAttribute(new nuiAttribute<bool>
    (nglString(_T("LockedC")), nuiUnitOnOff,
     nuiAttribute<bool>::GetterDelegate(this, &TITthread::IsLockedC),
     nuiAttribute<bool>::SetterDelegate(this, &TITthread::SetLockedC)));
}

nuiWidget* TITLLthread::InitGUI()
{
  nuiHBox* pBox = new nuiHBox();
  nuiVBox* pVBox = new nuiVBox(2);
  pBox->AddCell(pVBox);

  nuiAttribBase Base(GetAttribute(_T("LockedA")));
  pVBox->SetCell(0, Base.GetEditor(), nuiCenter);
  pVBox->SetCell(1, new nuiLabel(_T("shared ll A"), nuiFont::GetFont(8)));

  pVBox = new nuiVBox(2);
  pBox->AddCell(pVBox);

  Base = GetAttribute(_T("LockedB"));
  pVBox->SetCell(0, Base.GetEditor(), nuiCenter);
  pVBox->SetCell(1, new nuiLabel(_T("shared ll B"), nuiFont::GetFont(8)));

  pVBox = new nuiVBox(2);
  pBox->AddCell(pVBox);

  Base = GetAttribute(_T("LockedC"));
  pVBox->SetCell(0, Base.GetEditor(), nuiCenter);
  pVBox->SetCell(1, new nuiLabel(_T("shared ll C"), nuiFont::GetFont(8)));

  return pBox;
}



bool TITLLthread::WaitForLock()
{
  nuiNotification* pNotif = mQueue.Get(0);
  if (!pNotif)
    return false;

  // the thread receives the order to lock one of the shared cs
  if (!pNotif->GetName().Compare(NOTIF_LOCKA))
  {
    mLockedA = true;
    mSharedLLA.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKB))
  {
    mLockedB = true;
    mSharedLLB.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKC))
  {
    mLockedC = true;
    mSharedLLC.Lock();
  }
  else
  {
    delete pNotif;
    return false;
  }

  delete pNotif;
  return true;
}


bool TITLLthread::WaitForLockOrUnlock()
{
  nuiNotification* pNotif = mQueue.Get(0);
  if (!pNotif)
    return false;


  // the thread receives the order to lock one of the shared cs
  if (!pNotif->GetName().Compare(NOTIF_LOCKA))
  {
    mLockedA = true;
    mSharedLLA.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKB))
  {
    mLockedB = true;
    mSharedLLB.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_LOCKC))
  {
    mLockedC = true;
    mSharedLLC.Lock();
  }
  else if (!pNotif->GetName().Compare(NOTIF_UNLOCKA))
  {
    mSharedLLA.Unlock();
    mLockedA = false;
  }
  else if (!pNotif->GetName().Compare(NOTIF_UNLOCKB))
  {
    mSharedLLB.Unlock();
    mLockedB = false;
  }
  else if (!pNotif->GetName().Compare(NOTIF_UNLOCKC))
  {
    mSharedLLC.Unlock();
    mLockedC = false;
  }
  else
  {
    delete pNotif;
    return false;
  }

  delete pNotif;
  return true;

}



// static
nglLightLock TITLLthread::mSharedLLA(nglString(_T("LLshared A")));
nglLightLock TITLLthread::mSharedLLB(nglString(_T("LLshared B")));
nglLightLock TITLLthread::mSharedLLC(nglString(_T("LLshared C")));


