/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

/** \file
 * UTF-8 manipulation functions (interface).
 */

#ifndef parserutils_charset_utf8_h_
#define parserutils_charset_utf8_h_

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

#include <parserutils/errors.h>

parserutils_error parserutils_charset_utf8_to_ucs4(const uint8_t *s, size_t len,
		uint32_t *ucs4, size_t *clen);
parserutils_error parserutils_charset_utf8_from_ucs4(uint32_t ucs4, uint8_t **s,
		size_t *len);

parserutils_error parserutils_charset_utf8_length(const uint8_t *s, size_t max,
		size_t *len);
parserutils_error parserutils_charset_utf8_char_byte_length(const uint8_t *s,
		size_t *len);

parserutils_error parserutils_charset_utf8_prev(const uint8_t *s, uint32_t off,
		uint32_t *prevoff);
parserutils_error parserutils_charset_utf8_next(const uint8_t *s, uint32_t len,
		uint32_t off, uint32_t *nextoff);

parserutils_error parserutils_charset_utf8_next_paranoid(const uint8_t *s, 
		uint32_t len, uint32_t off, uint32_t *nextoff);

#endif

