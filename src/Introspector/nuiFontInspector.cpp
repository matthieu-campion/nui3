/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiFontInspector.h"
#include "nuiGrid.h"
#include "nuiToggleButton.h"
#include "nuiLabel.h"
#include "nuiFont.h"
#include "nuiIntrospector.h"


nuiFontInspector::nuiFontInspector()
: mSink(this)
{
  SetObjectClass(_T("nuiFontInspector"));
 
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  if (pDeco)
  {
    SetDecoration(pDeco, eDecorationBorder);
  }
  
  UpdateFonts();
  mSink.Connect(nuiFont::FontListChanged, &nuiFontInspector::OnFontsChanged);
}

nuiFontInspector::~nuiFontInspector()
{  
  
}

bool nuiFontInspector::OnFontsChanged(const nuiEvent& rEvent)
{
  UpdateFonts();
  return false;
}

void nuiFontInspector::UpdateFonts()
{
  Clear();
  
  nuiScrollView* pScrollView = new nuiScrollView();
  AddChild(pScrollView);
  nuiGrid* pGrid = new nuiGrid(10, 0);
  // Columns:
  // FontID, FamilyName, StyleName, Size, Bold, Italic, FixedWidth, GlyphCount, Path, Preview
  pGrid->DisplayGridBorder(true, 1);
  pGrid->SetColor(eBorder, nuiColor(200,200,200));
  pScrollView->AddChild(pGrid);
  
  std::vector<nuiFont*> Fonts;
  nuiFont::GetFonts(Fonts);
  std::vector<nglString> FontIDs;
  nuiFont::GetFontIDs(FontIDs);
  
  pGrid->AddRows(pGrid->GetNbRows(), Fonts.size() + 1);
  
  const nglChar* Headers[] = { _T("ID"), _T("Family"), _T("Style"), _T("Size"), _T("Bold"), _T("Italic"), _T("Fixed"), _T("Scaleable"), _T("Glyphs"), _T("Preview"), NULL };

  nuiDecoration* pTitleDeco = nuiDecoration::Get(INTROSPECTOR_DECO_GRID_TITLE);

  for (uint32 k = 0; Headers[k]; k++)
  {
    nuiLabel* pLabel = new nuiLabel(Headers[k]);
    pGrid->SetCell(k, 0, pLabel);
    pGrid->SetColumnExpand(k, nuiExpandShrinkAndGrow);
    
    if (pTitleDeco)
      pLabel->SetDecoration(pTitleDeco, eDecorationBorder);
  }
  

  
  for (uint32 i = 1, k = 0; i < Fonts.size() + 1; i++, k++)
  {
    uint32 j = 0;
    // FontID
    nuiLabel* pLabel = new nuiLabel(FontIDs[k]);
    pGrid->SetCell(j++, i, pLabel);
    
    
    // FamilyName
    pLabel = new nuiLabel(Fonts[k]->GetFamilyName());
    pGrid->SetCell(j++, i, pLabel);

    // StyleName
    pLabel = new nuiLabel(Fonts[k]->GetStyleName());
    pGrid->SetCell(j++, i, pLabel);

    // StyleName
    nglString str;
    str.SetCFloat(Fonts[k]->GetSize());
    pLabel = new nuiLabel(str);
    pGrid->SetCell(j++, i, pLabel);

    // Bold
    nuiToggleButton* pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(Fonts[k]->IsBold());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Italic
    pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(Fonts[k]->IsItalic());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Proportionnal
    pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(!Fonts[k]->IsMonospace());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Scaleable
    pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(Fonts[k]->IsScalable());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Glyph count
    str.SetCInt(Fonts[k]->GetGlyphCount());
    pLabel = new nuiLabel(str);
    pGrid->SetCell(j++, i, pLabel, nuiRight);

    // Preview
    nuiLabel* pPreviewLabel = new nuiLabel(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 01234567890!@#$%^&*(){}[]\\|/?<>,.:"), nuiFont::GetFont(FontIDs[k]));
    //nuiLabel* pPreviewLabel = new nuiLabel(_T("u - v - w - U - V - W"), nuiFont::GetFont(FontIDs[k]));
    pGrid->SetCell(j++, i, pPreviewLabel, nuiLeft);
    
    // nglFontBase
  }
}
