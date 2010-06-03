/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef parserutils_charset_mibenum_h_
#define parserutils_charset_mibenum_h_

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
#include <parserutils/functypes.h>

/* Convert an encoding alias to a MIB enum value */
uint16_t parserutils_charset_mibenum_from_name(const char *alias, size_t len);
/* Convert a MIB enum value into an encoding alias */
const char *parserutils_charset_mibenum_to_name(uint16_t mibenum);
/* Determine if a MIB enum value represents a Unicode variant */
bool parserutils_charset_mibenum_is_unicode(uint16_t mibenum);

#endif
