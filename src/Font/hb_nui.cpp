/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "ucdata.h"
#include "hb-private.hh"
#include "hb-unicode-private.hh"
#include "nuiUnicode.h"
#include "hb_nui.h"


//HB_BEGIN_DECLS

static unsigned int
hb_nui_get_combining_class (hb_unicode_funcs_t *ufuncs,
                            hb_codepoint_t      unicode,
                            void               *user_data)

{
  return uccombining_class (unicode);
}

static unsigned int
hb_nui_get_eastasian_width (hb_unicode_funcs_t *ufuncs,
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


static hb_unicode_general_category_t
hb_nui_get_general_category (hb_unicode_funcs_t *ufuncs,
                             hb_codepoint_t      unicode,
                             void               *user_data)
{
  switch (ucprop_lookup(unicode, 0))
  {
    case UC_CN:			return HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
      
    case UC_LU:		return HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
    case UC_LL:		return HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
    case UC_LT:		return HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
    case UC_LM:		return HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
    case UC_LO:			return HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;
      
    case UC_MN:		return HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
    case UC_ME:		return HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
    case UC_MC:	return HB_UNICODE_GENERAL_CATEGORY_COMBINING_MARK;
      
    case UC_ND:		return HB_UNICODE_GENERAL_CATEGORY_DECIMAL_NUMBER;
    case UC_NL:			return HB_UNICODE_GENERAL_CATEGORY_LETTER_NUMBER;
    case UC_NO:			return HB_UNICODE_GENERAL_CATEGORY_OTHER_NUMBER;
      
    case UC_ZS:		return HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
    case UC_ZL:		return HB_UNICODE_GENERAL_CATEGORY_LINE_SEPARATOR;
    case UC_ZP:		return HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;
      
    case UC_CC:			return HB_UNICODE_GENERAL_CATEGORY_CONTROL;
    case UC_CF:			return HB_UNICODE_GENERAL_CATEGORY_FORMAT;
    case UC_CO:		return HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
    case UC_OS:			return HB_UNICODE_GENERAL_CATEGORY_SURROGATE;
      
      
    case UC_PD:		return HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
    case UC_PS:		return HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
    case UC_PE:		return HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
    case UC_PC:		return HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
    case UC_PO:		return HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;
      
    case UC_SM:			return HB_UNICODE_GENERAL_CATEGORY_MATH_SYMBOL;
    case UC_SC:		return HB_UNICODE_GENERAL_CATEGORY_CURRENCY_SYMBOL;
    case UC_SK:		return HB_UNICODE_GENERAL_CATEGORY_MODIFIER_SYMBOL;
    case UC_SO:			return HB_UNICODE_GENERAL_CATEGORY_OTHER_SYMBOL;
      
//    case UC_PI:		return HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
//    case UC_PF:		return HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }
  
  return HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

static hb_codepoint_t
hb_nui_get_mirroring (hb_unicode_funcs_t *ufuncs,
                      hb_codepoint_t      unicode,
                      void               *user_data)
{
  return ucismirroring(unicode);
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
hb_nui_get_script (hb_unicode_funcs_t *ufuncs,
                   hb_codepoint_t      unicode,
                   void               *user_data)
{
  nuiUnicodeScript script = nuiGetUnicodeScript(unicode);
  return hb_get_script_from_nui(script);
}

static hb_unicode_funcs_t nui_ufuncs = {
  HB_REFERENCE_COUNT_INVALID, /* ref_count */
  NULL, /* parent */
  TRUE, /* immutable */
  {
    hb_nui_get_combining_class,
    hb_nui_get_eastasian_width,
    hb_nui_get_general_category,
    hb_nui_get_mirroring,
    hb_nui_get_script
  }
};

hb_unicode_funcs_t *
hb_nui_get_unicode_funcs (void)
{
  return &nui_ufuncs;
}


//HB_END_DECLS
