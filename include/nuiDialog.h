/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiDialog_h__
#define __nuiDialog_h__

//#include "nui.h"
#include "nuiModalContainer.h"

class nuiGrid;
class nuiButton;

class NUI_API nuiDialog : public nuiModalContainer
{
public: 
  enum DialogResult
  {
    eDialogCanceled,
    eDialogAccepted
  };
  enum DialogButtonType ///< Button type which determines button position
  { 
//    eDialogButtonCustom = 0,  // that can't work man... :)
    eDialogButtonOk     = 1,  // bit 1
    eDialogButtonCancel = 2,  // bit 2
    eDialogButtonApply  = 4,   // bit 4
    eDialogButtonCustom = 8
  };

  nuiDialog(nuiContainer* pParent); ///< beware, you must call InitDialog() after that
  virtual ~nuiDialog();
  
  virtual void SetDecoration(const nglString& rName); // inherited from nuiWidget, to give the decoration to the nuiDialog's internal pane
  virtual const nglString& GetDecorationName();
  
  void InitAttributes();
  virtual void    InitDialog(nuiSimpleContainer* pLayoutContainer = NULL, uint32 buttons = 0);
  virtual void    InitDialog(const nglString& rMessage, nuiSimpleContainer* pLayoutContainer = NULL, uint32 buttons = eDialogButtonOk);
  virtual void    InitDialog(nuiLabel* pLabel, nuiSimpleContainer* pLayoutContainer = NULL, uint32 buttons = eDialogButtonOk);

  void SetContents(nuiWidget* pWidget, nuiPosition Position = nuiFill);
  
  nuiSimpleContainer* GetLayoutContainer(); ///< Get the decorative container that future child of the Dialog should be parented to
  void SetLayoutContainer(nuiSimpleContainer* pContainer, bool saveChilds = false); ///< Set the decorative container that future child of the Dialog should be parented to
  
  void SetDefaultButton(DialogButtonType button);
  
  virtual void SetDefaultPos(); ///< set default position (which is center of the TopLevel) to the mpLayoutContainer via SetUserPos
  virtual void SetUserPos(nuiSize X, nuiSize Y);  ///< wrap to mpLayerContainer SetUserPos
  virtual void SetDefaultAnimations();  ///< set default animations
  
  // using these two function below, makes the Dialog creates a nuiGrid* mpGlobalGrid, and the mpButtonsGrid which will be contained in mpGobalGrid cell[0,1]. mpGobalGrid cell[0,0] will be used to layout wether a Label or a user specific widget
  virtual nuiButton* AddButton(const nglString& rButtonLabel, DialogButtonType type = eDialogButtonCustom, bool IsDefaultButton = false); ///< virtual, creates a default button style using given text, set the button by calling AddButton(nuiButton*). override it to change the default button
  void          AddButton(nuiButton* pButtonWidget, DialogButtonType type = eDialogButtonCustom, bool IsDefaultButton = false); ///< add the given nuiButton* pWidgetButton to the mpButtonsGrid according to the DialogButtonType order, create the mpGlobalGrid and mpButtonsGrid if necessary

  nuiButton* GetButton(DialogButtonType type);
  nuiGrid* GetButtonsGrid();
  
  DialogResult GetResult();

  virtual void ExitDialog(const nuiEvent& rEvent);
  
  nuiSimpleEventSource<0> DialogApplied;
  nuiSimpleEventSource<0> DialogDone;
protected:
  void          CreateGrids(); ///< creates grids to display content and buttons
  void          SetDefaultButtons(uint32 buttons); ///< set default buttons by calling AddButton(const nglString&, nuiDialogButtonType) accordind to uint32 buttons representative bits. skips eDialogButtonCustom type
  
  virtual nuiSimpleContainer* CreateDefaultLayout(); ///< creates a the default the decorative Container mpLayoutContainer, override it to change its look
  
  virtual void  OnOk(const nuiEvent& rEvent);
  virtual void  OnCancel(const nuiEvent& rEvent);
  virtual void  OnApply(const nuiEvent& rEvent);
  virtual void  OnAccept(const nuiEvent& rEvent);

	nuiEventSink<nuiDialog> mSink;
  nuiSimpleContainer*   mpLayoutContainer;
  nuiGrid*        mpGlobalGrid;
  nuiGrid*        mpButtonsGrid;
  nuiSimpleContainer* mpPane;

  DialogResult    mResult;
  nuiButton*      mpButtonOk;
  nuiButton*      mpButtonCancel;
  nuiButton*      mpButtonApply;
  nuiButton*      mpDefaultButton;
};

#endif // __nuiDialog_h__

