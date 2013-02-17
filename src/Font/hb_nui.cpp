/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "hb.h"
#include "hb_nui.h"
#include "ucdata.h"

//#include "hb-unicode-private.hh"

//#include "hb-private.hh"

//#include "hb-unicode.h"
#include "hb-object-private.hh"
//#include <intrin.h>


static unsigned int
nui_hb_get_combining_class (hb_unicode_funcs_t *ufuncs,
                            hb_codepoint_t      unicode,
                            void               *user_data)

{
  return uccombining_class (unicode);
}

static unsigned int
nui_hb_get_eastasian_width (hb_unicode_funcs_t *ufuncs,
                            hb_codepoint_t      unicode,
                            void               *user_data)
{
  // Not implemented, consider all code points as narrow.
//  switch (u_getIntPropertyValue(unicode, UCHAR_EAST_ASIAN_WIDTH))
//  {
//    case U_EA_WIDE:
//    case U_EA_FULLWIDTH:
//      return 2;
//    case U_EA_NEUTRAL:
//    case U_EA_AMBIGUOUS:
//    case U_EA_HALFWIDTH:
//    case U_EA_NARROW:
//      return 1;
//  }
  return 1;
}

int32_t ucprop_lookup(uint32_t code, uint32_t n);

static const char* HB_UnicodeCats[] = 
{
  "CONTROL,             Cc",
  "FORMAT,              Cf",
  "UNASSIGNED,          Cn",
  "PRIVATE_USE,         Co",
  "SURROGATE,           Cs",
  "LOWERCASE_LETTER,    Ll",
  "MODIFIER_LETTER,     Lm",
  "OTHER_LETTER,        Lo",
  "TITLECASE_LETTER,    Lt",
  "UPPERCASE_LETTER,    Lu",
  "COMBINING_MARK,      Mc",
  "ENCLOSING_MARK,      Me",
  "NON_SPACING_MARK,    Mn",
  "DECIMAL_NUMBER,      Nd",
  "LETTER_NUMBER,       Nl",
  "OTHER_NUMBER,        No",
  "CONNECT_PUNCTUATION,	Pc",
  "DASH_PUNCTUATION,    Pd",
  "CLOSE_PUNCTUATION,   Pe",
  "FINAL_PUNCTUATION,   Pf",
  "INITIAL_PUNCTUATION,	Pi",
  "OTHER_PUNCTUATION,   Po",
  "OPEN_PUNCTUATION,    Ps",
  "CURRENCY_SYMBOL,     Sc",
  "MODIFIER_SYMBOL,     Sk",
  "MATH_SYMBOL,         Sm",
  "OTHER_SYMBOL,        So",
  "LINE_SEPARATOR,      Zl",
  "PARAGRAPH_SEPARATOR,	Zp",
  "SPACE_SEPARATOR,     Zs"
};

