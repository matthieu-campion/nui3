/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_charset_detect_h_
#define css_charset_detect_h_

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

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/types.h>

#include <parserutils/errors.h>

/* Extract a charset from a chunk of data */
parserutils_error css_charset_extract(const uint8_t *data, size_t len,
		uint16_t *mibenum, uint32_t *source);

#endif

