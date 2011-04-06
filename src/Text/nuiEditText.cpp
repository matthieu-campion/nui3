// nuiEditText.cpp
// 

#include "nui.h"
#include "nuiEditText.h"
#include "nuiFont.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"

#include "nglDataObjects.h"

//class nuiEditTest2 : nuiSimpleContainer
nuiEditText::nuiEditText(const nglString& rText)
: nuiComposite(),
  mCursorPos(0),
  mAnchorPos(0),
  mCompositionPos(-1),
  mCompositionLength(0),

  mDropCursorPos(-1),
  mCommandStackPos(0),
  mpFont(NULL),
  mTextHeight(0),
  mSelectGap(0),
  mCommandDown(false),
  mShiftDown(false),
  mAltDown(false),
  mSelecting(false),
  mSelectionActive(false),
  mIsEditable(true),
  mFollowModifications(true),
  mStartDragging(false),
  mTextColorSet(false)
{
  if (SetObjectClass(_T("nuiEditText")))
    InitAttributes();
  
  SetFont(nuiTheme::Default);
  SetText(rText);
  InitCommands();
  InitKeyBindings();
}

bool nuiEditText::Load(const nuiXMLNode* pNode)
{
  mCursorPos = 0;
  mAnchorPos = 0;
  mCompositionPos = -1;
  mCompositionLength = 0;
  mDropCursorPos = -1;
  mCommandStackPos = 0;
  mpFont = NULL;
  mTextHeight = 0;
  mSelectGap = 0;
  mCommandDown = false;
  mShiftDown = false;
  mAltDown = false;
  mSelecting = false;
  mSelectionActive = false;
  mIsEditable = true;
  mFollowModifications = true;
  mStartDragging = false;
  
  SetFont(nuiTheme::Default);
  SetText(pNode->GetValue());
  InitCommands();
  InitKeyBindings();
  
  return true;
}

nuiEditText::~nuiEditText()
{
  ClearBlocks();
  ClearCommandStack();
  if (mpFont)
    mpFont->Release();
}


void nuiEditText::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("TextColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiEditText::GetTextColor), 
                nuiMakeDelegate(this, &nuiEditText::SetTextColor)));
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Font")), nuiUnitName,
                nuiMakeDelegate(this, &nuiEditText::_GetFont), 
                nuiMakeDelegate(this, &nuiEditText::_SetFont)));
}


nuiXMLNode* nuiEditText::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  return nuiSimpleContainer::Serialize(pParentNode, Recursive);
}

void nuiEditText::InitProperties()
{

}

bool nuiEditText::Draw(nuiDrawContext* pContext)
{
  uint SelectionBegin, SelectionEnd;
  SelectionBegin = MIN(mCursorPos, mAnchorPos);
  SelectionEnd = MAX(mCursorPos, mAnchorPos);

  nuiSize width = GetRect().GetWidth();

  nuiRect cliprect(GetVisibleRect());

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);

  nuiColor c(GetColor(eNormalTextBg));
  c.Multiply(GetMixedAlpha());
  pContext->SetFillColor(c);
  pContext->DrawRect(GetRect().Size(), eFillShape);

  nuiColor textColor;
  if (mTextColorSet)
    textColor = mTextColor;
  else
    textColor = GetColor(eNormalTextFg);
    
  pContext->SetTextColor(textColor);
  pContext->SetFillColor(GetColor(eSelectionMarkee));
  uint count = (uint32)mpBlocks.size();
  nuiSize PosX = 0, PosY = 0;

  nuiRect inter;
  for (uint i = 0; i < count; i++)
  {
    TextBlock* pBlock = mpBlocks[i];

    if (inter.Intersect(cliprect, pBlock->GetRect()))
    {
      pBlock->Draw(pContext, PosX, PosY, SelectionBegin, SelectionEnd, mCompositionPos, mCompositionPos + mCompositionLength, width);
    }
    PosY += pBlock->GetHeight();
  }

  nglFontInfo fontinfo;
  mpFont->GetInfo(fontinfo);

  if (HasFocus())
  {
    // Draw the cursor:
    nuiSize CursorX, CursorY;
    GetCursorPos(CursorX, CursorY);

    nuiRect cursor(CursorX, CursorY - fontinfo.Ascender, 2.0f, mpFont->GetHeight());
    cursor.RoundToBelow();
    pContext->SetFillColor(GetColor(eNormalTextFg));
    pContext->DrawRect(cursor, eFillShape);

    if (mStartDragging)
    {
      nglFontInfo info;
      mpFont->GetInfo(info);
      CursorX = 0;
      CursorY = info.Ascender;
      GetCoordsFromPos(mDropCursorPos, CursorX, CursorY);

      nuiRect dropCursor(CursorX, CursorY - fontinfo.Ascender, 2.0f, mpFont->GetHeight());
      dropCursor.RoundToBelow();
      pContext->SetFillColor(GetColor(eNormalTextFg));
      pContext->DrawRect(dropCursor, eFillShape);
    }
  }

  return nuiSimpleContainer::DrawChildren(pContext);
}

nuiRect nuiEditText::CalcIdealSize()
{
  nuiRect global;
  uint count = (uint)mpBlocks.size();
  for (uint i = 0; i < count; i++)
  {
    nuiRect rect(mpBlocks[i]->GetIdealSize());
    rect.MoveTo(0, global.Bottom());
    global.Union(global, rect);
  }
  
  global.Right() += 1.f; /// for Cursor size

  return global;
}

bool nuiEditText::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  nuiRect global;
  uint count = (uint)mpBlocks.size();
  nuiSize y = 0;
  for (uint i = 0; i < count; i++)
  {
    nuiRect rect(mpBlocks[i]->GetIdealSize());
    rect.Move(0, y);
    mpBlocks[i]->SetRect(rect);
    y += rect.GetHeight();
  }
  return true;
}

void nuiEditText::SetFont(const nglString& rFontSymbol)
{
  nuiFont* pFont = nuiFont::GetFont(rFontSymbol);
  if (pFont)
    SetFont(pFont);
}

void nuiEditText::_SetFont(const nglString& rFontSymbol)
{
  SetFont(rFontSymbol);
}

const nglString& nuiEditText::_GetFont() const
{
  if (mpFont)
    return mpFont->GetObjectName();
  return nglString::Null;
}


void nuiEditText::SetTextColor(const nuiColor& Color)
{
  mTextColorSet = true;
  mTextColor = Color;
  Invalidate();
}

const nuiColor& nuiEditText::GetTextColor() const
{
  return mTextColor;
}


