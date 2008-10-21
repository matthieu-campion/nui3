/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "nuiComboBox.h"
#include "nuiFontManager.h"
#include "nuiBackgroundPane.h"

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

nuiTreeNode* GetTree(const char* pNames[])
{
  nuiTreeNode* pTree = new nuiTreeNode(_T("tree"));
  for (uint32 i = 0; pNames[i]; i++)
  {
    nuiTreeNode* pNode = new nuiTreeNode(nglString(pNames[i]));
    pTree->AddChild(pNode);
  }
  return pTree;
}

nuiTreeNode* GetFamilyTree()
{
  static const char* pNames[] = 
  {
    "eFamily_Any",
    "eFamily_NoFit",
    "eFamily_TextDisplay",
    "eFamily_Script",
    "eFamily_Decorative",
    "eFamily_Pictorial",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetSerifTree()
{
  static const char* pNames[] = 
  {
    "eSerif_Any",
    "eSerif_NoFit",
    "eSerif_Cove",
    "eSerif_ObtuseCove",
    "eSerif_SquareCore",
    "eSerif_ObtuseSquareCove",
    "eSerif_Square",
    "eSerif_Thin",
    "eSerif_Bone",
    "eSerif_Exaggerated",
    "eSerif_Triangle",
    "eSerif_NormalSans",
    "eSerif_ObtuseSans",
    "eSerif_PerpSans",
    "eSerif_Flared",
    "eSerif_Rounded",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetWeightTree()
{
  static const char* pNames[] = 
  {
    "eWeight_Any",
    "eWeight_NoFit",
    "eWeight_VeryLight",
    "eWeight_Light",
    "eWeight_Thin",
    "eWeight_Book",
    "eWeight_Medium",
    "eWeight_Demi",
    "eWeight_Bold",
    "eWeight_Heavy",
    "eWeight_Black",
    "eWeight_Nord",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetProportionTree()
{
  static const char* pNames[] = 
  {
    "eProportion_Any",
    "eProportion_NoFit",
    "eProportion_OldStyle",
    "eProportion_Modern",
    "eProportion_EvenWidth",
    "eProportion_Expanded",
    "eProportion_Condensed",
    "eProportion_VeryExpanded",
    "eProportion_VeryCondensed",
    "eProportion_Monospaced",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetContrastTree()
{
  static const char* pNames[] = 
  {
    "eContrast_Any",
    "eContrast_NoFit",
    "eContrast_None",
    "eContrast_VeryLow",
    "eContrast_Low",
    "eContrast_MediumLow",
    "eContrast_Medium",
    "eContrast_MediumHigh",
    "eContrast_High",
    "eContrast_VeryHigh",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetStrokeVariationTree()
{
  static const char* pNames[] = 
  {
    "eStrokeVariation_Any",
    "eStrokeVariation_NoFit",
    "eStrokeVariation_GradualDiag",
    "eStrokeVariation_GradualTran",
    "eStrokeVariation_GradualVert",
    "eStrokeVariation_GradualHorz",
    "eStrokeVariation_RapidVert",
    "eStrokeVariation_RapidHorz",
    "eStrokeVariation_InstantVert",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetArmStyleTree()
{
  static const char* pNames[] = 
  {
    "eArmStyle_Any",
    "eArmStyle_NoFit",
    "eArmStyle_StraightHorz",
    "eArmStyle_StraightWedge",
    "eArmStyle_StraightVert",
    "eArmStyle_StraightSingleSerif",
    "eArmStyle_StraightDoubleSerif",
    "eArmStyle_BentHorz",
    "eArmStyle_BentWedge",
    "eArmStyle_BentVert",
    "eArmStyle_BentSingleSerif",
    "eArmStyle_BentDoubleSerif",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetLetterFormTree()
{
  static const char* pNames[] = 
  {
    "eLetterForm_Any",
    "eLetterForm_NoFit",
    "eLetterForm_NormalCompact",
    "eLetterForm_NormalWeighted",
    "eLetterForm_NormalBoxed",
    "eLetterForm_NormalFlattened",
    "eLetterForm_NormalRounded",
    "eLetterForm_NormalOffCenter",
    "eLetterForm_NormalSquare",
    "eLetterForm_ObliqueCompact",
    "eLetterForm_ObliqueWeighted",
    "eLetterForm_ObliqueBoxed",
    "eLetterForm_ObliqueFlattened",
    "eLetterForm_ObliqueRounded",
    "eLetterForm_ObliqueOffCenter",
    "eLetterForm_ObliqueSquare",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetMidLineTree()
{
  static const char* pNames[] = 
  {
    "eMidLine_Any",
    "eMidLine_NoFit",
    "eMidLine_StandardTrimmed",
    "eMidLine_StandardPointed",
    "eMidLine_StandardSerifed",
    "eMidLine_HightTrimmed",
    "eMidLine_HightPointed",
    "eMidLine_HightSerifed",
    "eMidLine_ConstantTrimmed",
    "eMidLine_ConstantPointed",
    "eMidLine_ConstantSerifed",
    "eMidLine_LowTrimmed",
    "eMidLine_LowPointed",
    "eMidLine_LowSerifed",
    NULL
  };
  return GetTree(pNames);
}

nuiTreeNode* GetXHeightTree()
{
  static const char* pNames[] = 
  {
    "eXHeight_Any",
    "eXHeight_NoFit",
    "eXHeight_ConstantSmall",
    "eXHeight_ConstantStandard",
    "eXHeight_ConstantLarge",
    "eXHeight_DuckingSmall",
    "eXHeight_DuckingStandard",
    "eXHeight_DuckingLarge",
    NULL
  };
  return GetTree(pNames);
}


void MainWindow::OnCreation()
{
  nuiHBox* pMainBox = new nuiHBox();
  AddChild(pMainBox);
  
  nuiVBox* pRequestBox = new nuiVBox();

  mpPanoseFamily = new nuiComboBox(GetFamilyTree());
  mpPanoseSerif = new nuiComboBox(GetSerifTree());
  mpPanoseWeight = new nuiComboBox(GetWeightTree());
  mpPanoseProportion = new nuiComboBox(GetProportionTree());
  mpPanoseContrast = new nuiComboBox(GetContrastTree());
  mpPanoseStrokeVariation = new nuiComboBox(GetStrokeVariationTree());
  mpPanoseArmStyle = new nuiComboBox(GetArmStyleTree());
  mpPanoseLetterForm = new nuiComboBox(GetLetterFormTree());
  mpPanoseMidLine = new nuiComboBox(GetMidLineTree());
  mpPanoseXHeight = new nuiComboBox(GetXHeightTree());
  
  static const char* pNames[] = 
  {
    "Family",
    "Serif",
    "Weight",
    "Proportion",
    "Contrast",
    "StrokeVariation",
    "ArmStyle",
    "LetterForm",
    "MidLine",
    "XHeight",
    NULL
  };
  
  nuiComboBox* pCombos[] =
  {
    mpPanoseFamily,
    mpPanoseSerif,
    mpPanoseWeight,
    mpPanoseProportion,
    mpPanoseContrast,
    mpPanoseStrokeVariation,
    mpPanoseArmStyle,
    mpPanoseLetterForm,
    mpPanoseMidLine,
    mpPanoseXHeight,
    NULL
  };
  
  uint32 i = 0;
  for (; pCombos[i]; i++)
  {
    nuiHBox* pHB = new nuiHBox();
    pHB->AddCell(new nuiLabel(nglString(pNames[i])), nuiLeft);
    pHB->SetCellExpand(0, nuiExpandShrinkAndGrow);
    pHB->AddCell(pCombos[i], nuiRight);
    pCombos[i]->SetUserHeight(20);
    pCombos[i]->SetUserWidth(200);
    pCombos[i]->SetSelected((uint)0);
    pRequestBox->AddCell(pHB, nuiFillHorizontal);
    mpCombos.push_back(pCombos[i]);
    
    mEventSink.Connect(pCombos[i]->SelectionChanged, &MainWindow::OnRequestChanged);
  }

//  nuiDefaultDecoration::Dialog(pRequestBox);
  nuiBackgroundPane* pPane = new nuiBackgroundPane(eOutterBackground);
  pPane->AddChild(pRequestBox);
  pRequestBox->SetExpand(nuiExpandShrinkAndGrow);
  pMainBox->SetExpand(nuiExpandShrinkAndGrow);

  mpFontScroll = new nuiScrollView();
  
  pMainBox->AddCell(pPane);
  pMainBox->AddCell(mpFontScroll);
  pMainBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  
  const nuiEvent event;
  OnRequestChanged(event);
}

bool MainWindow::OnRequestChanged(const nuiEvent& rEvent)
{
  uint8 bytes[10];
  for (uint i = 0; i < mpCombos.size(); i++)
  {
    bytes[i] = mpCombos[i]->GetValue();
  }

  UpdateFontList(bytes);
  return false;
}

void MainWindow::UpdateFontList(uint8 bytes[10])
{
  mPanose.SetBytes(bytes);
  mpFontScroll->Clear(true);
  
  nuiFontRequest request;
  request.MustBeSimilar(mPanose, 10);
  std::list<nuiFontRequestResult> Fonts;
  nuiFontManager::GetManager().RequestFont(request, Fonts);
  
  nuiGrid* pGrid = new nuiGrid(4, Fonts.size() + 1);
  
  uint32 index = 1;
  std::list<nuiFontRequestResult>::const_iterator it = Fonts.begin();
  std::list<nuiFontRequestResult>::const_iterator end = Fonts.end();
  while (it != end)
  {
    const nuiFontRequestResult& rResult(*it);
    nglString str;
    str.SetCFloat(rResult.GetScore());
    pGrid->SetCell(0, index, new nuiLabel(str), nuiLeft, true, true);
    pGrid->SetCell(1, index, new nuiLabel(rResult.GetPath().GetNodeName().GetChars()), nuiLeft, true, true);
    str.SetCInt(rResult.GetFace());
    pGrid->SetCell(2, index, new nuiLabel(str), nuiLeft, true, true);
    pGrid->SetCell(3, index, new nuiLabel(rResult.GetPath().GetParent().GetChars()), nuiLeft, true, true);
    ++it;
    index++;
  }

  for (uint32 k = 0; k < pGrid->GetNbColumns(); k++)
  {
    pGrid->SetColumnExpand(k, nuiExpandShrinkAndGrow);
  }
  pGrid->SetPosition(nuiFill);
  
  pGrid->DisplayGridBorder(true, 1.0);
  mpFontScroll->AddChild(pGrid);
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



