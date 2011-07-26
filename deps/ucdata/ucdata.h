/*
 * Copyright 2001 Computing Research Labs, New Mexico State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COMPUTING RESEARCH LAB OR NEW MEXICO STATE UNIVERSITY BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef _h_ucdata
#define _h_ucdata

/*
 * $Id: ucdata.h,v 1.6 2001/01/02 18:46:20 mleisher Exp $
 */

#if defined _MSC_VER
  #if (!defined HB_COMMON_H)
    typedef signed char int8_t;
    typedef unsigned char uint8_t;
    typedef signed short int16_t;
    typedef unsigned short uint16_t;
    typedef signed int int32_t;
    typedef unsigned int uint32_t;
    typedef signed long long int64_t;
    typedef unsigned long long uint64_t;
  #endif
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#undef __
#if (defined __STDC__) || (defined WIN32)
#define __(x) x
#else
#define __(x) ()
#endif

#define UCDATA_VERSION "2.4"

/**************************************************************************
 *
 * Masks and macros for character properties.
 *
 **************************************************************************/

/*
 * Values that can appear in the `mask1' parameter of the ucisprop()
 * function.
 */
#define UC_MN 0x00000001 /* Mark, Non-Spacing          */
#define UC_MC 0x00000002 /* Mark, Spacing Combining    */
#define UC_ME 0x00000004 /* Mark, Enclosing            */
#define UC_ND 0x00000008 /* Number, Decimal Digit      */
#define UC_NL 0x00000010 /* Number, Letter             */
#define UC_NO 0x00000020 /* Number, Other              */
#define UC_ZS 0x00000040 /* Separator, Space           */
#define UC_ZL 0x00000080 /* Separator, Line            */
#define UC_ZP 0x00000100 /* Separator, Paragraph       */
#define UC_CC 0x00000200 /* Other, Control             */
#define UC_CF 0x00000400 /* Other, Format              */
#define UC_OS 0x00000800 /* Other, Surrogate           */
#define UC_CO 0x00001000 /* Other, Private Use         */
#define UC_CN 0x00002000 /* Other, Not Assigned        */
#define UC_LU 0x00004000 /* Letter, Uppercase          */
#define UC_LL 0x00008000 /* Letter, Lowercase          */
#define UC_LT 0x00010000 /* Letter, Titlecase          */
#define UC_LM 0x00020000 /* Letter, Modifier           */
#define UC_LO 0x00040000 /* Letter, Other              */
#define UC_PC 0x00080000 /* Punctuation, Connector     */
#define UC_PD 0x00100000 /* Punctuation, Dash          */
#define UC_PS 0x00200000 /* Punctuation, Open          */
#define UC_PE 0x00400000 /* Punctuation, Close         */
#define UC_PO 0x00800000 /* Punctuation, Other         */
#define UC_SM 0x01000000 /* Symbol, Math               */
#define UC_SC 0x02000000 /* Symbol, Currency           */
#define UC_SK 0x04000000 /* Symbol, Modifier           */
#define UC_SO 0x08000000 /* Symbol, Other              */
#define UC_L  0x10000000 /* Left-To-Right              */
#define UC_R  0x20000000 /* Right-To-Left              */
#define UC_EN 0x40000000 /* European Number            */
#define UC_ES 0x80000000 /* European Number Separator  */

/*
 * Values that can appear in the `mask2' parameter of the ucisprop()
 * function.
 */
#define UC_ET 0x00000001 /* European Number Terminator */
#define UC_AN 0x00000002 /* Arabic Number              */
#define UC_CS 0x00000004 /* Common Number Separator    */
#define UC_B  0x00000008 /* Block Separator            */
#define UC_S  0x00000010 /* Segment Separator          */
#define UC_WS 0x00000020 /* Whitespace                 */
#define UC_ON 0x00000040 /* Other Neutrals             */
/*
 * Implementation specific character properties.
 */
#define UC_CM 0x00000080 /* Composite                  */
#define UC_NB 0x00000100 /* Non-Breaking               */
#define UC_SY 0x00000200 /* Symmetric                  */
#define UC_HD 0x00000400 /* Hex Digit                  */
#define UC_QM 0x00000800 /* Quote Mark                 */
#define UC_MR 0x00001000 /* Mirroring                  */
#define UC_SS 0x00002000 /* Space, other               */

#define UC_CP 0x00004000 /* Defined                    */

/*
 * Added for UnicodeData-2.1.3.
 */
#define UC_PI 0x00008000 /* Punctuation, Initial       */
#define UC_PF 0x00010000 /* Punctuation, Final         */

