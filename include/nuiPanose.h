/*
 *  nuiPanose.h
 *  nui3
 *
 *  Created by Sebastien Metrot on 10/20/08.
 *  Copyright 2008 MXP4. All rights reserved.
 *
 */

#pragma once

#include "nui.h"

/////////////////////////////
// PANOSE Structures and defines:
enum nuiFontFamily
{
  eFamily_Any = 0,
  eFamily_NoFit,
  eFamily_TextDisplay,
  eFamily_Script,
  eFamily_Decorative,
  eFamily_Pictorial
};

enum nuiFontSerif
{
  eSerif_Any = 0,
  eSerif_NoFit,
  eSerif_Cove,
  eSerif_ObtuseCove,
  eSerif_SquareCore,
  eSerif_ObtuseSquareCove,
  eSerif_Square,
  eSerif_Thin,
  eSerif_Bone,
  eSerif_Exaggerated,
  eSerif_Triangle,
  eSerif_NormalSans,
  eSerif_ObtuseSans,
  eSerif_PerpSans,
  eSerif_Flared,
  eSerif_Rounded
};


enum nuiFontWeight
{
  eWeight_Any = 0,
  eWeight_NoFit,
  eWeight_VeryLight,
  eWeight_Light,
  eWeight_Thin,
  eWeight_Book,
  eWeight_Medium,
  eWeight_Demi,
  eWeight_Bold,
  eWeight_Heavy,
  eWeight_Black,
  eWeight_Nord
};

enum nuiFontProportion
{
  eProportion_Any = 0,
  eProportion_NoFit,
  eProportion_OldStyle,
  eProportion_Modern,
  eProportion_EvenWidth,
  eProportion_Expanded,
  eProportion_Condensed,
  eProportion_VeryExpanded,
  eProportion_VeryCondensed,
  eProportion_Monospaced
};

enum nuiFontContrast
{
  eContrast_Any = 0,
  eContrast_NoFit,
  eContrast_None,
  eContrast_VeryLow,
  eContrast_Low,
  eContrast_MediumLow,
  eContrast_Medium,
  eContrast_MediumHigh,
  eContrast_High,
  eContrast_VeryHigh
};


enum nuiFontStrokeVariation
{
  eStrokeVariation_Any = 0,
  eStrokeVariation_NoFit,
  eStrokeVariation_GradualDiag,
  eStrokeVariation_GradualTran,
  eStrokeVariation_GradualVert,
  eStrokeVariation_GradualHorz,
  eStrokeVariation_RapidVert,
  eStrokeVariation_RapidHorz,
  eStrokeVariation_InstantVert
};

enum nuiFontArmStyle
{
  eArmStyle_Any = 0,
  eArmStyle_NoFit,
  eArmStyle_StraightHorz,
  eArmStyle_StraightWedge,
  eArmStyle_StraightVert,
  eArmStyle_StraightSingleSerif,
  eArmStyle_StraightDoubleSerif,
  eArmStyle_BentHorz,
  eArmStyle_BentWedge,
  eArmStyle_BentVert,
  eArmStyle_BentSingleSerif,
  eArmStyle_BentDoubleSerif
};

enum nuiFontLetterForm
{
  eLetterForm_Any = 0,
  eLetterForm_NoFit,
  eLetterForm_NormalCompact,
  eLetterForm_NormalWeighted,
  eLetterForm_NormalBoxed,
  eLetterForm_NormalFlattened,
  eLetterForm_NormalRounded,
  eLetterForm_NormalOffCenter,
  eLetterForm_NormalSquare,
  eLetterForm_ObliqueCompact,
  eLetterForm_ObliqueWeighted,
  eLetterForm_ObliqueBoxed,
  eLetterForm_ObliqueFlattened,
  eLetterForm_ObliqueRounded,
  eLetterForm_ObliqueOffCenter,
  eLetterForm_ObliqueSquare
};


