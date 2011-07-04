
#line 1 "hb-ot-shape-complex-indic-machine.rl"
/*
 * Copyright © 2011  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_SHAPE_COMPLEX_INDIC_MACHINE_HH
#define HB_OT_SHAPE_COMPLEX_INDIC_MACHINE_HH

#include "hb-private.hh"

HB_BEGIN_DECLS


#line 38 "hb-ot-shape-complex-indic-machine.hh"
static const unsigned char _indic_syllable_machine_trans_keys[] = {
	0u, 0u, 5u, 5u, 1u, 2u, 1u, 5u, 5u, 5u, 1u, 2u, 0u, 12u, 0u, 12u, 
	0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 
	0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 
	0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0u, 12u, 0
};

static const char _indic_syllable_machine_key_spans[] = {
	0, 1, 2, 5, 1, 2, 13, 13, 
	13, 13, 13, 13, 13, 13, 13, 13, 
	13, 13, 13, 13, 13, 13, 13, 13, 
	13, 13, 13, 13, 13, 13, 13
};

static const short _indic_syllable_machine_index_offsets[] = {
	0, 0, 2, 5, 11, 13, 16, 30, 
	44, 58, 72, 86, 100, 114, 128, 142, 
	156, 170, 184, 198, 212, 226, 240, 254, 
	268, 282, 296, 310, 324, 338, 352
};

static const char _indic_syllable_machine_indicies[] = {
	0, 1, 2, 2, 1, 2, 2, 
	1, 1, 0, 1, 3, 1, 4, 4, 
	1, 5, 4, 6, 7, 1, 1, 1, 
	1, 1, 1, 1, 1, 8, 1, 9, 
	10, 11, 12, 13, 14, 15, 15, 16, 
	17, 18, 19, 20, 1, 9, 10, 11, 
	12, 13, 21, 15, 15, 16, 17, 18, 
	19, 20, 1, 5, 4, 6, 7, 22, 
	1, 23, 24, 25, 26, 27, 1, 8, 
	1, 5, 4, 6, 7, 1, 1, 23, 
	24, 25, 26, 27, 1, 8, 1, 5, 
	4, 6, 7, 1, 1, 1, 1, 25, 
	26, 27, 1, 8, 1, 5, 4, 6, 
	7, 28, 2, 1, 1, 25, 26, 27, 
	1, 8, 1, 5, 4, 6, 7, 1, 
	2, 1, 1, 25, 26, 27, 1, 8, 
	1, 5, 4, 6, 7, 1, 1, 1, 
	1, 1, 1, 27, 1, 8, 1, 5, 
	4, 6, 7, 1, 1, 1, 1, 1, 
	1, 5, 1, 8, 1, 5, 4, 6, 
	7, 29, 0, 23, 23, 25, 26, 27, 
	1, 8, 1, 5, 4, 6, 7, 1, 
	0, 23, 23, 25, 26, 27, 1, 8, 
	1, 9, 10, 11, 12, 1, 14, 15, 
	15, 16, 17, 18, 19, 20, 1, 9, 
	4, 4, 12, 1, 1, 30, 30, 1, 
	17, 18, 1, 20, 1, 9, 4, 4, 
	12, 1, 1, 1, 1, 1, 17, 18, 
	1, 20, 1, 9, 10, 11, 12, 1, 
	1, 1, 1, 1, 1, 18, 1, 20, 
	1, 9, 10, 11, 12, 1, 1, 1, 
	1, 1, 1, 31, 1, 20, 1, 9, 
	10, 11, 12, 1, 1, 1, 1, 1, 
	1, 1, 1, 20, 1, 9, 10, 11, 
	12, 32, 33, 1, 1, 16, 17, 18, 
	1, 20, 1, 9, 10, 11, 12, 1, 
	33, 1, 1, 16, 17, 18, 1, 20, 
	1, 9, 10, 11, 12, 1, 1, 1, 
	1, 16, 17, 18, 1, 20, 1, 9, 
	10, 11, 12, 1, 34, 1, 1, 16, 
	17, 18, 1, 20, 1, 9, 10, 11, 
	12, 1, 1, 35, 35, 1, 17, 18, 
	1, 20, 1, 9, 10, 11, 12, 1, 
	1, 1, 1, 1, 17, 18, 1, 20, 
	1, 9, 4, 4, 7, 1, 1, 30, 
	30, 1, 17, 18, 1, 8, 1, 0
};

static const char _indic_syllable_machine_trans_targs[] = {
	2, 0, 11, 5, 7, 6, 8, 9, 
	16, 6, 7, 8, 9, 18, 19, 4, 
	24, 21, 22, 27, 16, 30, 10, 1, 
	3, 12, 14, 15, 13, 17, 20, 23, 
	25, 26, 28, 29
};

static const char _indic_syllable_machine_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 1, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0
};

static const char _indic_syllable_machine_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1
};

static const int indic_syllable_machine_start = 6;
static const int indic_syllable_machine_first_final = 6;
static const int indic_syllable_machine_error = 0;

static const int indic_syllable_machine_en_main = 6;


#line 38 "hb-ot-shape-complex-indic-machine.rl"



#line 80 "hb-ot-shape-complex-indic-machine.rl"



#include <stdio.h>
#include <string.h>

static void
find_syllables (hb_ot_shape_context_t *c)
{
  unsigned int p, pe, eof;
  int cs;
  
#line 156 "hb-ot-shape-complex-indic-machine.hh"
	{
	cs = indic_syllable_machine_start;
	}

#line 94 "hb-ot-shape-complex-indic-machine.rl"


  p = 0;
  pe = eof = c->buffer->len;

  
#line 168 "hb-ot-shape-complex-indic-machine.hh"
	{
	int _slen;
	int _trans;
	const unsigned char *_keys;
	const char *_inds;
	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _indic_syllable_machine_trans_keys + (cs<<1);
	_inds = _indic_syllable_machine_indicies + _indic_syllable_machine_index_offsets[cs];

	_slen = _indic_syllable_machine_key_spans[cs];
	_trans = _inds[ _slen > 0 && _keys[0] <=( c->buffer->info[p].indic_category()) &&
		( c->buffer->info[p].indic_category()) <= _keys[1] ?
		( c->buffer->info[p].indic_category()) - _keys[0] : _slen ];

	cs = _indic_syllable_machine_trans_targs[_trans];

	if ( _indic_syllable_machine_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _indic_syllable_machine_trans_actions[_trans] ) {
	case 1:
#line 62 "hb-ot-shape-complex-indic-machine.rl"
	{
  //fprintf (stderr, "Syll %d\n", p);
}
	break;
#line 199 "hb-ot-shape-complex-indic-machine.hh"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	switch ( _indic_syllable_machine_eof_actions[cs] ) {
	case 1:
#line 62 "hb-ot-shape-complex-indic-machine.rl"
	{
  //fprintf (stderr, "Syll %d\n", p);
}
	break;
#line 217 "hb-ot-shape-complex-indic-machine.hh"
	}
	}

	_out: {}
	}

#line 101 "hb-ot-shape-complex-indic-machine.rl"

}

HB_END_DECLS

#endif /* HB_OT_SHAPE_COMPLEX_INDIC_MACHINE_HH */