void nuiEditText::InitKeyBindings()
{
  SetHotKeyMask(nuiCommandKey | nuiWinKey);
  
  mCommandKeyBindings[NK_HOME] = eGoDocBegin;
  mCommandKeyBindings[NK_END] = eGoDocEnd;
  mKeyBindings[NK_ENTER] = eNewLine;
  mKeyBindings[NK_PAD_ENTER] = eNewLine;
  mKeyBindings[NK_HOME] = eGoLineBegin;
  mKeyBindings[NK_END] = eGoLineEnd;
  mAltKeyBindings[NK_HOME] = eGoParagraphBegin;
  mAltKeyBindings[NK_END] = eGoParagraphEnd;
  mKeyBindings[NK_UP] = eGoUp;
  mKeyBindings[NK_DOWN] = eGoDown;
  mKeyBindings[NK_LEFT] = eGoLeft;
  mKeyBindings[NK_RIGHT] = eGoRight;
  mKeyBindings[NK_PAGEUP] = eGoPageUp;
  mKeyBindings[NK_PAGEDOWN] = eGoPageDown;
  mCommandKeyBindings[NK_RIGHT] = eGoNextWord;
  mCommandKeyBindings[NK_LEFT] = eGoPreviousWord;
  mKeyBindings[NK_ESC] = eDisableSelection;
  mCommandKeyBindings[NK_A] = eSelectAll;
  //  mKeyBindings[] = eDeleteSelection;
  //  mKeyBindings[] = eSelectParagraph;
  //  mKeyBindings[] = eSelectLine;
  //  mKeyBindings[] = eSelectWord;

  mCommandKeyBindings[NK_C] = eCopy;
  mCommandKeyBindings[NK_X] = eCut;
  mCommandKeyBindings[NK_V] = ePaste;

  mKeyBindings[NK_DELETE] = eDeleteForward;
  mKeyBindings[NK_BACKSPACE] = eDeleteBackward;
  mCommandKeyBindings[NK_Z] = eUndo;
  //  mCommandKeyBindings[NK_Z] = eRedo;
  //  mKeyBindings[] = eShowCursor;
  
  SetWantKeyboardFocus(true);
}

bool nuiEditText::TextInput(const nglString& rUnicodeString)
{
  nglString str(rUnicodeString);
  str.Replace(nglChar(13), '\n');
  nuiObject* pObject = new nuiObject();
  pObject->SetProperty(_T("Text"), str);
  Do(eInsertText, pObject);
  
  return true;
}

void nuiEditText::TextCompositionStarted()
{
  // Nothing to do (?!)
}

void nuiEditText::TextCompositionConfirmed()
{
  SetAnchorPos(mCompositionPos + mCompositionLength);
  SetCursorPos(mCompositionPos + mCompositionLength);

  mSelectionActive = false;

  mCompositionPos = -1;
  mCompositionLength = 0;
  Invalidate();
}

void nuiEditText::TextCompositionCanceled()
{
  if (mCompositionPos < 0)
  {
    // Start composition
    mCompositionPos = mCursorPos;
    mCompositionLength = 0;
  }
  else
  {
    // Composition already active: use the selection to replace the existing composition string
    mAnchorPos = mCompositionPos;
    mCursorPos = mCompositionPos + mCompositionLength;
  }
  
  mSelectionActive = true;
  Do(eDeleteSelection, NULL);
  mSelectionActive = false;
  
  mCompositionPos = -1;
  mCompositionLength = 0;
  
  Invalidate();
}

void nuiEditText::TextCompositionUpdated(const nglString& rString, int32 CursorPosition)
{
  printf("TextCompositionUpdated: '%s' %d\n", rString.GetChars(), CursorPosition);
  if (mCompositionPos < 0)
  {
    // Start composition
    mCompositionPos = mCursorPos;
    mCompositionLength = 0;
  }
  else
  {
    // Composition already active: use the selection to replace the existing composition string
    mAnchorPos = mCompositionPos;
    mCursorPos = mCompositionPos + mCompositionLength;
  }

  mSelectionActive = true;
  Do(eDeleteSelection, NULL);
  mSelectionActive = false;
  
  TextInput(rString);
  mCompositionLength = rString.GetLength();
  SetCursorPos(mCompositionPos + CursorPosition);
  printf("  New cursor pos: %d\n", mCursorPos);
}

nglString nuiEditText::GetTextComposition() const
{
  if (mCompositionPos < 0)
    return nglString::Null;
  return mText.Extract(mCompositionPos, mCompositionLength);
}

void nuiEditText::TextCompositionIndexToPoint(int32 CursorPosition, float& x, float& y) const
{
  float X = 0;
  float Y = 0;
  GetCoordsFromPos(CursorPosition, X, Y);
  x = ToBelow(X);
  y = ToBelow(Y);
}

bool nuiEditText::KeyDown(const nglKeyEvent& rEvent)
{
  if (IsKeyDown(NK_LMOD) || IsKeyDown(NK_RMOD))
  {
    mCommandDown = true;
  }

  if (rEvent.mKey == NK_LSHIFT || rEvent.mKey == NK_RSHIFT)
  {
    mShiftDown = true;
    Do(eStartSelection, new nuiObject());
    return true;
  }

  if (rEvent.mKey == NK_LALT || rEvent.mKey == NK_RALT)
  {
    mAltDown = true;
    return true;
  }

  if (mCommandDown)
  {
    if (rEvent.mKey == NK_Z)
    {
      if (mShiftDown)
        Redo();
      else
        Undo();
      return true;
    }

    std::map<nglKeyCode, CommandId>::iterator it = mCommandKeyBindings.find(rEvent.mKey);
    if (it != mCommandKeyBindings.end())
    {
      Do(it->second, new nuiObject());
      return true;
    }
    return false;
  }

  if (mAltDown)
  {
    std::map<nglKeyCode, CommandId>::iterator it = mAltKeyBindings.find(rEvent.mKey);
    if (it != mAltKeyBindings.end())
    {
      Do(it->second, new nuiObject());
      return true;
    }
    return false;
  }

  std::map<nglKeyCode, CommandId>::iterator it = mKeyBindings.find(rEvent.mKey);
  if (it != mKeyBindings.end())
  {
    Do(it->second, new nuiObject());
    return true;
  }

//  if ((uint)Char)
//  {
//    if (Char == 13)
//      Char = '\n';
//    nuiObject* pObject = new nuiObject();
//    pObject->SetProperty(_T("Text"), nglString(Char));
//    Do(eInsertText, pObject);
//    return true;
//  }

  return false;
}

bool nuiEditText::KeyUp(const nglKeyEvent& rEvent)
{
  std::map<nglKeyCode, CommandId>::iterator it = mKeyBindings.find(rEvent.mKey);

  mCommandDown = false;

  if (rEvent.mKey == NK_LSHIFT || rEvent.mKey == NK_RSHIFT)
  {
    mShiftDown = false;
    Do(eStopSelection, new nuiObject());
    return true;
  }

  if (rEvent.mKey == NK_LALT || rEvent.mKey == NK_RALT)
  {
    mAltDown = false;
    return true;
  }

  if (it != mKeyBindings.end())
  {
    return true;
  }

  return !(IsKeyDown(NK_LMOD) || IsKeyDown(NK_RMOD) || IsKeyDown(NK_LALT) || IsKeyDown(NK_RALT));
}

void nuiEditText::MoveDropCursorTo(nuiSize X, nuiSize Y)
{
  uint Pos = GetPosFromCoords(X, Y, true);
  
  if (Pos > (uint)mText.GetLength())
    Pos = mText.GetLength();
  mDropCursorPos = Pos;
  
  //if (mFollowModifications)
  {
    nuiSize x, y;
    nglFontInfo info;
    mpFont->GetInfo(info);
    x = 0;
    y = info.Ascender;
    GetCoordsFromPos(mDropCursorPos, x, y);
    
    nuiRect r(x, y - mpFont->GetHeight(), 2.0f, mpFont->GetHeight()*2.0f);  
    SetHotRect(r);
  }
  Invalidate();
}

void nuiEditText::MoveCursorTo(nuiSize X, nuiSize Y)
{
  MoveCursorTo(GetPosFromCoords(X, Y, true));
}

