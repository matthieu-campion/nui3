/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTable_h__
#define __nuiTable_h__

#include "nuiContainer.h"
#include "nuiXML.h"
#include "nuiMouseEvent.h"

class nuiTable;

typedef class nuiTableRow* nuiTableRowPtr;
typedef std::list<nuiTableRowPtr> nuiTableRowList;

/// This class is not supposed to be used by the user but inderictely by the use of nuiTable. Please be aware that anything in this class may change without notice!
class NUI_API nuiTableRow : public nuiObject
{
public:
  nuiTableRow(nuiTable* pParent);
  nuiTableRow(nuiTable* pParent,const nuiWidgetList& rList);
  virtual ~nuiTableRow();

  virtual bool SetParent(nuiTable* pParent);

  virtual nuiWidgetPtr GetCell(uint col); ///< Return the given Cell or NULL.
  virtual bool SetCell(uint col, nuiWidgetPtr pWidget, bool Delete = true ); ///< Set the given position in the row to the given object .
  virtual bool InsertCell(uint col, nuiWidgetPtr pWidget); ///< Insert a cell at the given position in the row.
  virtual bool RemoveCell(uint col, bool Delete = true); ///< Remove the Cell at the given position in the row.
  virtual bool ClearCell(uint col, bool Delete = true); ///< Clear the given position in the row.
  virtual bool Clear(bool Delete = true); ///< Clear all cell in the row.
  virtual uint GetSize(); ///< Return the number of cells of this row.

  // Tree rows management:
  virtual bool SetParent(nuiTableRowPtr pParent); ///< Change reparent this row to another row. 
  virtual nuiTableRowPtr GetParentRow(); ///< Returns a pointer to this row's parent (or NULL if it has no parent or if it's parent is directly the nuiTable).
  virtual bool AddRow(nuiTableRowPtr pRow); ///< Add a child row to this one.
  virtual bool DelRow(nuiTableRowPtr pRow, bool Delete = true); ///< Remove a child row from this one. 
  virtual bool InsertRow(int index, nuiTableRowPtr pRow); ///< Insert the given row at the given position in this row children. The row will be reparented correctly. 
  virtual int GetRow(nuiTableRowPtr pRow); ///< Returns the child index of the given row pointer.
  virtual nuiTableRowPtr GetRow(nuiSize X, nuiSize Y); ///< Returns the child under the given local coordinates.
  virtual nuiTableRowPtr GetRow(int Row); ///< Returns the row pointer of the given child index.
  virtual uint GetRowCount(); ///< Returns the number of child rows.
  virtual nuiTableRowPtr GetAbove(); ///< Return a pointer to the row above this one (it may be this row's parent or even an unrelated row). 
  virtual nuiTableRowPtr GetBelow(); ///< Return a pointer to the row below this one (it may be this row's parent or even an unrelated row). 
  virtual nuiTableRowPtr GetNextSibling(); ///< Return a pointer to the row in this row's parent's row list.
  virtual nuiTableRowPtr GetFirstVisibleParent(); ///< Return a pointer to the first visible row parent of this row.
  virtual bool IsVisible() const; ///< Return true if the row is on a visible branch (it may be offscreen thought). 


  virtual bool Draw (nuiDrawContext* pContext, uint min, uint max, int SelectedColumn, bool Header = false, nuiTableRowPtr pCursorRow = NULL);
  virtual nuiRect CalcIdealSize(nuiDrawContext* pContext); ///< Call CalcIdealSize on each element of the list & compute the ideal height of the row.
  virtual bool SetRects(const std::list<nuiSize>* pSizes, nuiSize X, nuiSize Y, nuiSize XOffset = 0, nuiSize YOffset = 0); ///< Set the size of the row.
  virtual nuiSize GetHeight(); ///< Return the height of the actual row without taking its children rows into account.
  virtual nuiSize GetTotalHeight(); ///< Return the height of the actual row with taking its children rows into account.
  virtual bool HandleClick(nuiSize X, nuiSize Y); ///< Return true if the nuiTableRow handled the click event (to open/close the child elements for exemple). 

