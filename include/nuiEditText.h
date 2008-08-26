/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#ifndef __nuiEditText_h__
#define __nuiEditText_h__

#include "nuiComposite.h"
#include "nuiTheme.h"

#include "nglDragAndDropObjects.h"
#include "nglDataObjects.h"
#include "nuiFontBase.h"

class nuiFont;
class nuiXMLNode;

class NUI_API nuiEditText : public nuiComposite
{
protected:
  class NUI_API TextBlock;

public:
  nuiEditText(const nglString& rText = nglString::Empty);
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiEditText();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;
  void InitProperties();

  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  virtual bool TextInput(const nglString& rUnicodeString);
  virtual bool KeyDown  (const nglKeyEvent& rEvent);
  virtual bool KeyUp    (const nglKeyEvent& rEvent);

  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved    (nuiSize X, nuiSize Y);

  virtual bool OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y);
  virtual void OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual void OnDragged(nglDragAndDrop* pDragObject);
  virtual void OnStopDragging();

  enum CommandId
  {
    eGoDocBegin = 0,
    eGoDocEnd,
    eGoLineBegin,
    eGoLineEnd,
    eGoParagraphBegin,
    eGoParagraphEnd,
    eGoUp,
    eGoDown,
    eGoLeft,
    eGoRight,
    eGoPageUp,
    eGoPageDown,
    eGoNextWord,
    eGoPreviousWord,
    eStartSelection,
    eStopSelection,
    eStartCommand,
    eStopCommand,
    eStartShift,
    eStopShift,
    eStartAlt,
    eStopAlt,
    eDisableSelection,
    eSelectAll,
    eDeleteSelection,
    eSelectParagraph,
    eSelectLine,
    eSelectWord,

    eCopy,
    eCut,
    ePaste,

    eDeleteForward,
    eDeleteBackward,

    eUndo,
    eRedo,

    eShowCursor,

    eInsertText,

    eLastCommand
  };

  bool Do(CommandId command, nuiObject* pParams);
  bool Undo();
  bool Redo();
  bool CanUndo() const;
  bool CanRedo() const;

  nglString GetSelection() const;
  const nglString& GetText() const;
  void SetText(const nglString& rText);

  void MoveDropCursorTo(nuiSize X, nuiSize Y);

  void MoveCursorTo(nuiSize X, nuiSize Y);
  void SetCursorPos(uint Pos);
  uint GetCursorPos() const;
  void SetAnchorPos(uint Pos);
  uint GetAnchorPos() const;
  void MoveCursorTo(uint Pos); ///< Move the cursor to a position. Move the anchor too if we are not currently in selection mode.

  TextBlock* GetBlock(uint Pos) const;
  uint GetPosFromCoords(uint x, uint y, bool IgnoreWidth) const;
  uint GetPosFromCoords(nuiSize x, nuiSize y, bool IgnoreWidth) const;
  bool GetCoordsFromPos(uint Pos, uint& x, uint& y) const;
  bool GetCoordsFromPos(uint Pos, nuiSize& x, nuiSize& y) const;
  void GetCursorPos(uint& rX, uint& rY);
  void GetCursorPos(nuiSize& rX, nuiSize& rY);

  bool SetFont(nuiTheme::FontStyle FontStyle = nuiTheme::Fixed);
  bool SetFont(nuiFont* pFont, bool AlreadyAcquired = false);
  nuiFont* GetFont();

  void SetEditable(bool Set);
  bool IsEditable();

  void AddText(const nglString& rText); ///< Adds the given string to the currently held text
  void Print(const nglChar* pFormat, ...); ///< Adds the given string to the currently held text
  void SetFollowModifications(bool Set);


  // events
  nuiSimpleEventSource<0> TextChanged;