bool nuiEditText::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool handled = false;

  if (Button & nglMouseInfo::ButtonLeft || Button & nglMouseInfo::ButtonRight)
  {
    uint pos = GetPosFromCoords(X, Y, true);
    if ( mAnchorPos != mCursorPos && 
         MIN(mAnchorPos, mCursorPos) < pos && 
         pos < MAX(mAnchorPos, mCursorPos)  )
    {
      mStartDragging = true;
      mDragging = false;
      Grab();
      return true;
    }

    handled = true;
    if (Button & nglMouseInfo::ButtonDoubleClick)
    {
      if (!mCommandStack.empty() && mCommandStack.back().first == eSelectWord)
        Do(eSelectAll, new nuiObject());
      else
        Do(eSelectWord, new nuiObject());
    }
    else
    {
      MoveCursorTo(X,Y);
      mSelecting = true;
    }
  }
  Grab();
  Invalidate();
  return handled;
}

bool nuiEditText::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool handled = false;
  if (Button & nglMouseInfo::ButtonLeft || Button & nglMouseInfo::ButtonRight)
  {
    if (mStartDragging)
      MoveCursorTo(X,Y);
    mStartDragging = false;
    mDragging = false;
    mSelecting = false;
    handled = true;
  }
  Ungrab();
  return handled;
}

bool nuiEditText::MouseMoved(nuiSize X, nuiSize Y)
{
  if (mStartDragging && !mDragging)
  {
    Ungrab();
    mDragging = true;
    nglDragAndDrop* pDnd = new nglDragAndDrop(eDropEffectCopy);
    pDnd->AddType(new nglDataTextObject(_T("ngl/Text")));
    pDnd->AddSupportedDropEffect(eDropEffectMove);
    
    /*bool res = (unused)*/ Drag(pDnd);
    return true;
  }

  if (mSelecting)
  {
    MoveCursorTo(X,Y);
    return true;
  }
  return false;
}


// Do/Undo/Redo and commands;

void nuiEditText::InitCommands()
{
  mCommands.resize(eLastCommand);
  mCommands[eGoDocBegin] = &nuiEditText::GoDocBegin;
  mCommands[eGoDocEnd] = &nuiEditText::GoDocEnd;
  mCommands[eGoLineBegin] = &nuiEditText::GoLineBegin;
  mCommands[eGoLineEnd] = &nuiEditText::GoLineEnd;
  mCommands[eGoParagraphBegin] = &nuiEditText::GoParagraphBegin;
  mCommands[eGoParagraphEnd] = &nuiEditText::GoParagraphEnd;
  mCommands[eGoUp] = &nuiEditText::GoUp;
  mCommands[eGoDown] = &nuiEditText::GoDown;
  mCommands[eGoLeft] = &nuiEditText::GoLeft;
  mCommands[eGoRight] = &nuiEditText::GoRight;
  mCommands[eGoPageUp] = &nuiEditText::GoPageUp;
  mCommands[eGoPageDown] = &nuiEditText::GoPageDown;
  mCommands[eGoNextWord] = &nuiEditText::GoNextWord;
  mCommands[eGoPreviousWord] = &nuiEditText::GoPreviousWord;
  mCommands[eStartSelection] = &nuiEditText::StartSelection;
  mCommands[eStopSelection] = &nuiEditText::StopSelection;
  mCommands[eStartCommand] = &nuiEditText::StartCommand;
  mCommands[eStopCommand] = &nuiEditText::StopCommand;
  mCommands[eDisableSelection] = &nuiEditText::DisableSelection;
  mCommands[eSelectAll] = &nuiEditText::SelectAll;
  mCommands[eDeleteSelection] = &nuiEditText::DeleteSelection;
  mCommands[eSelectParagraph] = &nuiEditText::SelectParagraph;
  mCommands[eSelectLine] = &nuiEditText::SelectLine;
  mCommands[eSelectWord] = &nuiEditText::SelectWord;

  mCommands[eCopy] = &nuiEditText::Copy;
  mCommands[eCut] = &nuiEditText::Cut;
  mCommands[ePaste] = &nuiEditText::Paste;

  mCommands[eDeleteForward] = &nuiEditText::DeleteForward;
  mCommands[eDeleteBackward] = &nuiEditText::DeleteBackward;

  mCommands[eUndo] = &nuiEditText::Undo;
  mCommands[eRedo] = &nuiEditText::Redo;

  mCommands[eShowCursor] = &nuiEditText::ShowCursor;

  mCommands[eInsertText] = &nuiEditText::InsertText;
  mCommands[eNewLine] = &nuiEditText::NewLine;
}


void nuiEditText::ClearCommandStack()
{
  mCommandStackPos = 0;
  CropCommandStack();
}

void nuiEditText::CropCommandStack()
{
  for (uint i = mCommandStackPos; i < mCommandStack.size(); i++)
    delete mCommandStack[i].second;
  mCommandStack.resize(mCommandStackPos);
}

void nuiEditText::PushCommand(CommandId Id, nuiObject* pParams)
{
  CropCommandStack();
  mCommandStack.push_back(std::pair<CommandId, nuiObject*>(Id, pParams));
  mCommandStackPos++;
}

bool nuiEditText::Do(CommandId command, nuiObject* pParams)
{
  if (!pParams)
    pParams = new nuiObject();
  pParams->SetProperty(_T("Operation"), _T("Do"));
  CommandFunction pCommand = mCommands[command];
  if (!(this->*pCommand)(pParams))
    return false;

  // The command executed without error, lets add it to the undo stack:
  PushCommand(command, pParams);
  
  // and inform the user the text has changed
  if ( (command == eDeleteSelection) 
     || (command == eCut) || (command == ePaste) 
     || (command == eDeleteForward) || (command == eDeleteBackward) 
     || (command == eInsertText) )
  {
    TextChanged(); // send event
  }

  return true;
}

bool nuiEditText::Undo()
{
  if (!CanUndo())
    return false;
  mCommandStackPos--;
  std::pair<CommandId, nuiObject*>& rPair = mCommandStack[mCommandStackPos];
  rPair.second->SetProperty(_T("Operation"), _T("Undo"));
  CommandFunction pCommand = mCommands[rPair.first];
  return (this->*pCommand)(rPair.second);
}

bool nuiEditText::Redo()
{
  bool res = false;
  if (!CanRedo())
    return false;
  std::pair<CommandId, nuiObject*>& rPair = mCommandStack[mCommandStackPos];
  rPair.second->SetProperty(_T("Operation"), _T("Do"));
  CommandFunction pCommand = mCommands[rPair.first];
  res = (this->*pCommand)(rPair.second);
  mCommandStackPos++;
  return res;
}

bool nuiEditText::CanUndo() const
{
  return mCommandStackPos > 0;
}

bool nuiEditText::CanRedo() const
{
  return mCommandStackPos < mCommandStack.size();
}

nglString nuiEditText::GetSelection() const
{
  uint start = MIN(mAnchorPos, mCursorPos);
  uint end = MAX(mAnchorPos, mCursorPos);
  return mText.Extract(start, end - start);
}

const nglString& nuiEditText::GetText() const
{
  return mText;
}

void nuiEditText::SetText(const nglString& rText)
{
  mText = rText;
  ClearBlocks();
  CreateBlocks(mText, mpBlocks);
  MoveCursorTo(mText.GetLength());
}

void nuiEditText::ClearBlocks()
{
  uint count = (uint)mpBlocks.size();
  for (uint i = 0; i < count; i++)
  {
    delete mpBlocks[i];
  }

  mpBlocks.clear();
}

