/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiUnicode.h"

typedef struct nuiUnicodeRangeDesc
{
  nglChar RangeStart;
  nglChar RangeEnd;
  nuiUnicodeRange Range;
};

nuiUnicodeRangeDesc nuiUnicodeRanges[] =
{// Start , End   , Enum                                   
  { 0x0000,	0x007F,	eBasicLatin },
  { 0x0080,	0x00FF,	eC1ControlsAndLatin1Supplement },
  { 0x0100,	0x017F,	eLatinExtendedA },
  { 0x0180,	0x024F,	eLatinExtendedB },
  { 0x0250,	0x02AF,	eIPAExtensions },
  { 0x02B0,	0x02FF,	eSpacingModifierLetters },
  { 0x0300,	0x036F,	eCombiningDiacriticalMarks },
  { 0x0370,	0x03FF,	eGreekAndCoptic },
  { 0x0400,	0x04FF,	eCyrillic },
  { 0x0500,	0x052F,	eCyrillicSupplement },
  { 0x0530,	0x058F,	eArmenian },
  { 0x0590,	0x05FF,	eHebrew },
  { 0x0600,	0x06FF,	eArabic },
  { 0x0700,	0x074F,	eSyriac },
  { 0x0750,	0x077F,	eUndefined },
  { 0x0780,	0x07BF,	eThaana },
  { 0x07C0,	0x08FF,	eUndefined },
  { 0x0900,	0x097F,	eDevanagari },
  { 0x0980,	0x09FF,	eBengaliAndAssamese },
  { 0x0A00,	0x0A7F,	eGurmukhi },
  { 0x0A80,	0x0AFF,	eGujarati },
  { 0x0B00,	0x0B7F,	eOriya },
  { 0x0B80,	0x0BFF,	eTamil },
  { 0x0C00,	0x0C7F,	eTelugu },
  { 0x0C80,	0x0CFF,	eKannada },
  { 0x0D00,	0x0DFF,	eMalayalam },
  { 0x0D80,	0x0DFF,	eSinhala },
  { 0x0E00,	0x0E7F,	eThai },
  { 0x0E80,	0x0EFF,	eLao },
  { 0x0F00,	0x0FFF,	eTibetan },
  { 0x1000,	0x109F,	eMyanmar },
  { 0x10A0,	0x10FF,	eGeorgian },
  { 0x1100,	0x11FF,	eHangulJamo },
  { 0x1200,	0x137F,	eEthiopic },
  { 0x1380,	0x139F,	eUndefined },
  { 0x13A0,	0x13FF,	eCherokee },
  { 0x1400,	0x167F,	eUnifiedCanadianAboriginalSyllabics },
  { 0x1680,	0x169F,	eOgham },
  { 0x16A0,	0x16FF,	eRunic },
  { 0x1700,	0x171F,	eTagalog },
  { 0x1720,	0x173F,	eHanunoo },
  { 0x1740,	0x175F,	eBuhid },
  { 0x1760,	0x177F,	eTagbanwa },
  { 0x1780,	0x17FF,	eKhmer },
  { 0x1800,	0x18AF,	eMongolian },
  { 0x18B0,	0x18FF,	eUndefined },
  { 0x1900,	0x194F,	eLimbu },
  { 0x1950,	0x197F,	eTaiLe },
  { 0x1980,	0x19DF,	eUndefined },
  { 0x19E0,	0x19FF,	eKhmerSymbols },
  { 0x1A00,	0x1CFF,	eUndefined },
  { 0x1D00,	0x1D7F,	ePhoneticExtensions },
  { 0x1D80,	0x1DFF,	eUndefined },
  { 0x1E00,	0x1EFF,	eLatinExtendedAdditional },
  { 0x1F00,	0x1FFF,	eGreekExtended },
  { 0x2000,	0x206F,	eGeneralPunctuation },
  { 0x2070,	0x209F,	eSuperscriptsAndSubscripts },
  { 0x20A0,	0x20CF,	eCurrencySymbols },
  { 0x20D0,	0x20FF,	eCombiningDiacriticalMarksForSymbols },
  { 0x2100,	0x214F,	eLetterlikeSymbols },
  { 0x2150,	0x218F,	eNumberForms },
  { 0x2190,	0x21FF,	eArrows },
  { 0x2200,	0x22FF,	eMathematicalOperators },
  { 0x2300,	0x23FF,	eMiscellaneousTechnical },
  { 0x2400,	0x243F,	eControlPictures },
  { 0x2440,	0x245F,	eOpticalCharacterRecognition },
  { 0x2460,	0x24FF,	eEnclosedAlphanumerics },
  { 0x2500,	0x257F,	eBoxDrawing },
  { 0x2580,	0x259F,	eBlockElements },
  { 0x25A0,	0x25FF,	eGeometricShapes },
  { 0x2600,	0x26FF,	eMiscellaneousSymbols },
  { 0x2700,	0x27BF,	eDingbats },
  { 0x27C0,	0x27EF,	eMiscellaneousMathematicalSymbolsA },
  { 0x27F0,	0x27FF,	eSupplementalArrowsA },
  { 0x2800,	0x28FF,	eBraillePatterns },
  { 0x2900,	0x297F,	eSupplementalArrowsB },
  { 0x2980,	0x29FF,	eMiscellaneousMathematicalSymbolsB },
  { 0x2A00,	0x2AFF,	eSupplementalMathematicalOperators },
  { 0x2B00,	0x2BFF,	eMiscellaneousSymbolsAndArrows },
  { 0x2C00,	0x2E7F,	eUndefined },
  { 0x2E80,	0x2EFF,	eCJKRadicalsSupplement },
  { 0x2F00,	0x2FDF,	eKangxiRadicals },
  { 0x2FE0,	0x2FEF,	eUndefined },
  { 0x2FF0,	0x2FFF,	eIdeographicDescriptionCharacters },
  { 0x3000,	0x303F,	eCJKSymbolsAndPunctuation },
  { 0x3040,	0x309F,	eHiragana },
  { 0x30A0,	0x30FF,	eKatakana },
  { 0x3100,	0x312F,	eBopomofo },
  { 0x3130,	0x318F,	eHangulCompatibilityJamo },
  { 0x3190,	0x319F,	eKanbun },
  { 0x31A0,	0x31BF,	eBopomofoExtended },
  { 0x31C0,	0x31EF,	eUndefined },
  { 0x31F0,	0x31FF,	eKatakanaPhoneticExtensions },
  { 0x3200,	0x32FF,	eEnclosedCJKLettersAndMonths },
  { 0x3300,	0x33FF,	eCJKCompatibility },
  { 0x3400,	0x4DBF,	eCJKUnifiedIdeographsExtensionA },
  { 0x4DC0,	0x4DFF,	eYijingHexagramSymbols },
  { 0x4E00,	0x9FAF,	eCJKUnifiedIdeographs },
  { 0x9FB0,	0x9FFF,	eUndefined },
  { 0xA000,	0xA48F,	eYiSyllables },
  { 0xA490,	0xA4CF,	eYiRadicals },
  { 0xA4D0,	0xABFF,	eUndefined },
  { 0xAC00,	0xD7AF,	eHangulSyllables },
  { 0xD7B0,	0xD7FF,	eUndefined },
  { 0xD800,	0xDBFF,	eHighSurrogateArea },
  { 0xDC00,	0xDFFF,	eLowSurrogateArea },
  { 0xE000,	0xF8FF,	ePrivateUseArea },
  { 0xF900,	0xFAFF,	eCJKCompatibilityIdeographs },
  { 0xFB00,	0xFB4F,	eAlphabeticPresentationForms },
  { 0xFB50,	0xFDFF,	eArabicPresentationFormsA },
  { 0xFE00,	0xFE0F,	eVariationSelectors },
  { 0xFE10,	0xFE1F,	eUndefined },
  { 0xFE20,	0xFE2F,	eCombiningHalfMarks },
  { 0xFE30,	0xFE4F,	eCJKCompatibilityForms },
  { 0xFE50,	0xFE6F,	eSmallFormVariants },
  { 0xFE70,	0xFEFF,	eArabicPresentationFormsB },
  { 0xFF00,	0xFFEF,	eHalfwidthAndFullwidthForms },
  { 0xFFF0,	0xFFFF,	eSpecials },
  { 0x10000,	0x1007F,	eLinearBSyllabary },
  { 0x10080,	0x100FF,	eLinearBIdeograms },
  { 0x10100,	0x1013F,	eAegeanNumbers },
  { 0x10140,	0x102FF,	eUndefined },
  { 0x10300,	0x1032F,	eOldItalic },
  { 0x10330,	0x1034F,	eGothic },
  { 0x10380,	0x1039F,	eUgaritic },
  { 0x10400,	0x1044F,	eDeseret },
  { 0x10450,	0x1047F,	eShavian },
  { 0x10480,	0x104AF,	eOsmanya },
  { 0x104B0,	0x107FF,	eUndefined },
  { 0x10800,	0x1083F,	eCypriotSyllabary },
  { 0x10840,	0x1CFFF,	eUndefined },
  { 0x1D000,	0x1D0FF,	eByzantineMusicalSymbols },
  { 0x1D100,	0x1D1FF,	eMusicalSymbols },
  { 0x1D200,	0x1D2FF,	eUndefined },
  { 0x1D300,	0x1D35F,	eTaiXuanJingSymbols },
  { 0x1D360,	0x1D3FF,	eUndefined },
  { 0x1D400,	0x1D7FF,	eMathematicalAlphanumericSymbols },
  { 0x1D800,	0x1FFFF,	eUndefined },
  { 0x20000,	0x2A6DF,	eCJKUnifiedIdeographsExtensionB },
  { 0x2A6E0,	0x2F7FF,	eUndefined },
  { 0x2F800,	0x2FA1F,	eCJKCompatibilityIdeographsSupplement },
  { 0x2FAB0,	0xDFFFF,	eUnused },
  { 0xE0000,	0xE007F,	eTags },
  { 0xE0080,	0xE00FF,	eUnused },
  { 0xE0100,	0xE01EF,	eVariationSelectorsSupplement },
  { 0xE01F0,	0xEFFFF,	eUnused },
  { 0xF0000,	0xFFFFD,	eSupplementaryPrivateUseAreaA },
  { 0xFFFFE,	0xFFFFF,	eUnused },
  { 0x100000,	0x10FFFD,	eSupplementaryPrivateUseAreaB }
}; 


