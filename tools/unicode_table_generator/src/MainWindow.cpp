/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSV.h"

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
{
  SetDebugMode(true);
  
  nuiScrollView* pScroll = new nuiScrollView();
  AddChild(pScroll);
  mpText = new nuiText();
  pScroll->AddChild(mpText);
  
  mpText->AddText(_T("unicode script parser\n"));
  
  nuiCSV csv(_T(','));
  nglIStream* pStream = nglPath(_T("/Users/meeloo/Desktop/scripts.txt")).OpenRead();
  
  if (!pStream)
  {
    mpText->AddText(_T("Error loading text file"));
    return;
  }
  
  bool res = csv.Load(pStream, false);
  
  if (!res)
  {
    mpText->AddText(_T("Error parsing csv file"));
    return;
  }
  
  
  std::map<nglString, std::vector< std::pair<nglChar, nglChar> > > scripts;
  const std::vector<std::vector<nglString> >& rDoc(csv.GetDocument());

  for (uint32 i = 0; i < rDoc.size(); i++)
  {
    nglChar low = 0;
    nglChar high = 0;
    nglString script;
    const std::vector<nglString>& rLine(rDoc[i]);
    if (rLine.size() == 2)
    {
      // double first entry
      low = nglString(rLine[0].Extract(2)).GetCInt(16);
      high = low;
      script = rLine[1];
    }
    else if (rLine.size() == 3)
    {
      // complete line
      low = nglString(rLine[0].Extract(2)).GetCInt(16);
      high = nglString(rLine[1].Extract(2)).GetCInt(16);
      script = rLine[2];
    }
    
    if (scripts[script].empty())
      scripts[script].push_back(std::make_pair(low, high));
    else
    {
      std::vector< std::pair<nglChar, nglChar> >& rScript(scripts[script]);
      uint32 s = rScript.size() - 1;
      if (rScript[s].second + 1 == low)
        rScript[s].second = high;
      else
        rScript.push_back(std::make_pair(low, high));
    }
    
//    nglString str;
//    str.CFormat(_T("%ls\t%d - %d\n"), script.GetChars(), low, high);
//    mpText->AddText(str);
                    
  }

  
  std::map<nglString, std::vector< std::pair<nglChar, nglChar> > >::iterator it = scripts.begin();
  std::map<nglString, std::vector< std::pair<nglChar, nglChar> > >::iterator end = scripts.end();
  while (it != end)
  {
    const std::vector< std::pair<nglChar, nglChar> >& rScript(it->second);
    nglString script(it->first);
    nglString str;
    for (uint32 i = 0; i < rScript.size(); i++)
    {
      nglChar low = rScript[i].first;
      nglChar high = rScript[i].second;
      str.CFormat(_T("%ls\t%d - %d\n"), script.GetChars(), low, high);
      mpText->AddText(str);
      NGL_OUT(str.GetChars());
    }
    
    ++it;
  }
  
}

MainWindow::~MainWindow()
{
}