void nuiEditText::CreateBlocks(const nglString& rText, std::vector<TextBlock*>& rpBlocks)
{
  uint pos = 0;
  uint lastpos = 0;
  uint len = rText.GetLength();
  nuiRect global;
  nuiSize y = 0;

  while (pos < len)
  {
    nglChar c = 0;
    bool cont = true;

    // Isolate the next paragraph:
    while (pos < len && cont)
    {
      c = rText.GetChar(pos);
      pos++;
      if (c == '\n') // Eat CR
      {
        cont = false;
      }
    }

    TextBlock* pBlock = new TextBlock(mpFont, rText, lastpos, pos);
    nuiRect rect(pBlock->GetIdealSize());
    rect.Move(0, y);
    pBlock->SetRect(rect);
    y += rect.GetHeight();
    rpBlocks.push_back(pBlock);

    lastpos = pos;
  }

  if (rText.GetChar(pos-1) == '\n')
  {
    TextBlock* pBlock = new TextBlock(mpFont, rText, pos, pos);
    nuiRect rect(pBlock->GetIdealSize());
    rect.Move(0, y);
    pBlock->SetRect(rect);
    y += rect.GetHeight();
    rpBlocks.push_back(pBlock);
  }

  // Force relayout:
  for (uint i = 0; i < mpBlocks.size(); i++)
    mpBlocks[i]->Layout();

  InvalidateLayout();
}

void nuiEditText::SaveCursorPos(nuiObject* pParams) const
{
  nglString str;
  str.SetCUInt(mCursorPos);
  pParams->SetProperty(_T("CursorPos"), str);
}

void nuiEditText::LoadCursorPos(nuiObject* pParams)
{
  NGL_ASSERT(pParams->HasProperty(_T("CursorPos")));
  SetCursorPos(pParams->GetProperty(_T("CursorPos")).GetCUInt());
  Invalidate();
}

void nuiEditText::SaveAnchorPos(nuiObject* pParams) const
{
  nglString str;
  str.SetCUInt(mAnchorPos);
  pParams->SetProperty(_T("AnchorPos"), str);
}

void nuiEditText::LoadAnchorPos(nuiObject* pParams)
{
  NGL_ASSERT(pParams->HasProperty(_T("AnchorPos")));
  SetAnchorPos(pParams->GetProperty(_T("AnchorPos")).GetCUInt());
  Invalidate();
}

void nuiEditText::SavePos(nuiObject* pParams) const
{
  SaveCursorPos(pParams);
  SaveAnchorPos(pParams);
  if (mSelectionActive)
    pParams->SetProperty(_T("SelectionActive"), _T("true"));
}

void nuiEditText::LoadPos(nuiObject* pParams)
{
  LoadCursorPos(pParams);
  LoadAnchorPos(pParams);
  mSelectionActive = (pParams->HasProperty(_T("SelectionActive")) && pParams->GetProperty(_T("SelectionActive")) == _T("true"));
  Invalidate();
}

void nuiEditText::MoveCursorTo(uint Pos)
{
  if (!mSelecting)
  {
    SetAnchorPos(Pos);
    mSelectionActive = false;
  }
  else
  {
    mSelectionActive = (mAnchorPos != Pos);
  }
  SetCursorPos(Pos);
}

