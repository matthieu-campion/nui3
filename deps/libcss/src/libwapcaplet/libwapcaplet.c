/* libwapcaplet.c
 *
 * String internment and management tools.
 *
 * Copyright 2009 The NetSurf Browser Project.
 *		  Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#include <string.h>
#include <assert.h>

#include "libwapcaplet/libwapcaplet.h"

#ifndef UNUSED
#define UNUSED(x) ((x) = (x))
#endif

typedef uint32_t lwc_refcounter;

static inline lwc_hash
lwc_calculate_hash(const char *str, size_t len)
{
	lwc_hash z = 0x811c9dc5;
	

	while (len > 0) {
		z *= 0x01000193;
		z ^= *str++;
                len--;
	}

	return z;
}

struct lwc_string_s {
        lwc_string **	prevptr;
        lwc_string *	next;
        size_t		len;
        lwc_hash	hash;
        lwc_refcounter	refcnt;
        lwc_string *	insensitive;
};

#define STR_OF(str) ((char *)(str + 1))
#define CSTR_OF(str) ((const char *)(str + 1))

#define NR_BUCKETS_DEFAULT	(4091)

typedef struct lwc_context_s {
        lwc_allocator_fn	alloc;
        void *			alloc_pw;
        lwc_string **		buckets;
        lwc_hash		bucketcount;
} lwc_context;

static lwc_context *ctx = NULL;

#define LWC_ALLOC(s) ctx->alloc(NULL, s, ctx->alloc_pw)
#define LWC_FREE(p) ctx->alloc(p, 0, ctx->alloc_pw)

typedef lwc_hash (*lwc_hasher)(const char *, size_t);
typedef int (*lwc_strncmp)(const char *, const char *, size_t);
typedef void (*lwc_memcpy)(char *, const char *, size_t);

lwc_error
lwc_initialise(lwc_allocator_fn alloc, void *pw, lwc_hash buckets)
{
        assert(alloc);
        
        if (ctx != NULL)
                return lwc_error_initialised;
        
        ctx = alloc(NULL, sizeof(lwc_context), pw);
        
        if (ctx == NULL)
                return lwc_error_oom;
        
        memset(ctx, 0, sizeof(lwc_context));
        
        ctx->bucketcount = (buckets > 0) ? buckets : NR_BUCKETS_DEFAULT;
        ctx->alloc = alloc;
        ctx->alloc_pw = pw;
        ctx->buckets = alloc(NULL, sizeof(lwc_string *) * ctx->bucketcount, pw);
        
        if (ctx->buckets == NULL) {
                alloc(ctx, 0, pw);
                return lwc_error_oom;
        }
        
        memset(ctx->buckets, 0, sizeof(lwc_string *) * ctx->bucketcount);
        
        return lwc_error_ok;
}

static lwc_error
__lwc_intern(const char *s, size_t slen,
             lwc_string **ret,
             lwc_hasher hasher,
             lwc_strncmp compare,
             lwc_memcpy copy)
{
        lwc_hash h;
        lwc_hash bucket;
        lwc_string *str;
        
        assert((s != NULL) || (slen == 0));
        assert(ret);
        
        h = hasher(s, slen);
        bucket = h % ctx->bucketcount;
        str = ctx->buckets[bucket];
        
        while (str != NULL) {
                if ((str->hash == h) && (str->len == slen)) {
                        if (compare(CSTR_OF(str), s, slen) == 0) {
                                str->refcnt++;
                                *ret = str;
                                return lwc_error_ok;
                        }
                }
                str = str->next;
        }
        
        /* Add one for the additional NUL. */
        *ret = str = LWC_ALLOC(sizeof(lwc_string) + slen + 1);
        
        if (str == NULL)
                return lwc_error_oom;
        
        str->prevptr = &(ctx->buckets[bucket]);
        str->next = ctx->buckets[bucket];
        if (str->next != NULL)
                str->next->prevptr = &(str->next);
        ctx->buckets[bucket] = str;

        str->len = slen;
        str->hash = h;
        str->refcnt = 1;
        str->insensitive = NULL;
        
        copy(STR_OF(str), s, slen);

        /* Guarantee NUL termination */
        STR_OF(str)[slen] = '\0';
        
        return lwc_error_ok;
}