  virtual void SetSelected(bool Flag, bool ChangeChildren = false); ///< Change the selection state of a row
  virtual bool IsSelected();
  virtual void SelectAll(); ///< Select this item and all its children.
  virtual void DeselectAll(); ///< Deselect this item and all its children.

  virtual void Open(bool Set);
  virtual bool IsOpened();
protected:
  nuiSize mHeight, mWidth, mTotalHeight;
  nuiSize mXOffset, mYOffset;
  nuiSize mX, mY;
  nuiTable* mpParent;
  nuiTableRowPtr mpParentRow;
  nuiTableRowList mpRows;
  nuiWidgetList mpCells;
  bool mSelected;
  bool mOpen;
};


/// This class implements an abstract table structure. A table is what is used to contain the elements of a nuiList widget
class NUI_API nuiTable : public nuiSimpleContainer
{
public:
  /** @name Life cycle */
  //@{
  nuiTable();
  virtual bool Load(const nuiXMLNode* pNode);

  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;
  void InitProperties();
  
  virtual ~nuiTable();
  //@}

  virtual bool AddChild(nuiWidgetPtr pChild);


  /** @name Column management */
  //@{
  virtual bool InsertColumn(uint col, nuiWidgetPtr pWidget);  ///< Create a column at the given position and create a header with the widget \param pHeader.
  virtual bool InsertColumn(uint col, const nglString& rHeader); ///< Create a column at the given position and create a header with \param rHeader as the name.
  virtual bool SetColumn(uint col, nuiWidgetPtr pWidget);  ///< Set a column header at the given position and create a header with the widget \param pWidget.
  virtual bool SetColumn(uint col, const nglString& rText);  ///< Set a column header at the given position and create a header with the widget \param rText.
  virtual bool RemoveColumn(uint col, bool Delete = true); ///< Remove a column and destroy all its elements unless \param Delete != true.
  virtual bool ClearColumn(uint col, bool Delete = true); ///< This method removes all the elements of a column without removing the column from the table. If Delete = false no element will be deleted from the heap.
  virtual uint GetColumnCount(); ///< Return the number of column in the table.
  virtual int GetColumn(nuiSize X, nuiSize Y); /// Return the column pointed to by X,Y or -1.
  virtual void ShowColumnHeaders(bool set); ///< Show or hide column headers.
  virtual bool GetShowColumnHeaders(); ///< Return true if the column headers are shown.
  virtual void SetColumnSize(uint Column, nuiSize Size); ///< Change the size in pixels of the given column.
  virtual nuiSize GetColumnSize(uint Column); ///< Return the size in pixels of the given column.
  //@}

  /** @name Row management */
  //@{
  virtual uint GetRowCount(); ///< Return the total number of lines.
  virtual bool RemoveRow(uint row, bool Delete = true);
  virtual nuiTableRowPtr InsertRow(uint row, const nuiWidgetList& rList); ///< Insert a row in the table at the given index, fill it with given widget list and return a pointer to this row.
  virtual nuiTableRowPtr InsertRow(uint row); ///< Insert an empty row in the table at the given index and return a pointer to this row.
  virtual bool ClearRow(uint row, bool Delete = true);
  virtual int  GetRow(nuiSize X, nuiSize Y); /// Return the row pointed to by X,Y or -1.
  virtual nuiTableRowPtr GetRowPtr(nuiSize X, nuiSize Y); /// Return the row pointed to by X,Y or -1.
  virtual nuiTableRowPtr GetRow(uint row); /// Return a pointer to the row's nuiObject for property access.
  virtual bool ShowRow(uint32 row); ///< Make the given row visible in the view.

  virtual uint32 GetRow(nuiTableRowPtr pRow);
  virtual nuiTableRowPtr InsertRow(uint row, nuiTableRowPtr pRow);
  virtual bool SetRow(uint row, nuiTableRowPtr pRow, bool Delete = true);
  //@}

