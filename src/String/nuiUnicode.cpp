/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiUnicode.h"

typedef struct nuiUnicodeRangeDesc
{
  uint32 RangeStart;
  uint32 RangeEnd;
  nuiUnicodeRange Range;
};

nuiUnicodeRangeDesc nuiUnicodeRanges[] =
{// Start  End   , Enum                                   
  { 0x0000, 0x007F, eRangeBasicLatin },
  { 0x0080, 0x00FF, eRangeC1ControlsAndLatin1Supplement },
  { 0x0100, 0x017F, eRangeLatinExtendedA },
  { 0x0180, 0x024F, eRangeLatinExtendedB },
  { 0x0250, 0x02AF, eRangeIPAExtensions },
  { 0x02B0, 0x02FF, eRangeSpacingModifierLetters },
  { 0x0300, 0x036F, eRangeCombiningDiacriticalMarks },
  { 0x0370, 0x03FF, eRangeGreekAndCoptic },
  { 0x0400, 0x04FF, eRangeCyrillic },
  { 0x0500, 0x052F, eRangeCyrillicSupplement },
  { 0x0530, 0x058F, eRangeArmenian },
  { 0x0590, 0x05FF, eRangeHebrew },
  { 0x0600, 0x06FF, eRangeArabic },
  { 0x0700, 0x074F, eRangeSyriac },
  { 0x0750, 0x077F, eRangeUndefined },
  { 0x0780, 0x07BF, eRangeThaana },
  { 0x07C0, 0x08FF, eRangeUndefined },
  { 0x0900, 0x097F, eRangeDevanagari },
  { 0x0980, 0x09FF, eRangeBengaliAndAssamese },
  { 0x0A00, 0x0A7F, eRangeGurmukhi },
  { 0x0A80, 0x0AFF, eRangeGujarati },
  { 0x0B00, 0x0B7F, eRangeOriya },
  { 0x0B80, 0x0BFF, eRangeTamil },
  { 0x0C00, 0x0C7F, eRangeTelugu },
  { 0x0C80, 0x0CFF, eRangeKannada },
  { 0x0D00, 0x0DFF, eRangeMalayalam },
  { 0x0D80, 0x0DFF, eRangeSinhala },
  { 0x0E00, 0x0E7F, eRangeThai },
  { 0x0E80, 0x0EFF, eRangeLao },
  { 0x0F00, 0x0FFF, eRangeTibetan },
  { 0x1000, 0x109F, eRangeMyanmar },
  { 0x10A0, 0x10FF, eRangeGeorgian },
  { 0x1100, 0x11FF, eRangeHangulJamo },
  { 0x1200, 0x137F, eRangeEthiopic },
  { 0x1380, 0x139F, eRangeUndefined },
  { 0x13A0, 0x13FF, eRangeCherokee },
  { 0x1400, 0x167F, eRangeUnifiedCanadianAboriginalSyllabics },
  { 0x1680, 0x169F, eRangeOgham },
  { 0x16A0, 0x16FF, eRangeRunic },
  { 0x1700, 0x171F, eRangeTagalog },
  { 0x1720, 0x173F, eRangeHanunoo },
  { 0x1740, 0x175F, eRangeBuhid },
  { 0x1760, 0x177F, eRangeTagbanwa },
  { 0x1780, 0x17FF, eRangeKhmer },
  { 0x1800, 0x18AF, eRangeMongolian },
  { 0x18B0, 0x18FF, eRangeUndefined },
  { 0x1900, 0x194F, eRangeLimbu },
  { 0x1950, 0x197F, eRangeTaiLe },
  { 0x1980, 0x19DF, eRangeUndefined },
  { 0x19E0, 0x19FF, eRangeKhmerSymbols },
  { 0x1A00, 0x1CFF, eRangeUndefined },
  { 0x1D00, 0x1D7F, eRangePhoneticExtensions },
  { 0x1D80, 0x1DFF, eRangeUndefined },
  { 0x1E00, 0x1EFF, eRangeLatinExtendedAdditional },
  { 0x1F00, 0x1FFF, eRangeGreekExtended },
  { 0x2000, 0x206F, eRangeGeneralPunctuation },
  { 0x2070, 0x209F, eRangeSuperscriptsAndSubscripts },
  { 0x20A0, 0x20CF, eRangeCurrencySymbols },
  { 0x20D0, 0x20FF, eRangeCombiningDiacriticalMarksForSymbols },
  { 0x2100, 0x214F, eRangeLetterlikeSymbols },
  { 0x2150, 0x218F, eRangeNumberForms },
  { 0x2190, 0x21FF, eRangeArrows },
  { 0x2200, 0x22FF, eRangeMathematicalOperators },
  { 0x2300, 0x23FF, eRangeMiscellaneousTechnical },
  { 0x2400, 0x243F, eRangeControlPictures },
  { 0x2440, 0x245F, eRangeOpticalCharacterRecognition },
  { 0x2460, 0x24FF, eRangeEnclosedAlphanumerics },
  { 0x2500, 0x257F, eRangeBoxDrawing },
  { 0x2580, 0x259F, eRangeBlockElements },
  { 0x25A0, 0x25FF, eRangeGeometricShapes },
  { 0x2600, 0x26FF, eRangeMiscellaneousSymbols },
  { 0x2700, 0x27BF, eRangeDingbats },
  { 0x27C0, 0x27EF, eRangeMiscellaneousMathematicalSymbolsA },
  { 0x27F0, 0x27FF, eRangeSupplementalArrowsA },
  { 0x2800, 0x28FF, eRangeBraillePatterns },
  { 0x2900, 0x297F, eRangeSupplementalArrowsB },
  { 0x2980, 0x29FF, eRangeMiscellaneousMathematicalSymbolsB },
  { 0x2A00, 0x2AFF, eRangeSupplementalMathematicalOperators },
  { 0x2B00, 0x2BFF, eRangeMiscellaneousSymbolsAndArrows },
  { 0x2C00, 0x2E7F, eRangeUndefined },
  { 0x2E80, 0x2EFF, eRangeCJKRadicalsSupplement },
  { 0x2F00, 0x2FDF, eRangeKangxiRadicals },
  { 0x2FE0, 0x2FEF, eRangeUndefined },
  { 0x2FF0, 0x2FFF, eRangeIdeographicDescriptionCharacters },
  { 0x3000, 0x303F, eRangeCJKSymbolsAndPunctuation },
  { 0x3040, 0x309F, eRangeHiragana },
  { 0x30A0, 0x30FF, eRangeKatakana },
  { 0x3100, 0x312F, eRangeBopomofo },
  { 0x3130, 0x318F, eRangeHangulCompatibilityJamo },
  { 0x3190, 0x319F, eRangeKanbun },
  { 0x31A0, 0x31BF, eRangeBopomofoExtended },
  { 0x31C0, 0x31EF, eRangeUndefined },
  { 0x31F0, 0x31FF, eRangeKatakanaPhoneticExtensions },
  { 0x3200, 0x32FF, eRangeEnclosedCJKLettersAndMonths },
  { 0x3300, 0x33FF, eRangeCJKCompatibility },
  { 0x3400, 0x4DBF, eRangeCJKUnifiedIdeographsExtensionA },
  { 0x4DC0, 0x4DFF, eRangeYijingHexagramSymbols },
  { 0x4E00, 0x9FAF, eRangeCJKUnifiedIdeographs },
  { 0x9FB0, 0x9FFF, eRangeUndefined },
  { 0xA000, 0xA48F, eRangeYiSyllables },
  { 0xA490, 0xA4CF, eRangeYiRadicals },
  { 0xA4D0, 0xABFF, eRangeUndefined },
  { 0xAC00, 0xD7AF, eRangeHangulSyllables },
  { 0xD7B0, 0xD7FF, eRangeUndefined },
  { 0xD800, 0xDBFF, eRangeHighSurrogateArea },
  { 0xDC00, 0xDFFF, eRangeLowSurrogateArea },
  { 0xE000, 0xF8FF, eRangePrivateUseArea },
  { 0xF900, 0xFAFF, eRangeCJKCompatibilityIdeographs },
  { 0xFB00, 0xFB4F, eRangeAlphabeticPresentationForms },
  { 0xFB50, 0xFDFF, eRangeArabicPresentationFormsA },
  { 0xFE00, 0xFE0F, eRangeVariationSelectors },
  { 0xFE10, 0xFE1F, eRangeUndefined },
  { 0xFE20, 0xFE2F, eRangeCombiningHalfMarks },
  { 0xFE30, 0xFE4F, eRangeCJKCompatibilityForms },
  { 0xFE50, 0xFE6F, eRangeSmallFormVariants },
  { 0xFE70, 0xFEFF, eRangeArabicPresentationFormsB },
  { 0xFF00, 0xFFEF, eRangeHalfwidthAndFullwidthForms },
  { 0xFFF0, 0xFFFF, eRangeSpecials },
  { 0x10000, 0x1007F, eRangeLinearBSyllabary },
  { 0x10080, 0x100FF, eRangeLinearBIdeograms },
  { 0x10100, 0x1013F, eRangeAegeanNumbers },
  { 0x10140, 0x102FF, eRangeUndefined },
  { 0x10300, 0x1032F, eRangeOldItalic },
  { 0x10330, 0x1034F, eRangeGothic },
  { 0x10380, 0x1039F, eRangeUgaritic },
  { 0x10400, 0x1044F, eRangeDeseret },
  { 0x10450, 0x1047F, eRangeShavian },
  { 0x10480, 0x104AF, eRangeOsmanya },
  { 0x104B0, 0x107FF, eRangeUndefined },
  { 0x10800, 0x1083F, eRangeCypriotSyllabary },
  { 0x10840, 0x1CFFF, eRangeUndefined },
  { 0x1D000, 0x1D0FF, eRangeByzantineMusicalSymbols },
  { 0x1D100, 0x1D1FF, eRangeMusicalSymbols },
  { 0x1D200, 0x1D2FF, eRangeUndefined },
  { 0x1D300, 0x1D35F, eRangeTaiXuanJingSymbols },
  { 0x1D360, 0x1D3FF, eRangeUndefined },
  { 0x1D400, 0x1D7FF, eRangeMathematicalAlphanumericSymbols },
  { 0x1D800, 0x1FFFF, eRangeUndefined },
  { 0x20000, 0x2A6DF, eRangeCJKUnifiedIdeographsExtensionB },
  { 0x2A6E0, 0x2F7FF, eRangeUndefined },
  { 0x2F800, 0x2FA1F, eRangeCJKCompatibilityIdeographsSupplement },
  { 0x2FAB0, 0xDFFFF, eRangeUnused },
  { 0xE0000, 0xE007F, eRangeTags },
  { 0xE0080, 0xE00FF, eRangeUnused },
  { 0xE0100, 0xE01EF, eRangeVariationSelectorsSupplement },
  { 0xE01F0, 0xEFFFF, eRangeUnused },
  { 0xF0000, 0xFFFFD, eRangeSupplementaryPrivateUseAreaA },
  { 0xFFFFE, 0xFFFFF, eRangeUnused },
  { 0x100000, 0x10FFFD, eRangeSupplementaryPrivateUseAreaB }
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
  
  return eRangeNone;
}