lwc_error
lwc_intern_string(const char *s, size_t slen,
                  lwc_string **ret)
{
        return __lwc_intern(s, slen, ret,
                            lwc_calculate_hash,
                            strncmp, (lwc_memcpy)memcpy);
}

lwc_error
lwc_intern_substring(lwc_string *str,
                     size_t ssoffset, size_t sslen,
                     lwc_string **ret)
{
        assert(str);
        assert(ret);
        
        if (ssoffset >= str->len)
                return lwc_error_range;
        if ((ssoffset + sslen) > str->len)
                return lwc_error_range;
        
        return lwc_intern_string(CSTR_OF(str) + ssoffset, sslen, ret);
}

lwc_string *
lwc_string_ref(lwc_string *str)
{
        assert(str);
        
        str->refcnt++;
        
        return str;
}

void
lwc_string_unref(lwc_string *str)
{
        assert(str);
        
        if (--(str->refcnt) > 1)
                return;
        
        if ((str->refcnt == 1) && (str->insensitive != str))
                return;
        
        *(str->prevptr) = str->next;
        
        if (str->next != NULL)
                str->next->prevptr = str->prevptr;

        if (str->insensitive != NULL && str->refcnt == 0)
                lwc_string_unref(str->insensitive);

#ifndef NDEBUG
        memset(str, 0xA5, sizeof(*str) + str->len);
#endif
        
        LWC_FREE(str);
}

/**** Shonky caseless bits ****/

static inline char
dolower(const char c)
{
        if (c >= 'A' && c <= 'Z')
                return c + 'a' - 'A';
        return c;
}

static inline lwc_hash
lwc_calculate_lcase_hash(const char *str, size_t len)
{
	lwc_hash z = 0x811c9dc5;
	

	while (len > 0) {
		z *= 0x01000193;
		z ^= dolower(*str++);
                len--;
	}

	return z;
}

static int
lwc_lcase_strncmp(const char *s1, const char *s2, size_t n)
{
        while (n--) {
                if (*s1++ != dolower(*s2++))
                        /** @todo Test this somehow? */
                        return 1;
        }
        return 0;
}

static void
lwc_lcase_memcpy(char *target, const char *source, size_t n)
{
        while (n--) {
                *target++ = dolower(*source++);
        }
}

static lwc_error
lwc_intern_caseless_string(lwc_string *str)
{
        assert(str);
        assert(str->insensitive == NULL);
        
        return __lwc_intern(CSTR_OF(str),
                            str->len, &(str->insensitive),
                            lwc_calculate_lcase_hash,
                            lwc_lcase_strncmp,
                            lwc_lcase_memcpy);
}

lwc_error
lwc_string_caseless_isequal(lwc_string *str1,
                            lwc_string *str2,
                            bool *ret)
{
        lwc_error err;
        assert(str1);
        assert(str2);
        
        if (str1->insensitive == NULL) {
                err = lwc_intern_caseless_string(str1);
                if (err != lwc_error_ok)
                        return err;
        }
        if (str2->insensitive == NULL) {
                err = lwc_intern_caseless_string(str2);
                if (err != lwc_error_ok)
                        return err;
        }
        
        *ret = (str1->insensitive == str2->insensitive);
        return lwc_error_ok;
}

/**** Simple accessors ****/

const char *
lwc_string_data(lwc_string *str)
{
        assert(str);
        
        return CSTR_OF(str);
}

size_t
lwc_string_length(lwc_string *str)
{
        assert(str);
        
        return str->len;
}

uint32_t
lwc_string_hash_value(lwc_string *str)
{
	assert(str);

	return str->hash;
}

/**** Iteration ****/

void
lwc_iterate_strings(lwc_iteration_callback_fn cb, void *pw)
{
        lwc_hash n;
        lwc_string *str;
        
        for (n = 0; n < ctx->bucketcount; ++n) {
                for (str = ctx->buckets[n]; str != NULL; str = str->next)
                        cb(str, pw);
        }
}
