/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiFontBrowser.h"

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiSplitter* pSplitter = new nuiSplitter(nuiHorizontal);
  pSplitter->SetMasterChild(false);
  AddChild(pSplitter);
  
  nuiFontBrowser* pBrowser = new nuiFontBrowser();
  pSplitter->AddChild(pBrowser);

  nuiScrollView* pScrollView = new nuiScrollView(true,true);
  pSplitter->AddChild(pScrollView);
  
  pScrollView->AddChild(CreateFontWindow());

  InvalidateLayout();
  pSplitter->SetHandlePos(50);

}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



nuiWidget* MainWindow::CreateFontWindow()
{
  nuiText* pText = new nuiText(_T(""));
  pText->SetFont(nuiTheme::Fixed);
  
  nuiFontManager& rFontManager(nuiFontManager::GetManager());
  
  std::vector<nuiFontDesc*> fonts;
  rFontManager.GetFonts(fonts);
  pText->Print(_T("%d fonts in the system.\n"), fonts.size());
  
  if (0)
  {
    pText->Print(_T("Requesting all the monospace fonts and prefer the ones that have bold glyphs:\n"));
    
    nuiFontRequest request;
    request.SetMonospace(1.0f, true);
    request.SetBold(true, 0.5f);
    
    nglString dump;
    std::list<nuiFontRequestResult> result;
    rFontManager.RequestFont(request, result);
    DumpFonts(result, dump);
    
    
    pText->Print(_T("%ls"), dump.GetChars());
  }
  
  if (1)
  {
    pText->Print(_T("Requesting all the fonts from the arial family and prefer the ones that have italic glyphs:\n"));
    
    nuiFontRequest request;
    request.SetName(_T("arial"), 1.0f, true);
    request.SetItalic(true, 0.5f);
    
    nglString dump;
    std::list<nuiFontRequestResult> result;
    rFontManager.RequestFont(request, result);
    DumpFonts(result, dump);
    
    
    pText->Print(_T("%ls"), dump.GetChars());
  }
  
  if (0)
  {
    
    pText->Print(_T("Requesting all the fonts than have the unicode character 0x4e2d:\n"));
    
    nuiFontRequest request;
    request.MustHaveGlyph(0x4e2d, 1.0f);
    
    nglString dump;
    std::list<nuiFontRequestResult> result;
    rFontManager.RequestFont(request, result);
    DumpFonts(result, dump);
    
    pText->Print(_T("%ls"), dump.GetChars());
  }
  
  return pText;
}



void MainWindow::DumpFonts(const std::list<nuiFontRequestResult>& rFonts, nglString& rString)
{
  std::list<nuiFontRequestResult>::const_iterator it = rFonts.begin();
  std::list<nuiFontRequestResult>::const_iterator end = rFonts.end();
  
  while (it != end)
  {
    const nuiFontRequestResult& rResult = *it;
    rString.AddFormat(_T("Score:\t\t%f\n"), rResult.GetScore());
    const nuiFontDesc* pFontDesc = rResult.GetFontDesc();
    rString.AddFormat(_T("Path:\t\t%ls\n"), pFontDesc->GetPath().GetChars());
    rString.AddFormat(_T("Family name:\t\t%ls\n"), pFontDesc->GetName().GetChars());
    rString.AddFormat(_T("Style  name:\t\t%ls\n"), pFontDesc->GetStyle().GetChars());
    rString.AddFormat(_T("Face:\t\t%d\n"), pFontDesc->GetFace());
    
    rString.AddFormat(_T("Bold:\t\t%ls\n"), YESNO(pFontDesc->GetBold()));
    rString.AddFormat(_T("Italic:\t\t%ls\n"), YESNO(pFontDesc->GetItalic()));
    rString.AddFormat(_T("Monospace:\t\t%ls\n"), YESNO(pFontDesc->GetMonospace()));
    rString.AddFormat(_T("Scalable:\t\t%ls\n"), YESNO(pFontDesc->GetScalable()));
    
    rString.AddFormat(_T("Encodings:\t\t%d\n"), (uint32)pFontDesc->GetEncodings().size());
    //rString.AddFormat(_T("Glyphs:\t\t%d\n"), (uint32)pFontDesc->GetGlyphs().size());
    
    if (!pFontDesc->GetScalable())
      rString.AddFormat(_T("Sizes:\t\t%d\n"), (uint32)pFontDesc->GetSizes().size());
    
    rString.AddFormat(_T("\n\n"));
    
    ++it;
  }
  
}