#define NAME(X) case eRange##X: return _T(#X); break;
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
#undef NAME

nuiUnicodeDirection nuiGetUnicodeDirection(nglChar ch)
{
  //#FIXME need to add BiDi algo here
  return eLeftToRight;
}

bool nuiIsUnicodeBlank(nglChar ch)
{
  //#FIXME add the actual unicode blank detection algo here
  return ch <= 32;
}

nuiTextRange::nuiTextRange()
{
  mLength = 0;
  mDirection = 0;
  mScript = eScriptCommon;
  mRange = eRangeNone;
  mBlank = false;
}

bool nuiSplitText(const nglString& rSourceString, nuiTextRangeList& rRanges, nuiSplitTextFlag flags)
{
  uint32 size = rSourceString.GetLength();

  rRanges.clear();
  if (!size)
    return true;

  const bool scriptchange = flags & nuiST_ScriptChange;
  const bool rangechange = flags & nuiST_RangeChange;
  const bool wordboundary = flags & nuiST_WordBoundary;
  const bool directionchange = flags & nuiST_DirectionChange;
  const bool mergecommonscript = flags & nuiST_MergeCommonScript;
  uint32 lastpos = 0;
  uint32 curpos = 0;
  const nglChar& ch = rSourceString[curpos];
  int32 direction = nuiGetUnicodeDirection(ch);
  int32 newdirection = direction;
  
  nglChar scriptlow = 0;
  nglChar scripthi = 0;
  nuiUnicodeScript script = nuiGetUnicodeScript(ch, scriptlow, scripthi);
  nuiUnicodeScript newscript = script;

  nglChar rangelow = 0;
  nglChar rangehi = 0;
  nuiUnicodeRange range = nuiGetUnicodeRange(ch, rangelow, rangehi);
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
      if (ch < scriptlow || ch > scripthi) // still in the last range?
      {
        if (!wordboundary)
          newblank = nuiIsUnicodeBlank(ch);
        if (!newblank)
        {
          newscript = nuiGetUnicodeScript(ch, scriptlow, scripthi);
          if ((newscript != script) && !(mergecommonscript && newscript == eScriptCommon))
          {
            brk = true;
          }
        }
      }
    }
    
    if (rangechange)
    {
      if (ch < rangelow || ch > rangehi) // still in the last range?
      {
        if (!wordboundary)
          newblank = nuiIsUnicodeBlank(ch);
        if (!newblank)
        {
          newrange = nuiGetUnicodeRange(ch, rangelow, rangehi);
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
      r.mScript = script; // What script if this range of text
      r.mRange = range; // What script if this range of text
      r.mBlank = blank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
      
      rRanges.push_back(r);
      
      lastpos = curpos;
      direction = newdirection;
      script = newscript;
      range = newrange;
      blank = newblank;
    }
    
    curpos++;
  }

  // Last range:
  nuiTextRange r;
  r.mLength = curpos - lastpos; // count of unicode code points
  r.mDirection = direction; // even: Left to right, odd: right to left
  r.mScript = script; // What script if this range of text
  r.mRange = range; // What script if this range of text
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
  nglChar low, hi;
  return nuiGetUnicodeScript(ch, low, hi);
}