nuiUnicodeRange nuiGetUnicodeRange(nglChar ch)
{
  nglChar l, h;
  return nuiGetUnicodeRange(ch, l, h);
}

nuiUnicodeRange nuiGetUnicodeRange(nglChar ch, nglChar& rLow, nglChar& rHigh)
{
  const uint32 count = sizeof(nuiUnicodeRanges) / sizeof(nuiUnicodeRangeDesc);
  for (uint32 i = 0; i < count; i++)
  {
    if (nuiUnicodeRanges[i].RangeStart <= ch && nuiUnicodeRanges[i].RangeEnd >= ch)
    {
      rLow = nuiUnicodeRanges[i].RangeStart;
      rHigh = nuiUnicodeRanges[i].RangeEnd;
      return nuiUnicodeRanges[i].Range;      
    }
  }
  
  return eNone;
}

#define NAME(X) case e##X: return _T(#X); break;
nglString nuiGetUnicodeRangeName(nuiUnicodeRange range)
{
  switch (range)
  {
  NAME(None);
  NAME(Undefined);
  NAME(Unused);
  NAME(BasicLatin);
  NAME(C1ControlsAndLatin1Supplement);
  NAME(LatinExtendedA);
  NAME(LatinExtendedB);
  NAME(IPAExtensions);
  NAME(SpacingModifierLetters);
  NAME(CombiningDiacriticalMarks);
  NAME(GreekAndCoptic);
  NAME(Cyrillic);
  NAME(CyrillicSupplement);
  NAME(Armenian);
  NAME(Hebrew);
  NAME(Arabic);
  NAME(Syriac);
  NAME(Thaana);
  NAME(Devanagari);
  NAME(BengaliAndAssamese);
  NAME(Gurmukhi);
  NAME(Gujarati);
  NAME(Oriya);
  NAME(Tamil);
  NAME(Telugu);
  NAME(Kannada);
  NAME(Malayalam);
  NAME(Sinhala);
  NAME(Thai);
  NAME(Lao);
  NAME(Tibetan);
  NAME(Myanmar);
  NAME(Georgian);
  NAME(HangulJamo);
  NAME(Ethiopic);
  NAME(Cherokee);
  NAME(UnifiedCanadianAboriginalSyllabics);
  NAME(Ogham);
  NAME(Runic);
  NAME(Tagalog);
  NAME(Hanunoo);
  NAME(Buhid);
  NAME(Tagbanwa);
  NAME(Khmer);
  NAME(Mongolian);
  NAME(Limbu);
  NAME(TaiLe);
  NAME(KhmerSymbols);
  NAME(PhoneticExtensions);
  NAME(LatinExtendedAdditional);
  NAME(GreekExtended);
  NAME(GeneralPunctuation);
  NAME(SuperscriptsAndSubscripts);
  NAME(CurrencySymbols);
  NAME(CombiningDiacriticalMarksForSymbols);
  NAME(LetterlikeSymbols);
  NAME(NumberForms);
  NAME(Arrows);
  NAME(MathematicalOperators);
  NAME(MiscellaneousTechnical);
  NAME(ControlPictures);
  NAME(OpticalCharacterRecognition);
  NAME(EnclosedAlphanumerics);
  NAME(BoxDrawing);
  NAME(BlockElements);
  NAME(GeometricShapes);
  NAME(MiscellaneousSymbols);
  NAME(Dingbats);
  NAME(MiscellaneousMathematicalSymbolsA);
  NAME(SupplementalArrowsA);
  NAME(BraillePatterns);
  NAME(SupplementalArrowsB);
  NAME(MiscellaneousMathematicalSymbolsB);
  NAME(SupplementalMathematicalOperators);
  NAME(MiscellaneousSymbolsAndArrows);
  NAME(CJKRadicalsSupplement);
  NAME(KangxiRadicals);
  NAME(IdeographicDescriptionCharacters);
  NAME(CJKSymbolsAndPunctuation);
  NAME(Hiragana);
  NAME(Katakana);
  NAME(Bopomofo);
  NAME(HangulCompatibilityJamo);
  NAME(Kanbun);
  NAME(BopomofoExtended);
  NAME(KatakanaPhoneticExtensions);
  NAME(EnclosedCJKLettersAndMonths);
  NAME(CJKCompatibility);
  NAME(CJKUnifiedIdeographsExtensionA);
  NAME(YijingHexagramSymbols);
  NAME(CJKUnifiedIdeographs);
  NAME(YiSyllables);
  NAME(YiRadicals);
  NAME(HangulSyllables);
  NAME(HighSurrogateArea);
  NAME(LowSurrogateArea);
  NAME(PrivateUseArea);
  NAME(CJKCompatibilityIdeographs);
  NAME(AlphabeticPresentationForms);
  NAME(ArabicPresentationFormsA);
  NAME(VariationSelectors);
  NAME(CombiningHalfMarks);
  NAME(CJKCompatibilityForms);
  NAME(SmallFormVariants);
  NAME(ArabicPresentationFormsB);
  NAME(HalfwidthAndFullwidthForms);
  NAME(Specials);
  NAME(LinearBSyllabary);
  NAME(LinearBIdeograms);
  NAME(AegeanNumbers);
  NAME(OldItalic);
  NAME(Gothic);
  NAME(Ugaritic);
  NAME(Deseret);
  NAME(Shavian);
  NAME(Osmanya);
  NAME(CypriotSyllabary);
  NAME(ByzantineMusicalSymbols);
  NAME(MusicalSymbols);
  NAME(TaiXuanJingSymbols);
  NAME(MathematicalAlphanumericSymbols);
  NAME(CJKUnifiedIdeographsExtensionB);
  NAME(CJKCompatibilityIdeographsSupplement);
  NAME(Tags);
  NAME(VariationSelectorsSupplement);
  NAME(SupplementaryPrivateUseAreaA);
  NAME(SupplementaryPrivateUseAreaB);
  }
  
  NGL_ASSERT(0);
  return nglString::Null;
}