bool nuiEditText::GoDocBegin(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);
    MoveCursorTo(0);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoDocEnd(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);
    MoveCursorTo(mText.GetLength());
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoLineBegin(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    TextBlock* pBlock = GetBlock(mCursorPos);
    if (!pBlock)
      return false;

    SavePos(pParams);
    MoveCursorTo(pBlock->GetLineBeginFromPos(mCursorPos));
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoLineEnd(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    TextBlock* pBlock = GetBlock(mCursorPos);
    if (!pBlock)
      return false;

    SavePos(pParams);
    MoveCursorTo(pBlock->GetLineEndFromPos(mCursorPos));
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoParagraphBegin(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    TextBlock* pBlock = GetBlock(mCursorPos);
    if (!pBlock)
      return false;

    SavePos(pParams);
    MoveCursorTo(pBlock->GetPos());
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoParagraphEnd(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    TextBlock* pBlock = GetBlock(mCursorPos);
    if (!pBlock)
      return false;

    SavePos(pParams);
    MoveCursorTo(pBlock->GetEnd());
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoUp(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    TextBlock* pBlock = GetBlock(mCursorPos);
    if (!pBlock)
      return false;
    uint pos = pBlock->GetLineBeginFromPos(mCursorPos);
    uint offset = mCursorPos - pos;
    /* unused: uint line = 0;*/

    if (!pos)
      return false;// We already are on the first line
    pBlock = GetBlock(pos-1);
    NGL_ASSERT(pBlock);

    // Now ask for the line above the current line:
    pos = MIN(pBlock->GetLineEndFromPos(pos), pBlock->GetLineBeginFromPos(pos) + offset);

    SavePos(pParams);
    MoveCursorTo(pos);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoDown(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    TextBlock* pBlock = GetBlock(mCursorPos);
    if (!pBlock)
      return false;
    uint pos = pBlock->GetLineBeginFromPos(mCursorPos);
    uint offset = mCursorPos - pos;
    /* unused: uint line = 0; */

    if (pos >= (uint)mText.GetLength())
      return false;// We already are on the first line
    pos = pBlock->GetLineEndFromPos(pos) + 1;
    pBlock = GetBlock(pos);
    if (!pBlock)
      return false;

    // Now ask for the line above the current line:
    pos = MIN(pBlock->GetLineEndFromPos(pos), pBlock->GetLineBeginFromPos(pos) + offset);

    SavePos(pParams);
    MoveCursorTo(pos);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoLeft(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    if (!mCursorPos)
      return false;

    // Do
    SavePos(pParams);
    MoveCursorTo(mCursorPos - 1);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoRight(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    if (mCursorPos == (uint)mText.GetLength())
      return false;

    // Do
    SavePos(pParams);
    MoveCursorTo(mCursorPos + 1);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoPageUp(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    nuiSize x,y;
    if (!GetCoordsFromPos(mCursorPos, x, y))
      return false;
    SavePos(pParams);
    MoveCursorTo(GetPosFromCoords(x, y - (GetParent()->GetRect().GetHeight() / mpFont->GetHeight()), true));
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoPageDown(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    nuiSize x,y;
    if (!GetCoordsFromPos(mCursorPos, x, y))
      return false;
    SavePos(pParams);
    MoveCursorTo(GetPosFromCoords(x, y + (GetParent()->GetRect().GetHeight() / mpFont->GetHeight()), true));
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoNextWord(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);

    uint pos = mCursorPos;
    uint len = mText.GetLength();
    // Find next blank:
    bool cont;
    for (cont = true; pos < len && cont; pos++)
    {
      nglChar c = mText.GetChar(pos);
      if (c <= ' ')
        cont = false;
    }

    // Find next non blank:
    for (cont = true; pos < len && cont; pos++)
    {
      nglChar c = mText.GetChar(pos);
      if (c != ' ' && c != '\t')
      {
        MoveCursorTo(pos);
        return true;
      }
    }

    MoveCursorTo(pos);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::GoPreviousWord(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);

    uint pos = mCursorPos;
    /* unused: uint len = mText.GetLength(); */
    // Find next blank:
    bool cont;
    for (cont = true; pos && cont; pos--)
    {
      nglChar c = mText.GetChar(pos-1);
      if (c != ' ' && c != '\t')
        cont = false;
    }

    // Find next non blank:
    for (cont = true; pos && cont; pos--)
    {
      nglChar c = mText.GetChar(pos-1);
      if (c <= ' ')
      {
        MoveCursorTo(pos);
        return true;
      }
    }

    MoveCursorTo(pos);
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::StartSelection(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mSelecting = true;
  }
  else
  {
    // Undo
    mSelecting = false;
  }
  return true;
}

bool nuiEditText::StopSelection(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mSelecting = false;
  }
  else
  {
    // Undo
    mSelecting = true;
  }
  return true;
}

bool nuiEditText::StartCommand(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mCommandDown = true;
  }
  else
  {
    // Undo
    mCommandDown = false;
  }
  return true;
}

bool nuiEditText::StopCommand(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mCommandDown = false;
  }
  else
  {
    // Undo
    mCommandDown = true;
  }
  return true;
}

bool nuiEditText::StartShift(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mShiftDown = true;
  }
  else
  {
    // Undo
    mShiftDown = false;
  }
  return true;
}

bool nuiEditText::StopShift(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mShiftDown = false;
  }
  else
  {
    // Undo
    mShiftDown = true;
  }
  return true;
}

bool nuiEditText::StartAlt(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mAltDown = true;
  }
  else
  {
    // Undo
    mAltDown = false;
  }
  return true;
}

bool nuiEditText::StopAlt(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    mAltDown = false;
  }
  else
  {
    // Undo
    mAltDown = true;
  }
  return true;
}

bool nuiEditText::DisableSelection(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation")) == _T("Do"))
  {
    // Do
    SavePos(pParams);
    SetAnchorPos(mCursorPos);
    mSelectionActive = false;
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::SelectAll(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);

    SetAnchorPos(0);
    SetCursorPos(mText.GetLength());
    mSelectionActive = mAnchorPos != mCursorPos;
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::DeleteSelection(nuiObject* pParams)
{
  if (!IsEditable())
    return false;

  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    uint pos = 0;
    uint end = 0;
    SavePos(pParams);
    if (mSelectionActive && mAnchorPos != mCursorPos)
    {
      pos = MIN(mAnchorPos, mCursorPos);
      end = MAX(mAnchorPos, mCursorPos);
    }
    else
    {
      return false;
    }

    end = MIN(mText.GetLength(), end);
    pParams->SetProperty(_T("Text"), mText.Extract(pos, end - pos));
    mText.Delete(pos, end - pos);

    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    MoveCursorTo(pos);
    mSelectionActive = false;
    InvalidateLayout();
  }
  else
  {
    // Undo
    mText.Insert(pParams->GetProperty(_T("Text")), mCursorPos);

    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::SelectParagraph(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);

    TextBlock* pBlock = GetBlock(mCursorPos);
    uint start = pBlock->GetPos();
    uint end = pBlock->GetEnd();

    SetAnchorPos(start);
    SetCursorPos(end);
    mSelectionActive = mAnchorPos != mCursorPos;
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::SelectLine(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);

    TextBlock* pBlock = GetBlock(mCursorPos);
    uint start = pBlock->GetLineBeginFromPos(mCursorPos);
    uint end = pBlock->GetLineEndFromPos(mCursorPos);

    SetAnchorPos(start);
    SetCursorPos(end);
    mSelectionActive = mAnchorPos != mCursorPos;
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::SelectWord(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    SavePos(pParams);

    uint start = mCursorPos;
    uint end = mCursorPos;
    uint len = mText.GetLength();
    if (isalnum(mText.GetChar(start)))
    {
      // Select a word:
      while (start && isalnum(mText.GetChar(start-1)))
        start--;
      while (end < len && isalnum(mText.GetChar(end)))
        end++;
    }
    else
    {
      // Select a sign that is not a word
      while (start && !isalnum(mText.GetChar(start-1)) && mText.GetChar(start-1) > ' ')
        start--;
      while (end < len && !isalnum(mText.GetChar(end)) && mText.GetChar(start-1) > ' ')
        end++;
    }

    SetAnchorPos(start);
    SetCursorPos(end);
    mSelectionActive = mAnchorPos != mCursorPos;
  }
  else
  {
    // Undo
    LoadPos(pParams);
  }
  return true;
}


bool nuiEditText::Copy(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    App->GetClipBoard().SetData(nglMimeTextSource(GetSelection()));
  }
  return false;
}

bool nuiEditText::Cut(nuiObject* pParams)
{
  if (!IsEditable())
    return false;

  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    App->GetClipBoard().SetData(nglMimeTextSource(GetSelection()));
    Do(eDeleteSelection, new nuiObject());
  }
  return false;
}

bool nuiEditText::Paste(nuiObject* pParams)
{
  if (!IsEditable())
    return false;

  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    nglClipBoard& rClipBoard = App->GetClipBoard();
    if (rClipBoard.IsAvailable(nglMimeSource::TextMimeData))
    {
      nglMimeTextSource src;
      rClipBoard.GetData(src);
      // Do
      nuiObject* pObject = new nuiObject();
      pObject->SetProperty(_T("Text"), src.GetText());
      Do(eInsertText, pObject);
    }
  }
  return false;
}

bool nuiEditText::DeleteForward(nuiObject* pParams)
{
  if (!IsEditable())
    return false;

  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    uint pos = 0;
    uint end = 0;
    SavePos(pParams);
    if (mSelectionActive)
    {
      pos = MIN(mAnchorPos, mCursorPos);
      end = MAX(mAnchorPos, mCursorPos);
    }
    else
    {
      if (mCursorPos == (uint)mText.GetLength()) // Can't delete forward at the start of the text
        return false;
      pos = mCursorPos;
      end = mCursorPos + 1;
    }

    if (mCompositionPos >= 0 && pos >= mCompositionPos + mCompositionLength)
      return false;
    
    pParams->SetProperty(_T("Text"), mText.Extract(pos, end - pos));
    mText.Delete(pos, end - pos);

    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    if (mCompositionPos >= 0)
      mCompositionLength--;

    MoveCursorTo(pos);
    mSelectionActive = false;
    InvalidateLayout();
  }
  else
  {
    // Undo
    mText.Insert(pParams->GetProperty(_T("Text")), mCursorPos);

    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    LoadPos(pParams);
  }
  return true;
}

bool nuiEditText::DeleteBackward(nuiObject* pParams)
{
  if (!IsEditable())
    return false;

  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    uint pos = 0;
    uint end = 0;

    SavePos(pParams);
    if (mSelectionActive)
    {
      pos = MIN(mAnchorPos, mCursorPos);
      end = MAX(mAnchorPos, mCursorPos);
    }
    else
    {
      if (!mCursorPos) // Can't delete backward at the end of the text
        return false;

      pos = mCursorPos - 1;
      end = mCursorPos;
    }
    
    if (mCompositionPos >= 0 && pos < mCompositionPos)
      return false;

    pParams->SetProperty(_T("Text"), mText.Extract(pos, end - pos));
    mText.Delete(pos, end - pos);

    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    MoveCursorTo(pos);
    if (mCompositionPos >= 0)
      mCompositionLength--;

    mSelectionActive = false;
    InvalidateLayout();
  }
  else
  {
    // Undo
    mText.Insert(pParams->GetProperty(_T("Text")), mCursorPos);

    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    LoadPos(pParams);
  }
  return true;
}


bool nuiEditText::Undo(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    Undo();
    return false;
  }
  else
  {
    // Undo
    Redo();
    return false;
  }
  return true;
}

bool nuiEditText::Redo(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
    Redo();
    return false;
  }
  else
  {
    // Undo
    Undo();
    return false;
  }
  return true;
}