static hb_unicode_general_category_t
nui_hb_get_general_category (hb_unicode_funcs_t *ufuncs,
                             hb_codepoint_t      unicode,
                             void               *user_data)
{
  hb_unicode_general_category_t res = HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
#define TEST(A, B) if (ucprop_lookup(unicode, A)) res = B;
  
 
  TEST(UC_CN_I, HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED);
      
  TEST(UC_LU_I, HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER);
  TEST(UC_LL_I, HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER);
  TEST(UC_LT_I, HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER);
  TEST(UC_LM_I, HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER);
  TEST(UC_LO_I, HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
      
  TEST(UC_MN_I, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  TEST(UC_ME_I, HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK);
  //TEST(UC_MC_I, HB_UNICODE_GENERAL_CATEGORY_COMBINING_MARK);
  TEST(UC_MC_I, HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK);
  
  TEST(UC_ND_I, HB_UNICODE_GENERAL_CATEGORY_DECIMAL_NUMBER);
  TEST(UC_NL_I, HB_UNICODE_GENERAL_CATEGORY_LETTER_NUMBER);
  TEST(UC_NO_I, HB_UNICODE_GENERAL_CATEGORY_OTHER_NUMBER);
      
  TEST(UC_ZS_I, HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR);
  TEST(UC_ZL_I, HB_UNICODE_GENERAL_CATEGORY_LINE_SEPARATOR);
  TEST(UC_ZP_I, HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR);
      
  TEST(UC_CC_I, HB_UNICODE_GENERAL_CATEGORY_CONTROL);
  TEST(UC_CF_I, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
  TEST(UC_CO_I, HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE);
  TEST(UC_OS_I, HB_UNICODE_GENERAL_CATEGORY_SURROGATE);
      
      
  TEST(UC_PD_I, HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION);
  TEST(UC_PS_I, HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION);
  TEST(UC_PE_I, HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION);
  TEST(UC_PC_I, HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION);
  TEST(UC_PO_I, HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION);
      
  TEST(UC_SM_I, HB_UNICODE_GENERAL_CATEGORY_MATH_SYMBOL);
  TEST(UC_SC_I, HB_UNICODE_GENERAL_CATEGORY_CURRENCY_SYMBOL);
  TEST(UC_SK_I, HB_UNICODE_GENERAL_CATEGORY_MODIFIER_SYMBOL);
  TEST(UC_SO_I, HB_UNICODE_GENERAL_CATEGORY_OTHER_SYMBOL);
      
  TEST(UC_PI_I, HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION);
  TEST(UC_PF_I, HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION);

#undef TEST

  //printf("category %x -> %s\n", unicode, HB_UnicodeCats[res]);
  return res;
}

static hb_codepoint_t
nui_hb_get_mirroring (hb_unicode_funcs_t *ufuncs,
                      hb_codepoint_t      unicode,
                      void               *user_data)
{
  return nuiGetMirrorringChar(unicode);
}

hb_script_t hb_get_script_from_nui(nuiUnicodeScript script)
{
  switch (script)
  {
    case eScriptCommon:
      return HB_SCRIPT_COMMON;
    case eScriptLatin:
      return HB_SCRIPT_LATIN;
    case eScriptArabic:
      return HB_SCRIPT_ARABIC;
    case eScriptArmenian:
      return HB_SCRIPT_ARMENIAN;
    case eScriptBalinese:
      return HB_SCRIPT_BALINESE;
    case eScriptBengali:
      return HB_SCRIPT_BENGALI;
    case eScriptBopomofo:
      return HB_SCRIPT_BOPOMOFO;
    case eScriptBraille:
      return HB_SCRIPT_BRAILLE;
    case eScriptBuginese:
      return HB_SCRIPT_BUGINESE;
    case eScriptBuhid:
      return HB_SCRIPT_BUHID;
    case eScriptCanadian_Aboriginal:
      return HB_SCRIPT_CANADIAN_ABORIGINAL;
    case eScriptCarian:
      return HB_SCRIPT_CARIAN;
    case eScriptCham:
      return HB_SCRIPT_CHAM;
    case eScriptCherokee:
      return HB_SCRIPT_CHEROKEE;
    case eScriptCoptic:
      return HB_SCRIPT_COPTIC;
    case eScriptCuneiform:
      return HB_SCRIPT_CUNEIFORM;
    case eScriptCypriot:
      return HB_SCRIPT_CYPRIOT;
    case eScriptCyrillic:
      return HB_SCRIPT_CYRILLIC;
    case eScriptDeseret:
      return HB_SCRIPT_DESERET;
    case eScriptDevanagari:
      return HB_SCRIPT_DEVANAGARI;
    case eScriptEthiopic:
      return HB_SCRIPT_ETHIOPIC;
    case eScriptGeorgian:
      return HB_SCRIPT_GEORGIAN;
    case eScriptGlagolitic:
      return HB_SCRIPT_GLAGOLITIC;
    case eScriptGothic:
      return HB_SCRIPT_GOTHIC;
    case eScriptGreek:
      return HB_SCRIPT_GREEK;
    case eScriptGujarati:
      return HB_SCRIPT_GUJARATI;
    case eScriptGurmukhi:
      return HB_SCRIPT_GURMUKHI;
    case eScriptHan:
      return HB_SCRIPT_HAN;
    case eScriptHangul:
      return HB_SCRIPT_HANGUL;
    case eScriptHanunoo:
      return HB_SCRIPT_HANUNOO;
    case eScriptHebrew:
      return HB_SCRIPT_HEBREW;
    case eScriptHiragana:
      return HB_SCRIPT_HIRAGANA;
    case eScriptInherited:
      return HB_SCRIPT_INHERITED;
    case eScriptKannada:
      return HB_SCRIPT_KANNADA;
    case eScriptKatakana:
      return HB_SCRIPT_KATAKANA;
    case eScriptKayah_Li:
      return HB_SCRIPT_KAYAH_LI;
    case eScriptKharoshthi:
      return HB_SCRIPT_KHAROSHTHI;
    case eScriptKhmer:
      return HB_SCRIPT_KHMER;
    case eScriptLao:
      return HB_SCRIPT_LAO;
    case eScriptLepcha:
      return HB_SCRIPT_LEPCHA;
    case eScriptLimbu:
      return HB_SCRIPT_LIMBU;
    case eScriptLinear_B:
      return HB_SCRIPT_LINEAR_B;
    case eScriptLycian:
      return HB_SCRIPT_LYCIAN;
    case eScriptLydian:
      return HB_SCRIPT_LYDIAN;
    case eScriptMalayalam:
      return HB_SCRIPT_MALAYALAM;
    case eScriptMongolian:
      return HB_SCRIPT_MONGOLIAN;
    case eScriptMyanmar:
      return HB_SCRIPT_MYANMAR;
    case eScriptNew_Tai_Lue:
      return HB_SCRIPT_TAI_LE;
    case eScriptNko:
      return HB_SCRIPT_NKO;
    case eScriptOgham:
      return HB_SCRIPT_OGHAM;
    case eScriptOl_Chiki:
      return HB_SCRIPT_OL_CHIKI;
    case eScriptOld_Italic:
      return HB_SCRIPT_OLD_ITALIC;
    case eScriptOld_Persian:
      return HB_SCRIPT_OLD_PERSIAN;
    case eScriptOriya:
      return HB_SCRIPT_ORIYA;
    case eScriptOsmanya:
      return HB_SCRIPT_OSMANYA;
    case eScriptPhags_Pa:
      return HB_SCRIPT_PHAGS_PA;
    case eScriptPhoenician:
      return HB_SCRIPT_PHOENICIAN;
    case eScriptRejang:
      return HB_SCRIPT_REJANG;
    case eScriptRunic:
      return HB_SCRIPT_RUNIC;
    case eScriptSaurashtra:
      return HB_SCRIPT_SAURASHTRA;
    case eScriptShavian:
      return HB_SCRIPT_SHAVIAN;
    case eScriptSinhala:
      return HB_SCRIPT_SINHALA;
    case eScriptSundanese:
      return HB_SCRIPT_SUNDANESE;
    case eScriptSyloti_Nagri:
      return HB_SCRIPT_SYLOTI_NAGRI;
    case eScriptSyriac:
      return HB_SCRIPT_SYRIAC;
    case eScriptTagalog:
      return HB_SCRIPT_TAGALOG;
    case eScriptTagbanwa:
      return HB_SCRIPT_TAGBANWA;
    case eScriptTai_Le:
      return HB_SCRIPT_TAI_LE;
    case eScriptTamil:
      return HB_SCRIPT_TAMIL;
    case eScriptTelugu:
      return HB_SCRIPT_TELUGU;
    case eScriptThaana:
      return HB_SCRIPT_THAANA;
    case eScriptThai:
      return HB_SCRIPT_THAI;
    case eScriptTibetan:
      return HB_SCRIPT_TIBETAN;
    case eScriptTifinagh:
      return HB_SCRIPT_TIFINAGH;
    case eScriptUgaritic:
      return HB_SCRIPT_UGARITIC;
    case eScriptVai:
      return HB_SCRIPT_VAI;
    case eScriptYi:
      return HB_SCRIPT_YI;
  }
  return HB_SCRIPT_UNKNOWN;
}

static hb_script_t
nui_hb_get_script (hb_unicode_funcs_t *ufuncs,
                   hb_codepoint_t      unicode,
                   void               *user_data)
{
  nuiUnicodeScript script = nuiGetUnicodeScript(unicode);
  return hb_get_script_from_nui(script);
}

static hb_bool_t
nui_hb_get_compose (hb_unicode_funcs_t *ufuncs,
                        hb_codepoint_t      a,
                        hb_codepoint_t      b,
                        hb_codepoint_t     *ab,
                        void               *user_data)
{
  nglString str;
  str.Append((nglUChar)a);
  str.Append((nglUChar)b);
  str.ToCanonicalComposition();
  if (str.GetULength() == 1)
  {
    *ab = str.GetUChar(0);
    return TRUE;
  }
  return FALSE;
}

static hb_bool_t
nui_hb_get_decompose (hb_unicode_funcs_t *ufuncs,
                          hb_codepoint_t      ab,
                          hb_codepoint_t     *a,
                          hb_codepoint_t     *b,
                          void               *user_data)
{
  nglString str;
  str.Append((nglUChar)ab);
  str.ToCanonicalDecomposition();
  if (str.GetULength() == 2)
  {
    *a = str.GetUChar(0);
    *b = str.GetUChar(1);
    return TRUE;
  }
  return FALSE;
}

hb_unicode_funcs_t *
nui_hb_get_unicode_funcs (void)
{
  static hb_unicode_funcs_t* pFunc = NULL;
  if (!pFunc)
  {
    pFunc = hb_unicode_funcs_get_empty();
    hb_unicode_funcs_set_combining_class_func(pFunc, nui_hb_get_combining_class, NULL, NULL);
    hb_unicode_funcs_set_eastasian_width_func(pFunc, nui_hb_get_eastasian_width, NULL, NULL);
    hb_unicode_funcs_set_general_category_func(pFunc, nui_hb_get_general_category, NULL, NULL);
    hb_unicode_funcs_set_mirroring_func(pFunc, nui_hb_get_mirroring, NULL, NULL);
    hb_unicode_funcs_set_script_func(pFunc, nui_hb_get_script, NULL, NULL);
    hb_unicode_funcs_set_compose_func(pFunc, nui_hb_get_compose, NULL, NULL);
    hb_unicode_funcs_set_decompose_func(pFunc, nui_hb_get_decompose, NULL, NULL);
  }
  return pFunc;
}