/*
 * Property indices for use in ucprop_lookup
 * 
 */
#define UC_MN_I 0 /* Mark, Non-Spacing          */
#define UC_MC_I 1 /* Mark, Spacing Combining    */
#define UC_ME_I 2 /* Mark, Enclosing            */
#define UC_ND_I 3 /* Number, Decimal Digit      */
#define UC_NL_I 4 /* Number, Letter             */
#define UC_NO_I 5 /* Number, Other              */
#define UC_ZS_I 6 /* Separator, Space           */
#define UC_ZL_I 7 /* Separator, Line            */
#define UC_ZP_I 8 /* Separator, Paragraph       */
#define UC_CC_I 9 /* Other, Control             */
#define UC_CF_I 10 /* Other, Format              */
#define UC_OS_I 11 /* Other, Surrogate           */
#define UC_CO_I 12 /* Other, Private Use         */
#define UC_CN_I 13 /* Other, Not Assigned        */
#define UC_LU_I 14 /* Letter, Uppercase          */
#define UC_LL_I 15 /* Letter, Lowercase          */
#define UC_LT_I 16 /* Letter, Titlecase          */
#define UC_LM_I 17 /* Letter, Modifier           */
#define UC_LO_I 18 /* Letter, Other              */
#define UC_PC_I 19 /* Punctuation, Connector     */
#define UC_PD_I 20 /* Punctuation, Dash          */
#define UC_PS_I 21 /* Punctuation, Open          */
#define UC_PE_I 22 /* Punctuation, Close         */
#define UC_PO_I 23 /* Punctuation, Other         */
#define UC_SM_I 24 /* Symbol, Math               */
#define UC_SC_I 25 /* Symbol, Currency           */
#define UC_SK_I 26 /* Symbol, Modifier           */
#define UC_SO_I 27 /* Symbol, Other              */
#define UC_L_I  28 /* Left-To-Right              */
#define UC_R_I  29 /* Right-To-Left              */
#define UC_EN_I 30 /* European Number            */
#define UC_ES_I 31 /* European Number Separator  */
  
  /*
   * Values that can appear in the `mask2' parameter of the ucisprop()
   * function.
   */
#define UC_ET_I 32 /* European Number Terminator */
#define UC_AN_I 33 /* Arabic Number              */
#define UC_CS_I 34 /* Common Number Separator    */
#define UC_B_I  35 /* Block Separator            */
#define UC_S_I  36 /* Segment Separator          */
#define UC_WS_I 37 /* Whitespace                 */
#define UC_ON_I 38 /* Other Neutrals             */
  /*
   * Implementation specific character properties.
   */
#define UC_CM_I 39 /* Composite                  */
#define UC_NB_I 40 /* Non-Breaking               */
#define UC_SY_I 41 /* Symmetric                  */
#define UC_HD_I 42 /* Hex Digit                  */
#define UC_QM_I 43 /* Quote Mark                 */
#define UC_MR_I 44 /* Mirroring                  */
#define UC_SS_I 45 /* Space, other               */
  
#define UC_CP_I 46 /* Defined                    */
  
  /*
   * Added for UnicodeData-2.1.3.
   */
#define UC_PI_I 47 /* Punctuation, Initial       */
#define UC_PF_I 48 /* Punctuation, Final         */

  
/*
 * This is the primary function for testing to see if a character has some set
 * of properties.  The macros that test for various character properties all
 * call this function with some set of masks.
 */
extern int32_t ucisprop __((uint32_t code, uint32_t mask1,
                        uint32_t mask2));

extern int32_t ucprop_lookup __((uint32_t code, uint32_t n));

extern void ucprop_get __((uint32_t code, uint32_t* mask1, uint32_t* mask2));
  
  
#define ucisalpha(cc) ucisprop(cc, UC_LU|UC_LL|UC_LM|UC_LO|UC_LT, 0)
#define ucisdigit(cc) ucisprop(cc, UC_ND, 0)
#define ucisalnum(cc) ucisprop(cc, UC_LU|UC_LL|UC_LM|UC_LO|UC_LT|UC_ND, 0)
#define uciscntrl(cc) ucisprop(cc, UC_CC|UC_CF, 0)
#define ucisspace(cc) ucisprop(cc, UC_ZS|UC_SS, 0)
#define ucisblank(cc) ucisprop(cc, UC_ZS, 0)
#define ucispunct(cc) ucisprop(cc, UC_PD|UC_PS|UC_PE|UC_PO, UC_PI|UC_PF)
#define ucisgraph(cc) ucisprop(cc, UC_MN|UC_MC|UC_ME|UC_ND|UC_NL|UC_NO|\
                               UC_LU|UC_LL|UC_LT|UC_LM|UC_LO|UC_PC|UC_PD|\
                               UC_PS|UC_PE|UC_PO|UC_SM|UC_SM|UC_SC|UC_SK|\
                               UC_SO, UC_PI|UC_PF)
