/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

enum nuiUnicodeScript
{
  eScriptCommon,
  eScriptLatin,
  eScriptArabic,
  eScriptArmenian,
  eScriptBalinese,
  eScriptBengali,
  eScriptBopomofo,
  eScriptBraille,
  eScriptBuginese,
  eScriptBuhid,
  eScriptCanadian_Aboriginal,
  eScriptCarian,
  eScriptCham,
  eScriptCherokee,
  eScriptCoptic,
  eScriptCuneiform,
  eScriptCypriot,
  eScriptCyrillic,
  eScriptDeseret,
  eScriptDevanagari,
  eScriptEthiopic,
  eScriptGeorgian,
  eScriptGlagolitic,
  eScriptGothic,
  eScriptGreek,
  eScriptGujarati,
  eScriptGurmukhi,
  eScriptHan,
  eScriptHangul,
  eScriptHanunoo,
  eScriptHebrew,
  eScriptHiragana,
  eScriptInherited,
  eScriptKannada,
  eScriptKatakana,
  eScriptKayah_Li,
  eScriptKharoshthi,
  eScriptKhmer,
  eScriptLao,
  eScriptLepcha,
  eScriptLimbu,
  eScriptLinear_B,
  eScriptLycian,
  eScriptLydian,
  eScriptMalayalam,
  eScriptMongolian,
  eScriptMyanmar,
  eScriptNew_Tai_Lue,
  eScriptNko,
  eScriptOgham,
  eScriptOl_Chiki,
  eScriptOld_Italic,
  eScriptOld_Persian,
  eScriptOriya,
  eScriptOsmanya,
  eScriptPhags_Pa,
  eScriptPhoenician,
  eScriptRejang,
  eScriptRunic,
  eScriptSaurashtra,
  eScriptShavian,
  eScriptSinhala,
  eScriptSundanese,
  eScriptSyloti_Nagri,
  eScriptSyriac,
  eScriptTagalog,
  eScriptTagbanwa,
  eScriptTai_Le,
  eScriptTamil,
  eScriptTelugu,
  eScriptThaana,
  eScriptThai,
  eScriptTibetan,
  eScriptTifinagh,
  eScriptUgaritic,
  eScriptVai,
  eScriptYi
};

nuiUnicodeScript nuiGetUnicodeScript(nglUChar ch);
nuiUnicodeScript nuiGetUnicodeScript(nglUChar ch, nglUChar& rLow, nglUChar& rHigh);
nglString nuiGetUnicodeScriptName(nuiUnicodeScript script);