protected:
  // Commands:
  bool GoDocBegin(nuiObject* pParams);
  bool GoDocEnd(nuiObject* pParams);
  bool GoLineBegin(nuiObject* pParams);
  bool GoLineEnd(nuiObject* pParams);
  bool GoParagraphBegin(nuiObject* pParams);
  bool GoParagraphEnd(nuiObject* pParams);
  bool GoUp(nuiObject* pParams);
  bool GoDown(nuiObject* pParams);
  bool GoLeft(nuiObject* pParams);
  bool GoRight(nuiObject* pParams);
  bool GoPageUp(nuiObject* pParams);
  bool GoPageDown(nuiObject* pParams);
  bool GoNextWord(nuiObject* pParams);
  bool GoPreviousWord(nuiObject* pParams);
  bool StartSelection(nuiObject* pParams);
  bool StopSelection(nuiObject* pParams);
  bool StartCommand(nuiObject* pParams);
  bool StopCommand(nuiObject* pParams);
  bool StartShift(nuiObject* pParams);
  bool StopShift(nuiObject* pParams);
  bool StartAlt(nuiObject* pParams);
  bool StopAlt(nuiObject* pParams);
  bool DisableSelection(nuiObject* pParams);
  bool SelectAll(nuiObject* pParams);
  bool DeleteSelection(nuiObject* pParams);
  bool SelectParagraph(nuiObject* pParams);
  bool SelectLine(nuiObject* pParams);
  bool SelectWord(nuiObject* pParams);

  bool Copy(nuiObject* pParams);
  bool Cut(nuiObject* pParams);
  virtual bool Paste(nuiObject* pParams);

  bool DeleteForward(nuiObject* pParams);
  bool DeleteBackward(nuiObject* pParams);

  bool Undo(nuiObject* pParams);
  bool Redo(nuiObject* pParams);

  bool ShowCursor(nuiObject* pParams);

  bool InsertText(nuiObject* pParams);

  class NUI_API FontLayout : public nuiFontLayout
  {
  public:
    FontLayout(nglFontBase& rFont, float PenX = 0.0f, float PenY = 0.0f);
    virtual ~FontLayout();

    virtual void OnGlyph(nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph);

    const std::vector<uint>& GetLines()
    {
      return mLineIndices;
    }
  protected:
    std::vector<uint> mLineIndices;
  };

  class NUI_API TextBlock
  {
  public:
    TextBlock(nuiFont* pFont, const nglString& rString, uint begin, uint end);
    virtual ~TextBlock();

    void Draw(nuiDrawContext* pContext, nuiSize X, nuiSize Y, uint SelectionBegin, uint SelectionEnd, nuiSize WidgetWidth);
    const nuiRect& GetIdealSize();

    void SetRect(const nuiRect& rRect);
    const nuiRect& GetRect() const;

    uint GetPos() const;
    uint GetLength() const;
    uint GetEnd() const;
    void SetPos(uint Pos);
    void SetLength(uint Length);
    void SetEnd(uint End);

    uint GetLineHeight();

    nuiSize GetHeight();

    uint GetLineBeginFromPos(uint Pos);
    uint GetLineEndFromPos(uint Pos);
    bool GetCoordsFromPos(uint Pos, uint& rX, uint& rY);
    bool GetCoordsFromPos(uint Pos, nuiSize& rX, nuiSize& rY);
    uint GetPosFromCoords(uint X, uint Y);
    uint GetPosFromCoords(nuiSize X, nuiSize Y);

    bool ContainsPos(uint Pos);

    void Layout();
    void InvalidateLayout();

  protected:
    uint mBegin;
    uint mEnd;
    FontLayout* mpLayout;
    nuiRect mRect;
    nuiRect mIdealRect;
    const nglString& mrString;
    nuiFont* mpFont;

    bool mLayoutOK;
  };

  std::vector<TextBlock*> mpBlocks;
  nglString mText;
  uint mCursorPos; // Position in the text string
  uint mAnchorPos; // Position in the text string
  int32 mDropCursorPos; // Position in the text string, -1 is disabled

  std::vector<std::pair<CommandId, nuiObject*> > mCommandStack;
  uint mCommandStackPos;
  void CropCommandStack();
  void ClearCommandStack();
  void PushCommand(CommandId, nuiObject*);

  void SaveCursorPos(nuiObject* pParams) const;
  void LoadCursorPos(nuiObject* pParams);

  void SaveAnchorPos(nuiObject* pParams) const;
  void LoadAnchorPos(nuiObject* pParams);

  void SavePos(nuiObject* pParams) const;
  void LoadPos(nuiObject* pParams);

  typedef bool (nuiEditText::*CommandFunction)(nuiObject* pParams);
  std::vector<CommandFunction> mCommands;

  nuiFont* mpFont;
  nuiSize mTextHeight;
  nuiSize mSelectGap;

  void ClearBlocks();
  void CreateBlocks(const nglString& rText, std::vector<TextBlock*>& rpBlocks);

  std::map<nglKeyCode, CommandId> mKeyBindings;
  std::map<nglKeyCode, CommandId> mCommandKeyBindings;
  std::map<nglKeyCode, CommandId> mAltKeyBindings;
  void InitKeyBindings();
  void InitCommands();

  bool mCommandDown;
  bool mShiftDown;
  bool mAltDown;
  bool mSelecting; 
  bool mSelectionActive; 
  bool mIsEditable;
  bool mFollowModifications;
  bool mStartDragging;
  bool mDragging;
};

#endif // __nuiEditText_h__