bool nuiEditText::ShowCursor(nuiObject* pParams)
{
  if (pParams->GetProperty(_T("Operation"))  == _T("Do"))
  {
    // Do
  }
  else
  {
    // Undo
  }
  return true;
}

bool nuiEditText::InsertText(nuiObject* pParams)
{
  if (!IsEditable())
    return false;

  const nglString& rText(pParams->GetProperty(_T("Text")));
  const nglString& rOp(pParams->GetProperty(_T("Operation")));
  if (rOp  == _T("Do"))
  {
    // Do
    if (mSelectionActive)
      Do(eDeleteSelection, new nuiObject());

    SavePos(pParams);
    mText.Insert(rText, mCursorPos);
    bool old = mSelecting;
    mSelecting = false;
    
    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    MoveCursorTo(mCursorPos + rText.GetLength());
    mSelecting = old;
    
    mSelectionActive = false;
    InvalidateLayout();
  }
  else
  {
    // Undo
    mText.Delete(mCursorPos - rText.GetLength(), rText.GetLength());
    ClearBlocks();
    CreateBlocks(mText, mpBlocks);

    LoadPos(pParams);
  }
  

  return true;
}

bool nuiEditText::NewLine(nuiObject* pParams)
{
  if (mCompositionPos < 0)
  {
    nuiObject* pObj = new nuiObject();
    pObj->SetProperty(_T("Text"), _T("\n"));
    return Do(eInsertText, pObj);
  }
  return false;
}


void nuiEditText::SetCursorPos(uint Pos)
{
  if (Pos > (uint)mText.GetLength())
    Pos = mText.GetLength();
  mCursorPos = Pos;
  if (mFollowModifications)
  {
    nuiSize x, y;
    GetCursorPos(x,y);
    nglFontInfo info;
    mpFont->GetInfo(info);
    nuiRect r(x, y - mpFont->GetHeight(), 2.0f, mpFont->GetHeight()*2.0f);
    SetHotRect(r);
  }
  Invalidate();

  //NGL_OUT(_T("Cursor pos = %d\n"), Pos);
}

uint nuiEditText::GetCursorPos() const
{
  return mCursorPos;
}

void nuiEditText::GetCursorPos(uint& rX, uint& rY)
{
  nuiSize x,y;
  GetCursorPos(x,y);
  rX = ToNearest(x);
  rY = ToNearest(y);
}

void nuiEditText::GetCursorPos(nuiSize& rX, nuiSize& rY)
{
  nglFontInfo info;
  mpFont->GetInfo(info);
  rX = 0;
  rY = info.Ascender;
  GetCoordsFromPos(mCursorPos, rX, rY);
}

void nuiEditText::SetAnchorPos(uint Pos)
{
  if (Pos > (uint)mText.GetLength())
    Pos = mText.GetLength();
  mAnchorPos = Pos;
  Invalidate();
}

uint nuiEditText::GetAnchorPos() const
{
  return mAnchorPos;
}

bool nuiEditText::SetFont(nuiTheme::FontStyle FontStyle)
{
  SetFont(nuiFont::GetFont(12));

  ClearBlocks();
  CreateBlocks(mText, mpBlocks);
  InvalidateLayout();  
  return true;
}

bool nuiEditText::SetFont(nuiFont* pFont, bool AlreadyAcquired)
{
  if (pFont == mpFont)
    return true;

  if (mpFont)
    mpFont->Release();

  mpFont = pFont;
  if (mpFont)
  {
    if (!AlreadyAcquired)
      mpFont->Acquire();
    nglFontInfo fontinfo;
    mpFont->GetInfo(fontinfo);
    mTextHeight = (nuiSize)ToAbove(fontinfo.AdvanceMaxH);
    nuiSize linegap = mTextHeight - (fontinfo.Ascender - fontinfo.Descender);
    mSelectGap = (linegap / 2) - fontinfo.Descender;
  
    ClearBlocks();
    CreateBlocks(mText, mpBlocks);
    InvalidateLayout();
  }
  return true;
}


nuiFont* nuiEditText::GetFont()
{
  return mpFont;
}

nuiEditText::TextBlock* nuiEditText::GetBlock(uint Pos) const
{
  for (uint i = 0; i < mpBlocks.size(); i++)
  {
    if (mpBlocks[i]->ContainsPos(Pos))
      return mpBlocks[i];
  }
  if (!mpBlocks.empty())
    return mpBlocks.back();
  return NULL;
}

uint nuiEditText::GetPosFromCoords(uint x, uint y, bool IgnoreWidth) const
{
  return GetPosFromCoords((nuiSize)x, (nuiSize)y, IgnoreWidth);
}

uint nuiEditText::GetPosFromCoords(nuiSize x, nuiSize y, bool IgnoreWidth) const
{
  if (y < 0)
    return 0;

  for (uint i = 0; i < mpBlocks.size(); i++)
  {
    TextBlock* pBlock = mpBlocks[i];
    if (IgnoreWidth)
    {
      const nuiRect& rRect(pBlock->GetRect());
      if (rRect.Top() <= y && rRect.Bottom() >= y) 
        return pBlock->GetPosFromCoords(x,y); 
    }
    else
    {
      if (pBlock->GetRect().IsInside(x,y))
        return pBlock->GetPosFromCoords(x,y); 
    }
  }

  return mText.GetLength();
}

bool nuiEditText::GetCoordsFromPos(uint Pos, uint& x, uint& y) const
{
  nuiSize fx, fy;
  if (GetCoordsFromPos(Pos, fx, fy))
  {
    x = ToNearest(fx);
    y = ToNearest(fy);
    return true;
  }

  return false;
}

bool nuiEditText::GetCoordsFromPos(uint Pos, nuiSize& x, nuiSize& y) const
{
  TextBlock* pBlock = GetBlock(Pos);
  if (!pBlock)
    return false;

  pBlock->GetCoordsFromPos(Pos, x, y);
  return true;
}

void nuiEditText::SetEditable(bool Set)
{
  mIsEditable = Set;
  Invalidate();
}

bool nuiEditText::IsEditable()
{
  return mIsEditable;
}

void nuiEditText::AddText(const nglString& rText)
{
  bool editable = IsEditable();
  SetEditable(true);
  Do(eDisableSelection, new nuiObject());
  Do(eGoDocEnd, new nuiObject());
  nuiObject* pObj = new nuiObject();
  pObj->SetProperty(_T("Text"), rText);
  Do(eInsertText, pObj);
  Do(eGoDocEnd, new nuiObject());
  SetEditable(editable);
}

void nuiEditText::Print(const nglChar* pFormat, ...)
{
  nglString str;

  if (!pFormat)
    return;
  
  va_list args;
  
  va_start(args, pFormat);
  str.Formatv(pFormat, args);
  va_end(args);
  
  AddText(str);
}

void nuiEditText::SetFollowModifications(bool Set)
{
  mFollowModifications = Set;
}

/////////////////////////////////////////
// nuiEditText::FontLayout
nuiEditText::FontLayout::FontLayout(nglFontBase& rFont, float PenX , float PenY)
: nuiFontLayout(rFont, PenX, PenY)
{
  mLineIndices.push_back(0); // Always create a default first line on the first char!
}

nuiEditText::FontLayout::~FontLayout()
{
}