nuiUnicodeDirection nuiGetUnicodeDirection(nglChar ch)
{
  //#FIXME need to add BiDi algo here
  return eLeftToRight;
}

bool nuiIsUnicodeBlank(nglChar ch)
{
  //#FIXME add the actual unicode blank detection algo here
  return ch < 32;
}

nuiTextRange::nuiTextRange()
{
  mLength = 0; // count of unicode code points
  mDirection = 0; // even: Left to right, odd: right to left
  mScript = eNone; // What script if this range of text
  mBlank = false; // Does this range contains strictly blank (space, tab, return, etc.) code points.
}

bool nuiSplitText(const nglString& rSourceString, nuiTextRangeList& rRanges, nuiSplitTextFlag flags)
{
  uint32 size = rSourceString.GetLength();

  rRanges.clear();
  if (!size)
    return true;

  const bool scriptchange = flags & nuiST_ScriptChange;
  const bool wordboundary = flags & nuiST_WordBoundary;
  const bool directionchange = flags & nuiST_DirectionChange;
  uint32 lastpos = 0;
  uint32 curpos = 0;
  const nglChar& ch = rSourceString[curpos];
  int32 direction = nuiGetUnicodeDirection(ch);
  int32 newdirection = direction;
  nglChar low = 0;
  nglChar hi = 0;
  nuiUnicodeRange range = nuiGetUnicodeRange(ch, low, hi);
  nuiUnicodeRange newrange = range;
  bool blank = nuiIsUnicodeBlank(ch);
  bool newblank = blank;
  curpos++;
  
  while (curpos != size)
  {
    bool brk = false;
    const nglChar& ch = rSourceString[curpos];

    if (wordboundary)
    {
      newblank = nuiIsUnicodeBlank(ch);
      if (newblank != blank)
        brk = true;
    }
    
    if (scriptchange)
    {
      if (ch < low || ch > hi) // still in the last range?
      {
        if (!wordboundary)
          newblank = nuiIsUnicodeBlank(ch);
        if (!newblank)
        {
          newrange = nuiGetUnicodeRange(ch, low, hi);
          if (newrange != range)
            brk = true;
        }
      }
    }
    
    if (directionchange)
    {
      newdirection = nuiGetUnicodeDirection(ch);
      if (newdirection != direction)
        brk = true;
    }
    
    if (brk)
    {
      nuiTextRange r;
      r.mLength = curpos - lastpos; // count of unicode code points
      r.mDirection = direction; // even: Left to right, odd: right to left
      r.mScript = range; // What script if this range of text
      r.mBlank = blank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
      
      rRanges.push_back(r);
      
      lastpos = curpos;
      direction = newdirection;
      range = newrange;
      blank = newblank;
    }
    
    curpos++;
  }

  // Last range:
  nuiTextRange r;
  r.mLength = curpos - lastpos; // count of unicode code points
  r.mDirection = direction; // even: Left to right, odd: right to left
  r.mScript = range; // What script if this range of text
  r.mBlank = blank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
  
  rRanges.push_back(r);
  
  
  return true;
}

