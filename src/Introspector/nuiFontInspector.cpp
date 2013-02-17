/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"

int32 nuiFontInspector::UpdatingFonts = 0;

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

void nuiFontInspector::OnFontsChanged(const nuiEvent& rEvent)
{
  UpdateFonts();
}

void nuiFontInspector::UpdateFonts()
{
  if (UpdatingFonts)
    return;
  UpdatingFonts++;
  
  Clear();
  
  nuiScrollView* pScrollView = new nuiScrollView();
  AddChild(pScrollView);
  nuiGrid* pGrid = new nuiGrid(11, 0);
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
  
  const nglChar* Headers[] = { _T("ID"), _T("Family"), _T("Style"), _T("Size"), _T("Bold"), _T("Italic"), _T("Fixed"), _T("Scaleable"), _T("Glyphs"), _T("Panose"), _T("Preview"), NULL };

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
    nuiFontBase* pFont = Fonts[k];
    
    nuiLabel* pLabel = new nuiLabel(FontIDs[k]);
    pGrid->SetCell(j++, i, pLabel);
    
    
    // FamilyName
    pLabel = new nuiLabel(pFont->GetFamilyName());
    pGrid->SetCell(j++, i, pLabel);

    // StyleName
    pLabel = new nuiLabel(pFont->GetStyleName());
    pGrid->SetCell(j++, i, pLabel);

    // Size
    nglString str;
    float size = pFont->GetSize();
    str.SetCFloat(size);
    pLabel = new nuiLabel(str);
    pGrid->SetCell(j++, i, pLabel);

    // Bold
    nuiToggleButton* pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(pFont->IsBold());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Italic
    pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(pFont->IsItalic());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Proportionnal
    pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(!pFont->IsMonospace());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Scaleable
    pToggle = new nuiToggleButton();
    pToggle->SetDisplayAsCheckBox(true);
    pToggle->SetPressed(pFont->IsScalable());
    pToggle->SetEnabled(false);
    pGrid->SetCell(j++, i, pToggle, nuiCenter);

    // Glyph count
    str.SetCInt(pFont->GetGlyphCount());
    pLabel = new nuiLabel(str);
    pGrid->SetCell(j++, i, pLabel, nuiRight);

    // Panose
    nuiPanose panose(pFont->GetPanoseBytes());
    nuiText* pText = new nuiText(panose.Dump());
    pGrid->SetCell(j++, i, pText, nuiFillHorizontal);

    // Preview
    nuiLabel* pPreviewLabel = new nuiLabel(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n01234567890!@#$%^&*(){}[]\\|/?<>,.:"), nuiFont::GetFont(FontIDs[k]));
    //nuiLabel* pPreviewLabel = new nuiLabel(_T("u - v - w - U - V - W"), nuiFont::GetFont(FontIDs[k]));
    pGrid->SetCell(j++, i, pPreviewLabel, nuiLeft);
    
    // nglFontBase
  }
  UpdatingFonts--;
}

bool nuiFontInspector::SetRect(const nuiRect& rRect)
{
  UpdatingFonts++;

  bool res = nuiSimpleContainer::SetRect(rRect);
  
  UpdatingFonts--;
  
  return res;
}

bool nuiFontInspector::Draw(nuiDrawContext* pContext)
{
  UpdatingFonts++;
  
  bool res = nuiSimpleContainer::Draw(pContext);
  
  UpdatingFonts--;
  
  return res;
}

nuiRect nuiFontInspector::CalcIdealSize()
{
  UpdatingFonts++;
  
  nuiRect res = nuiSimpleContainer::CalcIdealSize();
  
  UpdatingFonts--;
  
  return res;
}