  /** @name Cell management */
  //@{
  virtual nuiWidgetPtr GetCell(uint row, uint col); ///< Return the given Cell. IF .
  virtual bool SetCell(uint row, uint col, nuiWidgetPtr pWidget, bool Delete = true); ///< Set the given position in the table to the given object .
  virtual bool ClearCell(uint row, uint col, bool Delete = true); ///< Clear the given position in the table.
  //@}


  /** @name Selection */
  //@{
  virtual bool GetUnselectable(); ///< Return true if the list items are unselectable by clicking on it.
  virtual bool GetMultiSelectable(); ///< Return true if multiple list items can be selected.
  virtual void SetUnselectable(bool set); ///< If true then the selected list items can be deselected by clicking on it.
  virtual void SetMultiSelectable(bool set); ///< If true then multiple list items can be selected at the same time.
  virtual uint DeselectAll(); ///< Deselect all items.
  virtual uint SelectAll(); ///< Select all items (if the list is multiselectable only).
  virtual uint GetSelected(std::list<uint>& selitems); ///< Populate \param selitems with all the selected row numbers.
  virtual uint GetUnselected(std::list<uint>& unselitems); ///< Populate \param unselitems with all the not selected row numbers.
  virtual uint GetSelected(nuiTableRowList& selitems); ///< Populate \param selitems with all the selected row objects. (1 object per row for property access).
  virtual uint GetUnselected(nuiTableRowList& unselitems); ///< Populate \param unselitems with all the not selected row objects. (1 object per row for property access).
  virtual void ClearTable(bool Delete); ///< Remove all the elements from the table.
  virtual bool Clear(bool Delete = true); ///< Inherited from nuiContainer and redirected to ClearTable(). 
  //@}

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect); ///< This method asks the object to recalculate its layout and to force using the given bounding rectangle. It returns false in case of error. If you override this method, do not forget to call ValidateLayout() once in it. 

  virtual bool Draw(nuiDrawContext* pContext); ///< This method asks the object to draw itself. It returns false in case of error. You must call Validate() once in this method if you decide to override it. You must not draw the widget if it is not visible (check the result of IsVisible() before drawing anything but after having called Validate()).
  virtual nuiWidgetPtr GetChild(nuiSize X, nuiSize Y);

  /** @name Incomming events */
  //@{
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);
  virtual bool KeyDown(const nglKeyEvent& rEvent);
  virtual bool KeyUp(const nglKeyEvent& rEvent);
  virtual void OnSetFocus(nuiWidgetPtr pWidget);
  //@}

  /** @name Outgoing events */
  //@{
  nuiMouseClicked Clicked; ///< This event is called whenever an item is clicked.
  nuiSimpleEventSource<nuiWidgetActivated> Activated; ///< This event is called whenever an item is chosen, that is to say, double clicked. This can only happend in a mono-selection list.
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged; ///< This event is called whenever selection of the items of the table have changed. 
  //@}

protected:
  virtual bool DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool DispatchMouseUnclick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);

  void DrawTableChild(nuiDrawContext* pContext, nuiWidget* pWidget);

  bool mMultiSelectable;
  bool mUnselectable;
  bool mFixedAspectRatio;

  nuiTableRowList mpRows;
  nuiTableRow mHeader;

  nuiEventSink<nuiTable> mEventSink;

  nuiTableRowPtr mpLastItem;
  bool mClicked;

  int32 mSelectedCol; /// The currently selected column
  bool mDisplayHeaders;
  bool mResize;
  int32 mClickedColumn;
  nuiSize mClickX;
  nuiSize mClickY;

//  nuiUInt32 mCursorLine;
  nuiTableRowPtr mpCursorRow;
  bool mSelection;
  bool mKeyboardSelect;
  bool mDisplayCursor;
  uint32 mSelectionStart;
  uint32 mMoveOnly;

  friend class nuiTableRow;
};


#endif // __nuiTable_h__

