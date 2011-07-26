/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiList_h__
#define __nuiList_h__

//#include "nui.h"
#include "nglPath.h"
#include "nuiApplication.h"
#include "nuiEvent.h"
#include "nuiMouseEvent.h"
#include "nuiContainer.h"

/// This class implements a simple list of elements. It has no multicolumn support, see nuiTable for multicolumn lists.
class NUI_API nuiList : public nuiSimpleContainer
{
public:
  nuiList(nuiOrientation orientation = nuiVertical);
  virtual ~nuiList();

  void InitProperties();

  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  void SetOrientation(nuiOrientation orientation); /// Set the widget orientation.
  nuiOrientation GetOrientation(); ///< Get the widget orientation.
  nuiWidgetPtr GetItem(nuiSize X,nuiSize Y); ///< Return a pointer to the child pointed by X,Y or NULL.
  nuiWidgetPtr GetIdealItem(nuiSize X,nuiSize Y); ///< Return the item pointed to by the given coordinates without taking any ongoing animation into account.
  nuiWidgetPtr GetIdealNextItem(nuiSize X,nuiSize Y); ///<  Return the item where we should insert if the insertion point if (X, Y).
  int32 GetItemNumber(nuiWidgetPtr pWidget); ///< Return the index of the child pointed by pWidget.
  bool GetUnselectable(); ///< Return true if the list items are unselectable by clicking on it.
  bool GetMultiSelectable(); ///< Return true if multiple list items can be selected.
  void SetUnselectable(bool set); ///< If true then the selected list items can be deselected by clicking on it.
  void SetMultiSelectable(bool set); ///< If true then multiple list items can be selected at the same time.
  uint DeselectAll(); ///< Deselect all items.
  uint SelectAll(); ///< Select all items (if the list is multiselectable only).
  void SelectItem(uint ItemNumber);
  void SelectItemSilent(uint ItemNumber);
  void SelectItem(nuiWidgetPtr pItem);
  void SelectItemSilent(nuiWidgetPtr pItem);
  uint GetSelected(nuiWidgetList& selitems); ///< Populate \param selitems with all the selected items.
  nuiWidget* GetSelected (); ///< return the first selected item.
  nuiTokenBase* GetSelectedToken ();///< return the nuiTokenBase of the first selected item. (it's to be casted into the expected nuiToken<T>).
  uint GetUnselected(nuiWidgetList& unselitems); ///< Populate \param unselitems with all the not selected items.
  bool ShowRow(int32 number); ///< Make the given row visible by setting the hot rect.  
  
  // Helper function to make lists of files:
  bool PopulateFiles(const nglPath& rPath); ///< Fill the list box with the list of the files contained in the given path.
  bool PopulateDirs(const nglPath& rPath);  ///< Fill the list box with the list of the directories contained in the given path.
  bool Populate(const nglPath& rPath, bool Files = true, bool Dirs = true); ///< Fill the list box with the list of the files and/or the directories contained in the given path.

  // Other atributes:
  nuiSize GetBorderSize(); ///< Retreive the current border around the area of the enclosed widget.
  void SetBorderSize(nuiSize BorderSize); ///< Set the current border around the area of the enclosed widget.
  void SetFixedAspectRatio(bool flag);
  bool GetFixedAspectRatio();

  // Class attributes:
  static nuiSize GetDefaultBorderSize(); ///< Retreive the default border around the area of the enclosed widget of every button that will be created.
  static void SetDefaultBorderSize(nuiSize BorderSize); ///< Set the default border around the area of the enclosed widget of every button that will be created.

  bool CanMoveItems();
  void SetCanMoveItems(bool set);
  
  // Outgoing event:
  nuiMouseClicked Clicked; ///< This event is called whenever an item is clicked.
  nuiMouseClicked DoubleClicked; ///< This event is called whenever an item is chosen, that is to say, double clicked. This can only happend in a mono-selection list.
  nuiSimpleEventSource<nuiWidgetActivated> Activated; ///< This event is called whenever an item is chosen, that is to say, double clicked. This can only happend in a mono-selection list.
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged; ///< This event is called whenever selection of the items of the list have changed. 
  nuiSignal2<nuiWidget*,nuiWidget*> ItemMoved;

  // Incomming events:
  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
  bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved(nuiSize X, nuiSize Y);
  
  // temp.
  void MoveChild(nuiWidget* pSelectedChild, nuiWidget* pDestinationChild);

  void SetMoveAnimationDuration(float duration);
  void SetMoveAnimationEasing(const nuiEasingMethod& rMethod);

  void Sort(const nuiFastDelegate2<nuiWidget*, nuiWidget*, bool>& rSortDelegate);
  
protected:
  nuiOrientation mOrientation; /// mHorizontal is true if the containers' layout if horizontal, false if vertical.
  float           mBorderSize;

  // Keyboard handling variables:
  bool mMoveOnly;
  bool mKeyboardSelect;
  int32 mCursorLine;
  bool mDisplayCursor;
  int32 mSelectionStart;
  bool mSelection;
  
  bool mClicked;
  bool mCanMoveItems;
  bool mMoved;

  static nuiSize    mDefaultBorderSize;
  bool           mMultiSelectable;
  bool           mUnselectable;
  bool           mFixedAspectRatio;
  nuiWidget* mpGrabedItem;
  nuiWidgetPtr     mpLastItem;
  nuiWidgetPtr     mpLastDestinationItem;
  nuiEventSink<nuiList> mEventSink;
  
  void OnChildAdded(const nuiEvent& rEvent);
  void OnChildDeleted(const nuiEvent& rEvent);
  
  float mMoveAnimDuration;
  nuiEasingMethod mMoveAnimEasing;
};

#endif // __nuiList_h__