nuiUnicodeScript nuiGetUnicodeScript(nglChar ch, nglChar& rLow, nglChar& rHigh)
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

#define NAME(X) case eScript##X: return _T(#X); break;
nglString nuiGetUnicodeScriptName(nuiUnicodeScript script)
{
  switch (script)
  {
    NAME(Common);
    NAME(Latin);
    NAME(Arabic);
    NAME(Armenian);
    NAME(Balinese);
    NAME(Bengali);
    NAME(Bopomofo);
    NAME(Braille);
    NAME(Buginese);
    NAME(Buhid);
    NAME(Canadian_Aboriginal);
    NAME(Carian);
    NAME(Cham);
    NAME(Cherokee);
    NAME(Coptic);
    NAME(Cuneiform);
    NAME(Cypriot);
    NAME(Cyrillic);
    NAME(Deseret);
    NAME(Devanagari);
    NAME(Ethiopic);
    NAME(Georgian);
    NAME(Glagolitic);
    NAME(Gothic);
    NAME(Greek);
    NAME(Gujarati);
    NAME(Gurmukhi);
    NAME(Han);
    NAME(Hangul);
    NAME(Hanunoo);
    NAME(Hebrew);
    NAME(Hiragana);
    NAME(Inherited);
    NAME(Kannada);
    NAME(Katakana);
    NAME(Kayah_Li);
    NAME(Kharoshthi);
    NAME(Khmer);
    NAME(Lao);
    NAME(Lepcha);
    NAME(Limbu);
    NAME(Linear_B);
    NAME(Lycian);
    NAME(Lydian);
    NAME(Malayalam);
    NAME(Mongolian);
    NAME(Myanmar);
    NAME(New_Tai_Lue);
    NAME(Nko);
    NAME(Ogham);
    NAME(Ol_Chiki);
    NAME(Old_Italic);
    NAME(Old_Persian);
    NAME(Oriya);
    NAME(Osmanya);
    NAME(Phags_Pa);
    NAME(Phoenician);
    NAME(Rejang);
    NAME(Runic);
    NAME(Saurashtra);
    NAME(Shavian);
    NAME(Sinhala);
    NAME(Sundanese);
    NAME(Syloti_Nagri);
    NAME(Syriac);
    NAME(Tagalog);
    NAME(Tagbanwa);
    NAME(Tai_Le);
    NAME(Tamil);
    NAME(Telugu);
    NAME(Thaana);
    NAME(Thai);
    NAME(Tibetan);
    NAME(Tifinagh);
    NAME(Ugaritic);
    NAME(Vai);
    NAME(Yi);
  }
  
  NGL_ASSERT(0);
  return nglString::Null;
}
#undef NAME

