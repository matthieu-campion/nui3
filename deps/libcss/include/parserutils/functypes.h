/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007-8 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef parserutils_functypes_h_
#define parserutils_functypes_h_

#ifndef WIN32
#include <stdbool.h>
#include <stdint.h>
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

#include <stdlib.h>

#include <parserutils/types.h>

/* Type of allocation function for parserutils */
typedef void *(*parserutils_alloc)(void *ptr, size_t size, void *pw);

#endif

