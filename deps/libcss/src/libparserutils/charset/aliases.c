/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <ctype.h>

#ifndef WIN32
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#else
typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef int bool;
#define false   (bool)0
#define true    (bool)1
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "charset/aliases.h"
#include "utils/utils.h"

static char* internal_fgets(char* buf, int n, FILE* fp);

struct alias {
	struct alias *next;
	parserutils_charset_aliases_canon *canon;
	uint16_t name_len;
	char name[1];
};

#define HASH_SIZE (43)
static parserutils_charset_aliases_canon *canon_tab[HASH_SIZE];
static struct alias *alias_tab[HASH_SIZE];

static parserutils_error parserutils_charset_create_alias(const char *alias,
		parserutils_charset_aliases_canon *c, 
		parserutils_alloc alloc, void *pw);
static parserutils_charset_aliases_canon *parserutils_charset_create_canon(
		const char *canon, uint16_t mibenum, 
		parserutils_alloc alloc, void *pw);
static int aliascmp(const char *s1, const char *s2, size_t s2_len);
static uint32_t parserutils_charset_hash_val(const char *alias, size_t len);

/**
 * Create alias data from Aliases file
 *
 * \param filename  The path to the Aliases file
 * \param alloc     Memory (de)allocation function
 * \param pw        Pointer to client-specific private data (may be NULL)
 * \return PARSERUTILS_OK on success, appropriate error otherwise.
 */
parserutils_error parserutils_charset_aliases_create(const char *filename,
		parserutils_alloc alloc, void *pw)
{
	char buf[300];
	FILE *fp;

	if (filename == NULL || alloc == NULL)
		return PARSERUTILS_BADPARM;

	fp = fopen(filename, "r");
  int index = 0;
  char* (*local_fgets)(char* buf, int n, FILE* fp) = fgets;
	if (fp == NULL)
  {
		//return PARSERUTILS_FILENOTFOUND;
    // Use the internal definitions
    fp = (FILE*)&index;
    local_fgets = internal_fgets;
  }

	while (local_fgets(buf, sizeof buf, fp))
  {
		char *p, *aliases = 0, *mib, *end;
		parserutils_charset_aliases_canon *cf;

		if (buf[0] == 0 || buf[0] == '#')
			/* skip blank lines or comments */
			continue;

		buf[strlen(buf) - 1] = 0; /* lose terminating newline */
		end = buf + strlen(buf);

		/* find end of canonical form */
		for (p = buf; *p && !isspace(*p) && !iscntrl(*p); p++)
			; /* do nothing */
		if (p >= end)
			continue;
		*p++ = '\0'; /* terminate canonical form */

		/* skip whitespace */
		for (; *p && isspace(*p); p++)
			; /* do nothing */
		if (p >= end)
			continue;
		mib = p;

		/* find end of mibenum */
		for (; *p && !isspace(*p) && !iscntrl(*p); p++)
			; /* do nothing */
		if (p < end)
			*p++ = '\0'; /* terminate mibenum */

		cf = parserutils_charset_create_canon(buf, atoi(mib), alloc, pw);
		if (cf == NULL)
			continue;

		/* skip whitespace */
		for (; p < end && *p && isspace(*p); p++)
			; /* do nothing */
		if (p >= end)
			continue;
		aliases = p;

		while (p < end)
    {
			/* find end of alias */
			for (; *p && !isspace(*p) && !iscntrl(*p); p++)
				; /* do nothing */
			if (p > end)
				/* stop if we've gone past the end */
				break;
			/* terminate current alias */
			*p++ = '\0';

			if (parserutils_charset_create_alias(aliases, cf,	alloc, pw) != PARSERUTILS_OK)
				break;

			/* in terminating, we may have advanced
			 * past the end - check this here */
			if (p >= end)
				break;

			/* skip whitespace */
			for (; *p && isspace(*p); p++)
				; /* do nothing */

			if (p >= end)
				/* gone past end => stop */
				break;

			/* update pointer to current alias */
			aliases = p;
		}
	}

  if (local_fgets != internal_fgets)
    fclose(fp);

	return PARSERUTILS_OK;
}

/**
 * Free all alias data
 *
 * \param alloc  Memory (de)allocation function
 * \param pw     Pointer to client-specific private data
 */
