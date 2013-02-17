/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "ucdata.h"

typedef struct nuiUnicodeRangeDesc
{
  uint32 RangeStart;
  uint32 RangeEnd;
  nuiUnicodeRange Range;
} nuiUnicodeRangeDesc;

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


nuiUnicodeRange nuiGetUnicodeRange(nglUChar ch)
{
  nglUChar l, h;
  return nuiGetUnicodeRange(ch, l, h);
}

nuiUnicodeRange nuiGetUnicodeRange(nglUChar ch, nglUChar& rLow, nglUChar& rHigh)
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

nuiUnicodeDirection nuiGetUnicodeDirection(nglUChar ch)
{
  //#FIXME need to add BiDi algo here
  return eLeftToRight;
}

bool nuiIsUnicodeBlank(nglUChar ch)
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

nuiTextRange::~nuiTextRange()
{
}

bool nuiSplitText(const nuiUCharIterator& Iterator, nuiTextRangeList& rRanges, nuiSplitTextFlag flags, int32 start, int32 length, const nuiTextSplitterDelegate& rSplitDelegate)
{
  int32 size = Iterator.GetLength();
  if (length < 0)
    length = size;
  if (length + start > size)
    length = size - start;

  rRanges.clear();
  if (!size)
    return true;

  const bool scriptchange = flags & nuiST_StrictScriptChange;
  const bool rangechange = flags & nuiST_RangeChange;
  const bool wordboundary = flags & nuiST_WordBoundary;
  const bool directionchange = flags & nuiST_DirectionChange;
  const bool mergecommonscript = flags & nuiST_MergeCommonScript;
  const bool printable = flags & nuiST_Printable;
  
  int32 lastpos = start;
  int32 curpos = start;
  nglUChar oldch = 0;
  nglUChar ch = Iterator.GetNextUChar(curpos);
  int32 direction = nuiGetUnicodeDirection(ch);
  int32 newdirection = direction;
  bool print = ucisprint(ch);
  bool newprint = print;
  
  nglUChar scriptlow = 0;
  nglUChar scripthi = 0;
  nuiUnicodeScript script = nuiGetUnicodeScript(ch, scriptlow, scripthi);
  nuiUnicodeScript newscript = script;

  nglUChar rangelow = 0;
  nglUChar rangehi = 0;
  nuiUnicodeRange range = rangechange ? nuiGetUnicodeRange(ch, rangelow, rangehi) : eRangeUnused;
  nuiUnicodeRange newrange = range;
  bool blank = nuiIsUnicodeBlank(ch);
  bool newblank = blank;
  
  
  while (curpos < start + length)
  {
    bool brk = false;
    int32 pos = curpos;
    ch = Iterator.GetNextUChar(curpos);

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
          if (newscript != script)
          {
            if (mergecommonscript)
            {
              if (newscript == eScriptCommon || newscript == eScriptInherited)
              {
                // Continue with the current script as it is
              }
              else if (script == eScriptCommon || script == eScriptInherited)
              {
                script = newscript; // Merge scripts by replacing the common script by the actual one, but don't break
              }
              else
              {
                // Script really changed so we break
                brk = true;
              }
            }
            else
            {
              // Script changed so we break
              brk = true;
            }
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
    
    if (printable)
    {
      newprint = ucisprint(ch);
      if (newprint != print)
        brk = true;
    }
    
    if (!brk && rSplitDelegate)
      brk = rSplitDelegate(oldch, ch, pos);
    
    if (brk)
    {
      nuiTextRange r;
      r.mLength = pos - lastpos; // count of unicode code points
      r.mDirection = direction; // even: Left to right, odd: right to left
      r.mScript = script; // What script if this range of text
      r.mRange = range; // What script if this range of text
      r.mBlank = blank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
      r.mPrintable = print;
      
      rRanges.push_back(r);
      
      lastpos = pos;
      direction = newdirection;
      script = newscript;
      range = newrange;
      blank = newblank;
      print = newprint;
    }
    
    oldch = ch;
  }

  // Last range:
  if (curpos > lastpos)
  {
    nuiTextRange r;
    r.mLength = curpos - lastpos; // count of unicode code points
    r.mDirection = direction; // even: Left to right, odd: right to left
    r.mScript = script; // What script if this range of text
    r.mRange = range; // What script if this range of text
    r.mBlank = blank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
    
    rRanges.push_back(r);
  }  
  
  return true;
}

bool nuiSplitText(const nglString& rSourceString, nuiTextRangeList& rRanges, nuiSplitTextFlag flags, int32 start, int32 length, const nuiTextSplitterDelegate& rDelegate)
{
  nuiUCharIterator_String it(rSourceString);
  return nuiSplitText(it, rRanges, flags, start, length, rDelegate);
}

bool nuiSplitText(const std::vector<nglUChar>& rSourceString, nuiTextRangeList& rRanges, nuiSplitTextFlag flags, int32 start, int32 length, const nuiTextSplitterDelegate& rDelegate)
{
  nuiUCharIterator_Vector it(rSourceString);
  return nuiSplitText(it, rRanges, flags, start, length, rDelegate);
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


nuiUnicodeScript nuiGetUnicodeScript(nglUChar ch)
{
  nglUChar low, hi;
  return nuiGetUnicodeScript(ch, low, hi);
}

nuiUnicodeScript nuiGetUnicodeScript(nglUChar ch, nglUChar& rLow, nglUChar& rHigh)
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

///////// Mirroring Characters:
static const nglUChar gMirrorsArray[] =
{
  0x0028, 0x0029,// LEFT PARENTHESIS
  0x0029, 0x0028,// RIGHT PARENTHESIS
  0x003C, 0x003E,// LESS-THAN SIGN
  0x003E, 0x003C,// GREATER-THAN SIGN
  0x005B, 0x005D,// LEFT SQUARE BRACKET
  0x005D, 0x005B,// RIGHT SQUARE BRACKET
  0x007B, 0x007D,// LEFT CURLY BRACKET
  0x007D, 0x007B,// RIGHT CURLY BRACKET
  0x00AB, 0x00BB,// LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
  0x00BB, 0x00AB,// RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
  0x0F3A, 0x0F3B,// TIBETAN MARK GUG RTAGS GYON
  0x0F3B, 0x0F3A,// TIBETAN MARK GUG RTAGS GYAS
  0x0F3C, 0x0F3D,// TIBETAN MARK ANG KHANG GYON
  0x0F3D, 0x0F3C,// TIBETAN MARK ANG KHANG GYAS
  0x169B, 0x169C,// OGHAM FEATHER MARK
  0x169C, 0x169B,// OGHAM REVERSED FEATHER MARK
  0x2039, 0x203A,// SINGLE LEFT-POINTING ANGLE QUOTATION MARK
  0x203A, 0x2039,// SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
  0x2045, 0x2046,// LEFT SQUARE BRACKET WITH QUILL
  0x2046, 0x2045,// RIGHT SQUARE BRACKET WITH QUILL
  0x207D, 0x207E,// SUPERSCRIPT LEFT PARENTHESIS
  0x207E, 0x207D,// SUPERSCRIPT RIGHT PARENTHESIS
  0x208D, 0x208E,// SUBSCRIPT LEFT PARENTHESIS
  0x208E, 0x208D,// SUBSCRIPT RIGHT PARENTHESIS
  0x2208, 0x220B,// ELEMENT OF
  0x2209, 0x220C,// NOT AN ELEMENT OF
  0x220A, 0x220D,// SMALL ELEMENT OF
  0x220B, 0x2208,// CONTAINS AS MEMBER
  0x220C, 0x2209,// DOES NOT CONTAIN AS MEMBER
  0x220D, 0x220A,// SMALL CONTAINS AS MEMBER
  0x2215, 0x29F5,// DIVISION SLASH
  0x223C, 0x223D,// TILDE OPERATOR
  0x223D, 0x223C,// REVERSED TILDE
  0x2243, 0x22CD,// ASYMPTOTICALLY EQUAL TO
  0x2252, 0x2253,// APPROXIMATELY EQUAL TO OR THE IMAGE OF
  0x2253, 0x2252,// IMAGE OF OR APPROXIMATELY EQUAL TO
  0x2254, 0x2255,// COLON EQUALS
  0x2255, 0x2254,// EQUALS COLON
  0x2264, 0x2265,// LESS-THAN OR EQUAL TO
  0x2265, 0x2264,// GREATER-THAN OR EQUAL TO
  0x2266, 0x2267,// LESS-THAN OVER EQUAL TO
  0x2267, 0x2266,// GREATER-THAN OVER EQUAL TO
  0x2268, 0x2269,// [BEST FIT] LESS-THAN BUT NOT EQUAL TO
  0x2269, 0x2268,// [BEST FIT] GREATER-THAN BUT NOT EQUAL TO
  0x226A, 0x226B,// MUCH LESS-THAN
  0x226B, 0x226A,// MUCH GREATER-THAN
  0x226E, 0x226F,// [BEST FIT] NOT LESS-THAN
  0x226F, 0x226E,// [BEST FIT] NOT GREATER-THAN
  0x2270, 0x2271,// [BEST FIT] NEITHER LESS-THAN NOR EQUAL TO
  0x2271, 0x2270,// [BEST FIT] NEITHER GREATER-THAN NOR EQUAL TO
  0x2272, 0x2273,// [BEST FIT] LESS-THAN OR EQUIVALENT TO
  0x2273, 0x2272,// [BEST FIT] GREATER-THAN OR EQUIVALENT TO
  0x2274, 0x2275,// [BEST FIT] NEITHER LESS-THAN NOR EQUIVALENT TO
  0x2275, 0x2274,// [BEST FIT] NEITHER GREATER-THAN NOR EQUIVALENT TO
  0x2276, 0x2277,// LESS-THAN OR GREATER-THAN
  0x2277, 0x2276,// GREATER-THAN OR LESS-THAN
  0x2278, 0x2279,// [BEST FIT] NEITHER LESS-THAN NOR GREATER-THAN
  0x2279, 0x2278,// [BEST FIT] NEITHER GREATER-THAN NOR LESS-THAN
  0x227A, 0x227B,// PRECEDES
  0x227B, 0x227A,// SUCCEEDS
  0x227C, 0x227D,// PRECEDES OR EQUAL TO
  0x227D, 0x227C,// SUCCEEDS OR EQUAL TO
  0x227E, 0x227F,// [BEST FIT] PRECEDES OR EQUIVALENT TO
  0x227F, 0x227E,// [BEST FIT] SUCCEEDS OR EQUIVALENT TO
  0x2280, 0x2281,// [BEST FIT] DOES NOT PRECEDE
  0x2281, 0x2280,// [BEST FIT] DOES NOT SUCCEED
  0x2282, 0x2283,// SUBSET OF
  0x2283, 0x2282,// SUPERSET OF
  0x2284, 0x2285,// [BEST FIT] NOT A SUBSET OF
  0x2285, 0x2284,// [BEST FIT] NOT A SUPERSET OF
  0x2286, 0x2287,// SUBSET OF OR EQUAL TO
  0x2287, 0x2286,// SUPERSET OF OR EQUAL TO
  0x2288, 0x2289,// [BEST FIT] NEITHER A SUBSET OF NOR EQUAL TO
  0x2289, 0x2288,// [BEST FIT] NEITHER A SUPERSET OF NOR EQUAL TO
  0x228A, 0x228B,// [BEST FIT] SUBSET OF WITH NOT EQUAL TO
  0x228B, 0x228A,// [BEST FIT] SUPERSET OF WITH NOT EQUAL TO
  0x228F, 0x2290,// SQUARE IMAGE OF
  0x2290, 0x228F,// SQUARE ORIGINAL OF
  0x2291, 0x2292,// SQUARE IMAGE OF OR EQUAL TO
  0x2292, 0x2291,// SQUARE ORIGINAL OF OR EQUAL TO
  0x2298, 0x29B8,// CIRCLED DIVISION SLASH
  0x22A2, 0x22A3,// RIGHT TACK
  0x22A3, 0x22A2,// LEFT TACK
  0x22A6, 0x2ADE,// ASSERTION
  0x22A8, 0x2AE4,// TRUE
  0x22A9, 0x2AE3,// FORCES
  0x22AB, 0x2AE5,// DOUBLE VERTICAL BAR DOUBLE RIGHT TURNSTILE
  0x22B0, 0x22B1,// PRECEDES UNDER RELATION
  0x22B1, 0x22B0,// SUCCEEDS UNDER RELATION
  0x22B2, 0x22B3,// NORMAL SUBGROUP OF
  0x22B3, 0x22B2,// CONTAINS AS NORMAL SUBGROUP
  0x22B4, 0x22B5,// NORMAL SUBGROUP OF OR EQUAL TO
  0x22B5, 0x22B4,// CONTAINS AS NORMAL SUBGROUP OR EQUAL TO
  0x22B6, 0x22B7,// ORIGINAL OF
  0x22B7, 0x22B6,// IMAGE OF
  0x22C9, 0x22CA,// LEFT NORMAL FACTOR SEMIDIRECT PRODUCT
  0x22CA, 0x22C9,// RIGHT NORMAL FACTOR SEMIDIRECT PRODUCT
  0x22CB, 0x22CC,// LEFT SEMIDIRECT PRODUCT
  0x22CC, 0x22CB,// RIGHT SEMIDIRECT PRODUCT
  0x22CD, 0x2243,// REVERSED TILDE EQUALS
  0x22D0, 0x22D1,// DOUBLE SUBSET
  0x22D1, 0x22D0,// DOUBLE SUPERSET
  0x22D6, 0x22D7,// LESS-THAN WITH DOT
  0x22D7, 0x22D6,// GREATER-THAN WITH DOT
  0x22D8, 0x22D9,// VERY MUCH LESS-THAN
  0x22D9, 0x22D8,// VERY MUCH GREATER-THAN
  0x22DA, 0x22DB,// LESS-THAN EQUAL TO OR GREATER-THAN
  0x22DB, 0x22DA,// GREATER-THAN EQUAL TO OR LESS-THAN
  0x22DC, 0x22DD,// EQUAL TO OR LESS-THAN
  0x22DD, 0x22DC,// EQUAL TO OR GREATER-THAN
  0x22DE, 0x22DF,// EQUAL TO OR PRECEDES
  0x22DF, 0x22DE,// EQUAL TO OR SUCCEEDS
  0x22E0, 0x22E1,// [BEST FIT] DOES NOT PRECEDE OR EQUAL
  0x22E1, 0x22E0,// [BEST FIT] DOES NOT SUCCEED OR EQUAL
  0x22E2, 0x22E3,// [BEST FIT] NOT SQUARE IMAGE OF OR EQUAL TO
  0x22E3, 0x22E2,// [BEST FIT] NOT SQUARE ORIGINAL OF OR EQUAL TO
  0x22E4, 0x22E5,// [BEST FIT] SQUARE IMAGE OF OR NOT EQUAL TO
  0x22E5, 0x22E4,// [BEST FIT] SQUARE ORIGINAL OF OR NOT EQUAL TO
  0x22E6, 0x22E7,// [BEST FIT] LESS-THAN BUT NOT EQUIVALENT TO
  0x22E7, 0x22E6,// [BEST FIT] GREATER-THAN BUT NOT EQUIVALENT TO
  0x22E8, 0x22E9,// [BEST FIT] PRECEDES BUT NOT EQUIVALENT TO
  0x22E9, 0x22E8,// [BEST FIT] SUCCEEDS BUT NOT EQUIVALENT TO
  0x22EA, 0x22EB,// [BEST FIT] NOT NORMAL SUBGROUP OF
  0x22EB, 0x22EA,// [BEST FIT] DOES NOT CONTAIN AS NORMAL SUBGROUP
  0x22EC, 0x22ED,// [BEST FIT] NOT NORMAL SUBGROUP OF OR EQUAL TO
  0x22ED, 0x22EC,// [BEST FIT] DOES NOT CONTAIN AS NORMAL SUBGROUP OR EQUAL
  0x22F0, 0x22F1,// UP RIGHT DIAGONAL ELLIPSIS
  0x22F1, 0x22F0,// DOWN RIGHT DIAGONAL ELLIPSIS
  0x22F2, 0x22FA,// ELEMENT OF WITH LONG HORIZONTAL STROKE
  0x22F3, 0x22FB,// ELEMENT OF WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
  0x22F4, 0x22FC,// SMALL ELEMENT OF WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
  0x22F6, 0x22FD,// ELEMENT OF WITH OVERBAR
  0x22F7, 0x22FE,// SMALL ELEMENT OF WITH OVERBAR
  0x22FA, 0x22F2,// CONTAINS WITH LONG HORIZONTAL STROKE
  0x22FB, 0x22F3,// CONTAINS WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
  0x22FC, 0x22F4,// SMALL CONTAINS WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
  0x22FD, 0x22F6,// CONTAINS WITH OVERBAR
  0x22FE, 0x22F7,// SMALL CONTAINS WITH OVERBAR
  0x2308, 0x2309,// LEFT CEILING
  0x2309, 0x2308,// RIGHT CEILING
  0x230A, 0x230B,// LEFT FLOOR
  0x230B, 0x230A,// RIGHT FLOOR
  0x2329, 0x232A,// LEFT-POINTING ANGLE BRACKET
  0x232A, 0x2329,// RIGHT-POINTING ANGLE BRACKET
  0x2768, 0x2769,// MEDIUM LEFT PARENTHESIS ORNAMENT
  0x2769, 0x2768,// MEDIUM RIGHT PARENTHESIS ORNAMENT
  0x276A, 0x276B,// MEDIUM FLATTENED LEFT PARENTHESIS ORNAMENT
  0x276B, 0x276A,// MEDIUM FLATTENED RIGHT PARENTHESIS ORNAMENT
  0x276C, 0x276D,// MEDIUM LEFT-POINTING ANGLE BRACKET ORNAMENT
  0x276D, 0x276C,// MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT
  0x276E, 0x276F,// HEAVY LEFT-POINTING ANGLE QUOTATION MARK ORNAMENT
  0x276F, 0x276E,// HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT
  0x2770, 0x2771,// HEAVY LEFT-POINTING ANGLE BRACKET ORNAMENT
  0x2771, 0x2770,// HEAVY RIGHT-POINTING ANGLE BRACKET ORNAMENT
  0x2772, 0x2773,// LIGHT LEFT TORTOISE SHELL BRACKET
  0x2773, 0x2772,// LIGHT RIGHT TORTOISE SHELL BRACKET
  0x2774, 0x2775,// MEDIUM LEFT CURLY BRACKET ORNAMENT
  0x2775, 0x2774,// MEDIUM RIGHT CURLY BRACKET ORNAMENT
  0x27C3, 0x27C4,// OPEN SUBSET
  0x27C4, 0x27C3,// OPEN SUPERSET
  0x27C5, 0x27C6,// LEFT S-SHAPED BAG DELIMITER
  0x27C6, 0x27C5,// RIGHT S-SHAPED BAG DELIMITER
  0x27C8, 0x27C9,// REVERSE SOLIDUS PRECEDING SUBSET
  0x27C9, 0x27C8,// SUPERSET PRECEDING SOLIDUS
  0x27D5, 0x27D6,// LEFT OUTER JOIN
  0x27D6, 0x27D5,// RIGHT OUTER JOIN
  0x27DD, 0x27DE,// LONG RIGHT TACK
  0x27DE, 0x27DD,// LONG LEFT TACK
  0x27E2, 0x27E3,// WHITE CONCAVE-SIDED DIAMOND WITH LEFTWARDS TICK
  0x27E3, 0x27E2,// WHITE CONCAVE-SIDED DIAMOND WITH RIGHTWARDS TICK
  0x27E4, 0x27E5,// WHITE SQUARE WITH LEFTWARDS TICK
  0x27E5, 0x27E4,// WHITE SQUARE WITH RIGHTWARDS TICK
  0x27E6, 0x27E7,// MATHEMATICAL LEFT WHITE SQUARE BRACKET
  0x27E7, 0x27E6,// MATHEMATICAL RIGHT WHITE SQUARE BRACKET
  0x27E8, 0x27E9,// MATHEMATICAL LEFT ANGLE BRACKET
  0x27E9, 0x27E8,// MATHEMATICAL RIGHT ANGLE BRACKET
  0x27EA, 0x27EB,// MATHEMATICAL LEFT DOUBLE ANGLE BRACKET
  0x27EB, 0x27EA,// MATHEMATICAL RIGHT DOUBLE ANGLE BRACKET
  0x27EC, 0x27ED,// MATHEMATICAL LEFT WHITE TORTOISE SHELL BRACKET
  0x27ED, 0x27EC,// MATHEMATICAL RIGHT WHITE TORTOISE SHELL BRACKET
  0x27EE, 0x27EF,// MATHEMATICAL LEFT FLATTENED PARENTHESIS
  0x27EF, 0x27EE,// MATHEMATICAL RIGHT FLATTENED PARENTHESIS
  0x2983, 0x2984,// LEFT WHITE CURLY BRACKET
  0x2984, 0x2983,// RIGHT WHITE CURLY BRACKET
  0x2985, 0x2986,// LEFT WHITE PARENTHESIS
  0x2986, 0x2985,// RIGHT WHITE PARENTHESIS
  0x2987, 0x2988,// Z NOTATION LEFT IMAGE BRACKET
  0x2988, 0x2987,// Z NOTATION RIGHT IMAGE BRACKET
  0x2989, 0x298A,// Z NOTATION LEFT BINDING BRACKET
  0x298A, 0x2989,// Z NOTATION RIGHT BINDING BRACKET
  0x298B, 0x298C,// LEFT SQUARE BRACKET WITH UNDERBAR
  0x298C, 0x298B,// RIGHT SQUARE BRACKET WITH UNDERBAR
  0x298D, 0x2990,// LEFT SQUARE BRACKET WITH TICK IN TOP CORNER
  0x298E, 0x298F,// RIGHT SQUARE BRACKET WITH TICK IN BOTTOM CORNER
  0x298F, 0x298E,// LEFT SQUARE BRACKET WITH TICK IN BOTTOM CORNER
  0x2990, 0x298D,// RIGHT SQUARE BRACKET WITH TICK IN TOP CORNER
  0x2991, 0x2992,// LEFT ANGLE BRACKET WITH DOT
  0x2992, 0x2991,// RIGHT ANGLE BRACKET WITH DOT
  0x2993, 0x2994,// LEFT ARC LESS-THAN BRACKET
  0x2994, 0x2993,// RIGHT ARC GREATER-THAN BRACKET
  0x2995, 0x2996,// DOUBLE LEFT ARC GREATER-THAN BRACKET
  0x2996, 0x2995,// DOUBLE RIGHT ARC LESS-THAN BRACKET
  0x2997, 0x2998,// LEFT BLACK TORTOISE SHELL BRACKET
  0x2998, 0x2997,// RIGHT BLACK TORTOISE SHELL BRACKET
  0x29B8, 0x2298,// CIRCLED REVERSE SOLIDUS
  0x29C0, 0x29C1,// CIRCLED LESS-THAN
  0x29C1, 0x29C0,// CIRCLED GREATER-THAN
  0x29C4, 0x29C5,// SQUARED RISING DIAGONAL SLASH
  0x29C5, 0x29C4,// SQUARED FALLING DIAGONAL SLASH
  0x29CF, 0x29D0,// LEFT TRIANGLE BESIDE VERTICAL BAR
  0x29D0, 0x29CF,// VERTICAL BAR BESIDE RIGHT TRIANGLE
  0x29D1, 0x29D2,// BOWTIE WITH LEFT HALF BLACK
  0x29D2, 0x29D1,// BOWTIE WITH RIGHT HALF BLACK
  0x29D4, 0x29D5,// TIMES WITH LEFT HALF BLACK
  0x29D5, 0x29D4,// TIMES WITH RIGHT HALF BLACK
  0x29D8, 0x29D9,// LEFT WIGGLY FENCE
  0x29D9, 0x29D8,// RIGHT WIGGLY FENCE
  0x29DA, 0x29DB,// LEFT DOUBLE WIGGLY FENCE
  0x29DB, 0x29DA,// RIGHT DOUBLE WIGGLY FENCE
  0x29F5, 0x2215,// REVERSE SOLIDUS OPERATOR
  0x29F8, 0x29F9,// BIG SOLIDUS
  0x29F9, 0x29F8,// BIG REVERSE SOLIDUS
  0x29FC, 0x29FD,// LEFT-POINTING CURVED ANGLE BRACKET
  0x29FD, 0x29FC,// RIGHT-POINTING CURVED ANGLE BRACKET
  0x2A2B, 0x2A2C,// MINUS SIGN WITH FALLING DOTS
  0x2A2C, 0x2A2B,// MINUS SIGN WITH RISING DOTS
  0x2A2D, 0x2A2E,// PLUS SIGN IN LEFT HALF CIRCLE
  0x2A2E, 0x2A2D,// PLUS SIGN IN RIGHT HALF CIRCLE
  0x2A34, 0x2A35,// MULTIPLICATION SIGN IN LEFT HALF CIRCLE
  0x2A35, 0x2A34,// MULTIPLICATION SIGN IN RIGHT HALF CIRCLE
  0x2A3C, 0x2A3D,// INTERIOR PRODUCT
  0x2A3D, 0x2A3C,// RIGHTHAND INTERIOR PRODUCT
  0x2A64, 0x2A65,// Z NOTATION DOMAIN ANTIRESTRICTION
  0x2A65, 0x2A64,// Z NOTATION RANGE ANTIRESTRICTION
  0x2A79, 0x2A7A,// LESS-THAN WITH CIRCLE INSIDE
  0x2A7A, 0x2A79,// GREATER-THAN WITH CIRCLE INSIDE
  0x2A7D, 0x2A7E,// LESS-THAN OR SLANTED EQUAL TO
  0x2A7E, 0x2A7D,// GREATER-THAN OR SLANTED EQUAL TO
  0x2A7F, 0x2A80,// LESS-THAN OR SLANTED EQUAL TO WITH DOT INSIDE
  0x2A80, 0x2A7F,// GREATER-THAN OR SLANTED EQUAL TO WITH DOT INSIDE
  0x2A81, 0x2A82,// LESS-THAN OR SLANTED EQUAL TO WITH DOT ABOVE
  0x2A82, 0x2A81,// GREATER-THAN OR SLANTED EQUAL TO WITH DOT ABOVE
  0x2A83, 0x2A84,// LESS-THAN OR SLANTED EQUAL TO WITH DOT ABOVE RIGHT
  0x2A84, 0x2A83,// GREATER-THAN OR SLANTED EQUAL TO WITH DOT ABOVE LEFT
  0x2A8B, 0x2A8C,// LESS-THAN ABOVE DOUBLE-LINE EQUAL ABOVE GREATER-THAN
  0x2A8C, 0x2A8B,// GREATER-THAN ABOVE DOUBLE-LINE EQUAL ABOVE LESS-THAN
  0x2A91, 0x2A92,// LESS-THAN ABOVE GREATER-THAN ABOVE DOUBLE-LINE EQUAL
  0x2A92, 0x2A91,// GREATER-THAN ABOVE LESS-THAN ABOVE DOUBLE-LINE EQUAL
  0x2A93, 0x2A94,// LESS-THAN ABOVE SLANTED EQUAL ABOVE GREATER-THAN ABOVE SLANTED EQUAL
  0x2A94, 0x2A93,// GREATER-THAN ABOVE SLANTED EQUAL ABOVE LESS-THAN ABOVE SLANTED EQUAL
  0x2A95, 0x2A96,// SLANTED EQUAL TO OR LESS-THAN
  0x2A96, 0x2A95,// SLANTED EQUAL TO OR GREATER-THAN
  0x2A97, 0x2A98,// SLANTED EQUAL TO OR LESS-THAN WITH DOT INSIDE
  0x2A98, 0x2A97,// SLANTED EQUAL TO OR GREATER-THAN WITH DOT INSIDE
  0x2A99, 0x2A9A,// DOUBLE-LINE EQUAL TO OR LESS-THAN
  0x2A9A, 0x2A99,// DOUBLE-LINE EQUAL TO OR GREATER-THAN
  0x2A9B, 0x2A9C,// DOUBLE-LINE SLANTED EQUAL TO OR LESS-THAN
  0x2A9C, 0x2A9B,// DOUBLE-LINE SLANTED EQUAL TO OR GREATER-THAN
  0x2AA1, 0x2AA2,// DOUBLE NESTED LESS-THAN
  0x2AA2, 0x2AA1,// DOUBLE NESTED GREATER-THAN
  0x2AA6, 0x2AA7,// LESS-THAN CLOSED BY CURVE
  0x2AA7, 0x2AA6,// GREATER-THAN CLOSED BY CURVE
  0x2AA8, 0x2AA9,// LESS-THAN CLOSED BY CURVE ABOVE SLANTED EQUAL
  0x2AA9, 0x2AA8,// GREATER-THAN CLOSED BY CURVE ABOVE SLANTED EQUAL
  0x2AAA, 0x2AAB,// SMALLER THAN
  0x2AAB, 0x2AAA,// LARGER THAN
  0x2AAC, 0x2AAD,// SMALLER THAN OR EQUAL TO
  0x2AAD, 0x2AAC,// LARGER THAN OR EQUAL TO
  0x2AAF, 0x2AB0,// PRECEDES ABOVE SINGLE-LINE EQUALS SIGN
  0x2AB0, 0x2AAF,// SUCCEEDS ABOVE SINGLE-LINE EQUALS SIGN
  0x2AB3, 0x2AB4,// PRECEDES ABOVE EQUALS SIGN
  0x2AB4, 0x2AB3,// SUCCEEDS ABOVE EQUALS SIGN
  0x2ABB, 0x2ABC,// DOUBLE PRECEDES
  0x2ABC, 0x2ABB,// DOUBLE SUCCEEDS
  0x2ABD, 0x2ABE,// SUBSET WITH DOT
  0x2ABE, 0x2ABD,// SUPERSET WITH DOT
  0x2ABF, 0x2AC0,// SUBSET WITH PLUS SIGN BELOW
  0x2AC0, 0x2ABF,// SUPERSET WITH PLUS SIGN BELOW
  0x2AC1, 0x2AC2,// SUBSET WITH MULTIPLICATION SIGN BELOW
  0x2AC2, 0x2AC1,// SUPERSET WITH MULTIPLICATION SIGN BELOW
  0x2AC3, 0x2AC4,// SUBSET OF OR EQUAL TO WITH DOT ABOVE
  0x2AC4, 0x2AC3,// SUPERSET OF OR EQUAL TO WITH DOT ABOVE
  0x2AC5, 0x2AC6,// SUBSET OF ABOVE EQUALS SIGN
  0x2AC6, 0x2AC5,// SUPERSET OF ABOVE EQUALS SIGN
  0x2ACD, 0x2ACE,// SQUARE LEFT OPEN BOX OPERATOR
  0x2ACE, 0x2ACD,// SQUARE RIGHT OPEN BOX OPERATOR
  0x2ACF, 0x2AD0,// CLOSED SUBSET
  0x2AD0, 0x2ACF,// CLOSED SUPERSET
  0x2AD1, 0x2AD2,// CLOSED SUBSET OR EQUAL TO
  0x2AD2, 0x2AD1,// CLOSED SUPERSET OR EQUAL TO
  0x2AD3, 0x2AD4,// SUBSET ABOVE SUPERSET
  0x2AD4, 0x2AD3,// SUPERSET ABOVE SUBSET
  0x2AD5, 0x2AD6,// SUBSET ABOVE SUBSET
  0x2AD6, 0x2AD5,// SUPERSET ABOVE SUPERSET
  0x2ADE, 0x22A6,// SHORT LEFT TACK
  0x2AE3, 0x22A9,// DOUBLE VERTICAL BAR LEFT TURNSTILE
  0x2AE4, 0x22A8,// VERTICAL BAR DOUBLE LEFT TURNSTILE
  0x2AE5, 0x22AB,// DOUBLE VERTICAL BAR DOUBLE LEFT TURNSTILE
  0x2AEC, 0x2AED,// DOUBLE STROKE NOT SIGN
  0x2AED, 0x2AEC,// REVERSED DOUBLE STROKE NOT SIGN
  0x2AF7, 0x2AF8,// TRIPLE NESTED LESS-THAN
  0x2AF8, 0x2AF7,// TRIPLE NESTED GREATER-THAN
  0x2AF9, 0x2AFA,// DOUBLE-LINE SLANTED LESS-THAN OR EQUAL TO
  0x2AFA, 0x2AF9,// DOUBLE-LINE SLANTED GREATER-THAN OR EQUAL TO
  0x2E02, 0x2E03,// LEFT SUBSTITUTION BRACKET
  0x2E03, 0x2E02,// RIGHT SUBSTITUTION BRACKET
  0x2E04, 0x2E05,// LEFT DOTTED SUBSTITUTION BRACKET
  0x2E05, 0x2E04,// RIGHT DOTTED SUBSTITUTION BRACKET
  0x2E09, 0x2E0A,// LEFT TRANSPOSITION BRACKET
  0x2E0A, 0x2E09,// RIGHT TRANSPOSITION BRACKET
  0x2E0C, 0x2E0D,// LEFT RAISED OMISSION BRACKET
  0x2E0D, 0x2E0C,// RIGHT RAISED OMISSION BRACKET
  0x2E1C, 0x2E1D,// LEFT LOW PARAPHRASE BRACKET
  0x2E1D, 0x2E1C,// RIGHT LOW PARAPHRASE BRACKET
  0x2E20, 0x2E21,// LEFT VERTICAL BAR WITH QUILL
  0x2E21, 0x2E20,// RIGHT VERTICAL BAR WITH QUILL
  0x2E22, 0x2E23,// TOP LEFT HALF BRACKET
  0x2E23, 0x2E22,// TOP RIGHT HALF BRACKET
  0x2E24, 0x2E25,// BOTTOM LEFT HALF BRACKET
  0x2E25, 0x2E24,// BOTTOM RIGHT HALF BRACKET
  0x2E26, 0x2E27,// LEFT SIDEWAYS U BRACKET
  0x2E27, 0x2E26,// RIGHT SIDEWAYS U BRACKET
  0x2E28, 0x2E29,// LEFT DOUBLE PARENTHESIS
  0x2E29, 0x2E28,// RIGHT DOUBLE PARENTHESIS
  0x3008, 0x3009,// LEFT ANGLE BRACKET
  0x3009, 0x3008,// RIGHT ANGLE BRACKET
  0x300A, 0x300B,// LEFT DOUBLE ANGLE BRACKET
  0x300B, 0x300A,// RIGHT DOUBLE ANGLE BRACKET
  0x300C, 0x300D,// [BEST FIT] LEFT CORNER BRACKET
  0x300D, 0x300C,// [BEST FIT] RIGHT CORNER BRACKET
  0x300E, 0x300F,// [BEST FIT] LEFT WHITE CORNER BRACKET
  0x300F, 0x300E,// [BEST FIT] RIGHT WHITE CORNER BRACKET
  0x3010, 0x3011,// LEFT BLACK LENTICULAR BRACKET
  0x3011, 0x3010,// RIGHT BLACK LENTICULAR BRACKET
  0x3014, 0x3015,// LEFT TORTOISE SHELL BRACKET
  0x3015, 0x3014,// RIGHT TORTOISE SHELL BRACKET
  0x3016, 0x3017,// LEFT WHITE LENTICULAR BRACKET
  0x3017, 0x3016,// RIGHT WHITE LENTICULAR BRACKET
  0x3018, 0x3019,// LEFT WHITE TORTOISE SHELL BRACKET
  0x3019, 0x3018,// RIGHT WHITE TORTOISE SHELL BRACKET
  0x301A, 0x301B,// LEFT WHITE SQUARE BRACKET
  0x301B, 0x301A,// RIGHT WHITE SQUARE BRACKET
  0xFE59, 0xFE5A,// SMALL LEFT PARENTHESIS
  0xFE5A, 0xFE59,// SMALL RIGHT PARENTHESIS
  0xFE5B, 0xFE5C,// SMALL LEFT CURLY BRACKET
  0xFE5C, 0xFE5B,// SMALL RIGHT CURLY BRACKET
  0xFE5D, 0xFE5E,// SMALL LEFT TORTOISE SHELL BRACKET
  0xFE5E, 0xFE5D,// SMALL RIGHT TORTOISE SHELL BRACKET
  0xFE64, 0xFE65,// SMALL LESS-THAN SIGN
  0xFE65, 0xFE64,// SMALL GREATER-THAN SIGN
  0xFF08, 0xFF09,// FULLWIDTH LEFT PARENTHESIS
  0xFF09, 0xFF08,// FULLWIDTH RIGHT PARENTHESIS
  0xFF1C, 0xFF1E,// FULLWIDTH LESS-THAN SIGN
  0xFF1E, 0xFF1C,// FULLWIDTH GREATER-THAN SIGN
  0xFF3B, 0xFF3D,// FULLWIDTH LEFT SQUARE BRACKET
  0xFF3D, 0xFF3B,// FULLWIDTH RIGHT SQUARE BRACKET
  0xFF5B, 0xFF5D,// FULLWIDTH LEFT CURLY BRACKET
  0xFF5D, 0xFF5B,// FULLWIDTH RIGHT CURLY BRACKET
  0xFF5F, 0xFF60,// FULLWIDTH LEFT WHITE PARENTHESIS
  0xFF60, 0xFF5F,// FULLWIDTH RIGHT WHITE PARENTHESIS
  0xFF62, 0xFF63,// [BEST FIT] HALFWIDTH LEFT CORNER BRACKET
  0xFF63, 0xFF62,// [BEST FIT] HALFWIDTH RIGHT CORNER BRACKET
  0, 0
};

static std::map<nglUChar, nglUChar> gMirrors;

nglUChar nuiGetMirrorringChar(nglUChar ch)
{
  if (gMirrors.empty())
  {
    int32 i = 0;
    while (gMirrorsArray[i])
    {
      gMirrors[gMirrorsArray[i]] = gMirrorsArray[i+1];
      i += 2;
    }
  }
  
  std::map<nglUChar, nglUChar>::const_iterator it = gMirrors.find(ch);
  if (it != gMirrors.end())
    return it->second;

  return ch;
}
