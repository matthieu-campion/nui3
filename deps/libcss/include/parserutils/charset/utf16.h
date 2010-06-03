/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

/** \file
 * UTF-16 manipulation functions (interface).
 */

#ifndef parserutils_charset_utf16_h_
#define parserutils_charset_utf16_h_

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

#define INT8_MAX         127
#define INT16_MAX        32767
#define INT32_MAX        2147483647
#define INT64_MAX        9223372036854775807LL

#define INT8_MIN          -128
#define INT16_MIN         -32768
   /*
      Note:  the literal "most negative int" cannot be written in C --
      the rules in the standard (section 6.4.4.1 in C99) will give it
      an unsigned type, so INT32_MIN (and the most negative member of
      any larger signed type) must be written via a constant expression.
   */
#define INT32_MIN        (-INT32_MAX-1)
#define INT64_MIN        (-INT64_MAX-1)

#define UINT8_MAX         255
#define UINT16_MAX        65535
#define UINT32_MAX        4294967295U
#define UINT64_MAX        18446744073709551615ULL

#endif

#include <parserutils/errors.h>

parserutils_error parserutils_charset_utf16_to_ucs4(const uint8_t *s, 
		size_t len, uint32_t *ucs4, size_t *clen);
parserutils_error parserutils_charset_utf16_from_ucs4(uint32_t ucs4, 
		uint8_t *s, size_t *len);

parserutils_error parserutils_charset_utf16_length(const uint8_t *s, 
		size_t max, size_t *len);
parserutils_error parserutils_charset_utf16_char_byte_length(const uint8_t *s,
		size_t *len);

parserutils_error parserutils_charset_utf16_prev(const uint8_t *s, 
		uint32_t off, uint32_t *prevoff);
parserutils_error parserutils_charset_utf16_next(const uint8_t *s, 
		uint32_t len, uint32_t off, uint32_t *nextoff);

parserutils_error parserutils_charset_utf16_next_paranoid(const uint8_t *s,
		uint32_t len, uint32_t off, uint32_t *nextoff);

#endif

