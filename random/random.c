/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This file is part of the <FTMGS> Library                                   */
/*                                                                            */
/* This library is free software: you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation, either               */
/* version 2.1 of the License, or (at your option) any later                  */
/* version.                                                                   */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               */
/* GNU Lesser General Public License for more details.                        */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General                  */
/* Public License along with this library. If not, see                        */
/* <http://www.gnu.org/licenses/>.                                            */
/*----------------------------------------------------------------------------*/
#include "random.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stddef.h>

#include "sha.h"
#include "system_rnd.h"
#include "system_endian.h"

/*#include "system_mutex.h"*/

/*----------------------------------------------------------------------------*/
#ifndef UNUSED__
#ifdef __GNUC__
#define UNUSED__ __attribute__ ((unused))
#else
#define UNUSED__
#endif
#endif
/*----------------------------------------------------------------------------*/
#define STATIC_ASSERT(Expr__,Msg__) \
    extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
/*----------------------------------------------------------------------------*/
STATIC_ASSERT(TRUE_ENTROPY == 0, Bad_entropy_definition);
/*----------------------------------------------------------------------------*/
/* 
 * NISTSP80090. 10.1.1.1. pg: 35
 */
enum {
	RANDOM_CTX_SEEDLEN = 55
};
struct rndctx_t {
	char v[RANDOM_CTX_SEEDLEN];		/* zero */
	char c[RANDOM_CTX_SEEDLEN];		/* zero */
	unsigned counter;
	/*MUTEX_VAR(mutex)*/
};
/*----------------------------------------------------------------------------*/
/* Menezes HAC, Algorithm 14.7, pg 594 */
/* z may be equal to a or b or both */
/* big-endian representation */
UNUSED__
static void add_mod2k(void* z, unsigned zlen,
					  const void* a, unsigned alen,
					  const void* b, unsigned blen)
{
	unsigned carry, aux;
	unsigned char* pz;
	const unsigned char* pa;
	const unsigned char* pb;
	const void* x;
	if (zlen > 0) {
		if (alen < blen) {
			x = a;
			a = b;
			b = x;
			aux = alen;
			alen = blen;
			blen = aux;
		}
		pa = ((const unsigned char*)a)+alen;
		pb = ((const unsigned char*)b)+blen;
		pz = ((unsigned char*)z)+zlen;
		if (zlen < alen) {
			a = ((const unsigned char*)a)+(alen - zlen);
			alen = zlen;
			if (zlen < blen) {
				b = ((const unsigned char*)b)+(blen - zlen);
				blen = zlen;
			}
		}
		assert((blen <= alen)&&(alen <= zlen));
		carry = 0;
		while ((const void*)pb > b) {
			aux = (unsigned)*--pa + (unsigned)*--pb + carry;
			carry = !(aux < 0x100U);
			*--pz = (unsigned char)aux;
		}
		while ((const void*)pa > a) {
			aux = (unsigned)*--pa + carry;
			carry = !(aux < 0x100U);
			*--pz = (unsigned char)aux;
		}
		if ((void*)pz > z) {
			*--pz = (unsigned char)carry;
			while ((void*)pz > z) {
				*--pz = 0;
			}
		}
	}
}
/*----------------------------------------------------------------------------*/
static void inc_mod2k(void* a, unsigned alen,
					  const void* b, unsigned blen)
{
	unsigned carry, aux;
	unsigned char* pa;
	const unsigned char* pb;
	if (alen > 0) {
		pa = ((unsigned char*)a)+alen;
		pb = ((const unsigned char*)b)+blen;
		if (alen < blen) {
			b = ((const unsigned char*)b)+(blen - alen);
			blen = alen;
		}
		carry = 0;
		while ((const void*)pb > b) {
			aux = (unsigned)*--pa + (unsigned)*--pb + carry;
			carry = !(aux < 0x100U);
			*pa = (unsigned char)aux;
		}
		while (((void*)pa > a)&&(carry != 0)) {
			aux = (unsigned)*--pa + carry;
			carry = !(aux < 0x100U);
			*pa = (unsigned char)aux;
		}
	}
}
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* 
 * Based on NIST-SP-800-90
 */
/*-----------------------------------------------------------------*/
/*static*/ unsigned  memcat(void* dst, unsigned* len, unsigned maxlen,
						const void* org, unsigned n)
{
	assert(*len <= maxlen);
	assert(((const char*)org+n <= (const char*)dst)
		   || ((const char*)org >= (const char*)dst+*len));
	if (n > maxlen - *len) {
		n = maxlen - *len;
	}
	memcpy((char*)dst+*len, org, n);
	*len += n;
	return n;
}
/*-----------------------------------------------------------------*/
/* 
 * Random Numbers
 */
