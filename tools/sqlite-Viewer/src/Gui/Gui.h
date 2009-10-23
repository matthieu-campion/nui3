/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class Inspector;
class Navigator;
class Toolbar;

#define NOTIF_TABLE_SELECTED _T("TableSelected")
#define NOTIF_DB_SELECTED _T("DBSelected")

/// the GUI top-level, with a notification receiver
class Gui: public nuiSimpleContainer, public nuiNotificationObserver
{
public:
  
  Gui();
  virtual ~Gui();

protected:
  
  /// toolbar action receiver
  bool OnToolbarActivated(const nuiEvent& rEvent);
  
  /// open-file-dialog action receiver
  bool OnFileSelected(const nuiEvent& rEvent);  
  
  void OpenFileDialog();  
  void OpenDB(const nglPath& rPath);
  
private:
  
  /// what we need to make this instance handle the notification properly
  virtual void ConnectTopLevel();
  virtual void OnNotification(const nuiNotification& rNotif);

  Inspector* mpInspector;
  Navigator* mpNavigator;
  Toolbar* mpToolbar;
  nuiEventSink<Gui> mEventSink;
  
};