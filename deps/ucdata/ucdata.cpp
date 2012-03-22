/*
 * Copyright 2001 Computing Research Labs, New Mexico State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COMPUTING RESEARCH LAB OR NEW MEXICO STATE UNIVERSITY BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef lint
#ifdef __GNUC__
static char rcsid[] __attribute__ ((unused)) = "$Id: ucdata.c,v 1.4 2001/01/02 18:46:20 mleisher Exp $";
#else
static char rcsid[] = "$Id: ucdata.c,v 1.4 2001/01/02 18:46:20 mleisher Exp $";
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include "ucdata.h"
#include "ucdata_static.h"

#include <vector>
#include <algorithm>

#ifdef WIN32
#define __STDC__
#endif

/**************************************************************************
 *
 * Miscellaneous types, data, and support functions.
 *
 **************************************************************************/

typedef struct {
    uint16_t bom;
    uint16_t cnt;
    union {
        uint32_t bytes;
        uint16_t len[2];
    } size;
} _ucheader_t;

/*
 * A simple array of 32-bit masks for lookup.
 */
static uint32_t masks32[32] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020,
    0x00000040, 0x00000080, 0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000, 0x00010000, 0x00020000,
    0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000,
    0x40000000, 0x80000000
};

#define endian_short(cc) (((cc) >> 8) | (((cc) & 0xff) << 8))
#define endian_long(cc) ((((cc) & 0xff) << 24)|((((cc) >> 8) & 0xff) << 16)|\
                        ((((cc) >> 16) & 0xff) << 8)|((cc) >> 24))

static FILE *
#ifdef __STDC__
_ucopenfile(char *paths, char *filename, char *mode)
#else
_ucopenfile(paths, filename, mode)
char *paths, *filename, *mode;
#endif
{
    FILE *f;
    char *fp, *dp, *pp, path[BUFSIZ];

    if (filename == 0 || *filename == 0)
      return 0;

    dp = paths;
    while (dp && *dp) {
        pp = path;
        while (*dp && *dp != ':')
          *pp++ = *dp++;
        *pp++ = '/';

        fp = filename;
        while (*fp)
          *pp++ = *fp++;
        *pp = 0;

        if ((f = fopen(path, mode)) != 0)
          return f;

        if (*dp == ':')
          dp++;
    }

    return 0;
}

/**************************************************************************
 *
 * Support for the character properties.
 *
 **************************************************************************/

static uint32_t  _ucprop_size = 0;
static uint16_t *_ucprop_offsets;
static uint32_t  *_ucprop_ranges;

/*
 * Return -1 on error, 0 if okay
 */