void parserutils_charset_aliases_destroy(parserutils_alloc alloc, void *pw)
{
	parserutils_charset_aliases_canon *c, *d;
	struct alias *a, *b;
	int i;

	for (i = 0; i != HASH_SIZE; i++) {
		for (c = canon_tab[i]; c; c = d) {
			d = c->next;
			alloc(c, 0, pw);
		}
		canon_tab[i] = NULL;

		for (a = alias_tab[i]; a; a = b) {
			b = a->next;
			alloc(a, 0, pw);
		}
		alias_tab[i] = NULL;
	}
}

/**
 * Retrieve the MIB enum value assigned to an encoding name
 *
 * \param alias  The alias to lookup
 * \param len    The length of the alias string
 * \return The MIB enum value, or 0 if not found
 */
uint16_t parserutils_charset_mibenum_from_name(const char *alias, size_t len)
{
	parserutils_charset_aliases_canon *c;

	if (alias == NULL)
		return 0;

	c = parserutils_charset_alias_canonicalise(alias, len);
	if (c == NULL)
		return 0;

	return c->mib_enum;
}

/**
 * Retrieve the canonical name of an encoding from the MIB enum
 *
 * \param mibenum The MIB enum value
 * \return Pointer to canonical name, or NULL if not found
 */
const char *parserutils_charset_mibenum_to_name(uint16_t mibenum)
{
	int i;
	parserutils_charset_aliases_canon *c;

	for (i = 0; i != HASH_SIZE; i++)
		for (c = canon_tab[i]; c; c = c->next)
			if (c->mib_enum == mibenum)
				return c->name;

	return NULL;
}

/**
 * Detect if a parserutils_charset is Unicode
 *
 * \param mibenum  The MIB enum to consider
 * \return true if a Unicode variant, false otherwise
 */
bool parserutils_charset_mibenum_is_unicode(uint16_t mibenum)
{
	static uint16_t ucs4;
	static uint16_t ucs2;
	static uint16_t utf8;
	static uint16_t utf16;
	static uint16_t utf16be;
	static uint16_t utf16le;
	static uint16_t utf32;
	static uint16_t utf32be;
	static uint16_t utf32le;

	if (ucs4 == 0) {
		ucs4 = parserutils_charset_mibenum_from_name("UCS-4", 
				SLEN("UCS-4"));
		ucs2 = parserutils_charset_mibenum_from_name("UCS-2", 
				SLEN("UCS-2"));
		utf8 = parserutils_charset_mibenum_from_name("UTF-8", 
				SLEN("UTF-8"));
		utf16 = parserutils_charset_mibenum_from_name("UTF-16", 
				SLEN("UTF-16"));
		utf16be = parserutils_charset_mibenum_from_name("UTF-16BE",
				SLEN("UTF-16BE"));
		utf16le = parserutils_charset_mibenum_from_name("UTF-16LE",
				SLEN("UTF-16LE"));
		utf32 = parserutils_charset_mibenum_from_name("UTF-32", 
				SLEN("UTF-32"));
		utf32be = parserutils_charset_mibenum_from_name("UTF-32BE",
				SLEN("UTF-32BE"));
		utf32le = parserutils_charset_mibenum_from_name("UTF-32LE",
				SLEN("UTF-32LE"));
	}

	return (mibenum == ucs4 || mibenum == ucs2 || mibenum == utf8 ||
			mibenum == utf16 || mibenum == utf16be || 
			mibenum == utf16le || mibenum == utf32 ||
			mibenum == utf32be || mibenum == utf32le);
}

#define IS_PUNCT_OR_SPACE(x) \
		((0x09 <= (x) && (x) <= 0x0D) || \
				(0x20 <= (x) && (x) <= 0x2F) || \
				(0x3A <= (x) && (x) <= 0x40) || \
				(0x5B <= (x) && (x) <= 0x60) || \
				(0x7B <= (x) && (x) <= 0x7E))


/**
 * Compare name "s1" to name "s2" (of size s2_len) case-insensitively
 * and ignoring ASCII punctuation characters.
 *
 * See http://www.whatwg.org/specs/web-apps/current-work/#character0
 *
 * \param s1		Alias to compare to
 * \param s2		Alias to compare
 * \param s2_len	Length of "s2"
 * \returns 0 if equal, 1 otherwise
 */
int aliascmp(const char *s1, const char *s2, size_t s2_len)
{
	size_t s2_pos = 0;

	if (s1 == NULL || s2_len == 0)
		return 1;

	while (true) {
		while (IS_PUNCT_OR_SPACE(*s1))
			s1++;
		while (s2_pos < s2_len &&
				IS_PUNCT_OR_SPACE(s2[s2_pos])) {
			s2_pos++;
		}

		if (s2_pos == s2_len)
			return (*s1 != '\0') ? 1 : 0;

		if (tolower(*s1) != tolower(s2[s2_pos]))
			break;
		s1++;
		s2_pos++;
	}

	return 1;
}


