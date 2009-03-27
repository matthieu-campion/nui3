/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiBackgroundPane.h"



#define NOTIF_PING _T("ping")
#define NOTIF_PONG _T("pong")
#define NOTIF_KILL _T("kill")



//***************************************************************************************************************************
//***************************************************************************************************************************
//
// MessageQueue tutorial
//
// two threads are playing ping-pong, using the nuiMessageQueue(s).
// each thread has a nuiMessageQueue (asynchrone messages for threads communication)
//
// when thread(A) receives "ping", it says "pong" and send "ping"
// thread(A) sends "ping".
// thread(B) receives "ping", says "pong" and sends "ping".
// thread(A) receives "ping", says "pong", and so on...
//
// the main class has also a MessageQueue, in order for the threads to be able to output the messages on window 
// (ngl is not multi-threaded, they can't output the message themselves. They send a message to the main window to do it).
//
// We don't have to declare the MessageQueue for the main class : the MainWindow inherits from nuiMainWindow, which 
// inherits from nuiNotificationManager (which is a notification post office :). 
// It means we can make some GetMainWindow()->PostNotification(...) calls.
//
// All we have to do is to make MainWindow inherist from nuiNotificationObserver, register the observer's expected notifications by the manager,
// and overload the virtual nuiNotificationObserver::OnNotification method to process the notifs.
//
//



/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), nuiNotificationObserver(), mEventSink(this)
{
  mpThread1 = NULL;
  mpThread2 = NULL;  
  
  RegisterWithManager(this, NOTIF_PING);
  RegisterWithManager(this, NOTIF_PONG);
}

MainWindow::~MainWindow()
{
  nuiEvent event;
  OnStop(event);
}



void MainWindow::OnCreation()
{
  // mainbox for layout
  nuiVBox* pMainBox = new nuiVBox(0);
  AddChild(pMainBox);
  
  // a button to start/stop the process
  nuiButton* pBtn = new nuiToggleButton(_T("Start/Stop"));
  pBtn->SetBorder(40,0,0,0);
  mEventSink.Connect(pBtn->ButtonPressed, &MainWindow::OnStart);
  mEventSink.Connect(pBtn->ButtonDePressed, &MainWindow::OnStop);
  pMainBox->AddCell(pBtn, nuiLeft);
  
  
  // horiz. box for the two panes' layout
  nuiHBox* hbox = new nuiHBox(2);
  hbox->SetExpand(nuiExpandShrinkAndGrow);
  pMainBox->AddCell(hbox);
  pMainBox->SetCellExpand(pMainBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  
  //** THREAD1 ********************************************************
  //
  // nuiBackgroundPane -> nuiSCrollView -> nuiLabel
  //
  
  // decorated pane for background
  nuiBackgroundPane* pane = new nuiBackgroundPane();
  pane->SetBorder(40,20, 40, 40);
  hbox->SetCell(0, pane);
  hbox->SetCellExpand(0, nuiExpandShrinkAndGrow);

  // a scrollview in the pane
  mpViews[0] = new nuiScrollView(false/* horiz. bar*/, true/* vert. bar*/);
  mEventSink.Connect(mpViews[0]->OffsetsChanged, &MainWindow::OnScrollbarMoved, (void*)mpViews[0]); // be warn when the scrollbar is moving
  pane->AddChild(mpViews[0]);

  // label for output
  mLabels[0] = _T("THREAD1 : hey I wanna play!\n");
  mpLabels[0] = new nuiLabel(mLabels[0]);
  mpLabels[0]->SetPosition(nuiTop);
  mpViews[0]->AddChild(mpLabels[0]);
  
  
  //** THREAD2 : same thing **********************************************

  pane = new nuiBackgroundPane();
  pane->SetBorder(40,20, 40, 40);
  hbox->SetCell(1, pane);
  hbox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  mpViews[1] = new nuiScrollView(false/* horiz. bar*/, true/* vert. bar*/);
  mEventSink.Connect(mpViews[1]->OffsetsChanged, &MainWindow::OnScrollbarMoved, (void*)mpViews[1]);
  pane->AddChild(mpViews[1]);
  mLabels[1] = _T("THREAD2 : yeah, let's play ping pong!\n");
  mpLabels[1] = new nuiLabel(mLabels[1]);
  mpLabels[1]->SetPosition(nuiTop);
  mpViews[1]->AddChild(mpLabels[1]);
  
}


//
// just a trick to make the two scrollbars move together
//
bool MainWindow::OnScrollbarMoved(const nuiEvent& event)
{
  // get the scrollbar given as the event's user parameter
  nuiScrollView* pView = (nuiScrollView*)event.mpUser;
  
  // guess who is the other scrollbar
  nuiScrollView* pOtherView = (pView == mpViews[0])? mpViews[1] : mpViews[0];
  
  // ask the other one to take the position that the first one is giving
  pOtherView->GetScrollBar(nuiVertical)->GetRange().SetValue(pView->GetScrollBar(nuiVertical)->GetRange().GetValue());
  return true;
}


//
// launch the threads
//
bool MainWindow::OnStart(const nuiEvent& rEvent)
{
  mLabels[0] = _T("THREAD1 : hey I wanna play!\n");
  mLabels[1] = _T("THREAD2 : yeah, let's play ping pong!\n");

  mpThread1 = new MessageQueueThread(0);
  mpThread2 = new MessageQueueThread(1);
  
  mpThread1->SetInterlocutor(mpThread2); // thread1 will send the messages to thread2
  mpThread2->SetInterlocutor(mpThread1); // thread2 will send the messages to thread1
  
  // launch the threads    
  mpThread1->Start();
  mpThread2->Start();  
  
  return true;
}


//
// ask the threads to stop and wait for them to do so
//
bool MainWindow::OnStop(const nuiEvent& rEvent)
{
  if (mpThread1 && mpThread2)
  {
    mpThread1->Post(new nuiNotification (NOTIF_KILL));
    mpThread2->Post(new nuiNotification (NOTIF_KILL));

    mpThread1->Join();
    mpThread2->Join();
    
    delete mpThread1;
    delete mpThread2;  

    mpThread1 = NULL;
    mpThread2 = NULL;
  }
  
  return true;
}



//
// format and print a message to a given output area
//
void MainWindow::AddText(uint32 ID, const nglString rText)
{
  nglTime time;
  nglTimeInfo info;
  time.GetLocalTime(info);
  nglString timeStr;
  timeStr.Format(_T("%d:%d:%d"), info.Hours, info.Minutes, info.Seconds);

  mLabels[ID].Append(_T("\n")).Append(timeStr).Append(_T(" : ")).Append(rText);
  mpLabels[ID]->SetText(mLabels[ID]);
}


//
// virtual OnNotification from nuiNotificationObserver
//
// this allows any object (that inherits from nuiNotificationObserver) to receive asynchrone messages.
// we use it here to let the main thread output the messages:
// we don't want to let the threads do it (crash situation... :)
//
// don't forget to register the notifications this fellow has to catch! (c.f. MainWindow::MainWindow above)
//
void MainWindow::OnNotification(const nuiNotification& rNotification)
{
  const nglString& rName = rNotification.GetName();
  
  uint ID;
  bool res = nuiGetTokenValue<uint>(rNotification.GetToken(), ID);
  NGL_ASSERT(res);

  AddText (ID, rName);  
}




void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}
    
    
    



