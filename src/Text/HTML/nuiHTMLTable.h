/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLItem.h"

#pragma once

class nuiHTMLTable : public nuiHTMLItem
{
public:
  nuiHTMLTable(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, bool Inline);
  virtual ~nuiHTMLTable();
  
  virtual void Draw(nuiDrawContext* pContext);
  virtual void Layout(nuiHTMLContext& rContext);
  
  void AddRow();
  void AddItemEnd(nuiHTMLItem* pItem);
  
  void PushContext(const nuiHTMLContext& rContext);
  void PopContext(nuiHTMLContext& rContext);
  
  void GetItemsAt(std::vector<nuiHTMLItem*>& rHitItems, float X, float Y) const;
  
  enum Frame
  {
    eVoid, //: No sides. This is the default value.
    eAbove, //: The top side only.
    eBelow, //: The bottom side only.
    eHSides, //: The top and bottom sides only.
    eVSides, //: The right and left sides only.
    eLHS, //: The left-hand side only.
    eRHS, //: The right-hand side only.
    eBox, //: All four sides.
    eBorder //: All four sides.
  };
  
  enum Rules
  {
    eNone,
    eGroups,
    eRows,
    eCols,
    eAll
  };
  
protected:
  Frame mFrame;
  float mBorder;
  
  class Cell
  {
  public:
    Cell();
    
    Cell* mpMasterCell;
    uint32 mColSpan;
    uint32 mRowSpan;
    nuiHTMLItem* mpItem;
    float mIdealWidth;
    float mIdealHeight;
    float mRequestedWidth;
    float mRequestedHeight;
    float mWidth;
    float mHeight;
    nuiHTMLItem::Unit mWidthUnit;
    nuiHTMLItem::Unit mHeightUnit;
    nglString mAbbr;
    bool mHeader;
    Frame mFrame;
    float mBorder;
    float mSpacing;
    float mPadding;
  };
  
  std::vector<std::vector<Cell> > mCells;
};