void nuiEditText::FontLayout::OnGlyph (nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph)
{
  nuiFontLayout::OnGlyph(pFont, rString, Pos, pGlyph);
#if 0  
  nglChar c = rString[Pos];

  // Handle new line control char

  if (c == '\n')
  {
    mGlyphPrev = 0;
    return;
  }
  else if (c == '\t')
  {
    nglFontInfo info;
    mFont.GetInfo(info);
    uint ndx = 0;
    mFont.GetGlyphIndexes(_T(" "), 1, &ndx, 1);
    nglGlyphInfo Glyph;
    Glyph.Index = ndx;
    Glyph.AdvanceX = info.AdvanceMaxW * mSpacesPerTab;
    Glyph.AdvanceY = 0;
    Glyph.BearingX = 0;
    Glyph.BearingY = 0;
    Glyph.Height = 0;
    Glyph.Width = Glyph.AdvanceX;

    AddGlyph(mPenX, mPenY, Pos, &Glyph);
    mPenX += Glyph.AdvanceX;
  }

  if (c < _T(' ') ||  // skip control chars (includes newline)
    !pGlyph)        // no glyph to render
  {
    mGlyphPrev = 0;
    return;
  }

  // Add kerning vector if applicable
  float kx = 0, ky = 0;
  if (GetKerning(pGlyph->Index, kx, ky))
  {
    mPenX += kx;
    mPenY += ky;
  }

  // Add this glyph to the layout with the current position pen position
  AddGlyph(mPenX, mPenY, Pos, pGlyph);

  // Proceed with glyph advance
  mPenX += pGlyph->AdvanceX;
  mPenY += pGlyph->AdvanceY;
#endif
}



/////////////////////////////////////////////
//nuiEditText::TextBlock
/////////////////////////////////////////////
nuiEditText::TextBlock::TextBlock(nuiFont* pFont, const nglString& rString, uint begin, uint end)
: mBegin(begin),
mEnd(end),
mpLayout(NULL),
mrString(rString),
mpFont(pFont),
mLayoutOK(false)
{
  mBegin = begin;
  mEnd = end;
  if (mpFont)
    mpFont->Acquire();
}

nuiEditText::TextBlock::~TextBlock()
{
  delete mpLayout;
  mpFont->Release();
}

void nuiEditText::TextBlock::Draw(nuiDrawContext* pContext, nuiSize X, nuiSize Y, uint SelectionBegin, uint SelectionEnd, uint CompositionBegin, uint CompositionEnd, nuiSize width)
{
  if (!(SelectionBegin == SelectionEnd || SelectionBegin > GetEnd() || SelectionEnd < GetPos())) // Are we concerned about the selection?
  {
    // Draw the selection:
    if (SelectionBegin <= GetPos() && SelectionEnd >= GetEnd())
    {
      // We are completely enclosed by the selection
      nuiRect r(mRect);
      r.Right() = width;
      pContext->DrawRect(r, eFillShape);
    }
    else
    {
      const std::vector<uint>& rLines = mpLayout->GetLines();
      uint lines = (uint)rLines.size();
      nuiSize height = mpFont->GetHeight();
      nuiSize offset = mRect.Top();

      for (uint i = 0; i < lines; i++)
      {
        uint linepos = rLines[i] + GetPos();
        uint lineend = GetLineEndFromPos(linepos);
        if (!(SelectionBegin > lineend || SelectionEnd < linepos))
        {
          // We have at least a part in the selection
          nuiSize StartX = 0, 
            StopX = width, 
            StartY = offset + i * height, 
            StopY = offset + (i+1) * height;

          nuiSize dummy = 0;

          if (SelectionBegin >= linepos)
          {
            GetCoordsFromPos(SelectionBegin, StartX, dummy);
          }

          if (SelectionEnd <= lineend)
          {
            GetCoordsFromPos(SelectionEnd, StopX, dummy);
          }

          nuiRect r(StartX, StartY, StopX, StopY, false);

          pContext->DrawRect(r, eFillShape);
        }
      }
    }
  }

  if (!(CompositionBegin == CompositionEnd || CompositionBegin > GetEnd() || CompositionEnd < GetPos())) // Are we concerned about the composition?
  {
    // Draw the selection:
    if (CompositionBegin <= GetPos() && CompositionEnd >= GetEnd())
    {
      // We are completely enclosed by the selection
      nuiRect r(mRect);
      r.Right() = width;
      pContext->DrawRect(r, eFillShape);
    }
    else
    {
      const std::vector<uint>& rLines = mpLayout->GetLines();
      uint lines = (uint)rLines.size();
      nuiSize height = mpFont->GetHeight();
      nuiSize offset = mRect.Top();
      
      for (uint i = 0; i < lines; i++)
      {
        uint linepos = rLines[i] + GetPos();
        uint lineend = GetLineEndFromPos(linepos);
        if (!(CompositionBegin > lineend || CompositionEnd < linepos))
        {
          // We have at least a part in the selection
          nuiSize StartX = 0, 
          StopX = width, 
          StartY = offset + i * height, 
          StopY = offset + (i+1) * height;
          
          nuiSize dummy = 0;
          
          if (CompositionBegin >= linepos)
          {
            GetCoordsFromPos(CompositionBegin, StartX, dummy);
          }
          
          if (CompositionEnd <= lineend)
          {
            GetCoordsFromPos(CompositionEnd, StopX, dummy);
          }
          
          //nuiRect r(StartX, StartY, StopX, StopY, false);
          
          //pContext->DrawRect(r, eFillShape);
          pContext->DrawLine(StartX, StopY, StopX, StopY);
        }
      }
    }
  }
  
  pContext->SetFont(mpFont);
  pContext->DrawText(X, Y, *mpLayout);

}

void nuiEditText::TextBlock::SetRect(const nuiRect& rRect)
{
  mRect = rRect;
}

uint nuiEditText::TextBlock::GetPos() const
{
  return mBegin;
}

uint nuiEditText::TextBlock::GetLength() const
{
  return mEnd - mBegin;
}

uint nuiEditText::TextBlock::GetEnd() const
{
  return mEnd;
}

void nuiEditText::TextBlock::SetPos(uint Pos)
{
  uint len = GetLength();
  mBegin = Pos;
  SetLength(len);
}

void nuiEditText::TextBlock::SetLength(uint Length)
{
  SetEnd(mBegin + Length);
}

void nuiEditText::TextBlock::SetEnd(uint End)
{
  mEnd = End;
  InvalidateLayout();
}


uint nuiEditText::TextBlock::GetLineHeight()
{
  nglFontInfo info;
  mpFont->GetInfo(info);
  return ToAbove(info.Height);
}


nuiSize nuiEditText::TextBlock::GetHeight()
{
  Layout();
  return mIdealRect.GetHeight();
}


uint nuiEditText::TextBlock::GetLineBeginFromPos(uint Pos)
{
  return GetPos();
}

uint nuiEditText::TextBlock::GetLineEndFromPos(uint Pos)
{
  if (GetLength())
  {
    if (mrString.GetChar(GetEnd()-1) == '\n')
      return GetEnd()-1;
  }
  return GetEnd();
}

bool nuiEditText::TextBlock::GetCoordsFromPos(uint Pos, uint& rX, uint& rY)
{
  nuiSize x, y;
  bool res = GetCoordsFromPos(Pos, x, y);

  rX = ToNearest(x);
  rY = ToNearest(y);

  return res;
}

