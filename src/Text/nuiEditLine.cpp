/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

using namespace std;

nuiEditLine::nuiEditLine(const nglString& rText)
: nuiEditText(rText)
{
  if (SetObjectClass(_T("nuiEditLine")))
    InitAttributes();
  mMaxDisplayedChar = 0;
}

void nuiEditLine::InitAttributes()
{
  AddAttribute(new nuiAttribute<int>
               (nglString(_T("CharacterCountLimit")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiEditLine::GetCharacterCountLimit),
                nuiMakeDelegate(this, &nuiEditLine::LimitCharacterCount)));
}

nuiEditLine::~nuiEditLine()
{
}

bool nuiEditLine::TextInput(const nglString& rUnicodeString)
{
  bool res = nuiEditText::TextInput(rUnicodeString);
  if (mMaxDisplayedChar > 0)
  {
    if (GetText().GetLength() > mMaxDisplayedChar) // wrap
    {
      SetText(GetText().Extract(0, mMaxDisplayedChar));
    }
  }
  return res;
}

bool nuiEditLine::KeyDown (const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_UP)
  {
    HistoryUp();
    return true;
  }
  else if (rEvent.mKey == NK_DOWN)
  {
    HistoryDown();
    return true;
  }
  else if (rEvent.mKey == NK_TAB)
  {
    return AutoComplete();
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
  {
    if (Activated())
    {
      nuiTopLevel* pTop = GetTopLevel();
      pTop->EndTextInput();
    }
    Invalidate();
    return true;
  }
  else if (rEvent.mKey == NK_ESC)
  {
    Aborted();
    {
      nuiTopLevel* pTop = GetTopLevel();
      pTop->EndTextInput();
    }
    Invalidate();
    return true;
  }
  
  bool res = nuiEditText::KeyDown(rEvent);
  
  if (mMaxDisplayedChar > 0)
  {
    if (GetText().GetLength() > mMaxDisplayedChar) // wrap
    {
      SetText(GetText().Extract(0, mMaxDisplayedChar));
    }
  }
  return res;
}

bool nuiEditLine::KeyUp (const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_UP || rEvent.mKey == NK_DOWN || rEvent.mKey == NK_TAB)
  {
    return true;
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
  {
    return true;
  }
  else if (rEvent.mKey == NK_ESC)
  {
    return true;
  }
  bool res = nuiEditText::KeyUp(rEvent);
  
  if (mMaxDisplayedChar > 0)
  {
    if (GetText().GetLength() > mMaxDisplayedChar) // wrap
    {
      SetText(GetText().Extract(0, mMaxDisplayedChar));
    }
  }
  return res;
}

nuiRect nuiEditLine::CalcIdealSize()
{
  mIdealRect = nuiEditText::CalcIdealSize();
  mIdealRect.mBottom = mTextHeight;
  return mIdealRect;
}

void nuiEditLine::LimitCharacterCount(int maxDisplayedChar)
{
  mMaxDisplayedChar = maxDisplayedChar;
}

int nuiEditLine::GetCharacterCountLimit()
{
  return mMaxDisplayedChar;
}
