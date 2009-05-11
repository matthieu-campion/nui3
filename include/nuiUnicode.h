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

nuiUnicodeScript nuiGetUnicodeScript(nglChar ch);
nuiUnicodeScript nuiGetUnicodeScript(nglChar ch, nglChar& rLow, nglChar& rHigh);
nglString nuiGetUnicodeScriptName(nuiUnicodeScript script);


enum nuiUnicodeRange
{
  eNone = 0,
  eUndefined,
  eUnused,
  eBasicLatin,
  eC1ControlsAndLatin1Supplement,
  eLatinExtendedA,
  eLatinExtendedB,
  eIPAExtensions,
  eSpacingModifierLetters,
  eCombiningDiacriticalMarks,
  eGreekAndCoptic,
  eCyrillic,
  eCyrillicSupplement,
  eArmenian,
  eHebrew,
  eArabic,
  eSyriac,
  eThaana,
  eDevanagari,
  eBengaliAndAssamese,
  eGurmukhi,
  eGujarati,
  eOriya,
  eTamil,
  eTelugu,
  eKannada,
  eMalayalam,
  eSinhala,
  eThai,
  eLao,
  eTibetan,
  eMyanmar,
  eGeorgian,
  eHangulJamo,
  eEthiopic,
  eCherokee,
  eUnifiedCanadianAboriginalSyllabics,
  eOgham,
  eRunic,
  eTagalog,
  eHanunoo,
  eBuhid,
  eTagbanwa,
  eKhmer,
  eMongolian,
  eLimbu,
  eTaiLe,
  eKhmerSymbols,
  ePhoneticExtensions,
  eLatinExtendedAdditional,
  eGreekExtended,
  eGeneralPunctuation,
  eSuperscriptsAndSubscripts,
  eCurrencySymbols,
  eCombiningDiacriticalMarksForSymbols,
  eLetterlikeSymbols,
  eNumberForms,
  eArrows,
  eMathematicalOperators,
  eMiscellaneousTechnical,
  eControlPictures,
  eOpticalCharacterRecognition,
  eEnclosedAlphanumerics,
  eBoxDrawing,
  eBlockElements,
  eGeometricShapes,
  eMiscellaneousSymbols,
  eDingbats,
  eMiscellaneousMathematicalSymbolsA,
  eSupplementalArrowsA,
  eBraillePatterns,
  eSupplementalArrowsB,
  eMiscellaneousMathematicalSymbolsB,
  eSupplementalMathematicalOperators,
  eMiscellaneousSymbolsAndArrows,
  eCJKRadicalsSupplement,
  eKangxiRadicals,
  eIdeographicDescriptionCharacters,
  eCJKSymbolsAndPunctuation,
  eHiragana,
  eKatakana,
  eBopomofo,
  eHangulCompatibilityJamo,
  eKanbun,
  eBopomofoExtended,
  eKatakanaPhoneticExtensions,
  eEnclosedCJKLettersAndMonths,
  eCJKCompatibility,
  eCJKUnifiedIdeographsExtensionA,
  eYijingHexagramSymbols,
  eCJKUnifiedIdeographs,
  eYiSyllables,
  eYiRadicals,
  eHangulSyllables,
  eHighSurrogateArea,
  eLowSurrogateArea,
  ePrivateUseArea,
  eCJKCompatibilityIdeographs,
  eAlphabeticPresentationForms,
  eArabicPresentationFormsA,
  eVariationSelectors,
  eCombiningHalfMarks,
  eCJKCompatibilityForms,
  eSmallFormVariants,
  eArabicPresentationFormsB,
  eHalfwidthAndFullwidthForms,
  eSpecials,
  eLinearBSyllabary,
  eLinearBIdeograms,
  eAegeanNumbers,
  eOldItalic,
  eGothic,
  eUgaritic,
  eDeseret,
  eShavian,
  eOsmanya,
  eCypriotSyllabary,
  eByzantineMusicalSymbols,
  eMusicalSymbols,
  eTaiXuanJingSymbols,
  eMathematicalAlphanumericSymbols,
  eCJKUnifiedIdeographsExtensionB,
  eCJKCompatibilityIdeographsSupplement,
  eTags,
  eVariationSelectorsSupplement,
  eSupplementaryPrivateUseAreaA,
  eSupplementaryPrivateUseAreaB
};

nuiUnicodeRange nuiGetUnicodeRange(nglChar ch);
nuiUnicodeRange nuiGetUnicodeRange(nglChar ch, nglChar& rLow, nglChar& rHigh);
nglString nuiGetUnicodeRangeName(nuiUnicodeRange range);

enum nuiUnicodeDirection
{
  eLeftToRight,
  eRightToLeft
};

nuiUnicodeDirection nuiGetUnicodeDirection(nglChar ch);



bool nuiIsUnicodeBlank(nglChar ch);

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

