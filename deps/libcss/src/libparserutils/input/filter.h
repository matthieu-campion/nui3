/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef parserutils_input_filter_h_
#define parserutils_input_filter_h_

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
#include <parserutils/functypes.h>

typedef struct parserutils_filter parserutils_filter;

/**
 * Input filter option types
 */
typedef enum parserutils_filter_opttype {
	PARSERUTILS_FILTER_SET_ENCODING       = 0
} parserutils_filter_opttype;

/**
 * Input filter option parameters
 */
typedef union parserutils_filter_optparams {
	/** Parameters for encoding setting */
	struct {
		/** Encoding name */
		const char *name;
	} encoding;
} parserutils_filter_optparams;


/* Create an input filter */
parserutils_error parserutils_filter_create(const char *int_enc,
		parserutils_alloc alloc, void *pw, parserutils_filter **filter);
/* Destroy an input filter */
parserutils_error parserutils_filter_destroy(parserutils_filter *input);

/* Configure an input filter */
parserutils_error parserutils_filter_setopt(parserutils_filter *input,
		parserutils_filter_opttype type,
		parserutils_filter_optparams *params);

/* Process a chunk of data */
parserutils_error parserutils_filter_process_chunk(parserutils_filter *input,
		const uint8_t **data, size_t *len,
		uint8_t **output, size_t *outlen);

/* Reset an input filter's state */
parserutils_error parserutils_filter_reset(parserutils_filter *input);

#endif

