/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiMatrixView.h
\brief Matrix with SpinnerLabels Layout container
*/

#pragma once

#include "nuiContainer.h"
#include "nuiGrid.h"
#include "nuiSpinnerLabel.h"
#include "nuiFormater.h"


class nuiMatrixViewFormater : public nuiFormater
{
public:

  nuiMatrixViewFormater();
  ~nuiMatrixViewFormater();

  virtual bool Format (double dValue, nglString& rResult) const;
  virtual double GetCDouble (const nglString& text) const;

  
private:

};



class nuiMatrixViewItemDesc
{
public:
  nuiMatrixViewItemDesc(int32 col, int32 row, const nuiRange& rRange)
  {
    mCol = col;
    mRow = row;
    mRange = rRange;
    mFillColor.Set(0,0,0,0);
    mStrokeColor.Set(0,0,0,0);
    mSelectionColor.Set(1.f,1.f,1.f,1.f);
    mSelectionSize = 2;
  }
  
  int32 GetColumn() const
  {
    return mCol;
  }
  int32 GetRow() const
  {
    return mRow;
  }
  
  double GetValue() const
  {
    return mRange.GetValue();
  }
  
  const nuiRange& GetRange() const
  {
    return mRange;
  }
  
  const nuiColor& GetFillColor() const
  {
    return mFillColor;
  }
  
  const nuiColor& GetStrokeColor() const
  {
    return mStrokeColor;
  }
  
  const nuiColor& GetSelectionColor() const
  {
    return mSelectionColor;
  }
  
  uint32 GetSelectionSize() const
  {
    return mSelectionSize;
  }
  
  void SetValue(double value)
  {
    mRange.SetValue(value);
  }
  
  void SetFillColor(const nuiColor& rColor)
  {
    mFillColor = rColor;
  }
  
  void SetStrokeColor(const nuiColor& rColor)
  {
    mStrokeColor = rColor;
  }
  
  void SetSelectionColor(const nuiColor& rColor)
  {
    mSelectionColor = rColor;
  }
  
  void SetSelectionSize(uint32 size)
  {
    mSelectionSize = size;
  }
  
  
private:
  nuiColor mFillColor;
  nuiColor mStrokeColor;
  nuiColor mSelectionColor;
  uint32 mSelectionSize;
  nuiRange mRange;
  int32 mCol;
  int32 mRow;
};


class nuiMatrixView;


class nuiMatrixViewItem : public nuiFormatedLabel
{
  friend class nuiMatrixView;
  
public:

  double GetValue();
  void SetValue(double value);
  void SetRelativeValue(double delta);

  
  uint GetRow();
  uint GetCol();
  
  nuiRange& GetRange();
  
  const nuiMatrixViewItemDesc& GetDesc() const;
  
  
  virtual bool Draw(nuiDrawContext* pContext);
    
private:

  nuiMatrixViewItem(nuiMatrixView* pParent, uint col, uint row, const nuiRange& rRange, const nuiFormater& rFormater);
  ~nuiMatrixViewItem();
  
  nuiRange mRange;
  nuiMatrixView* mpParent;
  nuiMatrixViewItemDesc mDesc;

};










/// This class is widget built on nuiGrid, containing spinerLabels
class NUI_API nuiMatrixView : public nuiGrid, public nglTimer  // timer to handle the wheel up/down action

{
public:
  nuiMatrixView (uint nbcolumns = 3, uint nbrows = 3, bool EnableColumnHeader=false, bool EnableRowHeader=false,
                const nuiFormater& rFormater = nuiMatrixView::mDefaultFormater, const nuiRange& rRange = nuiMatrixView::mDefaultRange,
                nuiSize defaultHSpacing=0.0f, nuiSize defaultVSpacing=0.0f);
  virtual ~nuiMatrixView();
  
  void Init(); // DON'T FORGET TO INIT THE MATRIX, MOST OF THE TIME AFTER HAVING CONNECTED THE SIGNALS AND EVENT
  

