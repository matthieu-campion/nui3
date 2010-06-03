/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef parserutils_charset_aliases_h_
#define parserutils_charset_aliases_h_

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

#include <parserutils/charset/mibenum.h>

typedef struct parserutils_charset_aliases_canon {
	struct parserutils_charset_aliases_canon *next;
	uint16_t mib_enum;
	uint16_t name_len;
	char name[1];
} parserutils_charset_aliases_canon;

/* Load encoding aliases from file */
parserutils_error parserutils_charset_aliases_create(const char *filename,
		parserutils_alloc alloc, void *pw);
/* Destroy encoding aliases */
void parserutils_charset_aliases_destroy(parserutils_alloc alloc, void *pw);

/* Canonicalise an alias name */
parserutils_charset_aliases_canon *parserutils_charset_alias_canonicalise(
		const char *alias, size_t len);

#ifndef NDEBUG
void parserutils_charset_aliases_dump(void);
#endif

#endif
