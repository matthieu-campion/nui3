/*
 *  nuiPanose.cpp
 *  nui3
 *
 *  Created by Sebastien Metrot on 10/20/08.
 *  Copyright 2008 MXP4. All rights reserved.
 *
 */

#include "nuiPanose.h"

nuiPanose::nuiPanose(const nuiFontPanoseBytes& rPanoseBytes)
{
  mFamily = eFamily_Any;
  mSerif = eSerif_Any;
  mWeight = eWeight_Any;
  mProportion = eProportion_Any;
  mContrast = eContrast_Any;
  mStrokeVariation = eStrokeVariation_Any;
  mArmStyle = eArmStyle_Any;
  mLetterForm = eLetterForm_Any;
  mMidLine = eMidLine_Any;
  mXHeight = eXHeight_Any;
}

nuiPanose::nuiPanose(uint8 pPanoseBytes[10])
{
  mFamily = (nuiFontFamily) pPanoseBytes[0];
  mSerif = (nuiFontSerif) pPanoseBytes[1];
  mWeight = (nuiFontWeight) pPanoseBytes[2];
  mProportion = (nuiFontProportion) pPanoseBytes[3];
  mContrast = (nuiFontContrast) pPanoseBytes[4];
  mStrokeVariation = (nuiFontStrokeVariation) pPanoseBytes[5];
  mArmStyle = (nuiFontArmStyle) pPanoseBytes[6];
  mLetterForm = (nuiFontLetterForm) pPanoseBytes[7];
  mMidLine = (nuiFontMidLine) pPanoseBytes[8];
  mXHeight = (nuiFontXHeight) pPanoseBytes[9];
}

nuiPanose::nuiPanose(const nuiPanose& rPanose)
{
  mFamily = rPanose.mFamily;
  mSerif = rPanose.mSerif;
  mWeight = rPanose.mWeight;
  mProportion = rPanose.mProportion;
  mContrast = rPanose.mContrast;
  mStrokeVariation= rPanose.mStrokeVariation;
  mArmStyle = rPanose.mArmStyle;
  mLetterForm = rPanose.mLetterForm;
  mMidLine = rPanose.mMidLine;
  mXHeight = rPanose.mXHeight;
}

void nuiPanose::GetBytes(nuiFontPanoseBytes& rBytes) const
{
  rBytes.mFamily = mFamily;
  rBytes.mSerif = mSerif;
  rBytes.mWeight = mWeight;
  rBytes.mProportion = mProportion;
  rBytes.mContrast = mContrast;
  rBytes.mStrokeVariation = mStrokeVariation;
  rBytes.mArmStyle = mArmStyle;
  rBytes.mLetterForm = mLetterForm;
  rBytes.mMidLine = mMidLine;
  rBytes.mXHeight = mXHeight;
}

void nuiPanose::SetBytes(const nuiFontPanoseBytes& rBytes)
{
  mFamily = (nuiFontFamily) rBytes.mFamily;
  mSerif = (nuiFontSerif) rBytes.mSerif;
  mWeight = (nuiFontWeight) rBytes.mWeight;
  mProportion = (nuiFontProportion) rBytes.mProportion;
  mContrast = (nuiFontContrast) rBytes.mContrast;
  mStrokeVariation= (nuiFontStrokeVariation) rBytes.mStrokeVariation;
  mArmStyle = (nuiFontArmStyle) rBytes.mArmStyle;
  mLetterForm = (nuiFontLetterForm) rBytes.mLetterForm;
  mMidLine = (nuiFontMidLine) rBytes.mMidLine;
  mXHeight = (nuiFontXHeight) rBytes.mXHeight;
}

uint32 nuiPanose::GetDistance(const nuiPanose& rPanose) const
{
  return 0;
}


nuiFontFamily nuiPanose::GetFamily() const
{
  return mFamily;
}

nuiFontSerif nuiPanose::GetSerif() const
{
  return mSerif;
}

nuiFontWeight nuiPanose::GetWeight() const
{
  return mWeight;
}

nuiFontProportion nuiPanose::GetProportion() const
{
  return mProportion;
}

nuiFontContrast nuiPanose::GetContrast() const
{
  return mContrast;
}

nuiFontStrokeVariation nuiPanose::GetStrokeVariation() const
{
  return mStrokeVariation;
}

nuiFontArmStyle nuiPanose::GetArmStyle() const
{
  return mArmStyle;
}

nuiFontLetterForm nuiPanose::GetLetterForm() const
{
  return mLetterForm;
}

nuiFontMidLine nuiPanose::GetMidLine() const
{
  return mMidLine;
}

nuiFontXHeight nuiPanose::GetXHeight() const
{
  return mXHeight;
}


void nuiPanose::SetFamily(nuiFontFamily set)
{
  mFamily = set;
}

void nuiPanose::SetSerif(nuiFontSerif set)
{
  mSerif = set;
}

void nuiPanose::SetWeight(nuiFontWeight set)
{
  mWeight = set;
}

void nuiPanose::SetProportion(nuiFontProportion set)
{
  mProportion = set;
}

void nuiPanose::SetContrast(nuiFontContrast set)
{
  mContrast = set;
}

void nuiPanose::SetStrokeVariation(nuiFontStrokeVariation set)
{
  mStrokeVariation = set;
}

void nuiPanose::SetArmStyle(nuiFontArmStyle set)
{
  mArmStyle = set;
}

void nuiPanose::SetLetterForm(nuiFontLetterForm set)
{
  mLetterForm = set;
}

void nuiPanose::SetMidLine(nuiFontMidLine set)
{
  mMidLine = set;
}

void nuiPanose::SetXHeight(nuiFontXHeight set)
{
  mXHeight = set;
}


nglString nuiPanose::Dump() const
{
}


