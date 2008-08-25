/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiEditLine.h"

using namespace std;

nuiEditLine::nuiEditLine(const nglString& rText)
: nuiEditText(rText)
{
  SetObjectClass(_T("nuiEditLine"));
  mMaxDisplayedChar = 0;
}

bool nuiEditLine::Load(const nuiXMLNode* pNode)
{
  nuiEditText::Load(pNode);
  SetObjectClass(_T("nuiEditLine"));
  mMaxDisplayedChar = 0;
  return true;
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
    AutoComplete();
    return true;
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
  {
    Selected();
    Invalidate();
    return true;
  }
  else if (rEvent.mKey == NK_ESC)
  {
    Aborted();
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

void nuiEditLine::LimitCharacterNumber(int maxDisplayedChar)
{
  mMaxDisplayedChar = maxDisplayedChar;
}
