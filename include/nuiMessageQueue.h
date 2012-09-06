/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiMessageQueue_h__
#define __nuiMessageQueue_h__

#include "nui.h"
#include "nglCriticalSection.h"
#include "nglSyncEvent.h"

class nuiNotification;

/// implements a message queue for multi-threaded communication
/// see nuiTest, MessageQueueWindow, for an application example
class NUI_API nuiMessageQueue
{
public : 

  nuiMessageQueue();
  ~nuiMessageQueue();
  
  bool Post(nuiNotification* notif); ///< add a message to the message queue. !! DON'T USE THE SAME nuiNotification TWICE : IT'S SUPPOSED TO BE DELETED WHEN IT'S RECEIVED.
  nuiNotification* Get(uint32 time = std::numeric_limits<uint32>::max()); ///< extract the first message of the queue. Will block 'til a message is posted or 'til the timeout is reached. After timeout, return NULL if there is no message.
                                                /// !!! YOU ARE RESPONSIBLE FOR DELETING THE nuiNotification OBJECT. !!!
  
private : 

  nglCriticalSection mQueueCS;
  std::queue<nuiNotification*> mQueue;
  nglSyncEvent mSyncEvent;
};



#endif
