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
#include "nuiBackgroundPane.h"


#define NOTIF_FILEBROWSE_DONE _T("FILEBROWSE_DONE")


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this), nuiNotificationObserver()
{
  // register the notif. message by the notification manager (MainWindow inherits from nuiMainWindow, which inherits from nuiNotificationManager)
  // the message will be processed by the virtual method OnNotification, overloaded from nuiNotificationObserver class
  RegisterObserver(NOTIF_FILEBROWSE_DONE, this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // box for layout
  nuiHBox* pBox = new nuiHBox(0);
  pBox->SetPosition(nuiCenter);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pBox);
  
  // a label with a background for result output
  nuiBackgroundPane* pPane = new nuiBackgroundPane(eInnerBackground);
  pBox->AddCell(pPane, nuiCenter);
  mpLabel = new nuiLabel();
  mpLabel->SetUserSize(300,12);
  mpLabel->UseEllipsis(true);
  pPane->AddChild(mpLabel);
  
  // browse button to open the file selector
  nuiButton* pBtn = new nuiButton(_T("browse"));
  pBox->AddCell(pBtn);
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnBrowse);
  
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


void MainWindow::OnBrowse(const nuiEvent& rEvent)
{
  nglPath browsedPath             = nglPath(ePathUser);
  nglPath rootPath                = nglPath(_T("/"));
  nglString filter                = _T("*");
  bool showHiddenFiles            = false;
  nglString defaultEditLineValue  = nglString::Null;
  
  // nuiDialogSelectFile is a combo: it inherits from nuiDialog and contains a nuiFileSelector and a nuiEditLine.
  // It handles the user dialogs with the nuiDialog object (ok and cancel button), and with the file selector (double click on a file/folder).
  // It's been developped to easier the integration of a file selector in a application
  //
  // Of course, instead of using the nuiDialogSelectFile object, you are able to implement your own file selector,
  // using the nuiDialog class and nuiFileSelector class. Take a look to nuiDialogSelectFile to see how it's done.
  //
  mpDialog = new nuiDialogSelectFile(this, _T("SELECT A FILE TO LOAD"), browsedPath, rootPath, defaultEditLineValue, filter, showHiddenFiles);
  mEventSink.Connect(mpDialog->FileSelected, &MainWindow::OnDialogDone);    
  
  // we wish this dialog box to be modal
  mpDialog->DoModal();
  
  rEvent.Cancel();
}



void MainWindow::OnDialogDone(const nuiEvent& event)
{
  nuiDialog::DialogResult result = mpDialog->GetResult();
  
  // returning false in a dialog callback means: "close the dialog box".
  // returning true means "don't close the dialog box": it can be usefull if the user choice doesn't match with the application constraints for instance.

  if (result == nuiDialog::eDialogCanceled)
    return;
  
  if (result == nuiDialog::eDialogAccepted)
  {
    // store the user choice
    mBrowsedFile = mpDialog->GetSelectedFile();
    
    // send an asynchrone message to the nuiNotificationManager (the MainWindow itself in that case), to tell the application to go on
    PostNotification(new nuiNotification(NOTIF_FILEBROWSE_DONE));
    
    // note: instead of using an asynchrone message, you can make a synchrone call to another method.
    // But in that case, returning false will close and delete the dialog, which will make your application crash since
    // you are still in the dialog process (OnDialogDone is an event callback from the dialog process).
    //
    // To avoid the crash, you could:
    // - hide the dialog: mpDialog->SetVisible(false)
    // - return true (<=> "don't close and delete the dialog automatically")
    // - delete manually the dialog when your main process exits
    // this solution is a bit ugly, and using an asynchrone message to get out of the dialog process is much better.
  }
}



// virtual from nuiNotificationObserver
void MainWindow::OnNotification(const nuiNotification& rNotif)
{
  const nglString& rName = rNotif.GetName();
  
  // notification receiver
  if (!rName.Compare(NOTIF_FILEBROWSE_DONE))
  {
    // do what you want...
    mpLabel->SetText(mBrowsedFile.GetPathName());
    return;
  }
}