bool nuiEditText::TextBlock::GetCoordsFromPos(uint Pos, nuiSize& rX, nuiSize& rY)
{
  Layout();
  Pos -= GetPos();
  rX = rY = 0;
  uint count = mpLayout->GetGlyphCount();
  if (Pos > count)
    return false;
  else if (!count)
  {
    rX = mRect.Left();
    rY = mpFont->GetHeight() + mRect.Top();
    return true;
  }

  // General Case:
  uint index = Pos;
  if (Pos == count)
    index--;

  const nglGlyphLayout* pGlyph = mpLayout->GetGlyph(index);
  rX = pGlyph->X + mRect.Left();
  rY = pGlyph->Y + mRect.Top();

  if (Pos == count)
  {
    nglGlyphInfo glyphinfo;
    pGlyph->mpFont->GetGlyphInfo(glyphinfo, pGlyph->Index, nglFontBase::eGlyphNative);
    rX += glyphinfo.AdvanceX;
  }
  return true;
}

uint nuiEditText::TextBlock::GetPosFromCoords(uint X, uint Y)
{
  return GetPosFromCoords((nuiSize)X, (nuiSize)Y);
}

uint nuiEditText::TextBlock::GetPosFromCoords(nuiSize X, nuiSize Y)
{
  uint line = ToBelow((Y - mRect.Top()) / mpFont->GetHeight());
  uint linescount = (uint)mpLayout->GetLines().size();
  if (line >= linescount)
    line = linescount - 1;
  uint pos = mpLayout->GetLines()[line];
  const nglGlyphLayout* pGlyph = NULL;
  nuiSize lastx = 0;

  uint count = mpLayout->GetGlyphCount();
  for (uint i = pos; i < count; i++)
  {
    pGlyph = mpLayout->GetGlyph(i);
    if (pGlyph)
    {
      nglGlyphInfo info;
      mpFont->GetGlyphInfo(info, pGlyph->Index, nglFontBase::eGlyphNative);
      nuiSize newx = pGlyph->X + (info.AdvanceX * .5f);
      if (X >= lastx && X < newx)
        return GetPos() + pGlyph->Pos;

      lastx = newx;
    }
  }

  if (pGlyph)
  {
    return GetLineEndFromPos(mpLayout->GetLines()[line]);
  }
  return GetPos();
}


bool nuiEditText::TextBlock::ContainsPos(uint Pos)
{
  return (Pos >= mBegin) && (Pos < mEnd);
}

const nuiRect& nuiEditText::TextBlock::GetIdealSize()
{
  Layout();
  return mIdealRect;
}

void nuiEditText::TextBlock::Layout()
{
  if (mLayoutOK)
    return;

  delete mpLayout;
  mpLayout = NULL;

  nglFontInfo fontinfo;
  mpFont->GetInfo(fontinfo);
  mpLayout = new FontLayout(*mpFont, 0, fontinfo.Ascender);
  nglString tmp(mrString.Extract(GetPos(), GetLength()));
  mpLayout->Layout(tmp);

  /*
  nglGlyphInfo metrics;
  mpLayout->GetMetrics(metrics);
  if (metrics.Height == 0)
    metrics.Height = mpFont->GetHeight();
  //mIdealRect.Set(metrics.BearingX, metrics.BearingY, metrics.Width, metrics.Height);
  //mIdealRect.Set(0.0f, 0.0f, metrics.Width, mpLayout->GetLines().size() * fontinfo.Height);
  */
  mIdealRect = mpLayout->GetRect().Size();
  if (
      (mrString[mEnd-1] == '\n') &&
      (mEnd < mrString.GetLength())
    )
  {
    if (GetLength() == 1)
      mIdealRect.SetSize(mIdealRect.GetWidth(), mIdealRect.GetHeight() + fontinfo.Height);
  }
  mLayoutOK = true;
}

const nuiRect& nuiEditText::TextBlock::GetRect() const
{
  return mRect;
}

void nuiEditText::TextBlock::InvalidateLayout()
{
  mLayoutOK = false;
}


nglDropEffect nuiEditText::OnCanDrop(nglDragAndDrop* pDragObject,nuiSize X,nuiSize Y)
{
  if (pDragObject->IsTypeSupported(_T("ngl/Text")))
  {
    MoveDropCursorTo(X,Y);

    if ( mAnchorPos != mCursorPos && 
         MIN(mAnchorPos, mCursorPos) < GetPosFromCoords(X,Y,true) && 
         GetPosFromCoords(X,Y,true) < MAX(mAnchorPos, mCursorPos)  )
    {
      pDragObject->SetDesiredDropEffect(eDropEffectNone);
      return eDropEffectNone;
    }
    if (mStartDragging && !(IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL)))
      pDragObject->SetDesiredDropEffect(eDropEffectMove);
    else
      pDragObject->SetDesiredDropEffect(eDropEffectCopy);
  
    return eDropEffectCopy;
  }
  return eDropEffectNone;
}

void nuiEditText::OnDropped(nglDragAndDrop* pDragObject,nuiSize X,nuiSize Y, nglMouseInfo::Flags Button)
{
  mDropCursorPos=-1;
  
  nglDataTextObject* pText = (nglDataTextObject*)pDragObject->GetType(_T("ngl/Text"));
  const nglString& text = pText->GetData();
  
  uint pos;
  if (mStartDragging && !(IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL)))
  {
    pDragObject->SetDesiredDropEffect(eDropEffectMove);
    
    uint savedCursorPos= mCursorPos;
    uint savedAnchorPos= mAnchorPos;
    
    pos = GetPosFromCoords(X,Y, false);
    if (pos > MIN(savedAnchorPos, savedCursorPos))
      pos -= text.GetLength();

    Do(eDeleteSelection, NULL);
    
  }
  else
  {
    pos = GetPosFromCoords(X,Y, false);
    pDragObject->SetDesiredDropEffect(eDropEffectCopy);
  }

  SetCursorPos(pos);
  SetAnchorPos(pos);

  nuiObject* pObj = new nuiObject();
  pObj->SetProperty(_T("Text"), text);
  Do(eInsertText, pObj);

  /*
  uint savedCursorPos= mCursorPos;
  uint savedAnchorPos= mAnchorPos;
  */

  
  /*
  if (mStartDragging && pDragObject->GetDesiredDropEffect() == eDropEffectMove)
  {
    mSelectionActive=true;
    if ( savedCursorPos < mCursorPos - text.GetLength() )
    {
      mCursorPos= savedCursorPos;
      mAnchorPos= savedAnchorPos;
    }
    else 
    {
      mCursorPos= savedCursorPos+text.GetLength();
      mAnchorPos= savedAnchorPos+text.GetLength();
    }
    Do(eDeleteSelection, NULL);
  }
  */
}

void nuiEditText::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
  nglString data = GetSelection();
  
  nglDataTextObject* pText = (nglDataTextObject*)pDragObject->GetType(_T("ngl/Text"));
  if (!pText)
    pText = new nglDataTextObject(_T("ngl/Text"));
  pText->SetData(data);
  pDragObject->AddType(pText);
}

void nuiEditText::OnDragStop(bool canceled)
{
  mStartDragging = false;
  mDragging = false;
}



void nuiEditText::OnSetFocus(nuiWidgetPtr pWidget)
{
  if (pWidget == this)
  {
    nuiTopLevel* pTop = GetTopLevel();
    if (pTop)
    {
      nuiRect r(GetRect().Size());
      LocalToGlobal(r);
      pTop->StartTextInput(r.Left(), r.Top(), r.GetWidth(), r.GetHeight());
    }
  }
  else if (!pWidget)
  {
    nuiTopLevel* pTop = GetTopLevel();
    if (pTop)
    {
      pTop->EndTextInput();
    }
  }
}

