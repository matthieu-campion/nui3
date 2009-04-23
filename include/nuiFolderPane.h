/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiFolderPane_h__
#define __nuiFolderPane_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiTitledPane.h"
#include "nuiShapeView.h"
#include "nuiLabelAttribute.h"

/// This widget inherits from nuiTitledPane to create a pane area with a label(or a widget) as a title and with an arrow to open/close the client area
class NUI_API nuiFolderPane : public nuiTitledPane
{
public:
  nuiFolderPane(nuiWidget* pTitleWidget = NULL, bool opened=true);
  nuiFolderPane(const nglString& rText, bool opened=true);
  nuiFolderPane(nuiLabelAttribute* pLabel, bool opened=true);
  void Init (bool opened);
  virtual ~nuiFolderPane();

  bool IsOpened();  
  virtual void Open();
  virtual void Close();
  
  void EnableInteractiveClose(bool set);
	
	nuiSimpleEventSource<nuiWidgetActivated> Opened;   ///< send an event when the folderpane has been opened
	nuiSimpleEventSource<nuiWidgetActivated> Closed; ///< send an event when the folderpane has been closed

  nuiWidget* GetContentsOnClosing();
  void SetContentsOnClosing(nuiWidget* pContentsWidget, nuiPosition position=nuiFill);
  
	virtual void SetTitle(const nglString& rTitle, nuiPosition position=nuiFill);
	virtual void SetTitle(nuiWidget* pTitleWidget, nuiPosition position=nuiFill);
          void SetTitleWithHandle(const nglString& rTitle, nuiPosition position=nuiNoPosition);
          void SetTitleWithHandle(nuiWidget* pTitleWidget, nuiPosition position=nuiNoPosition);
  
protected:

  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  
  bool mIsOpened;
  bool mInteractiveCloseEnabled;
  bool mClicked;
};

#endif // __nuiFolderPane_h__
