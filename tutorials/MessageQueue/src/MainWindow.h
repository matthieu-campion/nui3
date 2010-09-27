/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiScrollView.h"

class MessageQueueThread;

class MainWindow : public nuiMainWindow, public nuiNotificationObserver
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();
  
protected:
  
  void OnStart(const nuiEvent& rEvent); 
  void OnStop(const nuiEvent& rEvent); 

  void OnScrollbarMoved(const nuiEvent& event);
  void AddText(uint32 ID, const nglString rText);

  virtual void OnNotification(const nuiNotification& rNotification);
  
private:

  nuiLabel* mpLabels[2];
  nglString mLabels[2];
  nuiScrollView* mpViews[2];
  MessageQueueThread* mpThread1;
  MessageQueueThread* mpThread2;
  nuiEventSink<MainWindow> mEventSink;
};



class MessageQueueThread : public nglThread
{
  public : 
  
  MessageQueueThread(uint ID);
  ~MessageQueueThread();
  
  void SetInterlocutor(MessageQueueThread* interloc);
  void Post(nuiNotification* notif);
  
  virtual void OnStart();
  
  private : 
  
  uint mID;
  bool THREAD1;
  MessageQueueThread* mpInterloc;
  nuiMessageQueue mQueue;
  bool mKill;
  
  
};