  // static
  static nglKeyCode GetGrabMouseKey();
  static nglKeyCode GetRelativeKey();
  static nglKeyCode GetFineSensitivityKey();

  //
  // setters/getters
  //
  bool IsColumnHeaderEnabled();
  bool IsRowHeaderEnabled();  
  void EnableSelectAll(bool canSelectAll);
  bool IsSelectAllEnabled();

  bool SetColumnHeader (uint col, nuiWidget* pWidget, nuiPosition position = nuiFill, bool ReplaceExisting = true);
  bool SetRowHeader   (uint col, nuiWidget* pWidget, nuiPosition position = nuiFill, bool ReplaceExisting = true);
  
  bool GetCellValue(uint col, uint row, double& value);
  bool SetCellValue (uint col, uint row, double value);  ///< change the value of a cell's spinnerLabel
  
  void SetCellEnabled(uint col, uint row, bool set);
  bool IsCellEnabled(uint col, uint row);
  
  const std::map<nuiMatrixViewItem*, bool>& GetSelectedItems();



  //
  // class attributes
  //
  static nuiMatrixViewFormater mDefaultFormater;
  static nuiRange mDefaultRange;


  //
  // events
  //
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged; ///< This event is called whenever selection of the items of the list have changed. 
  nuiSimpleEventSource<nuiWidgetSelected> ValueChanged; ///< This event is called whenever one or more items values have changed. 
  nuiSimpleEventSource<nuiWidgetSelected> InteractiveValueChanged; ///< This event is called whenever one or more items values are currently changing
  
  //
  // signal
  //
  nuiSignal1<nuiMatrixViewItemDesc&> GetItemDesc;
  
  //
  // tools
  //
  void AddRows(uint pos, uint rows);            // not allowed
  void AddColumns(uint pos, uint columns);      // not allowed
  void RemoveRows(uint pos, uint rows);         // not allowed
  void RemoveColumns(uint pos, uint columns);   // not allowed


  //
  // selection
  //
  bool ForceSelectColumn(uint col, bool set);
  bool ForceSelectRow(uint row, bool set);
  bool SelectAll();
  bool DeselectAll();




private:

  virtual bool Draw(nuiDrawContext* pContext);



  //
  // events
  //
  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
          bool Selected(uint col, uint row);
          bool MouseDoubleClicked(uint col, uint row);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button); 
  virtual bool MouseMoved(nuiSize X, nuiSize Y);
          void RelativeSpinCells(nuiSize delta);
          void SpinCells(nuiSize delta);
          void Renamed(const nuiEvent& rEvent);
          
  virtual void OnTick(nglTime Lap); // timer event


  
  //
  // selection
  //
  bool SelectColumn(uint col);
  bool SelectRow(uint row);
  bool IsSelected(uint col, uint row);
  bool SelectCell(uint col, uint row, bool select, bool checkSelection=true);
  bool IsAllColumnCellsSelected(uint col, bool select);
  bool IsAllRowCellsSelected(uint row, bool select);


  //
  // tools
  //
  bool GetCellCoord(int mouseX, int mouseY, uint& col, uint& row);



  //
  // class attributes
  //
  static nuiSize mDefaultSensitivity;
  static nglKeyCode mGrabMouseKey;
  static nglKeyCode mRelativeKey;
  static nglKeyCode mFineSensitivityKey;

  
  // 
  // instance attributes
  //
  nuiFormater mFormater;
  nuiRange mRange;
  
  // 
  // running attributes
  //
  bool mInited;
  bool mColumnHeader;
  bool mRowHeader;
  bool mClicked;
  bool mCanChange;
  bool mChanged;
  nuiPoint mClickPos;
  nuiMatrixViewItem* mClickedItem;
  nuiEventSink<nuiMatrixView>  mEventSink;

  
  //
  // selection attributes
  //
  bool mCanSelectAll;
  bool mAllSelected;
  std::vector<bool> mRowSelected;
  std::vector<bool> mColumnSelected;
  std::map<nuiMatrixViewItem*, bool> mSelectedItems;

};