/**
 * Retrieve the canonical form of an alias name
 *
 * \param alias  The alias name
 * \param len    The length of the alias name
 * \return Pointer to canonical form or NULL if not found
 */
parserutils_charset_aliases_canon *parserutils_charset_alias_canonicalise(
		const char *alias, size_t len)
{
	uint32_t hash;
	parserutils_charset_aliases_canon *c;
	struct alias *a;

	if (alias == NULL)
		return NULL;

	hash = parserutils_charset_hash_val(alias, len);

	for (c = canon_tab[hash]; c; c = c->next)
		if (aliascmp(c->name, alias, len) == 0)
			break;
	if (c)
		return c;

	for (a = alias_tab[hash]; a; a = a->next)
		if (aliascmp(a->name, alias, len) == 0)
			break;
	if (a)
		return a->canon;

	return NULL;
}


/**
 * Create an alias
 *
 * \param alias  The alias name
 * \param c      The canonical form
 * \param alloc  Memory (de)allocation function
 * \param pw     Pointer to client-specific private data (may be NULL)
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error parserutils_charset_create_alias(const char *alias, 
		parserutils_charset_aliases_canon *c,
		parserutils_alloc alloc, void *pw)
{
	struct alias *a;
	uint32_t hash;

	if (alias == NULL || c == NULL || alloc == NULL)
		return PARSERUTILS_BADPARM;

	a = alloc(NULL, sizeof(struct alias) + strlen(alias) + 1, pw);
	if (a == NULL)
		return PARSERUTILS_NOMEM;

	a->canon = c;
	a->name_len = strlen(alias);
	strcpy(a->name, alias);
	a->name[a->name_len] = '\0';

	hash = parserutils_charset_hash_val(alias, a->name_len);

	a->next = alias_tab[hash];
	alias_tab[hash] = a;

	return PARSERUTILS_OK;
}

/**
 * Create a canonical form
 *
 * \param canon    The canonical name
 * \param mibenum  The MIB enum value
 * \param alloc    Memory (de)allocation function
 * \param pw       Pointer to client-specific private data (may be NULL)
 * \return Pointer to canonical form or NULL on error
 */
parserutils_charset_aliases_canon *parserutils_charset_create_canon(
		const char *canon, uint16_t mibenum, 
		parserutils_alloc alloc, void *pw)
{
	parserutils_charset_aliases_canon *c;
	uint32_t hash, len;

	if (canon == NULL || alloc == NULL)
		return NULL;

	len = strlen(canon);

	c = alloc(NULL, sizeof(parserutils_charset_aliases_canon) + len + 1, pw);
	if (c == NULL)
		return NULL;

	c->mib_enum = mibenum;
	c->name_len = len;
	strcpy(c->name, canon);
	c->name[len] = '\0';

	hash = parserutils_charset_hash_val(canon, len);

	c->next = canon_tab[hash];
	canon_tab[hash] = c;

	return c;
}

/**
 * Hash function
 *
 * \param alias  String to hash
 * \param len    Number of bytes to hash (<= strlen(alias))
 * \return The hashed value
 */
uint32_t parserutils_charset_hash_val(const char *alias, size_t len)
{
	const char *s = alias;
	uint32_t h = 5381;

	if (alias == NULL)
		return 0;

	while (len--) {
		if (IS_PUNCT_OR_SPACE(*s)) {
			s++;
		} else {
			h = (h * 33) ^ (*s++ & ~0x20); /* case insensitive */
		}
	}

	return h % HASH_SIZE;
}


#ifndef NDEBUG
/**
 * Dump all alias data to stdout
 */
void parserutils_charset_aliases_dump(void)
{
	parserutils_charset_aliases_canon *c;
	struct alias *a;
	int i;
	size_t size = 0;

	for (i = 0; i != HASH_SIZE; i++) {
		for (c = canon_tab[i]; c; c = c->next) {
			printf("%d %s\n", i, c->name);
			size += offsetof(parserutils_charset_aliases_canon, 
					name) + c->name_len;
		}

		for (a = alias_tab[i]; a; a = a->next) {
			printf("%d %s\n", i, a->name);
			size += offsetof(struct alias, name) + a->name_len;
		}
	}

	size += (sizeof(canon_tab) / sizeof(canon_tab[0]));
	size += (sizeof(alias_tab) / sizeof(alias_tab[0]));

	printf("%u\n", (unsigned int) size);
}
#endif







