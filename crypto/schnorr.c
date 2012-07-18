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
#include "schnorr.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "iexport.h"
#include "generator.h"
/*----------------------------------------------------------------------------*/
void schnorr_create_keys(schnorr_pbkey_t* pk,
						 schnorr_prkey_t* sk,
						 const sphere_t* sph,
						 unsigned nu,
						 rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_5_bigint_t(p, p1, q, q1, order);
	/*--------------------------------*/
	set_syspar(&pk->sp, nu);
	/*--------------------------------*/
	bi_random_sgprime_nb(p, p1, pk->sp.nu/2, DEF_NTESTS, rnd_ctx);
	do {
		bi_random_sgprime_nb(q, q1, pk->sp.nu/2, DEF_NTESTS, rnd_ctx);
	} while ( bi_equals(p, q) );
	bi_mul(pk->n, p, q);
	/*--------------------------------*/
	bi_mul(order, p1, q1);
	qrn_generator(pk->g, pk->n, order, p1, q1, rnd_ctx);
	/*--------------------------------*/
	do {
		bi_random_sph_ek(sk->x, sph, rnd_ctx);
		/*bi_random_nb(sk->x, pk->sp.nu/2, rnd_ctx);*/
	} while (bi_iszero(sk->x));
	bi_powmod_sec(pk->y, pk->g, sk->x, pk->n);
	/*--------------------------------*/
	END_VAR_5_bigint_t(p, p1, q, q1, order);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/* 
 * HASH( msg, {bi}, {ni}, {oi} ) [same order as SOK]
 */
static void schnorr_hash(bigint_t c,
						 const void* dat_digest,
						 unsigned dat_digestlen,
						 const bigint_t b,
						 const schnorr_pbkey_t* pk)
{
	USHAContext sha_ctx;
	/*--------------------------------*/
	bi_sha_reset(&sha_ctx, pk->sp.k);
	/*--------------------------------*/
	sha_input(&sha_ctx, dat_digest, dat_digestlen);
	bi_sha_input(&sha_ctx, b);
	bi_sha_input(&sha_ctx, pk->n);
	bi_sha_input(&sha_ctx, pk->g);
	bi_sha_input(&sha_ctx, pk->y);
	/*--------------------------------*/
	bi_sha_result_len(&sha_ctx, c, pk->sp.k/CHAR_BIT);
}
/*----------------------------------------------------------------------------*/
bool_t schnorr_sign(schnorr_sign_t* sg,
				  const void* dat_digest,
				  unsigned dat_digestlen,
				  const schnorr_pbkey_t* pk,
				  const schnorr_prkey_t* sk,
				  const sphere_t* sph,
				  rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	BEG_VAR_2_bigint_t(k, r);
	/*--------------------------------*/
	DBG(bi_powmod_dbg(r, pk->g, sk->x, pk->n);)
	assert(bi_equals(r, pk->y));
	ok = bi_belongs_sph_ek(sk->x, sph);
	/*assert(bi_nbits(sk->x) <= pk->sp.nu/2);*/
	if (ok) {
		/*--------------------------------*/
		do {
			bi_random_sph(k, sph, rnd_ctx);
			/*bi_random_nb(k, pk->sp.nu/2, rnd_ctx);*/
		} while (bi_iszero(k));
		bi_powmod_sec(r, pk->g, k, pk->n);
		schnorr_hash(sg->c, dat_digest, dat_digestlen, r, pk);
		bi_mul(sg->sx, sk->x, sg->c);
		bi_add(sg->sx, sg->sx, k);
		/* bi_mod(sg->sx, sg->sx, unknown_order); */
		assert(bi_nbits(sg->sx) <= sph->log2radius+pk->sp.k);
	}
	/*--------------------------------*/
	END_VAR_2_bigint_t(k, r);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t schnorr_sign_dat(unsigned which_sha,
					  schnorr_sign_t* sg,
					  const void* dat,
					  unsigned datlen,
					  const schnorr_pbkey_t* pk,
					  const schnorr_prkey_t* sk,
					  const sphere_t* sph,
					  rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = schnorr_sign(sg, dat_digest, dat_digestlen, pk, sk, sph, rnd_ctx);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t schnorr_vrfy(const schnorr_sign_t* sg,
					const void* dat_digest,
					unsigned dat_digestlen,
					const schnorr_pbkey_t* pk,
					const sphere_t* sph)
{
	BEG_BIGINT_CHK();
	bool_t ok;
	if (bi_nbits(sg->c) > pk->sp.k) {
		DBG(fprintf(stderr,"Failure: bi_nbits(sg->c): %u out of range [%u]\n",
					bi_nbits(sg->c), pk->sp.k);)
		ok = FALSE;
	} else if (bi_nbits(sg->sx) > sph->log2radius+pk->sp.k) {
		DBG(fprintf(stderr, "Failure: bi_nbits(sg->sx): %u out of range "
					"(eps*(mu+k)+1) [%u]\n",
					bi_nbits(sg->sx),sph->log2radius);)
		ok = FALSE;
	} else {
		BEG_VAR_3_bigint_t(v, w, c1);
		bi_powmod_sec(v, pk->g, sg->sx, pk->n);
		bi_powmod_sec(w, pk->y, sg->c, pk->n);
		bi_invmod(w, w, pk->n);
		bi_mulmod(v, v, w, pk->n);
		schnorr_hash(c1, dat_digest, dat_digestlen, v, pk);
		ok = bi_equals(c1, sg->c);
		END_VAR_3_bigint_t(v, w, c1);
	}
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t schnorr_vrfy_dat(unsigned which_sha,
						const schnorr_sign_t* sg,
						const void* dat,
						unsigned datlen,
						const schnorr_pbkey_t* pk,
						const sphere_t* sph)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = schnorr_vrfy(sg, dat_digest, dat_digestlen, pk, sph);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int schnorr_pbkey_t_chk_members(const struct schnorr_pbkey_t* p, int code)/*auto*/
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
	struct dummy_schnorr_pbkey_t {
		syspar_t sp;
		bigint_t n;		/* zero */
		bigint_t g;		/* zero */
		bigint_t y;		/* zero */
	};
	CHK_FIELD__(dummy_schnorr_pbkey_t, schnorr_pbkey_t, sp);
	CHK_FIELD__(dummy_schnorr_pbkey_t, schnorr_pbkey_t, n);
	CHK_FIELD__(dummy_schnorr_pbkey_t, schnorr_pbkey_t, g);
	CHK_FIELD__(dummy_schnorr_pbkey_t, schnorr_pbkey_t, y);
	CHK_SIZE__(dummy_schnorr_pbkey_t, schnorr_pbkey_t);
	return (code == 428264136); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void schnorr_pbkey_t_ctor(struct schnorr_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(schnorr_pbkey_t_chk_members(p,428264136));
	syspar_t_ctor(&p->sp);
	bi_ctor(p->n);
	bi_ctor(p->g);
	bi_ctor(p->y);
}
/*----------------------------------------------------------------------------*/
void schnorr_pbkey_t_dtor(struct schnorr_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(schnorr_pbkey_t_chk_members(p,428264136));
	bi_clear_zero(p->y);
	bi_dtor(p->y);
	bi_clear_zero(p->g);
	bi_dtor(p->g);
	bi_clear_zero(p->n);
	bi_dtor(p->n);
	syspar_t_dtor(&p->sp);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void schnorr_pbkey_t_asg(struct schnorr_pbkey_t* p, const struct schnorr_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(schnorr_pbkey_t_chk_members(p,428264136));
	if (p != o) {
		syspar_t_asg(&p->sp, &o->sp);
		bi_asg(p->n, o->n);
		bi_asg(p->g, o->g);
		bi_asg(p->y, o->y);
	}
}
/*----------------------------------------------------------------------------*/
void schnorr_pbkey_t_move(struct schnorr_pbkey_t* p, struct schnorr_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(schnorr_pbkey_t_chk_members(p,428264136));
	if (p != o) {
		syspar_t_move(&p->sp, &o->sp);
		bi_asg_si(p->n, 0);
		bi_swap(p->n, o->n);
		bi_asg_si(p->g, 0);
		bi_swap(p->g, o->g);
		bi_asg_si(p->y, 0);
		bi_swap(p->y, o->y);
	}
}
/*----------------------------------------------------------------------------*/
struct schnorr_pbkey_t* schnorr_pbkey_t_new()/*auto*/
{
	struct schnorr_pbkey_t* p = (struct schnorr_pbkey_t*)malloc(sizeof(struct schnorr_pbkey_t));
	if (p != NULL) {
		schnorr_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct schnorr_pbkey_t* schnorr_pbkey_t_clone(const struct schnorr_pbkey_t* o)/*auto*/
{
	struct schnorr_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct schnorr_pbkey_t*)malloc(sizeof(struct schnorr_pbkey_t));
		if (p != NULL) {
			schnorr_pbkey_t_ctor(p);
			schnorr_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void schnorr_pbkey_t_delete(struct schnorr_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		schnorr_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int schnorr_prkey_t_chk_members(const struct schnorr_prkey_t* p, int code)/*auto*/
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
	struct dummy_schnorr_prkey_t {
		bigint_t x;		/* zero */
	};
	CHK_FIELD__(dummy_schnorr_prkey_t, schnorr_prkey_t, x);
	CHK_SIZE__(dummy_schnorr_prkey_t, schnorr_prkey_t);
	return (code == 451805986); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void schnorr_prkey_t_ctor(struct schnorr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(schnorr_prkey_t_chk_members(p,451805986));
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void schnorr_prkey_t_dtor(struct schnorr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(schnorr_prkey_t_chk_members(p,451805986));
	bi_clear_zero(p->x);
	bi_dtor(p->x);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void schnorr_prkey_t_asg(struct schnorr_prkey_t* p, const struct schnorr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(schnorr_prkey_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
void schnorr_prkey_t_move(struct schnorr_prkey_t* p, struct schnorr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(schnorr_prkey_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg_si(p->x, 0);
		bi_swap(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct schnorr_prkey_t* schnorr_prkey_t_new()/*auto*/
{
	struct schnorr_prkey_t* p = (struct schnorr_prkey_t*)malloc(sizeof(struct schnorr_prkey_t));
	if (p != NULL) {
		schnorr_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct schnorr_prkey_t* schnorr_prkey_t_clone(const struct schnorr_prkey_t* o)/*auto*/
{
	struct schnorr_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct schnorr_prkey_t*)malloc(sizeof(struct schnorr_prkey_t));
		if (p != NULL) {
			schnorr_prkey_t_ctor(p);
			schnorr_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void schnorr_prkey_t_delete(struct schnorr_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		schnorr_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int schnorr_sign_t_chk_members(const struct schnorr_sign_t* p, int code)/*auto*/
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
	struct dummy_schnorr_sign_t {
		bigint_t c;		/* zero */
		bigint_t sx;	/* zero */
	};
	CHK_FIELD__(dummy_schnorr_sign_t, schnorr_sign_t, c);
	CHK_FIELD__(dummy_schnorr_sign_t, schnorr_sign_t, sx);
	CHK_SIZE__(dummy_schnorr_sign_t, schnorr_sign_t);
	return (code == 476205863); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void schnorr_sign_t_ctor(struct schnorr_sign_t* p)/*auto*/
{
	assert(p != NULL);
	assert(schnorr_sign_t_chk_members(p,476205863));
	bi_ctor(p->c);
	bi_ctor(p->sx);
}
/*----------------------------------------------------------------------------*/
void schnorr_sign_t_dtor(struct schnorr_sign_t* p)/*auto*/
{
	assert(p != NULL);
	assert(schnorr_sign_t_chk_members(p,476205863));
	bi_clear_zero(p->sx);
	bi_dtor(p->sx);
	bi_clear_zero(p->c);
	bi_dtor(p->c);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void schnorr_sign_t_asg(struct schnorr_sign_t* p, const struct schnorr_sign_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(schnorr_sign_t_chk_members(p,476205863));
	if (p != o) {
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
void schnorr_sign_t_move(struct schnorr_sign_t* p, struct schnorr_sign_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(schnorr_sign_t_chk_members(p,476205863));
	if (p != o) {
		bi_asg_si(p->c, 0);
		bi_swap(p->c, o->c);
		bi_asg_si(p->sx, 0);
		bi_swap(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
struct schnorr_sign_t* schnorr_sign_t_new()/*auto*/
{
	struct schnorr_sign_t* p = (struct schnorr_sign_t*)malloc(sizeof(struct schnorr_sign_t));
	if (p != NULL) {
		schnorr_sign_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct schnorr_sign_t* schnorr_sign_t_clone(const struct schnorr_sign_t* o)/*auto*/
{
	struct schnorr_sign_t* p = NULL;
	if (o != NULL) {
		p = (struct schnorr_sign_t*)malloc(sizeof(struct schnorr_sign_t));
		if (p != NULL) {
			schnorr_sign_t_ctor(p);
			schnorr_sign_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void schnorr_sign_t_delete(struct schnorr_sign_t* p)/*auto*/
{
	if (p != NULL) {
		schnorr_sign_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