static int32_t
#ifdef __STDC__
_ucprop_load(char *paths, int32_t reload)
#else
_ucprop_load(paths, reload)
char *paths;
int32_t reload;
#endif
{
    FILE *in;
    uint32_t size, i;
    _ucheader_t hdr;

    if (_ucprop_size > 0) {
        if (!reload)
          /*
           * The character properties have already been loaded.
           */
          return 0;

        /*
         * Unload the current character property data in preparation for
         * loading a new copy.  Only the first array has to be deallocated
         * because all the memory for the arrays is allocated as a single
         * block.
         */
        free((char *) _ucprop_offsets);
        _ucprop_size = 0;
    }

    if ((in = _ucopenfile(paths, "ctype.dat", "rb")) == 0)
      return -1;

    /*
     * Load the header.
     */
    fread((char *) &hdr, sizeof(_ucheader_t), 1, in);

    if (hdr.bom == 0xfffe) {
        hdr.cnt = endian_short(hdr.cnt);
        hdr.size.bytes = endian_long(hdr.size.bytes);
    }

    if ((_ucprop_size = hdr.cnt) == 0) {
        fclose(in);
        return -1;
    }

    /*
     * Allocate all the storage needed for the lookup table.
     */
    _ucprop_offsets = (uint16_t *) malloc(hdr.size.bytes);

    /*
     * Calculate the offset into the storage for the ranges.  The offsets
     * array is on a 4-byte boundary and one larger than the value provided in
     * the header count field.  This means the offset to the ranges must be
     * calculated after aligning the count to a 4-byte boundary.
     */
    if ((size = ((hdr.cnt + 1) * sizeof(uint16_t))) & 3)
      size += 4 - (size & 3);
    size >>= 1;
    _ucprop_ranges = (uint32_t *) (_ucprop_offsets + size);

    /*
     * Load the offset array.
     */
    fread((char *) _ucprop_offsets, sizeof(uint16_t), size, in);

    /*
     * Do an endian swap if necessary.  Don't forget there is an extra node on
     * the end with the final index.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i <= _ucprop_size; i++)
          _ucprop_offsets[i] = endian_short(_ucprop_offsets[i]);
    }

    /*
     * Load the ranges.  The number of elements is in the last array position
     * of the offsets.
     */
    fread((char *) _ucprop_ranges, sizeof(uint32_t),
          _ucprop_offsets[_ucprop_size], in);

    fclose(in);

    /*
     * Do an endian swap if necessary.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i < _ucprop_offsets[_ucprop_size]; i++)
          _ucprop_ranges[i] = endian_long(_ucprop_ranges[i]);
    }
    return 0;
}

/*
 * Return -1 on error, 0 if okay
 */
bool compare_range(const std::pair<uint32_t, int8_t>& rLeft, const std::pair<uint32_t, int8_t>& rRight)
{
  return (rLeft.first < rRight.first);
}

static std::vector<std::pair<uint32_t, int8_t> > ucprops_ranges;


static int32_t
_ucprop_load_static()
{
  _ucprop_offsets = (uint16_t*)ctype_props_offsets;
  _ucprop_ranges = (uint32_t*)ctype_props;
  _ucprop_size = sizeof(ctype_props_offsets) / sizeof(ctype_props_offsets[0]) - 2;
  
  std::vector<std::pair<uint32_t, int8_t> > ranges;
  ranges.reserve(sizeof(ctype_props) / 4);
  
  for (uint32_t n = 0; n < _ucprop_size; n++)
  {
    int32_t l, r, m;
    int32_t v1 = 1 << n;
    int32_t v2 = -v1;
    
    /*
     * There is an extra node on the end of the offsets to allow this routine
     * to work right.  If the index is 0xffff, then there are no nodes for the
     * property.
     */
    if ((l = _ucprop_offsets[n]) != 0xffff)
    {
      /*
       * Locate the next offset that is not 0xffff.  The sentinel at the end of
       * the array is the max index value.
       */
      for (m = 1;
           n + m < _ucprop_size && _ucprop_offsets[n + m] == 0xffff; m++) ;
      r = _ucprop_offsets[n + m] - 1;
      
      // Scan the range l to r:
      uint32_t i = l;
      while (i < r)
      {
        const uint32_t a = _ucprop_ranges[i];
        const uint32_t b = _ucprop_ranges[i + 1] - 1;
        ranges.push_back(std::make_pair(a, v1));
        ranges.push_back(std::make_pair(b, v2));
        i += 2;
      }
    }
  }
  
  //printf("found %d range stops\n", ranges.size());
  std::sort(ranges.begin(), ranges.end(), compare_range);
  
  ucprops_ranges.push_back(ranges.front());

  uint32_t pos = ranges.front().first;
  int8_t val = ranges.front().second;
  size_t s = ranges.size();
  
  for (uint32_t i = 1; i < s; i++)
  {
    const std::pair<uint32_t, int8_t>& rRange(ranges[i]);
    const uint32_t newpos = rRange.first;
    const int8_t newval = rRange.second;
    
    if (newval < 0)
      val &= ~(-newval);
    else
      val |= newval;
    
    if (newpos == pos)
    {
      // Modify in place
      ucprops_ranges.back().second = val;
    }
    else
    {
      // New range start
      pos = newpos;
      ucprops_ranges.push_back(std::make_pair(pos, val));
    }
  }
  
  //printf("ranges computed: %d\n", ucprops_ranges.size());
  return 1;
}


static void
#ifdef __STDC__
_ucprop_unload(void)
#else
_ucprop_unload()
#endif
{
    if (_ucprop_size == 0 || _ucprop_offsets == ctype_props_offsets)
      return;

    /*
     * Only need to free the offsets because the memory is allocated as a
     * single block.
     */
    free((char *) _ucprop_offsets);
    _ucprop_size = 0;
}

#if 0
int32_t
#ifdef __STDC__
ucprop_lookup(uint32_t code, uint32_t n)
#else
ucprop_lookup(code, n)
uint32_t code, n;
#endif
{
  //printf("ucprop_lookup %d %d\n", code, n);
    int32_t l, r, m;

    /*
     * There is an extra node on the end of the offsets to allow this routine
     * to work right.  If the index is 0xffff, then there are no nodes for the
     * property.
     */
    if ((l = _ucprop_offsets[n]) == 0xffff)
      return 0;

    /*
     * Locate the next offset that is not 0xffff.  The sentinel at the end of
     * the array is the max index value.
     */
    for (m = 1;
         n + m < _ucprop_size && _ucprop_offsets[n + m] == 0xffff; m++) ;

    r = _ucprop_offsets[n + m] - 1;

    while (l <= r) {
        /*
         * Determine a "mid" point and adjust to make sure the mid point is at
         * the beginning of a range pair.
         */
        m = (l + r) >> 1;
        m -= (m & 1);
        if (code > _ucprop_ranges[m + 1])
          l = m + 2;
        else if (code < _ucprop_ranges[m])
          r = m - 2;
        else if (code >= _ucprop_ranges[m] && code <= _ucprop_ranges[m + 1])
          return 1;
    }
    return 0;
}
#else

static uint32_t last_range_start = -1;
static uint32_t last_range_end = -1;
static int8_t last_range_value = 0;

//#define STATS
#ifdef STATS
static uint32_t miss = 0;
static uint32_t calls = 0;
#endif

int32_t
#ifdef __STDC__
ucprop_lookup(uint32_t code, uint32_t n)
#else
ucprop_lookup(code, n)
uint32_t code, n;
#endif
{
#ifdef STATS
  calls++;
#endif
  
  if (last_range_start <= code && code < last_range_end)
    return ((last_range_value & (1 << n)) >> n);
  
  //printf("ucprop_lookup %d %d\n", code, n);
  int32_t l = 0, r = ucprops_ranges.size() - 1, m;
  
  while (l < r)
  {
    m = (l + r) >> 1;
    
    const uint32_t ll = ucprops_ranges[l].first;
    const uint32_t mm = ucprops_ranges[m].first;
    const uint32_t rr = ucprops_ranges[r].first;
    
    if (code >= mm)
      l = m;
    else if (code < mm)
      r = m;
    if (code >= mm && code < ucprops_ranges[m + 1].first)
    {
      last_range_start = ucprops_ranges[m].first;
      last_range_end = ucprops_ranges[m + 1].first;
      last_range_value = ucprops_ranges[m].second;
#ifdef STATS
      miss++;
      //printf("calls %d / miss %d (%f%%)\n", calls, miss, 100.0f * (float)miss / (float)calls);
#endif
      return ((last_range_value & (1 << n)) >> n);
    }
  }
#ifdef STATS
  miss++;
#endif
  return 0;
}

#endif

void
#ifdef __STDC__
ucprop_get(uint32_t code, uint32_t* mask1, uint32_t* mask2)
#else
ucprop_get(code, mask1, mask2)
uint32_t code;
uint32_t* mask1;
uint32_t* mask2;
#endif
{
  uint32_t i;
  for (i = 0; mask1 && i < 32; i++) {
    if (ucprop_lookup(code, i))
      *mask1 |= masks32[i];
  }
  
  for (i = 32; mask2 && i < _ucprop_size; i++) {
    if (ucprop_lookup(code, i))
      *mask2 |= masks32[i & 31];
  }
}


int32_t
#ifdef __STDC__
ucisprop(uint32_t code, uint32_t mask1, uint32_t mask2)
#else
ucisprop(code, mask1, mask2)
uint32_t code, mask1, mask2;
#endif
{
    uint32_t i;

    if (mask1 == 0 && mask2 == 0)
      return 0;

    for (i = 0; mask1 && i < 32; i++) {
        if ((mask1 & masks32[i]) && ucprop_lookup(code, i))
          return 1;
    }

    for (i = 32; mask2 && i < _ucprop_size; i++) {
        if ((mask2 & masks32[i & 31]) && ucprop_lookup(code, i))
          return 1;
    }

    return 0;
}

/**************************************************************************
 *
 * Support for case mapping.
 *
 **************************************************************************/

static uint32_t _uccase_size;
static uint16_t _uccase_len[2];
static uint32_t *_uccase_map;

/*
 * Return -1 on error, 0 if okay
 */
static int32_t
#ifdef __STDC__
_uccase_load(char *paths, int32_t reload)
#else
_uccase_load(paths, reload)
char *paths;
int32_t reload;
#endif
{
    FILE *in;
    uint32_t i;
    _ucheader_t hdr;

    if (_uccase_size > 0) {
        if (!reload)
          /*
           * The case mappings have already been loaded.
           */
          return 0;

        free((char *) _uccase_map);
        _uccase_size = 0;
    }

    if ((in = _ucopenfile(paths, "case.dat", "rb")) == 0)
      return -1;

    /*
     * Load the header.
     */
    fread((char *) &hdr, sizeof(_ucheader_t), 1, in);

    if (hdr.bom == 0xfffe) {
        hdr.cnt = endian_short(hdr.cnt);
        hdr.size.len[0] = endian_short(hdr.size.len[0]);
        hdr.size.len[1] = endian_short(hdr.size.len[1]);
    }

    /*
     * Set the node count and lengths of the upper and lower case mapping
     * tables.
     */
    _uccase_size = hdr.cnt * 3;
    _uccase_len[0] = hdr.size.len[0] * 3;
    _uccase_len[1] = hdr.size.len[1] * 3;

    _uccase_map = (uint32_t *)
        malloc(_uccase_size * sizeof(uint32_t));

    /*
     * Load the case mapping table.
     */
    fread((char *) _uccase_map, sizeof(uint32_t), _uccase_size, in);

    /*
     * Do an endian swap if necessary.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i < _uccase_size; i++)
          _uccase_map[i] = endian_long(_uccase_map[i]);
    }
    return 0;
}

static int32_t
_uccase_load_static()
{
  /*
   * Set the node count and lengths of the upper and lower case mapping
   * tables.
   */
  _uccase_size = case_num_nodes * 3;
  _uccase_len[0] = case_upper_size * 3;
  _uccase_len[1] = case_lower_size * 3;
  
  _uccase_map = (uint32_t*)case_table;
  return 1;
}


static void
#ifdef __STDC__
_uccase_unload(void)
#else
_uccase_unload()
#endif
{
    if (_uccase_size == 0 || _uccase_map == case_table)
      return;

    free((char *) _uccase_map);
    _uccase_size = 0;
}

static uint32_t
#ifdef __STDC__
_uccase_lookup(uint32_t code, int32_t l, int32_t r, int32_t field)
#else
_uccase_lookup(code, l, r, field)
uint32_t code;
int32_t l, r;
int32_t field;
#endif
{
    int32_t m;

    /*
     * Do the binary search.
     */
    while (l <= r) {
        /*
         * Determine a "mid" point and adjust to make sure the mid point is at
         * the beginning of a case mapping triple.
         */
        m = (l + r) >> 1;
        m -= (m % 3);
        if (code > _uccase_map[m])
          l = m + 3;
        else if (code < _uccase_map[m])
          r = m - 3;
        else if (code == _uccase_map[m])
          return _uccase_map[m + field];
    }

    return code;
}

uint32_t
#ifdef __STDC__
uctoupper(uint32_t code)
#else
uctoupper(code)
uint32_t code;
#endif
{
    int32_t field;
    int32_t l, r;

    if (ucisupper(code))
      return code;

    if (ucislower(code)) {
        /*
         * The character is lower case.
         */
        field = 2;
        l = _uccase_len[0];
        r = (l + _uccase_len[1]) - 3;
    } else {
        /*
         * The character is title case.
         */
        field = 1;
        l = _uccase_len[0] + _uccase_len[1];
        r = _uccase_size - 3;
    }
    return _uccase_lookup(code, l, r, field);
}

uint32_t
#ifdef __STDC__
uctolower(uint32_t code)
#else
uctolower(code)
uint32_t code;
#endif
{
    int32_t field;
    int32_t l, r;

    if (ucislower(code))
      return code;

    if (ucisupper(code)) {
        /*
         * The character is upper case.
         */
        field = 1;
        l = 0;
        r = _uccase_len[0] - 3;
    } else {
        /*
         * The character is title case.
         */
        field = 2;
        l = _uccase_len[0] + _uccase_len[1];
        r = _uccase_size - 3;
    }
    return _uccase_lookup(code, l, r, field);
}

uint32_t
#ifdef __STDC__
uctotitle(uint32_t code)
#else
uctotitle(code)
uint32_t code;
#endif
{
    int32_t field;
    int32_t l, r;

    if (ucistitle(code))
      return code;

    /*
     * The offset will always be the same for converting to title case.
     */
    field = 2;

    if (ucisupper(code)) {
        /*
         * The character is upper case.
         */
        l = 0;
        r = _uccase_len[0] - 3;
    } else {
        /*
         * The character is lower case.
         */
        l = _uccase_len[0];
        r = (l + _uccase_len[1]) - 3;
    }
    return _uccase_lookup(code, l, r, field);
}

/**************************************************************************
 *
 * Support for compositions.
 *
 **************************************************************************/

static uint32_t  _uccomp_size;
static uint32_t *_uccomp_data;

/*
 * Return -1 on error, 0 if okay
 */
static int32_t
#ifdef __STDC__
_uccomp_load(char *paths, int32_t reload)
#else
_uccomp_load(paths, reload)
char *paths;
int32_t reload;
#endif
{
    FILE *in;
    uint32_t size, i;
    _ucheader_t hdr;

    if (_uccomp_size > 0) {
        if (!reload)
            /*
             * The compositions have already been loaded.
             */
            return 0;

        free((char *) _uccomp_data);
        _uccomp_size = 0;
    }

    if ((in = _ucopenfile(paths, "comp.dat", "rb")) == 0)
        return -1;

    /*
     * Load the header.
     */
    fread((char *) &hdr, sizeof(_ucheader_t), 1, in);

    if (hdr.bom == 0xfffe) {
        hdr.cnt = endian_short(hdr.cnt);
        hdr.size.bytes = endian_long(hdr.size.bytes);
    }

    _uccomp_size = hdr.cnt;
    _uccomp_data = (uint32_t *) malloc(hdr.size.bytes);

    /*
     * Read the composition data in.
     */
    size = hdr.size.bytes / sizeof(uint32_t);
    fread((char *) _uccomp_data, sizeof(uint32_t), size, in);

    /*
     * Do an endian swap if necessary.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i < size; i++)
            _uccomp_data[i] = endian_long(_uccomp_data[i]);
    }

    /*
     * Assume that the data is ordered on count, so that all compositions
     * of length 2 come first. Only handling length 2 for now.
     */
    for (i = 1; i < size; i += 4)
      if (_uccomp_data[i] != 2)
        break;
    _uccomp_size = i - 1;

    return 0;
}

static int32_t
_uccomp_load_static()
{
  uint32_t i = 0;
  _uccomp_size = sizeof(comp_list) / sizeof(uint32_t);
  _uccomp_data = (uint32_t *)comp_list;
  
  /*
   * Assume that the data is ordered on count, so that all compositions
   * of length 2 come first. Only handling length 2 for now.
   */
  for (i = 1; i < _uccomp_size; i += 4)
    if (_uccomp_data[i] != 2)
      break;
  _uccomp_size = i - 1;
  
  return 1;
}

static void
#ifdef __STDC__
_uccomp_unload(void)
#else
_uccomp_unload()
#endif
{
    if (_uccomp_size == 0 || _uccomp_data == comp_list)
        return;

    free((char *) _uccomp_data);
    _uccomp_size = 0;
}

int32_t
#ifdef __STDC__
uccomp(uint32_t node1, uint32_t node2, uint32_t *comp)
#else
uccomp(node1, node2, comp)
uint32_t node1, node2, *comp;
#endif
{
    int32_t l, r, m;

    l = 0;
    r = _uccomp_size - 1;

    while (l <= r) {
        m = ((r + l) >> 1);
        m -= m & 3;
        if (node1 > _uccomp_data[m+2])
          l = m + 4;
        else if (node1 < _uccomp_data[m+2])
          r = m - 4;
        else if (node2 > _uccomp_data[m+3])
          l = m + 4;
        else if (node2 < _uccomp_data[m+3])
          r = m - 4;
        else {
            *comp = _uccomp_data[m];
            return 1;
        }
    }
    return 0;
}

int32_t
#ifdef __STDC__
uccomp_hangul(uint32_t *str, int32_t len)
#else
uccomp_hangul(str, len)
uint32_t *str;
int32_t len;
#endif
{
    const int32_t SBase = 0xAC00, LBase = 0x1100,
        VBase = 0x1161, TBase = 0x11A7,
        LCount = 19, VCount = 21, TCount = 28,
        NCount = VCount * TCount,   /* 588 */
        SCount = LCount * NCount;   /* 11172 */
    
    int32_t i, rlen;
    uint32_t ch, last, lindex, sindex;

    last = str[0];
    rlen = 1;
    for ( i = 1; i < len; i++ ) {
        ch = str[i];

        /* check if two current characters are L and V */
        lindex = last - LBase;
        if (0 <= lindex && lindex < LCount) {
            uint32_t vindex = ch - VBase;
            if (0 <= vindex && vindex < VCount) {
                /* make syllable of form LV */
                last = SBase + (lindex * VCount + vindex) * TCount;
                str[rlen-1] = last; /* reset last */
                continue;
            }
        }
        
        /* check if two current characters are LV and T */
        sindex = last - SBase;
        if (0 <= sindex && sindex < SCount && (sindex % TCount) == 0) {
            uint32_t tindex = ch - TBase;
            if (0 <= tindex && tindex <= TCount) {
                /* make syllable of form LVT */
                last += tindex;
                str[rlen-1] = last; /* reset last */
                continue;
            }
        }

        /* if neither case was true, just add the character */
        last = ch;
        str[rlen] = ch;
        rlen++;
    }
    return rlen;
}

int32_t
#ifdef __STDC__
uccanoncomp(uint32_t *str, int32_t len)
#else
uccanoncomp(str, len)
uint32_t *str;
int32_t len;
#endif
{
    int32_t i, stpos, copos;
    uint32_t cl, prevcl, st, ch, co;

    st = str[0];
    stpos = 0;
    copos = 1;
    prevcl = uccombining_class(st) == 0 ? 0 : 256;
        
    for (i = 1; i < len; i++) {
        ch = str[i];
        cl = uccombining_class(ch);
        if (uccomp(st, ch, &co) && (prevcl < cl || prevcl == 0))
          st = str[stpos] = co;
        else {
            if (cl == 0) {
                stpos = copos;
                st = ch;
            }
            prevcl = cl;
            str[copos++] = ch;
        }
    }

    return uccomp_hangul(str, copos);
}

/**************************************************************************
 *
 * Support for decompositions.
 *
 **************************************************************************/

static uint32_t  _ucdcmp_size;
static uint32_t *_ucdcmp_nodes;
static uint32_t *_ucdcmp_decomp;

/*
 * Return -1 on error, 0 if okay
 */
static int32_t
#ifdef __STDC__
_ucdcmp_load(char *paths, int32_t reload)
#else
_ucdcmp_load(paths, reload)
char *paths;
int32_t reload;
#endif
{
    FILE *in;
    uint32_t size, i;
    _ucheader_t hdr;

    if (_ucdcmp_size > 0) {
        if (!reload)
            /*
             * The decompositions have already been loaded.
             */
            return 0;

        free((char *) _ucdcmp_nodes);
        _ucdcmp_size = 0;
    }

    if ((in = _ucopenfile(paths, "decomp.dat", "rb")) == 0)
        return -1;

    /*
     * Load the header.
     */
    fread((char *) &hdr, sizeof(_ucheader_t), 1, in);

    if (hdr.bom == 0xfffe) {
        hdr.cnt = endian_short(hdr.cnt);
        hdr.size.bytes = endian_long(hdr.size.bytes);
    }

    _ucdcmp_size = hdr.cnt << 1;
    _ucdcmp_nodes = (uint32_t *) malloc(hdr.size.bytes);
    _ucdcmp_decomp = _ucdcmp_nodes + (_ucdcmp_size + 1);

    /*
     * Read the decomposition data in.
     */
    size = hdr.size.bytes / sizeof(uint32_t);
    fread((char *) _ucdcmp_nodes, sizeof(uint32_t), size, in);

    /*
     * Do an endian swap if necessary.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i < size; i++)
            _ucdcmp_nodes[i] = endian_long(_ucdcmp_nodes[i]);
    }
    return 0;
}

static int32_t
_ucdcmp_load_static()
{
  uint32_t size, i;
  
  _ucdcmp_size = decomps_size;
  _ucdcmp_nodes = (uint32_t*)decomps_list;
  _ucdcmp_decomp = (uint32_t*)decomps;
  
  return 1;
}

static void
#ifdef __STDC__
_ucdcmp_unload(void)
#else
_ucdcmp_unload()
#endif
{
    if (_ucdcmp_size == 0 || _ucdcmp_nodes == decomps_list)
      return;

    /*
     * Only need to free the offsets because the memory is allocated as a
     * single block.
     */
    free((char *) _ucdcmp_nodes);
    _ucdcmp_size = 0;
}

int32_t
#ifdef __STDC__
ucdecomp(uint32_t code, uint32_t *num, uint32_t **decomp)
#else
ucdecomp(code, num, decomp)
uint32_t code, *num, **decomp;
#endif
{
    int32_t l, r, m;

    l = 0;
    r = _ucdcmp_nodes[_ucdcmp_size] - 1;

    while (l <= r) {
        /*
         * Determine a "mid" point and adjust to make sure the mid point is at
         * the beginning of a code+offset pair.
         */
        m = (l + r) >> 1;
        m -= (m & 1);
        if (code > _ucdcmp_nodes[m])
          l = m + 2;
        else if (code < _ucdcmp_nodes[m])
          r = m - 2;
        else if (code == _ucdcmp_nodes[m]) {
            *num = _ucdcmp_nodes[m + 3] - _ucdcmp_nodes[m + 1];
            *decomp = &_ucdcmp_decomp[_ucdcmp_nodes[m + 1]];
            return 1;
        }
    }
    return 0;
}

int32_t
#ifdef __STDC__
ucdecomp_hangul(uint32_t code, uint32_t *num, uint32_t decomp[])
#else
ucdecomp_hangul(code, num, decomp)
uint32_t code, *num, decomp[];
#endif
{
    if (!ucishangul(code))
      return 0;

    code -= 0xac00;
    decomp[0] = 0x1100 + (uint32_t) (code / 588);
    decomp[1] = 0x1161 + (uint32_t) ((code % 588) / 28);
    decomp[2] = 0x11a7 + (uint32_t) (code % 28);
    *num = (decomp[2] != 0x11a7) ? 3 : 2;

    return 1;
}

int32_t
#ifdef __STDC__
uccanondecomp(const uint32_t *in, int32_t inlen,
              uint32_t **out, int32_t *outlen)
#else
uccanondecomp(in, inlen, out, outlen)
const uint32_t *in;
int32_t inlen;
uint32_t **out;
int32_t *outlen;
#endif
{
    int32_t i, j, k, l, size;
    uint32_t num, klass, *decomp, hangdecomp[3];

    size = inlen;
    *out = (uint32_t *) malloc(size * sizeof(**out));
    if (*out == NULL)
        return *outlen = -1;

    i = 0;
    for (j = 0; j < inlen; j++) {
        if (ucdecomp(in[j], &num, &decomp)) {
            if (size - i < num) {
                size = inlen + i - j + num - 1;
                *out = (uint32_t *) realloc(*out, size * sizeof(**out));
                if (*out == NULL)
                    return *outlen = -1;
            }
            for (k = 0; k < num; k++) {
                klass = uccombining_class(decomp[k]);
                if (klass == 0) {
                    (*out)[i] = decomp[k];
                } else {
                    for (l = i; l > 0; l--)
                        if (klass >= uccombining_class((*out)[l-1]))
                            break;
                    memmove(*out + l + 1, *out + l, (i - l) * sizeof(**out));
                    (*out)[l] = decomp[k];
                }
                i++;
            }
        } else if (ucdecomp_hangul(in[j], &num, hangdecomp)) {
            if (size - i < num) {
                size = inlen + i - j + num - 1;
                *out = (uint32_t *) realloc(*out, size * sizeof(**out));
                if (*out == NULL)
                    return *outlen = -1;
            }
            for (k = 0; k < num; k++) {
                (*out)[i] = hangdecomp[k];
                i++;
            }
        } else {
            if (size - i < 1) {
                size = inlen + i - j;
                *out = (uint32_t *) realloc(*out, size * sizeof(**out));
                if (*out == NULL)
                    return *outlen = -1;
            }
            klass = uccombining_class(in[j]);
            if (klass == 0) {
                (*out)[i] = in[j];
            } else {
                for (l = i; l > 0; l--)
                    if (klass >= uccombining_class((*out)[l-1]))
                        break;
                memmove(*out + l + 1, *out + l, (i - l) * sizeof(**out));
                (*out)[l] = in[j];
            }
            i++;
        }
    }
    return *outlen = i;
}

/**************************************************************************
 *
 * Support for combining classes.
 *
 **************************************************************************/

static uint32_t  _uccmcl_size;
static uint32_t *_uccmcl_nodes;

/*
 * Return -1 on error, 0 if okay
 */
static int32_t
#ifdef __STDC__
_uccmcl_load(char *paths, int32_t reload)
#else
_uccmcl_load(paths, reload)
char *paths;
int32_t reload;
#endif
{
    FILE *in;
    uint32_t i;
    _ucheader_t hdr;

    if (_uccmcl_size > 0) {
        if (!reload)
            /*
             * The combining classes have already been loaded.
             */
            return 0;

        free((char *) _uccmcl_nodes);
        _uccmcl_size = 0;
    }

    if ((in = _ucopenfile(paths, "cmbcl.dat", "rb")) == 0)
        return -1;

    /*
     * Load the header.
     */
    fread((char *) &hdr, sizeof(_ucheader_t), 1, in);

    if (hdr.bom == 0xfffe) {
        hdr.cnt = endian_short(hdr.cnt);
        hdr.size.bytes = endian_long(hdr.size.bytes);
    }

    _uccmcl_size = hdr.cnt * 3;
    _uccmcl_nodes = (uint32_t *) malloc(hdr.size.bytes);

    /*
     * Read the combining classes in.
     */
    fread((char *) _uccmcl_nodes, sizeof(uint32_t), _uccmcl_size, in);

    /*
     * Do an endian swap if necessary.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i < _uccmcl_size; i++)
            _uccmcl_nodes[i] = endian_long(_uccmcl_nodes[i]);
    }
    return 0;
}

static int32_t
_uccmcl_load_static()
{
  uint32_t i;
  
  _uccmcl_size = sizeof(combining_class) / sizeof(uint32_t);
  _uccmcl_nodes = (uint32_t *)combining_class;

  return 0;
}


static void
#ifdef __STDC__
_uccmcl_unload(void)
#else
_uccmcl_unload()
#endif
{
    if (_uccmcl_size == 0 || _uccmcl_nodes == combining_class)
      return;

    free((char *) _uccmcl_nodes);
    _uccmcl_size = 0;
}

uint32_t
#ifdef __STDC__
uccombining_class(uint32_t code)
#else
uccombining_class(code)
uint32_t code;
#endif
{
    int32_t l, r, m;

    l = 0;
    r = _uccmcl_size - 1;

    while (l <= r) {
        m = (l + r) >> 1;
        m -= (m % 3);
        if (code > _uccmcl_nodes[m + 1])
          l = m + 3;
        else if (code < _uccmcl_nodes[m])
          r = m - 3;
        else if (code >= _uccmcl_nodes[m] && code <= _uccmcl_nodes[m + 1])
          return _uccmcl_nodes[m + 2];
    }
    return 0;
}

/**************************************************************************
 *
 * Support for numeric values.
 *
 **************************************************************************/

static uint32_t *_ucnum_nodes;
static uint32_t _ucnum_size;
static short *_ucnum_vals;

/*
 * Return -1 on error, 0 if okay
 */
static int32_t
#ifdef __STDC__
_ucnumb_load(char *paths, int32_t reload)
#else
_ucnumb_load(paths, reload)
char *paths;
int32_t reload;
#endif
{
    FILE *in;
    uint32_t size, i;
    _ucheader_t hdr;

    if (_ucnum_size > 0) {
        if (!reload)
          /*
           * The numbers have already been loaded.
           */
          return 0;

        free((char *) _ucnum_nodes);
        _ucnum_size = 0;
    }

    if ((in = _ucopenfile(paths, "num.dat", "rb")) == 0)
      return -1;

    /*
     * Load the header.
     */
    fread((char *) &hdr, sizeof(_ucheader_t), 1, in);

    if (hdr.bom == 0xfffe) {
        hdr.cnt = endian_short(hdr.cnt);
        hdr.size.bytes = endian_long(hdr.size.bytes);
    }

    _ucnum_size = hdr.cnt;
    _ucnum_nodes = (uint32_t *) malloc(hdr.size.bytes);
    _ucnum_vals = (short *) (_ucnum_nodes + _ucnum_size);

    /*
     * Read the combining classes in.
     */
    fread((char *) _ucnum_nodes, sizeof(uint8_t), hdr.size.bytes, in);

    /*
     * Do an endian swap if necessary.
     */
    if (hdr.bom == 0xfffe) {
        for (i = 0; i < _ucnum_size; i++)
          _ucnum_nodes[i] = endian_long(_ucnum_nodes[i]);

        /*
         * Determine the number of values that have to be adjusted.
         */
        size = (hdr.size.bytes -
                (_ucnum_size * (sizeof(uint32_t) << 1))) /
            sizeof(short);

        for (i = 0; i < size; i++)
          _ucnum_vals[i] = endian_short(_ucnum_vals[i]);
    }
    return 0;
}

static int32_t
_ucnumb_load_static()
{
  _ucnum_size = sizeof(number_mappings_idx) / sizeof(int32_t);
  _ucnum_nodes = (uint32_t *)number_mappings_idx;
  _ucnum_vals = (int16_t *)number_mappings;
  return 0;
}

static void
#ifdef __STDC__
_ucnumb_unload(void)
#else
_ucnumb_unload()
#endif
{
    if (_ucnum_size == 0 || _ucnum_nodes == number_mappings_idx)
      return;

    free((char *) _ucnum_nodes);
    _ucnum_size = 0;
}

int32_t
#ifdef __STDC__
ucnumber_lookup(uint32_t code, struct ucnumber *num)
#else
ucnumber_lookup(code, num)
uint32_t code;
struct ucnumber *num;
#endif
{
    int32_t l, r, m;
    short *vp;

    l = 0;
    r = _ucnum_size - 1;
    while (l <= r) {
        /*
         * Determine a "mid" point and adjust to make sure the mid point is at
         * the beginning of a code+offset pair.
         */
        m = (l + r) >> 1;
        m -= (m & 1);
        if (code > _ucnum_nodes[m])
          l = m + 2;
        else if (code < _ucnum_nodes[m])
          r = m - 2;
        else {
            vp = _ucnum_vals + _ucnum_nodes[m + 1];
            num->numerator = (int32_t) *vp++;
            num->denominator = (int32_t) *vp;
            return 1;
        }
    }
    return 0;
}

int32_t
#ifdef __STDC__
ucdigit_lookup(uint32_t code, int32_t *digit)
#else
ucdigit_lookup(code, digit)
uint32_t code;
int32_t *digit;
#endif
{
    int32_t l, r, m;
    short *vp;

    l = 0;
    r = _ucnum_size - 1;
    while (l <= r) {
        /*
         * Determine a "mid" point and adjust to make sure the mid point is at
         * the beginning of a code+offset pair.
         */
        m = (l + r) >> 1;
        m -= (m & 1);
        if (code > _ucnum_nodes[m])
          l = m + 2;
        else if (code < _ucnum_nodes[m])
          r = m - 2;
        else {
            vp = _ucnum_vals + _ucnum_nodes[m + 1];
            if (*vp == *(vp + 1)) {
              *digit = *vp;
              return 1;
            }
            return 0;
        }
    }
    return 0;
}

struct ucnumber
#ifdef __STDC__
ucgetnumber(uint32_t code)
#else
ucgetnumber(code)
uint32_t code;
#endif
{
    struct ucnumber num;

    /*
     * Initialize with some arbitrary value, because the caller simply cannot
     * tell for sure if the code is a number without calling the ucisnumber()
     * macro before calling this function.
     */
    num.numerator = num.denominator = -111;

    (void) ucnumber_lookup(code, &num);

    return num;
}

int32_t
#ifdef __STDC__
ucgetdigit(uint32_t code)
#else
ucgetdigit(code)
uint32_t code;
#endif
{
    int32_t dig;

    /*
     * Initialize with some arbitrary value, because the caller simply cannot
     * tell for sure if the code is a number without calling the ucisdigit()
     * macro before calling this function.
     */
    dig = -111;

    (void) ucdigit_lookup(code, &dig);

    return dig;
}

/**************************************************************************
 *
 * Setup and cleanup routines.
 *
 **************************************************************************/

/*
 * Return 0 if okay, negative on error
 */
int32_t
#ifdef __STDC__
ucdata_load(char *paths, int32_t masks)
#else
ucdata_load(paths, masks)
char *paths;
int32_t masks;
#endif
{
    int32_t error = 0;

    if (masks & UCDATA_CTYPE)
      error |= _ucprop_load(paths, 0) < 0 ? UCDATA_CTYPE : 0;
    if (masks & UCDATA_CASE)
      error |= _uccase_load(paths, 0) < 0 ? UCDATA_CASE : 0;
    if (masks & UCDATA_DECOMP)
      error |= _ucdcmp_load(paths, 0) < 0 ? UCDATA_DECOMP : 0;
    if (masks & UCDATA_CMBCL)
      error |= _uccmcl_load(paths, 0) < 0 ? UCDATA_CMBCL : 0;
    if (masks & UCDATA_NUM)
      error |= _ucnumb_load(paths, 0) < 0 ? UCDATA_NUM : 0;
    if (masks & UCDATA_COMP)
      error |= _uccomp_load(paths, 0) < 0 ? UCDATA_COMP : 0;

    return -error;
}

void
#ifdef __STDC__
ucdata_unload(int32_t masks)
#else
ucdata_unload(masks)
int32_t masks;
#endif
{
    if (masks & UCDATA_CTYPE)
      _ucprop_unload();
    if (masks & UCDATA_CASE)
      _uccase_unload();
    if (masks & UCDATA_DECOMP)
      _ucdcmp_unload();
    if (masks & UCDATA_CMBCL)
      _uccmcl_unload();
    if (masks & UCDATA_NUM)
      _ucnumb_unload();
    if (masks & UCDATA_COMP)
      _uccomp_unload();
}

/*
 * Return 0 if okay, negative on error
 */
int32_t
#ifdef __STDC__
ucdata_reload(char *paths, int32_t masks)
#else
ucdata_reload(paths, masks)
char *paths;
int32_t masks;
#endif
{
    int32_t error = 0;

    if (masks & UCDATA_CTYPE)
        error |= _ucprop_load(paths, 1) < 0 ? UCDATA_CTYPE : 0;
    if (masks & UCDATA_CASE)
        error |= _uccase_load(paths, 1) < 0 ? UCDATA_CASE : 0;
    if (masks & UCDATA_DECOMP)
        error |= _ucdcmp_load(paths, 1) < 0 ? UCDATA_DECOMP : 0;
    if (masks & UCDATA_CMBCL)
        error |= _uccmcl_load(paths, 1) < 0 ? UCDATA_CMBCL : 0;
    if (masks & UCDATA_NUM)
        error |= _ucnumb_load(paths, 1) < 0 ? UCDATA_NUM : 0;
    if (masks & UCDATA_COMP)
        error |= _uccomp_load(paths, 1) < 0 ? UCDATA_COMP : 0;

    return -error;
}

void ucdata_init_static()
{
  _ucprop_load_static();
  _uccase_load_static();
  _uccomp_load_static();
  _ucdcmp_load_static();
  _uccmcl_load_static();
  _ucnumb_load_static();
}

#ifdef TEST

void
#ifdef __STDC__
main(void)
#else
main()
#endif
{
  int32_t dig;
  uint32_t res;
  uint32_t i, lo, *dec;
  struct ucnumber num;
  
  ucdata_setup(".");
  _ucprop_load_static();
  _uccase_load_static();
  _uccomp_load_static();
  _ucdcmp_load_static();
  _uccmcl_load_static();
  _ucnumb_load_static();


  
  
  if (ucisweak(0x30))
    printf("WEAK\n");
  else
    printf("NOT WEAK\n");
  
  printf("LOWER 0x%04lX\n", uctolower(0xff3a));
  printf("UPPER 0x%04lX\n", uctoupper(0xff5a));
  
  if (ucisalpha(0x1d5))
    printf("ALPHA\n");
  else
    printf("NOT ALPHA\n");
  
  if (ucisupper(0x1d5)) {
    printf("UPPER\n");
    lo = uctolower(0x1d5);
    printf("0x%04lx\n", lo);
    lo = uctotitle(0x1d5);
    printf("0x%04lx\n", lo);
  } else
    printf("NOT UPPER\n");
  
  if (ucistitle(0x1d5))
    printf("TITLE\n");
  else
    printf("NOT TITLE\n");
  
  if (uciscomposite(0x1d5))
    printf("COMPOSITE\n");
  else
    printf("NOT COMPOSITE\n");
  
  if (ucdecomp(0x1d5, &lo, &dec)) {
    for (i = 0; i < lo; i++)
      printf("0x%04lx ", dec[i]);
    putchar('\n');
  }
  
  res = dec[0];
  for (i = 0; i < lo - 1; i++)
  {
    if (uccomp(res, dec[i + 1], &res))
    {
      printf("pass %d composed to 0x%04lx\n", i, res);
    }
    else {
      printf("error on pass %d\n", i);
    }
    
    {
      uint32_t* str = _T("é");
      int32_t i = 0;
      uint32_t* res = NULL;
      uint32_t len = wcslen(str);
      uint32_t reslen = 0;
      
      printf("Original: '%s' (%d)\n", str, len);
      for (i = 0; i < len; i++)
        printf("0x%04x ", str[i]);
      printf("\n");
      
      uccanondecomp(str, len, &res, &reslen);
      printf("Result: '%s' (%d)\n", res, reslen);
      for (i = 0; i < reslen; i++)
        printf("0x%04x ", res[i]);
      printf("\n");
      
      reslen = uccanoncomp(res, reslen);
      res[reslen] = 0;
      printf("Back to the original: '%s' (%d)\n", res, reslen);
      for (i = 0; i < reslen; i++)
        printf("0x%04x ", res[i]);
      printf("\n");
    }
    
    if ((lo = uccombining_class(0x41)) != 0)
      printf("0x41 CCL %ld\n", lo);
    
    if (ucisxdigit(0xfeff))
      printf("0xFEFF HEX DIGIT\n");
    else
      printf("0xFEFF NOT HEX DIGIT\n");
    
    if (ucisdefined(0x10000))
      printf("0x10000 DEFINED\n");
    else
      printf("0x10000 NOT DEFINED\n");
    
    if (ucnumber_lookup(0x30, &num)) {
      if (num.numerator != num.denominator)
        printf("UCNUMBER: 0x30 = %d/%d\n", num.numerator, num.denominator);
      else
        printf("UCNUMBER: 0x30 = %d\n", num.numerator);
    } else
      printf("UCNUMBER: 0x30 NOT A NUMBER\n");
    
    if (ucnumber_lookup(0xbc, &num)) {
      if (num.numerator != num.denominator)
        printf("UCNUMBER: 0xbc = %d/%d\n", num.numerator, num.denominator);
      else
        printf("UCNUMBER: 0xbc = %d\n", num.numerator);
    } else
      printf("UCNUMBER: 0xbc NOT A NUMBER\n");
    
    
    if (ucnumber_lookup(0xff19, &num)) {
      if (num.numerator != num.denominator)
        printf("UCNUMBER: 0xff19 = %d/%d\n", num.numerator, num.denominator);
      else
        printf("UCNUMBER: 0xff19 = %d\n", num.numerator);
    } else
      printf("UCNUMBER: 0xff19 NOT A NUMBER\n");
    
    if (ucnumber_lookup(0x4e00, &num)) {
      if (num.numerator != num.denominator)
        printf("UCNUMBER: 0x4e00 = %d/%d\n", num.numerator, num.denominator);
      else
        printf("UCNUMBER: 0x4e00 = %d\n", num.numerator);
    } else
      printf("UCNUMBER: 0x4e00 NOT A NUMBER\n");
    
    if (ucdigit_lookup(0x06f9, &dig))
      printf("UCDIGIT: 0x6f9 = %d\n", dig);
    else
      printf("UCDIGIT: 0x6f9 NOT A NUMBER\n");
    
    dig = ucgetdigit(0x0969);
    printf("UCGETDIGIT: 0x969 = %d\n", dig);
    
    num = ucgetnumber(0x30);
    if (num.numerator != num.denominator)
      printf("UCGETNUMBER: 0x30 = %d/%d\n", num.numerator, num.denominator);
    else
      printf("UCGETNUMBER: 0x30 = %d\n", num.numerator);
    
    num = ucgetnumber(0xbc);
    if (num.numerator != num.denominator)
      printf("UCGETNUMBER: 0xbc = %d/%d\n", num.numerator, num.denominator);
    else
      printf("UCGETNUMBER: 0xbc = %d\n", num.numerator);
    
    num = ucgetnumber(0xff19);
    if (num.numerator != num.denominator)
      printf("UCGETNUMBER: 0xff19 = %d/%d\n", num.numerator, num.denominator);
    else
      printf("UCGETNUMBER: 0xff19 = %d\n", num.numerator);
    
    ucdata_cleanup();
    exit(0);
  }
}

/* Expected Result:
 WEAK
 LOWER 0xFF5A
 UPPER 0xFF3A
 ALPHA
 UPPER
 0x01d6
 0x01d5
 NOT TITLE
 COMPOSITE
 0x0055 0x0308 0x0304 
 0xFEFF NOT HEX DIGIT
 0x10000 DEFINED
 UCNUMBER: 0x30 = 0
 UCNUMBER: 0xbc = 1/4
 UCNUMBER: 0xff19 = 9
 UCNUMBER: 0x4e00 NOT A NUMBER
 UCDIGIT: 0x6f9 = 9
 UCGETDIGIT: 0x969 = 3
 UCGETNUMBER: 0x30 = 0
 UCGETNUMBER: 0xbc = 1/4
 UCGETNUMBER: 0xff19 = 9
*/ 

#endif /* TEST */