#define ucisprint(cc) ucisprop(cc, UC_MN|UC_MC|UC_ME|UC_ND|UC_NL|UC_NO|\
                               UC_LU|UC_LL|UC_LT|UC_LM|UC_LO|UC_PC|UC_PD|\
                               UC_PS|UC_PE|UC_PO|UC_SM|UC_SM|UC_SC|UC_SK|\
                               UC_SO|UC_ZS, UC_PI|UC_PF)
#define ucisupper(cc) ucisprop(cc, UC_LU, 0)
#define ucislower(cc) ucisprop(cc, UC_LL, 0)
#define ucistitle(cc) ucisprop(cc, UC_LT, 0)
#define ucisxdigit(cc) ucisprop(cc, 0, UC_HD)

#define ucisisocntrl(cc) ucisprop(cc, UC_CC, 0)
#define ucisfmtcntrl(cc) ucisprop(cc, UC_CF, 0)

#define ucissymbol(cc) ucisprop(cc, UC_SM|UC_SC|UC_SO|UC_SK, 0)
#define ucisnumber(cc) ucisprop(cc, UC_ND|UC_NO|UC_NL, 0)
#define ucisnonspacing(cc) ucisprop(cc, UC_MN, 0)
#define ucisopenpunct(cc) ucisprop(cc, UC_PS, 0)
#define ucisclosepunct(cc) ucisprop(cc, UC_PE, 0)
#define ucisinitialpunct(cc) ucisprop(cc, 0, UC_PI)
#define ucisfinalpunct(cc) ucisprop(cc, 0, UC_PF)

#define uciscomposite(cc) ucisprop(cc, 0, UC_CM)
#define ucishex(cc) ucisprop(cc, 0, UC_HD)
#define ucisquote(cc) ucisprop(cc, 0, UC_QM)
#define ucissymmetric(cc) ucisprop(cc, 0, UC_SY)
#define ucismirroring(cc) ucisprop(cc, 0, UC_MR)
#define ucisnonbreaking(cc) ucisprop(cc, 0, UC_NB)

/*
 * Directionality macros.
 */
#define ucisrtl(cc) ucisprop(cc, UC_R, 0)
#define ucisltr(cc) ucisprop(cc, UC_L, 0)
#define ucisstrong(cc) ucisprop(cc, UC_L|UC_R, 0)
#define ucisweak(cc) ucisprop(cc, UC_EN|UC_ES, UC_ET|UC_AN|UC_CS)
#define ucisneutral(cc) ucisprop(cc, 0, UC_B|UC_S|UC_WS|UC_ON)
#define ucisseparator(cc) ucisprop(cc, 0, UC_B|UC_S)

/*
 * Other macros inspired by John Cowan.
 */
#define ucismark(cc) ucisprop(cc, UC_MN|UC_MC|UC_ME, 0)
#define ucismodif(cc) ucisprop(cc, UC_LM, 0)
#define ucisletnum(cc) ucisprop(cc, UC_NL, 0)
#define ucisconnect(cc) ucisprop(cc, UC_PC, 0)
#define ucisdash(cc) ucisprop(cc, UC_PD, 0)
#define ucismath(cc) ucisprop(cc, UC_SM, 0)
#define uciscurrency(cc) ucisprop(cc, UC_SC, 0)
#define ucismodifsymbol(cc) ucisprop(cc, UC_SK, 0)
#define ucisnsmark(cc) ucisprop(cc, UC_MN, 0)
#define ucisspmark(cc) ucisprop(cc, UC_MC, 0)
#define ucisenclosing(cc) ucisprop(cc, UC_ME, 0)
#define ucisprivate(cc) ucisprop(cc, UC_CO, 0)
#define ucissurrogate(cc) ucisprop(cc, UC_OS, 0)
#define ucislsep(cc) ucisprop(cc, UC_ZL, 0)
#define ucispsep(cc) ucisprop(cc, UC_ZP, 0)

#define ucisidentstart(cc) ucisprop(cc, UC_LU|UC_LL|UC_LT|UC_LO|UC_NL, 0)
#define ucisidentpart(cc) ucisprop(cc, UC_LU|UC_LL|UC_LT|UC_LO|UC_NL|\
                                   UC_MN|UC_MC|UC_ND|UC_PC|UC_CF, 0)

