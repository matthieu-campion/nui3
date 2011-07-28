/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiNotification_h__
#define __nuiNotification_h__

#include "nuiObject.h"
#include "nuiMessageQueue.h"

class nuiNotificationManager;

class NUI_API nuiNotification : public nuiObject
{
public:
  nuiNotification(const nglString& rName);
  
  const nglString& GetName() const;

};

class NUI_API nuiNotificationObserver
{
public:
  nuiNotificationObserver();
  virtual ~nuiNotificationObserver();
  
  virtual void OnNotification(const nuiNotification& rNotification) = 0;
  void RegisterWithManager(nuiNotificationManager* pNotificationManager, const nglString& rNotificationName);
  void UnregisterAll();
  void UnregisterManager(nuiNotificationManager* pManager);
private:
  std::map<nuiNotificationManager*, int> mNotificationManagers;
  bool mInOperation;
  
};

class NUI_API nuiNotificationManager : public nuiRefCount
{
public:
  nuiNotificationManager();
  virtual ~nuiNotificationManager();
  
  void PostNotification(nuiNotification* pNotification); ///< Put this notification in a queue in order to broadcast when the system feels like it.
  void BroadcastNotification(const nuiNotification& rNotification); ///< Send this notification now to all registered observers.
  void BroadcastQueuedNotifications(); ///< Broadcast all the notifications that have been queued up to now.
  void RegisterObserver(const nglString& rNotificationName, nuiNotificationObserver* pObserver); ///< Register an observer for the given notification type. If the type is nglString::Empty, all the notifications will be sent to the observer.
  void UnregisterObserver(nuiNotificationObserver* pObserver, const nglString& rNotificationName = nglString::Null); ///< Unregister pObserver so that it doesn't receive the given notification. By default it is removed from all notification types (nglString::Null).
  void Clear();

private:
  std::map<nglString, std::set<nuiNotificationObserver*> > mObservers;
  nuiMessageQueue mQueue;
};

#endif