//***************************************************************************************************************************
//***************************************************************************************************************************
//
// class MessageQueueThread
//
//

MessageQueueThread::MessageQueueThread (uint ID) 
: nglThread ()
{
  mID = ID;
  THREAD1 = (mID == 0);
  mKill = false;
}

MessageQueueThread::~MessageQueueThread () 
{
  
}


void MessageQueueThread::SetInterlocutor(MessageQueueThread* interloc)
{
  mpInterloc = interloc;
}

void MessageQueueThread::Post(nuiNotification* notif)
{
  mQueue.Post(notif);
}


void MessageQueueThread::OnStart()
{
  // first thread starts the talking session
  if (THREAD1)
  {
    // wait 1s before sending the first ping 
    MsSleep(300);
    
    // THREAD1 output "ping"
    nuiNotification* notif = new nuiNotification(NOTIF_PING);
    notif->SetToken(new nuiToken<uint>(mID)); // add a token to the notif: the sender's ID
    GetMainWindow()->PostNotification(notif);
    
    // THREAD1 says "ping" to THREAD2
    notif = new nuiNotification(NOTIF_PING);
    notif->SetToken (new nuiToken<uint>(mID));
    mpInterloc->Post(notif);
  }
  
  
  nuiNotification* message;
  while (!mKill)
  {
    // wait for a message to come
    message = mQueue.Get();
    
    
    // a 'ping' has been received
    if (!message->GetName().Compare(NOTIF_PING))
    {
      MsSleep(300);
      
      // output a 'pong'
      nuiNotification* notif = new nuiNotification(NOTIF_PONG);
      notif->SetToken (new nuiToken<uint>(mID));
      GetMainWindow()->PostNotification(notif);
      
      // wait for 500ms before sending an answer: let the user have the time to read what's going on:)
      MsSleep(500);
      
      // output a 'ping'
      notif = new nuiNotification (NOTIF_PING);
      notif->SetToken (new nuiToken<uint>(mID));
      GetMainWindow()->PostNotification(notif);
      
      // say 'ping' to the other thread
      notif = new nuiNotification (NOTIF_PING);
      notif->SetToken (new nuiToken<uint>(mID));
      mpInterloc->Post(notif);
    }
    
    // a 'kill' message has been reveived
    else if (!message->GetName().Compare(NOTIF_KILL))
    {
      // put a flag for the next cycle
      mKill = true;
    }
    
    
    else 
    {
      // should not happen
      NGL_ASSERT(0);
    }
    
    
    // don't forget to delete the nuiNotification
    if (message) 
      delete message;
    
  }
  
}