/*-----------------------------------------------------------------*/
/* 
 * Highest Security strengths for Random Number Generation (BITS)
 *		(table 3 pg 64 NIST SP 800-57)
 *		(table 2 pg 43 NIST SP 800-90)
 *
 * 				Sha1	Sha224	| Sha256 |	Sha384	Sha512
 *				-------------------------------------------
 *	Outlen		160		224		| 256	 |	384		512
 *	HSStrength	128		192		| 256	 |	256		256
 *	MinEntropy	128		192		| 256	 |	256		256
 *	SeedLen		440		440		| 440	 |	888		888
 */
/*----------------------------------*/
enum {
	/*RANDOM_CTX_SEEDLEN	= 55,*/
	RANDOM_CTX_ENTROPY	= 32,
	RANDOM_CTX_NONCE	= (RANDOM_CTX_ENTROPY/2),
	RANDOM_CTX_HASH		= SHA256
};
/*----------------------------------*/
STATIC_ASSERT(RANDOM_CTX_ENTROPY + RANDOM_CTX_NONCE < RANDOM_CTX_SEEDLEN,
			  Internal_random_context_bits);
/*-----------------------------------------------------------------*/
/* 
 * NISTSP80090. 10.4.1. pg: 65
 */
static unsigned hash_df(SHAversion which_sha,
						void* dst, unsigned maxlen,
						const void* input1, unsigned ilen1,
						const void* input2, unsigned ilen2,
						const void* input3, unsigned ilen3)
{
	unsigned olen = 0;
	unsigned len = ((maxlen + (unsigned)USHAHashSize(which_sha) - 1)
					/ (unsigned)USHAHashSize(which_sha)); /* round-up */
	uint8_t counter = 1;
	USHAContext sha_context;
	uint8_t msg_digest[USHAMaxHashSize];
	unsigned i;
	for (i = 1; i <= len; ++i) {
		USHAReset(&sha_context, which_sha);
		USHAInput(&sha_context, (const uint8_t*)&counter, sizeof(counter));
		USHAInput(&sha_context, (const uint8_t*)&maxlen, sizeof(maxlen));
		USHAInput(&sha_context, (const uint8_t*)input1, ilen1);
		USHAInput(&sha_context, (const uint8_t*)input2, ilen2);
		USHAInput(&sha_context, (const uint8_t*)input3, ilen3);
		USHAResult(&sha_context, msg_digest);
		memcat(dst, &olen, maxlen,
			   msg_digest, (unsigned)USHAHashSize(which_sha));
		++counter;
	}
	assert(olen == maxlen);
	return olen;
}
/*-----------------------------------------------------------------*/
/* 
 * NISTSP80090. 10.1.1.4. pg: 39
 */