enum nuiFontMidLine
{
  eMidLine_Any = 0,
  eMidLine_NoFit,
  eMidLine_StandardTrimmed,
  eMidLine_StandardPointed,
  eMidLine_StandardSerifed,
  eMidLine_HightTrimmed,
  eMidLine_HightPointed,
  eMidLine_HightSerifed,
  eMidLine_ConstantTrimmed,
  eMidLine_ConstantPointed,
  eMidLine_ConstantSerifed,
  eMidLine_LowTrimmed,
  eMidLine_LowPointed,
  eMidLine_LowSerifed
};


enum nuiFontXHeight
{
  eXHeight_Any = 0,
  eXHeight_NoFit,
  eXHeight_ConstantSmall,
  eXHeight_ConstantStandard,
  eXHeight_ConstantLarge,
  eXHeight_DuckingSmall,
  eXHeight_DuckingStandard,
  eXHeight_DuckingLarge
};


typedef struct nuiFontPanoseBytes
{
  uint8 mFamily;            // 0
  uint8 mSerif;             // 1
  uint8 mWeight;            // 2
  uint8 mProportion;        // 3
  uint8 mContrast;          // 4
  uint8 mStrokeVariation;   // 5
  uint8 mArmStyle;          // 6
  uint8 mLetterForm;        // 7
  uint8 mMidLine;           // 8
  uint8 mXHeight;           // 9
} nuiFontPanoseBytes;

class nuiPanose
{
public:
  nuiPanose(const nuiFontPanoseBytes& rPanoseBytes);  
  nuiPanose(uint8 pPanoseBytes[10]);  
  nuiPanose(const nuiPanose& rPanose);
  
  void GetBytes(nuiFontPanoseBytes& rBytes) const;
  void SetBytes(const nuiFontPanoseBytes& rBytes);

  uint32 GetDistance(const nuiPanose& rPanose) const;
  
  nuiFontFamily           GetFamily         () const;
  nuiFontSerif            GetSerif          () const;
  nuiFontWeight           GetWeight         () const;
  nuiFontProportion       GetProportion     () const;
  nuiFontContrast         GetContrast       () const;
  nuiFontStrokeVariation  GetStrokeVariation() const;
  nuiFontArmStyle         GetArmStyle       () const;
  nuiFontLetterForm       GetLetterForm     () const;
  nuiFontMidLine          GetMidLine        () const;
  nuiFontXHeight          GetXHeight        () const;
  
  void                    SetFamily         (nuiFontFamily           set);
  void                    SetSerif          (nuiFontSerif            set);
  void                    SetWeight         (nuiFontWeight           set);
  void                    SetProportion     (nuiFontProportion       set);
  void                    SetContrast       (nuiFontContrast         set);
  void                    SetStrokeVariation(nuiFontStrokeVariation  set);
  void                    SetArmStyle       (nuiFontArmStyle         set);
  void                    SetLetterForm     (nuiFontLetterForm       set);
  void                    SetMidLine        (nuiFontMidLine          set);
  void                    SetXHeight        (nuiFontXHeight          set);
  
  nglString Dump() const;
  
private:
  nuiFontFamily           mFamily;            // 0
  nuiFontSerif            mSerif;             // 1
  nuiFontWeight           mWeight;            // 2
  nuiFontProportion       mProportion;        // 3
  nuiFontContrast         mContrast;          // 4
  nuiFontStrokeVariation  mStrokeVariation;   // 5
  nuiFontArmStyle         mArmStyle;          // 6
  nuiFontLetterForm       mLetterForm;        // 7
  nuiFontMidLine          mMidLine;           // 8
  nuiFontXHeight          mXHeight;           // 9

  uint32 CompareFamily         (nuiFontFamily           set) const;
  uint32 CompareSerif          (nuiFontSerif            set) const;
  uint32 CompareWeight         (nuiFontWeight           set) const;
  uint32 CompareProportion     (nuiFontProportion       set) const;
  uint32 CompareContrast       (nuiFontContrast         set) const;
  uint32 CompareStrokeVariation(nuiFontStrokeVariation  set) const;
  uint32 CompareArmStyle       (nuiFontArmStyle         set) const;
  uint32 CompareLetterForm     (nuiFontLetterForm       set) const;
  uint32 CompareMidLine        (nuiFontMidLine          set) const;
  uint32 CompareXHeight        (nuiFontXHeight          set) const;
};