static std::vector<std::pair<nglString, nglTextEncoding> > encodings;

nglTextEncoding nuiGetTextEncodingFromString(const nglString& WebString)
{
  if (encodings.empty())
  {
    // ISO standards
    encodings.push_back(std::make_pair(_T("ISO_8859_1"), eISO8859_1));    ///< Latin1 (West European)
    encodings.push_back(std::make_pair(_T("ISO_8859_2"), eISO8859_2));  ///< Latin2 (East European)
    encodings.push_back(std::make_pair(_T("ISO_8859_3"), eISO8859_3));  ///< Latin3 (South European)
    encodings.push_back(std::make_pair(_T("ISO_8859_4"), eISO8859_4));  ///< Latin4 (North European)
    encodings.push_back(std::make_pair(_T("ISO_8859_5"), eISO8859_5));  ///< Cyrillic (see also KOI8)
    encodings.push_back(std::make_pair(_T("ISO_8859_6"), eISO8859_6));  ///< Arabic
    encodings.push_back(std::make_pair(_T("ISO_8859_7"), eISO8859_7));  ///< Greek
    encodings.push_back(std::make_pair(_T("ISO_8859_8"), eISO8859_8));  ///< Hebrew
    encodings.push_back(std::make_pair(_T("ISO_8859_9"), eISO8859_9));  ///< Latin5 (Turkish)
    encodings.push_back(std::make_pair(_T("ISO_8859_10"), eISO8859_10)); ///< Latin6 (Nordic)
    encodings.push_back(std::make_pair(_T("ISO_8859_11"), eISO8859_11)); ///< Thai
    encodings.push_back(std::make_pair(_T("ISO_8859_12"), eISO8859_12)); ///< reserved
    encodings.push_back(std::make_pair(_T("ISO_8859_13"), eISO8859_13)); ///< Latin7 (Baltic Rim)
    encodings.push_back(std::make_pair(_T("ISO_8859_14"), eISO8859_14)); ///< Latin8 (Celtic)
    encodings.push_back(std::make_pair(_T("ISO_8859_15"), eISO8859_15)); ///< Latin15 or Latin0 (Latin1 replacement, with 'euro')
    
    encodings.push_back(std::make_pair(_T("8859_1"), eISO8859_1));    ///< Latin1 (West European)
    encodings.push_back(std::make_pair(_T("8859_2"), eISO8859_2));  ///< Latin2 (East European)
    encodings.push_back(std::make_pair(_T("8859_3"), eISO8859_3));  ///< Latin3 (South European)
    encodings.push_back(std::make_pair(_T("8859_4"), eISO8859_4));  ///< Latin4 (North European)
    encodings.push_back(std::make_pair(_T("8859_5"), eISO8859_5));  ///< Cyrillic (see also KOI8)
    encodings.push_back(std::make_pair(_T("8859_6"), eISO8859_6));  ///< Arabic
    encodings.push_back(std::make_pair(_T("8859_7"), eISO8859_7));  ///< Greek
    encodings.push_back(std::make_pair(_T("8859_8"), eISO8859_8));  ///< Hebrew
    encodings.push_back(std::make_pair(_T("8859_9"), eISO8859_9));  ///< Latin5 (Turkish)
    encodings.push_back(std::make_pair(_T("8859_10"), eISO8859_10)); ///< Latin6 (Nordic)
    encodings.push_back(std::make_pair(_T("8859_11"), eISO8859_11)); ///< Thai
    encodings.push_back(std::make_pair(_T("8859_12"), eISO8859_12)); ///< reserved
    encodings.push_back(std::make_pair(_T("8859_13"), eISO8859_13)); ///< Latin7 (Baltic Rim)
    encodings.push_back(std::make_pair(_T("8859_14"), eISO8859_14)); ///< Latin8 (Celtic)
    encodings.push_back(std::make_pair(_T("8859_15"), eISO8859_15)); ///< Latin15 or Latin0 (Latin1 replacement, with 'euro')
    
    
    // Mac
    encodings.push_back(std::make_pair(_T("APPLEROMAN"), eAppleRoman)); ///< Apple Roman
    encodings.push_back(std::make_pair(_T("MACINTOSH"), eAppleRoman)); ///< Apple Roman
    encodings.push_back(std::make_pair(_T("MAC"), eAppleRoman)); ///< Apple Roman
    
    // Windows code pages
    encodings.push_back(std::make_pair(_T("CP_1250"), eCP1250));     ///< Code page 1250 (Central Europe)
    encodings.push_back(std::make_pair(_T("CP_1251"), eCP1251));     ///< Code page 1251 (Cyrillic)
    encodings.push_back(std::make_pair(_T("CP_1252"), eCP1252));     ///< Code page 1252 (Latin I)
    encodings.push_back(std::make_pair(_T("CP_1253"), eCP1253));     ///< Code page 1253 (Greek)
    encodings.push_back(std::make_pair(_T("CP_1254"), eCP1254));     ///< Code page 1254 (Turkish)
    encodings.push_back(std::make_pair(_T("CP_1255"), eCP1255));     ///< Code page 1255 (Hebrew)
    encodings.push_back(std::make_pair(_T("CP_1256"), eCP1256));     ///< Code page 1256 (Arabic)
    encodings.push_back(std::make_pair(_T("CP_1257"), eCP1257));     ///< Code page 1257 (Baltic)
    encodings.push_back(std::make_pair(_T("CP_1258"), eCP1258));     ///< Code page 1258 (Viet Nam)
    encodings.push_back(std::make_pair(_T("CP_874"),  eCP874));      ///< Code page 874 (Thai)
    
    encodings.push_back(std::make_pair(_T("1250"), eCP1250));     ///< Code page 1250 (Central Europe)
    encodings.push_back(std::make_pair(_T("1251"), eCP1251));     ///< Code page 1251 (Cyrillic)
    encodings.push_back(std::make_pair(_T("1252"), eCP1252));     ///< Code page 1252 (Latin I)
    encodings.push_back(std::make_pair(_T("1253"), eCP1253));     ///< Code page 1253 (Greek)
    encodings.push_back(std::make_pair(_T("1254"), eCP1254));     ///< Code page 1254 (Turkish)
    encodings.push_back(std::make_pair(_T("1255"), eCP1255));     ///< Code page 1255 (Hebrew)
    encodings.push_back(std::make_pair(_T("1256"), eCP1256));     ///< Code page 1256 (Arabic)
    encodings.push_back(std::make_pair(_T("1257"), eCP1257));     ///< Code page 1257 (Baltic)
    encodings.push_back(std::make_pair(_T("1258"), eCP1258));     ///< Code page 1258 (Viet Nam)
    encodings.push_back(std::make_pair(_T("874"),  eCP874));      ///< Code page 874 (Thai)
    
    // Multi-byte encodings_
    encodings.push_back(std::make_pair(_T("KOI8_R"),   eKOI8R));      ///< KOI8-R (Cyrillic)
    encodings.push_back(std::make_pair(_T("S_JIS"),    eSJIS));       ///< Shift-JIS (Japanese)
    encodings.push_back(std::make_pair(_T("SHIFT_JIS"),    eSJIS));       ///< Shift-JIS (Japanese)
    encodings.push_back(std::make_pair(_T("EUC_JP"),   eEUCJP));      ///< EUC-JP (Japanese)
    encodings.push_back(std::make_pair(_T("GB_2312"),  eGB2312));     ///< GB-2312 (Chinese)
    encodings.push_back(std::make_pair(_T("BIG_5"),    eBig5));       ///< Big5 (Traditional chinese)
    
    // Windows code pages
    encodings.push_back(std::make_pair(_T("CP_932"),   eCP932));      ///< Code page 932 (Japanese Shift-JIS)
    encodings.push_back(std::make_pair(_T("CP_936"),   eCP936));      ///< Code page 936 (Simplified Chinese GBK, \e not GB2312)
    encodings.push_back(std::make_pair(_T("CP_949"),   eCP949));      ///< Code page 949 (Korean)
    encodings.push_back(std::make_pair(_T("CP_950"),   eCP950));      ///< Code page 950 (big5)
    encodings.push_back(std::make_pair(_T("CP_1200"),  eCP1200));     ///< Code page 1200 (Unicode BMP, UCS-2, native endian)
    
    encodings.push_back(std::make_pair(_T("932"),   eCP932));      ///< Code page 932 (Japanese Shift-JIS)
    encodings.push_back(std::make_pair(_T("936"),   eCP936));      ///< Code page 936 (Simplified Chinese GBK, \e not GB2312)
    encodings.push_back(std::make_pair(_T("949"),   eCP949));      ///< Code page 949 (Korean)
    encodings.push_back(std::make_pair(_T("950"),   eCP950));      ///< Code page 950 (big5)
    encodings.push_back(std::make_pair(_T("1200"),  eCP1200));     ///< Code page 1200 (Unicode BMP, UCS-2, native endian)
    
    
    // Unicode variants
    encodings.push_back(std::make_pair(_T("UTF_7"), eUTF7));       ///< UTF-7 (Unicode)
    encodings.push_back(std::make_pair(_T("UTF_8"), eUTF8));       ///< UTF-8 (Unicode)
    encodings.push_back(std::make_pair(_T("UCS_2"), eUCS2));       ///< UCS-2 (Unicode)
    encodings.push_back(std::make_pair(_T("UCS_4"), eUCS4));       ///< UCS-4 (Unicode)
  }
  
  for (uint32 i = 0; i < encodings.size(); i++)
  {
    nglTextEncoding res = encodings[i].second;
    nglString input(encodings[i].first);
    nglString source(WebString);
    source.Replace(_T(" "), nglString::Empty);
    source.ToUpper();
    
    //NGL_OUT(_T("1) Web: '%s' Source: '%s' Input: '%s'\n"), WebString.GetChars(), source.GetChars(), input.GetChars());
    if (source == input)
      return res;
    
    // Try - instead of _
    input.Replace('_', '-');
    //NGL_OUT(_T("2) Web: '%s' Source: '%s' Input: '%s'\n"), WebString.GetChars(), source.GetChars(), input.GetChars());
    if (source == input)
      return res;
    
    // Try ' ' instead of _
    input.Replace(_T("-"), nglString::Empty);
    //NGL_OUT(_T("3) Web: '%s' Source: '%s' Input: '%s'\n"), WebString.GetChars(), source.GetChars(), input.GetChars());
    if (source == input)
      return res;
  }
  
  return eUTF8;
}