static unsigned hash_gen(SHAversion which_sha,
						 void* output, unsigned maxlen,
						 const void* ctx_v, unsigned vlen)
{
	uint8_t one = 1;
	unsigned m = ((maxlen + (unsigned)USHAHashSize(which_sha) - 1)
				  / (unsigned)USHAHashSize(which_sha)); /* round-up */
	USHAContext sha_context;
	uint8_t msg_digest[USHAMaxHashSize];
	char data[RANDOM_CTX_SEEDLEN];
	unsigned i;
	unsigned olen = 0;
	if (vlen < sizeof(data)) {
		memset(data, 0, (sizeof(data)-vlen));
		memcpy(data+(sizeof(data)-vlen), ctx_v, vlen);
	} else {
		memcpy(data, (const char*)ctx_v+(vlen-sizeof(data)), sizeof(data));
	}
	for (i = 1; i <= m; ++i) {
		USHAReset(&sha_context, which_sha);
		USHAInput(&sha_context, (const uint8_t*)data, sizeof(data));
		USHAResult(&sha_context, msg_digest);
		memcat(output, &olen, maxlen,
			   msg_digest, (unsigned)USHAHashSize(which_sha));
		inc_mod2k(data, sizeof(data), &one, sizeof(one));
	}
	assert(olen == maxlen);
	return olen;
}
/*-----------------------------------------------------------------*/
static unsigned hash_tag(SHAversion which_sha,
						 uint8_t msg_digest[USHAMaxHashSize],
						 const void* input_data1, unsigned idtlen1,
						 const void* input_data2, unsigned idtlen2)
{
	USHAContext sha_context;
	USHAReset(&sha_context, which_sha);
	USHAInput(&sha_context, (const uint8_t*)input_data1, idtlen1);
	USHAInput(&sha_context, (const uint8_t*)input_data2, idtlen2);
	USHAResult(&sha_context, msg_digest);
	return (unsigned)USHAHashSize(which_sha);
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/* 
 * NISTSP80090. 10.1.1.2. pg: 37
 */
unsigned random_seed(rnd_ctx_t* rnd_ctx, unsigned entropy_src)
{
	uint8_t tag = 0x00;
	unsigned le = 0;
	unsigned ln = 0;
	assert(rnd_ctx != NULL);
	/*BEGIN_MUTEX(&rnd_ctx->mutex);*/
	if (entropy_src != NO_ENTROPY) {
		le = entropy(rnd_ctx->c, RANDOM_CTX_ENTROPY, entropy_src);
		ln = nonce(rnd_ctx->c+le, RANDOM_CTX_NONCE);
	}
	hash_df(RANDOM_CTX_HASH, rnd_ctx->v, sizeof(rnd_ctx->v),
			rnd_ctx->c, le+ln, NULL, 0, NULL, 0);
	hash_df(RANDOM_CTX_HASH, rnd_ctx->c, sizeof(rnd_ctx->c),
			&tag, sizeof(tag), rnd_ctx->v, sizeof(rnd_ctx->v), NULL, 0);
	rnd_ctx->counter = 1;
	/*END_MUTEX(&rnd_ctx->mutex);*/
	/*------------------------------*/
	return (le+ln);
}
/*----------------------------------*/
/* 
 * NISTSP80090. 10.1.1.3. pg: 37
 */
unsigned random_reseed(rnd_ctx_t* rnd_ctx, unsigned entropy_src)
{
	char aux[RANDOM_CTX_SEEDLEN];
	uint8_t tag0 = 0x00;
	uint8_t tag1 = 0x01;
	unsigned le = 0;
	unsigned ln = 0;
	assert(rnd_ctx != NULL);
	/*BEGIN_MUTEX(&rnd_ctx->mutex);*/
	aux[0] = 0x01;
	if (entropy_src != NO_ENTROPY) {
		le = entropy(rnd_ctx->c, RANDOM_CTX_ENTROPY, entropy_src);
		ln = nonce(rnd_ctx->c+le, RANDOM_CTX_NONCE);
	}
	memcpy(aux, rnd_ctx->v, sizeof(aux));
	hash_df(RANDOM_CTX_HASH, rnd_ctx->v, sizeof(rnd_ctx->v),
			&tag1, sizeof(tag1), aux, sizeof(aux), rnd_ctx->c, le+ln);
	hash_df(RANDOM_CTX_HASH, rnd_ctx->c, sizeof(rnd_ctx->c),
			&tag0, sizeof(tag0), rnd_ctx->v, sizeof(rnd_ctx->v), NULL, 0);
	rnd_ctx->counter = 1;
	/*END_MUTEX(&rnd_ctx->mutex);*/
	/*------------------------------*/
	return (le+ln);
}
/*-----------------------------------------------------------------*/
/* 
 * NISTSP80090. 10.1.1.4. pg: 38
 */
void random_bytes(void* data, unsigned nbytes, rnd_ctx_t* rnd_ctx)
{
	uint8_t tag = 0x03;
	uint8_t msg_digest[USHAMaxHashSize];
	unsigned digest_len;
	unsigned counter = int2bigendian(rnd_ctx->counter);
	assert(nbytes <= 65535); /*NISTSP80090 table 2: 2^19/2^3*/
	assert(rnd_ctx != NULL);
	/*BEGIN_MUTEX(&rnd_ctx->mutex);*/
	hash_gen(RANDOM_CTX_HASH, data, nbytes, rnd_ctx->v, sizeof(rnd_ctx->v));
	digest_len = hash_tag(RANDOM_CTX_HASH, msg_digest,
						  &tag, sizeof(tag), rnd_ctx->v, sizeof(rnd_ctx->v));
	inc_mod2k(rnd_ctx->v, sizeof(rnd_ctx->v), msg_digest, digest_len);
	inc_mod2k(rnd_ctx->v, sizeof(rnd_ctx->v), rnd_ctx->c, sizeof(rnd_ctx->c));
	inc_mod2k(rnd_ctx->v, sizeof(rnd_ctx->v), &counter, sizeof(counter));
	++rnd_ctx->counter;
	/*END_MUTEX(&rnd_ctx->mutex);*/
}
/*-----------------------------------------------------------------*/
/* r rnd such that 0 <= r < max */
unsigned random_uint(unsigned max, rnd_ctx_t* rnd_ctx)
{
	unsigned data[2];
	random_bytes(data, sizeof(data), rnd_ctx);
	return (unsigned)((max*(double)data[0])/(1.0+UINT_MAX));
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define MUTEX_VAR_dtor(mutex_var__)	DESTROY_MUTEX(mutex_var__)
#define MUTEX_VAR_ctor(mutex_var__)	CREATE_MUTEX(mutex_var__)
#define MUTEX_VAR_asg(mutex_var_d__, mutex_var_o__)	do{DESTROY_MUTEX(mutex_var_d__);CREATE_MUTEX(mutex_var_d__);}while(0)
#define MUTEX_VAR_move(mutex_var_d__, mutex_var_o__) do{DESTROY_MUTEX(mutex_var_d__);CREATE_MUTEX(mutex_var_d__);}while(0)
/*----------------------------------------------------------------------------*/
void rndctx_t_ctor(struct rndctx_t* p);
void rndctx_t_dtor(struct rndctx_t* p);
void rndctx_t_asg(struct rndctx_t* d, const struct rndctx_t* o);
void rndctx_t_move(struct rndctx_t* d, struct rndctx_t* o);
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int rndctx_t_chk_members(const struct rndctx_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_rndctx_t {
		char v[RANDOM_CTX_SEEDLEN];		/* zero */
		char c[RANDOM_CTX_SEEDLEN];		/* zero */
		unsigned counter;
		/*MUTEX_VAR(mutex)*/
	};
	CHK_FIELD__(dummy_rndctx_t, rndctx_t, v);
	CHK_FIELD__(dummy_rndctx_t, rndctx_t, c);
	CHK_FIELD__(dummy_rndctx_t, rndctx_t, counter);
	/*CHK_FIELD__(dummy_rndctx_t, rndctx_t, mutex);*/
	CHK_SIZE__(dummy_rndctx_t, rndctx_t);
	return (code == 435758464); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void rndctx_t_ctor(struct rndctx_t* p)/*auto*/
{
	int i; (void)i;
	assert(p != NULL);
	assert(rndctx_t_chk_members(p,435758464));
	memset(p->v, 0, sizeof(p->v));
	memset(p->c, 0, sizeof(p->c));
	p->counter = 0;
	/*MUTEX_VAR_ctor(&p->mutex);*/
}
/*----------------------------------------------------------------------------*/
void rndctx_t_dtor(struct rndctx_t* p)/*auto*/
{
	int i; (void)i;
	assert(p != NULL);
	assert(rndctx_t_chk_members(p,435758464));
	/*MUTEX_VAR_dtor(&p->mutex);*/
	memset(p->c, 0, sizeof(p->c));
	memset(p->v, 0, sizeof(p->v));
	(void)p;
}
/*----------------------------------------------------------------------------*/
void rndctx_t_asg(struct rndctx_t* p, const struct rndctx_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(rndctx_t_chk_members(p,435758464));
	if (p != o) {
		int i; (void)i;
		memcpy(p->v, o->v, sizeof(p->v));
		memcpy(p->c, o->c, sizeof(p->c));
		p->counter = o->counter;
		/*MUTEX_VAR_asg(&p->mutex, &o->mutex);*/
	}
}
/*----------------------------------------------------------------------------*/
void rndctx_t_move(struct rndctx_t* p, struct rndctx_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(rndctx_t_chk_members(p,435758464));
	if (p != o) {
		int i; (void)i;
		memcpy(p->v, o->v, sizeof(p->v));
		memcpy(p->c, o->c, sizeof(p->c));
		p->counter = o->counter;
		/*MUTEX_VAR_move(&p->mutex, &o->mutex);*/
	}
}
/*----------------------------------------------------------------------------*/
struct rndctx_t* rndctx_t_new()/*auto*/
{
	struct rndctx_t* p = (struct rndctx_t*)malloc(sizeof(struct rndctx_t));
	if (p != NULL) {
		rndctx_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct rndctx_t* rndctx_t_clone(const struct rndctx_t* o)/*auto*/
{
	struct rndctx_t* p = NULL;
	if (o != NULL) {
		p = (struct rndctx_t*)malloc(sizeof(struct rndctx_t));
		if (p != NULL) {
			rndctx_t_ctor(p);
			rndctx_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void rndctx_t_delete(struct rndctx_t* p)/*auto*/
{
	if (p != NULL) {
		rndctx_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