//////////////
static const char* sAliases[] =
{
  "US-ASCII		3		iso-ir-6 ANSI_X3.4-1986 ISO_646.irv:1991 ASCII ISO646-US ANSI_X3.4-1968 us IBM367 cp367 csASCII\n",
"ISO-10646-UTF-1		27		csISO10646UTF1\n",
"ISO_646.basic:1983	28		ref csISO646basic1983\n",
"INVARIANT		29		csINVARIANT\n",
"ISO_646.irv:1983	30		iso-ir-2 irv csISO2IntlRefVersion\n",
"BS_4730			20		iso-ir-4 ISO646-GB gb uk csISO4UnitedKingdom\n",
"NATS-SEFI		31		iso-ir-8-1 csNATSSEFI\n",
"NATS-SEFI-ADD		32		iso-ir-8-2 csNATSSEFIADD\n",
"NATS-DANO		33		iso-ir-9-1 csNATSDANO\n",
"NATS-DANO-ADD		34		iso-ir-9-2 csNATSDANOADD\n",
"SEN_850200_B		35		iso-ir-10 FI ISO646-FI ISO646-SE se csISO10Swedish\n",
"SEN_850200_C		21		iso-ir-11 ISO646-SE2 se2 csISO11SwedishForNames\n",
"KS_C_5601-1987		36		iso-ir-149 KS_C_5601-1989 KSC_5601 korean csKSC56011987\n",
"ISO-2022-KR		37		csISO2022KR\n",
"EUC-KR			38		csEUCKR EUCKR\n",
"ISO-2022-JP		39		csISO2022JP\n",
"ISO-2022-JP-2		40		csISO2022JP2\n",
"ISO-2022-CN		104\n",
"ISO-2022-CN-EXT		105\n",
"JIS_C6220-1969-jp	41		JIS_C6220-1969 iso-ir-13 katakana x0201-7 csISO13JISC6220jp\n",
"JIS_C6220-1969-ro	42		iso-ir-14 jp ISO646-JP csISO14JISC6220ro\n",
"IT			22		iso-ir-15 ISO646-IT csISO15Italian\n",
"PT			43		iso-ir-16 ISO646-PT csISO16Portuguese\n",
"ES			23		iso-ir-17 ISO646-ES csISO17Spanish\n",
"greek7-old		44		iso-ir-18 csISO18Greek7Old\n",
"latin-greek		45		iso-ir-19 csISO19LatinGreek\n",
"DIN_66003		24		iso-ir-21 de ISO646-DE csISO21German\n",
"NF_Z_62-010_(1973)	46		iso-ir-25 ISO646-FR1 csISO25French\n",
"Latin-greek-1		47		iso-ir-27 csISO27LatinGreek1\n",
"ISO_5427		48		iso-ir-37 csISO5427Cyrillic\n",
"JIS_C6226-1978		49		iso-ir-42 csISO42JISC62261978\n",
"BS_viewdata		50		iso-ir-47 csISO47BSViewdata\n",
"INIS			51		iso-ir-49 csISO49INIS\n",
"INIS-8			52		iso-ir-50 csISO50INIS8\n",
"INIS-cyrillic		53		iso-ir-51 csISO51INISCyrillic\n",
"ISO_5427:1981		54		iso-ir-54 ISO5427Cyrillic1981\n",
"ISO_5428:1980		55		iso-ir-55 csISO5428Greek\n",
"GB_1988-80		56		iso-ir-57 cn ISO646-CN csISO57GB1988\n",
"GB_2312-80		57		iso-ir-58 chinese csISO58GB231280\n",
"NS_4551-1		25		iso-ir-60 ISO646-NO no csISO60DanishNorwegian csISO60Norwegian1\n",
"NS_4551-2		58		ISO646-NO2 iso-ir-61 no2 csISO61Norwegian2\n",
"NF_Z_62-010		26		iso-ir-69 ISO646-FR fr csISO69French\n",
"videotex-suppl		59		iso-ir-70 csISO70VideotexSupp1\n",
"PT2			60		iso-ir-84 ISO646-PT2 csISO84Portuguese2\n",
"ES2			61		iso-ir-85 ISO646-ES2 csISO85Spanish2\n",
"MSZ_7795.3		62		iso-ir-86 ISO646-HU hu csISO86Hungarian\n",
"JIS_C6226-1983		63		iso-ir-87 x0208 JIS_X0208-1983 csISO87JISX0208\n",
"greek7			64		iso-ir-88 csISO88Greek7\n",
"ASMO_449		65		ISO_9036 arabic7 iso-ir-89 csISO89ASMO449\n",
"iso-ir-90		66		csISO90\n",
"JIS_C6229-1984-a	67		iso-ir-91 jp-ocr-a csISO91JISC62291984a\n",
"JIS_C6229-1984-b	68		iso-ir-92 ISO646-JP-OCR-B jp-ocr-b csISO92JISC62991984b\n",
"JIS_C6229-1984-b-add	69		iso-ir-93 jp-ocr-b-add csISO93JIS62291984badd\n",
"JIS_C6229-1984-hand	70		iso-ir-94 jp-ocr-hand csISO94JIS62291984hand\n",
"JIS_C6229-1984-hand-add	71		iso-ir-95 jp-ocr-hand-add csISO95JIS62291984handadd\n",
"JIS_C6229-1984-kana	72		iso-ir-96 csISO96JISC62291984kana\n",
"ISO_2033-1983		73		iso-ir-98 e13b csISO2033\n",
"ANSI_X3.110-1983	74		iso-ir-99 CSA_T500-1983 NAPLPS csISO99NAPLPS\n",
"ISO-8859-1		4		iso-ir-100 ISO_8859-1 ISO_8859-1:1987 latin1 l1 IBM819 CP819 csISOLatin1 8859_1 ISO8859-1\n",
"ISO-8859-2		5		iso-ir-101 ISO_8859-2 ISO_8859-2:1987 latin2 l2 csISOLatin2 8859_2 ISO8859-2\n",
"T.61-7bit		75		iso-ir-102 csISO102T617bit\n",
"T.61-8bit		76		T.61 iso-ir-103 csISO103T618bit\n",
"ISO-8859-3		6		iso-ir-109 ISO_8859-3 ISO_8859-3:1988 latin3 l3 csISOLatin3 8859_3 ISO8859-3\n",
"ISO-8859-4		7		iso-ir-110 ISO_8859-4 ISO_8859-4:1988 latin4 l4 csISOLatin4 8859_4 ISO8859-4\n",
"ECMA-cyrillic		77		iso-ir-111 KOI8-E csISO111ECMACyrillic\n",
"CSA_Z243.4-1985-1	78		iso-ir-121 ISO646-CA csa7-1 ca csISO121Canadian1\n",
"CSA_Z243.4-1985-2	79		iso-ir-122 ISO646-CA2 csa7-2 csISO122Canadian2\n",
"CSA_Z243.4-1985-gr	80		iso-ir-123 csISO123CSAZ24341985gr\n",
"ISO-8859-6		9		iso-ir-127 ISO_8859-6 ISO_8859-6:1987 ECMA-114 ASMO-708 arabic csISOLatinArabic\n",
"ISO-8859-6-E		81		csISO88596E ISO_8859-6-E\n",
"ISO-8859-6-I		82		csISO88596I ISO_8859-6-I\n",
"ISO-8859-7		10		iso-ir-126 ISO_8859-7 ISO_8859-7:1987 ELOT_928 ECMA-118 greek greek8 csISOLatinGreek 8859_7 ISO8859-7\n",
"T.101-G2		83		iso-ir-128 csISO128T101G2\n",
"ISO-8859-8		11		iso-ir-138 ISO_8859-8 ISO_8859-8:1988 hebrew csISOLatinHebrew 8859_8 ISO8859-8\n",
"ISO-8859-8-E		84		csISO88598E ISO_8859-8-E\n",
"ISO-8859-8-I		85		csISO88598I ISO_8859-8-I\n",
"CSN_369103		86		iso-ir-139 csISO139CSN369103\n",
"JUS_I.B1.002		87		iso-ir-141 ISO646-YU js yu csISO141JUSIB1002\n",
"ISO_6937-2-add		14		iso-ir-142 csISOTextComm\n",
"IEC_P27-1		88		iso-ir-143 csISO143IECP271\n",
"ISO-8859-5		8		iso-ir-144 ISO_8859-5 ISO_8859-5:1988 cyrillic csISOLatinCyrillic 8859_5 ISO8859-5\n",
"JUS_I.B1.003-serb	89		iso-ir-146 serbian csISO146Serbian\n",
"JUS_I.B1.003-mac	90		macedonian iso-ir-147 csISO147Macedonian\n",
"ISO-8859-9		12		iso-ir-148 ISO_8859-9 ISO_8859-9:1989 latin5 l5 csISOLatin5 8859_9 ISO8859-9\n",
"greek-ccitt		91		iso-ir-150 csISO150 csISO150GreekCCITT\n",
"NC_NC00-10:81		92		cuba iso-ir-151 ISO646-CU csISO151Cuba\n",
"ISO_6937-2-25		93		iso-ir-152 csISO6937Add\n",
"GOST_19768-74		94		ST_SEV_358-88 iso-ir-153 csISO153GOST1976874\n",
"ISO_8859-supp		95		iso-ir-154 latin1-2-5 csISO8859Supp\n",
"ISO_10367-box		96		iso-ir-155 csISO10367Box\n",
"ISO-8859-10		13		iso-ir-157 l6 ISO_8859-10:1992 csISOLatin6 latin6 8859_10 ISO8859-10\n",
"latin-lap		97		lap iso-ir-158 csISO158Lap\n",
"JIS_X0212-1990		98		x0212 iso-ir-159 csISO159JISX02121990\n",
"DS_2089			99		DS2089 ISO646-DK dk csISO646Danish\n",
"us-dk			100		csUSDK\n",
"dk-us			101		csDKUS\n",
"JIS_X0201		15		X0201 csHalfWidthKatakana\n",
"KSC5636			102		ISO646-KR csKSC5636\n",
"ISO-10646-UCS-2		1000		csUnicode UCS-2 UCS2\n",
"ISO-10646-UCS-4		1001		csUCS4 UCS-4 UCS4\n",
"DEC-MCS			2008		dec csDECMCS\n",
"hp-roman8		2004		roman8 r8 csHPRoman8\n",
"macintosh		2027		mac csMacintosh MACROMAN MAC-ROMAN X-MAC-ROMAN\n",
"IBM037			2028		cp037 ebcdic-cp-us ebcdic-cp-ca ebcdic-cp-wt ebcdic-cp-nl csIBM037\n",
"IBM038			2029		EBCDIC-INT cp038 csIBM038\n",
"IBM273			2030		CP273 csIBM273\n",
"IBM274			2031		EBCDIC-BE CP274 csIBM274\n",
"IBM275			2032		EBCDIC-BR cp275 csIBM275\n",
"IBM277			2033		EBCDIC-CP-DK EBCDIC-CP-NO csIBM277\n",
"IBM278			2034		CP278 ebcdic-cp-fi ebcdic-cp-se csIBM278\n",
"IBM280			2035		CP280 ebcdic-cp-it csIBM280\n",
"IBM281			2036		EBCDIC-JP-E cp281 csIBM281\n",
"IBM284			2037		CP284 ebcdic-cp-es csIBM284\n",
"IBM285			2038		CP285 ebcdic-cp-gb csIBM285\n",
"IBM290			2039		cp290 EBCDIC-JP-kana csIBM290\n",
"IBM297			2040		cp297 ebcdic-cp-fr csIBM297\n",
"IBM420			2041		cp420 ebcdic-cp-ar1 csIBM420\n",
"IBM423			2042		cp423 ebcdic-cp-gr csIBM423\n",
"IBM424			2043		cp424 ebcdic-cp-he csIBM424\n",
"IBM437			2011		cp437 437 csPC8CodePage437\n",
"IBM500			2044		CP500 ebcdic-cp-be ebcdic-cp-ch csIBM500\n",
"IBM775			2087		cp775 csPC775Baltic\n",
"IBM850			2009		cp850 850 csPC850Multilingual\n",
"IBM851			2045		cp851 851 csIBM851\n",
"IBM852			2010		cp852 852 csPCp852\n",
"IBM855			2046		cp855 855 csIBM855\n",
"IBM857			2047		cp857 857 csIBM857\n",
"IBM860			2048		cp860 860 csIBM860\n",
"IBM861			2049		cp861 861 cp-is csIBM861\n",
"IBM862			2013		cp862 862 csPC862LatinHebrew\n",
"IBM863			2050		cp863 863 csIBM863\n",
"IBM864			2051		cp864 csIBM864\n",
"IBM865			2052		cp865 865 csIBM865\n",
"IBM866			2086		cp866 866 csIBM866\n",
"IBM868			2053		CP868 cp-ar csIBM868\n",
"IBM869			2054		cp869 869 cp-gr csIBM869\n",
"IBM870			2055		CP870 ebcdic-cp-roece ebcdic-cp-yu csIBM870\n",
"IBM871			2056		CP871 ebcdic-cp-is csIBM871\n",
"IBM880			2057		cp880 EBCDIC-Cyrillic csIBM880\n",
"IBM891			2058		cp891 csIBM891\n",
"IBM903			2059		cp903 csIBM903\n",
"IBM904			2060		cp904 904 csIBBM904\n",
"IBM905			2061		CP905 ebcdic-cp-tr csIBM905\n",
"IBM918			2062		CP918 ebcdic-cp-ar2 csIBM918\n",
"IBM1026			2063		CP1026 csIBM1026\n",
"EBCDIC-AT-DE		2064		csIBMEBCDICATDE\n",
"EBCDIC-AT-DE-A		2065		csEBCDICATDEA\n",
"EBCDIC-CA-FR		2066		csEBCDICCAFR\n",
"EBCDIC-DK-NO		2067		csEBCDICDKNO\n",
"EBCDIC-DK-NO-A		2068		csEBCDICDKNOA\n",
"EBCDIC-FI-SE		2069		csEBCDICFISE\n",
"EBCDIC-FI-SE-A		2070		csEBCDICFISEA\n",
"EBCDIC-FR		2071		csEBCDICFR\n",
"EBCDIC-IT		2072		csEBCDICIT\n",
"EBCDIC-PT		2073		csEBCDICPT\n",
"EBCDIC-ES		2074		csEBCDICES\n",
"EBCDIC-ES-A		2075		csEBCDICESA\n",
"EBCDIC-ES-S		2076		csEBCDICESS\n",
"EBCDIC-UK		2077		csEBCDICUK\n",
"EBCDIC-US		2078		csEBCDICUS\n",
"UNKNOWN-8BIT		2079		csUnknown8BiT\n",
"MNEMONIC		2080		csMnemonic\n",
"MNEM			2081		csMnem\n",
"VISCII			2082		csVISCII\n",
"VIQR			2083		csVIQR\n",
"KOI8-R			2084		csKOI8R\n",
"KOI8-U			2088\n",
"IBM00858		2089		CCSID00858 CP00858 PC-Multilingual-850+euro\n",
"IBM00924		2090		CCSID00924 CP00924 ebcdic-Latin9--euro\n",
"IBM01140		2091		CCSID01140 CP01140 ebcdic-us-37+euro\n",
"IBM01141		2092		CCSID01141 CP01141 ebcdic-de-273+euro\n",
"IBM01142		2093		CCSID01142 CP01142 ebcdic-dk-277+euro ebcdic-no-277+euro\n",
"IBM01143		2094		CCSID01143 CP01143 ebcdic-fi-278+euro ebcdic-se-278+euro\n",
"IBM01144		2095		CCSID01144 CP01144 ebcdic-it-280+euro\n",
"IBM01145		2096		CCSID01145 CP01145 ebcdic-es-284+euro\n",
"IBM01146		2097		CCSID01146 CP01146 ebcdic-gb-285+euro\n",
"IBM01147		2098		CCSID01147 CP01147 ebcdic-fr-297+euro\n",
"IBM01148		2099		CCSID01148 CP01148 ebcdic-international-500+euro\n",
"IBM01149		2100		CCSID01149 CP01149 ebcdic-is-871+euro\n",
"Big5-HKSCS		2101\n",
"IBM1047			2102		IBM-1047\n",
"PTCP154			2103		csPTCP154 PT154 CP154 Cyrillic-Asian\n",
"Amiga-1251		2104		Ami1251 Amiga1251 Ami-1251\n",
"KOI7-switched		2105\n",
"UNICODE-1-1		1010		csUnicode11\n",
"SCSU			1011\n",
"UTF-7			1012\n",
"UTF-16BE		1013\n",
"UTF-16LE		1014\n",
"UTF-16			1015\n",
"CESU-8			1016		csCESU-8\n",
"UTF-32			1017\n",
"UTF-32BE		1018\n",
"UTF-32LE		1019\n",
"BOCU-1			1020		csBOCU-1\n",
"UNICODE-1-1-UTF-7	103		csUnicode11UTF7\n",
"UTF-8			106		UNICODE-1-1-UTF-8 UNICODE-2-0-UTF-8 utf8\n",
"ISO-8859-13		109		8859_13 ISO8859-13\n",
"ISO-8859-14		110		iso-ir-199 ISO_8859-14:1998 ISO_8859-14 latin8 iso-celtic l8 8859_14 ISO8859-14\n",
"ISO-8859-15		111		ISO_8859-15 Latin-9 8859_15 ISO8859-15\n",
"ISO-8859-16		112		iso-ir-226 ISO_8859-16:2001 ISO_8859-16 latin10 l10\n",
"GBK			113		CP936 MS936 windows-936\n",
"GB18030			114\n",
"OSD_EBCDIC_DF04_15	115\n",
"OSD_EBCDIC_DF03_IRV	116\n",
"OSD_EBCDIC_DF04_1	117\n",
"JIS_Encoding		16		csJISEncoding\n",
"Shift_JIS		17		MS_Kanji csShiftJIS X-SJIS Shift-JIS\n",
"EUC-JP			18		csEUCPkdFmtJapanese Extended_UNIX_Code_Packed_Format_for_Japanese EUCJP\n",
"Extended_UNIX_Code_Fixed_Width_for_Japanese	19		csEUCFixWidJapanese\n",
"ISO-10646-UCS-Basic	1002		csUnicodeASCII\n",
"ISO-10646-Unicode-Latin1	1003		csUnicodeLatin1 ISO-10646\n",
"ISO-Unicode-IBM-1261	1005		csUnicodeIBM1261\n",
"ISO-Unicode-IBM-1268	1006		csUnicodeIBM1268\n",
"ISO-Unicode-IBM-1276	1007		csUnicodeIBM1276\n",
"ISO-Unicode-IBM-1264	1008		csUnicodeIBM1264\n",
"ISO-Unicode-IBM-1265	1009		csUnicodeIBM1265\n",
"ISO-8859-1-Windows-3.0-Latin-1	2000		csWindows30Latin1\n",
"ISO-8859-1-Windows-3.1-Latin-1	2001		csWindows31Latin1\n",
"ISO-8859-2-Windows-Latin-2	2002		csWindows31Latin2\n",
"ISO-8859-9-Windows-Latin-5	2003		csWindows31Latin5\n",
"Adobe-Standard-Encoding	2005		csAdobeStandardEncoding\n",
"Ventura-US		2006		csVenturaUS\n",
"Ventura-International	2007		csVenturaInternational\n",
"PC8-Danish-Norwegian	2012		csPC8DanishNorwegian\n",
"PC8-Turkish		2014		csPC8Turkish\n",
"IBM-Symbols		2015		csIBMSymbols\n",
"IBM-Thai		2016		csIBMThai\n",
"HP-Legal		2017		csHPLegal\n",
"HP-Pi-font		2018		csHPPiFont\n",
"HP-Math8		2019		csHPMath8\n",
"Adobe-Symbol-Encoding	2020		csHPPSMath\n",
"HP-DeskTop		2021		csHPDesktop\n",
"Ventura-Math		2022		csVenturaMath\n",
"Microsoft-Publishing	2023		csMicrosoftPublishing\n",
"Windows-31J		2024		csWindows31J\n",
"GB2312			2025		csGB2312 EUC-CN EUCCN CN-GB\n",
"Big5			2026		csBig5 BIG-FIVE BIG-5 CN-BIG5 BIG_FIVE\n",
"windows-1250		2250		CP1250 MS-EE\n",
"windows-1251		2251		CP1251 MS-CYRL\n",
"windows-1252		2252		CP1252 MS-ANSI\n",
"windows-1253		2253		CP1253 MS-GREEK\n",
"windows-1254		2254		CP1254 MS-TURK\n",
"windows-1255		2255\n",
"windows-1256		2256		CP1256 MS-ARAB\n",
"windows-1257		2257		CP1257 WINBALTRIM\n",
"windows-1258		2258\n",
"TIS-620			2259\n",
"HZ-GB-2312		2085\n",

  // Additional encodings not defined by IANA
  // From Unicode Lib
"ISO-IR-182		4000\n",
"ISO-IR-197		4002\n",
"ISO-2022-JP-1		4008\n",
"MACCYRILLIC		4009		X-MAC-CYRILLIC MAC-CYRILLIC\n",
"MACUKRAINE		4010		X-MAC-UKRAINIAN MAC-UKRAINIAN\n",
"MACCENTRALEUROPE	4011		X-MAC-CENTRALEURROMAN MAC-CENTRALEURROMAN\n",
"JOHAB			4012\n",
"ISO-8859-11		4014		iso-ir-166 ISO_8859-11 ISO8859-11 8859_11\n",
"X-CURRENT		4999		X-SYSTEM\n",
"X-ACORN-LATIN1		5001\n",
"X-ACORN-FUZZY		5002\n",
  NULL
};

static char* internal_fgets(char* buf, int n, FILE* fp)
{
  int* p = (int*)fp;
  if (sAliases[*p])
  {
    strncpy(buf, sAliases[*p], n);
    (*p)++;
    return buf;
  }
  
  return NULL;
}
