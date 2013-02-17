/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiMessageQueue::nuiMessageQueue()
  : mQueueCS(_T("nuiMessageQueueCS"))
{
}


nuiMessageQueue::~nuiMessageQueue()
{
}
  
bool nuiMessageQueue::Post(nuiNotification* notif)
{
  nglCriticalSectionGuard guard(mQueueCS);
  mQueue.push(notif);
  
  // unlock thread waiting to read the message
  mSyncEvent.Set ();
  return true;
}

nuiNotification* nuiMessageQueue::Get(uint32 time)
{
  // wait for an incoming message
  mSyncEvent.Wait(time);
  
  mQueueCS.Lock();
  if (mQueue.empty()) 
  {
    mQueueCS.Unlock();
    return NULL;
  }
  nuiNotification* notif = mQueue.front();
  mQueue.pop();
  
  // no more messages. next call to Get will block 'til another message is posted.
  if (mQueue.empty()) mSyncEvent.Reset();
  mQueueCS.Unlock();
  
  
  
  return notif; 
}
