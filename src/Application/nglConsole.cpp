/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"


using namespace std;

/*
 * History
 */

list<nglString*>& nglConsole::GetHistory()
{
  return mHistory;
}

int nglConsole::GetHistory (list<const nglString*>& rMatches, nglString& rFilter, bool IsCaseSensitive)
{
  #if NGL_DISABLE_CONSOLE
  return 0;
  #else

  int count = 0;
  list<nglString*>::reverse_iterator h_entry;
  list<nglString*>::reverse_iterator h_begin = mHistory.rbegin();

  rMatches.clear();
  int size = rFilter.GetLength();
  for (h_entry = mHistory.rend(); h_entry != h_begin; h_entry--)
    if (!rFilter.Compare(**h_entry, 0, size, IsCaseSensitive))
    {
      rMatches.push_front(*h_entry);
      count++;
    }

  return count;
  #endif
}

nglString nglConsole::GetHistory (uint Line)
{
  #if NGL_DISABLE_CONSOLE
  return nglString::Null;
  #else

  uint i;
  list<nglString*>::iterator h_entry;
  list<nglString*>::iterator h_end = mHistory.end();

  for ( h_entry = mHistory.begin(), i = 0;
       (h_entry != h_end) && (i < Line);
        h_entry++, i++);

  return (i == Line) && h_entry != h_end ? **h_entry : nglString::Null;
  #endif
}

void nglConsole::UseHistory (bool Use, bool UseExpansion)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  mUseHistory = Use;
  mUseHExpansion = UseExpansion;
  #endif
}

void nglConsole::SetHistory (uint LineMax, uint CharMax)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  while ((!mHistory.empty()) &&
         (((LineMax > 0) && (mLineCnt > LineMax)) ||
          ((CharMax > 0) && (mCharCnt > CharMax))))
  {
    list<nglString*>::iterator tail = mHistory.end();
    tail--; // mHistory.end() return an iterator just past the last element
    if (*tail)
    {
      mCharCnt -= (*tail)->GetLength();
      mLineCnt--;
      delete (*tail);
      mHistory.erase (tail);
    }
  }
  mLineMax = LineMax;
  mCharMax = CharMax;
  #endif
}

void nglConsole::SetHistoryCase (bool IsCaseSensitive)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  mHistoryCase = IsCaseSensitive;
  #endif
}

void nglConsole::AddToHistory (const nglString& rLine)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  nglString* newline;
  uint new_linecnt, new_charcnt;

  if (!mUseHistory) return;

  // Filter empty lines
  nglString trimmed = rLine;
  trimmed.Trim();
  if (trimmed.GetLength() == 0) return;

  new_linecnt = mLineCnt + 1;
  new_charcnt = mCharCnt + rLine.GetLength();
  // Adjust history size according user-set limits
  while ((!mHistory.empty()) &&
         (((mLineMax > 0) && (new_linecnt > mLineMax)) ||
          ((mCharMax > 0) && (new_charcnt > mCharMax))))
  {
    list<nglString*>::iterator tail = mHistory.end();
    tail--; // mHistory.end() return an iterator just past the last element
    if (*tail)
    {
      new_charcnt -= (*tail)->GetLength();
      new_linecnt--;
      delete (*tail);
      mHistory.erase (tail);
    }
  }
  newline = new nglString (rLine);
  mHistory.push_front (newline);
  mLineCnt = new_linecnt;
  mCharCnt = new_charcnt;
  #endif
}

void nglConsole::ClearHistory ()
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  // free history content
  list<nglString*>::iterator h_entry;
  list<nglString*>::iterator h_end = mHistory.end();
  for (h_entry = mHistory.begin(); h_entry != h_end; ++h_entry)
    delete *h_entry;

  mHistory.clear();
  #endif
}

/*
 * Completion
 */

void nglConsole::UseCompletion (bool Use)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  mUseCompletion = Use;
  #endif
}

void nglConsole::SetCompletionCase (bool IsCaseSensitive)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  mCompletionCase = IsCaseSensitive;
  #endif
}


/*
 * Default user callbacks
 */

void nglConsole::OnInput (const nglString& rLine)
{
}

list<nglString>& nglConsole::OnCompletion (nglString& rLine, uint Start, uint End, list<nglString>& rAppendTo)
{
  return rAppendTo;
}


/*
 * Private intermediate callbacks
 */

void nglConsole::Input (nglString& rLine)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  rLine.TrimRight();
  if (mUseHExpansion && (!rLine.Compare(_T("!"), 0, 1)))
  {
    list<nglString*>::iterator h_entry;
    list<nglString*>::iterator h_end = mHistory.end();
    nglString base = rLine.Extract (1, -1);
    int size = base.GetLength();

    for (h_entry = mHistory.begin(); h_entry != h_end; ++h_entry)
      if (*h_entry)
      {
        if ((!base.Compare(_T("!"), 0, 1)) ||  // "!!" (repeat last)
            (!(*h_entry)->Compare (base, 0, size, mHistoryCase)))
        {
          NGL_OUT(_T("%s\n"), (nglChar*)(*h_entry));
          OnInput (**h_entry);
          return;
        }
      }
  }
  OnInput (rLine);
  #endif
}

list<nglString>& nglConsole::Completion (nglString& rLine, uint Start, uint End, list<nglString>& rAppendTo)
{
  // Handle history expansion via completion
  if (mUseHExpansion && (Start == 0) && (End > 0) && (rLine[0] == '!'))
  {
    // OK, we have at least the beginning '!'
    list<nglString*> hist = GetHistory();
    list<nglString*>::iterator hline;
    list<nglString*>::iterator hend = hist.end();

    // Traverse history and fetch matches
    for (hline = hist.begin(); hline != hend; ++hline)
      if (*hline)
      {
        /* End < 2 : the word to complete is '!' (display all history)
         * Else : match with rLine[1..End-1]
         */
        if ( (End < 2) ||
             (!rLine.Compare (**hline, 1, End - 1, mHistoryCase)) )
        {
          nglString newline = **hline;
          newline.Prepend ('!');
          rAppendTo.push_front(newline);
        }
      }
    return rAppendTo;
  }
  return OnCompletion (rLine, Start, End, rAppendTo);
}

void nglConsole::Setup()
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  mUseHistory = false;
  mUseHExpansion = false;
  mUseCompletion = false;
  mHistoryCase = false;
  mCompletionCase = false;
  mLineCnt = 0;
  mLineMax = 100;
  mCharCnt = 0;
  mCharMax = 0;
  #endif
}

void nglConsole::Output (const nglChar* pFormat, ...)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  nglCriticalSectionGuard guard(mCS);
  va_list args;

  va_start (args, pFormat);
  Outputv (pFormat, args);
  va_end (args);
  #endif

}

void nglConsole::Outputv (const nglChar* pFormat, va_list Args)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  if (this)   //#HACK This is a hack to have NGL_OUT working event when nuiInit hasn't been called yet
  {
    nglCriticalSectionGuard guard(mCS);
    mOutputBuffer.Formatv(pFormat, Args);
    OnOutput(mOutputBuffer);
  }
  else
  {
    nglString out;
    out.Formatv(pFormat, Args);
#ifdef _WIN32_
    OutputDebugString(out.GetChars());
#elif !defined _ANDROID_
    printf(_T("%s\n"), out.GetChars());
#else
    LOGI("%s", out.GetChars());
#endif
  }
  #endif
}

void nglConsole::Output (const nglString& rText)
{
  #if NGL_DISABLE_CONSOLE
  return;
  #else

  nglCriticalSectionGuard guard(mCS);
  OnOutput (rText);
  #endif
}
