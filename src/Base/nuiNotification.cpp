/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiNotification::nuiNotification(const nglString& rName)
{
  SetObjectName(rName);
}

const nglString& nuiNotification::GetName() const
{
  return GetObjectName();
}

//////////////////////////////////////////
nuiNotificationObserver::nuiNotificationObserver()
{
  mInOperation = false;
}

nuiNotificationObserver::~nuiNotificationObserver()
{
  UnregisterAll();
}

void nuiNotificationObserver::RegisterWithManager(nuiNotificationManager* pNotificationManager, const nglString& rNotificationName)
{
  mInOperation = true;
  pNotificationManager->RegisterObserver(rNotificationName, this);
  mNotificationManagers[pNotificationManager]++;
  mInOperation = false;
}

void nuiNotificationObserver::UnregisterAll()
{
  mInOperation = true;
  std::map<nuiNotificationManager*, int>::iterator it = mNotificationManagers.begin();
  std::map<nuiNotificationManager*, int>::iterator end = mNotificationManagers.end();

  while (it != end)
  {
    nuiNotificationManager* pManager = it->first;
    pManager->UnregisterObserver(this, nglString::Null);

    ++it;
  }
  mNotificationManagers.clear();
  mInOperation = false;
}

void nuiNotificationObserver::UnregisterManager(nuiNotificationManager* pManager)
{
  if (mInOperation)
    return;

  mInOperation = true;
  
  std::map<nuiNotificationManager*, int>::iterator it = mNotificationManagers.find(pManager);
  std::map<nuiNotificationManager*, int>::iterator end = mNotificationManagers.end();

  if (it != end)
  {
    nuiNotificationManager* pManager = it->first;
    pManager->UnregisterObserver(this, nglString::Null);
    if (!(it->second)--)
      mNotificationManagers.erase(it++);
  }
  
  mInOperation = false;
}

//////////////////////////////////////////
nuiNotificationManager::nuiNotificationManager()
{
}

nuiNotificationManager::~nuiNotificationManager()
{
  Clear();
}

void nuiNotificationManager::PostNotification(nuiNotification* pNotification)
{
  mQueue.Post(pNotification);
}

void nuiNotificationManager::BroadcastNotification(const nuiNotification& rNotification)
{
  std::map<nglString, std::set<nuiNotificationObserver*> >::iterator _it  = mObservers.find(rNotification.GetObjectName());
  if (_it != mObservers.end())
  {
    std::set<nuiNotificationObserver*> rObservers(_it->second);
    std::set<nuiNotificationObserver*>::iterator it  = rObservers.begin();
    std::set<nuiNotificationObserver*>::iterator end = rObservers.end();

    while (it != end)
    {
      nuiNotificationObserver* pObserver = *it;
      pObserver->OnNotification(rNotification);
      ++it;
    }
  }
}

void nuiNotificationManager::BroadcastQueuedNotifications()
{
  nuiNotification* pNotification = NULL;
  while ((pNotification = mQueue.Get(0)))
  {
    BroadcastNotification(*pNotification);
    delete pNotification;
  }
}

void nuiNotificationManager::Clear()
{
  nuiNotification* pNotification = NULL;
  while ((pNotification = mQueue.Get(0)))
  {
    delete pNotification;
  }
}

void nuiNotificationManager::RegisterObserver(const nglString& rNotificationName, nuiNotificationObserver* pObserver)
{
  mObservers[rNotificationName].insert(pObserver);
}

void nuiNotificationManager::UnregisterObserver(nuiNotificationObserver* pObserver, const nglString& rNotificationName)
{
  if (rNotificationName.IsNull())
  {
    std::map<nglString, std::set<nuiNotificationObserver*> >::iterator it  = mObservers.begin();
    std::map<nglString, std::set<nuiNotificationObserver*> >::iterator end = mObservers.end();
    
    while (it != end)
    {
      std::set<nuiNotificationObserver*>& rObservers = it->second;
      rObservers.erase(pObserver);
      pObserver->UnregisterManager(this);
      ++it;
    }
  }
  else
  {
    std::map<nglString, std::set<nuiNotificationObserver*> >::iterator it  = mObservers.find(rNotificationName);
    if (it != mObservers.end())
    {
      std::set<nuiNotificationObserver*>& rObservers = it->second;
      rObservers.erase(pObserver);
      pObserver->UnregisterManager(this);
    }
  }
}



