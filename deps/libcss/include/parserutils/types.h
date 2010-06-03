/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef parserutils_types_h_
#define parserutils_types_h_

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
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
typedef int bool;
#define false   (bool)0
#define true    (bool)1
#endif

#endif

