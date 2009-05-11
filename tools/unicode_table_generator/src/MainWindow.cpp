/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSV.h"
#include "nuiHTTP.h"
#include "nuiParser.h"

/*
 * MainWindow
 */

class Range
{
public:
  Range(const nglString& rName, int32 low, int32 hi)
  {
    mLow = low;
    mHi = hi;
    mName = rName;
  }
  
public:
  int32 mLow;
  int32 mHi;
  nglString mName;
};

bool RangeLess(const Range& rLeft, const Range& rRight)
{
  return rLeft.mLow < rRight.mLow;
}

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
{
  SetDebugMode(true);
  
  nuiScrollView* pScroll = new nuiScrollView();
  AddChild(pScroll);
  mpText = new nuiText();
  pScroll->AddChild(mpText);
  
  mpText->AddText(_T("unicode script parser\n"));
  
  nglString url = _T("http://www.unicode.org/Public/UNIDATA/Scripts.txt");
  nuiHTTPRequest req(url);
  nuiHTTPResponse* pResponse = req.SendRequest();
  if (!pResponse)
  {
    mpText->AddText(_T("Unable to connect to ") + url);
    return;
  }

  nglIMemory stream(&pResponse->GetBody()[0], pResponse->GetBody().size());
  nuiParser parser(&stream, url);
  
  std::list<Range> Ranges;
  std::set<nglString> Scripts;
  
#define ERROR(x) { mpText->AddText(x); mpText->AddText(_T("\n")); NGL_OUT(x); NGL_ASSERT(0); return; }
  parser.NextChar();
  while (!parser.IsDone())
  {
    if (!parser.SkipBlank())
      ERROR(_T("parser error\n"));
    
    if (parser.GetChar() == '#')
    {
      if (!parser.SkipToNextLine())
        ERROR(_T("parser error in comment"));
    }
    else
    {
      uint32 low = 0;
      uint32 high = 0;
      if (!parser.GetInteger(low, 16))
        ERROR(_T("parser error while looking for low code point"));
      
      high = low;
      
      if (parser.GetChar() == '.')
      {
        if (!(parser.NextChar() && parser.GetChar() == '.'))
          ERROR(_T("parser error while looking for range separator (..)"));
        
        if (!parser.NextChar())
          ERROR(_T("parser error while looking for range end (after ..)"));
        
        if (!parser.GetInteger(high, 16))
          ERROR(_T("parser error while looking for high code point"));
      }
      
      // now parse the script name
      if (!parser.SkipBlank())
        ERROR(_T("parser error while looking for ;\n"));
      
      if (parser.GetChar() != ';')
        ERROR(_T("parser error while looking for ;\n"));

      if (!parser.NextChar())
        ERROR(_T("parser error while looking script"));

      if (!parser.SkipBlank())
        ERROR(_T("parser error while looking for script name\n"));
      
      nglString script;
      if (!parser.GetSymbol(script))
        ERROR(_T("parser error while reading script name\n"));
      
      //NGL_OUT(_T("found %ls %04x..%04x\n"), script.GetChars(), low, high);
      Ranges.push_back(Range(script, low, high));
      Scripts.insert(script);
      
      if (!parser.SkipToNextLine())
        ERROR(_T("parser error skipping to next line"));

    }
  }

  std::set<nglString>::iterator sit = Scripts.begin();
  std::set<nglString>::iterator send = Scripts.end();

  NGL_OUT(_T("enum nuiUnicodeScript\n{\n"));
  while (sit != send)
  {
    nglString s(*sit);
    
    NGL_OUT(_T("  eScript%ls,\n"), s.GetChars());

    ++sit;
  }
  NGL_OUT(_T("};\n\n"));
  
  Ranges.sort(RangeLess);

  std::list<Range> miniranges;
  
  
  std::list<Range>::iterator it = Ranges.begin();
  std::list<Range>::iterator end = Ranges.end();
  Range old(nglString::Empty, 0, 0);
  old = *it;
  ++it;
  while (it != end)
  {
    Range& r(*it);
    
    if (r.mName == old.mName)
    {
      // merge this
      old.mHi = r.mHi;
    }
    else
    {
      miniranges.push_back(old);
      old = r;
    }
    
    ++it;
  }
  
  
  NGL_OUT(_T("class nuiScriptRange\n{\npublic:\n  nuiUnicodeScript mScript;\n  int32 mLow;\n  int32 mHigh;\n};\n\n"));
  
  NGL_OUT(_T("nuiScriptRange nuiScriptRanges[] =\n{\n"));
  it = miniranges.begin();
  end = miniranges.end();
  while (it != end)
  {
    Range& r(*it);
    NGL_OUT(_T("  { eScript%ls, %d, %d },\n"), r.mName.GetChars(), r.mLow, r.mHi);
    ++it;
  }    
  NGL_OUT(_T("};\n\n"));
}

MainWindow::~MainWindow()
{
}

