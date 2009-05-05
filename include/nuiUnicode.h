/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

enum nuiUnicodeRange
{
  eNone,
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
nuiUnicodeRange nuiGetUnicodeRange(nglChar ch, uint32& rLow, uint32& rHigh);

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
  
  uint32 mLength; // count of unicode code points
  int32 mDirection; // even: Left to right, odd: right to left
  nuiUnicodeRange mScript; // What script if this range of text
  bool mBlank; // Does this range contains strictly blank (space, tab, return, etc.) code points.
};

typedef std::list<nuiTextRange> nuiTextRangeList;
typedef uint32 nuiSplitTextFlag;

const nuiSplitTextFlag nuiST_ScriptChange    = 1 << 0;
const nuiSplitTextFlag nuiST_WordBoundary    = 1 << 1;
const nuiSplitTextFlag nuiST_DirectionChange = 1 << 2;
const nuiSplitTextFlag nuiST_All    = nuiST_ScriptChange | nuiST_WordBoundary | nuiST_DirectionChange;

bool nuiSplitText(const nglString& rSourceString, nuiTextRangeList& rRanges, nuiSplitTextFlag flags = nuiST_All);