#define ucisdefined(cc) ucisprop(cc, 0, UC_CP)
#define ucisundefined(cc) !ucisprop(cc, 0, UC_CP)

/*
 * Other miscellaneous character property macros.
 */
#define ucishan(cc) (((cc) >= 0x4e00 && (cc) <= 0x9fff) ||\
                     ((cc) >= 0xf900 && (cc) <= 0xfaff))
#define ucishangul(cc) ((cc) >= 0xac00 && (cc) <= 0xd7ff)

/**************************************************************************
 *
 * Functions for case conversion.
 *
 **************************************************************************/

extern uint32_t uctoupper __((uint32_t code));
extern uint32_t uctolower __((uint32_t code));
extern uint32_t uctotitle __((uint32_t code));

/**************************************************************************
 *
 * Functions for getting compositions.
 *
 **************************************************************************/

/*
 * This routine determines if there exists a composition of node1 and node2.
 * If it returns 0, there is no composition.  Any other value indicates a
 * composition was returned in comp.
 */
extern int32_t uccomp __((uint32_t node1, uint32_t node2,
		      uint32_t *comp));

/*
 * Does Hangul composition on the string str with length len, and returns
 * the length of the composed string.
 */
extern int32_t uccomp_hangul __((uint32_t *str, int32_t len));

/*
 * Does canonical composition on the string str with length len, and returns
 * the length of the composed string.
 */
extern int32_t uccanoncomp __((uint32_t *str, int32_t len));

/**************************************************************************
 *
 * Functions for getting decompositions.
 *
 **************************************************************************/

/*
 * This routine determines if the code has a decomposition.  If it returns 0,
 * there is no decomposition.  Any other value indicates a decomposition was
 * returned.
 */
extern int32_t ucdecomp __((uint32_t code, uint32_t *num,
                        uint32_t **decomp));

/*
 * If the code is a Hangul syllable, this routine decomposes it into the array
 * passed.  The array size should be at least 3.
 */
extern int32_t ucdecomp_hangul __((uint32_t code, uint32_t *num,
                               uint32_t decomp[]));

/*  
 * This routine does canonical decomposition of the string in of length
 * inlen, and returns the decomposed string in out with length outlen.
 * The memory for out is allocated by this routine. It returns the length
 * of the decomposed string if okay, and -1 on error.
 */
extern int32_t uccanondecomp __((const uint32_t *in, int32_t inlen,
			     uint32_t **out, int32_t *outlen));
  
/**************************************************************************
 *
 * Functions for getting combining classes.
 *
 **************************************************************************/

/*
 * This will return the combining class for a character to be used with the
 * Canonical Ordering algorithm.
 */
extern uint32_t uccombining_class __((uint32_t code));

/**************************************************************************
 *
 * Functions for getting numbers and digits.
 *
 **************************************************************************/

struct ucnumber {
    int32_t numerator;
    int32_t denominator;
};

extern int32_t ucnumber_lookup __((uint32_t code, struct ucnumber *num));
extern int32_t ucdigit_lookup __((uint32_t code, int32_t *digit));

/*
 * For compatibility with John Cowan's "uctype" package.
 */
extern struct ucnumber ucgetnumber __((uint32_t code));
extern int32_t ucgetdigit __((uint32_t code));

/**************************************************************************
 *
 * Functions library initialization and cleanup.
 *
 **************************************************************************/

/*
 * Macros for specifying the data tables to be loaded, unloaded, or reloaded
 * by the ucdata_load(), ucdata_unload(), and ucdata_reload() routines.
 */
#define UCDATA_CASE   0x01
#define UCDATA_CTYPE  0x02
#define UCDATA_DECOMP 0x04
#define UCDATA_CMBCL  0x08
#define UCDATA_NUM    0x10
#define UCDATA_COMP   0x20

#define UCDATA_ALL (UCDATA_CASE|UCDATA_CTYPE|UCDATA_DECOMP|\
                    UCDATA_CMBCL|UCDATA_NUM|UCDATA_COMP)

/*
 * Functions to load, unload, and reload specific data files.
 */
extern int32_t ucdata_load __((char *paths, int32_t mask));
extern void ucdata_unload __((int32_t mask));
extern int32_t ucdata_reload __((char *paths, int32_t mask));

  
extern void ucdata_init_static __(());
/*
 * Deprecated functions, now just compatibility macros.
 */
#define ucdata_setup(p) ucdata_load(p, UCDATA_ALL)
#define ucdata_cleanup() ucdata_unload(UCDATA_ALL)

#undef __

#ifdef __cplusplus
}
#endif

#endif /* _h_ucdata */