enum nuiUnicodeRange
{
  eRangeNone = 0,
  eRangeUndefined,
  eRangeUnused,
  eRangeBasicLatin,
  eRangeC1ControlsAndLatin1Supplement,
  eRangeLatinExtendedA,
  eRangeLatinExtendedB,
  eRangeIPAExtensions,
  eRangeSpacingModifierLetters,
  eRangeCombiningDiacriticalMarks,
  eRangeGreekAndCoptic,
  eRangeCyrillic,
  eRangeCyrillicSupplement,
  eRangeArmenian,
  eRangeHebrew,
  eRangeArabic,
  eRangeSyriac,
  eRangeThaana,
  eRangeDevanagari,
  eRangeBengaliAndAssamese,
  eRangeGurmukhi,
  eRangeGujarati,
  eRangeOriya,
  eRangeTamil,
  eRangeTelugu,
  eRangeKannada,
  eRangeMalayalam,
  eRangeSinhala,
  eRangeThai,
  eRangeLao,
  eRangeTibetan,
  eRangeMyanmar,
  eRangeGeorgian,
  eRangeHangulJamo,
  eRangeEthiopic,
  eRangeCherokee,
  eRangeUnifiedCanadianAboriginalSyllabics,
  eRangeOgham,
  eRangeRunic,
  eRangeTagalog,
  eRangeHanunoo,
  eRangeBuhid,
  eRangeTagbanwa,
  eRangeKhmer,
  eRangeMongolian,
  eRangeLimbu,
  eRangeTaiLe,
  eRangeKhmerSymbols,
  eRangePhoneticExtensions,
  eRangeLatinExtendedAdditional,
  eRangeGreekExtended,
  eRangeGeneralPunctuation,
  eRangeSuperscriptsAndSubscripts,
  eRangeCurrencySymbols,
  eRangeCombiningDiacriticalMarksForSymbols,
  eRangeLetterlikeSymbols,
  eRangeNumberForms,
  eRangeArrows,
  eRangeMathematicalOperators,
  eRangeMiscellaneousTechnical,
  eRangeControlPictures,
  eRangeOpticalCharacterRecognition,
  eRangeEnclosedAlphanumerics,
  eRangeBoxDrawing,
  eRangeBlockElements,
  eRangeGeometricShapes,
  eRangeMiscellaneousSymbols,
  eRangeDingbats,
  eRangeMiscellaneousMathematicalSymbolsA,
  eRangeSupplementalArrowsA,
  eRangeBraillePatterns,
  eRangeSupplementalArrowsB,
  eRangeMiscellaneousMathematicalSymbolsB,
  eRangeSupplementalMathematicalOperators,
  eRangeMiscellaneousSymbolsAndArrows,
  eRangeCJKRadicalsSupplement,
  eRangeKangxiRadicals,
  eRangeIdeographicDescriptionCharacters,
  eRangeCJKSymbolsAndPunctuation,
  eRangeHiragana,
  eRangeKatakana,
  eRangeBopomofo,
  eRangeHangulCompatibilityJamo,
  eRangeKanbun,
  eRangeBopomofoExtended,
  eRangeKatakanaPhoneticExtensions,
  eRangeEnclosedCJKLettersAndMonths,
  eRangeCJKCompatibility,
  eRangeCJKUnifiedIdeographsExtensionA,
  eRangeYijingHexagramSymbols,
  eRangeCJKUnifiedIdeographs,
  eRangeYiSyllables,
  eRangeYiRadicals,
  eRangeHangulSyllables,
  eRangeHighSurrogateArea,
  eRangeLowSurrogateArea,
  eRangePrivateUseArea,
  eRangeCJKCompatibilityIdeographs,
  eRangeAlphabeticPresentationForms,
  eRangeArabicPresentationFormsA,
  eRangeVariationSelectors,
  eRangeCombiningHalfMarks,
  eRangeCJKCompatibilityForms,
  eRangeSmallFormVariants,
  eRangeArabicPresentationFormsB,
  eRangeHalfwidthAndFullwidthForms,
  eRangeSpecials,
  eRangeLinearBSyllabary,
  eRangeLinearBIdeograms,
  eRangeAegeanNumbers,
  eRangeOldItalic,
  eRangeGothic,
  eRangeUgaritic,
  eRangeDeseret,
  eRangeShavian,
  eRangeOsmanya,
  eRangeCypriotSyllabary,
  eRangeByzantineMusicalSymbols,
  eRangeMusicalSymbols,
  eRangeTaiXuanJingSymbols,
  eRangeMathematicalAlphanumericSymbols,
  eRangeCJKUnifiedIdeographsExtensionB,
  eRangeCJKCompatibilityIdeographsSupplement,
  eRangeTags,
  eRangeVariationSelectorsSupplement,
  eRangeSupplementaryPrivateUseAreaA,
  eRangeSupplementaryPrivateUseAreaB
};

nuiUnicodeRange nuiGetUnicodeRange(nglUChar ch);
nuiUnicodeRange nuiGetUnicodeRange(nglUChar ch, nglUChar& rLow, nglUChar& rHigh);
nglString nuiGetUnicodeRangeName(nuiUnicodeRange range);

enum nuiUnicodeDirection
{
  eLeftToRight,
  eRightToLeft
};

nuiUnicodeDirection nuiGetUnicodeDirection(nglUChar ch);



bool nuiIsUnicodeBlank(nglUChar ch);

class nuiTextRange
{
public:
  nuiTextRange();
  
  uint32 mLength; // Count of unicode code points
  int32 mDirection; // Even: Left to right, Odd: right to left
  nuiUnicodeScript mScript; // What script is this range of text
  nuiUnicodeRange mRange; // What unicode range is this range of text
  bool mBlank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
};

typedef std::list<nuiTextRange> nuiTextRangeList;
typedef uint32 nuiSplitTextFlag;

const nuiSplitTextFlag nuiST_StrictScriptChange = 1 << 0;
const nuiSplitTextFlag nuiST_WordBoundary    = 1 << 1;
const nuiSplitTextFlag nuiST_DirectionChange = 1 << 2;
const nuiSplitTextFlag nuiST_RangeChange     = 1 << 3;
const nuiSplitTextFlag nuiST_MergeCommonScript = 1 << 4;
const nuiSplitTextFlag nuiST_ScriptChange    = nuiST_StrictScriptChange | nuiST_MergeCommonScript;
const nuiSplitTextFlag nuiST_Natural         = nuiST_ScriptChange | nuiST_WordBoundary | nuiST_DirectionChange | nuiST_MergeCommonScript;
const nuiSplitTextFlag nuiST_All             = nuiST_Natural | nuiST_RangeChange;

bool nuiSplitText(const nglString& rSourceString, nuiTextRangeList& rRanges, nuiSplitTextFlag flags = nuiST_Natural);

nglTextEncoding nuiGetTextEncodingFromString(const nglString& WebString);