class nuiScriptRange
{
public:
  nuiUnicodeScript mScript;
  int32 mLow;
  int32 mHigh;
};

nuiScriptRange nuiScriptRanges[] =
{
{ eScriptCommon, 0, 64 },
{ eScriptLatin, 65, 90 },
{ eScriptCommon, 91, 96 },
{ eScriptLatin, 97, 122 },
{ eScriptCommon, 123, 169 },
{ eScriptLatin, 170, 170 },
{ eScriptCommon, 171, 185 },
{ eScriptLatin, 186, 186 },
{ eScriptCommon, 187, 191 },
{ eScriptLatin, 192, 214 },
{ eScriptCommon, 215, 215 },
{ eScriptLatin, 216, 246 },
{ eScriptCommon, 247, 247 },
{ eScriptLatin, 248, 696 },
{ eScriptCommon, 697, 735 },
{ eScriptLatin, 736, 740 },
{ eScriptCommon, 741, 767 },
{ eScriptInherited, 768, 879 },
{ eScriptGreek, 880, 883 },
{ eScriptCommon, 884, 884 },
{ eScriptGreek, 885, 893 },
{ eScriptCommon, 894, 894 },
{ eScriptGreek, 900, 900 },
{ eScriptCommon, 901, 901 },
{ eScriptGreek, 902, 902 },
{ eScriptCommon, 903, 903 },
{ eScriptGreek, 904, 993 },
{ eScriptCoptic, 994, 1007 },
{ eScriptGreek, 1008, 1023 },
{ eScriptCyrillic, 1024, 1315 },
{ eScriptArmenian, 1329, 1415 },
{ eScriptCommon, 1417, 1417 },
{ eScriptArmenian, 1418, 1418 },
{ eScriptHebrew, 1425, 1524 },
{ eScriptCommon, 1536, 1539 },
{ eScriptArabic, 1542, 1547 },
{ eScriptCommon, 1548, 1548 },
{ eScriptArabic, 1549, 1562 },
{ eScriptCommon, 1563, 1563 },
{ eScriptArabic, 1566, 1566 },
{ eScriptCommon, 1567, 1567 },
{ eScriptArabic, 1569, 1599 },
{ eScriptCommon, 1600, 1600 },
{ eScriptArabic, 1601, 1610 },
{ eScriptInherited, 1611, 1621 },
{ eScriptArabic, 1622, 1630 },
{ eScriptCommon, 1632, 1641 },
{ eScriptArabic, 1642, 1647 },
{ eScriptInherited, 1648, 1648 },
{ eScriptArabic, 1649, 1756 },
{ eScriptCommon, 1757, 1757 },
{ eScriptArabic, 1758, 1791 },
{ eScriptSyriac, 1792, 1871 },
{ eScriptArabic, 1872, 1919 },
{ eScriptThaana, 1920, 1969 },
{ eScriptNko, 1984, 2042 },
{ eScriptDevanagari, 2305, 2384 },
{ eScriptInherited, 2385, 2386 },
{ eScriptDevanagari, 2387, 2403 },
{ eScriptCommon, 2404, 2405 },
{ eScriptDevanagari, 2406, 2415 },
{ eScriptCommon, 2416, 2416 },
{ eScriptDevanagari, 2417, 2431 },
{ eScriptBengali, 2433, 2554 },
{ eScriptGurmukhi, 2561, 2677 },
{ eScriptGujarati, 2689, 2801 },
{ eScriptOriya, 2817, 2929 },
{ eScriptTamil, 2946, 3066 },
{ eScriptTelugu, 3073, 3199 },
{ eScriptKannada, 3202, 3311 },
{ eScriptCommon, 3313, 3314 },
{ eScriptMalayalam, 3330, 3455 },
{ eScriptSinhala, 3458, 3572 },
{ eScriptThai, 3585, 3642 },
{ eScriptCommon, 3647, 3647 },
{ eScriptThai, 3648, 3675 },
{ eScriptLao, 3713, 3805 },
{ eScriptTibetan, 3840, 4052 },
{ eScriptMyanmar, 4096, 4255 },
{ eScriptGeorgian, 4256, 4346 },
{ eScriptCommon, 4347, 4347 },
{ eScriptGeorgian, 4348, 4348 },
{ eScriptHangul, 4352, 4601 },
{ eScriptEthiopic, 4608, 5017 },
{ eScriptCherokee, 5024, 5108 },
{ eScriptCanadian_Aboriginal, 5121, 5750 },
{ eScriptOgham, 5760, 5788 },
{ eScriptRunic, 5792, 5866 },
{ eScriptCommon, 5867, 5869 },
{ eScriptRunic, 5870, 5872 },
{ eScriptTagalog, 5888, 5908 },
{ eScriptHanunoo, 5920, 5940 },
{ eScriptCommon, 5941, 5942 },
{ eScriptBuhid, 5952, 5971 },
{ eScriptTagbanwa, 5984, 6003 },
{ eScriptKhmer, 6016, 6137 },
{ eScriptMongolian, 6144, 6145 },
{ eScriptCommon, 6146, 6147 },
{ eScriptMongolian, 6148, 6148 },
{ eScriptCommon, 6149, 6149 },
{ eScriptMongolian, 6150, 6314 },
{ eScriptLimbu, 6400, 6479 },
{ eScriptTai_Le, 6480, 6516 },
{ eScriptNew_Tai_Lue, 6528, 6623 },
{ eScriptKhmer, 6624, 6655 },
{ eScriptBuginese, 6656, 6687 },
{ eScriptBalinese, 6912, 7036 },
{ eScriptSundanese, 7040, 7097 },
{ eScriptLepcha, 7168, 7247 },
{ eScriptOl_Chiki, 7248, 7295 },
{ eScriptLatin, 7424, 7461 },
{ eScriptGreek, 7462, 7466 },
{ eScriptCyrillic, 7467, 7467 },
{ eScriptLatin, 7468, 7516 },
{ eScriptGreek, 7517, 7521 },
{ eScriptLatin, 7522, 7525 },
{ eScriptGreek, 7526, 7530 },
{ eScriptLatin, 7531, 7543 },
{ eScriptCyrillic, 7544, 7544 },
{ eScriptLatin, 7545, 7614 },
{ eScriptGreek, 7615, 7615 },
{ eScriptInherited, 7616, 7679 },
{ eScriptLatin, 7680, 7935 },
{ eScriptGreek, 7936, 8190 },
{ eScriptCommon, 8192, 8203 },
{ eScriptInherited, 8204, 8205 },
{ eScriptCommon, 8206, 8304 },
{ eScriptLatin, 8305, 8305 },
{ eScriptCommon, 8308, 8318 },
{ eScriptLatin, 8319, 8319 },
{ eScriptCommon, 8320, 8334 },
{ eScriptLatin, 8336, 8340 },
{ eScriptCommon, 8352, 8373 },
{ eScriptInherited, 8400, 8432 },
{ eScriptCommon, 8448, 8485 },
{ eScriptGreek, 8486, 8486 },
{ eScriptCommon, 8487, 8489 },
{ eScriptLatin, 8490, 8491 },
{ eScriptCommon, 8492, 8497 },
{ eScriptLatin, 8498, 8498 },
{ eScriptCommon, 8499, 8525 },
{ eScriptLatin, 8526, 8526 },
{ eScriptCommon, 8527, 8543 },
{ eScriptLatin, 8544, 8584 },
{ eScriptCommon, 8592, 10239 },
{ eScriptBraille, 10240, 10495 },
{ eScriptCommon, 10496, 11092 },
{ eScriptGlagolitic, 11264, 11358 },
{ eScriptLatin, 11360, 11389 },
{ eScriptCoptic, 11392, 11519 },
{ eScriptGeorgian, 11520, 11557 },
{ eScriptTifinagh, 11568, 11631 },
{ eScriptEthiopic, 11648, 11742 },
{ eScriptCyrillic, 11744, 11775 },
{ eScriptCommon, 11776, 11824 },
{ eScriptHan, 11904, 12245 },
{ eScriptCommon, 12272, 12292 },
{ eScriptHan, 12293, 12293 },
{ eScriptCommon, 12294, 12294 },
{ eScriptHan, 12295, 12295 },
{ eScriptCommon, 12296, 12320 },
{ eScriptHan, 12321, 12329 },
{ eScriptInherited, 12330, 12335 },
{ eScriptCommon, 12336, 12343 },
{ eScriptHan, 12344, 12347 },
{ eScriptCommon, 12348, 12351 },
{ eScriptHiragana, 12353, 12438 },
{ eScriptInherited, 12441, 12442 },
{ eScriptCommon, 12443, 12444 },
{ eScriptHiragana, 12445, 12447 },
{ eScriptCommon, 12448, 12448 },
{ eScriptKatakana, 12449, 12538 },
{ eScriptCommon, 12539, 12540 },
{ eScriptKatakana, 12541, 12543 },
{ eScriptBopomofo, 12549, 12589 },
{ eScriptHangul, 12593, 12686 },
{ eScriptCommon, 12688, 12703 },
{ eScriptBopomofo, 12704, 12727 },
{ eScriptCommon, 12736, 12771 },
{ eScriptKatakana, 12784, 12799 },
{ eScriptHangul, 12800, 12830 },
{ eScriptCommon, 12832, 12895 },
{ eScriptHangul, 12896, 12926 },
{ eScriptCommon, 12927, 13007 },
{ eScriptKatakana, 13008, 13143 },
{ eScriptCommon, 13144, 13311 },
{ eScriptHan, 13312, 19893 },
{ eScriptCommon, 19904, 19967 },
{ eScriptHan, 19968, 40899 },
{ eScriptYi, 40960, 42182 },
{ eScriptVai, 42240, 42539 },
{ eScriptCyrillic, 42560, 42647 },
{ eScriptCommon, 42752, 42785 },
{ eScriptLatin, 42786, 42887 },
{ eScriptCommon, 42888, 42890 },
{ eScriptLatin, 42891, 43007 },
{ eScriptSyloti_Nagri, 43008, 43051 },
{ eScriptPhags_Pa, 43072, 43127 },
{ eScriptSaurashtra, 43136, 43225 },
{ eScriptKayah_Li, 43264, 43311 },
{ eScriptRejang, 43312, 43359 },
{ eScriptCham, 43520, 43615 },
{ eScriptHangul, 44032, 55203 },
{ eScriptHan, 63744, 64217 },
{ eScriptLatin, 64256, 64262 },
{ eScriptArmenian, 64275, 64279 },
{ eScriptHebrew, 64285, 64335 },
{ eScriptArabic, 64336, 64829 },
{ eScriptCommon, 64830, 64831 },
{ eScriptArabic, 64848, 65020 },
{ eScriptCommon, 65021, 65021 },
{ eScriptInherited, 65024, 65039 },
{ eScriptCommon, 65040, 65049 },
{ eScriptInherited, 65056, 65062 },
{ eScriptCommon, 65072, 65131 },
{ eScriptArabic, 65136, 65276 },
{ eScriptCommon, 65279, 65312 },
{ eScriptLatin, 65313, 65338 },
{ eScriptCommon, 65339, 65344 },
{ eScriptLatin, 65345, 65370 },
{ eScriptCommon, 65371, 65381 },
{ eScriptKatakana, 65382, 65391 },
{ eScriptCommon, 65392, 65392 },
{ eScriptKatakana, 65393, 65437 },
{ eScriptCommon, 65438, 65439 },
{ eScriptHangul, 65440, 65500 },
{ eScriptCommon, 65504, 65533 },
{ eScriptLinear_B, 65536, 65786 },
{ eScriptCommon, 65792, 65855 },
{ eScriptGreek, 65856, 65930 },
{ eScriptCommon, 65936, 66044 },
{ eScriptInherited, 66045, 66045 },
{ eScriptLycian, 66176, 66204 },
{ eScriptCarian, 66208, 66256 },
{ eScriptOld_Italic, 66304, 66339 },
{ eScriptGothic, 66352, 66378 },
{ eScriptUgaritic, 66432, 66463 },
{ eScriptOld_Persian, 66464, 66517 },
{ eScriptDeseret, 66560, 66639 },
{ eScriptShavian, 66640, 66687 },
{ eScriptOsmanya, 66688, 66729 },
{ eScriptCypriot, 67584, 67647 },
{ eScriptPhoenician, 67840, 67871 },
{ eScriptLydian, 67872, 67903 },
{ eScriptKharoshthi, 68096, 68184 },
{ eScriptCuneiform, 73728, 74867 },
{ eScriptCommon, 118784, 119142 },
{ eScriptInherited, 119143, 119145 },
{ eScriptCommon, 119146, 119162 },
{ eScriptInherited, 119163, 119170 },
{ eScriptCommon, 119171, 119172 },
{ eScriptInherited, 119173, 119179 },
{ eScriptCommon, 119180, 119209 },
{ eScriptInherited, 119210, 119213 },
{ eScriptCommon, 119214, 119261 },
{ eScriptGreek, 119296, 119365 },
{ eScriptCommon, 119552, 127123 },
{ eScriptHan, 131072, 195101 },
{ eScriptCommon, 917505, 917631 }
};


nuiUnicodeScript nuiGetUnicodeScript(nglChar ch)
{
  int32 low, hi;
  return nuiGetUnicodeScript(ch, low, hi);
}

nuiUnicodeScript nuiGetUnicodeScript(nglChar ch, int32& rLow, int32& rHigh)
{
  const int32 count = sizeof(nuiScriptRanges) / sizeof(nuiScriptRange);
  
  // Slow iterative search for now...
  for (int32 i = 0; i < count; i++)
  {
    if (ch >= nuiScriptRanges[i].mLow && ch <= nuiScriptRanges[i].mHigh)
    {
      rLow = nuiScriptRanges[i].mLow;
      rHigh = nuiScriptRanges[i].mHigh;
      return nuiScriptRanges[i].mScript;
    }
  }

  rLow = -1;
  rHigh = -1;
  return eScriptCommon;
}

